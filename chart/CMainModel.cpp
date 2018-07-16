#include "stdafx.h"
#include "CMainModel.h"

#define ERR_MSG MessageBox(parentDlg->m_hWnd, L"ƒопускаетс€ введение только числовых значений", L"¬нимание",  MB_OK | MB_ICONINFORMATION);

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
void CMainModel::setPatient(int index, const wstring& chartID)
{
	if (index >= getCountPatients())
		return;
	current = index;
	
	chartData.setPatient(index);
	chartData.loadChart(chartID);
	loadPatient();
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::loadPatient()
{
	vector<TableCommand_Ptr> table_commands;
	table_commands.push_back(TableCommand_Ptr(new CommandClear()));


	const auto& content = chartData.getAdministrations();
	for (size_t i = 0; i < content.size(); i++)
	{
		const auto& block_name = content.first(i);
		const auto& containerUnits = content.second(i);
		table_commands.push_back(TableCommand_Ptr(new CommandAddBlock(block_name, chartData.getBlockType(block_name))));

		for (size_t j=0; j<containerUnits.size(); j++)
			table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(block_name, *(containerUnits[j]), j)));
		/*for (const auto& containerUnit_ptr : containerUnits)
			table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(block_name, *containerUnit_ptr)));
			*/
	}

	//const auto& blocks = chartData.getBlockNames();

	/*for (const auto& block : blocks)
	{
		table_commands.push_back(TableCommand_Ptr(new CommandAddBlock(*block, chartData.getBlockType(*block))));
		for (const auto& containerUnit_ptr : content.at(*block))
			table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(*block, *containerUnit_ptr)));

	}*/

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
	// сперва отправл€ем команду в ѕредставление
	vector<TableCommand_Ptr> table_commands;
	table_commands.push_back(TableCommand_Ptr(new CommandDeleteContainerUnit(id)));
	Notify(table_commands);

	// затем удал€ем строку
	LogCommandPtr log_command = chartData.deleteDrug(id);
	if (log_command && !log_command->isEmpty() && WriteLog)
		logger.push_back(log_command);
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::deleteChildDrug(const ID& id)
{
	// сперва отправл€ем команду в ѕредставление
	vector<TableCommand_Ptr> table_commands;
	ID parentID = chartData.getContainerUnit(id)->getParentID();
	table_commands.push_back(TableCommand_Ptr(new CommandDeleteChildContainerUnit(parentID, id)));
	Notify(table_commands);

	// затем удал€ем строку
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
		//обновл€ем ѕредставление
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
		// обновл€етс€ представление
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
		// обновл€етс€ представление
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
		Unit old_unit = getUnit(ids[i], unit_number);
		if (LogCommandPtr log_command = chartData.getContainerUnit(ids[i])->updateUnit(unit_number, Unit(old_unit.getValue(), start, duration)))
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