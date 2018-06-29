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

	virtual void addUnit(const ID& id, const Unit& new_unit, bool redraw=true) = 0;
	virtual void deleteUnit(const ID& id, int unit_number, bool redraw=true) = 0;
	virtual void updateUnit(const ID& id, int unit_number, const Unit& unit, bool redraw = true) = 0;

	virtual void redrawView() = 0;

};