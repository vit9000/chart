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

			/*class DrugInfoExCopierEx : public IDBResultCopier, public Capture<MainBridge>
			{
			public:
				DrugInfoExCopierEx(MainBridge* mainBridge) : Capture(mainBridge) {}
				void push_back(IDBResult& rs) override
				{
					if (!ptr) return;

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
						ptr->bufferedDrugs[drug_name] = newDrugInfo;
						ptr->selectedDrugs.push_back(&ptr->bufferedDrugs[drug_name]);
						mute.unlock();
						
						rs.Next();
					}
				}
			};

			
			DrugInfoExCopierEx copier(this);
			if (db_connector)
				db_connector->getDrugList(str, copier);*/
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