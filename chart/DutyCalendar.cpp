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
	: CDialogEx(IDD_DIALOG_DUTY, pParent), X(rect.left), Y(rect.top), Width(rect.Width()), Height(rect.Height())
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
	ugc.SetDrawColor(225, 225, 185);

	ugc.Clear();
	ugc.SetDrawColor(0, 0, 0);

	int border = ugc.AssignToDPI(25);
	DrawCalendar(ugc, border, border, Width-border*2, Height-border*2);
}
//-------------------------------------------------------------------------
void CDutyCalendar::DrawCalendar(UGC& ugc, int x, int y, int width, int height)
{
	int lines = 5;
	int rows = 7;
	int w = width / rows;
	int h = height / lines;

	COleDateTime currentDate = COleDateTime::GetCurrentTime();
	int currentDay = currentDate.GetDay();
	COleDateTime prevMonth(currentDate);
	GetPrevMonth(prevMonth);
	COleDateTime nextMonth(currentDate);
	GetNextMonth(nextMonth);

	currentDate.SetDate(currentDate.GetYear(), currentDate.GetMonth(), 1);
	int startDayOfWeek = currentDate.GetDayOfWeek()-1;
	int monthDaysPrev = COleDateTimeSpan(currentDate - prevMonth).GetTotalDays();;
	int monthDaysCurr = COleDateTimeSpan(nextMonth - currentDate).GetTotalDays();

	int month = -1;
	int day = monthDaysPrev - (startDayOfWeek-2);
	for(int l=0; l<lines; l++)
		for (int r = 0; r < rows; r++)
		{
			int pos = rows * l + r;
			int _x = x + r * w;
			int _y = y + l * h;


			if ((month == -1 && day>monthDaysPrev) || (month == 0 && day > monthDaysCurr))
			{
				day = 1;
				month++;
			}
			
			if(month==0)
				ugc.DrawRectangle(_x, _y, w, h);
			else
			{
				ugc.SetDrawColor(90, 90, 90);
				ugc.FillRectangle(_x, _y, w, h);
				ugc.SetDrawColor(0, 0, 0);
			}

			ugc.DrawNumber(day, _x, _y);
			day++;
		}
}
//-------------------------------------------------------------------------
void CDutyCalendar::OnLButtonDown(UINT nFlags, CPoint point)
{
	//CWnd::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------
void CDutyCalendar::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialogEx::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------
void CDutyCalendar::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------
