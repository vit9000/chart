#pragma once
#include <thread>
#include <mutex>
using namespace std;
#include "TableObject.h"
#include "MouseShift.h"
#include "MainBridge.h"

class TableObjectResizable : public TableObject
{
protected:
	enum { MOVE = 0, START, DURATION };
	MouseShift mouseShift;
	int unitN;
	int highlight;
public:
	TableObjectResizable(IChartController** Controller,  const ContainerUnit* containerUnit)
		: TableObject(Controller, containerUnit),
		mouseShift(0), unitN(-1), highlight(MainBridge::getInstance().isLoading() ? 0 : 90)
	{
		rect.height = getDefaultHeight();
	}

	int getDefaultHeight() const override
	{
		return static_cast<int>(LINE_HEIGHT * DPIX() * 1.5);
	}

	void OnPaint(UGC& ugc) override
	{
		AnimateHighlight(ugc);
		//ugc.SetDrawColor(125, 160, 245);
		//ugc.FillRectangle(rect.x, rect.y, rect.reserved, rect.height);

		TableObject::OnPaint(ugc);

		int STEP = config->getStep();
		double minuteW = static_cast<double>((rect.width - rect.reserved) / ((static_cast<double>(config->getCountSteps()) + 1.)*STEP));

		
		ugc.SetTextSize(ValueFontSize);
		ugc.SetBold(true);

		int unit_index = 0;
		for (const auto& unit : unitContainer->getUnits())
		{
			unit_index = unit.getStart();
			int x = rect.x + rect.reserved;
			x += static_cast<int>(unit.getStart()*minuteW);
			int duration = static_cast<int>(unit.getDuration()*minuteW);

			if (unitN == unit_index)
				mouseShift.assignPosition(x, duration);
			
			ugc.SetDrawColor(color);
			ugc.SetTextSize(ValueFontSize);

			
			wstring value = unit.getValue();
			int textsizetemp = ValueFontSize;
			while (ugc.GetTextWidth(value) > duration)
			{
				textsizetemp-=2;
				ugc.SetTextSize(textsizetemp);
				if (textsizetemp <= 8) break;
			}
			
			//if (unit.isCompleted())
			//	ugc.SetDrawColor(150, 150, 150);
			DrawForm(ugc, value, x, rect.y,duration,rect.height);
			// рисуем статус - если назначение выполнено
			if (unit.isCompleted())
			{
				DrawCompletedStatus(ugc, x, rect.y, duration);
				//
			}


			if (child_objects.size() > 0) // рисуем фигурную скобку юнитов
			{
				ugc.SetDrawColor(0,0,0);
				int bitW = static_cast<int>(2 * ugc.getDPIX());
				
				int start = 0;
				if (unit.getValue().getDoubleValue() <= 0)// ищем первое не нулевое значение
				{
					for (start; start < static_cast<int>(child_objects.size()); start++)
					{
						const auto& val = child_objects[start]->getContainerUnit()->getUnit(unit_index).getValue();
						if (val.getDoubleValue() > 0)
							break;
					}
					start++;
				}
			
				int end = static_cast<int>(child_objects.size() - 1); // ищем последнее ненулевое значений
				for (end; end >= 0; end--)
				{
					try
					{
						const auto& val = child_objects[end]->getContainerUnit()->getUnit(unit_index).getValue();
						if (val.getDoubleValue() > 0)
							break;
					}
					catch (...) {}

				}
				int count = end-start+2;// количество строк для объединения
				if(count>1)
					ugc.DrawUnitedForm(x+bitW, rect.y + bitW + rect.height*start, bitW * 2, getDefaultHeight()*count - bitW * 2, 2);
			}
		}
		ugc.SetBold(false);
		DrawSumm(ugc, minuteW);

		if (button)
			button->OnDraw(ugc);

		if(!unitContainer->isChild())
			DrawColorMark(ugc);
	}

