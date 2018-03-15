#pragma once

#include "TableObjectMovable.h" // включает TableObject, TableObjectResizable


/*---------------------------------------------------------------------
-------------------         RESIZABLE         -------------------------
----------------------------------------------------------------------*/
class TableObject_Pump : public TableObjectResizable
{
public:
	TableObject_Pump(const ID& id, IChartController* Controller,const ContainerUnit* containerUnit)
		: TableObjectResizable(id, Controller, containerUnit)
	{
		sort_type = IVPUMP;
		color = Gdiplus::Color::Crimson;
	}
	void DrawForm(UGC& ugc, const wstring& value, int x, int y, int width, int height) override
	{
		//ugc.SetDrawColor(10, 10, 10);
		int h = static_cast<int>(4 * ugc.getDPIX());
		//int textW = ugc.GetTextWidth(value);
		
		//ugc.FillRectangle(x+textW, y+height/2-h/2, width-6-textW, h);
		int six = static_cast<int>(6 * ugc.getDPIX());
		int seven = static_cast<int>(7 * ugc.getDPIX());

		
		ugc.FillRectangle(x, y+six, width - six, height-six*2);
		
		h = height;
		ugc.FillTriangle(
			x + width - seven, y,
			x + width, y + h / 2,
			x + width - seven,  y + h
		);

		/*for (int i = 2; i < height - 2; i += static_cast<int>(4 * ugc.getDPIX()))
		{
			ugc.FillRectangle(x + 1, rect.y + i, 1, 2);
			//ugc.FillRectangle(x + width - 2, y + i, 1, 2);
		}*/
		ugc.SetAlign(UGC::LEFT);
		ugc.SetDrawColor(255, 255, 255);

		ugc.DrawString(value, x, y + height / 2 - ugc.GetTextHeight() / 2);
	}
	
};
//---------------------------------------------------------------------
class TableObject_IVdrops : public TableObjectResizable
{
public:
	TableObject_IVdrops(const ID& id, IChartController* Controller,  const ContainerUnit* containerUnit)
		: TableObjectResizable(id, Controller, containerUnit)
	{
		sort_type = IVDROPS;
		color = Gdiplus::Color::CornflowerBlue;//CadetBlue;;
	}
};
/*---------------------------------------------------------------------
-------------------         MOVABLE         ---------------------------
----------------------------------------------------------------------*/
class TableObject_IVbolus : public TableObjectMovable
{
public:
	TableObject_IVbolus(const ID& id, IChartController* Controller,  const ContainerUnit* containerUnit)
		: TableObjectMovable(id, Controller, containerUnit)
	{
		sort_type = IVBOLUS;
		color = Gdiplus::Color::ForestGreen;
	}
};
//---------------------------------------------------------------------
class TableObject_Tab : public TableObjectMovable
{
public:
	TableObject_Tab(const ID& id, IChartController* Controller,  const ContainerUnit* containerUnit)
		: TableObjectMovable(id, Controller, containerUnit)
	{
		sort_type = TAB;
		color = Gdiplus::Color::Chocolate;
	}
};