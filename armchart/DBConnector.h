#pragma once
#include "IDBConnector.h"
#include <atlstr.h> 
//#include "Global.h"

class DBConnector : public IDBConnector
{
private:
	std::wstring deptID;
public:
	DBConnector(const std::wstring dept_id) : deptID(dept_id) {}

	void getPatientList(const Push_Back_PatientInfo& push_back) const override;
	void getDrugList(const std::wstring& drug, const Push_Back_DrugInfo& push_back) const override;
	void getChartJSON(const PatientInfo& patient, const Push_Back_String& push_back) const override;
	//void getAdminWays(const Push_Back_AdminWay& push_back) const override;
	void getAdminWays() const override;
private:
	
};


