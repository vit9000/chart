#include "stdafx.h"
#include "MainBridge.h"
#include "ParserDrugForm.h"

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
int MainBridge::countPatients() const
{
	return static_cast<int>(patientList.size()); // здесь из базы данных загружаем
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

			auto func = [this](IDBResult& rs)
			{
				wstring prev_lu;
				while (!rs.Eof())
				{
					VCopier<wstring> lu;
					rs.GetStrValue(L"LU", lu);

					if (prev_lu == lu)
					{
						rs.Next();
						continue;
					}
					prev_lu = lu;

					VCopier<wstring> name;
					rs.GetStrValue(L"NAME", name);

					VCopier<wstring> id;
					rs.GetStrValue(L"ID", id);

					DrugInfoEx newDrugInfo = ParserDrugFrom(id, name, lu);
					std::mutex mute;
					auto& drug_name = newDrugInfo.name;
					mute.lock();
					this->bufferedDrugs[drug_name] = newDrugInfo;
					this->selectedDrugs.push_back(&this->bufferedDrugs[drug_name]);
					mute.unlock();

					rs.Next();
				}
			};
			QueryParameter param(L"DRUGNAME", str + L"%");
			sendSQLRequest(L"sql_GetDrugList", { param }, func);

			
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
	//drugInfoEx.GetAllowedAdminWays(temp);

	/*if (drugInfoEx.IsAdminWaysExists())
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
	*/
	for (const auto& p : allowedAdminWays)
	{
		result.push_back(p.second.first); // NAME
	}
}
//--------------------------------------------------------------------------------------------------------
int MainBridge::getAdminWayCode(const wstring& adminway)
{
	for (const auto& p : allowedAdminWays)
	{
		if (p.second.first == adminway)
		{
			return p.first; // CODE
		}
	}
	return -1;
}
//--------------------------------------------------------------------------------------------------------
int MainBridge::getAdminWayType(int adminway_code)
{
	if (allowedAdminWays.count(adminway_code) == 0)
		return -1;
	else return allowedAdminWays.at(adminway_code).second; // ADMIN_TYPE
}
//--------------------------------------------------------------------------------------------------------
bool MainBridge::getAdminWayName(wstring& adminwayname, int adminway_code)
{
	for (const auto& p : allowedAdminWays)
	{
		if (p.first == adminway_code)
		{
			adminwayname = p.second.first;
			return true;
		}
	}
	return false;
	
}
//--------------------------------------------------------------------------------------------------------
void MainBridge::loadAllowedAdminWays()
{
	/*allowedAdminWays = std::map<int, std::wstring>
	{
		{ ADMINWAY::INTRAVENOUS_DROPS,		L"внутривенно капельно" },
	{ ADMINWAY::INTRAVENOUS_BOLUS,		L"внутривенно болюсно" },
	{ ADMINWAY::INTRAVENOUS_INFUSION,	L"внутривенно микроструйно" },

	{ ADMINWAY::INTRAMUSCULAR,			L"внутримышечно" },
	{ ADMINWAY::SUBCUTANEOUS,			L"подкожно" },
	{ ADMINWAY::ENTERAL,				L"энтерально" },
	{ ADMINWAY::RECTAL,					L"ректально" },
	{ ADMINWAY::SPINAL,					L"спинальное пространство" },
	{ ADMINWAY::EPIDURAL_BOLUS,			L"эпидурально болюсно" },
	{ ADMINWAY::EPIDURAL_INFUSION,		L"эпидурально микроструйно" },
	{ ADMINWAY::EXTERNAL,				L"наружное применение" },
	{ ADMINWAY::INHALATION,				L"ингаляция" },
	{ ADMINWAY::NASAL,					L"назально" },
	{ ADMINWAY::EYE_DROPS,				L"ушные капли" },
	{ ADMINWAY::EAR_DROPS,				L"глазные капли" }
	};*/

	auto func = [this](IDBResult& rs)
	{
		while (!rs.Eof())
		{
			VCopier<wstring> text;
			rs.GetStrValue(L"TEXT", text);
			int code = rs.GetIntValue(L"CODE");
			int sortcode = rs.GetIntValue(L"SORTCODE");
			
			allowedAdminWays[code] = make_pair(std::move(text), sortcode);

			rs.Next();
		}
	};


	MainBridge::getInstance().sendSQLRequest(L"sql_LoadAdminWays", vector<QueryParameter>(), func);

}
//--------------------------------------------------------------------------------------------------------
const vector<PatientInfo>& MainBridge::getPatientList(double DutyDateTime, bool reload)
{
	if (patientList.empty() || reload)
	{
		class PatientInfoCopierEx : public IDBResultCopier, public Capture<MainBridge>
		{
		public:
			public: PatientInfoCopierEx(MainBridge* mainBridge) : Capture(mainBridge) {}
			void push_back(IDBResult& rs) override
			{
				while (!rs.Eof())
				{
					PatientInfo pi;
					VCopier<wstring> vsc;
					rs.GetStrValue(L"Fio", vsc);
					pi[PatientInfo::FIO] = std::move(vsc);

					rs.GetStrValue(L"Age", vsc);
					pi[PatientInfo::AGE] = std::move(vsc);

					rs.GetStrValue(L"Num", vsc);
					pi[PatientInfo::NUM] = std::move(vsc);

					rs.GetStrValue(L"st_num", vsc);
					pi[PatientInfo::ST_NUM] = std::move(vsc);

					rs.GetStrValue(L"Agr", vsc);
					pi[PatientInfo::CODE] = std::move(vsc);

					rs.GetStrValue(L"dep_prof", vsc);
					pi[PatientInfo::PROF_DEP] = std::move(vsc);

					rs.GetStrValue(L"diagnos", vsc);
					pi[PatientInfo::DIAGNOS] = std::move(vsc);

					rs.GetStrValue(L"doctor", vsc);
					pi[PatientInfo::DOCTOR] = std::move(vsc);

					rs.GetStrValue(L"viskeyid", vsc);
					pi[PatientInfo::VISITID] = std::move(vsc);
					
					
					if (ptr) 
						ptr->patientList.push_back(std::move(pi));
					
					rs.Next();
				}

			}
		};

		patientList.clear();
		PatientInfoCopierEx copier(this);
		if (db_connector)
			db_connector->getPatientList(DutyDateTime, copier);
	}
	return patientList;
}
//--------------------------------------------------------------------------------------------------------
void MainBridge::sendSQLRequest(const wstring& query, const vector<QueryParameter>& params, const std::function<void(IDBResult& rs)>& func)
{
	class DBResultCopier : public IDBResultCopier
	{
		std::function<void(IDBResult& rs)> func;
	public:
	public:
		DBResultCopier(const std::function<void(IDBResult& rs)>& function)
			: func(function) {}
		void push_back(IDBResult& rs) override
		{
			if (func)
				func(rs);
		}
	};
	DBResultCopier copier(func);
	if (db_connector)
		db_connector->sendQuery(query,params, copier);
}

void MainBridge::createNewChart(int time_type, double date, const wstring& visitid, wstring& created_chart_id)
{
	COleDateTime enddate = (COleDateTime)date;
	if (time_type == TIME_TYPE::ICU_CHART)
		enddate += COleDateTimeSpan(1, 0, 0, 0);
	else if (time_type = TIME_TYPE::ANESTH_CHART)
		enddate += COleDateTimeSpan(0, 3, 0, 0);
	else
	{
		MessageBox(0, L"time_type указан не верно", L"Ошибка", MB_ICONERROR | MB_OK);
		return;
	}

	// CREATING NEW CHART
	auto func = [this, &created_chart_id](IDBResult& rs)
	{
		if (rs.Eof()) return;
		VCopier<wstring> vsc;
		rs.GetStrValue(L"ID", vsc);
		created_chart_id = std::move(vsc);
	};
	vector<QueryParameter> params;
	params.push_back(QueryParameter(L"VISIT_ID", visitid));
	params.push_back(QueryParameter(L"BGNDAT", DateToString(date)));
	params.push_back(QueryParameter(L"TIME_TYPE", time_type));
	sendSQLRequest(L"sql_NewChart", params, func);

}