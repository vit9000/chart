#pragma once

#include <vector>
using std::vector;

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "ugc.h"
#include "dpix.h"


enum {DRUG_CONTENT=1};

class CTableContainer
{
private:
	vector< shared_ptr<CTableObject> > table_lines;
	IChartController* controller;
	Rect rect;
	const int MIN_HEADER_WIDTH;
public: 
	const int LINE_HEIGHT;
	const int HOUR_COUNT;

	CTableContainer(IChartController* Controller, const Rect& rectangle, int ContentType=DRUG_CONTENT)
		: controller(Controller), 
		rect(rectangle),
		MIN_HEADER_WIDTH(150),
		LINE_HEIGHT(30),
		HOUR_COUNT(24)
	{
		
	}

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
	void Add(const UnitContainer& unitContainer)
	{
		/*switch(TYPE)
		{
		default:
		case DRUG_CONTENT:
			int id = static_cast<int>(table_lines.size());
			table_lines.push_back(new CTableObject(id, controller, getObjectRect(id, rect), unitContainer));
			break;
		}*/
		int id = static_cast<int>(table_lines.size());
		table_lines.push_back(shared_ptr<CTableObject> (new CTableObject(id, controller, getObjectRect(id, rect), unitContainer)));
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
	Rect getObjectRect(int index, const Rect& rectangle)
	{
		Rect temprect(rectangle);
		DPIX dpix;
		temprect.height = dpix.getIntegerValue(LINE_HEIGHT);
		temprect.y = index * temprect.height + rect.y;
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
	
};
