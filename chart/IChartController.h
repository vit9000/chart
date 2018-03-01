#pragma once
#include <string>
#include "ID.h"
using namespace std;
class IChartController
{
public:
	
	virtual ~IChartController() {}
	virtual void objectMouseUp(const ID& id) = 0;
	virtual void setPatient(size_t index)=0;
	virtual void addDrug()=0;
	virtual void addDrugUnit(const ID& id, int start)=0;
	virtual void addParameterUnit(const ID& id, int start) = 0;
	virtual void updateUnitValue(const ID& id, int unit_number) = 0;
	virtual void updateUnitPosition(const ID& id, int unit_number, int start, int duration) = 0;
};
