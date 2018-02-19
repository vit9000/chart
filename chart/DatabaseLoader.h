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
public:
	vector<wstring> getParameters()
	{
		vector<wstring> params{ L"АДc", L"АДд", L"ЧСС",L"Per os/в зонд", L"По зонду/рвота", L"Диурез", L"По дренажам",
			L"Баланс",L"Температура",L"SpO2",L"Режим ИВЛ",L"FiO2",L"ЧД",L"МОД",L"ДО",L"ПДКВ" };
		return params;
	}

	vector<DBPatient> getPatients()
	{
		return { 
			{{ L"Иванов Александр Иванович" },{ L"A(II) Rh(+)"}, {90}, {1223}, {100628}},
			{{ L"Петров Юрий Петрович" },{ L"O(I) Rh(+)" },{ 75 },{ 1224 },{ 91743 } },
		};
	}

};