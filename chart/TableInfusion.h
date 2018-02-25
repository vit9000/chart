#pragma once
#include"TableDrug.h"

class TableInfusion : public TableDrug
{
public:
	TableInfusion(int ID, IChartController* Controller, const Rect& rectangle, const ContainerUnit* containerUnit)
		: TableDrug(ID, Controller, rectangle, containerUnit)
	{}

	void OnPaint(UGC& ugc) override
	{
		ugc.SetDrawColor(245, 155, 155);
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


			

			ugc.SetDrawColor(245, 155, 155);

			ugc.FillRectangle(x,
				rect.y,
				duration-6,
				rect.height);
			ugc.FillTriangle(x + duration - 7, rect.y,
				x + duration, rect.y + rect.height / 2,
				x + duration - 7, rect.y + rect.height);

			

			int h = rect.height / 3;

			

			ugc.SetDrawColor(10, 10, 10);

			ugc.DrawNumber(unit.getValue(), x + duration / 2, rect.y + rect.height / 2 - ugc.GetTextHeight() / 2);
			

			index++;
		}
		ugc.SetAlign(ugc.LEFT);

	}


	
};