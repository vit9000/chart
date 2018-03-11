#pragma once
#include "CTableBlock.h"


class CTableBlockHemodynamic : public CTableBlock
{
public:
	CTableBlockHemodynamic(const wstring& BlockName, const Rect& rectangle, IChartController* Controller)
		: CTableBlock(BlockName, rectangle, Controller)
	{
	}


	

	void DrawTable (UGC& ugc) 
	{
		ugc.SetTextSize(10);
		double minutePX = static_cast<double>((rect.width - rect.reserved) / (60.*25.));
		double bpPX = static_cast<double>(rect.height / 200.);
		ugc.SetDrawColor(Gdiplus::Color::Gray);
		int y_bottom = rect.y + rect.height;
		ugc.SetAlign(UGC::RIGHT);
		int text_height = ugc.GetTextHeight();
		for (int i = 20; i < 200; i+=20)
		{
			int yi = y_bottom - static_cast<int>(bpPX*i);
			ugc.DrawLine(rect.reserved+rect.x, yi, rect.x+rect.width, yi);

			ugc.DrawNumber(i, rect.reserved + rect.x - 2, yi - text_height / 2);
		}
		ugc.SetAlign(UGC::LEFT);
		int bitW = static_cast<int>(bpPX*16.);
		
		int color = 0;
		for (const auto& obj : objects)
		{
			
			for (const auto& unit : obj->getContainerUnit()->getUnits())
			{
				double value = unit.getValue().getDoubleValue();

				int x = rect.x + rect.reserved;
				x += static_cast<int>(unit.getStart()*minutePX);
				int duration = static_cast<int>(unit.getDuration()*minutePX);

				setColor(ugc, color);
				ugc.FillRectangle(x + duration / 2 - bitW / 2, rect.y + rect.height - static_cast<int>(value * bpPX) - bitW / 2, bitW, bitW);
				

			}
			color++;
		}
		
	}

	void setColor(UGC& ugc, int index)
	{
		switch (index)
		{
		case 0://ÀÄ
		case 1:
			ugc.SetDrawColor(255, 0, 0);
			break;
		case 2://×ÑÑ
			ugc.SetDrawColor(0, 0, 255);
			break;
		case 3://ÖÂÄ
			ugc.SetDrawColor(0, 255, 0);
			break;

		}
	}

	/*bool OnLButtonUp(int x, int y) override
	{
		if (IsThisObject(x, y))
		{
			if (controller)
			{
				if (x > rect.x + rect.reserved)
				{
					x = x - rect.reserved - rect.x;
					double bitW = (rect.width - rect.reserved) / 25.;
					int minute = static_cast<int>(x / bitW * 60);
					int unitN = unitContainer->find(minute);
					if (unitN >= 0)
						controller->updateUnitValue(id, unitN);
					else
						controller->addParameterUnit(id, minute / 60 * 60);
				}
				else
					controller->objectMouseUp(id);
				return true;
			}
		}
		return false;
	}*/


	virtual void resize(const Rect& rectangle)
	{
		rect.x = rectangle.x;
		rect.y = rectangle.y;
		rect.width = rectangle.width;
		rect.reserved = rectangle.reserved;
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		int h = headerHeight * 4 / 5;

		double dpix = DPIX();
		int border = static_cast<int>(5.*dpix);
		if (buttons.size() >= 1)
			buttons[0]->resize(Rect(rect.x + border, rect.y + headerHeight / 2 - h / 2, h, h));
		
		rect.height = headerHeight * 6;

		
	}


	virtual void OnPaint(UGC& ugc)
	{
		ugc.SetDrawColor(Gdiplus::Color::LightGray);
		ugc.FillRectangle(rect.x, rect.y, rect.width, headerHeight);
		ugc.SetAlign(UGC::CENTER);
		ugc.SetDrawColor(0, 0, 0);
		ugc.SetTextSize(12);
		ugc.DrawString(header, rect.x + rect.width / 2, rect.y + headerHeight / 2 - ugc.GetTextHeight() / 2);

		for (Button_Ptr& button : buttons)
			button->OnDraw(ugc);

		if (fullView)
		{
			ugc.SetAlign(UGC::LEFT);

			DrawTable(ugc);
			//for (const auto& obj : objects)
				//obj->OnPaint(ugc);
		}

		ugc.SetDrawColor(Gdiplus::Color::Gray);
		ugc.DrawLine(rect.x, rect.y + headerHeight, rect.x + rect.width, rect.y + headerHeight, 1);
		ugc.SetDrawColor(0, 0, 0);

		ugc.DrawLine(rect.x, rect.y, rect.x + rect.width, rect.y, 1);
	}
	//---------------------------------------------------------------------------
	virtual bool OnLButtonUp(int x, int y)
	{
		for (Button_Ptr& button : buttons)
		{
			if (button->OnLButtonUp(x, y))
				return true;
		}


		if (fullView)
		{
			//for (auto& obj : objects)
				//if (obj->OnLButtonUp(x, y))
					//return true;
		}
		return false;
	}
	//---------------------------------------------------------------------------
	virtual bool OnLButtonDown(int x, int y)
	{
		for (Button_Ptr& button : buttons)
		{
			if (button->OnLButtonDown(x, y))
				return true;
		}

		if (fullView)
		{
			//for (auto& obj : objects)
				//if (obj->OnLButtonDown(x, y))
					//return true;
		}
		return false;
	}
	//---------------------------------------------------------------------------
	virtual bool OnMouseMove(int x, int y, bool& move_aborted)
	{
		for (Button_Ptr& button : buttons)
		{
			if (button->OnMouseMove(x, y))
				return true;
		}

		bool status = false;
		if (fullView)
		{
			//for (auto& obj : objects)
				//if (obj->OnMouseMove(x, y))
					//status = true;
				//else if (obj->OnMouseMoveAbort())
					//move_aborted = true;

		}
		return status;
	}
};