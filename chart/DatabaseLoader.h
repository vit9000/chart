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
			{ { L"Иванов Александр Иванович" },{ DBPatient::BloodType(1,1) },{40}, { 90 },{ 1223 },{ 100628 } },
			{ { L"Петров Юрий Петрович" },{ DBPatient::BloodType(0,0) },{65}, { 75 },{ 1224 },{ 91743 } },
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
		/*vector<wstring> params{ L"АДc", L"АДд", L"ЧСС",L"Per os/в зонд", L"По зонду/рвота", L"Диурез", L"По дренажам",
			L"Баланс",L"Температура",L"SpO2",L"Режим ИВЛ",L"FiO2",L"ЧД",L"МОД",L"ДО",L"ПДКВ" };*/
		vector<wstring> params{ L"Гемодинамика",L"Per os/в зонд", L"По зонду/рвота", L"Диурез", L"По дренажам",
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
		return {  L"NaCl 0.9%" , L"Glucosae 5%" , L"Венофундин", L"Альбумин",L"KCl 4%" };
	}

	vector<wstring> getDrugsIVBolus()
	{
		return { L"Беталок" , L"Метоклопрамид", L"Фуросемид" };
	}

	vector<wstring> getDrugsIVInfusion()
	{
		return { L"Кордарон" , L"Дофамин", L"Адреналин" , L"Норадреналин", L"Мезатон" , L"Нитроглицерин"};
	}

	vector<wstring> getDrugsTabs()
	{
		return { L"Кордарон" , L"Эгилок", L"Тромбо асс" , L"Фуросемид", L"Ультоп"};
	}
};