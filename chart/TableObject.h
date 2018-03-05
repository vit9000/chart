#pragma once

#include <string>
using std::wstring;
#include "ID.h"
#include "ugc.h"
#include "IChartController.h"
#include "Rect.h"
#include "ContainerUnit.h"

class TableObject
{
protected:
	ID id;
	IChartController* controller;
	Rect rect;
	const ContainerUnit * unitContainer;
	wstring header;
	const int ValueFontSize;
public:
	TableObject(const ID& id_, IChartController* Controller, const Rect& rectangle, const ContainerUnit* containerUnit)
		: id(id_),
		controller(Controller), 
		rect(rectangle),
		unitContainer(containerUnit),
		ValueFontSize(10)
	{
		header = wstring(containerUnit->getName());
	}
	virtual ~TableObject() {}

	virtual void OnPaint(UGC& ugc)
	{
		
		ugc.SetDrawColor(155, 155, 155);
		ugc.DrawLine(rect.x, rect.y + rect.height, rect.x+rect.width, rect.y + rect.height);

		ugc.SetDrawColor(20, 20, 20);
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

	const Rect& getRect() const { return rect; }

	virtual void Resize(const Rect& rectangle)
	{
		rect.x = rectangle.x;
		rect.y = rectangle.y;
		rect.width = rectangle.width;
		rect.reserved = rectangle.reserved;
	}

	bool IsThisObject(int x, int y)
	{
		if (x >= rect.x+rect.reserved && x <= rect.x + rect.width
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
				//controller->objectMouseUp(id);
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

	virtual bool OnMouseMoveAbort()
	{
		return false;
	}

protected:
	void DrawSumm(UGC& ugc, double minuteW)
	{
		ugc.SetAlign(UGC::CENTER);
		int x = rect.x + rect.reserved + static_cast<int>(1440.*minuteW);
		int duration = static_cast<int>(60.*minuteW);
		ugc.DrawNumber(unitContainer->getSumm(), x + duration / 2, rect.y + rect.height / 2 - ugc.GetTextHeight() / 2);
		ugc.SetAlign(UGC::LEFT);
	}

};