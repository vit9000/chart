#pragma once

#include "ChartData.h"
#include "Observable.h"
#include "CommandEmpty.h"
#include "CommandClear.h"
#include "CommandAddContainerUnit.h"
#include "CommandAddBlock.h"
#include "DatabaseLoader.h"


class CMainModel : public Observable
{
private:
	ChartData chartData;
	int current;
public:
	CMainModel() :current(-1) {	}
	virtual int getCountPatients() const;
	const wstring& getContainerName(const ID& id);
	const DrugInfo& getDrugInfo(const ID& id);
	ChartData* getCurrentPatient();
	void setPatient(int index);
	void setPatient(const std::wstring& chartJSON);
	void loadPatient();
	void addDrug(int type, const DrugInfo& drugInfo);
	void addDrugToDrug(const ID& host_id, const DrugInfo& drugInfo);
	void addDrugUnit(const ID& id, const Value& value, int start, int duration);
	void addDrugUnits(const vector<ID>& ids, const vector<Value>& values, int start, int duration);
	void addParameterUnit(const ID& id, const Value& value, int start);
	void addParameterUnits(const vector<ID>& ids, const vector<Value>& values, int start);
	void deleteUnit(const ID& id, int unit_number);
	void updateUnitValue(const ID& id, int unit_number, const Value& value);
	void updateUnitValues(const vector<ID>& ids, int unit_number, const vector<Value>& values);
	void updateUnitPosition(const ID& id, int unit_number, int start, int duration);
	void updateUnitPositions(const vector<ID>& ids, int unit_number, int start, int duration);
	void NotifyEmpty();
	inline bool isChartLoaded() { return current != -1 ? true : false;  }
};