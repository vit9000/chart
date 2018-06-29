#pragma once


#include <sstream>
#include "IChartController.h"
#include "CMainModel.h"
#include "MessageDialog.h"

#include "NewLineDialog.h"
#include "ValueInputDlg.h"
#include "CursorHandler.h"

#include "SmartMenu.h"
#include "AdditionalFeatures.h"
#include "CMainController.h"

class CNurseController : public CMainController
{
private:
	CMainModel * model;
	CursorHandler * cursorHandler;
public:
	CNurseController(CMainModel* Model, CursorHandler * cursor_handler)
		: CMainController(ACCESS::NURSE_ACCESS, Model, cursor_handler)
	{}

	virtual ~CNurseController() {}

	void objectMouseUp(const ID& id) override;
	void setPatient(size_t index) override;
	
	void updateDrugUnitValue(const ID& id, int unit_number) override;
	void updateDrugUnitValues(const vector<ID>& ids, int unit_number) override;
// ����������� ����������
	void addDrug() override {};
	void showSmartMenu(int x, int y, const ID&id, int unit_number, MENU& menu) override {};
	void addDrugToDrug(const ID& host_id) override {};
	void addDrugUnit(const ID& id, int start) override {};
	void addDrugUnits(const vector<ID>& ids, int start) override {};
	void updateUnitPosition(const ID& id, int unit_number, int start, int duration) override {};
	void updateUnitPositions(const vector<ID>& ids, int unit_number, int start, int duration) override {};
	void SetMouseCursor(size_t index) override {};
private:
	void tempUpdateDrugUnitValue(const ID& id, int unit_number);
};

