#pragma once
#include <string>
#include <vector>
#include <functional>
#include "ID.h"
#include "Rect.h"
#include "ContainerUnit.h"
using namespace std;
class IChartController
{
public:
	
	virtual ~IChartController() {}
	virtual void objectMouseUp(const ID& id) = 0;
	virtual void setPatient(size_t index)=0;
	virtual void addDrug()=0;
	virtual void addDrugToDrug(const ID& host_id) = 0;
	virtual void addDrugUnit(const ID& id, int start)=0;
	virtual void addDrugUnits(const vector<ID>& ids, int start) = 0;
	virtual void addParameterUnit(const ID& id, int start, const Rect& rect) = 0;
	virtual void addParameterUnits(const vector<ID>& ids, int start) = 0;
	virtual void updateUnitValue(const ID& id, int unit_number) = 0;
	virtual void updateUnitValue(const ID& id, int unit_number, const Rect& rect) = 0;
	virtual void updateUnitValues(const vector<ID>& ids, int unit_number) = 0;
	virtual void updateUnitPosition(const ID& id, int unit_number, int start, int duration) = 0;
	virtual void updateUnitPositions(const vector<ID>& id, int unit_number, int start, int duration) = 0;
	virtual void showSmartMenu(int x, int y, const ID&id, vector<pair<wstring, function<void()>>>& menu)=0;

	virtual void repaint()=0;
	virtual void SetMouseCursor(size_t index) = 0;
	//virtual void setEditBox(const Rect& rect, function<void()> callBack) = 0;
};
