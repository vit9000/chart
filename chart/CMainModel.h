#pragma once

#include "Patient.h"
#include "Observable.h"
#include "CommandEmpty.h"
#include "CommandClear.h"
#include "CommandAddContainerUnit.h"

#include "DatabaseLoader.h"

class CMainModel : public Observable
{
private:
	Patient patient;
	size_t current;
public:
	CMainModel()
	{
		loadDatabase();
	}
public:
	virtual void loadDatabase()
	{
		
		DatabaseLoader db;
		
		DBPatient dbpatient = db.getPatient(0);
		vector<wstring> params = db.getParameters();
		patient = Patient(dbpatient.name);
		for (const wstring& param : params)
			patient.addParameter(param);
		
		setPatient(0);

		current = 0;
	}
public:
	virtual int getCountPatients() const
	{
		DatabaseLoader db;
		return db.countPatients();
	}
	//---------------------------------------------
	/*virtual Patient* getPatient(size_t i)
	{
		DatabaseLoader db;
		int count = db.countPatients();
		if (i >= count)
			return NULL;
		return  &database.at(i);
	}*/
	//---------------------------------------------
	virtual Patient* getCurrentPatient()
	{
		return &patient;
	}
	//---------------------------------------------
	virtual void setPatient(size_t index)
	{
		if (index >= getCountPatients())
			return;
		current = index;

		vector<TableCommand_Ptr> table_commands;
		table_commands.push_back(TableCommand_Ptr(new CommandClear()));
		const vector<ContainerUnit_Ptr>& drugs = patient.getAdministrations();
		for (size_t i = 0; i < drugs.size(); ++i)
		{
			table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(*(drugs[i]))));
		}

		Notify(table_commands);

	}
	//---------------------------------------------
	virtual void addDrug(const wstring& DrugName)
	{
		if (current >= getCountPatients())
			return;
		size_t index = patient.addDrug(DrugName);

		vector<TableCommand_Ptr> table_commands;
		table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(*(patient.getContainerUnit(index)))));
		Notify(table_commands);
	}
	//---------------------------------------------
	virtual void addDrugUnit(int index, double value, int start, int duration)
	{
		patient.addUnit(index, Unit(value, start, duration));//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		vector<TableCommand_Ptr> table_commands;
		table_commands.push_back(TableCommand_Ptr(new CommandEmpty()));
		Notify(table_commands);
		//Notify({ TableCommand_Ptr(new CommandEmpty()) });
	}


	virtual void addParameter(const wstring& Name)
	{
		if (current >= getCountPatients())
			return;
		size_t index = patient.addParameter(Name);

		vector<TableCommand_Ptr> table_commands;
		table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(*(patient.getContainerUnit(index)))));
		Notify(table_commands);
	}
	//---------------------------------------------
	virtual void addParameterUnit(int index, double value, int start)
	{
		patient.addUnit(index, Unit(value, start, 60));//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		NotifyEmpty();
		//Notify({ TableCommand_Ptr(new CommandEmpty()) });
	}

	void updateUnitValue(int index, int unit_number, double value)
	{
		auto& containerUnit = patient.getContainerUnit(index);
		Unit unit(containerUnit->getUnit(unit_number));
		unit.setValue(value);
		patient.getContainerUnit(index)->updateUnit(unit_number, unit);
		NotifyEmpty();
	}

	void updateUnitPosition(int index, int unit_number, int start, int duration)
	{
		auto& containerUnit = patient.getContainerUnit(index);
		double value = containerUnit->getUnit(unit_number).getValue();
		patient.getContainerUnit(index)->updateUnit(unit_number, Unit(value, start, duration));
		NotifyEmpty();
	}

	void NotifyEmpty()
	{
		vector<TableCommand_Ptr> table_commands;
		table_commands.push_back(TableCommand_Ptr(new CommandEmpty()));
		Notify(table_commands);
	}

};