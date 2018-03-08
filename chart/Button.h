#pragma once
#include "ugc.h"
#include "Rect.h"
#include "IChartController.h"
#include <functional>

class Button
{
protected:
	Rect rect;
	bool down;
	std::wstring text;
	
public:
	//!!!!!!!!!!!!!!!!!!!!!!!!
	std::function<void()> func;
	
	//!!!!!!!!!!!!!!!!!!!!!!!
	Button()
		:rect(Rect(0, 0, 1, 1, 0)),
		down(false)
		
	{
		
	}

	Button(const wstring& caption)
		: rect(Rect(0, 0, 1, 1, 0)),
		down(false),
		text(caption)
	{
		
	}

	void DrawForm(UGC& ugc, int x, int y, int w, int h)
	{
		ugc.FillEllipse(x, y, h);
		ugc.FillEllipse(x+w-h, y, h);
		ugc.FillRectangle(x+h/2, y, w-h, h);

	}

	void OnDraw(UGC& ugc)
	{
		ugc.SetDrawColor(60, 0, 0, 0);
		ugc.FillButtonForm(rect.x, rect.y, rect.width, rect.height);
		
		(!down)? ugc.SetDrawColor(60, 255, 255, 255) : ugc.SetDrawColor(Gdiplus::Color::LightGreen);
		ugc.FillButtonForm(rect.x+1, rect.y+1, rect.width-2, rect.height-2);
		
		ugc.SetAlign(UGC::CENTER);
		ugc.SetTextSize(12);
		ugc.SetDrawColor(60, 60, 60);
		ugc.DrawString(text, rect.x + rect.width / 2, rect.y + rect.height / 2 - ugc.GetTextHeight() / 2);
		ugc.SetAlign(UGC::LEFT);
	}

	virtual bool OnLButtonUp(int x, int y)
	{
		if (x >= rect.x && x < rect.x + rect.width &&
			y >= rect.y && y < rect.y + rect.height)
		{
			down = false;
			if(func)
				func();
			return true;
		}
		return false;
	}

	bool OnLButtonDown(int x, int y)
	{
		if (x >= rect.x && x < rect.x + rect.width &&
			y >= rect.y && y < rect.y + rect.height)
		{
			down = true;
			return true;
		}
		return false;
	}

	bool OnMouseMove(int x, int y)
	{
		if (x >= rect.x && x < rect.x + rect.width &&
			y >= rect.y && y < rect.y + rect.height)
		{
			
			return true;
		}
		down = false;
		return false;
	}

	void resize(const Rect& rectangle)
	{
		rect = rectangle;
	}
}; 
