#include "stdafx.h"
#include "DatabaseLoader.h"

DatabaseLoader::DatabaseLoader()
{

	dbpatient = {
		{ { L"Иванов Александр Иванович" },{ DBPatient::BloodType(1,1) },{ 40 },{ 90 },{ 1223 },{ 100628 } },
		{ { L"Петров Юрий Петрович" },{ DBPatient::BloodType(0,0) },{ 65 },{ 75 },{ 1224 },{ 91743 } },
	};
}
//--------------------------------------------------------------------------------------------------------
void DatabaseLoader::LoadDatabase()
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
//--------------------------------------------------------------------------------------------------------

int DatabaseLoader::countPatients() const
{
	return static_cast<int>(dbpatient.size());
}
//--------------------------------------------------------------------------------------------------------
DBPatient DatabaseLoader::getPatient(int index) const
{
	if (index >= countPatients())
		throw invalid_argument("getChartData: index >= countChartDatas()");
	return dbpatient.at(index);
}
//--------------------------------------------------------------------------------------------------------
ChartData DatabaseLoader::getAdministrations(int index) const
{
	if (index >= countPatients())
		throw invalid_argument("getAdministrations: index >= countChartDatas()");
	return administrations.at(index);
}
//--------------------------------------------------------------------------------------------------------
void DatabaseLoader::saveAdministrations(int index, const ChartData& p)
{
	if (index >= countPatients())
		throw invalid_argument("saveAdministrations: index >= countChartDatas()");
	administrations[index] = p;
}
//--------------------------------------------------------------------------------------------------------
void DatabaseLoader::getDrugNames(const wstring& str, CListBox *drugs_list)
{
	vector<wstring> result;
	findDrug(str, result);
	drugs_list->ResetContent();
	for (const auto& drug : result)
	{
		drugs_list->AddString(drug.c_str());
	}
}
//--------------------------------------------------------------------------------------------------------
bool DatabaseLoader::isDrugInfoExists(const wstring& name, DrugInfo& drugInfo) const
{
	SQL sql;
	sql.Connect();
	if (!sql.SendRequest(L"SELECT * FROM drugname_linker,druginfo WHERE drugname_linker.name = '" + name + L"' AND drugname_linker.id=druginfo.id;"))
		return false;

	if (sql.CountStrings() == 0)
		return false;
	auto result = sql.RecieveNextData();
	result.erase(result.begin(), result.begin() + 3);
	drugInfo = DrugInfo(name, result);
	return true;
}
//--------------------------------------------------------------------------------------------------------
bool DatabaseLoader::getDrugInfo(const wstring& name, DrugInfo& drugInfo)
{
	if (!isDrugInfoExists(name, drugInfo))
	{
		//parse(name, drugInfo);
		DBDrugDialog dlg;
		dlg.Init(name, drugInfo);
		if (dlg.DoModal() == IDOK)
		{
			return true;
		}
		else return false;
	}
	return true;
}
//--------------------------------------------------------------------------------------------------------
int DatabaseLoader::getAdminWayType(const wstring& adminway)
{
	SQL sql;
	sql.Connect();
	wstring request = L"SELECT * FROM admin_ways WHERE name ='" + adminway + L"';";
	if (!sql.SendRequest(request))
		return -1;

	wstringstream ss(sql.RecieveNextData()[0]);
	int temp = -1;
	ss >> temp;
	return temp;
}
//--------------------------------------------------------------------------------------------------------
void DatabaseLoader::findDrug(const wstring& str, vector<wstring>& result)
{
	if (str.size() == 0)
		return;
	result.clear();


	SQL sql;
	sql.Connect();
	sql.SendRequest(L"SELECT * FROM med122 WHERE name LIKE '" + str + wstring(L"%';"));
	size_t count = static_cast<size_t>(sql.CountStrings());
	result = vector<wstring>(count);
	for (auto& s : result)
		s = sql.RecieveNextData()[1];

}
//--------------------------------------------------------------------------------------------------------
vector<wstring> DatabaseLoader::getAllowedAdminWays(const wstring& name) const
{
	vector<wstring> result;
	SQL sql;
	sql.Connect();
	if (!sql.SendRequest(L"SELECT * FROM drugname_linker,druginfo WHERE drugname_linker.name = '" + name + L"' AND drugname_linker.id=druginfo.id;"))
		return result;
	if (sql.CountStrings() == 0)
		return result;
	vector<wstring> data = sql.RecieveNextData();
	wstringstream ss(data[8]);
	while (ss)
	{
		wstring temp;
		ss >> temp;
		if (temp.empty()) continue;
		wstring request = L"SELECT * FROM admin_ways WHERE id = '" + temp + L"';";
		if (!sql.SendRequest(request))
			return result;
		result.push_back(sql.RecieveNextData()[1]);

	}
	return result;
}