#pragma once
#include <string>
#include <vector>
#include <functional>
#include "ID.h"
#include "Rect.h"
#include "ContainerUnit.h"
using namespace std;

enum class ACCESS { FULL_ACCESS, NURSE_ACCESS, VIEW_ACCESS };

class IChartController
{
public:
	IChartController(const ACCESS& mode) : MODE(mode)
	{}
	
	const ACCESS MODE;
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
	virtual void updateDrugUnitValue(const ID& id, int unit_number) = 0;
	virtual void updateDrugUnitValues(const vector<ID>& ids, int unit_number) = 0;
	virtual void updateUnitPosition(const ID& id, int unit_number, int start, int duration) = 0;
	virtual void updateUnitPositions(const vector<ID>& id, int unit_number, int start, int duration) = 0;
	virtual void showSmartMenu(int x, int y, const ID&id, int unit_number, vector<pair<wstring, function<void()>>>& menu)=0;
	virtual void lineSmartMenu(int x, int y, const ID&id) = 0;
	virtual void updateDrugPos(const ID& id, int new_pos) = 0;

	virtual void repaint()=0;
	virtual void SetMouseCursor(size_t index) = 0;
	//virtual void setEditBox(const Rect& rect, function<void()> callBack) = 0;
};
