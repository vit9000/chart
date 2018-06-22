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
	Height(rect.Height())
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

	calendar.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(dpix(40), dpix(40), Width-dpix(40), Height-dpix(40)), this, NULL);

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
	//ugc.SetDrawColor(255, 255, 255);
	//ugc.Clear();
	ugc.SetDrawColor(0, 0, 0);
	
	ugc.DrawNumber(calendar.getSelectedDate().GetMonth(), 10, 10);
	
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
	
	//CDialogEx::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------
