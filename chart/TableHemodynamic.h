#pragma once
#include "TableParameter.h"
#include "ModelContainers.h"

class TableHemodynamic : public TableParameter
{
public:
	TableHemodynamic(const ID& id, IChartController* Controller, const ContainerUnit* containerUnit)
		: TableParameter(id, Controller, containerUnit)
	{
		rect.height *= 5;
	}

	void OnPaint(UGC& ugc) override
	{
		ugc.SetTextSize(10);
		double minutePX = static_cast<double>((rect.width - rect.reserved) / (60.*25.));
		double bpPX = static_cast<double>(rect.height / 200.);
		ugc.SetDrawColor(155, 155, 155);
		int y_bottom = rect.y + rect.height;
		ugc.SetAlign(UGC::RIGHT);
		int text_height = ugc.GetTextHeight();
		for (int i = 20; i < 200; i+=20)
		{
			int yi = y_bottom - static_cast<int>(bpPX*i);
			ugc.DrawLine(rect.reserved+rect.x, yi, rect.x+rect.width, yi);

			ugc.DrawNumber(i, rect.reserved + rect.x - 2, yi - text_height / 2);
		}
		ugc.SetAlign(UGC::LEFT);
		int bitW = static_cast<int>(bpPX*16.);
		if (const ContainerHemodynamic* containerHemodynamic = dynamic_cast<const ContainerHemodynamic*>(unitContainer))
		{
			for (const auto& unit : containerHemodynamic->getUnits())
			{
				const auto& value = unit.getValue().getDoubleValues();
				for(int i=0; i<static_cast<int>(value.size()); ++i)
				{
					int x = rect.x + rect.reserved;
					x += static_cast<int>(unit.getStart()*minutePX);
					int duration = static_cast<int>(unit.getDuration()*minutePX);

					setColor(ugc, i);
					try
					{
						ugc.FillRectangle(x + duration / 2 - bitW / 2, rect.y + rect.height - static_cast<int>(value[i] * bpPX) - bitW / 2, bitW, bitW);
					}
					catch(...)// (invalid_argument& ex)
					{}
				}
				
			}
		}
		
	}

	void setColor(UGC& ugc, int index)
	{
		switch (index)
		{
		case 0://��
		case 1:
			ugc.SetDrawColor(255, 0, 0);
			break;
		case 2://���
			ugc.SetDrawColor(0, 0, 255);
			break;
		case 3://���
			ugc.SetDrawColor(0, 255, 0);
			break;

		}
	}

	/*bool OnLButtonUp(int x, int y) override
	{
		if (IsThisObject(x, y))
		{
			if (controller)
			{
				if (x > rect.x + rect.reserved)
				{
					x = x - rect.reserved - rect.x;
					double bitW = (rect.width - rect.reserved) / 25.;
					int minute = static_cast<int>(x / bitW * 60);
					int unitN = unitContainer->find(minute);
					if (unitN >= 0)
						controller->updateUnitValue(id, unitN);
					else
						controller->addParameterUnit(id, minute / 60 * 60);
				}
				else
					controller->objectMouseUp(id);
				return true;
			}
		}
		return false;
	}*/
};