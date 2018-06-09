#pragma once
#include "type_defines.h"

class DataCopier
{
public:
	virtual void push_back_data(const void*) = 0;
};

class IDBConnector
{

public:
	IDBConnector() : copier(NULL) {}
	DataCopier * copier;
	virtual void getPatientList(const Push_Back_PatientInfo& push_back) const = 0;
	virtual void getDrugList(const std::wstring& drug, const Push_Back_DrugInfo& push_back) const = 0;
	virtual void getChartJSON(const PatientInfo& patient, const Push_Back_String& push_back) const = 0;
	//virtual void getAdminWays(const Push_Back_AdminWay& push_back) const = 0;
	virtual void getAdminWays() const = 0;
};