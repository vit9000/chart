#pragma once

#include <string>
using std::wstring;

#include "ugc.h"
#include "IChartController.h"
#include "Rect.h"
#include "UnitContainer.h"

class CTableObject
{
protected:
	int id;
	IChartController* controller;
	Rect rect;
	
	wstring header;
public:
	CTableObject(int ID, IChartController* Controller, const Rect& rectangle, const UnitContainer& unitContainer)
		: id(ID),
		controller(Controller), 
		rect(rectangle)
	{
		header = wstring(unitContainer.getName());
	}

	virtual void OnPaint(UGC& ugc)
	{
		ugc.SetDrawColor(100,100,0,0);
		ugc.FillRectangle(rect.x, rect.y, rect.reserved, rect.height);
		ugc.SetDrawColor(0,0,0);
		ugc.SetTextSize(12);
		ugc.DrawString(header, rect.x, rect.y + rect.height/2 - ugc.GetTextHeight()/2);

		ugc.SetDrawColor(100,0,100,0);
		ugc.FillRectangle(rect.x+rect.reserved, rect.y, rect.width-rect.reserved, rect.height);
		//ugc.DrawRectangle(rect.x + border, rect.y + border, rect.width - border*2, rect.height - border*2,static_cast<int>(2*DPIX()));		
	}

	virtual void Resize(const Rect& rectangle)
	{
		rect = Rect(rectangle);
	}

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