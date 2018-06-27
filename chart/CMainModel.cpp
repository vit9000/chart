#include "stdafx.h"
#include "CMainModel.h"

#define ERR_MSG MessageBox(parentDlg->m_hWnd, L"ƒопускаетс€ введение только числовых значений", L"¬нимание",  MB_OK | MB_ICONINFORMATION);

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
void CMainModel::setPatient(int index)
{
	if (index >= getCountPatients())
		return;
	current = index;
	MainBridge::getInstance().loadPatientChartByIndex(current);
	loadPatient();
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::setPatient(const std::wstring& chartJSON)
{
	current = 0;
	MainBridge::getInstance().loadPatientChartJSON(chartJSON);
	loadPatient();
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::loadPatient()
{
	chartData = MainBridge::getInstance().getAdministrations();
	vector<TableCommand_Ptr> table_commands;
	table_commands.push_back(TableCommand_Ptr(new CommandClear()));


	const auto& content = chartData.getAdministrations();
	for (size_t i = 0; i < content.size(); i++)
	{
		const auto& block_name = content.first(i);
		const auto& containerUnits = content.second(i);
		table_commands.push_back(TableCommand_Ptr(new CommandAddBlock(block_name, chartData.getBlockType(block_name))));
		for (const auto& containerUnit_ptr : containerUnits)
			table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(block_name, *containerUnit_ptr)));
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
void CMainModel::addDrug(int type, const DrugInfo& drugInfo)
{
	if (current >= getCountPatients())
		return;

	vector<TableCommand_Ptr> table_commands;
	wstring BlockName = chartData.getAdministrationsBlockName();
	auto containerUnit = chartData.addDrug(BlockName, ADMINWAY::getAdminTypeByWay(type), drugInfo, MainBridge::getInstance().getPatient(current));

	table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(BlockName, *containerUnit)));
	Notify(table_commands);
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::addDrugToDrug(const ID& host_id, const DrugInfo& drugInfo)
{
	if (current >= getCountPatients())
		return;

	auto containerUnit = chartData.addDrugToDrug(host_id, drugInfo, MainBridge::getInstance().getPatient(current));

	vector<TableCommand_Ptr> table_commands;
	table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(host_id.getBlockName(), *containerUnit)));
	Notify(table_commands);
}
//-----------------------------------------------------------------------------------------------------



void CMainModel::addUnit(const ID& id, const Value& value, int start, int duration, bool redraw)
{
	if (LogCommandPtr log_command = chartData.getContainerUnit(id)->addUnit(Unit(value, start, duration)))
	{
		//записываем все в LogCommandAdministrator
		logger.push_back(log_command);
		//обновл€ем ѕредставление
		NotifyEmpty();
	}
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::addUnits(const vector<ID>& ids, const vector<Value>& values, int start, int duration)
{
	for (size_t i = 0; i < ids.size(), i < values.size(); i++)
	{
		addUnit(ids[i], values[i], start, duration);

	}
	NotifyEmpty();
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::deleteUnit(const ID& id, int unit_number)
{
	if (LogCommandPtr log_command = chartData.getContainerUnit(id)->deleteUnit(unit_number))
	{
		// записываем все в LogCommandAdministrator
		logger.push_back(log_command);
		// обновл€етс€ представление
		NotifyEmpty();
	}
}
void CMainModel::updateUnit(const ID& id, int unit_number, const Unit& unit, bool redraw)
{
	ContainerUnit_Ptr containerUnit = chartData.getContainerUnit(id);
	if (LogCommandPtr log_command = containerUnit->updateUnit(unit_number, unit))
	{
		// записываем все в LogCommandAdministrator
		logger.push_back(log_command);
		// обновл€етс€ представление
		//if(redraw)
			NotifyEmpty();
	}
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::updateUnitValue(const ID& id, int unit_number, const Value& value)
{
	ContainerUnit_Ptr containerUnit = chartData.getContainerUnit(id);
	Unit unit(containerUnit->getUnit(unit_number));
	unit.setValue(value);
	unit.setCompleted(false);

	updateUnit(id, unit_number, unit);
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::updateUnitValues(const vector<ID>& ids, int unit_number, const vector<Value>& values)
{
	for (size_t i = 0; i < ids.size(), i<values.size(); i++)
	{
		ContainerUnit_Ptr containerUnit = chartData.getContainerUnit(ids[i]);
		Unit unit(containerUnit->getUnit(unit_number));
		unit.setValue(values[i]);
		updateUnit(ids[i], unit_number, unit, false);
	}
	NotifyEmpty();
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::updateUnitPosition(const ID& id, int unit_number, int start, int duration)
{
	ContainerUnit_Ptr containerUnit = chartData.getContainerUnit(id);
	const Value& value = containerUnit->getUnit(unit_number).getValue();
	Unit unit(value, start, duration);
	updateUnit(id, unit_number, unit, false);
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::updateUnitPositions(const vector<ID>& ids, int unit_number, int start, int duration)
{
	for (size_t i = 0; i < ids.size(); i++)
	{
		ContainerUnit_Ptr containerUnit = chartData.getContainerUnit(ids[i]);
		const Value& value = containerUnit->getUnit(unit_number).getValue();
		Unit unit(value, start, duration);
		updateUnit(ids[i], unit_number, unit, false);
	}
	NotifyEmpty();
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::NotifyEmpty()
{
	vector<TableCommand_Ptr> table_commands;
	table_commands.push_back(TableCommand_Ptr(new CommandEmpty()));
	Notify(table_commands);
}
//-----------------------------------------------------------------------------------------------------