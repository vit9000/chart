#pragma once
#include <string>
//#include "CMainModel.h"

using namespace std;
class IChartController
{
protected:
	//CMainModel *model;
public:
	
	virtual ~IChartController() {}
	virtual void objectMouseUp(int id) = 0;
	virtual void setPatient(size_t index)=0;
	virtual void addDrug()=0;
};
