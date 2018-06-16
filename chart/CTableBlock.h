#pragma once

#include "TableLines.h"
#include "TableParameter.h"

#include "StatusButton.h"
#include "Button.h"
#include "MouseShift.h"
#include "MainBridge.h"
typedef shared_ptr<TableObject> CTableObject_Ptr;
typedef shared_ptr<Button> Button_Ptr;


class CTableBlock
{
protected:
	IChartController** controller;
	wstring header;
	vector<CTableObject_Ptr> objects;
	Rect rect;
	
	vector<Button_Ptr> buttons;
	int headerHeight;
	bool fullView;
	bool Administrations;
	MouseShift mouseShiftY;
public:
	CTableBlock()
		: controller (nullptr),
		header(L""),
		rect(Rect(0,0,1,1,1)),
		headerHeight (static_cast<int>(TableObject::LINE_HEIGHT*DPIX())),
		fullView(true),
		Administrations(false),
		mouseShiftY(0)
	{
		rect.height = headerHeight;
	}
	//---------------------------------------------------------------------------
	CTableBlock(const wstring& BlockName, const Rect& rectangle, IChartController** Controller)
		: controller(Controller),
		header(BlockName),
		rect(rectangle),
		headerHeight(static_cast<int>(TableObject::LINE_HEIGHT*DPIX())),
		fullView(true),
		Administrations(false),
		mouseShiftY(0)
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
		/* раскоментировать, если потребуется не создавать кнопки в режиме Просмотра
		if ((*controller)->MODE == ACCESS::VIEW_ACCESS) return;// если создан контроллер просмотра, то кнопки добавлять не надо
		*/
		if (button_type == BUTTON_TYPE::RESIZE)
			AddResizeButton();
		else if (button_type == BUTTON_TYPE::ADMINISTRATIONS /*&& (*controller)->MODE == ACCESS::FULL_ACCESS*/)
			AddAdministrationsButton();

