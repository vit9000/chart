
#include "stdafx.h"
#include "VCalendar.h"

BEGIN_MESSAGE_MAP(VCalendar, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

void VCalendar::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);
	/*ICloser * ptr = dynamic_cast<ICloser *>(GetParent());
	if(ptr)
		ptr->Close();*/
	
}
//-------------------------------------------------------------------------

void VCalendar::BuildValiables()
{
	COleDateTime currentDate = getSelectedDate();
	COleDateTime prevMonth(currentDate);
	GetPrevMonth(prevMonth);
	COleDateTime nextMonth(currentDate);
	GetNextMonth(nextMonth);

	currentDate.SetDate(currentDate.GetYear(), currentDate.GetMonth(), 1);
	int startDayOfWeek = currentDate.GetDayOfWeek() - 1;
	if (startDayOfWeek <= 0) startDayOfWeek += 7;
	int monthDaysPrev = COleDateTimeSpan(currentDate - prevMonth).GetTotalDays();;
	int monthDaysCurr = COleDateTimeSpan(nextMonth - currentDate).GetTotalDays();

	int day = monthDaysPrev - (startDayOfWeek - 2);
	int month = -1;
	for (int R = 0; R<ROWS; R++)
		for (int C = 0; C < COLS; C++)
		{
			if ((month == -1 && day>monthDaysPrev) || (month == 0 && day > monthDaysCurr))
			{
				day = 1;
				month++;
			}
			int pos = COLS * R + C;
			date[pos] = { day, currentDate.GetMonth() + month };
			day++;
		}


}
//-------------------------------------------------------------------------
void VCalendar::OnSize(UINT nType, int cx, int cy)
{
	CRect rect;
	GetClientRect(&rect);
	X = dpix(15);
	Y = dpix(15);
	Width = rect.Width() - X*2;
	Height = rect.Height() - Y*2;
	bitW = (Width / COLS);
	bitH = (Height / (2 + ROWS));
	Width = bitW * COLS;
	Height = bitH * (2+ROWS);
	CWnd::OnSize(nType, cx, cy);
}
//-------------------------------------------------------------------------
void VCalendar::OnPaint()
{
	CWnd::OnPaint();

	CRect rect;
	GetClientRect(rect);
	UGC ugc(GetDC(), rect.Width(), rect.Height());
	//ugc.SetDrawColor(255, 255, 255);
	//ugc.Clear();

	int dpi_one = dpix(1);
	int dpi_h = dpix(6);
	ugc.SetTextSize(12);
	int TextHeight12 = ugc.GetTextHeight(12);
	int TextHeight10 = ugc.GetTextHeight(11);
	//ugc.SetAlign(UGC::CENTER);

	

	
	ugc.SetDrawColor(0, 126, 186);
	ugc.FillRectangle(0, 0, rect.Width(), bitH);
	ugc.SetDrawColor(255, 255, 255);
	ugc.SetAlign(UGC::CENTER);
	ugc.DrawString(monthes[selectedDate.GetMonth()] + L" " + selectedDate.Format(L"%Y").GetBuffer(), rect.Width() / 2, bitH/2-TextHeight12/2);
	ugc.SetAlign(UGC::LEFT);

	int Xi = X;
	int Yi = Y+bitH*2;
	// выходные дни
	ugc.SetDrawColor(255, 255, 144);
	ugc.FillRectangle(Xi+bitW * 5, Yi, bitW * 2, bitH*ROWS);

	for (int R = 0; R<ROWS; R++)
		for (int C = 0; C < COLS; C++)
		{
			int pos = COLS * R + C;
			VDate& d = date[pos];
			int next_day = (pos + 1 >= COLS * ROWS) ? d.day + 1 : date[pos + 1].day; // следующий день дл€ прорисовки
			
			int _x = Xi + C * bitW;
			int _y = Yi + R * bitH;
			// рамка €чейки
			
			ugc.SetDrawColor(190, 190, 190);
			ugc.DrawRectangle(_x, _y, bitW, bitH);

			// дни недели
			if (R == 0)
			{
				ugc.SetTextSize(12);
				ugc.SetAlign(UGC::CENTER);
				ugc.SetDrawColor(0, 126, 186);
				ugc.DrawString(week_days.at(C), _x + bitW / 2, Yi - TextHeight12);
				ugc.SetAlign(UGC::LEFT);
			}

			// рисуем выбранной даты рамочку
			if (selectedDate.GetMonth() == d.month)
			{
				if (d.day == selectedDate.GetDay())
				{	
					ugc.SetDrawColor(255, 190, 0);
					ugc.FillRectangle(_x+dpi_one, _y+dpi_one, bitW- dpi_one, bitH- dpi_one);	
				}
				else if (d.month == currentDate.GetMonth() && d.day == currentDate.GetDay())
				{
					ugc.SetDrawColor(0, 255, 0);
					ugc.FillRectangle(_x + dpi_one, _y + dpi_one, bitW - dpi_one, bitH - dpi_one);
				}
				
				
				ugc.SetDrawColor(0, 126, 186);
			}
			else// если не текущий мес€ц рисуем сереньким
				ugc.SetDrawColor(190, 190, 190);

			
			// рисуем день
			ugc.SetTextSize(12);
			ugc.SetBold(true);
			ugc.DrawNumber(d.day, _x, _y);
			ugc.SetBold(false);
			ugc.SetAlign(UGC::RIGHT);
			ugc.SetTextSize(10);
			ugc.DrawNumber(next_day, _x+bitW, _y+bitW - TextHeight10);
			ugc.SetAlign(UGC::LEFT);
		}
	
}
//-------------------------------------------------------------------------
void VCalendar::OnLButtonDown(UINT nFlags, CPoint point)
{
	//CWnd::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------
void VCalendar::OnMouseMove(UINT nFlags, CPoint point)
{
	//CDialogEx::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------
void VCalendar::OnLButtonUp(UINT nFlags, CPoint point)
{
	int mX = point.x - X;
	int mY = point.y - Y - bitH*2;

	if (mX >= 0 && mX <= Width && Y >= 0 && Y <= Height)
	{
		int row = mY / bitH; // получаем строку
		int col = mX / bitW; // получаем €чейку
		if (row < 0 || row >= ROWS) return;
		if (col < 0 || col >= COLS) return;
		int pos = COLS * row + col;
		VDate& d = date[pos];

		COleDateTime dt = COleDateTime::GetCurrentTime();
		dt.SetDateTime(dt.GetYear(), d.month, d.day, startDutyHour, 0, 0);
		SetDate(dt);
		RedrawWindow();
		if (pickerUpdater)
			pickerUpdater->Update(selectedDate);
	}

}
//-------------------------------------------------------------------------