#pragma once

#include <string>
using std::wstring;

#include "ugc.h"
#include "IChartController.h"
#include "Rect.h"
#include "ContainerUnit.h"

class CTableObject
{
protected:
	int id;
	IChartController* controller;
	Rect rect;
	const ContainerUnit * unitContainer;
	wstring header;
public:
	CTableObject(int ID, IChartController* Controller, const Rect& rectangle, const ContainerUnit* containerUnit)
		: id(ID),
		controller(Controller), 
		rect(rectangle),
		unitContainer(containerUnit)
	{
		
		header = wstring(unitContainer->getName());
	}
	virtual ~CTableObject() {}

	virtual void OnPaint(UGC& ugc)
	{
		
		ugc.SetDrawColor(20,20,20);
		ugc.SetTextSize(12);
		
		wstringstream ss;
		ss << header;
		const wstring& measureUnit = unitContainer->getMeasureUnit();
		if(measureUnit.size()!=0)
			ss << L" (" << unitContainer->getMeasureUnit() << L")";
		ugc.DrawString(ss.str(), rect.x, rect.y + rect.height / 2 - ugc.GetTextHeight() / 2);
		//ugc.SetDrawColor(100,0,100,0);
		//ugc.FillRectangle(rect.x+rect.reserved, rect.y, rect.Width()-rect.reserved, rect.height);

	}

	virtual void Resize(const Rect& rectangle)
	{
		rect = Rect(rectangle);
	}

	bool IsThisObject(int x, int y)
	{
		if (x >= rect.x && x <= rect.x + rect.width
			&& y >= rect.y && y <= rect.y + rect.height)
			return true;
		return false;
	}

	virtual bool OnLButtonUp(int x, int y)
	{
		if(IsThisObject(x,y))
		{
			if(controller)
			{
				controller->objectMouseUp(id);
				return true;
			}
		}
		return false;
	}

	virtual bool OnLButtonDown(int x, int y)
	{
		if (IsThisObject(x, y))
		{
				return true;
		}
		return false;
	}
	virtual bool OnMouseMove(int x, int y)
	{
		if (IsThisObject(x, y))
		{
			
				return true;
		}
		return false;
	}

};