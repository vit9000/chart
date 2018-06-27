#pragma once


#include "IModel.h"
#include "Observable.h"
#include "ChartData.h"
#include "CommandEmpty.h"
#include "CommandClear.h"
#include "CommandAddContainerUnit.h"
#include "CommandAddBlock.h"
#include "MainBridge.h"
#include "LogCommandAdministrator.h"


class CMainModel : public IModel, public Observable
{
private:
	ChartData chartData;
	LogCommandAdministrator logger;
	int current;
public:
	CMainModel() :current(-1) {	}
	
	void setEditMenu(CMenu * editMenu) { logger.setEditMenu(editMenu); }
	bool undo();
	bool redo();
	
	virtual int getCountPatients() const;
	const wstring& getContainerName(const ID& id);
	const DrugInfo& getDrugInfo(const ID& id);
	ChartData* getCurrentPatient();
	void setPatient(int index);
	void setPatient(const std::wstring& chartJSON);
	void loadPatient();
	
	void addDrug(int type, const DrugInfo& drugInfo) override;
	void addDrugToDrug(const ID& host_id, const DrugInfo& drugInfo) override;
	
	void addUnit(const ID& id, const Value& value, int start, int duration=60, bool redraw=true) override;
	void addUnit(const ID& id, const Unit& new_unit) override;
	void addUnits(const vector<ID>& ids, const vector<Value>& values, int start, int duration=60) override;
	
	void deleteUnit(const ID& id, int unit_number) override;

	void updateUnit(const ID& id, int unit_number, const Unit& unit, bool redraw=true) override;

	void updateUnitValue(const ID& id, int unit_number, const Value& value);
	void updateUnitValues(const vector<ID>& ids, int unit_number, const vector<Value>& values);
	void updateUnitPosition(const ID& id, int unit_number, int start, int duration);
	void updateUnitPositions(const vector<ID>& ids, int unit_number, int start, int duration);
	void NotifyEmpty();
	inline bool isChartLoaded() { return current != -1 ? true : false;  }
};