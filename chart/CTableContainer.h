#pragma once

#include <vector>
using std::vector;

#include <memory>
using std::shared_ptr;
//#include <boost/shared_ptr.hpp>
//using boost::shared_ptr;

#include "ugc.h"
#include "dpix.h"
#include "TableDrug.h"
#include "TableParameter.h"
#include "ContainerParameter.h"
#include "ContainerIVdrops.h"
#include "ContainerVIbolus.h"
#include "ContainerIVinfusion.h"
#include "ContainerTabs.h"
#include "TableTabsBolus.h"
#include "TableInfusion.h"
#include "ContainerHemodynamic.h"
#include "TableHemodynamic.h"


enum {DRUG_CONTENT=1};

typedef shared_ptr<CTableObject> CTableObject_Ptr;

class CTableContainer
{
private:
	vector<CTableObject_Ptr> table_lines;
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
		
	}

	int getLineHeight() { return LINE_HEIGHT; }
	virtual ~CTableContainer()
	{
		Clear();
	}

	void Clear()
	{
		//for(size_t i=0; i<table_lines.size(); ++i)
		//	delete table_lines[i]; // not need with shared_ptr 
		table_lines.clear();
	}

	size_t getCount()
	{
		return table_lines.size();
	}

	int getColumnWidth()
	{
		return (rect.width - MIN_HEADER_WIDTH) / HOUR_COUNT;
	}

	int getHeaderWidth()
	{
		return rect.width - getColumnWidth()*HOUR_COUNT;
	}
	//--------------------------------------------------

	void Add(const ContainerUnit* containerUnit)
	{

		int id = static_cast<int>(table_lines.size());
		
		if (const ContainerHemodynamic * temp = dynamic_cast<const ContainerHemodynamic*>(containerUnit))
		{
			Rect rect = getObjectRect(id, rect);
			rect.height *= 5;
			table_lines.push_back(CTableObject_Ptr(new TableHemodynamic(id, controller, rect, temp)));
		}

		else if (const ContainerParameter * temp = dynamic_cast<const ContainerParameter*>(containerUnit))
			table_lines.push_back(CTableObject_Ptr(new TableParameter(id, controller, getObjectRect(id, rect), temp)));
		
		else if (const ContainerIVdrops * temp = dynamic_cast<const ContainerIVdrops*>(containerUnit))
			table_lines.push_back(CTableObject_Ptr(new TableDrug(id, controller, getObjectRect(id, rect), temp)));
		
		else if (const ContainerIVbolus * temp = dynamic_cast<const ContainerIVbolus*>(containerUnit))
			table_lines.push_back(CTableObject_Ptr(new TableTabsBolus(id, controller, getObjectRect(id, rect), temp)));
		
		else if (const ContainerIVinfusion * temp = dynamic_cast<const ContainerIVinfusion*>(containerUnit))
			table_lines.push_back(CTableObject_Ptr(new TableInfusion(id, controller, getObjectRect(id, rect), temp)));
		
		else if (const ContainerTabs * temp = dynamic_cast<const ContainerTabs*>(containerUnit))
			table_lines.push_back(CTableObject_Ptr(new TableTabsBolus(id, controller, getObjectRect(id, rect), temp)));
	}
	//--------------------------------------------------
	void OnPaint(UGC& ugc)
	{
		for(size_t i=0; i<table_lines.size(); ++i)
			table_lines[i]->OnPaint(ugc);
	}
	//--------------------------------------------------
	void Resize(const Rect& rectangle)
	{
		rect = Rect(rectangle);
		
		for(size_t i=0; i<table_lines.size(); ++i)
			table_lines[i]->Resize(getObjectRect((int)i,rectangle));
	}
	//--------------------------------------------------
	const Rect& getRectByIndex(int index) const
	{
		if(index>= static_cast<int>(table_lines.size()))
		return { 0,0,0,0,0 };

		return table_lines.at(index)->getRect();
	}
	Rect getObjectRect(int index, const Rect& rectangle)
	{
		Rect temprect(rectangle);
		DPIX dpix;
		temprect.height = dpix.getIntegerValue(LINE_HEIGHT);
		if (index == 0) temprect.y = LINE_HEIGHT;
		else
		{
			const Rect& r = table_lines[index - 1]->getRect();
			temprect.y = r.y + r.height;//index * temprect.height + rect.y; 
		}
		
		temprect.reserved = dpix.getIntegerValue(getHeaderWidth());
		return temprect;
	}
	//--------------------------------------------------
	bool OnLButtonUp(int x, int y)
	{
		for(size_t i=0; i<table_lines.size(); ++i)
			if(table_lines[i]->OnLButtonUp(x,y))
				return true;

		return false;
	}
	//--------------------------------------------------
	bool OnLButtonDown(int x, int y)
	{
		for (size_t i = 0; i<table_lines.size(); ++i)
			if (table_lines[i]->OnLButtonDown(x, y))
				return true;

		return false;
	}
	//--------------------------------------------------
	bool OnMouseMove(int x, int y)
	{
		for (size_t i = 0; i<table_lines.size(); ++i)
			if (table_lines[i]->OnMouseMove(x, y))
				return true;

		return false;
	}
	//--------------------------------------------------
	
};
