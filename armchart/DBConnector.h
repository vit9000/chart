#pragma once
#include "IDBConnector.h"
#include <atlstr.h> 
//#include "Global.h"

class DBConnector : public IDBConnector
{
private:
	std::wstring deptID;
	DrugList drug_list;
	std::vector<PatientInfo> patientList;
	std::wstring json;
public:
	const std::vector<PatientInfo>& getPatientList(bool Update) override;
	const DrugList& getDrugList(const std::wstring& drug) override;
	const std::wstring& getChartJSON(const PatientInfo& patient) override;

	void clear() override { patientList.clear(); }

	inline void setDeptID(const std::wstring id) { deptID = id; }
private:
	void LoadDrugList(const TCHAR * sql);
};


