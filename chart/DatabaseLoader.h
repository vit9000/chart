#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "ChartData.h"
#include "DBPatient.h"
#include "ChartStructure.h"
#include "Parser.h"
#include "SQL.h"
using namespace std;





class DatabaseLoader
{

private:
	vector<DBPatient> dbpatient;
	vector<ChartData> administrations;
public:

	DatabaseLoader();
	void LoadDatabase();
	int countPatients() const;
	DBPatient getPatient(int index) const;
	ChartData getAdministrations(int index) const;
	void saveAdministrations(int index, const ChartData& p);
	void getDrugNames(const wstring& str, CListBox *drugs_list);
	bool isDrugInfoExists(const wstring& name, DrugInfo& drugInfo) const;
	bool getDrugInfo(const wstring& name, DrugInfo& drugInfo);
	int getAdminWayType(const wstring& adminway);
	void findDrug(const wstring& str, vector<wstring>& result);
	vector<wstring> getAllowedAdminWays(const wstring& name) const;
	
};