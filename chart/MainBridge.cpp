#include "stdafx.h"
#include "MainBridge.h"

MainBridge * MainBridge::p_instance = 0;
MainBridge::MainBridgeDestroyer MainBridge::destroyer;


MainBridge::MainBridge()
	: db_connector(nullptr)
{
	
	//loadAllowedAdminWays();
	
	//LoadPatientChart(0);// передаем индекс пациента из списка
}
//--------------------------------------------------------------------------------------------------------
MainBridge& MainBridge::getInstance() 
{
	if (!p_instance) {
		p_instance = new MainBridge();
		destroyer.initialize(p_instance);
	}
	return *p_instance;
}
//--------------------------------------------------------------------------------------------------------
void MainBridge::setDBConnector(IDBConnector* DBconnector) 
{ 
	db_connector = DBconnector;
	loadAllowedAdminWays(); 
}
//--------------------------------------------------------------------------------------------------------
void MainBridge::loadPatientChartByIndex(int index)
{
	if (patientList.size() == 0) return;

	class StringCopierEx : public StringCopier, public Capture<wstring>
	{
		public: StringCopierEx(wstring * _str) : Capture(_str) {}
			void push_back_data(const wstring& result) const override { if (ptr) (*ptr) = result; }
	};

	std::wstring fileJSON;
	StringCopierEx copier(&fileJSON);
	if (db_connector)
		db_connector->getChartJSON(patientList[index], copier);

	loadPatientChartJSON(fileJSON);

	administrations.loadChartTemplate(db_connector);
}

void MainBridge::loadPatientChartJSON(const std::wstring& fileJSON)
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
int MainBridge::countPatients() const
{
	return static_cast<int>(patientList.size()); // здесь из базы данных загружаем
}
//--------------------------------------------------------------------------------------------------------
DBPatient MainBridge::getPatient(int index) const
{	
	// здесь загрузка из базы данных
	return patient;//{ { L"Иванов Александр Иванович" },{ DBPatient::BloodType(1,1) },{ 40 },{ 90 },{ L"1223" },{ L"100628" } };
}
//--------------------------------------------------------------------------------------------------------
const ChartData& MainBridge::getAdministrations() const
{
	return administrations;
}
//--------------------------------------------------------------------------------------------------------
void MainBridge::saveAdministrations(int index)
{
	//	реализовать сохранение ChartData через сериализацию
}
//--------------------------------------------------------------------------------------------------------
const vector<const DrugInfoEx*>* MainBridge::getDrugsPtr()
{
	return &selectedDrugs;
}
//--------------------------------------------------------------------------------------------------------
void MainBridge::resetBufferedDrugs()
{
	selectedDrugs.clear();
	bufferedDrugs.clear();
}
//--------------------------------------------------------------------------------------------------------
void MainBridge::getDrugNames(const wstring& str, const function<void(bool)>& callBack, bool OnlyIV)
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
		
		std::mutex mute;
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


			

			class DrugInfoExCopierEx : public DrugInfoExCopier, public Capture<MainBridge>
			{	
			public:
				DrugInfoExCopierEx(MainBridge* mainBridge) : Capture(mainBridge) {}
				void push_back_data(const DrugInfoEx& newDrugInfo) const override
				{ 
					if (!ptr) return;
					std::mutex mute;
					auto& drug_name = newDrugInfo.name;
					mute.lock();
					ptr->bufferedDrugs[drug_name] = newDrugInfo;
					ptr->selectedDrugs.push_back(&ptr->bufferedDrugs[drug_name]);
					mute.unlock();
				}
			};

			
			DrugInfoExCopierEx copier(this);
			if (db_connector)
				db_connector->getDrugList(str, copier);

			
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
bool MainBridge::getDrugInfo(const wstring& name, DrugInfo& drugInfo)
{
	if (bufferedDrugs.count(name) == 0)
		return false;

	drugInfo = bufferedDrugs.at(name);

	return true;
}
//--------------------------------------------------------------------------------------------------------
void MainBridge::getAllowedAdminWays(const DrugInfoEx& drugInfoEx, vector<wstring>& result) const
{
	result.clear();
	result.reserve(allowedAdminWays.size());
	vector<int> temp;
	drugInfoEx.GetAllowedAdminWays(temp);


	if (drugInfoEx.IsAdminWaysExists())
	{
		for(const auto it : allowedAdminWays.getMap())
		{
			bool is_allowed = drugInfoEx.GetAllowedAdminWay(it.first);
			if (is_allowed)
				result.push_back(it.second);
		}
	}
	else 
		allowedAdminWays.getVector(result);
	
}
//--------------------------------------------------------------------------------------------------------
int MainBridge::getAdminWayType(const wstring& adminway)
{
	return ((allowedAdminWays.count(adminway) == 0) ? -1 : allowedAdminWays.at(adminway));
}
//--------------------------------------------------------------------------------------------------------
bool MainBridge::getAdminWayName(wstring& adminwayname, int adminway)
{
	if ((allowedAdminWays.count(adminway) == 0))
		return false;
	adminwayname =  allowedAdminWays.at(adminway);
	return true;
}
//--------------------------------------------------------------------------------------------------------
void MainBridge::loadAllowedAdminWays()
{
	class PairCopierEx : public PairCopier, public Capture<MainBridge>
	{	
	public: PairCopierEx(MainBridge* mainBridge) : Capture(mainBridge) {}
		void push_back_data(const pair<int, wstring>& result) const override { if (ptr) ptr->allowedAdminWays.insert(result); }
	};

	PairCopierEx copier(this);
	if(db_connector)
		db_connector->getAdminWays(copier);

}
//--------------------------------------------------------------------------------------------------------
const vector<PatientInfo>& MainBridge::getPatientList(double DutyDateTime, bool reload)
{
	if (patientList.empty() || reload)
	{
		class PatientInfoCopierEx : public PatientInfoCopier, public Capture<MainBridge>
		{	public: PatientInfoCopierEx(MainBridge* mainBridge) : Capture(mainBridge) {}
			void push_back_data(const PatientInfo& result) const override { if (ptr) ptr->patientList.push_back(result); }
		};

		patientList.clear();
		PatientInfoCopierEx copier(this);
		if (db_connector)
			db_connector->getPatientList(DutyDateTime, copier);
	}
	return patientList;
}