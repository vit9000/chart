#pragma once
#include <vector>
#include <string>
using namespace std;
#include "UnitContainer.h"

class Patient
{
private:
	wstring patient_name;
	vector<UnitContainer> administrations;
public:
	Patient(const wstring& patientName)
		: patient_name(patientName)
	{}
	
	const wstring& getName() const
	{ return patient_name; }

	size_t addDrug(const wstring& DrugName)
	{
		administrations.push_back(UnitContainer(DrugName));
		return administrations.size()-1;
	}

	bool addDrugDose(size_t i, const Unit& dose)
	{
		if(i>=administrations.size()) return false;
		administrations[i].addUnit(dose);
		return true;
	}

};