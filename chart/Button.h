#pragma once
#include "ugc.h"
#include "Rect.h"
//#include "IChartController.h"
#include <functional>

class Button
{
protected:
	Rect rect;
	bool down;
	std::wstring text;
	bool visible;
	
public:
	//!!!!!!!!!!!!!!!!!!!!!!!!
	std::function<void()> func;
	std::function<void()> repaint;
	
	//!!!!!!!!!!!!!!!!!!!!!!!
	Button()
		:rect(Rect(0, 0, 1, 1, 0)),
		down(false),
		visible(true)
		
	{
		
	}

	Button(const wstring& caption)
		: rect(Rect(0, 0, 1, 1, 0)),
		down(false),
		text(caption),
		visible(true)
	{
		
	}

	void SetVisible(bool visible_status)
	{
		visible = visible_status;
	}

	void DrawForm(UGC& ugc, int x, int y, int w, int h)
	{
		if (!visible) return;
		ugc.FillEllipse(x, y, h);
		ugc.FillEllipse(x+w-h, y, h);
		ugc.FillRectangle(x+h/2, y, w-h, h);

	}

	void OnDraw(UGC& ugc)
	{
		if (!visible) return;
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
		if (!visible) return false;
		if (x >= rect.x && x < rect.x + rect.width &&
			y >= rect.y && y < rect.y + rect.height)
		{
			down = false;
			if(repaint)
				repaint();
			if(func)
				func();
			return true;
		}
		return false;
	}

	bool OnLButtonDown(int x, int y)
	{
		if (!visible) return false;
		if (x >= rect.x && x < rect.x + rect.width &&
			y >= rect.y && y < rect.y + rect.height)
		{
			down = true;
			if (repaint)
				repaint();
			return true;
		}
		return false;
	}

	bool OnMouseMove(int x, int y)
	{
		if (!visible) return false;
		if (x >= rect.x && x < rect.x + rect.width &&
			y >= rect.y && y < rect.y + rect.height)
		{
			
			return true;
		}
		down = false;
		if (repaint)
			repaint();
		return false;
	}

	void resize(const Rect& rectangle)
	{
		rect = rectangle;
	}

	const Rect& getRect() const
	{
		return rect;
	}
}; 
