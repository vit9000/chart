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
	virtual void getPatientList() const = 0;
	virtual void getDrugList(const std::wstring& drug) const = 0;
	virtual void getChartJSON(const PatientInfo& patient) const = 0;
	virtual void getAdminWays() const = 0;
};