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
#include "LogCommandAdministrator.h"
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
	
	vector<PatientInfo> patientList;
	IDBConnector* db_connector;

	
	MainBridge();
public:
	void setDBConnector(IDBConnector* DBconnector);
	static MainBridge& MainBridge::getInstance();

	/* пациенты */
	const vector<PatientInfo>& getPatientList(double DutyDateTime, bool reload = false); /* db_connector */
	int countPatients() const;
	DBPatient getPatient(int index) const;

	/* карта */
	void loadPatientChartByIndex(int index); /* db_connector */
	void loadPatientChartJSON(const std::wstring& fileJSON);
	const ChartData& getAdministrations() const;
	void saveAdministrations(int index);

	/* список препаратов */
	void getDrugNames(const wstring& str, const function<void(bool)>& callBack, bool OnlyIV = false);
	const vector<const DrugInfoEx*>* getDrugsPtr();
	void resetBufferedDrugs();
	bool getDrugInfo(const wstring& name, DrugInfo& drugInfo);
	
	/* пути введения */
	void loadAllowedAdminWays(); /* db_connector */
	void getAllowedAdminWays(const DrugInfoEx& drugInfoEx, vector<wstring>& result) const;
	int  getAdminWayType(const wstring& adminway);
	bool getAdminWayName(wstring& adminwayname, int adminway);
	
	/* дополнительные функции db_connector*/
	inline void setAppMenu(CMenu* menu) { db_connector->setAppMenu(menu); }
	inline void executeApp(UINT nID) { db_connector->executeApp(nID); }
	inline void showAboutDlg() { db_connector->showAboutDlg(); }
	inline void showLogDlg() { db_connector->showLogDialog(); }
	

	// функция для получения параметра из БД по коду обращения - сделано шаблоном для всех типов возвращаемых значений
	template<class T>
	void getDBParam(int Code, T& result)
	{
		class TCopierEx : public DLLCopier<T>, public Capture<T>
		{
		public:
			TCopierEx(T * pointer) : Capture(pointer) {};
			void push_back_data(const T& result) const override { if (ptr) (*ptr) = result; }
		};

		TCopierEx copier(&result);
		if (db_connector)
			db_connector->GetParam<T>(Code, copier);
	}


protected:
	template<class T>
	class Capture
	{
	protected:
		T * ptr;
	public:
		Capture(T* pointer) : ptr(pointer) {}
	};
	friend Capture<MainBridge>;
	
};