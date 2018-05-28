#pragma once
#include "type_defines.h"

class IDBConnector
{
public:
	virtual const std::vector<PatientInfo>& getPatientList(bool Update=true) = 0;
	virtual const DrugList& getDrugList(const std::wstring& drug) = 0;
	virtual void getChartJSON(const PatientInfo& patient, const Push_Back_String& push_back) = 0;
	virtual void clear() = 0;
};