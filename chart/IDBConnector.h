#pragma once
#include "type_defines.h"


class IDBConnector
{
public:
	virtual void getPatientList(const Push_Back_PatientInfo& push_back) const = 0;
	virtual void getDrugList(const std::wstring& drug, const Push_Back_DrugInfo& push_back) const = 0;
	virtual void getChartJSON(const PatientInfo& patient, const Push_Back_String& push_back) const = 0;
};