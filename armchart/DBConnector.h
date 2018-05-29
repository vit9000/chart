#pragma once
#include "IDBConnector.h"
#include <atlstr.h> 
//#include "Global.h"

class DBConnector : public IDBConnector
{
private:
	std::wstring deptID;
public:
	void getPatientList(const Push_Back_PatientInfo& push_back) const override;
	void getDrugList(const std::wstring& drug, const Push_Back_DrugInfo& push_back) const override;
	void getChartJSON(const PatientInfo& patient, const Push_Back_String& push_back) const override;

	inline void setDeptID(const std::wstring id) { deptID = id; }
private:
	
};


