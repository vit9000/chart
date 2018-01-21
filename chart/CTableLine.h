#pragma once
#include <string>
using std::string;


#include "CTableObject.h"
#include "dpix.h"

class CTableLine : public CTableObject
{
protected:
	string header;
	int border;

public:
	CTableLine(int ID, IChartController* Controller, const Rect& rectangle)
		:CTableObject(ID, Controller, rectangle), 
		header("String name")
	{
		border = static_cast<int>(2. * DPIX());
	}

	virtual void OnPaint(UGC& ugc)
	{
		ugc.SetDrawColor(0,0,0);
		ugc.DrawRectangle(rect.x + border, rect.y + border, rect.width - border*2, rect.height - border*2,static_cast<int>(2*DPIX()));		
	}

	virtual void Resize(const Rect& rectangle)
	{
		rect.width = rectangle.width;
	}
};