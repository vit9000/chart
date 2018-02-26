#pragma once
#include "CTableObject.h"
#include "ContainerHemodynamic.h"

class TableHemodynamic : public CTableObject
{
public:
	TableHemodynamic(int ID, IChartController* Controller, const Rect& rectangle, const ContainerUnit* containerUnit)
		: CTableObject(ID, Controller, rectangle, containerUnit)
	{
		
	}

	void OnPaint(UGC& ugc) override
	{
		//ugc.SetDrawColor(100, 245, 245, 155);
		//ugc.FillRectangle(rect.x, rect.y, rect.reserved, rect.height);

		CTableObject::OnPaint(ugc);

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
			//if(i!=0)
				ugc.DrawNumber(i, rect.reserved - 2, yi - text_height / 2);
		}
		ugc.SetAlign(UGC::LEFT);
		int bitW = bpPX*16;
		if (const ContainerHemodynamic* containerHemodynamic = dynamic_cast<const ContainerHemodynamic*>(unitContainer))
		{
			for (const auto& unit : unitContainer->getUnits())
			{
				//for
				{
					int x = rect.x + rect.reserved;
					x += static_cast<int>(unit.getStart()*minutePX);
					int duration = static_cast<int>(unit.getDuration()*minutePX);

					//ugc.DrawNumber(unit.getValue(), x + duration / 2, rect.y + rect.height / 2 - ugc.GetTextHeight() / 2);
					ugc.SetDrawColor(255, 0, 0);
					
					ugc.FillRectangle(x + duration/2 - bitW/2, rect.y+rect.height-static_cast<int>(unit.getValue()*bpPX)-bitW/2, bitW, bitW);
				}
				
			}
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