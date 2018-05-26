#pragma once
#include "type_defines.h"

class IDBConnector
{
public:
	virtual const std::vector<PatientInfo>& getPatientList(bool Update=true) = 0;
	virtual const DrugList& getDrugList(const std::wstring& drug) = 0;
	virtual const std::wstring& getChartJSON(const PatientInfo& patient) = 0;
	virtual void clear() = 0;
};