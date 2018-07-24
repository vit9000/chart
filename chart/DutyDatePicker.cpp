
#include "stdafx.h"
#include "resource.h"
#include "DutyDatePicker.h"

BEGIN_MESSAGE_MAP(CDutyDatePicker, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
END_MESSAGE_MAP()


CDutyDatePicker::CDutyDatePicker()
	:
	Width(100),
	Height(100),
	isOpen(false),
	allowOpen(true),
	m_bMouseTracking(false)
{}
//-------------------------------------------------------------------------
void CDutyDatePicker::ParseDateTime(const wstring& StartDutyTime)
{
	COleDateTime&  st = currentDuty.startDutyDatetime;
	st = COleDateTime::GetCurrentTime();

	COleDateTime startDutyTime;
	if (!startDutyTime.ParseDateTime(StartDutyTime.c_str()))
	{
		MessageBox(L"¬рем€ начала дежурства введена в неверном формате. ѕо-умолчанию будет использоватьс€ 09:00", L"¬нимание", MB_OK | MB_ICONINFORMATION);
		startDutyTime.ParseDateTime(L"9:00:00");
	}
	if (st.GetHour() < startDutyTime.GetHour() ||
		(st.GetHour() == startDutyTime.GetHour() && st.GetMinute() < startDutyTime.GetMinute()))
		st -= COleDateTimeSpan(1, 0, 0, 0);

	st.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), startDutyTime.GetHour(), startDutyTime.GetMinute(), startDutyTime.GetSecond()); // врем€ и дата начала текущего дежурства установлено
	currentDuty.endDutyDatetime = currentDuty.startDutyDatetime + COleDateTimeSpan(1, 0, 0, 0);
}
//-------------------------------------------------------------------------
CDutyDatePicker::~CDutyDatePicker()
{
}
//-------------------------------------------------------------------------
void CDutyDatePicker::OnDestroy()
{
	CWnd::OnDestroy();
}
//-------------------------------------------------------------------------
void CDutyDatePicker::OnPaint()
{
	CWnd::OnPaint();

	UGC ugc(GetDC(), Width, Height);
	ugc.SetDrawColor(255, 255, 135);
	
	ugc.Clear();

	wstring dutyDate;
	wstring dutyTime;

	ugc.SetAlign(UGC::CENTER);

	int border = (Height - ugc.GetTextHeight(18)) / 2;

	DateToString(currentDuty.startDutyDatetime, dutyDate);
	TimeToString(currentDuty.startDutyDatetime, dutyTime);
	DrawSector(ugc, dutyDate, dutyTime, border, Width/2, border);

	DateToString(currentDuty.endDutyDatetime, dutyDate);
	TimeToString(currentDuty.endDutyDatetime, dutyTime);
	DrawSector(ugc, dutyDate, dutyTime, Width/2-border, Width / 2, border);

	ugc.SetDrawColor(0, 0, 0);
	ugc.DrawLine(Width/2-border, Height/2, Width/2+border, Height/2);

	border = ugc.getDPIX()(8);
	if(m_bMouseTracking)
	{
		int xi = Width - border * 2;
		int half = border / 2;
		int yi = Height / 2;
		if (!isOpen)
		{
			yi += half / 2;
			ugc.DrawLineAntialiased(xi, yi - half, xi + half, yi);
			ugc.DrawLineAntialiased(xi + half, yi, xi + half*2, yi-half);
		}
		else
		{
			yi -= half / 2;
			ugc.DrawLineAntialiased(xi, yi + half, xi + half, yi);
			ugc.DrawLineAntialiased(xi + half, yi, xi + half * 2, yi + half);
		}
	}
	
	ugc.SetAlign(UGC::LEFT);
}
//-------------------------------------------------------------------------
void CDutyDatePicker::DrawSector(UGC& ugc, const wstring& date, const wstring& time, int x, int width, int border)
{
	ugc.SetDrawColor(55, 55, 55);
	ugc.SetTextSize(12);
	ugc.SetBold(true);
	ugc.DrawString(date, x + width / 2, border);
	//ugc.SetDrawColor(0, 0, 0);
	ugc.SetTextSize(10);
	ugc.SetBold(false);
	ugc.DrawString(time, x + width / 2, Height - ugc.GetTextHeight() - border);
}
//-------------------------------------------------------------------------
void CDutyDatePicker::OnSize(UINT nType, int cx, int cy)
{
	CRect rect;
	GetClientRect(&rect);
	Width = rect.Width();
	Height = rect.Height();
	CWnd::OnSize(nType, cx, cy);
}
//-------------------------------------------------------------------------
void CDutyDatePicker::OnLButtonDown(UINT nFlags, CPoint point)
{
	
}
//-------------------------------------------------------------------------
void CDutyDatePicker::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bMouseTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = this->m_hWnd;

		if (::_TrackMouseEvent(&tme))
		{
			m_bMouseTracking = true;
		}
		allowOpen = !isOpen;
		RedrawWindow();
	}
	
}
//-------------------------------------------------------------------------
void CDutyDatePicker::OnMouseLeave()
{
	allowOpen = true;
	m_bMouseTracking = false;
	RedrawWindow();
}
//-------------------------------------------------------------------------
void CDutyDatePicker::OnLButtonUp(UINT nFlags, CPoint point)
{	
	if (!allowOpen)
	{
		allowOpen = true;
		return;
	}
	allowOpen = false;
	RECT rect;
	GetWindowRect(&rect);
	rect.top += Height;
	rect.bottom = rect.top+Width;
	CDutyCalendar dlg(rect, this, currentDuty.startDutyDatetime, { currentDuty.startDutyDatetime.GetHour(), currentDuty.startDutyDatetime.GetMinute() });
	dlg.DoModal();
}
//-------------------------------------------------------------------------
int CDutyDatePicker::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetFocus();
	return 0;
}
//-------------------------------------------------------------------------