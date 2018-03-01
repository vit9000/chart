#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "ChartData.h"
#include "DBPatient.h"
#include "ChartStructure.h"
using namespace std;





class DatabaseLoader
{

private:
	vector<DBPatient> dbpatient;
	vector<ChartData> administrations;
	
public:

	DatabaseLoader()
	{
		dbpatient = {
			{ { L"Иванов Александр Иванович" },{ DBPatient::BloodType(1,1) },{40}, { 90 },{ 1223 },{ 100628 } },
			{ { L"Петров Юрий Петрович" },{ DBPatient::BloodType(0,0) },{65}, { 75 },{ 1224 },{ 91743 } },
		};
	}

	void LoadDatabase()
	{
		ChartStructure structure;
		for (size_t i = 0; i < dbpatient.size(); ++i)
		{
			administrations.push_back(ChartData(dbpatient.at(i).name));
			
			for (const wstring& block : structure.getBlocks())
				for(const wstring& param : structure.getBlockParameters(block))
					administrations[i].addParameter(block, param);
		}
	}

	

	int countPatients() const
	{
		return static_cast<int>(dbpatient.size());
	}

	

	DBPatient getPatient(int index) const
	{
		if (index >= countPatients())
			throw invalid_argument("getChartData: index >= countChartDatas()");
		return dbpatient.at(index);
	}

	ChartData getAdministrations(int index) const
	{
		if (index >= countPatients())
			throw invalid_argument("getAdministrations: index >= countChartDatas()");
		return administrations.at(index);
	}

	void saveAdministrations(int index, const ChartData& p)
	{
		if (index >= countPatients())
			throw invalid_argument("saveAdministrations: index >= countChartDatas()");
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