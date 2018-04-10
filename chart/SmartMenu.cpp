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
	x(0), y(0)
{

}

CSmartMenu::~CSmartMenu()
{
}


void CSmartMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSmartMenu, CDialog)
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CSmartMenu::Init(int X, int Y, const vector<wstring>& Info)
{
	x = X;
	y = Y;
	info = std::move(Info);
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
	SetWindowPos(NULL, x, y-DPIX().getIntegerValue(10), 0, 0, SWP_NOSIZE | SWP_NOZORDER);
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
	
	CRect rect;
	GetClientRect(&rect);
	UGC ugc(GetDC(), rect.right, rect.bottom);
	ugc.SetDrawColor(255, 255, 255);
	ugc.Clear();
	ugc.SetTextSize(12);
	
	ugc.SetDrawColor(0, 0, 0);
	ugc.DrawRectangle(0, 0, ugc.getWidth()-1, ugc.getHeight()-1);

	int index = 0;
	int fontHeight = ugc.GetTextHeight();
	for (const auto& str : info)
	{
		ugc.DrawString(str, 0, index*fontHeight);
		index++;
	}

	CDialog::OnPaint();
}
//---------------------------------------------------------------------
void CSmartMenu::OnMouseMove(UINT nFlags, CPoint point)
{

	int x = point.x;
	int y = point.y;
	RECT r;
	GetClientRect(&r);
	int Width = r.right;
	int Height = r.bottom;
	if (x >= 0 && x <= Width && y >= 0 && y <= Height)
	{
		
	}
	
}