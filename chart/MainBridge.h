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
#include "bimap.h"
using namespace std;


struct DrugFinder
{
	bool working = false;
	wstring find_str;
};


class MainBridge
{

private:
	class MainBridgeDestroyer
	{
	private:
		MainBridge* instance;
	public:
		~MainBridgeDestroyer()
		{
			delete instance;
		}
		void initialize(MainBridge* p)
		{
			instance = p;
		}
	};
	static MainBridge* p_instance;
	static MainBridgeDestroyer destroyer;


	
private:
	DBPatient patient;
	ChartData administrations;
	DrugFinder drugFinder;
	map<wstring, DrugInfoEx> bufferedDrugs;
	vector<const DrugInfoEx*> selectedDrugs;
	bimap<int, wstring> allowedAdminWays;

	MainBridge();
	
	vector<PatientInfo> patientList;
	IDBConnector* db_connector;
public:
	
	void setDBConnector(IDBConnector* DBconnector);
	inline void setAppMenu(CMenu* menu) { db_connector->setAppMenu(menu); }
	inline void executeApp(UINT nID) { db_connector->executeApp(nID); }
	inline void showAboutDlg() { db_connector->showAboutDlg(); }

	const vector<PatientInfo>& getPatientList(bool reload = false);
	
	


	static MainBridge& MainBridge::getInstance();
	void LoadPatientChartByIndex(int index);
	void LoadPatientChartJSON(const std::wstring& fileJSON);
	int countPatients() const;
	DBPatient getPatient(int index) const;
	
	const ChartData& getAdministrations() const;
	void saveAdministrations(int index);
	const vector<const DrugInfoEx*>* getDrugsPtr();
	void resetBufferedDrugs();
	void getDrugNames(const wstring& str, const function<void(bool)>& callBack, bool OnlyIV=false);
	
	
	bool getDrugInfo(const wstring& name, DrugInfo& drugInfo);
	

	void getAllowedAdminWays(const DrugInfoEx& drugInfoEx, vector<wstring>& result) const;
	int getAdminWayType(const wstring& adminway);
	bool getAdminWayName(wstring& adminwayname, int adminway);
	void loadAllowedAdminWays();

	/*BOOL GetParamBool(int Code, const BoolCopier&) const;
	double GetParamNumber(int Code, const DoubleCopier&) const;
	wstring GetParamText(int Code, const StringCopier&) const;*/
	
	void GetParamText(int Code, wstring& text)
	{
		class StringCopierEx : public StringCopier, public Capture<wstring>
		{
		public: StringCopierEx(wstring * _str) : Capture(_str) {}
				void push_back_data(const wstring& result) const override { if (ptr) (*ptr) = result; }
		};

		StringCopierEx copier(&text);
		if (db_connector)
			db_connector->GetParamText(Code, copier);
	}


protected:
	template<class T>
	class Capture
	{
	protected:
		T * ptr;
	public:
		Capture(T* obj) : ptr(obj) {}
	};
	friend Capture<MainBridge>;
	
};