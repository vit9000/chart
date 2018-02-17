#pragma once

#include "CTableObject.h"

class TableDrug : public CTableObject
{
public:
	TableDrug(int ID, IChartController* Controller, const Rect& rectangle, const ContainerDrug* containerUnit)
		: CTableObject(ID, Controller, rectangle, containerUnit)

	{
		
	}

	void OnPaint(UGC& ugc) override
	{
		ugc.SetDrawColor(100, 100, 0, 0);
		ugc.FillRectangle(rect.x, rect.y, rect.reserved, rect.height);

		CTableObject::OnPaint(ugc);

		ugc.SetTextSize(10);
		double minuteW = static_cast<double>((rect.Width() - rect.reserved) / 1440.);
		for (const auto& unit : unitContainer->getUnits())
		{
			int x = rect.x + rect.reserved;
			x += static_cast<int>(unit.getStart()*minuteW);
			int duration = static_cast<int>(unit.getDuration()*minuteW);
			ugc.SetDrawColor(195, 0, 0);
			ugc.FillRectangle(x,
				rect.y,
				duration,
				rect.height);
			ugc.SetDrawColor(0, 0, 0);
			ugc.DrawNumber(unit.getValue(), x, rect.y);
			ugc.DrawRectangle(x,
				rect.y,
				duration,
				rect.height);
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
					double rW = (rect.Width() - rect.reserved) / 1440.;
					rW = x / rW;
					controller->addDrugUnit(id, static_cast<int>(rW));
				}
				else
					controller->objectMouseUp(id);
				return true;
			}
		}
		return false;
	}
};
