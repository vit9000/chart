#pragma once
#include "TableObjectResizable.h"


class TableObjectMovable : public TableObjectResizable
{
public:
	TableObjectMovable(int ID, IChartController* Controller, const Rect& rectangle, const ContainerUnit* containerUnit)
		: TableObjectResizable(ID, Controller, rectangle, containerUnit)
	{}

	void OnPaint(UGC& ugc) override
	{
		ugc.SetDrawColor(155, 155, 155);
		ugc.FillRectangle(rect.x, rect.y, rect.reserved, rect.height);
		TableObject::OnPaint(ugc);

		double minuteW = static_cast<double>((rect.width - rect.reserved) / 1440.);
		int index = 0;
		ugc.SetTextSize(ValueFontSize);
		ugc.SetAlign(ugc.CENTER);
		for (const auto& unit : unitContainer->getUnits())
		{
			int x = rect.x + rect.reserved;
			x += static_cast<int>(unit.getStart()*minuteW);
			int duration = static_cast<int>(unit.getDuration()*minuteW);

			if (unitN == index)
				mouseShift.assignPosition(x, duration);



			ugc.SetDrawColor(155, 155, 155);


			int h = rect.height / 3;

			ugc.SetDrawColor(10, 10, 10);

			//wstringstream ss;
			//ss << unit.getValue() << L" " << unitContainer->getMeasureUnit();
			ugc.DrawNumber(unit.getValue(), x + duration / 2, rect.y + rect.height / 2 - ugc.GetTextHeight() / 2);
			//ugc.DrawString(ss.str(), x + duration / 2, rect.y + rect.height / 2 - ugc.GetTextHeight() / 2);


			index++;
		}
		ugc.SetAlign(ugc.LEFT);

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
				int action = MOVE;
				mouseShift.setStart(x, action);
			}
			return true;
		}
		return false;
	}

};