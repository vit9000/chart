#include "stdafx.h"
#include "CMainModel.h"

int CMainModel::getCountPatients() const
{
	return DatabaseLoader::getInstance().countPatients();
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

	DatabaseLoader& databaseLoader = DatabaseLoader::getInstance();
	databaseLoader.LoadPatientChartJSON(current);
	chartData = databaseLoader.getAdministrations();

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
	auto containerUnit = chartData.addDrug(BlockName, type, drugInfo, DatabaseLoader::getInstance().getPatient(current));

	table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(BlockName, *containerUnit)));
	Notify(table_commands);
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::addDrugToDrug(const ID& host_id, int type, const DrugInfo& drugInfo)
{
	if (current >= getCountPatients())
		return;

	auto containerUnit = chartData.addDrugToDrug(host_id, type, drugInfo, DatabaseLoader::getInstance().getPatient(current));

	vector<TableCommand_Ptr> table_commands;
	table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(host_id.getBlockName(), *containerUnit)));
	Notify(table_commands);
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::addDrugUnit(const ID& id, const Value& value, int start, int duration)
{
	chartData.addUnit(id, Unit(value, start, duration));
	NotifyEmpty();
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::addDrugUnits(const vector<ID>& ids, const vector<Value>& values, int start, int duration)
{
	for (size_t i = 0; i<ids.size(), i<values.size(); i++)
		chartData.addUnit(ids[i], Unit(values[i], start, duration));
	NotifyEmpty();
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::addParameterUnit(const ID& id, const Value& value, int start)
{
	chartData.addUnit(id, Unit(value, start, 60));
	NotifyEmpty();

}

void CMainModel::addParameterUnits(const vector<ID>& ids, const vector<Value>& values, int start)
{
	for (size_t i = 0; i<ids.size(), i<values.size(); i++)
		chartData.addUnit(ids[i], Unit(values[i], start, 60));
	NotifyEmpty();
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::deleteUnit(const ID& id, int unit_number)
{
	chartData.getContainerUnit(id)->deleteUnit(unit_number);
	NotifyEmpty();
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::updateUnitValue(const ID& id, int unit_number, const Value& value)
{
	ContainerUnit_Ptr containerUnit = chartData.getContainerUnit(id);
	Unit unit(containerUnit->getUnit(unit_number));
	unit.setValue(value);
	unit.setCompleted(false);
	containerUnit->updateUnit(unit_number, unit);
	NotifyEmpty();
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::updateUnitValues(const vector<ID>& ids, int unit_number, const vector<Value>& values)
{
	for (size_t i = 0; i < ids.size(), i<values.size(); i++)
	{
		ContainerUnit_Ptr containerUnit = chartData.getContainerUnit(ids[i]);
		Unit unit(containerUnit->getUnit(unit_number));
		unit.setValue(values[i]);
		containerUnit->updateUnit(unit_number, unit);
	}
	NotifyEmpty();
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::updateUnitPosition(const ID& id, int unit_number, int start, int duration)
{
	ContainerUnit_Ptr containerUnit = chartData.getContainerUnit(id);
	const Value& value = containerUnit->getUnit(unit_number).getValue();
	containerUnit->updateUnit(unit_number, Unit(value, start, duration));
	NotifyEmpty();
}
//-----------------------------------------------------------------------------------------------------
void CMainModel::updateUnitPositions(const vector<ID>& ids, int unit_number, int start, int duration)
{
	for (size_t i = 0; i < ids.size(); i++)
	{
		ContainerUnit_Ptr containerUnit = chartData.getContainerUnit(ids[i]);
		const Value& value = containerUnit->getUnit(unit_number).getValue();
		containerUnit->updateUnit(unit_number, Unit(value, start, duration));
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