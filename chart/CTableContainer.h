#pragma once

#include <vector>
using std::vector;

#include <memory>
using std::shared_ptr;
//#include <boost/shared_ptr.hpp>
//using boost::shared_ptr;

#include "ugc.h"
#include "dpix.h"

#include "ModelContainers.h"
#include "ChartStructure.h"
#include "ID.h"
#include "CTableBlock.h"


class CTableContainer
{
private:
	map<wstring, CTableBlock> table_lines;
	vector<wstring> blocks;
	IChartController* controller;
	Rect rect;
	const int MIN_HEADER_WIDTH;
	bool move_aborted;
	int SCROLL;
public: 

	const int HOUR_COUNT;

	CTableContainer(IChartController* Controller, const Rect& rectangle)
		: controller(Controller),
		rect(rectangle),
		MIN_HEADER_WIDTH(150),
		HOUR_COUNT(24),
		move_aborted(false),
		SCROLL(0)
		
	{
		Default();
		
		
	}
	void setScroll(int new_value, bool resize=true)
	{
		
		rect.y -= new_value-SCROLL;
		SCROLL = new_value;
		Resize(rect);
	}
	
	virtual ~CTableContainer()
	{
		Default();
	}

	void Default()
	{
		blocks.clear();
		table_lines.clear();
		ChartStructure structure;
		blocks = structure.getBlocks();
		
	}
	//--------------------------------------------------
	size_t getBlocksCount()
	{
		return table_lines.size();
	}
	//--------------------------------------------------
	int getColumnWidth() const
	{
		return (rect.width - MIN_HEADER_WIDTH) / (HOUR_COUNT+1);
	}
	//--------------------------------------------------
	int getHeaderWidth() const
	{
		return rect.width - getColumnWidth()*(HOUR_COUNT+1);
	}
	//--------------------------------------------------
	int getContentHeight() const
	{
		int h = TableObject::LINE_HEIGHT;

		for (const auto& blockname : blocks)
		{
			if (table_lines.count(blockname) == 0)
				continue;
			const CTableBlock& block = table_lines.at(blockname);
			h += block.getRect().height;
		}
		return h;
	}
	//--------------------------------------------------
	void Add(const wstring& BlockName, const ContainerUnit* containerUnit)
	{
		if (table_lines.count(BlockName) == 0)
		{
			table_lines[BlockName] = CTableBlock(BlockName, rect, controller);
			table_lines[BlockName].AddButton(CTableBlock::BUTTON_TYPE::RESIZE);
		}
		
		ID id(BlockName, table_lines[BlockName].size());

		if (const ContainerHemodynamic * temp = dynamic_cast<const ContainerHemodynamic*>(containerUnit))
			table_lines[BlockName].push_back(CTableObject_Ptr(new TableHemodynamic(id, controller, temp)));

		else if (const ContainerParameter * temp = dynamic_cast<const ContainerParameter*>(containerUnit))
			table_lines[BlockName].push_back(CTableObject_Ptr(new TableParameter(id, controller, temp)));
		
		else if (const ContainerIVdrops * temp = dynamic_cast<const ContainerIVdrops*>(containerUnit))
			table_lines[BlockName].push_back(CTableObject_Ptr(new TableObject_IVdrops(id, controller, temp)));

		else if (const ContainerIVinfusion * temp = dynamic_cast<const ContainerIVinfusion*>(containerUnit))
			table_lines[BlockName].push_back(CTableObject_Ptr(new TableObject_Pump(id, controller, temp)));
		
		else if (const ContainerIVbolus * temp = dynamic_cast<const ContainerIVbolus*>(containerUnit))
			table_lines[BlockName].push_back(CTableObject_Ptr(new TableObject_IVbolus(id, controller, temp)));
		
		else if (const ContainerTabs * temp = dynamic_cast<const ContainerTabs*>(containerUnit))
			table_lines[BlockName].push_back(CTableObject_Ptr(new TableObject_Tab(id, controller, temp)));

		Resize(rect);
	}
	//--------------------------------------------------
	void OnPaint(UGC& ugc)
	{

		
		int headerWidth = getHeaderWidth();
		int columnWidth = getColumnWidth();
		ugc.SetDrawColor(Gdiplus::Color::Gray);
		for (int i = 0; i <= HOUR_COUNT; ++i)
		{
			int x = rect.x + headerWidth + i*columnWidth;
			ugc.DrawLine(x, 0, x, TableObject::LINE_HEIGHT + rect.height);
		}

		for (const wstring& block : blocks)
		{
			if (table_lines.count(block) == 0) continue;
			table_lines.at(block).OnPaint(ugc);
		}

		ugc.SetDrawColor(255, 255, 255);
		ugc.FillRectangle(rect.x, 0, rect.x + rect.width, TableObject::LINE_HEIGHT);
		ugc.SetDrawColor(Gdiplus::Color::Gray);
		ugc.DrawLine(rect.x, TableObject::LINE_HEIGHT, rect.x + rect.width, TableObject::LINE_HEIGHT);
		
		ugc.SetAlign(UGC::CENTER);
		ugc.SetTextSize(12);
		
		for (int i = 0; i <= HOUR_COUNT; ++i)
		{
			int x = rect.x + headerWidth + i*columnWidth;
			ugc.DrawLine(x, 0, x, TableObject::LINE_HEIGHT);
			if (i == HOUR_COUNT)
			{
				ugc.DrawString(L"Ñ", x + columnWidth / 2, TableObject::LINE_HEIGHT / 2 - ugc.GetTextHeight() / 2);
				break;
			}
			int number = 9 + i;
			if (number >= 24) number -= 24;
			ugc.DrawNumber(number, x + columnWidth / 2, TableObject::LINE_HEIGHT / 2 - ugc.GetTextHeight() / 2);

		}
		ugc.SetAlign(UGC::LEFT);
		
	}
	//--------------------------------------------------
	void Resize()
	{
		Resize(rect);
	}
	void Resize(const Rect& rectangle)
	{
		rect = Rect(rectangle);
		rect.height = 0;
		rect.reserved = getHeaderWidth();
		Rect r(rect);

		// ÄÎÁÀÂËÅÍÈÅ ÏÓÍÊÒÀ ÍÀÇÍÀ×ÅÍÈß
		ChartStructure s;
		wstring temp = s.getText(ChartStructure::ADMINISTRATIONS);
		if (table_lines.count(temp) == 0)
		{
			table_lines[temp] = CTableBlock(temp, rect, controller);
			table_lines[temp].AddButton(CTableBlock::BUTTON_TYPE::RESIZE);
			table_lines[temp].AddButton(CTableBlock::BUTTON_TYPE::ADMINISTRATIONS);
		}
		// ÐÅÑÀÉÇ ÏÎ ÂÑÅÌ ÏÎÊÀÇÀÒÅËßÌ
		for (const wstring& block : blocks)
		{
			if (table_lines.count(block) == 0) continue;
			CTableBlock& tableBlock = table_lines.at(block);
			tableBlock.resize(r);
			r.y += tableBlock.getRect().height;
			rect.height += tableBlock.getRect().height;
		}
	}
	//--------------------------------------------------
	
	//--------------------------------------------------
	bool OnLButtonUp(int x, int y)
	{
		if(move_aborted)
		{
			move_aborted = false;
			return false;
		}
		for (const wstring& block : blocks)
			if (table_lines.count(block) > 0)
			{
				if (table_lines.at(block).OnLButtonUp(x, y))
					return true;
			}
		return false;
	}
	//--------------------------------------------------
	bool OnLButtonDown(int x, int y)
	{
		move_aborted = false;
		for (const wstring& block : blocks)
			if (table_lines.count(block) > 0)
			{
				if (table_lines.at(block).OnLButtonDown(x, y))
					return true;
			}
		return false;
	}
	//--------------------------------------------------
	bool OnMouseMove(int x, int y)
	{
		bool status = false;
		for (const wstring& block : blocks)
			if (table_lines.count(block) > 0)
			{
				for (size_t i = 0; i < table_lines[block].size(); ++i)
				{
					if (table_lines.at(block).OnMouseMove(x, y, move_aborted))
						status = true;
				}
			}
		return status;
	}
	//--------------------------------------------------
	bool IsMoveAborted()
	{
		return move_aborted;
	}
};
