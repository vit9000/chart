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
	if (patientList.size() == 0) return;

	std::wstring fileJSON;
	db_connector->getChartJSON(patientList[index], GET_STRING_LAMBDA(fileJSON));//[&fileJSON](const wstring& result) { fileJSON = result; });
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
	return static_cast<int>(patientList.size()); // здесь из базы данных загружаем
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
			//if (!OnlyIV || (*startIt).second.isIVallowed())
			//должна быть проверка на возможность введения препарата внутривенно!!!!!
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
			selectedDrugs.clear();
			bufferedDrugs.clear();


			db_connector->getDrugList(str, 
				[this](const DrugInfo& newDrugInfo)
				{
					std::mutex mute;
					auto& drug_name = newDrugInfo.name;
					mute.lock();
					bufferedDrugs[drug_name] = newDrugInfo;
					selectedDrugs.push_back(&bufferedDrugs[drug_name]);
					mute.unlock();
				}
			);
			
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
bool DatabaseLoader::getDrugInfo(const wstring& name, DrugInfo& drugInfo)
{
	if (bufferedDrugs.count(name) == 0)
		return false;

	drugInfo = bufferedDrugs.at(name);

	return true;
}
//--------------------------------------------------------------------------------------------------------
vector<wstring> DatabaseLoader::getAllowedAdminWays(const wstring& name) const
{
	
	vector<wstring> result;
	result.reserve(allowedAdminWays.size());
	for (const auto& way : allowedAdminWays)
	{
		result.push_back(way.first);
	}
	
	return result;

}
//--------------------------------------------------------------------------------------------------------

/*bool DatabaseLoader::getExistsDrugInfo(SQL& sql, const wstring& name, DrugInfo& drugInfo) const
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
//--------------------------------------------------------------------------------------------------------*/



int DatabaseLoader::getAdminWayType(const wstring& adminway)
{
	
	return (allowedAdminWays.count(adminway)>0) ? allowedAdminWays.at(adminway) : -1;
}
//--------------------------------------------------------------------------------------------------------
void DatabaseLoader::loadAllowedAdminWays()
{
	/* ЗАГРУЗИТЬ В БАЗУ ДАННЫХ */
	/*allowedAdminWays = {
		{ L"в / в капельно", 1},
		{L"в / в микроструйно",2},
		{L"в / в болюсно",3},
		{L"в / м",4},
		{L"п / к",5},
		{L"энтерально",6},
		{L"ректально",7},
		{L"спинальное пространство",8},
		{L"эпидуральное пространство",9 },
		{L"эпидурально микроструйно", 10},
		{L"наружное применение",11},
		{L"ингаляция",12},
		{L"назально",13},
		{L"ушные капли",14},
		{L"глазные капли",15}
	};*/
	db_connector->getAdminWays([this](const std::pair<std::wstring, int>& result) { allowedAdminWays.insert(result);} );
}