
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
END_MESSAGE_MAP()


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
	Width = rect.Width();
	Height = rect.Height();
	bitW = (Width / COLS);
	bitH = (Height / (1 + ROWS)),
	CWnd::OnSize(nType, cx, cy);
}
void VCalendar::OnPaint()
{

	CWnd::OnPaint();

	UGC ugc(GetDC(), Width, Height);
	//ugc.SetDrawColor(255, 255, 255);
	//ugc.Clear();

	int dpi_one = dpix(1);
	int dpi_h = dpix(6);
	int selected_month = selectedDate.GetMonth();
	ugc.SetTextSize(12);
	int TextHeight = ugc.GetTextHeight();
	//ugc.SetAlign(UGC::CENTER);

	int X = 0;
	int Y = bitH;

	// выходные дни
	ugc.SetDrawColor(255, 255, 180);
	ugc.FillRectangle(bitW * 5, Y, bitW * 2, bitW*ROWS);

	for (int R = 0; R<ROWS; R++)
		for (int C = 0; C < COLS; C++)
		{
			int pos = COLS * R + C;
			VDate& d = date[pos];
			int next_day = (pos + 1 >= COLS * ROWS) ? d.day + 1 : date[pos + 1].day; // следующий день дл€ прорисовки
			
			int _x = X + C * bitW;
			int _y = Y + R * bitH;
			// рамка €чейки
			ugc.SetDrawColor(190, 190, 190);
			ugc.DrawRectangle(_x, _y, bitW, bitH);

			// дни недели
			if (R == 0)
			{
				ugc.SetAlign(UGC::CENTER);
				ugc.SetDrawColor(50, 50, 50);
				ugc.DrawString(week_days.at(C), _x + bitW / 2, Y - TextHeight);
				ugc.SetAlign(UGC::LEFT);
			}

			// рисуем выбранной даты рамочку
			if (selected_month == d.month)
			{
				if (d.day == getSelectedDate().GetDay())
				{
					
					ugc.SetDrawColor(255, 0, 0);
					ugc.FillRectangle(_x+dpi_one, _y+dpi_one, bitW- dpi_one, bitH- dpi_one);
					
				}
				ugc.SetDrawColor(50, 50, 50);
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
			ugc.DrawNumber(next_day, _x+bitW, _y+bitW - TextHeight*2/3);
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
	int X = point.x;
	int Y = point.y - bitH;

	if (X >= 0 && X <= Width && Y >= 0 && Y <= Height)
	{
		int row = Y / bitH; // получаем строку
		int col = X / bitW; // получаем €чейку
		if (row < 0 || row >= ROWS) return;
		if (col < 0 || col >= COLS) return;
		int pos = COLS * row + col;
		VDate& d = date[pos];

		COleDateTime dt = COleDateTime::GetCurrentTime();
		dt.SetDateTime(dt.GetYear(), d.month, d.day, startDutyHour, 0, 0);
		SetDate(dt);
		RedrawWindow();
	}

}
//-------------------------------------------------------------------------