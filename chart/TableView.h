#pragma once

#include <vector>


#include "CTableObject.h"
#include "CMainController.h"
#include "CTableLine.h"
#include "IModel.h"


class TableView : public CTableObject
{

	virtual ~TableView()
	{
		for(size_t i=0; i<table_lines.size(); ++i)
			delete table_lines[i];
		delete main_controller;	
	}

	virtual void OnPaint(UGC& ugc)
	{
		for(size_t i=0; i<table_lines.size(); ++i)
			table_lines[i]->OnPaint(ugc);

		ugc.SetDrawColor(255,0,0);
		Patient* patient = model->getCurrentPatient();
		if(patient)
			ugc.DrawString(patient->getName(), rect.x, rect.y);
		
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