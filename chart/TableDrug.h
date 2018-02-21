#pragma once

#include "CTableObject.h"

class TableDrug : public CTableObject
{
	protected:
	enum { MOVE=0, START, DURATION };
	class MouseShift
	{
		public:
		
		MouseShift(int x) : start_x(x), end_x(x), action(-1) {}
		int getShift() { if (action==-1) return 0; return end_x - start_x; }
		void setStart(int x, int action_) { start_x = x; end_x = x; action = action_; }
		void setEnd(int x) 
		{ 
			if(action!=-1)
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
	TableDrug(int ID, IChartController* Controller, const Rect& rectangle, const ContainerUnit* containerUnit)
		: CTableObject(ID, Controller, rectangle, containerUnit),
		mouseShift(0), unitN(-1)

	{
		
	}

	

	void OnPaint(UGC& ugc) override
	{
		ugc.SetDrawColor(165, 245, 155);
		ugc.FillRectangle(rect.x, rect.y, rect.reserved, rect.height);

		CTableObject::OnPaint(ugc);

		
		double minuteW = static_cast<double>((rect.Width() - rect.reserved) / 1440.);
		
		int index = 0;
		
		ugc.SetTextSize(10);
		ugc.SetAlign(ugc.CENTER);
		for (const auto& unit : unitContainer->getUnits())
		{
			int x = rect.x + rect.reserved;
			x += static_cast<int>(unit.getStart()*minuteW);
			int duration = static_cast<int>(unit.getDuration()*minuteW);
			
			if (unitN == index)
				mouseShift.assignPosition(x, duration);


			ugc.SetDrawColor(50, 160, 50);
			/*ugc.DrawRectangle(x,
				rect.y,
				duration,
				rect.height);*/
			
			ugc.SetDrawColor(165, 245, 155);
			
			ugc.FillRectangle(x,
				rect.y,
				duration,
				rect.height);

			ugc.SetDrawColor(50, 160, 50);

			int h = rect.height / 3;

			

			
			ugc.SetDrawColor(10, 10, 10);
			
			//wstringstream ss;
			//ss << unit.getValue() << L" " << unitContainer->getMeasureUnit();
			ugc.DrawNumber(unit.getValue(), x+ duration/2, rect.y+rect.height/2-ugc.GetTextHeight()/2);
			//ugc.DrawString(ss.str(), x + duration / 2, rect.y + rect.height / 2 - ugc.GetTextHeight() / 2);
			

			index++;
		}
		ugc.SetAlign(ugc.LEFT);

	}

	int getMinuteByX(int x)
	{
		x -= rect.reserved;
		double minute = (rect.Width() - rect.reserved) / 1440.;
		minute = x / minute;
		return minute;
	}

	bool OnLButtonUp(int x, int y) override
	{
		if (IsThisObject(x, y))
		{
			if (controller)
			{
				if (x > rect.x + rect.reserved)
				{
					if (mouseShift.is_action() && mouseShift.getShift()!=0)
					{
						// отправить запрос на обновление Юнита
						const Unit& unit = unitContainer->getUnit(unitN);
						double minuteW = static_cast<double>((rect.Width() - rect.reserved) / 1440.);
						int start = 0;
						int duration = 0;
						mouseShift.assignPosition(start, duration);
						mouseShift.reset();
						start /= minuteW;
						duration /= minuteW;
						controller->updateUnitPosition(id, unitN, unit.getStart()+start, unit.getDuration()+duration);
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
				else
					controller->objectMouseUp(id);
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
};
