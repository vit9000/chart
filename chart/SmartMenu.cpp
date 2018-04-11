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
	ITEM_HEIGHT(DPIX().getIntegerValue(20)), x(0), y(0), selected(-1)
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
	width = DPIX().getIntegerValue(200);
	height = static_cast<int>(menu.size()) * ITEM_HEIGHT;
	SetWindowPos(NULL, -1,-1, width, height, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	SetWindowPos(NULL, x, y-DPIX().getIntegerValue(10), 50, 50, SWP_NOSIZE | SWP_NOZORDER);

	bgColor = convertColor(GetSysColor(COLOR_MENU));
	highlightColor = convertColor(GetSysColor(COLOR_MENUHILIGHT));
	notselectedColor = convertColor(GetSysColor(COLOR_INACTIVECAPTION));
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
	ugc.SetDrawColor(notselectedColor);
	ugc.Clear();
	ugc.SetTextSize(10);
	
	ugc.SetDrawColor(notselectedColor);
	ugc.DrawRectangle(0, 0, width-1, height-1);

	int index = 0;
	int fontHeight = ugc.GetTextHeight();
	for (const auto& str : menu)
	{
		int y = index*ITEM_HEIGHT;
		ugc.SetDrawColor(0, 0, 0);
		if (str.second)
		{
			if (index == selected)
			{
				ugc.SetDrawColor(highlightColor);
			}
			else
				ugc.SetDrawColor(bgColor);
			ugc.FillRectangle(2, y+1, width-4, ITEM_HEIGHT-2);
			if (index == selected)
				ugc.SetDrawColor(255, 255, 255);
			else
				ugc.SetDrawColor(0, 0, 0);
		}
		ugc.DrawString(str.first, 0,y+ITEM_HEIGHT/2 - fontHeight/2);
		index++;
	}

	CDialog::OnPaint();
}
//---------------------------------------------------------------------
void CSmartMenu::OnMouseMove(UINT nFlags, CPoint point)
{
	int index = point.y / ITEM_HEIGHT;
	if (index >= menu.size())
		index = -1;
	if (selected == index) return;

	selected = index;
	RedrawWindow();
}