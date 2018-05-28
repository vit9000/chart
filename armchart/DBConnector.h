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
public:
	const std::vector<PatientInfo>& getPatientList(bool Update) override;
	const DrugList& getDrugList(const std::wstring& drug) override;
	void getChartJSON(const PatientInfo& patient, const Push_Back_String& push_back) override;

	void clear() override { patientList.clear(); }

	inline void setDeptID(const std::wstring id) { deptID = id; }
private:
	void LoadDrugList(const TCHAR * sql);
};


