#include "stdafx.h"
#include "DatabaseLoader.h"

DatabaseLoader * DatabaseLoader::p_instance = 0;
DatabaseLoader::DatabaseLoaderDestroyer DatabaseLoader::destroyer;


DatabaseLoader::DatabaseLoader()
	: db_connector(nullptr)
{
	
	loadAllowedAdminWays();
	
	//LoadPatientChart(0);// передаем индекс пациента из списка
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
void DatabaseLoader::LoadPatientChartByIndex(int index)
{
	const auto& patientList = db_connector->getPatientList(false);
	if (patientList.size() == 0) return;

	const std::wstring& fileJSON = db_connector->getChartJSON(patientList[index]);
	LoadPatientChartJSON(fileJSON);
}

void DatabaseLoader::LoadPatientChartJSON(const std::wstring& fileJSON)
{
	//auto patient = getPatient(index);
	//auto med_card_ID = patient.case_number;
	/*
	здесь реализовать загрузку файла из базы данных,
	а пока реализована загрузка локального файла
	*/
	
	
	
	administrations = ChartData(patient.name);
	{//десериализация
		JSON_Document document;
		document.Parse(fileJSON.c_str());
		if (document.IsObject())
		{
			DBPatient p;
			p.Deserialize(document[L"patient"]);
			patient = std::move(p);
			administrations.Deserialize(document[L"blocks"]);
		}
		else
			MessageBox(0, L"Неверный формат файла", L"Ошибка", MB_OK | MB_ICONERROR);
	}
	

	{// сериализация
		using namespace rapidjson;
		using jvalue = JSON_Value;
		JSON_Document document;
		
		auto& allocator = document.GetAllocator();
		document.SetObject();

		jvalue blocks(kArrayType);
		administrations.Serialize(blocks, allocator);

		document.AddMember(L"blocks", blocks, allocator);
		
		// сохранение
		JSON_StringBuffer buffer;
		JSON_Writer writer(buffer);
		document.Accept(writer);
		// получение строки
		wstring json = buffer.GetString(); 
		
		wstring temp = json;
	}
	
}
//--------------------------------------------------------------------------------------------------------
int DatabaseLoader::countPatients() const
{
	return static_cast<int>(db_connector->getPatientList().size()); // здесь из базы данных загружаем
}
//--------------------------------------------------------------------------------------------------------
DBPatient DatabaseLoader::getPatient(int index) const
{	
	// здесь загрузка из базы данных
	return patient;//{ { L"Иванов Александр Иванович" },{ DBPatient::BloodType(1,1) },{ 40 },{ 90 },{ L"1223" },{ L"100628" } };
}
//--------------------------------------------------------------------------------------------------------
const ChartData& DatabaseLoader::getAdministrations() const
{
	return administrations;
}
//--------------------------------------------------------------------------------------------------------
void DatabaseLoader::saveAdministrations(int index)
{
	//	реализовать сохранение ChartData через сериализацию
}
//--------------------------------------------------------------------------------------------------------
const vector<const DrugInfo*>* DatabaseLoader::getDrugsPtr()
{
	return &selectedDrugs;
}
//--------------------------------------------------------------------------------------------------------
void DatabaseLoader::resetBufferedDrugs()
{
	selectedDrugs.clear();
	bufferedDrugs.clear();
}
//--------------------------------------------------------------------------------------------------------
void DatabaseLoader::getDrugNames(const wstring& str, const function<void(bool)>& callBack, bool OnlyIV)
{
	selectedDrugs.clear();
	if (str.size() < 1)
	{
		drugFinder.find_str.clear();
		bufferedDrugs.clear();
		if (callBack)
			callBack(false);
		return;
	}

	if (str == drugFinder.find_str)
		return;
	drugFinder.find_str = str;

	// функция для фильтра
	auto fiterBuffered = [this, callBack, OnlyIV]()
	{
		wstring str2(this->drugFinder.find_str);
		str2[str2.size() - 1]++;
		
		std:mutex mute;
		mute.lock();
		auto startIt = bufferedDrugs.lower_bound(this->drugFinder.find_str);
		auto endIt = bufferedDrugs.lower_bound(str2);
		mute.unlock();

		selectedDrugs.clear();
		for (startIt; startIt != endIt; ++startIt)
		{
			if (!OnlyIV || (*startIt).second.isIVallowed())
				selectedDrugs.push_back(&(*startIt).second);
		}
		
	};

	
	// сокращаем количество запросов к БД
	if (bufferedDrugs.empty() && !drugFinder.working)// если буферизация пуста, тогда загружаем данные из БД
	{
		
		// загрузка всей информации о лекарствах во втором потоке
		thread t(
			[this, str, callBack, OnlyIV, fiterBuffered]()
		{
			if (!db_connector) return;

			if (callBack)
				callBack(true);
			this->drugFinder.working = true;
			
			auto drug_list = db_connector->getDrugList(str);//(*getDrug)(str);
			selectedDrugs.clear();
			bufferedDrugs.clear();
			std::mutex mute;
			for (auto& db_name : drug_list)
			{
				DrugInfo drugInfo;
				drugInfo.dbname = db_name;
				//getExistsDrugInfo(second_sql, db_name, drugInfo);

				mute.lock();
				bufferedDrugs[db_name] = drugInfo;
				selectedDrugs.push_back(&bufferedDrugs[db_name]);
				mute.unlock();
			}
			/*
			SQL sql;
			sql.Connect();
			sql.SendRequest(L"SELECT * FROM med122 WHERE name LIKE '" + str + wstring(L"%';"));
			size_t count = static_cast<size_t>(sql.CountStrings());

			SQL second_sql;
			second_sql.Connect();
			std::mutex mute;
			for (size_t i = 0; i<count; i++)
			{
				wstring db_name = sql.RecieveNextData()[1];
				DrugInfo drugInfo;
				drugInfo.dbname = db_name;
				getExistsDrugInfo(second_sql, db_name, drugInfo);

				mute.lock();
				bufferedDrugs[db_name] = drugInfo;
				mute.unlock();

				if(str == this->drugFinder.find_str)
				{
					if (!OnlyIV || bufferedDrugs[db_name].isIVallowed())
						selectedDrugs.push_back(&bufferedDrugs[db_name]);
				}
				else
					fiterBuffered();

				if (callBack)
					callBack((i == count - 1) ? false : true);	
					//callBack(true);
			}*/
			if (callBack)
				callBack(false);
			this->drugFinder.working = false;

		}
		);
		t.detach();
	}
	else if(!drugFinder.working)//если буферизация
	{
		fiterBuffered();

		if (callBack)
			callBack(false);
	}


}
//--------------------------------------------------------------------------------------------------------
bool DatabaseLoader::getExistsDrugInfo(SQL& sql, const wstring& name, DrugInfo& drugInfo) const
{
	if (!sql.SendRequest(L"SELECT * FROM drugname_linker,druginfo WHERE drugname_linker.name = '" + name + L"' AND drugname_linker.id=druginfo.id;"))
		return false;

	auto count = sql.CountStrings();
	if (count == 0)
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
		Parser p;
		p.ParseName(name, drugInfo);
		DBDrugDialog dlg;
		dlg.Init(name, drugInfo);
		if (dlg.DoModal() == IDOK)
		{
			bufferedDrugs[name] = drugInfo;
			return true;
		}
		else return false;
	}
	return true;
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