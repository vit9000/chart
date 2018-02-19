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
	vector<Patient> database;
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
		
		vector<DBPatient> dbpatients = db.getPatients();
		vector<wstring> params = db.getParameters();
		for (const auto& p : dbpatients)
		{
			Patient patient(p.name);
			for (const wstring& param : params)
				patient.addParameter(param);
			database.push_back(patient);
		}
		setPatient(0);

		current = 0;
	}
public:
	virtual size_t getCountPatients() const
	{
		return database.size();
	}
	//---------------------------------------------
	virtual Patient* getPatient(size_t i)
	{
		size_t count = getCountPatients();
		if (i >= count)
			return NULL;
		return  &database.at(i);
	}
	//---------------------------------------------
	virtual Patient* getCurrentPatient()
	{
		return getPatient(current);
	}
	//---------------------------------------------
	virtual void setPatient(size_t index)
	{
		if (index >= getCountPatients())
			return;
		current = index;

		vector<TableCommand_Ptr> table_commands;
		table_commands.push_back(TableCommand_Ptr(new CommandClear()));
		const vector<ContainerUnit_Ptr>& drugs = database[current].getAdministrations();
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
		Patient& patient = database[current];
		size_t index = patient.addDrug(DrugName);

		

		vector<TableCommand_Ptr> table_commands;
		table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(*(patient.getContainerUnit(index)))));
		Notify(table_commands);
	}
	//---------------------------------------------
	virtual void addDrugUnit(int index, double value, int start, int duration)
	{
		database[current].addUnit(index, Unit(value, start, duration));//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		vector<TableCommand_Ptr> table_commands;
		table_commands.push_back(TableCommand_Ptr(new CommandEmpty()));
		Notify(table_commands);
		//Notify({ TableCommand_Ptr(new CommandEmpty()) });
	}


	virtual void addParameter(const wstring& Name)
	{
		if (current >= getCountPatients())
			return;
		Patient& patient = database[current];
		size_t index = patient.addParameter(Name);

		vector<TableCommand_Ptr> table_commands;
		table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(*(patient.getContainerUnit(index)))));
		Notify(table_commands);
	}
	//---------------------------------------------
	virtual void addParameterUnit(int index, double value, int start)
	{
		database[current].addUnit(index, Unit(value, start, 60));//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		vector<TableCommand_Ptr> table_commands;
		table_commands.push_back(TableCommand_Ptr(new CommandEmpty()));
		Notify(table_commands);
		//Notify({ TableCommand_Ptr(new CommandEmpty()) });
	}

};