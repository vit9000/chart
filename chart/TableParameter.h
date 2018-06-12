#pragma once

#include "TableObject.h"

class TableParameter : public TableObject
{
public:
	TableParameter(IChartController** Controller, const ContainerUnit* containerUnit)
		: TableObject(Controller, containerUnit)
	{}

	void OnPaint(UGC& ugc) override
	{
		TableObject::OnPaint(ugc);

		ugc.SetTextSize(ValueFontSize);
		double minuteW = static_cast<double>((rect.width - rect.reserved) / (25.*60.));
		ugc.SetDrawColor(0, 0, 0);
		ugc.SetAlign(UGC::CENTER);
		int duration = static_cast<int>(60.*minuteW);
		for (const auto& unit : unitContainer->getUnits())
		{
			int x = rect.x + rect.reserved;
			x += static_cast<int>(unit.getStart()*minuteW);
			//int duration = static_cast<int>(unit.getDuration()*minuteW);		
			ugc.DrawString(unit.getValue(), x+duration/2, rect.y+rect.height/2 - ugc.GetTextHeight()/2);
		}

		DrawSumm(ugc, minuteW);
		ugc.SetAlign(UGC::LEFT);
	}

	bool OnLButtonUp(int x, int y) override
	{
		if (IsThisObject(x, y))
		{
			if (controller)
			{
				if (x > rect.x + rect.reserved)
				{
					x = x-rect.reserved-rect.x;
					double bitW = (rect.width - rect.reserved) / 25.;
					int minute = static_cast<int>(x / bitW * 60);
					if (minute > 1440) return false;
					int unitN = unitContainer->find(minute);

					int cellN = minute / 60;
					Rect r(rect.x + rect.reserved+ static_cast<int>(cellN*bitW), rect.y, static_cast<int>(bitW), rect.height);
					
					if (unitN >= 0)
						(*controller)->updateUnitValue(id, unitN, r);
					else
						(*controller)->addParameterUnit(id, minute/60*60, r);
				}
				else
					(*controller)->objectMouseUp(id);
				return true;
			}
		}
		return false;
	}
};
