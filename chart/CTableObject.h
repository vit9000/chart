#pragma once

#include "ugc.h"
#include "IChartController.h"

struct Rect
{
	Rect(int X, int Y, int Width, int Height, int Reserved=0)
	{
		x = X;
		y = Y;
		width = Width;
		height = Height;
		reserved = Reserved;
	}
	int x;
	int y;
	int width;
	int height;
	int reserved;
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
	virtual void Resize(const Rect& rectangle)=0;
	virtual bool OnLButtonUp(int x, int y)
	{
		if(x >= rect.x && x <= rect.x+rect.width 
			&& y >= rect.y && y <= rect.y+rect.height)
		{
			if(controller)
			{
				controller->objectMouseUp(id);
				return true;
			}
		}
		return false;
	}

};