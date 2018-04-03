#include "stdafx.h"
#include "DatabaseLoader.h"

DatabaseLoader * DatabaseLoader::p_instance = 0;
DatabaseLoader::DatabaseLoaderDestroyer DatabaseLoader::destroyer;


DatabaseLoader::DatabaseLoader()
{
	
	loadAllowedAdminWays();
	dbpatient = {
		{ { L"Иванов Александр Иванович" },{ DBPatient::BloodType(1,1) },{ 40 },{ 90 },{ 1223 },{ 100628 } },
		{ { L"Петров Юрий Петрович" },{ DBPatient::BloodType(0,0) },{ 65 },{ 75 },{ 1224 },{ 91743 } },
	};
	LoadDatabase();
}
//--------------------------------------------------------------------------------------------------------
DatabaseLoader& DatabaseLoader::getInstance() 
{
	if (!p_instance) {
		p_instance = new DatabaseLoader();
		destroyer.initialize(p_instance);
	}
	return *p_instance;
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
bool DatabaseLoader::getExistsDrugInfo(SQL& sql, const wstring& name, DrugInfo& drugInfo) const
{
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
bool DatabaseLoader::getExistsDrugInfo(const wstring& name, DrugInfo& drugInfo) const
{
	if (bufferedDrugs.count(name)>0)
	{
		if (!bufferedDrugs.at(name).isExistsInDB())
			return false;
		drugInfo = bufferedDrugs.at(name);
		return true;
	}
	SQL sql;
	sql.Connect();
	return getExistsDrugInfo(sql, name, drugInfo);
}
//--------------------------------------------------------------------------------------------------------
bool DatabaseLoader::getDrugInfo(const wstring& name, DrugInfo& drugInfo)
{
	if (!getExistsDrugInfo(name, drugInfo))
	{
		//parse(name, drugInfo);
		DBDrugDialog dlg;
		dlg.Init(name, drugInfo);
		if (dlg.DoModal() == IDOK)
		{
			bufferedDrugs.clear();
			return true;
		}
		else return false;
	}
	return true;
}
//--------------------------------------------------------------------------------------------------------

void DatabaseLoader::findDrug(const wstring& str, vector<wstring>& result)
{
	if (str.size() < 2)
	{
		bufferedDrugs.clear();
		return;
	}
	result.clear();

	// сокращаем количество запросов к БД
	if (bufferedDrugs.empty())// если буферизация пуста, тогда загружаем данные из БД
	{
		SQL sql;
		sql.Connect();
		sql.SendRequest(L"SELECT * FROM med122 WHERE name LIKE '" + str + wstring(L"%';"));
		size_t count = static_cast<size_t>(sql.CountStrings());
		result.resize(count);
		for (auto& s : result)
		{
			s = sql.RecieveNextData()[1];
			bufferedDrugs[s] = DrugInfo();
		}
		// загрузка всей информации о лекарствах во втором потоке
		thread t(
			[this]()
			{
				SQL sql;
				sql.Connect();
				for (auto& it : bufferedDrugs)
				{
					getExistsDrugInfo(sql, it.first, it.second);
				}
			}
		);
		t.detach();
	}
	else//если буферизация
	{
		auto startIt = bufferedDrugs.lower_bound(str);
		wstring str2(str);
		str2[str2.size() - 1]++;
		auto endIt = bufferedDrugs.lower_bound(str2);
		result.clear();
		for (startIt; startIt != endIt; ++startIt)
		{
			/*DrugInfo& drugInfo = (*startIt).second;
			if (drugInfo.isExistsInDB())
			{
				
				result.push_back(drugInfo.getFullName());
			}
			else*/
				result.push_back((*startIt).first);
		}
	}

}
//--------------------------------------------------------------------------------------------------------
vector<wstring> DatabaseLoader::getAllowedAdminWays(const wstring& name) const
{
	vector<wstring> result;
	wstringstream ways;
	if (bufferedDrugs.count(name)>0)
	{
		ways = wstringstream(bufferedDrugs.at(name).admin_ways);
	}
	else
	{
		DrugInfo drugInfo;
		if (!getExistsDrugInfo(name, drugInfo))//если нет такого в базе данных
			return result;
		ways = wstringstream(drugInfo.admin_ways);
	}
	
	while (ways)
	{
		size_t temp=100;
		ways >> temp;
		temp--;
		if (temp < allowedAdminWays.size())
			result.push_back(allowedAdminWays[temp]);
		
	}
	return result;

}
//--------------------------------------------------------------------------------------------------------
int DatabaseLoader::getAdminWayType(const wstring& adminway)
{
	for (int i = 0; i < static_cast<int>(allowedAdminWays.size()); i++)
		if (adminway == allowedAdminWays[i])
			return i+1;
	// если не сработало, тогда загружаем из базы данных

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
void DatabaseLoader::loadAllowedAdminWays()
{
	thread t([this]()
	{
		std::mutex mute;
		SQL sql;
		sql.Connect();
		wstring request = L"SELECT * FROM admin_ways;";
		if (!sql.SendRequest(request))
			return;
		for (auto i = 0; i < sql.CountStrings(); i++)
		{
			mute.lock();
			allowedAdminWays.push_back(sql.RecieveNextData()[1]);
			mute.unlock();
		}
	}
	);
	t.detach();
}