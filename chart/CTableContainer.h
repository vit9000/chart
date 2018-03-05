#pragma once

#include <vector>
using std::vector;

#include <memory>
using std::shared_ptr;
//#include <boost/shared_ptr.hpp>
//using boost::shared_ptr;

#include "ugc.h"
#include "dpix.h"
#include "TableLines.h"
#include "TableParameter.h"
#include "TableHemodynamic.h"
#include "ModelContainers.h"
#include "ChartStructure.h"
#include "ID.h"

enum {DRUG_CONTENT=1};

typedef shared_ptr<TableObject> CTableObject_Ptr;

class CTableContainer
{
private:
	map<wstring, vector<CTableObject_Ptr>> table_lines;
	vector<wstring> blocks;
	IChartController* controller;
	Rect rect;
	const int MIN_HEADER_WIDTH;
	int LINE_HEIGHT;
	bool move_aborted;
	int SCROLL;
public: 
	
	const int HOUR_COUNT;

	CTableContainer(IChartController* Controller, const Rect& rectangle, int ContentType = DRUG_CONTENT)
		: controller(Controller),
		rect(rectangle),
		MIN_HEADER_WIDTH(150),
		LINE_HEIGHT(22),
		HOUR_COUNT(24),
		move_aborted(false),
		SCROLL(0)
		
	{
		Default();
		
		
	}
	void setScroll(int new_value)
	{
		
		rect.y -= new_value-SCROLL;
		SCROLL = new_value;
		Resize(rect);
	}
	
	int getLineHeight() { return LINE_HEIGHT; }
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

	size_t getBlocksCount()
	{
		return table_lines.size();
	}

	int getColumnWidth() const
	{
		return (rect.width - MIN_HEADER_WIDTH) / (HOUR_COUNT+1);
	}

	int getHeaderWidth() const
	{
		return rect.width - getColumnWidth()*(HOUR_COUNT+1);
	}
	//--------------------------------------------------
	int getContentHeight() const
	{
		int h = LINE_HEIGHT;
		for (const auto& blockname : blocks)
		{
			if (table_lines.count(blockname) == 0)
				continue;
			const auto& block = table_lines.at(blockname);
			if (block.size() == 0)
				continue;
			const Rect& r = block[block.size() - 1]->getRect();
			h = r.y+r.height;
		}
		return h;
	}


