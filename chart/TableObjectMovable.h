#pragma once
#include "TableObjectResizable.h"


class TableObjectMovable : public TableObjectResizable
{
public:
	TableObjectMovable(IChartController** Controller, const ContainerUnit* containerUnit)
		: TableObjectResizable(Controller, containerUnit)
	{}

	void OnPaint(UGC& ugc) override
	{
		//ugc.SetDrawColor(155, 155, 155);
		//ugc.FillRectangle(rect.x, rect.y, rect.reserved, rect.height);
		AnimateHighlight(ugc);
		TableObject::OnPaint(ugc);

		int STEP = config->getStep();
		double minuteW = static_cast<double>((rect.width - rect.reserved) / ((static_cast<double>(config->getCountSteps()) + 1.)*STEP));
		int unit_index = 0;
		ugc.SetTextSize(UnitTextSize);
		ugc.SetAlign(ugc.CENTER);
		ugc.SetBold(true);
		for (const auto& unit : unitContainer->getUnits())
		{
			unit_index = unit.getStart();
			int x = rect.x + rect.reserved;
			x += static_cast<int>(unit.getStart()*minuteW);
			int duration = static_cast<int>(unit.getDuration()*minuteW);

			if (unitN == unit_index)
				mouseShift.assignPosition(x, duration);

			int h = rect.height / 3;
			if ((*controller)->MODE == ACCESS::VIEW_ACCESS)
			{
				int two = DPIX()(2);
				ugc.SetDrawColor(0,0,0);
				ugc.DrawRectangle(x+two, rect.y+two, duration-2*two, rect.height - 2*two);
			}
			else
			{
				ugc.SetDrawColor(color);
				ugc.FillRectangle(x, rect.y + 1, duration, rect.height - 1);
				ugc.SetDrawColor(255, 255, 255);
			}
			
			
			ugc.DrawString(unit.getValue(), x + duration / 2, rect.y + rect.height / 2 - ugc.GetTextHeight() / 2);
			
			// рисуем статус - если назначение выполнено
			if (unit.isCompleted())
			{
				DrawCompletedStatus(ugc, x, rect.y, duration);
			}
		}
		ugc.SetBold(false);

		DrawSumm(ugc, minuteW);
		ugc.SetAlign(ugc.LEFT);

		TableObjectResizable::DrawColorMark(ugc);

	}

	
	bool OnLButtonDown(int x, int y) override
	{
		if (IsThisObject(x, y))
		{
			if (button && button->OnLButtonDown(x, y))
				return true;

			int minute = getMinuteByX(x);
			unitN = unitContainer->find(minute);
			if (unitN >= 0 && (*controller)->MODE == ACCESS::FULL_ACCESS)
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


	int getAction(int x, int y) override
	{
		int minute = getMinuteByX(x - mouseShift.getShift());
		unitN = unitContainer->find(minute);
		if (unitN >= 0)	
			return MOVE;
		return -1;
	}
};