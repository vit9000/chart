#pragma once

#include <vector>
using std::vector;

#include "CTableObject.h"
#include "CMainController.h"
#include "CTableLine.h"

class TableView : public CTableObject
{
private:
	vector<CTableObject*> table_lines;
	IChartController * main_controller;

public:
	TableView(const Rect& rectangle)
		:CTableObject(1000, NULL, rectangle)
	{
		main_controller = new CMainController();

		int count = 5;
		Rect r(rectangle);
		r.height = rectangle.height/count;
		for(int id=1; id<=count; ++id)
		{	
			table_lines.push_back(new CTableLine(id, main_controller, r));
			r.y += r.height;
		}
	}

	virtual ~TableView()
	{
		for(size_t i=0; i<table_lines.size(); ++i)
			delete table_lines[i];
		delete main_controller;	
	}

	virtual void OnPaint(UGC& ugc)
	{
		ugc.SetDrawColor(0,255,0);
		ugc.FillRectangle(rect.x, rect.y, rect.width, rect.height);
		
		for(size_t i=0; i<table_lines.size(); ++i)
			table_lines[i]->OnPaint(ugc);
	}

	virtual void Resize(const Rect& rectangle)
	{
		rect = Rect(rectangle);
		for(size_t i=0; i<table_lines.size(); ++i)
			table_lines[i]->Resize(rectangle);
	}

	virtual void OnLButtonUp(int x, int y)
	{
		if(x >= rect.x && x <= rect.x+rect.width 
			&& y >= rect.y && y <= rect.y+rect.height)
		{
			for(size_t i=0; i<table_lines.size(); ++i)
				table_lines[i]->OnLButtonUp(x,y);
		}
	}
};