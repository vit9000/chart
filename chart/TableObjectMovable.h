#pragma once
#include "TableObjectResizable.h"


class TableObjectMovable : public TableObjectResizable
{
public:
	TableObjectMovable(const ID& id, IChartController* Controller, const ContainerUnit* containerUnit)
		: TableObjectResizable(id, Controller, containerUnit)
	{}

	void OnPaint(UGC& ugc) override
	{
		ugc.SetDrawColor(155, 155, 155);
		ugc.FillRectangle(rect.x, rect.y, rect.reserved, rect.height);
		TableObject::OnPaint(ugc);

		double minuteW = static_cast<double>((rect.width - rect.reserved) / (25.*60.));
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

			ugc.DrawString(unit.getValue(), x + duration / 2, rect.y + rect.height / 2 - ugc.GetTextHeight() / 2);
			

			index++;
		}

		DrawSumm(ugc, minuteW);
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