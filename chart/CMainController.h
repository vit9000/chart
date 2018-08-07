#pragma once

#include <sstream>
#include "IChartController.h"
#include "CMainModel.h"

#include "NewLineDialog.h"
#include "ValueInputDlg.h"
#include "CursorHandler.h"

#include "SmartMenu.h"
#include "AdditionalFeatures.h"

class CMainController : public IChartController
{
protected:
	CMainModel* model;
	CursorHandler * cursorHandler;

	CMainController(const ACCESS& access, CMainModel* Model, CursorHandler * cursor_handler)
		: IChartController(access), model(Model), cursorHandler(cursor_handler)
	{}
public:
	CMainController(CMainModel* Model, CursorHandler * cursor_handler)
		: IChartController(ACCESS::FULL_ACCESS), model(Model), cursorHandler(cursor_handler)
	{}

	virtual ~CMainController() {}

	void objectMouseUp(const ID& id) override;
	void setPatient(size_t index) override;
	void addDrug() override;
	void showSmartMenu(int x, int y, const ID&id, int unit_number, MENU& menu) override;
	void lineSmartMenu(int x, int y, const ID&id) override;
	void addDrugToDrug(const ID& host_id) override;
	void addDrugUnit(const ID& id, int start) override;
	void addDrugUnits(const vector<ID>& ids, int start) override;
	void addParameterUnit(const ID& id, int start, const Rect& rect) override;
	void addParameterUnits(const vector<ID>& ids, int start) override;
	void updateUnitValue(const ID& id, int unit_number, const Rect& rect) override;
	void updateUnitValue(const ID& id, int unit_number) override;
	void updateUnitValues(const vector<ID>& ids, int unit_number) override;
	inline void updateDrugUnitValue(const ID& id, int unit_number) override { updateUnitValue(id, unit_number); }
	inline void updateDrugUnitValues(const vector<ID>& ids, int unit_number) override { updateUnitValues(ids, unit_number); };
	void updateUnitPosition(const ID& id, int unit_number, int start, int duration) override;
	void updateUnitPositions(const vector<ID>& ids, int unit_number, int start, int duration) override;
	void repaint() override;
	void SetMouseCursor(size_t index) override;
	void updateDrugPos(const ID& id, int new_pos) { model->updateDrugPos(id, new_pos); };

	bool isAvailableUpdateUnit(const Unit& unit);
};

