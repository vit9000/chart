// DutyCalendar.cpp: файл реализации
//

#include "stdafx.h"
#include "chart.h"
#include "DutyCalendar.h"
#include "afxdialogex.h"
#include "ugc.h"


// Диалоговое окно CDutyCalendar

IMPLEMENT_DYNAMIC(CDutyCalendar, CDialogEx)

CDutyCalendar::CDutyCalendar(const CRect& rect, IPickerUpdater * PickerUpdater, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DUTY, pParent), 
	calendar(PickerUpdater),
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
	ON_WM_PAINT()
END_MESSAGE_MAP()


BOOL CDutyCalendar::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	DPIX dpix;
	
	
	SetWindowPos(NULL, -1, -1, Width, Height, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	SetWindowPos(NULL, X, Y, 50, 50, SWP_NOSIZE | SWP_NOZORDER);

	calendar.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, Width, Height), this, NULL);

	ModifyStyleEx(WS_EX_APPWINDOW, 0);
	ModifyStyleEx(0, WS_EX_TOOLWINDOW);

	return TRUE;
}

//-------------------------------------------------------------------------
void CDutyCalendar::OnPaint()
{
	CDialogEx::OnPaint();

}
//-------------------------------------------------------------------------
