#pragma once

#include "ugc.h"
#include "IChartController.h"

struct Rect
{
	Rect(int X, int Y, int Width, int Height)
	{
		x = X;
		y = Y;
		width = Width;
		height = Height;
	}
	int x;
	int y;
	int width;
	int height;
};

class CTableObject
{
protected:
	int id;
	IChartController* controller;
	Rect rect;
public:
	CTableObject(int ID, IChartController* Controller, const Rect& rectangle)
		: id(ID),
		controller(Controller), 
		rect(rectangle)
	{}
	virtual void OnPaint(UGC& ugc)=0;
	virtual void Resize(const Rect& rectangle) = 0;
	virtual void OnLButtonUp(int x, int y)
	{
		if(x >= rect.x && x <= rect.x+rect.width 
			&& y >= rect.y && y <= rect.y+rect.height)
		{
			if(controller)
				controller->ObjectMouseUp(id);
		}
	}

};