#pragma once
#include <vector>
#include <string>
//#include <boost/shared_ptr.hpp>
//using boost::shared_ptr
#include <memory>
using std::shared_ptr;
using namespace std;
#include "ModelContainers.h"

typedef shared_ptr<ContainerUnit> ContainerUnit_Ptr;

class Patient
{
private:
	wstring patient_name;
	vector<ContainerUnit_Ptr> administrations;
public:
	Patient()
	{
	}
	Patient(const wstring& patientName)
		: patient_name(patientName)
	{}
	
	const vector<ContainerUnit_Ptr>& getAdministrations() const { return administrations;}

	const wstring& getName() const
	{ return patient_name; }



	size_t addDrug(int type, const wstring& DrugName)
	{
		
		ContainerUnit_Ptr drug;
		switch (type)
		{
		default:
		case 0:
			drug = ContainerUnit_Ptr((ContainerUnit*)new ContainerIVdrops(DrugName));
			break;
		case 1:
			drug = ContainerUnit_Ptr((ContainerUnit*)new ContainerIVbolus(DrugName));
			break;
		case 2:
			drug = ContainerUnit_Ptr((ContainerUnit*)new ContainerIVinfusion(DrugName));
			break;
		case 3:
			drug = ContainerUnit_Ptr((ContainerUnit*)new ContainerTabs(DrugName));
			break;
		}


		
		
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
		if(ParameterName==L"Гемодинамика")
			administrations.push_back(ContainerUnit_Ptr(new ContainerHemodynamic()));
		else
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