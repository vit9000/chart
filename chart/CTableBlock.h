#pragma once

#include "TableLines.h"
#include "TableParameter.h"
#include "TableHemodynamic.h"
#include "StatusButton.h"
#include "Button.h"
typedef shared_ptr<TableObject> CTableObject_Ptr;
typedef shared_ptr<Button> Button_Ptr;


class CTableBlock
{
private:
	IChartController* controller;
	wstring header;
	vector<CTableObject_Ptr> objects;
	Rect rect;
	
	vector<Button_Ptr> buttons;
	int headerHeight;
	bool fullView;
	bool Administrations;
public:
	CTableBlock()
		: controller (nullptr),
		header(L""),
		rect(Rect(0,0,1,1,1)),
		headerHeight (TableObject::LINE_HEIGHT),
		fullView(true),
		Administrations(false)
	{
		rect.height = headerHeight;
		buttons.push_back(Button_Ptr(new StatusButton()));
	}
	//---------------------------------------------------------------------------
	CTableBlock(const wstring& BlockName, const Rect& rectangle, IChartController* Controller)
		: controller(Controller),
		header(BlockName),
		rect(rectangle),
		headerHeight(TableObject::LINE_HEIGHT),
		fullView(true),
		Administrations(false)
	{
		rect.height = headerHeight;
		buttons.push_back(Button_Ptr(new StatusButton()));
	}
	~CTableBlock()
	{
		
	}
	//---------------------------------------------------------------------------
	void ConvertToAdministrations()
	{
		Administrations = true;
		buttons.push_back(Button_Ptr(new Button(L"добавить назначение")));	
	}

	size_t size() const
	{
		return objects.size();
	}
	//---------------------------------------------------------------------------
	const CTableObject_Ptr& operator[](size_t index) const
	{
		return objects.at(index);
	}
	//---------------------------------------------------------------------------
	void push_back(CTableObject_Ptr& TableObject)
	{
		objects.push_back(TableObject);
		std::sort(objects.begin(), objects.end(), [](const CTableObject_Ptr& rhs, const CTableObject_Ptr& lhs) { return *rhs < *lhs; });


		rect.height += TableObject->getRect().height;
	}
	//---------------------------------------------------------------------------
	void resize(const Rect& rectangle)
	{
		rect.x = rectangle.x;
		rect.y = rectangle.y;
		rect.width = rectangle.width;
		rect.reserved = rectangle.reserved;

		int h = headerHeight*4/5;
		buttons[0]->resize(Rect(rect.x+5, rect.y+headerHeight/2-h/2, h, h));
		if (buttons.size()==2)
			buttons[1]->resize(Rect(rect.x + 10 + h, rect.y + headerHeight / 2 - h / 2, 250, h));
		
		//все кроме высоты
		Rect r(rect);
		r.y += headerHeight;
		for (size_t i = 0; i < objects.size(); ++i)
		{
			if (i > 0)
			{
				const Rect temp_rect = objects[i-1]->getRect();
				r.y = temp_rect.y + temp_rect.height;
			}
			objects[i]->Resize(r);
		}
	}
	//---------------------------------------------------------------------------
	Rect getRect() const
	{
		if(fullView)
			return rect;
		else
		{
			Rect temp(rect);
			temp.height = headerHeight;
			return temp;
		}
	}
	//---------------------------------------------------------------------------
	virtual void OnPaint(UGC& ugc)
	{
		ugc.SetDrawColor(210, 210, 210);
		ugc.FillRectangle(rect.x, rect.y, rect.width, headerHeight);
		ugc.SetAlign(UGC::CENTER);
		ugc.SetDrawColor(0, 0, 0);
		ugc.SetTextSize(12);
		ugc.DrawString(header, rect.x + rect.width / 2, rect.y + headerHeight / 2 - ugc.GetTextHeight() / 2);
		
		for(Button_Ptr& button : buttons)
			button->OnDraw(ugc);
		
		if (fullView)
		{
			ugc.SetAlign(UGC::LEFT);

			for (const auto& obj : objects)
				obj->OnPaint(ugc);
		}
		ugc.SetDrawColor(0, 0, 0);
		ugc.DrawLine(rect.x, rect.y, rect.x + rect.width, rect.y, 1);
	}
	//---------------------------------------------------------------------------
	virtual bool OnLButtonUp(int x, int y)
	{
		if (buttons[0]->OnLButtonUp(x, y))
		{
			fullView = !fullView;
			controller->repaint();
			return true;
		}
		if (buttons.size() == 2 && buttons[1]->OnLButtonUp(x, y))
		{
			controller->addDrug();
			return true;
		}
		
		if(fullView)
		{
			for (auto& obj : objects)
				if (obj->OnLButtonUp(x, y))
						return true;	
		}
		return false;
	}
	//---------------------------------------------------------------------------
	virtual bool OnLButtonDown(int x, int y)
	{
		for (Button_Ptr& button : buttons)
		{
			if (button->OnLButtonDown(x, y))
				return true;
		}

		if (fullView)
		{
			for (auto& obj : objects)
				if (obj->OnLButtonDown(x, y))
					return true;
		}
		return false;
	}
	//---------------------------------------------------------------------------
	virtual bool OnMouseMove(int x, int y, bool& move_aborted)
	{
		for (Button_Ptr& button : buttons)
		{
			if (button->OnMouseMove(x, y))
				return true;
		}

		bool status = false;
		if (fullView)
		{
			for (auto& obj : objects)
				if (obj->OnMouseMove(x, y))
					status = true;
				else if (obj->OnMouseMoveAbort())
					move_aborted = true;
				
		}
		return status;
	}
};
