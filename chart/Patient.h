#pragma once
#include <vector>
#include <string>
using namespace std;
#include "Unit.h"

class Patient
{
private:
	wstring patient_name;
	vector<Unit> administrations;
public:
	Patient(wstring patientName)
		: patient_name(patientName)
	{}
	
	 wstring& getName() 
	{ return patient_name;}

};