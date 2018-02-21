#pragma once
#include <vector>
#include <string>
#include "Patient.h"
using namespace std;

struct DBPatient
{
	wstring name;
	wstring blood_type;
	double weight;
	int case_number;
	int patient_number;
};

class DatabaseLoader
{

private:
	vector<DBPatient> patients;
	vector<Patient> administrations;

public:
	DatabaseLoader()
	{
		patients = {
			{ { L"������ ��������� ��������" }, { L"A(II) Rh(+)" }, { 90 }, { 1223 }, { 100628 }},
			{ { L"������ ���� ��������" },{ L"O(I) Rh(+)" },{ 75 },{ 1224 },{ 91743 } },
		};
		for (int i = 0; i < patients.size(); ++i)
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
		vector<wstring> params{ L"��c", L"���", L"���",L"Per os/� ����", L"�� �����/�����", L"������", L"�� ��������",
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

};