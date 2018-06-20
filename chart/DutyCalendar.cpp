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
	CDialogEx::OnCancel();
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
void VCalendar::Draw(UGC& ugc)
{
	
	COleDateTime currentDate = getCurrentDate();
	
	COleDateTime prevMonth(currentDate);
	CDutyCalendar::GetPrevMonth(prevMonth);
	COleDateTime nextMonth(currentDate);
	CDutyCalendar::GetNextMonth(nextMonth);

	currentDate.SetDate(currentDate.GetYear(), currentDate.GetMonth(), 1);
	int startDayOfWeek = currentDate.GetDayOfWeek()-1;
	int monthDaysPrev = COleDateTimeSpan(currentDate - prevMonth).GetTotalDays();;
	int monthDaysCurr = COleDateTimeSpan(nextMonth - currentDate).GetTotalDays();

	int month = -1;
	int day = monthDaysPrev - (startDayOfWeek-2);
	int dpi_one = dpix(1);
	
	int halfTextHeight = ugc.GetTextHeight()/2;
	ugc.SetAlign(UGC::CENTER);
	for(int l=0; l<lines; l++)
		for (int r = 0; r < rows; r++)
		{
			int pos = rows * l + r;
			int _x = x + r * bitW;
			int _y = y + l * bitH;
			// рамка ячейки
			ugc.SetDrawColor(190, 190, 190);
			ugc.DrawRectangle(_x, _y, bitW, bitH);
			// выходные дни
			if (r == 5 || r == 6)
			{
				ugc.SetDrawColor(255, 255, 180);
				ugc.FillRectangle(_x+dpi_one, _y+ dpi_one, bitW- dpi_one, bitH- dpi_one);
			}
			
			
			// дни недели
			if (l == 0)
			{
				ugc.SetDrawColor(0, 0, 255);
				ugc.DrawString(week_days.at(r), _x + bitW / 2, _y-bitW);
			}

			// переход через месяцы
			if ((month == -1 && day>monthDaysPrev) || (month == 0 && day > monthDaysCurr))
			{
				day = 1;
				month++;
			}

			
			
			if (month == 0)
			{
				if (day == currentDay)
				{
					ugc.SetDrawColor(255, 0, 0);
					int p = bitW / 24 * startDutyHour;
					if (r != 6)
					{
						ugc.FillRectangle(_x + p, _y + bitH - dpix(3), bitW, dpix(3));
					}
					else
					{
						ugc.FillRectangle(_x + p, _y + bitH - dpix(3), bitW-p, dpix(3));
						ugc.FillRectangle(x, _y + 2*bitH - dpix(3), p, dpix(3));
					}
				}
				ugc.SetDrawColor(0, 0, 255);
			}
			else// если не текущий месяц рисуем сереньким
				ugc.SetDrawColor(190, 190, 190);
			
			_x += bitW / 2;
			_y += halfTextHeight;
			// рисуем день
			ugc.DrawNumber(day, _x, _y);
			day++;
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
	calendar.OnLButtonClick(point.x, point.y);
	//CDialogEx::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------
bool VCalendar::OnLButtonClick(int X, int Y)
{
	if (X >= x && X <= x + width && Y >= y && Y <= y + height)
	{
	}
	return false;
}