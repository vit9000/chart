#pragma once
#include "IDBConnector.h"
#include <atlstr.h> 
//#include "Global.h"

class DBConnector : public IDBConnector
{
private:
	std::wstring deptID;
	std::vector<PatientInfo> patientList;
public:
	const std::vector<PatientInfo>& getPatientList(bool Update) override;
	void getDrugList(const std::wstring& drug, const Push_Back_DrugInfo& push_back) override;
	void getChartJSON(const PatientInfo& patient, const Push_Back_String& push_back) override;

	void clear() override { patientList.clear(); }

	inline void setDeptID(const std::wstring id) { deptID = id; }
private:
	
};


