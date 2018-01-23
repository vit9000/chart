#pragma once
#include <string>
using std::wstring;

#include "CTableObject.h"
#include "dpix.h"

class CTableLine : public CTableObject
{
protected:
	wstring header;
	int border;

public:
	CTableLine(int ID, IChartController* Controller, const Rect& rectangle)
		:CTableObject(ID, Controller, rectangle)

	{
		//CMainModel* model = Controller->getModel();

		border = static_cast<int>(2. * DPIX());
	}

	virtual void OnPaint(UGC& ugc)
	{
		ugc.SetDrawColor(100,100,0,0);
		ugc.FillRectangle(rect.x, rect.y, rect.reserved, rect.height);
		ugc.SetDrawColor(100,0,100,0);
		ugc.FillRectangle(rect.x+rect.reserved, rect.y, rect.width-rect.reserved, rect.height);
		//ugc.DrawRectangle(rect.x + border, rect.y + border, rect.width - border*2, rect.height - border*2,static_cast<int>(2*DPIX()));		
	}

	virtual void Resize(const Rect& rectangle)
	{
		rect.width = rectangle.width;
	}
};