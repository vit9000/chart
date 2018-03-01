#pragma once

#include "TableObjectMovable.h" // включает TableObject, TableObjectResizable


/*---------------------------------------------------------------------
-------------------         RESIZABLE         -------------------------
----------------------------------------------------------------------*/
class TableObject_Pump : public TableObjectResizable
{
public:
	TableObject_Pump(int ID, IChartController* Controller, const Rect& rectangle, const ContainerUnit* containerUnit)
		: TableObjectResizable(ID, Controller, rectangle, containerUnit)
	{}
	void DrawForm(UGC& ugc, int x, int y, int width, int height) override
	{
		ugc.FillRectangle(x, y, width-6, height);
		ugc.SetDrawColor(235, 235, 255);
		
		ugc.FillTriangle(x + width - 7, y,
			x + width, y + height / 2,
			x + width - 7,  y + height);

		for (int i = 2; i < height - 2; i += static_cast<int>(4 * ugc.getDPIX()))
		{
			ugc.FillRectangle(x + 1, rect.y + i, 1, 2);
			//ugc.FillRectangle(x + width - 2, y + i, 1, 2);
		}
	}
};
//---------------------------------------------------------------------
class TableObject_IVdrops : public TableObjectResizable
{
public:
	TableObject_IVdrops(int ID, IChartController* Controller, const Rect& rectangle, const ContainerUnit* containerUnit)
		: TableObjectResizable(ID, Controller, rectangle, containerUnit)
	{}
};
/*---------------------------------------------------------------------
-------------------         MOVABLE         ---------------------------
----------------------------------------------------------------------*/
class TableObject_IVbolus : public TableObjectMovable
{
public:
	TableObject_IVbolus(int ID, IChartController* Controller, const Rect& rectangle, const ContainerUnit* containerUnit)
		: TableObjectMovable(ID, Controller, rectangle, containerUnit)
	{}
};
//---------------------------------------------------------------------
class TableObject_Tab : public TableObjectMovable
{
public:
	TableObject_Tab(int ID, IChartController* Controller, const Rect& rectangle, const ContainerUnit* containerUnit)
		: TableObjectMovable(ID, Controller, rectangle, containerUnit)
	{}
};