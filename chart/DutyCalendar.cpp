// DutyCalendar.cpp: файл реализации
//

#include "stdafx.h"
#include "chart.h"
#include "DutyCalendar.h"
#include "afxdialogex.h"
#include "ugc.h"


// Диалоговое окно CDutyCalendar

IMPLEMENT_DYNAMIC(CDutyCalendar, CDialogEx)

CDutyCalendar::CDutyCalendar(const CRect& rect, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DUTY, pParent), 
	X(rect.left), 
	Y(rect.top), 
	Width(rect.Width()), 
	Height(rect.Height()),
	calendar(dpix(40), dpix(40), Width-dpix(80), Height-dpix(80))
{

}

CDutyCalendar::~CDutyCalendar()
{
}

void CDutyCalendar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDutyCalendar, CDialogEx)

	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_WM_CHAR()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


BOOL CDutyCalendar::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	DPIX dpix;
	SetWindowPos(NULL, -1, -1, Width, Height, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	SetWindowPos(NULL, X, Y, 50, 50, SWP_NOSIZE | SWP_NOZORDER);

	ModifyStyleEx(WS_EX_APPWINDOW, 0);
	ModifyStyleEx(0, WS_EX_TOOLWINDOW);
	return TRUE;
}

void CDutyCalendar::OnKillFocus(CWnd* pNewWnd)
{
	CDialogEx::OnKillFocus(pNewWnd);
	//CDialogEx::OnCancel();
}
//-------------------------------------------------------------------------
void CDutyCalendar::OnPaint()
{
	CDialogEx::OnPaint();

	UGC ugc(GetDC(), Width, Height);
	ugc.SetDrawColor(255, 255, 255);

	ugc.Clear();
	ugc.SetDrawColor(0, 0, 0);

	int border = ugc.AssignToDPI(25);
	calendar.Draw(ugc);
}
//-------------------------------------------------------------------------
void VCalendar::BuildValiables()
{
	COleDateTime currentDate = getCurrentDate();
	COleDateTime prevMonth(currentDate);
	GetPrevMonth(prevMonth);
	COleDateTime nextMonth(currentDate);
	GetNextMonth(nextMonth);

	currentDate.SetDate(currentDate.GetYear(), currentDate.GetMonth(), 1);
	int startDayOfWeek = currentDate.GetDayOfWeek() - 1;
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
			date[R][C] = {day, currentDate.GetMonth()+month};
			day++;
		}


}
//-------------------------------------------------------------------------
void VCalendar::Draw(UGC& ugc)
{
	int dpi_one = dpix(1);
	int dpi_h = dpix(6);
	int month = currentDate.GetMonth();
	int halfTextHeight = ugc.GetTextHeight()/2;
	ugc.SetAlign(UGC::CENTER);
	for(int R=0; R<ROWS; R++)
		for (int C = 0; C < COLS; C++)
		{
			VDate& d = date[R][C];

			//int pos = rows * l + r;
			int _x = x + C * bitW;
			int _y = y + R * bitH;
			// рамка ячейки
			ugc.SetDrawColor(190, 190, 190);
			ugc.DrawRectangle(_x, _y, bitW, bitH);
			// выходные дни
			if (C+1 == 6 || C+1 == 7)
			{
				ugc.SetDrawColor(255, 255, 180);
				ugc.FillRectangle(_x+dpi_one, _y+ dpi_one, bitW- dpi_one, bitH- dpi_one);
			}
			
			// дни недели
			if (R == 0)
			{
				ugc.SetDrawColor(0, 0, 255);
				ugc.DrawString(week_days.at(C), _x + bitW / 2, _y-bitW/2- halfTextHeight);
			}

			
			// рисуем рамочку
			if (month == d.month)
			{
				if (d.day == currentDay)
				{
					
					int p = bitW / 24 * startDutyHour;
					if (C+1 != 7)
					{
						ugc.SetDrawColor(255, 0, 0);
						ugc.FillRectangle(_x + p, _y + bitH - dpi_h, bitW, dpi_h);
						//ugc.SetDrawColor(100,255, 0, 0);
						ugc.FillRectangle(_x + p, _y + dpi_one, bitW, dpi_h);
					}
					else
					{
						ugc.SetDrawColor(255, 0, 0);
						ugc.FillRectangle(_x + p, _y + bitH - dpi_h, bitW - p, dpi_h);
						ugc.FillRectangle(x, _y + 2 * bitH - dpi_h, p, dpi_h);
						//ugc.SetDrawColor(100, 255, 0, 0);
						ugc.FillRectangle(_x + p, _y+dpi_one, bitW - p, dpi_h);
						ugc.FillRectangle(x, _y + dpi_one, p, dpi_h);
					}
				}
				ugc.SetDrawColor(0, 0, 255);
			}
			else// если не текущий месяц рисуем сереньким
				ugc.SetDrawColor(190, 190, 190);
			
			_x += bitW / 2;
			_y += halfTextHeight;
			// рисуем день
			ugc.DrawNumber(d.day, _x, _y);
		}
	ugc.SetAlign(UGC::LEFT);
}
//-------------------------------------------------------------------------
void CDutyCalendar::OnLButtonDown(UINT nFlags, CPoint point)
{
	//CWnd::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------
void CDutyCalendar::OnMouseMove(UINT nFlags, CPoint point)
{
	//CDialogEx::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------
void CDutyCalendar::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(calendar.OnLButtonClick(point.x, point.y))
		RedrawWindow();
	//CDialogEx::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------
bool VCalendar::OnLButtonClick(int X, int Y)
{
	if (X >= x && X <= x + width && Y >= y && Y <= y + height)
	{
		int row = Y / bitH - 1; // получаем строку
		int col = X / bitW - 1; // получаем ячейку
		if (row < 0 || row >= ROWS) return false;
		if (col < 0 || col >= COLS) return false;

		VDate& d = date[row][col];

		COleDateTime dt = COleDateTime::GetCurrentTime();
		dt.SetDateTime(dt.GetYear(), d.month, d.day, startDutyHour, 0, 0);
		SetDate(dt);
		return true;
	}
	return false;
}