	//---------------------------------------------------------------------
	void DrawCompletedStatus(UGC& ugc, int x, int y, int width)
	{
		
		
		int d = static_cast<int>(12 * ugc.getDPIX());
		int two = static_cast<int>(2 * ugc.getDPIX());
		x = x + width - d;


		((*controller)->MODE == ACCESS::VIEW_ACCESS) ? ugc.SetDrawColor(0, 0, 0) : ugc.SetDrawColor(0, 255, 0);
		ugc.FillEllipse(x, rect.y, d);
			ugc.SetDrawColor(255, 255, 255);
		
		d -= two * 2;
		x += two;
		//ugc.DrawLineAntialiased
		ugc.DrawLineAntialiased(x, y + d / 2, x + d / 2, y + d+two/2, 2);
		ugc.DrawLineAntialiased(x + d / 2, y + d+two/2, x + d, y+two, 2);

	}

	void DrawColorMark(UGC& ugc)
	{
		if ((*controller)->MODE == ACCESS::VIEW_ACCESS) return;
		if (unitContainer->isChild() || child_objects.size()>0) return;
		//if (child_objects.size() > 0) return;


		ugc.SetDrawColor(color);
		

		int bitW = static_cast<int>(2 * ugc.getDPIX());
		bool tick = false;
		for (int y = rect.y + bitW; y < rect.y + rect.height - bitW; y += bitW)
		{
			int xi = static_cast<int>(tick ? bitW * 1.5 : 0);
			ugc.FillRectangle(rect.x + xi + bitW, y, bitW, bitW);
			tick = !tick;
		}

		/*
		int h = rect.height / 3;
		
		int bitW = static_cast<int>(2 * ugc.getDPIX());
		int c = h / bitW;
		h = c*bitW;
		
		int temp = static_cast<int>(c/1.5);
	
		for (int i = 0; i<static_cast<int>(c/1.5); i++)
		{
			for (int j = 0; j<temp; j++)
				ugc.FillRectangle(rect.x + static_cast<int>(bitW*j*1.5)+ bitW, 
									rect.y + bitW + i*static_cast<int>(bitW*1.5), 
									bitW, bitW);
			temp--;
		}
		
		*/

	}
	//---------------------------------------------------------------------
protected:
	void getPosition(int& start, int& duration)
	{
		int STEP = config->getStep();
		double minuteW = static_cast<double>((rect.width - rect.reserved) / ((static_cast<double>(config->getCountSteps()) + 1.)*STEP));
		mouseShift.assignPosition(start, duration);
		mouseShift.reset();
		if (minuteW > 0)
		{
			start = static_cast<int>(start / minuteW);
			duration = static_cast<int>(duration / minuteW);
		}
		const Unit& unit = unitContainer->getUnit(unitN);
		start += unit.getStart();
		duration += unit.getDuration();
	}
public:
	bool OnRButtonUp(int x, int y) override
	{
		if (IsThisObject(x, y))
		{
			if (x < rect.reserved) // заголовок
			{
				if (!controller) return true;
				(*controller)->lineSmartMenu(x, y, id);
				return true;
			}

			int temp = unitN;
			int action = getAction(x, y);
			if (action >= 0)
				ShowSmartMenu(x,y, unitN);
			unitN = temp;
			return true;
		}
		return false;
	}

	virtual void ShowSmartMenu(int x, int y, int uN)
	{
		if (!controller) return;
		vector<pair<wstring, function<void()>>> info;
		const DrugInfo& drugInfo = unitContainer->getDrugInfo();
		info.push_back(make_pair(drugInfo.name, nullptr));
		info.push_back(make_pair(unitContainer->getUnitDetails(uN), nullptr));
	
		(*controller)->showSmartMenu(x, y, id, uN, info);
	}

	bool OnLButtonUp(int x, int y) override
	{
		if (IsThisObject(x, y))
		{
			highlight = 0;

			if (button && button->OnLButtonUp(x, y))
				return true;
			if (controller && (*controller))
			{
				if (x > rect.x + rect.reserved)
				{
					if (mouseShift.is_action() && mouseShift.getShift() != 0)
					{
						// отправить запрос на обновление Юнита
						int start(0), duration(0);
						getPosition(start, duration);
						(*controller)->updateUnitPosition(id, unitN, start, duration);
						
					}
					else
					{
						mouseShift.reset();
						if (unitN >= 0)
							(*controller)->updateDrugUnitValue(id, unitN);
						else
							(*controller)->addDrugUnit(id, static_cast<int>(getMinuteByX(x)));
					}

				}
				unitN = -1;
				
				//else
				//	controller->objectMouseUp(id);
				return true;
			}
		}
		return false;
	}

