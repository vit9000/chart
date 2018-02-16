#pragma once
#include <vector>
#include <string>
//#include <boost/shared_ptr.hpp>
//using boost::shared_ptr
#include <memory>
using std::shared_ptr;
using namespace std;
#include "ContainerUnit.h"

typedef shared_ptr<ContainerUnit> ContainerUnit_Ptr;

class Patient
{
private:
	wstring patient_name;
	vector<ContainerUnit_Ptr> administrations;
public:
	Patient(const wstring& patientName)
		: patient_name(patientName)
	{}
	
	const vector<ContainerUnit_Ptr>& getAdministrations() const { return administrations;}

	const wstring& getName() const
	{ return patient_name; }

	size_t addDrug(const wstring& DrugName)
	{
		administrations.push_back(ContainerUnit_Ptr(new ContainerUnit(DrugName)));
		return administrations.size()-1;
	}

	bool addDrugDose(size_t i, const Unit& dose)
	{
		if(i>=administrations.size()) return false;
		administrations[i]->addUnit(dose);
		return true;
	}

};