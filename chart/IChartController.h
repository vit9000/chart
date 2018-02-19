#pragma once
#include <string>

using namespace std;
class IChartController
{
public:
	
	virtual ~IChartController() {}
	virtual void objectMouseUp(int id) = 0;
	virtual void setPatient(size_t index)=0;
	virtual void addDrug()=0;
	virtual void addDrugUnit(int ID, int start)=0;
	virtual void addParameterUnit(int ID, int start) = 0;
	virtual void updateUnitValue(int ID, int unit_number) = 0;
	virtual void updateUnitPosition(int ID, int unit_number, int start, int duration) = 0;
};
