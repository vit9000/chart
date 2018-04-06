#pragma once

#include <string>
using std::wstring;
#include "ID.h"
#include "ugc.h"
#include "IChartController.h"
#include "Rect.h"
#include "ContainerUnit.h"
#include "Button.h"

class TableObject
{
protected:
	int sort_type;

	ID id;
	IChartController* controller;
	Rect rect;
	const ContainerUnit * unitContainer;
	wstring header;
	const int ValueFontSize;

	typedef std::shared_ptr<TableObject> Obj_Ptr;
	vector<Obj_Ptr> child_objects;

	Button * button;
	Gdiplus::Color color;
public:
	static const int LINE_HEIGHT = 20;

	TableObject(IChartController* Controller, const ContainerUnit* containerUnit)
		: sort_type(containerUnit->getAdminWay()),
		id(containerUnit->getID()),
		controller(Controller),
		rect(Rect(0, 0, 1, LINE_HEIGHT, 1)),
		unitContainer(containerUnit),
		ValueFontSize(10),
		button(nullptr)

	{
		switch (sort_type)
		{
		case 1:
			color = Gdiplus::Color::CornflowerBlue;
			break;
		case 2:
			color = Gdiplus::Color::Crimson;
			break;
		case 3:
			color = Gdiplus::Color::ForestGreen;
			break;
		case 4:
			color = Gdiplus::Color::Red;
			break;

		case 6:
			color = Gdiplus::Color::Chocolate;
			break;
		case 8:
		case 9:
		case 10:
			color = Gdiplus::Color::Green;
			break;
		default:
			color = Gdiplus::Color::Gray;

		}
		//LINE_HEIGHT = 22 * DPIX();
		rect.height = static_cast<int>(LINE_HEIGHT * DPIX());
		header = wstring(containerUnit->getName());
	}
	virtual ~TableObject() {
		if (button)
			delete button;
	}

	const ID& getID()
	{
		return id;
	}

	vector<ID> getAllIDs()
	{
		vector<ID> ids{ getID() };
		for (const auto& obj : child_objects)
			ids.push_back(obj->getID());
		return ids;
	}

	virtual void addChild(const ContainerUnit* new_container)
	{
		/*if (new_container)
		{
			child_objects.push_back(Obj_Ptr(new TableObject(controller, new_container)));
			controller->repaint();
		}*/
	}

	const ContainerUnit * getContainerUnit()
	{
		return unitContainer;
	}
	void SetSpecColor(UGC& ugc)
	{
		ugc.SetDrawColor(color);
	}

	virtual void OnPaint(UGC& ugc)
	{
		
		
		ugc.SetDrawColor(20, 20, 20);
		
		//wstringstream ss;
		//ss << header;
		const auto& drugInfo = unitContainer->getDrugInfo();
		const wstring& measureUnit = unitContainer->getMeasureUnit();
		int y_shift = 0;
		
		if (measureUnit.empty())
		{
			
			y_shift = rect.height / 2 - ugc.GetTextHeight() / 2;
		}
		else
		{
	
			ugc.SetTextSize(9);
			ugc.SetAlign(UGC::RIGHT);
			y_shift = 0;// rect.height / 3 - static_cast<int>(2 * ugc.getDPIX());
			int y = rect.y + rect.height - ugc.GetTextHeight();
			ugc.DrawString(measureUnit, static_cast<int>(rect.x + rect.reserved), y);
			ugc.SetAlign(UGC::LEFT);
			
			if (drugInfo.isSolution())
			{
				ugc.DrawString((drugInfo.dilution.empty()) ? drugInfo.getPercentString() : drugInfo.dilution,
					static_cast<int>(rect.x + 10 * ugc.getDPIX()), y);
			}
			
		}
		
		ugc.SetDrawColor(20, 20, 20);
		ugc.SetTextSize(11);
		ugc.DrawString(header, static_cast<int>(rect.x + 10 * ugc.getDPIX()), rect.y + y_shift);
	
		if (button)
			button->OnDraw(ugc);
		
	}

	virtual void addAddButton()
	{

	}
	

	const Rect& getRect() const 
	{ 
		return rect; 
	}

	virtual void ResizeButton()
	{
		if (button)
		{
			Rect r(rect);
			int height = static_cast<int>(LINE_HEIGHT * DPIX());
			int h = height * 4 / 5;
			double dpix = DPIX();
			int border = static_cast<int>(2.*dpix);
			button->resize(Rect(rect.x+rect.reserved - h - border, rect.y + height / 2 - h / 2, h, h));
			
		}
	}

	virtual void Resize(const Rect& rectangle)
	{
		rect.x = rectangle.x;
		rect.y = rectangle.y;
		rect.width = rectangle.width;
		rect.reserved = rectangle.reserved;
		// все кроме высоты
		ResizeButton();
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
			if (button && button->OnLButtonUp(x, y))
				return true;

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
			if (button && button->OnLButtonDown(x, y))
				return true;
			return true;
		}
		return false;
	}
	virtual bool OnMouseMove(int x, int y)
	{
		if (IsThisObject(x, y))
		{
			if (button && button->OnMouseMove(x, y))
				return true;
			return true;
		}
		
		return false;
	}

	virtual bool OnMouseMoveAbort()
	{
		return false;
	}

	friend bool operator<(const TableObject& lhs, const TableObject& rhs)
	{
		return lhs.sort_type < rhs.sort_type;
	}
protected:
	void DrawSumm(UGC& ugc, double minuteW)
	{
		ugc.SetAlign(UGC::CENTER);
		int x = rect.x + rect.reserved + static_cast<int>(1440.*minuteW);
		int duration = static_cast<int>(60.*minuteW);
		ugc.SetTextSize(ValueFontSize);
		ugc.SetDrawColor(10, 10, 10);
		ugc.DrawString(unitContainer->getSumm(), x + duration / 2, rect.y + rect.height / 2 - ugc.GetTextHeight() / 2);
		ugc.SetAlign(UGC::LEFT);
	}

};

