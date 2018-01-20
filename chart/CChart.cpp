#include "stdafx.h"
#include "CChart.h"

BEGIN_MESSAGE_MAP(CChart, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CChart::CChart()
{
	Width = 300;
	Height = 300;
	tableView = null;
//	
}
//------------------------------------------------------------
CChart::~CChart()
{
	delete tableView;
	DestroyWindow();
}
//------------------------------------------------------------
void CChart::OnPaint()
{
	CWnd::OnPaint();

	CRect rect;
	GetClientRect(&rect);
	UGC ugc(GetDC(), rect.Width(), rect.Height());
	ugc.SetDrawColor(255,0,0);
	ugc.Clear();
	
	if(tableView)
		tableView->OnPaint(ugc);
}
//------------------------------------------------------------
void CChart::OnSize(UINT nType, int cx, int cy)
{
	SetBounds();
	if(!tableView)
		tableView = new TableView(Rect(0,0,Width, Height));
	else
		tableView->Resize(Rect(0,0,Width, Height));
	RedrawWindow();
	CWnd::OnSize(nType, cx, cy);
}
//------------------------------------------------------------
void CChart::SetBounds()
{
	
	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.bottom -= rect.top;
	rect.right -= rect.left;

	Width = rect.Width();
	Height = rect.Height();

	
}
//------------------------------------------------------------
void CChart::OnLButtonUp(UINT flags, CPoint point)
{
	int x = point.x;
	int y = point.y;
	if(tableView)
		tableView->OnLButtonUp(x, y);
}
//------------------------------------------------------------
void CChart::OnMouseMove(UINT nFlags, CPoint point)
{
	
	int x = point.x;
	int y = point.y;
}
//------------------------------------------------------------