#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "ChartData.h"
#include "DBPatient.h"
#include "ChartStructure.h"
#include "Drugstore.h"
using namespace std;





class DatabaseLoader
{

private:
	vector<DBPatient> dbpatient;
	vector<ChartData> administrations;
public:

	DatabaseLoader()
	{
		Drugstore& drugstore = Drugstore::getInstance();//инициализация 

		dbpatient = {
			{ { L"Иванов Александр Иванович" },{ DBPatient::BloodType(1,1) },{40}, { 90 },{ 1223 },{ 100628 } },
			{ { L"Петров Юрий Петрович" },{ DBPatient::BloodType(0,0) },{65}, { 75 },{ 1224 },{ 91743 } },
		};
	}

	

	void LoadDatabase()
	{
		ChartStructure * chartStructure = ChartStructure::getInstance();
		for (size_t i = 0; i < dbpatient.size(); ++i)
		{
			administrations.push_back(ChartData(dbpatient.at(i).name));
			
			for (const wstring& block : chartStructure->getBlocks())
			{
				administrations[i].addBlock(block);
				for (const auto& param : chartStructure->getBlockParameters(block))
					administrations[i].addParameter(block, param.first, param.second);
			}
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

	void getDrugNames(const wstring& str, CListBox *drugs_list)
	{
		Drugstore& drugstore = Drugstore::getInstance();
		
		vector<wstring> result;
		drugstore.find(str, result);
		drugs_list->ResetContent();
		for (const auto& drug : result)
		{
			drugs_list->AddString(drug.c_str());
		}
	}

	inline bool getDrugInfo(const wstring& name, DrugInfo& drugInfo)
	{
		return Drugstore::getInstance().getDrugInfo(name, drugInfo);
	}


	
};