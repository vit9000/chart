#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "Patient.h"
#include "DBPatient.h"
using namespace std;





class DatabaseLoader
{

private:
	vector<DBPatient> patients;
	vector<Patient> administrations;

public:
	DatabaseLoader()
	{
		patients = {
			{ { L"������ ��������� ��������" },{ DBPatient::BloodType(1,1) },{40}, { 90 },{ 1223 },{ 100628 } },
			{ { L"������ ���� ��������" },{ DBPatient::BloodType(0,0) },{65}, { 75 },{ 1224 },{ 91743 } },
		};
	}

	void LoadDatabase()
	{
		
		for (size_t i = 0; i < patients.size(); ++i)
		{
			administrations.push_back(Patient(patients.at(i).name));
			for (const wstring& param : getParameters())
				administrations[i].addParameter(param);

		}
	}

	vector<wstring> getParameters()
	{
		/*vector<wstring> params{ L"��c", L"���", L"���",L"Per os/� ����", L"�� �����/�����", L"������", L"�� ��������",
			L"������",L"�����������",L"SpO2",L"����� ���",L"FiO2",L"��",L"���",L"��",L"����" };*/
		vector<wstring> params{ L"������������",L"Per os/� ����", L"�� �����/�����", L"������", L"�� ��������",
			L"������"}; 
		return params;
	}

	int countPatients() const
	{
		return static_cast<int>(patients.size());
	}

	

	DBPatient getPatient(int index) const
	{
		if (index >= countPatients())
			throw invalid_argument("getPatient: index >= countPatients()");
		return patients.at(index);
	}

	Patient getAdministrations(int index) const
	{
		if (index >= countPatients())
			throw invalid_argument("getAdministrations: index >= countPatients()");
		return administrations.at(index);
	}

	void saveAdministrations(int index, const Patient& p)
	{
		if (index >= countPatients())
			throw invalid_argument("saveAdministrations: index >= countPatients()");
		administrations[index] = p;
	}

	vector<wstring> getDrugsIVDrops()
	{
		return {  L"NaCl 0.9%" , L"Glucosae 5%" , L"����������", L"��������",L"KCl 4%" };
	}

	vector<wstring> getDrugsIVBolus()
	{
		return { L"�������" , L"�������������", L"���������" };
	}

	vector<wstring> getDrugsIVInfusion()
	{
		return { L"��������" , L"�������", L"���������" , L"������������", L"�������" , L"�������������"};
	}

	vector<wstring> getDrugsTabs()
	{
		return { L"��������" , L"������", L"������ ���" , L"���������", L"������"};
	}
};