#pragma once

#include "TableObject.h"

class TableObjectResizable : public TableObject
{
protected:
	enum { MOVE = 0, START, DURATION };
	class MouseShift
	{
	public:

		MouseShift(int x) : start_x(x), end_x(x), action(-1) {}
		int getShift() { if (action == -1) return 0; return end_x - start_x; }
		void setStart(int x, int action_) { start_x = x; end_x = x; action = action_; }
		void setEnd(int x)
		{
			if (action != -1)
				end_x = x;
		}
		void reset() { start_x = 0; end_x = 0; action = -1; }
		bool is_action() { return action != -1; }
		int getAction() { return action; }

		void assignPosition(int& start, int& duration)
		{
			switch (getAction())
			{

			case START:
				duration -= getShift();
			case MOVE:
				start += getShift();
				break;
			case DURATION:
				duration += getShift();
			}
		}



		int start_x;
		int end_x;
		int action;
	} mouseShift;
	int unitN;
	bool FillRectangle;
	Gdiplus::Color color;
public:
	TableObjectResizable(IChartController* Controller,  const ContainerUnit* containerUnit)
		: TableObject(Controller, containerUnit),
		mouseShift(0), unitN(-1),
		color(255,0,0),
		FillRectangle(false)
	{
		rect.height = getDefaultHeight();
	}

	int getDefaultHeight()
	{
		return static_cast<int>(LINE_HEIGHT * DPIX() * 1.5);
	}

	void OnPaint(UGC& ugc) override
	{
		//ugc.SetDrawColor(125, 160, 245);
		//ugc.FillRectangle(rect.x, rect.y, rect.reserved, rect.height);

		TableObject::OnPaint(ugc);


		double minuteW = static_cast<double>((rect.width - rect.reserved) / (25.*60.));

		int index = 0;

		ugc.SetTextSize(ValueFontSize);
		ugc.SetBold(true);
		for (const auto& unit : unitContainer->getUnits())
		{
			int x = rect.x + rect.reserved;
			x += static_cast<int>(unit.getStart()*minuteW);
			int duration = static_cast<int>(unit.getDuration()*minuteW);

			if (unitN == index)
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
			DrawForm(ugc, value, x,rect.y,duration,rect.height);

			index++;
		}
		ugc.SetBold(false);
		DrawSumm(ugc, minuteW);

		if (button)
			button->OnDraw(ugc);

		DrawColorMark(ugc);
	}

	//---------------------------------------------------------------------
	void DrawColorMark(UGC& ugc)
	{
		ugc.SetDrawColor(color);
		int bitW = static_cast<int>(2 * ugc.getDPIX());
		bool tick = false;
		for (int i = bitW; i < rect.height-bitW; i += bitW)
		{
			int shift_x = static_cast<int>((tick) ? bitW*1.5 : 0);
			ugc.FillRectangle(rect.x + bitW + shift_x, rect.y + i, bitW, bitW);
			tick = !tick;
		}
	}
	//---------------------------------------------------------------------
protected:
	void getPosition(int& start, int& duration)
	{
		double minuteW = static_cast<double>((rect.width - rect.reserved) / (60.*25.));
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
	bool OnLButtonUp(int x, int y) override
	{
		if (IsThisObject(x, y))
		{
			if (button && button->OnLButtonUp(x, y))
				return true;
			if (controller)
			{
				if (x > rect.x + rect.reserved)
				{
					if (mouseShift.is_action() && mouseShift.getShift() != 0)
					{
						// отправить запрос на обновление Юнита
						int start(0), duration(0);
						getPosition(start, duration);
						controller->updateUnitPosition(id, unitN, start, duration);
						
					}
					else
					{
						mouseShift.reset();
						if (unitN >= 0)
							controller->updateUnitValue(id, unitN);
						else
							controller->addDrugUnit(id, static_cast<int>(getMinuteByX(x)));
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
			
				int action = getAction(x, y);
				if(action>=0)
					mouseShift.setStart(x, action);
		
			return true;
		}
		return false;
	}
	bool OnMouseMove(int x, int y) override
	{
		if (IsThisObject(x, y))
		{
			mouseShift.setEnd(x);
			int temp = unitN;
			int action = getAction(x, y);
			if (action >= 0)
					controller->SetMouseCursor(action);
			unitN = temp;
			
			return true;
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
		if(FillRectangle)
			ugc.FillRectangle(x, y, width, height);
		else
			ugc.FillDropsShape(x, y, width, height);
		
		ugc.SetDrawColor(255, 255, 255);
		if (Value(value).getDoubleValue() > 0)
		{
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
		double maxmin = 60.*25.;
		double minute = (rect.width - rect.reserved) / maxmin;
		minute = x / minute;
		if (minute > maxmin) return -1;
		return static_cast<int>(minute);
	}
};



