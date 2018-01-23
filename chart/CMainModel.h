#pragma once

#include <vector>
using std::vector;
#include "Patient.h"
#include "Observable.h"

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

		if(current < 0) current=0;
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
		Notify();
	}
	//---------------------------------------------
	virtual void addDrug(const wstring& DrugName)
	{
		if(current >= getCountPatients())
			return;
		Patient& patient = database[current];
		size_t index = patient.addDrug(DrugName);
		//patient.addDrugDose(index, Unit(500, 60, 120));

		Notify();
	}
	//---------------------------------------------
	
};