#pragma once


#include "ugc.h"
#include "Rect.h"
#include "Button.h"

class StatusButton : public Button
{
protected:
	bool checked;

	
public:
	
	StatusButton()
		: Button(L"–"),
		checked(true)
	{}


	/*void OnDraw(UGC& ugc)
	{
		
		ugc.SetDrawColor(60, 0, 0, 0);
		ugc.FillEllipse(rect.x, rect.y, rect.height);
		ugc.SetDrawColor(60, 255, 255, 255);
		ugc.FillEllipse(rect.x+1, rect.y+1, rect.height-2);
		
		
		wstring temp = wstring((checked)?L"–":L"+");
		ugc.SetAlign(UGC::CENTER);
		ugc.SetTextSize(14);
		ugc.SetDrawColor(60, 60, 60);
		ugc.DrawString(temp, rect.x + rect.width / 2, rect.y + rect.height / 2 - ugc.GetTextHeight() / 2);
		ugc.SetAlign(UGC::LEFT);
	}*/

	bool OnLButtonUp(int x, int y) override
	{
		if (x >= rect.x && x < rect.x + rect.width &&
			y >= rect.y && y < rect.y + rect.height)
		{
			down = false;
			checked = !checked;
			text = wstring((checked) ? L"–" : L"+");
			return true;
		}
		return false;
	}

	
};