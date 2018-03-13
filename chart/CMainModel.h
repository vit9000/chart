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
	const wstring& getContainerName(ID id)
	{
		return chartData.getContainerUnit(id.getBlockName(), static_cast<size_t>(id.getIndex()))->getName();
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

		const map<wstring, vector<ContainerUnit_Ptr>>& content = chartData.getAdministrations();
		ChartStructure * chartStructure = ChartStructure::getInstance();
		for (const auto& block : content)
		{
			table_commands.push_back(TableCommand_Ptr(new CommandAddBlock(block.first, chartStructure->getBlockType(block.first))));
			for (size_t i = 0; i < block.second.size(); ++i)
			{
				table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(block.first, *(block.second[i]))));
			}
		}

		Notify(table_commands);

	}
	//---------------------------------------------
	virtual void addDrug(int type, const wstring& DrugName)
	{
		if (current >= getCountPatients())
			return;
		
		vector<TableCommand_Ptr> table_commands;
		wstring BlockName = ChartStructure::getInstance()->getAdministrationsBlockName();
		size_t index = chartData.addDrug(BlockName, type, DrugName);
		
		table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(BlockName, *(chartData.getContainerUnit(BlockName,index)))));
		Notify(table_commands);
	}
	//---------------------------------------------
	virtual void addDrugUnit(const ID& id, const Value& value, int start, int duration)
	{
		chartData.addUnit(id.getBlockName(), id.getIndex(), Unit(value, start, duration));//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		vector<TableCommand_Ptr> table_commands;
		table_commands.push_back(TableCommand_Ptr(new CommandEmpty()));
		Notify(table_commands);
	}

	//---------------------------------------------
	virtual void addParameterUnit(const ID& id, const Value& value, int start)
	{
		chartData.addUnit(id.getBlockName(), id.getIndex(), Unit(value, start, 60));//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		NotifyEmpty();
		//Notify({ TableCommand_Ptr(new CommandEmpty()) });
	}

	virtual void addParameterUnits(const vector<ID>& ids, const vector<Value>& values, int start)
	{
		for(size_t i = 0; i<ids.size(), i<values.size(); i++)
			chartData.addUnit(ids[i].getBlockName(), ids[i].getIndex(), Unit(values[i], start, 60));//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		NotifyEmpty();
		//Notify({ TableCommand_Ptr(new CommandEmpty()) });
	}

	void updateUnitValue(const ID& id, int unit_number, const Value& value)
	{
		auto& containerUnit = chartData.getContainerUnit(id.getBlockName(), id.getIndex());
		Unit unit(containerUnit->getUnit(unit_number));
		unit.setValue(value);
		chartData.getContainerUnit(id.getBlockName(), id.getIndex())->updateUnit(unit_number, unit);
		NotifyEmpty();
	}

	void updateUnitValues(const vector<ID>& ids, int unit_number, const vector<Value>& values)
	{
		for (size_t i = 0; i < ids.size(), i<values.size(); i++)
		{
			auto& containerUnit = chartData.getContainerUnit(ids[i].getBlockName(), ids[i].getIndex());
			Unit unit(containerUnit->getUnit(unit_number));
			unit.setValue(values[i]);
			chartData.getContainerUnit(ids[i].getBlockName(), ids[i].getIndex())->updateUnit(unit_number, unit);
		}
		NotifyEmpty();
	}


	void updateUnitPosition(const ID& id, int unit_number, int start, int duration)
	{
		auto& containerUnit = chartData.getContainerUnit(id.getBlockName(), id.getIndex());
		const Value& value = containerUnit->getUnit(unit_number).getValue();
		chartData.getContainerUnit(id.getBlockName(), id.getIndex())->updateUnit(unit_number, Unit(value, start, duration));
		NotifyEmpty();
	}

	void NotifyEmpty()
	{
		vector<TableCommand_Ptr> table_commands;
		table_commands.push_back(TableCommand_Ptr(new CommandEmpty()));
		Notify(table_commands);
	}

};