#pragma once

#include "Patient.h"
#include "Observable.h"
#include "CommandAddDrug.h"
#include "CommandClear.h"
#include "CommandAddContainerUnit.h"


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
protected:
	virtual void loadDatabase()
	{
		database.push_back(Patient (L"Иванов Иван Иванович"));
		database.push_back(Patient (L"Петров Петр Петрович"));
		current=0;
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
		if(i >= count) 
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
		if(index >= getCountPatients())
			return;
		current = index; 
		
		vector<TableCommand_Ptr> table_commands;
		table_commands.push_back(TableCommand_Ptr(new CommandClear()));
		const vector<ContainerUnit_Ptr>& drugs = database[current].getAdministrations();
		for(size_t i=0; i<drugs.size(); ++i)
		{
			table_commands.push_back(TableCommand_Ptr(new CommandAddContainerUnit(*(drugs[i]))));
		}

		Notify(table_commands);

	}
	//---------------------------------------------
	virtual void addDrug(const wstring& DrugName)
	{
		if(current >= getCountPatients())
			return;
		Patient& patient = database[current];
		size_t index = patient.addDrug(DrugName);
		//patient.addDrugDose(index, Unit(500, 60, 120));

		vector<TableCommand_Ptr> table_commands;
		table_commands.push_back(TableCommand_Ptr(new CommandAddDrug(DrugName)));
		Notify(table_commands);
	}
	//---------------------------------------------

};