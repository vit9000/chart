#pragma once

#include "TableLines.h"
#include "TableParameter.h"

#include "StatusButton.h"
#include "Button.h"
typedef shared_ptr<TableObject> CTableObject_Ptr;
typedef shared_ptr<Button> Button_Ptr;


class CTableBlock
{
protected:
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
		headerHeight (static_cast<int>(TableObject::LINE_HEIGHT*DPIX())),
		fullView(true),
		Administrations(false)
	{
		rect.height = headerHeight;
	}
	//---------------------------------------------------------------------------
	CTableBlock(const wstring& BlockName, const Rect& rectangle, IChartController* Controller)
		: controller(Controller),
		header(BlockName),
		rect(rectangle),
		headerHeight(static_cast<int>(TableObject::LINE_HEIGHT*DPIX())),
		fullView(true),
		Administrations(false)
	{
		rect.height = headerHeight;
		
	}

	~CTableBlock()
	{
		
	}
	//---------------------------------------------------------------------------
	enum class BUTTON_TYPE{RESIZE, ADMINISTRATIONS};
	void AddButton(const BUTTON_TYPE& button_type)
	{
		if (button_type == BUTTON_TYPE::RESIZE)
			AddResizeButton();
		else if (button_type == BUTTON_TYPE::ADMINISTRATIONS)
			AddAdministrationsButton();
	}
	//---------------------------------------------------------------------------
	CTableObject_Ptr getTableObject(const ID& id)
	{
		for (CTableObject_Ptr& obj : objects)
		{
			if (obj->getID() == id)
				return obj;
		}
		return nullptr;
	}
private:
	void AddResizeButton()
	{
		buttons.push_back(Button_Ptr(new StatusButton()));
		buttons[buttons.size()-1]->func = [this]() {
			fullView = !fullView;
			controller->repaint();
			
		};
	}
	//---------------------------------------------------------------------------
	void AddAdministrationsButton()
	{
		
		Administrations = true;
		buttons.push_back(Button_Ptr(new Button(L"добавить назначение")));	
		buttons[buttons.size()-1]->func = [this]() {
			
			controller->addDrug();  };
	}
public:
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
	virtual void resize(const Rect& rectangle)
	{
		rect.x = rectangle.x;
		rect.y = rectangle.y;
		rect.width = rectangle.width;
		rect.reserved = rectangle.reserved;
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		int h = headerHeight*4/5;

		DPIX dpix;
		
		int border = static_cast<int>(5.*dpix);
		if (buttons.size() >= 1)
			buttons[0]->resize(Rect(rect.x+ border, rect.y+headerHeight/2-h/2, h, h));
		if (buttons.size()>=2)
			buttons[1]->resize(Rect(rect.x + border*2 + h, rect.y + headerHeight / 2 - h / 2, dpix.getIntegerValue(250.), h));
		
		//все кроме высоты
		Rect r(rect);
		r.y += headerHeight;
		rect.height = headerHeight;
		int adminway=0;
		for (size_t i = 0; i < objects.size(); ++i)
		{
			

			if (i > 0)
			{
				const Rect temp_rect = objects[i-1]->getRect();
				r.y = temp_rect.y + temp_rect.height;
				
			}
			if (Administrations && adminway != objects[i]->getContainerUnit()->getAdminWay())
			{
				adminway = objects[i]->getContainerUnit()->getAdminWay();
				int temp = dpix.getIntegerValue(12);
				r.y += temp;
				rect.height += temp;
			}
			objects[i]->Resize(r);
			rect.height+= objects[i]->getRect().height;
			
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
		ugc.SetDrawColor(Gdiplus::Color::LightGray);
		ugc.FillRectangle(rect.x, rect.y, rect.width, headerHeight);
		ugc.SetAlign(UGC::CENTER);
		ugc.SetDrawColor(0, 0, 0);
		ugc.SetTextSize(12);
		ugc.DrawString(header, rect.x + rect.width / 2, rect.y + headerHeight / 2 - ugc.GetTextHeight() / 2);
		
		for(Button_Ptr& button : buttons)
			button->OnDraw(ugc);
		
		wstring name;
		if (fullView)
		{
			ugc.SetAlign(UGC::LEFT);

			for (const auto& obj : objects)
			{
				obj->OnPaint(ugc);
				ugc.SetDrawColor(155, 155, 155);
				const Rect& r = obj->getRect();
				ugc.DrawLine(r.x, r.y + r.height, r.x + r.width, r.y + r.height);
				// рисуем название группы препаратов
				if (name != obj->getContainerUnit()->getAdminWayName())
				{
					
					int temp = static_cast<int>(ugc.getDPIX() * 12);
					ugc.SetDrawColor(200, 200, 200);
					ugc.FillRectangle(r.x, r.y-temp, r.x + rect.width, temp);
					//ugc.SetDrawColor(255, 255, 255);
					name = obj->getContainerUnit()->getAdminWayName();
					//obj->SetSpecColor(ugc);
					ugc.SetDrawColor(10, 10, 10);
					ugc.SetTextSize(8);
					ugc.SetBold(true);
					ugc.DrawString(name, r.x, r.y - static_cast<int>(ugc.getDPIX() * 13));
					ugc.SetBold(false);
					ugc.SetDrawColor(100,100,100);
					
				}
			}
		}
		
		ugc.SetDrawColor(100, 100, 100);
		ugc.DrawLine(rect.x, rect.y+headerHeight, rect.x + rect.width, rect.y+ headerHeight, 1);
		ugc.SetDrawColor(0, 0, 0);
		
		ugc.DrawLine(rect.x, rect.y, rect.x + rect.width, rect.y, 1);
	}
	//---------------------------------------------------------------------------
	virtual bool OnRButtonUp(int x, int y)
	{
		if (fullView)
		{
			for (auto& obj : objects)
				if (obj->OnRButtonUp(x, y))
					return true;
		}
		return false;
	}
	//---------------------------------------------------------------------------
	virtual bool OnLButtonUp(int x, int y)
	{
		for (Button_Ptr& button : buttons)
		{
			if (button->OnLButtonUp(x, y))
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