	void Add(const wstring& BlockName, const ContainerUnit* containerUnit)
	{
		ID id(BlockName, table_lines[BlockName].size());
		
		if (const ContainerHemodynamic * temp = dynamic_cast<const ContainerHemodynamic*>(containerUnit))
		{
			Rect r = getObjectRect(BlockName, id.getIndex(), rect);
			r.height *= 5;
			table_lines[BlockName].push_back(CTableObject_Ptr(new TableHemodynamic(id, controller, r, temp)));
		}

		else if (const ContainerParameter * temp = dynamic_cast<const ContainerParameter*>(containerUnit))
			table_lines[BlockName].push_back(CTableObject_Ptr(new TableParameter(id, controller, getObjectRect(BlockName, id.getIndex(), rect), temp)));
		
		else if (const ContainerIVdrops * temp = dynamic_cast<const ContainerIVdrops*>(containerUnit))
			table_lines[BlockName].push_back(CTableObject_Ptr(new TableObject_IVdrops(id, controller, getObjectRect(BlockName, id.getIndex(), rect), temp)));

		else if (const ContainerIVinfusion * temp = dynamic_cast<const ContainerIVinfusion*>(containerUnit))
			table_lines[BlockName].push_back(CTableObject_Ptr(new TableObject_Pump(id, controller, getObjectRect(BlockName, id.getIndex(), rect), temp)));
		
		else if (const ContainerIVbolus * temp = dynamic_cast<const ContainerIVbolus*>(containerUnit))
			table_lines[BlockName].push_back(CTableObject_Ptr(new TableObject_IVbolus(id, controller, getObjectRect(BlockName, id.getIndex(), rect), temp)));
		
		else if (const ContainerTabs * temp = dynamic_cast<const ContainerTabs*>(containerUnit))
			table_lines[BlockName].push_back(CTableObject_Ptr(new TableObject_Tab(id, controller, getObjectRect(BlockName, id.getIndex(), rect), temp)));
	}
	//--------------------------------------------------
	void OnPaint(UGC& ugc)
	{

	
		
		int y= LINE_HEIGHT;
		for(const wstring& block : blocks)
		{
			int temp = y;
			for (size_t i = 0; i < table_lines[block].size(); ++i)
			{
				table_lines[block][i]->OnPaint(ugc);
				y += table_lines[block][i]->getRect().height;
			}
			//ugc.SetDrawColor(0, 0, 0);
			//ugc.FillRectangle(rect.x, y-1, rect.width, 2);

			/*int textW = ugc.GetTextWidth(block);
			if (y - temp > textW)
				temp = y - (y - temp) / 2 + textW / 2;
			else
				temp = y + textW;
			ugc.DrawVerticalString(block,rect.x/2-ugc.GetTextHeight()/2, temp);*/
		}

		ugc.SetDrawColor(255, 255, 255);
		ugc.FillRectangle(rect.x, 0, rect.x + rect.width, LINE_HEIGHT);
		ugc.SetDrawColor(155, 155, 155);
		ugc.DrawLine(rect.x, LINE_HEIGHT, rect.x + rect.width, LINE_HEIGHT);

		ugc.SetAlign(UGC::CENTER);
		ugc.SetTextSize(12);
		int headerWidth = getHeaderWidth();
		int columnWidth = getColumnWidth();
		for (int i = 0; i <= HOUR_COUNT; ++i)
		{
			int x = rect.x + headerWidth + i*columnWidth;
			ugc.DrawLine(x, 0, x, LINE_HEIGHT+rect.height);

			if (i == HOUR_COUNT)
			{
				ugc.DrawString(L"Ñ", x + columnWidth / 2, LINE_HEIGHT / 2 - ugc.GetTextHeight() / 2);
				break;
			}
			int number = 9 + i;
			if (number >= 24) number -= 24;
			ugc.DrawNumber(number, x + columnWidth / 2, LINE_HEIGHT / 2 - ugc.GetTextHeight() / 2);

		}
		ugc.SetAlign(UGC::LEFT);
		
	}
	//--------------------------------------------------
	void Resize(const Rect& rectangle)
	{
		rect = Rect(rectangle);
		
		for (const wstring& block : blocks)
			for (size_t i = 0; i < table_lines[block].size(); ++i)
				table_lines[block][i]->Resize(getObjectRect(block, (int)i, rect));
	}
	//--------------------------------------------------
	Rect getRectByIndex(const wstring& blockname, int index) const
	{
		if(table_lines.count(blockname)==0 || index>= static_cast<int>(table_lines.at(blockname).size()))
			return { 0,0,0,0,0 };

		return table_lines.at(blockname).at(index)->getRect();
	}
	
	Rect getObjectRect(const wstring& blockname, int index, const Rect& rectangle) const
	{
		
		Rect temprect(rectangle);
		DPIX dpix;
		temprect.height = dpix.getIntegerValue(LINE_HEIGHT);
		
		const Rect *r = nullptr;
		int counter = 0;
		for (const auto& block : blocks)
		{
			if (table_lines.count(block)>0)
			{
				if (block == blockname)
				{
					if (index == 0 && counter == 0) temprect.y = rectangle.y;
					if (index>0)
						r = &(table_lines.at(blockname)[index - 1]->getRect());
					if (r)
						temprect.y = r->y + r->height;
				}
				else
				{
					const auto& vec = table_lines.at(block);
					if(vec.size()>0)
						r = &(vec[vec.size() - 1]->getRect());
				}
				counter++;
			}

		}
		
		temprect.reserved = dpix.getIntegerValue(getHeaderWidth());
		return temprect;
	}
	//--------------------------------------------------
	bool OnLButtonUp(int x, int y)
	{
		if(move_aborted)
		{
			move_aborted = false;
			return false;
		}
		for (const wstring& block : blocks)
			for(size_t i=0; i<table_lines[block].size(); ++i)
				if(table_lines[block][i]->OnLButtonUp(x,y))
					return true;

		return false;
	}
	//--------------------------------------------------
	bool OnLButtonDown(int x, int y)
	{
		move_aborted = false;
		for (const wstring& block : blocks)
			for (size_t i = 0; i<table_lines[block].size(); ++i)
				if (table_lines[block][i]->OnLButtonDown(x, y))
					return true;

		return false;
	}
	//--------------------------------------------------
	bool OnMouseMove(int x, int y)
	{
		bool status = false;
		for (const wstring& block : blocks)
			for (size_t i = 0; i < table_lines[block].size(); ++i)
			{
				if (table_lines[block][i]->OnMouseMove(x, y))
					status = true;
				else if (table_lines[block][i]->OnMouseMoveAbort())
					move_aborted = true;
			}
		return status;
	}
	//--------------------------------------------------
	bool IsMoveAborted()
	{
		return move_aborted;
	}
};
