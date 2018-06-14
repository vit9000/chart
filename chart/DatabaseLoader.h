#pragma once
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include "ChartData.h"
#include "DBPatient.h"
#include "Constants.h"
#include "Parser.h"
#include "SQL.h"
#include <mutex>
#include "DrugListView.h"
#include "type_defines.h"
#include "IDBConnector.h"
using namespace std;


struct DrugFinder
{
	bool working = false;
	wstring find_str;
};


class DatabaseLoader : public DataCopier
{

private:
	class DatabaseLoaderDestroyer
	{
	private:
		DatabaseLoader* instance;
	public:
		~DatabaseLoaderDestroyer()
		{
			delete instance;
		}
		void initialize(DatabaseLoader* p)
		{
			instance = p;
		}
	};
	static DatabaseLoader* p_instance;
	static DatabaseLoaderDestroyer destroyer;


	

	DBPatient patient;
	ChartData administrations;
	DrugFinder drugFinder;
	map<wstring, DrugInfo> bufferedDrugs;
	vector<const DrugInfo*> selectedDrugs;
	map<wstring, int> allowedAdminWays;

	DatabaseLoader();
	
	vector<PatientInfo> patientList;
	IDBConnector* db_connector;
public:
	void setDBConnector(IDBConnector* DBconnector);
	inline const vector<PatientInfo>& getPatientList(bool reload = false) 
	{ 
		if (patientList.empty() || reload)
		{
			patientList.clear();
			PushBackFunction = [this](const void* result)
			{
				const auto* pi = reinterpret_cast<const PatientInfo*>(result);
				patientList.push_back(*pi);
			};
			db_connector->getPatientList();
			PushBackFunction = nullptr;
		}
		return patientList;
	}
	


	static DatabaseLoader& DatabaseLoader::getInstance();
	void LoadPatientChartByIndex(int index);
	void LoadPatientChartJSON(const std::wstring& fileJSON);
	int countPatients() const;
	DBPatient getPatient(int index) const;
	
	const ChartData& getAdministrations() const;
	void saveAdministrations(int index);
	const vector<const DrugInfo*>* getDrugsPtr();
	void resetBufferedDrugs();
	void getDrugNames(const wstring& str, const function<void(bool)>& callBack, bool OnlyIV=false);
	
	
	bool getDrugInfo(const wstring& name, DrugInfo& drugInfo);
	

	vector<wstring> getAllowedAdminWays() const;
	
	int getAdminWayType(const wstring& adminway);
	void getAdminWayName(wstring& adminwayname, int adminway);
	void loadAllowedAdminWays();

	function<void(const void*)> PushBackFunction;
	void push_back_data(const void* data) override
	{
		if (PushBackFunction)
			PushBackFunction(data);
	}
};