	virtual int getAction(int x, int y)
	{
		int minute = getMinuteByX(x- mouseShift.getShift());
		unitN = unitContainer->find(minute);
		if (unitN >= 0)
		{
			const auto& unit = unitContainer->getUnit(unitN);
			minute -= unit.getStart();
			int border = unit.getDuration() / 3;
			if (border > 20) border = 20;
			int action = MOVE;
			if (minute < border)
				action = START;
			else if (minute > unit.getDuration() - border)
				action = DURATION;
			return action;
		}
		return -1;
		
	}

	bool OnLButtonDown(int x, int y) override
	{
		if (IsThisObject(x, y))
		{
			if (button && button->OnLButtonDown(x, y))
				return true;
			int action = getAction(x, y);
			if (action >= 0 && (*controller)->MODE == ACCESS::FULL_ACCESS)
				mouseShift.setStart(x, action);
			
		
			return true;
		}
		return false;
	}
	bool OnMouseMove(int x, int y) override
	{
		if (IsThisObject(x, y))
		{
			//if(mouseShift.getAction()>=0)
			mouseShift.setEnd(x);

			int temp = unitN;
			int action = getAction(x, y);
			if (action >= 0)
				(*controller)->SetMouseCursor(action);
			unitN = temp;
			
			return mouseShift.is_action();
			
		}
		
		return false;
	}

	bool OnMouseMoveAbort() override
	{
		if (mouseShift.is_action())
		{
			mouseShift.reset();
			return true;
		}
		return false;
	}
protected:
	virtual void DrawForm(UGC& ugc, const wstring& value, int x, int y, int width, int height)
	{
		int aX = 0;
		if (child_objects.size() > 0 || unitContainer->isChild())
			aX = static_cast<int>(7 * ugc.getDPIX());

		
		if (Value(value).getDoubleValue() > 0)  
		{
			
			int f = static_cast<int>(1 * ugc.getDPIX());
			if ((*controller)->MODE == ACCESS::VIEW_ACCESS)
			{
				int d = f * 2;
				ugc.SetDrawColor(255, 255, 255);
				ugc.FillRectangle(x + aX + d, y + d, width - aX - 2 * d, height - 2 * d);
				ugc.SetDrawColor(0, 0, 0);
				ugc.DrawDropsShape(x + aX + d, y + d, width - aX - 2 * d, height - 2 * d);
			}
			else
				ugc.FillDropsShape(x + aX, y + f, width - aX, height - f);

			x += aX;
			width -= aX;

			((*controller)->MODE == ACCESS::VIEW_ACCESS) ? ugc.SetDrawColor(0, 0, 0) : ugc.SetDrawColor(255, 255, 255);
			if (width < int(height*0.75))
				ugc.DrawVerticalString(value, x + width / 2 - ugc.GetTextHeight() / 2, y + height / 2 + ugc.GetTextWidth(value) / 2);
			else
			{
				ugc.SetAlign(ugc.CENTER);
				ugc.DrawString(value, x + width / 2, y + height / 2 - ugc.GetTextHeight() / 2);
				ugc.SetAlign(ugc.LEFT);
			}
		}

		

	}
	int getMinuteByX(int x)
	{
		x = x - rect.reserved - rect.x;
		int STEP = config->getStep();
		double maxmin = (static_cast<double>(config->getCountSteps()) + 1.)*STEP;
		double minute = (rect.width - rect.reserved) / maxmin;
		minute = x / minute;
		if (minute > maxmin) return -1;
		return static_cast<int>(minute);
	}

	void AnimateHighlight(UGC& ugc)
	{
		if (highlight > 0)
		{
			ugc.SetDrawColor(highlight, 0, 255, 0);
			ugc.FillRectangle(rect.x, rect.y, rect.width, rect.height);
			highlight -= 10;
			thread t([this]() {
				std::this_thread::sleep_for(200ms);
				if(highlight>0)
					(*controller)->repaint(); });
			t.detach();
		}
	}
};



