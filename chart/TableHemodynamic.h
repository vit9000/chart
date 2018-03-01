#pragma once
#include "TableObject.h"
#include "ModelContainers.h"

class TableHemodynamic : public TableObject
{
public:
	TableHemodynamic(int ID, IChartController* Controller, const Rect& rectangle, const ContainerUnit* containerUnit)
		: TableObject(ID, Controller, rectangle, containerUnit)
	{
		
	}

	void OnPaint(UGC& ugc) override
	{
		//ugc.SetDrawColor(100, 245, 245, 155);
		//ugc.FillRectangle(rect.x, rect.y, rect.reserved, rect.height);

		TableObject::OnPaint(ugc);

		ugc.SetTextSize(10);
		double minutePX = static_cast<double>((rect.width - rect.reserved) / 1440.);
		double bpPX = static_cast<double>(rect.height / 200.);
		ugc.SetDrawColor(155, 155, 155);
		int y_bottom = rect.y + rect.height;
		ugc.SetAlign(UGC::RIGHT);
		int text_height = ugc.GetTextHeight();
		for (int i = 20; i < 200; i+=20)
		{
			int yi = y_bottom - static_cast<int>(bpPX*i);
			ugc.DrawLine(rect.reserved, yi, rect.width, yi);

			ugc.DrawNumber(i, rect.reserved - 2, yi - text_height / 2);
		}
		ugc.SetAlign(UGC::LEFT);
		int bitW = static_cast<int>(bpPX*16.);
		if (const ContainerHemodynamic* containerHemodynamic = dynamic_cast<const ContainerHemodynamic*>(unitContainer))
		{
			for (const auto& unit : containerHemodynamic->getUnits())
			{
				const Value& value = unit.getValue();
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
					catch (invalid_argument& ex)
					{}
				}
				
			}
		}
		
	}

	void setColor(UGC& ugc, int index)
	{
		switch (index)
		{
		case 0://ÀÄ
		case 1:
			ugc.SetDrawColor(255, 0, 0);
			break;
		case 2://×ÑÑ
			ugc.SetDrawColor(0, 0, 255);
			break;
		case 3://ÖÂÄ
			ugc.SetDrawColor(0, 255, 0);
			break;

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
					double bitW = (rect.width - rect.reserved) / 24.;
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
	}
};