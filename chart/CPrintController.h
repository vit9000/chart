#pragma once



#include <sstream>
#include "IChartController.h"
#include "CMainModel.h"
/*#include "MessageDialog.h"

#include "NewLineDialog.h"
#include "ValueInputDlg.h"
#include "CursorHandler.h"
*/
#include "SmartMenu.h"
#include "AdditionalFeatures.h"

class CPrintController : public IChartController
{
private:
	CMainModel * model;
	//CursorHandler * cursorHandler;
public:
	//CPrintController(CMainModel* Model, CursorHandler * cursor_handler)
	CPrintController(CMainModel* Model)
		: IChartController(ACCESS::VIEW_ACCESS), model(Model)// cursorHandler(cursor_handler)
	{}

	virtual ~CPrintController() {}

	void objectMouseUp(const ID& id) override;
	void setPatient(size_t index) override;
	void addDrug() override {};
	void showSmartMenu(int x, int y, const ID&id, int unit_number, MENU& menu) override {};
	void addDrugToDrug(const ID& host_id) override {};
	void addDrugUnit(const ID& id, int start) override {};
	void addDrugUnits(const vector<ID>& ids, int start) override {};
	void addParameterUnit(const ID& id, int start, const Rect& rect) override {};
	void addParameterUnits(const vector<ID>& ids, int start) override {};
	void updateUnitValue(const ID& id, int unit_number, const Rect& rect) override {};
	void updateUnitValue(const ID& id, int unit_number) override {};
	void updateUnitValues(const vector<ID>& ids, int unit_number) override {};
	void updateDrugUnitValue(const ID& id, int unit_number) override {};
	void updateDrugUnitValues(const vector<ID>& ids, int unit_number) override {};
	void updateUnitPosition(const ID& id, int unit_number, int start, int duration) override {};
	void updateUnitPositions(const vector<ID>& ids, int unit_number, int start, int duration) override {};
	void repaint() override;
	void SetMouseCursor(size_t index) override {};
};
