#pragma once

#include "IModel.h"

class IChartController
{
protected:
	IModel *model;
public:
	IChartController(IModel* Model):model(Model){}
	virtual ~IChartController() {}
	virtual void objectMouseUp(int id) = 0;
	virtual void setDatabase(size_t index)=0;
};
