#pragma once
#include <vector>
#include <string>
//#include <boost/shared_ptr.hpp>
//using boost::shared_ptr
#include <memory>
using std::shared_ptr;
using namespace std;
#include "ContainerUnit.h"
#include "ContainerDrug.h"
#include "ContainerParameter.h"

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
		ContainerUnit_Ptr drug(new ContainerDrug(DrugName));
		
		administrations.push_back(drug);
		return administrations.size() - 1;
	}
	ContainerUnit_Ptr getContainerUnit(size_t index)
	{
		if (index >= administrations.size()) throw out_of_range("getContainerUnit out_of_range");
		return administrations.at(index);
	}

	size_t addParameter(const wstring& ParameterName)
	{
		administrations.push_back(ContainerUnit_Ptr(new ContainerParameter(ParameterName)));
		return administrations.size() - 1;
	}

	bool addUnit(size_t i, const Unit& unit)
	{
		if(i>=administrations.size()) return false;
		administrations[i]->addUnit(unit);
		return true;
	}
	

};