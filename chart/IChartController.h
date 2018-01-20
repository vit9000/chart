#pragma once


class IChartController
{
public:
	virtual ~IChartController() {}
	virtual void ObjectMouseUp(int id) = 0;
};