		SetButtonsVisible();
	}
	//---------------------------------------------------------------------------
	void SetButtonsVisible()
	{
		auto size = buttons.size();
		if (size > 0)
			buttons[0]->SetVisible(((*controller)->MODE == ACCESS::FULL_ACCESS) || ((*controller)->MODE == ACCESS::NURSE_ACCESS));
		if(size==2)
			buttons[1]->SetVisible(((*controller)->MODE == ACCESS::FULL_ACCESS));
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
			(*controller)->repaint();
			
		};
	}
	//---------------------------------------------------------------------------
	void AddAdministrationsButton()
	{
		
		Administrations = true;
		buttons.push_back(Button_Ptr(new Button(L"добавить назначение")));	
		buttons[buttons.size()-1]->func = [this]() {
			
			(*controller)->addDrug();  };
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
	void insert(CTableObject_Ptr& TableObject)
	{
		rect.height += TableObject->getRect().height;
		if (!Administrations)
		{
			objects.push_back(TableObject);
			return;
		}

		auto temp_it = objects.end();
		for (auto it = objects.begin(); it != objects.end(); ++it)
		{
			if (**it == *TableObject)// если найден, идем дальше до конца
			{
				while (it != objects.end() && **it == *TableObject )
				{
					it++;
				}
				temp_it = it;
				break;
			}
			else if (temp_it == objects.end() && **it > *TableObject)
				temp_it = it;

		}
		
		objects.insert(temp_it, TableObject);

		//objects.push_back(TableObject);
		//std::sort(objects.begin(), objects.end(), [](const CTableObject_Ptr& rhs, const CTableObject_Ptr& lhs) { return *rhs < *lhs; });
		
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
		int adminway=-1;
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
protected:
	void DrawHeader(UGC& ugc)
	{
		ugc.SetDrawColor((*controller)->MODE == ACCESS::VIEW_ACCESS ? Gdiplus::Color::White : Gdiplus::Color::LightGray);
		ugc.FillRectangle(rect.x, rect.y, rect.width, headerHeight);
		ugc.SetAlign(UGC::CENTER);
		ugc.SetDrawColor(0, 0, 0);
		ugc.SetTextSize(12);
		ugc.DrawString(header, rect.x + rect.width / 2, rect.y + headerHeight / 2 - ugc.GetTextHeight() / 2);

		for (Button_Ptr& button : buttons)
			button->OnDraw(ugc);
	}

public:
	virtual void OnPaint(UGC& ugc)
	{
		DrawHeader(ugc);
		
		wstring name;
		if (fullView)
		{
			ugc.SetAlign(UGC::LEFT);

			for (size_t i=0; i<objects.size(); ++i)
			{
				const auto& obj = objects[i];
				// изменяем положение строки, если есть смещение
				if (i == mouseShiftY.getIndex())
				{
					Rect r(obj->getRect());
					r.y += mouseShiftY.getShift();
					if (r.y < rect.y+headerHeight)
						r.y = rect.y+ headerHeight;
					else if (r.y + r.height > rect.y + rect.height)
						r.y = rect.y + rect.height - r.height;
					obj->Resize(r);
					mouseShiftY.resetShift();
				}
				else
					obj->OnPaint(ugc);
				
				ugc.SetDrawColor(155, 155, 155);
				const Rect& r = obj->getRect();
				ugc.DrawLine(r.x, r.y + r.height, r.x + r.width, r.y + r.height);
				// рисуем название группы препаратов
				wstring way_name;
				MainBridge::getInstance().getAdminWayName(way_name, obj->getContainerUnit()->getAdminWay());
				if (name != way_name)
				{
					
					int temp = static_cast<int>(ugc.getDPIX() * 12);
					if((*controller)->MODE != ACCESS::VIEW_ACCESS) // серая полоса не нужна в Просмотре
					{
						ugc.SetDrawColor(200, 200, 200);
						ugc.FillRectangle(r.x, r.y - temp, r.x + rect.width, temp);
					}
					name = way_name;
					
					ugc.SetDrawColor(10, 10, 10);
					ugc.SetTextSize(8);
					ugc.SetBold(true);
					ugc.DrawString(name, r.x, r.y - static_cast<int>(ugc.getDPIX() * 13));
					ugc.SetBold(false);
					ugc.SetDrawColor(100,100,100);
					
				}
			}
		}
	
		// рисуем поверх перемещаемый объект
		int index = mouseShiftY.getIndex();
		if (index >= 0)
		{
			const auto& r = objects[index]->getRect();
			ugc.SetDrawColor(140, 255, 255, 255);
			ugc.FillRectangle(r.x, r.y, r.width, r.height);
			objects[index]->OnPaint(ugc);
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
			if (Administrations && mouseShiftY.getIndex()>=0)
			{
				//assignTableObjectPos();
				resize(rect);
				mouseShiftY.reset();
				(*controller)->repaint();
				return true;
			}
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
			if (Administrations && x < DPIX().getIntegerValue(8))
			{
				if (objects.size() < 1) return true;
				for (size_t i = 0; i < objects.size(); ++i)
				{
					if (objects[i]->IsThisObject(x, y) && (*controller)->MODE == ACCESS::FULL_ACCESS)
					{
						mouseShiftY.setStart(y, 0, i);
						return true;
					}
				}
			}
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
			if (x < DPIX().getIntegerValue(8))
			{
				(*controller)->SetMouseCursor(3);
			}
				if (Administrations && mouseShiftY.getIndex() >= 0)
				{
					(*controller)->SetMouseCursor(3);
					mouseShiftY.setEnd(y);
					assignTableObjectPos();
					/*if (y<rect.y || y>rect.y + rect.height)
					{
					mouseShiftY.reset();
					}*/

					return true;
				}
			
			
			for (auto& obj : objects)
			{
				if (obj->OnMouseMove(x, y))
					status = true;
				else if (obj->OnMouseMoveAbort())
					move_aborted = true;
			}
			
		}
		return status;
	}



	void assignTableObjectPos()
	{
		int index = mouseShiftY.getIndex();
		if (index < 0) return;
		auto index_r = objects[index]->getRect();
		
		
		for (size_t i = 0; i<objects.size(); ++i)
		{
			if (i == static_cast<size_t>(index))
				continue;
			const auto& r = objects[i]->getRect();
			if((mouseShiftY.getDirection() ==  1) && (index_r.y >= r.y && index_r .y<=r.y+r.height-DPIX().getIntegerValue(10)) || 
			   (mouseShiftY.getDirection() == -1) && (index_r.y+index_r.height >= r.y+DPIX().getIntegerValue(10) && index_r.y + index_r.height <= r.y + r.height))
			{
				swap(objects[i], objects[index]);
				mouseShiftY.setIndex((int)i);
				resize(rect);
				objects[i]->Resize(index_r);
				break;
			}
		}
		
		
	}
};
