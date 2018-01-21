#include "stdafx.h"
#include "CChartView.h"

BEGIN_MESSAGE_MAP(CChartView, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CChartView::CChartView()
{
	Width = 300;
	Height = 300;
	main_controller = new CMainController();
	tableModel.Register(this);
	//!!!!!!!!!!!!!!!!!!!!!!!!
	// удалить когда реализую добавление через интерфейс
	table_lines.push_back(new CTableLine(1, main_controller, Rect(0,0, Width, 100)));
	table_lines.push_back(new CTableLine(2, main_controller, Rect(0,100, Width, 100)));
//	
}
//------------------------------------------------------------
CChartView::~CChartView()
{
	for(size_t i=0; i<table_lines.size(); ++i)
		delete table_lines[i];
	delete main_controller;	
	DestroyWindow();
}
//------------------------------------------------------------
void CChartView::OnPaint()
{
	CWnd::OnPaint();

	CRect rect;
	GetClientRect(&rect);
	UGC ugc(GetDC(), rect.Width(), rect.Height());
	ugc.SetDrawColor(255,255,255);
	ugc.Clear();
	
	for(size_t i=0; i<table_lines.size(); ++i)
		table_lines[i]->OnPaint(ugc);

	ugc.SetDrawColor(255,0,0);
	Patient* patient = tableModel.getCurrentPatient();
	if(patient)
		ugc.DrawString(patient->getName(), 0, 0);
}
//------------------------------------------------------------
void CChartView::OnSize(UINT nType, int cx, int cy)
{
	SetBounds();
	for(size_t i=0; i<table_lines.size(); ++i)
			table_lines[i]->Resize(Rect(0,0,Width, Height));
	
	RedrawWindow();
	CWnd::OnSize(nType, cx, cy);
}
//------------------------------------------------------------
void CChartView::SetBounds()
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
void CChartView::OnLButtonUp(UINT flags, CPoint point)
{
	int x = point.x;
	int y = point.y;
	if(x >= 0 && x <= Width && y >= 0 && y <= Height)
	{
		for(size_t i=0; i<table_lines.size(); ++i)
			table_lines[i]->OnLButtonUp(x,y);
	}
}
//------------------------------------------------------------
void CChartView::OnMouseMove(UINT nFlags, CPoint point)
{
	
	int x = point.x;
	int y = point.y;
}
//------------------------------------------------------------