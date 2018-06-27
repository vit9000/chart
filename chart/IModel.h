#pragma once

#include <vector>
#include "DrugInfo.h"
#include "ID.h"
#include "Value.h"
#include "Unit.h"

using namespace std;
class IModel
{
protected:
	bool WriteLog;
public:
	IModel() : WriteLog(true) {	}

	virtual void addDrug(int type, const DrugInfo& drugInfo) = 0;
	virtual void addDrugToDrug(const ID& host_id, const DrugInfo& drugInfo) = 0;

	virtual void addUnit(const ID& id, const Value& value, int start, int duration = 60, bool redraw = true) = 0;
	virtual void addUnit(const ID& id, const Unit& new_unit) = 0;
	virtual void addUnits(const vector<ID>& ids, const vector<Value>& values, int start, int duration = 60) = 0;

	virtual void deleteUnit(const ID& id, int unit_number) = 0;

	virtual void updateUnit(const ID& id, int unit_number, const Unit& unit, bool redraw = true) = 0;

};