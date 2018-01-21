#pragma once
#include <vector>
using std::vector;

#include "Patient.h"
#include "IObservable.h"

class IModel : public IObservable
{
protected:
	// Variables
	vector<Patient> database;
	size_t current;

	// Methods
	virtual void loadDatabase()=0;
public:
	IModel() { current=0;  }
	virtual unsigned int getCountPatients() { return database.size();}
	virtual Patient* getPatient(size_t i)  
	{ 
		size_t count = getCountPatients();
		if(i >= count) 
			return NULL; 
		return  &database.at(i);
	}
	virtual Patient* getCurrentPatient()  { return getPatient(current);}
	void setDatabase(size_t index) 
	{ 
		if(index >= getCountPatients())
			return;
		current = index; 
		Notify();
	}
};