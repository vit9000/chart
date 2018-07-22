#include "stdafx.h"
#include "CMainModel.h"

#define ERR_MSG MessageBox(parentDlg->m_hWnd, L"Допускается введение только числовых значений", L"Внимание",  MB_OK | MB_ICONINFORMATION);

bool CMainModel::undo()
{
	WriteLog = false;
	bool temp = logger.undo(*this); 
	redrawView(); 
	WriteLog = true;
	return temp;
}

bool CMainModel::redo()
{
	WriteLog = false;
	bool temp = logger.redo(*this);
	redrawView();
	WriteLog = true;
	return temp;
}

int CMainModel::getCountPatients() const
{
	return MainBridge::getInstance().countPatients();
}
//-----------------------------------------------------------------------------------------------------
const wstring& CMainModel::getContainerName(const ID& id)
{
	return chartData.getContainerUnit(id)->getName();
}
//-----------------------------------------------------------------------------------------------------
const DrugInfo& CMainModel::getDrugInfo(const ID& id)
{
	return chartData.getContainerUnit(id)->getDrugInfo();
}
//-----------------------------------------------------------------------------------------------------
ChartData* CMainModel::getCurrentPatient()
{
	return &chartData;
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::SaveAndCloseChart()
{
	if (current < 0) return;
	chartData.saveChart(logger); // сохраняем текущее состояние карты
	chartData.clear(); // очищаем данные карты
	logger.reset(); // сбрасываем команды undo и redo
	current = -1;
	NotifyEmpty();
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::setPatient(int index, const wstring& chartID)
{
	if (index >= getCountPatients())
		return;
	current = index;
	chartData.setPatient(index);
	chartData.loadChart(chartID);
	loadChartView();
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::loadChartView() // посылаем команды в представление для прорисовки структуры карты назначений
{
	vector<TableCommand_Ptr> table_commands;
	table_commands.push_back(TableCommand_Ptr(new CommandClear()));


	const auto& content = chartData.getAdministrations();
	const auto& block_names = chartData.getBlockNames();
	for (size_t i = 0; i < content.size(); i++) // блоки (секции)
	{
		const auto& block_name = block_names[i];
		const auto& containerUnits = content.at(block_name);
		table_commands.push_back(TableCommand_Ptr(new CommandAddBlock(block_name, chartData.getBlockType(block_name))));

		for (size_t j = 0; j < containerUnits.size(); j++) // строки 
		{
			const ContainerUnit_Ptr& cu_ptr = containerUnits[j];
			table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(block_name, *cu_ptr, j)));
			if (cu_ptr->isParent())
			{
				for (const auto& child : cu_ptr->getChilds()) // дочерние строки
				{
					table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(block_name, *child, j)));
				}
			}
		}
	}

	Notify(table_commands);
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::moveDrug(const ID& id, int new_pos, bool redraw)
{
	vector<TableCommand_Ptr> table_commands;
	table_commands.push_back(TableCommand_Ptr(new CommandMoveDrug(id, new_pos)));
	Notify(table_commands);

	if(redraw)
		redrawView();
}
//------------------------------------------------------------------------------------------------------
void CMainModel::updateDrugPos(const ID& id, int new_pos)
{
	auto log_command = chartData.updateDrugPos(id, new_pos);
	if (log_command && WriteLog)
		logger.push_back(log_command);

	redrawView();
}
//------------------------------------------------------------------------------------------------------
void CMainModel::addDrug(const ID& id, int adminWayCode, const DrugInfo& drugInfo)
{
	addDrug(id, adminWayCode, drugInfo, map<int, Unit>(), -1);
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::addDrug(const ID& id, int adminWayCode, const DrugInfo& drugInfo, const map<int, Unit>& units, int position)
{
	vector<TableCommand_Ptr> table_commands;
	wstring BlockName = chartData.getAdministrationsBlockName();
	auto _pair = chartData.addDrug(position, id, BlockName, MainBridge::getInstance().getAdminWayType(adminWayCode), drugInfo);
	auto& containerUnit = _pair.first;
	auto& pos = _pair.second;
	containerUnit->addUnits(units);
	table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(BlockName, *containerUnit, pos)));
	Notify(table_commands);

	if(WriteLog)
		logger.push_back(LogCommandPtr(new LogCommand_AddDrug(*containerUnit, pos)));
	
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::addChildDrug(const ID& id, const ID& host_id, const DrugInfo& drugInfo)
{
	addChildDrug(id, host_id, drugInfo, map<int, Unit>());
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::addChildDrug(const ID& id, const ID& host_id, const DrugInfo& drugInfo, const map<int, Unit>& units)
{
	auto _pair = chartData.addChildDrug(id, host_id, drugInfo);
	auto& containerUnit = _pair.first;
	auto& pos = _pair.second;
	containerUnit->addUnits(units);
	vector<TableCommand_Ptr> table_commands;
	table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(host_id.getBlockName(), *containerUnit, pos)));
	Notify(table_commands);

	if (WriteLog)
		logger.push_back(LogCommandPtr(new LogCommand_AddDrugToDrug(host_id, *containerUnit)));
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::deleteDrug(const ID& id)
{
	// сперва отправляем команду в Представление
	vector<TableCommand_Ptr> table_commands;
	table_commands.push_back(TableCommand_Ptr(new CommandDeleteContainerUnit(id)));
	Notify(table_commands);

	// затем удаляем строку
	LogCommandPtr log_command = chartData.deleteDrug(id);
	if (log_command && !log_command->isEmpty() && WriteLog)
		logger.push_back(log_command);
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::deleteChildDrug(const ID& id)
{
	// сперва отправляем команду в Представление
	vector<TableCommand_Ptr> table_commands;
	ID parentID = chartData.getContainerUnit(id)->getParentID();
	table_commands.push_back(TableCommand_Ptr(new CommandDeleteChildContainerUnit(parentID, id)));
	Notify(table_commands);

	// затем удаляем строку
	LogCommandPtr log_command = chartData.deleteDrug(id);
	if (log_command && WriteLog)
		logger.push_back(log_command);
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::addUnit(const ID& id, const Unit& new_unit, bool redraw)
{
	if (LogCommandPtr log_command = chartData.getContainerUnit(id)->addUnit(new_unit))
	{
		//записываем все в LogCommandAdministrator
		if(WriteLog)
			logger.push_back(log_command);
		//обновляем Представление
		if(redraw)
			redrawView();
	}
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::addUnits(const vector<ID>& ids, const vector<Value>& values, int start, int duration)
{
	LogCommand_Union* com  = new LogCommand_Union();
	for (size_t i = 0; i < ids.size(), i < values.size(); i++)
	{
		//addUnit(ids[i], values[i], start, duration);
		if (LogCommandPtr log_command = chartData.getContainerUnit(ids[i])->addUnit(Unit(values[i], start, duration)))
			com->add(log_command);
	}
	if (WriteLog)
		logger.push_back(LogCommandPtr(com));
	else 
		delete com;
	redrawView();
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::deleteUnit(const ID& id, int unit_number, bool redraw)
{
	if (LogCommandPtr log_command = chartData.getContainerUnit(id)->deleteUnit(unit_number))
	{
		// записываем все в LogCommandAdministrator
		if (WriteLog)
			logger.push_back(log_command);
		// обновляется представление
		if(redraw)
			redrawView();
	}
}
void CMainModel::updateUnit(const ID& id, int unit_number, const Unit& unit, bool redraw)
{
	ContainerUnit_Ptr containerUnit = chartData.getContainerUnit(id);
	if (LogCommandPtr log_command = containerUnit->updateUnit(unit_number, unit))
	{
		// записываем все в LogCommandAdministrator
		if (WriteLog)
			logger.push_back(log_command);
		// обновляется представление
		if(redraw)
			redrawView();
	}
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::updateUnits(const vector<ID>& ids, int unit_number, const vector<Unit>& units)
{
	LogCommand_Union* com = new LogCommand_Union();
	for (size_t i = 0; i < ids.size(), i < units.size(); i++)
	{
		const Unit& unit = units[i];
		if (LogCommandPtr log_command = chartData.getContainerUnit(ids[i])->updateUnit(unit_number, units[i]))
			com->add(log_command);
	}
	if (WriteLog)
		logger.push_back(LogCommandPtr(com));
	else
		delete com;
	redrawView();
}

//-----------------------------------------------------------------------------------------------------
void CMainModel::updateUnitValues(const vector<ID>& ids, int unit_number, const vector<Value>& values)
{
	if (ids.size() == 0 || ids.size() != values.size()) return;

	LogCommand_Union* com = new LogCommand_Union();
	Unit old_unit = getUnit(ids[0], unit_number);
	for (size_t i = 0; i < ids.size(), i < values.size(); i++)
	{
		if (LogCommandPtr log_command = chartData.getContainerUnit(ids[i])->updateUnit(unit_number, Unit(values[i], old_unit.getStart(), old_unit.getDuration())))
			com->add(log_command);
	}
	if (WriteLog)
		logger.push_back(LogCommandPtr(com));
	else
		delete com;

	redrawView();
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::updateUnitPositions(const vector<ID>& ids, int unit_number, int start, int duration)
{
	if (ids.size() == 0) return;

	LogCommand_Union* com = new LogCommand_Union();
	
	for (size_t i = 0; i < ids.size(); i++)
	{
		Unit unit = getUnit(ids[i], unit_number);
		unit.setStart(start);
		unit.setDuration(duration);
		if (LogCommandPtr log_command = chartData.getContainerUnit(ids[i])->updateUnit(unit_number, unit))
			com->add(log_command);
	}
	if (WriteLog)
		logger.push_back(LogCommandPtr(com));
	else
		delete com;

	redrawView();
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::NotifyEmpty()
{
	vector<TableCommand_Ptr> table_commands;
	table_commands.push_back(TableCommand_Ptr(new CommandEmpty()));
	Notify(table_commands);
}
//-----------------------------------------------------------------------------------------------------