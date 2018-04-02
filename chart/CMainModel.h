#pragma once

#include "ChartData.h"
#include "Observable.h"
#include "CommandEmpty.h"
#include "CommandClear.h"
#include "CommandAddContainerUnit.h"
#include "CommandAddBlock.h"
#include "DatabaseLoader.h"
#include "ChartStructure.h"

class CMainModel : public Observable
{
private:
	DatabaseLoader db;
	ChartData chartData;
	int current;
public:
	CMainModel() :current(-1)
	{
		db.LoadDatabase();
		setPatient(0);
	}

public:
	virtual int getCountPatients() const
	{
		return db.countPatients();
	}
	//---------------------------------------------
	const wstring& getContainerName(const ID& id)
	{
		return chartData.getContainerUnit(id)->getName();
	}
	const DrugInfo& getDrugInfo(const ID& id)
	{
		return chartData.getContainerUnit(id)->getDrugInfo();
	}
	//---------------------------------------------
	virtual ChartData* getCurrentPatient()
	{
		return &chartData;
	}
	//---------------------------------------------
	virtual void setPatient(int index)
	{
		
		if (index >= getCountPatients())
			return;
		if (current != -1)
			db.saveAdministrations(current, chartData);
		
		chartData = db.getAdministrations(index);
		
		current = index;

		vector<TableCommand_Ptr> table_commands;
		table_commands.push_back(TableCommand_Ptr(new CommandClear()));

		const map<wstring, map<int, ContainerUnit_Ptr>>& content = chartData.getAdministrations();
		ChartStructure * chartStructure = ChartStructure::getInstance();
		for (const auto& block : content)
		{
			table_commands.push_back(TableCommand_Ptr(new CommandAddBlock(block.first, chartStructure->getBlockType(block.first))));
			for(const auto& containerUnit_ptr : block.second)
				table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(block.first, *(containerUnit_ptr.second))));
			/*for (size_t i = 0; i < block.second.size(); ++i)
			{
				table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(block.first, *(block.second[i]))));
			}*/
		}

		Notify(table_commands);

	}
	//---------------------------------------------
	virtual void addDrug(int type, const DrugInfo& drugInfo)
	{
		if (current >= getCountPatients())
			return;
		
		vector<TableCommand_Ptr> table_commands;
		wstring BlockName = ChartStructure::getInstance()->getAdministrationsBlockName();
		auto containerUnit = chartData.addDrug(BlockName, type, drugInfo);
		
		table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(BlockName, *containerUnit)));
		Notify(table_commands);
	}
	//---------------------------------------------
	virtual void addDrugToDrug(const ID& host_id, int type, const DrugInfo& drugInfo)
	{
		if (current >= getCountPatients())
			return;

		auto containerUnit = chartData.addDrugToDrug(host_id, type, drugInfo);

		vector<TableCommand_Ptr> table_commands;
		table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(host_id.getBlockName(), *containerUnit)));
		Notify(table_commands);
	}
	//---------------------------------------------
	virtual void addDrugUnit(const ID& id, const Value& value, int start, int duration)
	{
		chartData.addUnit(id, Unit(value, start, duration));//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		NotifyEmpty();
	}

	virtual void addDrugUnits(const vector<ID>& ids, const vector<Value>& values, int start, int duration)
	{
		for (size_t i = 0; i<ids.size(), i<values.size(); i++)
			chartData.addUnit(ids[i], Unit(values[i], start, duration));//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		NotifyEmpty();
		
	}
	//---------------------------------------------
	virtual void addParameterUnit(const ID& id, const Value& value, int start)
	{
		chartData.addUnit(id, Unit(value, start, 60));//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		NotifyEmpty();
		
	}

	virtual void addParameterUnits(const vector<ID>& ids, const vector<Value>& values, int start)
	{
		for(size_t i = 0; i<ids.size(), i<values.size(); i++)
			chartData.addUnit(ids[i], Unit(values[i], start, 60));//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		NotifyEmpty();
		
	}

	void updateUnitValue(const ID& id, int unit_number, const Value& value)
	{
		ContainerUnit_Ptr containerUnit = chartData.getContainerUnit(id);
		Unit unit(containerUnit->getUnit(unit_number));
		unit.setValue(value);
		containerUnit->updateUnit(unit_number, unit);
		NotifyEmpty();
	}

	void updateUnitValues(const vector<ID>& ids, int unit_number, const vector<Value>& values)
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


	void updateUnitPosition(const ID& id, int unit_number, int start, int duration)
	{
		ContainerUnit_Ptr containerUnit = chartData.getContainerUnit(id);
		const Value& value = containerUnit->getUnit(unit_number).getValue();
		containerUnit->updateUnit(unit_number, Unit(value, start, duration));
		NotifyEmpty();
	}

	void updateUnitPositions(const vector<ID>& ids, int unit_number, int start, int duration)
	{
		for (size_t i = 0; i < ids.size(); i++)
		{
			ContainerUnit_Ptr containerUnit = chartData.getContainerUnit(ids[i]);
			const Value& value = containerUnit->getUnit(unit_number).getValue();
			containerUnit->updateUnit(unit_number, Unit(value, start, duration));
		}
		NotifyEmpty();
	}

	void NotifyEmpty()
	{
		vector<TableCommand_Ptr> table_commands;
		table_commands.push_back(TableCommand_Ptr(new CommandEmpty()));
		Notify(table_commands);
	}

};