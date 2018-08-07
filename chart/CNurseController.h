#pragma once

#include "CMainController.h"

class CNurseController : public CMainController
{
public:
	CNurseController(CMainModel* Model, CursorHandler * cursor_handler)
		: CMainController(ACCESS::NURSE_ACCESS, Model, cursor_handler)
	{}

	virtual ~CNurseController() {}

	void objectMouseUp(const ID& id) override;
	void setPatient(size_t index) override;
	
	void updateDrugUnitValue(const ID& id, int unit_number) override;
	void updateDrugUnitValues(const vector<ID>& ids, int unit_number) override;
// выключенный функционал
	void addDrug() override {};
	void showSmartMenu(int x, int y, const ID&id, int unit_number, MENU& menu) override {};
	void addDrugToDrug(const ID& host_id) override {};
	void addDrugUnit(const ID& id, int start) override {};
	void addDrugUnits(const vector<ID>& ids, int start) override {};
	void updateUnitPosition(const ID& id, int unit_number, int start, int duration) override {};
	void updateUnitPositions(const vector<ID>& ids, int unit_number, int start, int duration) override {};
	void SetMouseCursor(size_t index) override {};
	void lineSmartMenu(int x, int y, const ID&id) override {};
	void updateDrugPos(const ID& id, int new_pos) {};
private:
	void tempUpdateDrugUnitValue(const ID& id, int unit_number);
};

