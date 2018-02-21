#pragma once
#include <vector>
#include <string>
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

public:
	DatabaseLoader()
	{
		patients = {
			{ { L"Иванов Александр Иванович" }, { L"A(II) Rh(+)" }, { 90 }, { 1223 }, { 100628 }},
			{ { L"Петров Юрий Петрович" },{ L"O(I) Rh(+)" },{ 75 },{ 1224 },{ 91743 } },
		};
	}

	vector<wstring> getParameters()
	{
		/*vector<wstring> params{ L"АДc", L"АДд", L"ЧСС",L"Per os/в зонд", L"По зонду/рвота", L"Диурез", L"По дренажам",
			L"Баланс",L"Температура",L"SpO2",L"Режим ИВЛ",L"FiO2",L"ЧД",L"МОД",L"ДО",L"ПДКВ" };*/
		vector<wstring> params{ L"АДc", L"АДд", L"ЧСС",L"Per os/в зонд", L"По зонду/рвота", L"Диурез", L"По дренажам",
			L"Баланс"}; 
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

};