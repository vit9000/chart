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
	
public: 
	
	const int HOUR_COUNT;

	CTableContainer(IChartController* Controller, const Rect& rectangle, int ContentType=DRUG_CONTENT)
		: controller(Controller), 
		rect(rectangle),
		MIN_HEADER_WIDTH(150),
		LINE_HEIGHT(22),
		HOUR_COUNT(24)
		
	{
		Default();
		
		
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
		return (rect.width - MIN_HEADER_WIDTH) / HOUR_COUNT;
	}

	int getHeaderWidth() const
	{
		return rect.width - getColumnWidth()*HOUR_COUNT;
	}
	//--------------------------------------------------

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
			for (size_t i = 0; i < table_lines[block].size(); ++i)
			{
				table_lines[block][i]->OnPaint(ugc);
				y += table_lines[block][i]->getRect().height;
			}
			ugc.SetDrawColor(0, 0, 0);
			ugc.FillRectangle(rect.x, y-1, rect.width, 2);

		}
	}
	//--------------------------------------------------
	void Resize(const Rect& rectangle)
	{
		rect = Rect(rectangle);
		

		for (const wstring& block : blocks)
			for(size_t i=0; i<table_lines[block].size(); ++i)
				table_lines[block][i]->Resize(getObjectRect(block, (int)i,rectangle));
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
					if (index == 0 && counter == 0) temprect.y = LINE_HEIGHT;
					if (index>0)
						r = &(table_lines.at(blockname)[index - 1]->getRect());
					if (r)
						temprect.y = r->y + r->height;
				}
				else
				{
					const auto& vec = table_lines.at(block);
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
		for (const wstring& block : blocks)
			for(size_t i=0; i<table_lines[block].size(); ++i)
				if(table_lines[block][i]->OnLButtonUp(x,y))
					return true;

		return false;
	}
	//--------------------------------------------------
	bool OnLButtonDown(int x, int y)
	{
		for (const wstring& block : blocks)
			for (size_t i = 0; i<table_lines[block].size(); ++i)
				if (table_lines[block][i]->OnLButtonDown(x, y))
					return true;

		return false;
	}
	//--------------------------------------------------
	bool OnMouseMove(int x, int y)
	{
		for (const wstring& block : blocks)
			for (size_t i = 0; i<table_lines[block].size(); ++i)
				if (table_lines[block][i]->OnMouseMove(x, y))
				return true;

		return false;
	}
	//--------------------------------------------------
	
};
