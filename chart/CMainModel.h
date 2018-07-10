#pragma once


#include "IModel.h"
#include "Observable.h"
#include "ChartData.h"
#include "CommandEmpty.h"
#include "CommandClear.h"
#include "CommandAddContainerUnit.h"
#include "CommandDeleteContainerUnit.h"
#include "CommandAddBlock.h"
#include "CommandMoveDrug.h"
#include "MainBridge.h"
#include "LogCommandAdministrator.h"


class CMainModel : public IModel, public Observable
{
private:
	ChartData chartData;
	PatientInfo patient;

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
	void setPatient(int index, double date, int time_type);
	void loadPatient();
	const PatientInfo& getPatient() { return patient; }
	

	void addDrug(const ID& id, int type, const DrugInfo& drugInfo) override;
	void addDrug(const ID& id, int type, const DrugInfo& drugInfo, const map<int, Unit>& units, int pos) override;
	void addChildDrug(const ID& id, const ID& host_id, const DrugInfo& drugInfo) override;
	void addChildDrug(const ID& id, const ID& host_id, const DrugInfo& drugInfo, const map<int, Unit>& units) override;
	void deleteDrug(const ID& id) override;
	void deleteChildDrug(const ID& id) override;
	void moveDrug(const ID& id, int new_pos, bool redraw = true) override;
	void updateDrugPos(const ID& id, int new_pos);

	void addUnit(const ID& id, const Unit& new_unit, bool redraw=true) override;
	void deleteUnit(const ID& id, int unit_number, bool redraw=true) override;
	void updateUnit(const ID& id, int unit_number, const Unit& unit, bool redraw=true) override;
	void redrawView() override { NotifyEmpty(); };

	void addUnits(const vector<ID>& ids, const vector<Value>& values, int start, int duration);
	void updateUnits(const vector<ID>& ids, int unit_number, const vector<Unit>& units);

	inline Unit getUnit(const ID& id, int unit_number) { return getCurrentPatient()->getContainerUnit(id)->getUnit(unit_number); }
	//void updateUnitValue(const ID& id, int unit_number, const Value& value);
	void updateUnitValues(const vector<ID>& ids, int unit_number, const vector<Value>& values);
	//void updateUnitPosition(const ID& id, int unit_number, int start, int duration);
	void updateUnitPositions(const vector<ID>& ids, int unit_number, int start, int duration);
	void NotifyEmpty();
	inline bool isChartLoaded() { return current != -1 ? true : false;  }
};