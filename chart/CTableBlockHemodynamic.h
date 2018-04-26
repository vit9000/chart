#pragma once
#include "CTableBlock.h"


class CTableBlockHemodynamic : public CTableBlock
{
	int type;
public:
	CTableBlockHemodynamic(const wstring& BlockName, const Rect& rectangle, IChartController* Controller, int Type)
		: CTableBlock(BlockName, rectangle, Controller), type(Type)
	{
	}


	void OnPaint(UGC& ugc) override
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
			DrawTable(ugc);
		
		ugc.SetDrawColor(Gdiplus::Color::Gray);
		ugc.DrawLine(rect.x, rect.y + headerHeight, rect.x + rect.width, rect.y + headerHeight, 1);
		ugc.SetDrawColor(0, 0, 0);

		ugc.DrawLine(rect.x, rect.y, rect.x + rect.width, rect.y, 1);
	}

	void DrawTable (UGC& ugc) 
	{
		ugc.SetTextSize(10);
		double minutePX = static_cast<double>((rect.width - rect.reserved) / (60.*25.));
		int max = (type== static_cast<int>(BLOCK_TYPE::PLOT_PA)) ? 100 : 200;
		double bpPX = static_cast<double>((rect.height-headerHeight) / (double)max);
		ugc.SetDrawColor(Gdiplus::Color::Gray);
		int y_bottom = rect.y + rect.height;
		ugc.SetAlign(UGC::RIGHT);
		int text_height = ugc.GetTextHeight();
		// разметка
		
		for (int i = 20; i < max; i+=20)
		{
			int yi = y_bottom - static_cast<int>(bpPX*i);
			ugc.DrawLine(rect.reserved+rect.x, yi, rect.x+rect.width, yi);
			ugc.DrawNumber(i, rect.reserved + rect.x - 2, yi - text_height / 2);
		}
		ugc.SetAlign(UGC::LEFT);
		int bitW = static_cast<int>(ugc.getDPIX()*8);
		
		int color = (type == static_cast<int>(BLOCK_TYPE::PLOT_PA)) ? 6 : 0;
		ugc.SetTextSize(8);
		int y = rect.y+headerHeight;
		int textH = ugc.GetTextHeight();
		for (const auto& obj : objects)
		{
			y += textH;
			DrawForm(ugc, color, rect.x+textH, y+textH/4, textH/2, textH/2);
			ugc.SetDrawColor(0, 0, 0);
			ugc.DrawString(obj->getContainerUnit()->getName(), rect.x+textH*2, y);


			int lastX = -1;
			int lastY = -1;
			for (const auto& unit : obj->getContainerUnit()->getUnits())
			{
				double value = unit.getValue().getDoubleValue();
				if (value == Value::EMPTY) continue;
				
				int x = rect.x + rect.reserved;
				x += static_cast<int>(unit.getStart()*minutePX);
				int duration = static_cast<int>(unit.getDuration()*minutePX);

				int X = x + duration / 2 - bitW / 2;
				int Y = rect.y + rect.height - static_cast<int>(value * bpPX) - bitW / 2;
				// обработка выхода за пределы графика
				if (Y < rect.y+headerHeight) Y = rect.y+headerHeight;
				else if (Y > rect.y + rect.height- bitW) Y = rect.y + rect.height - bitW;

				if (lastX > 0)
					ugc.DrawLineAntialiased(lastX+bitW/2, lastY+bitW/2, X+bitW/2, Y+bitW/2, 2);
				
				DrawForm(ugc, color, X, Y, bitW, bitW);
				
				lastX = X;
				lastY = Y;

			}
			color++;
		}
		
	}

	void DrawForm(UGC& ugc, int index, int x, int y, int w, int h)
	{
		setColor(ugc, index);
		switch (index)
		{
		case 0://АД
		case 1:
			ugc.FillRectangle(x, y, w, h);
			break;
		case 2://ЧСС
			ugc.FillEllipse(x, y, w);
			break;
		case 3://ЦВД
			ugc.FillDiamondShape(x, y, w, h);
			
			break;
		case 4: // доп САД
		case 5: // доп ДАД
			ugc.FillTriangle(x,y+h, x+w/2,y, x+w, y+h);
			break;
		case 6: // ЛCАД
		case 7: // ЛДАД
			ugc.FillDiamondShape(x, y, w, h);
			break;
		case 8: //Л СрАД
			ugc.FillTriangle(x, y + h, x + w / 2, y, x + w, y + h);
			break;

		}
	}

	void setColor(UGC& ugc, int index)
	{
		switch (index)
		{
		case 0://АД
		case 1:
			ugc.SetDrawColor(255, 0, 0);
			break;
		case 2://ЧСС
			ugc.SetDrawColor(0, 0, 255);
			break;
		case 3://ЦВД
			ugc.SetDrawColor(0, 255, 0);
			break;
		case 4: // доп САД
		case 5: // доп ДАД
			ugc.SetDrawColor(110, 25, 5);
			break;
		case 6: // ЛCАД
		case 7: // ЛДАД
			ugc.SetDrawColor(220, 145, 5);
			break;
		case 8: // ЛСрАД
			ugc.SetDrawColor(170, 5, 210);
			break;


		}
	}



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
		

		int count = (type == static_cast<int>(BLOCK_TYPE::PLOT_PA)) ? 6 : 8;
		rect.height = headerHeight * count;

		
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
			if (OnLButtonUp2(x, y))
				return true;
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


	private:
		bool IsThisObject(int x, int y)
		{
			if (x >= rect.x + rect.reserved && x <= rect.x + rect.width
				&& y >= rect.y && y <= rect.y + rect.height)
				return true;
			return false;
		}

		bool OnLButtonUp2(int x, int y)
		{
			if (IsThisObject(x, y))
			{
				if (controller && objects.size()>0)
				{
					if (x > rect.x + rect.reserved)
					{
						x = x - rect.reserved - rect.x;
						double bitW = (rect.width - rect.reserved) / 25.;
						int minute = static_cast<int>(x / bitW * 60);
						const ContainerUnit* unitContainer = objects[0]->getContainerUnit();
						int unitN = unitContainer->find(minute);

						vector<ID> ids;
						for (const auto& obj : objects)
							ids.push_back(obj->getID());
						if (unitN >= 0)
							controller->updateUnitValues(ids, unitN);
						else
							controller->addParameterUnits(ids, minute / 60 * 60);
					}
					
					return true;
				}
			}
			return false;
		}
};