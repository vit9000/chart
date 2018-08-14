#pragma once

#include "TableObjectMovable.h" // включает TableObject, TableObjectResizable
#include "TableObject_IVdrops.h"

/*---------------------------------------------------------------------
-------------------         RESIZABLE         -------------------------
----------------------------------------------------------------------*/
class TableObject_Pump : public TableObjectResizable
{
public:
	TableObject_Pump(IChartController** Controller,const ContainerUnit* containerUnit)
		: TableObjectResizable(Controller, containerUnit)
	{
		
		
	}
	void DrawForm(UGC& ugc, const wstring& value, int x, int y, int width, int height) override
	{
		if((*controller)->MODE == ACCESS::VIEW_ACCESS)
		{
			int one = DPIX()(1);
			
			int seven = DPIX()(6);
			ugc.SetDrawColor(0, 0, 0);

			int shift = ugc.GetTextWidth(value);
			int w = width - shift;
			if(w>=seven)
				ugc.FillRectangle(x+shift, y + height/2-one, w, one*2);
			seven = height/4;
			ugc.DrawLineAntialiased(x + width - seven, y + seven,			x + width, y + height / 2, 2);
			ugc.DrawLineAntialiased(x + width - seven, y + height - seven,	x + width, y + height / 2, 2);
		}
		else
		{
			int six = static_cast<int>(6 * DPIX());
			int seven = static_cast<int>(7 * DPIX());

			ugc.FillRectangle(x, y + six, width - six, height - six * 2);
			ugc.FillTriangle(
				x + width - seven, y,
				x + width, y + height / 2,
				x + width - seven, y + height
			);
			ugc.SetDrawColor(255, 255, 255);	
		}
		ugc.SetAlign(UGC::LEFT);
		ugc.DrawString(value, x, y + height / 2 - ugc.GetTextHeight() / 2);
	}
	
};
//---------------------------------------------------------------------

/*---------------------------------------------------------------------
-------------------         MOVABLE         ---------------------------
----------------------------------------------------------------------*/
/*class TableObject_IVbolus : public TableObjectMovable
{
public:
	TableObject_IVbolus(IChartController* Controller,  const ContainerUnit* containerUnit)
		: TableObjectMovable(Controller, containerUnit)
	{
		
		
	}
};*/
//---------------------------------------------------------------------
