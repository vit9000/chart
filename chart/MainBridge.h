#pragma once
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <map>

#include "Constants.h"
#include <mutex>
#include "DrugListView.h"
#include "type_defines.h"
#include "IDBConnector.h"
#include "LogCommandAdministrator.h"
//#include "bimap.h"
#include "utils.h" 
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
		MainBridge * instance;
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
	bool loading;

	DrugFinder drugFinder;
	map<wstring, DrugInfoEx> bufferedDrugs;
	vector<const DrugInfoEx*> selectedDrugs;
	map<int, pair<wstring, int>> allowedAdminWays; /* <CODE , pair<NAME, ADMIN_TYPE>  */

	vector<PatientInfo> patientList;
	IDBConnector* db_connector;


	MainBridge();
public:
	bool isLoading() { return loading; }
	void setLoading(bool status) { loading = status; }
	//IDBConnector * getDBConnector() { return db_connector; }
	void setDBConnector(IDBConnector* DBconnector);
	static MainBridge& MainBridge::getInstance();

	/* пациенты */
	const vector<PatientInfo>& getPatientList(double DutyDateTime, bool reload = false); /* db_connector */
	int countPatients() const;
	
	
	/* список препаратов */
	void getDrugNames(const wstring& str, const function<void(bool)>& callBack, bool OnlyIV = false);
	const vector<const DrugInfoEx*>* getDrugsPtr();
	void resetBufferedDrugs();
	bool getDrugInfo(const wstring& name, DrugInfo& drugInfo);
	
	/* пути введения */
	void loadAllowedAdminWays(); /* db_connector */
	void getAllowedAdminWays(const DrugInfoEx& drugInfoEx, vector<wstring>& result) const;
	int getAdminWayCode(const wstring& adminway);
	int getAdminWayType(int adminway_code);
	bool getAdminWayName(wstring& adminwayname, int adminway_code);
	
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

	void sendSQLRequest(const wstring& query, const vector<QueryParameter>& params, const std::function<void(IDBResult& rs)>& func);
	void createNewChart(int time_type, double date, const wstring& visitid, wstring& created_chart_id);
	
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