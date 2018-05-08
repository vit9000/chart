#pragma once
#include <string>
#include <vector>
#include <functional>
#include "ID.h"
#include "Rect.h"
#include "ContainerUnit.h"
#include "CallBackFunction.h"
#include "shared_ptr.h"

#include <memory>
using namespace std;

class IChartController;

typedef Callback::CallBackFunction<IChartController> CallbackController;
typedef SMART_POINTER::shared_ptr<CallbackController> CallbackController_Ptr;
 //typedef std::shared_ptr<CallbackController> CallbackController_Ptr;
//typedef CallbackController* CallbackController_Ptr;
typedef pair<wstring, CallbackController_Ptr> MENU_ITEM;
typedef vector<MENU_ITEM> MENU;


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
	virtual void showSmartMenu(int x, int y, const ID&id, int unit_number, MENU& menu)=0;
	virtual void setComplete(const ID& id, int unit_number, bool temp)=0;
	virtual void deleteUnit(const ID& id, int unit_number)=0;
	virtual void drugInformation(const ID& id)=0;
	virtual void repaint()=0;
	virtual void SetMouseCursor(size_t index) = 0;
	//virtual void setEditBox(const Rect& rect, function<void()> callBack) = 0;
};
