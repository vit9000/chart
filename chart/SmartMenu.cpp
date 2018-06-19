// SmartMenu.cpp : implementation file
//

#include "stdafx.h"
#include "chart.h"
#include "SmartMenu.h"
#include "afxdialogex.h"


// SmartMenu dialog

IMPLEMENT_DYNAMIC(CSmartMenu, CDialog)

CSmartMenu::CSmartMenu(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SMART_MENU, pParent), 
	ITEM_HEIGHT(DPIX().getIntegerValue(20)), x(0), y(0), selected(-1),
	mouseTracked(false)
{}

CSmartMenu::~CSmartMenu()
{}


void CSmartMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSmartMenu, CDialog)
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

void CSmartMenu::Init(int X, int Y, const MENU& Menu)
{
	x = X;
	y = Y;
	menu = std::move(Menu);
}
//---------------------------------------------------------------------
void CSmartMenu::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	delete this;
}
//---------------------------------------------------------------------
BOOL CSmartMenu::OnInitDialog()
{
	CDialog::OnInitDialog();
	DPIX dpix;
	width = dpix.getIntegerValue(200);
	height = static_cast<int>(menu.size()) * ITEM_HEIGHT + dpix.getIntegerValue(2);
	SetWindowPos(NULL, -1,-1, width, height, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	SetWindowPos(NULL, x, y-DPIX().getIntegerValue(10), 50, 50, SWP_NOSIZE | SWP_NOZORDER);

	bgColor = convertColor(GetSysColor(COLOR_MENU));
	highlightColor = convertColor(GetSysColor(COLOR_MENUHILIGHT));
	notselectedColor = convertColor(GetSysColor(COLOR_INACTIVECAPTION));
	ModifyStyleEx(WS_EX_APPWINDOW, 0);	
	ModifyStyleEx(0, WS_EX_TOOLWINDOW);
	return TRUE;
}
//---------------------------------------------------------------------
void CSmartMenu::OnKillFocus(CWnd* pNewWnd)
{
	CDialog::OnKillFocus(pNewWnd);
	DestroyWindow();
}
//---------------------------------------------------------------------
void CSmartMenu::OnPaint()
{
	
	UGC ugc(GetDC(), width, height);
	ugc.SetDrawColor(bgColor);
	ugc.Clear();
	ugc.SetTextSize(10);
	

	int index = 0;
	int fontHeight = ugc.GetTextHeight();
	for (const auto& str : menu)
	{
		int y = index*ITEM_HEIGHT;
		ugc.SetDrawColor(0, 0, 0);
		if (str.second)
		{
			ugc.SetDrawColor(notselectedColor);
			ugc.DrawDashLine(0, y, width, y);
			ugc.SetDrawColor(0, 0, 0);
			if (index == selected)
			{
				ugc.SetDrawColor(highlightColor);
				ugc.FillRectangle(0, y+1, width, ITEM_HEIGHT-1);
				ugc.SetDrawColor(255, 255, 255);
			}
			
		}
		ugc.DrawString(str.first, 0,y+ITEM_HEIGHT/2 - fontHeight/2);
		index++;
	}

	CDialog::OnPaint();
}
//---------------------------------------------------------------------

void CSmartMenu::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!mouseTracked)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 1;
		_TrackMouseEvent(&tme);
		mouseTracked = true;
	}

	int index = point.y / ITEM_HEIGHT;
	if (index >= static_cast<int>(menu.size()))
		index = -1;
	if (selected == index) return;

	selected = index;
	RedrawWindow();
}
//---------------------------------------------------------------------
LRESULT CSmartMenu::OnMouseLeave(WPARAM WParam, LPARAM LParam)
{
	if(selected!=-1)
	{
		mouseTracked = false;
		selected = -1;
		RedrawWindow();
	}
	
	return CDialog::Default();
}
//---------------------------------------------------------------------
void CSmartMenu::OnLButtonUp(UINT flags, CPoint point)
{
	if (selected < 0) return;

	auto& func = menu[selected].second;
	if (func)
	{
		std::thread t(func);
		t.detach();
	}
}
//---------------------------------------------------------------------
void CSmartMenu::OnLButtonDown(UINT flags, CPoint point)
{
	DestroyWindow();
}
//---------------------------------------------------------------------
