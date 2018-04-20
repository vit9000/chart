#pragma once
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include "ChartData.h"
#include "DBPatient.h"
#include "ChartStructure.h"
#include "Parser.h"
#include "SQL.h"
#include <mutex>
#include "DrugListView.h"
using namespace std;


struct DrugFinder
{
	bool working = false;
	wstring find_str;
};


class DatabaseLoader
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

	vector<DBPatient> dbpatient;
	vector<ChartData> administrations;
	DrugFinder drugFinder;
	map<wstring, DrugInfo> bufferedDrugs;
	vector<const DrugInfo*> selectedDrugs;
	vector<wstring> allowedAdminWays;

	DatabaseLoader();
	void LoadDatabase();
public:
	static DatabaseLoader& DatabaseLoader::getInstance();
	
	int countPatients() const;
	DBPatient getPatient(int index) const;
	ChartData getAdministrations(int index) const;
	void saveAdministrations(int index, const ChartData& p);
	const vector<const DrugInfo*>* getDrugsPtr();
	void resetBufferedDrugs();
	void getDrugNames(const wstring& str, const function<void(bool)>& callBack, bool OnlyIV=false);
	bool getExistsDrugInfo(SQL& sql, const wstring& name, DrugInfo& drugInfo) const;
	bool getExistsDrugInfo(const wstring& name, DrugInfo& drugInfo) const;
	bool getDrugInfo(const wstring& name, DrugInfo& drugInfo);
	int getAdminWayType(const wstring& adminway);
	
	vector<wstring> getAllowedAdminWays(const wstring& name) const;
	void loadAllowedAdminWays();
	
};