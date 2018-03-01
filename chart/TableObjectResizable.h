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
public:
	TableObjectResizable(const ID& id, IChartController* Controller, const Rect& rectangle, const ContainerUnit* containerUnit)
		: TableObject(id, Controller, rectangle, containerUnit),
		mouseShift(0), unitN(-1)

	{

	}



	void OnPaint(UGC& ugc) override
	{
		ugc.SetDrawColor(125, 160, 245);
		ugc.FillRectangle(rect.x, rect.y, rect.reserved, rect.height);

		TableObject::OnPaint(ugc);


		double minuteW = static_cast<double>((rect.width - rect.reserved) / (25.*60.));

		int index = 0;

		ugc.SetTextSize(ValueFontSize);
		
		for (const auto& unit : unitContainer->getUnits())
		{
			int x = rect.x + rect.reserved;
			x += static_cast<int>(unit.getStart()*minuteW);
			int duration = static_cast<int>(unit.getDuration()*minuteW);

			if (unitN == index)
				mouseShift.assignPosition(x, duration);

			ugc.SetDrawColor(155, 155, 245);

			DrawForm(ugc,unit.getValue(), x,rect.y,duration,rect.height);

			index++;
		}
		DrawSumm(ugc, minuteW);

	}

	

	bool OnLButtonUp(int x, int y) override
	{
		if (IsThisObject(x, y))
		{
			if (controller)
			{
				if (x > rect.x + rect.reserved)
				{
					if (mouseShift.is_action() && mouseShift.getShift() != 0)
					{
						// отправить запрос на обновление Юнита
						const Unit& unit = unitContainer->getUnit(unitN);
						double minuteW = static_cast<double>((rect.width - rect.reserved) / (60.*25.));
						int start = 0;
						int duration = 0;
						mouseShift.assignPosition(start, duration);
						mouseShift.reset();
						if (minuteW > 0)
						{
							start = static_cast<int>(start / minuteW);
							duration = static_cast<int>(duration / minuteW);
						}
						controller->updateUnitPosition(id, unitN, unit.getStart() + start, unit.getDuration() + duration);
						unitN = -1;
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
				//else
				//	controller->objectMouseUp(id);
				return true;
			}
		}
		return false;
	}
	bool OnLButtonDown(int x, int y) override
	{
		if (IsThisObject(x, y))
		{
			int minute = getMinuteByX(x);
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
				mouseShift.setStart(x, action);
			}
			return true;
		}
		return false;
	}
	bool OnMouseMove(int x, int y) override
	{
		if (IsThisObject(x, y))
		{
			mouseShift.setEnd(x);
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
	virtual void DrawForm(UGC& ugc, double value, int x, int y, int width, int height)
	{
		ugc.FillRectangle(x, y, width, height);
		ugc.SetDrawColor(235, 235, 255);
		for (int i = 2; i < height - 2; i += static_cast<int>(4 * ugc.getDPIX()))
		{
			ugc.FillRectangle(x + 1, rect.y + i, 1, 2);
			ugc.FillRectangle(x + width - 2, y + i, 1, 2);

		}
		ugc.SetAlign(ugc.CENTER);
		ugc.SetDrawColor(10, 10, 10);
		ugc.DrawNumber(value, x + width / 2, y + height / 2 - ugc.GetTextHeight() / 2);
		ugc.SetAlign(ugc.LEFT);

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



