#pragma once

#include "CTableObject.h"

class TableParameter : public CTableObject
{
public:
	TableParameter(int ID, IChartController* Controller, const Rect& rectangle, const ContainerParameter* containerUnit)
		: CTableObject(ID, Controller, rectangle, containerUnit)

	{

	}

	void OnPaint(UGC& ugc) override
	{
		//ugc.SetDrawColor(100, 245, 245, 155);
		//ugc.FillRectangle(rect.x, rect.y, rect.reserved, rect.height);

		CTableObject::OnPaint(ugc);

		ugc.SetTextSize(10);
		double minuteW = static_cast<double>((rect.Width() - rect.reserved) / 1440.);
		for (const auto& unit : unitContainer->getUnits())
		{
			int x = rect.x + rect.reserved;
			x += static_cast<int>(unit.getStart()*minuteW);
			int duration = static_cast<int>(unit.getDuration()*minuteW);
			ugc.SetDrawColor(100, 245, 245, 155);
			ugc.FillRectangle(x,
				rect.y,
				duration,
				rect.height);
			ugc.SetDrawColor(0, 0, 0);
			ugc.DrawNumber(unit.getValue(), x, rect.y);
		}

	}

	bool OnLButtonUp(int x, int y) override
	{
		if (IsThisObject(x, y))
		{
			if (controller)
			{
				if (x > rect.x + rect.reserved)
				{
					x -= rect.reserved;
					double bitW = (rect.Width() - rect.reserved) / 24.;
					int minute = static_cast<int>(x / bitW) * 60;
					int unitN = unitContainer->find(minute);
					if (unitN >= 0)
						controller->updateUnitValue(id, unitN);
					else
						controller->addParameterUnit(id, static_cast<int>(minute));
				}
				else
					controller->objectMouseUp(id);
				return true;
			}
		}
		return false;
	}
};
