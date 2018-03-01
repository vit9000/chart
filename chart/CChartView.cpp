#include "stdafx.h"
#include "CChartView.h"

BEGIN_MESSAGE_MAP(CChartView, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CChartView::CChartView()
{
	Width = 300;
	Height = 300;
	model = new CMainModel();
	model->Register(this);
	main_controller = new CMainController(model);
	table_container = new CTableContainer(main_controller, Rect(0,0,Width, Height));
	
}
//------------------------------------------------------------
CChartView::~CChartView()
{
	delete table_container;
	delete main_controller;	
	delete model;
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
	
	
	ugc.SetAlign(UGC::LEFT);

	table_container->OnPaint(ugc);

	ugc.SetAlign(UGC::CENTER);
	// Draw round with +
	ugc.SetDrawColor(105,105,105);
	
	//Rect r = table_container->getRectByIndex(table_container->getCount() - 1);
	
	/*int h = table_container->getLineHeight();
	ugc.FillEllipse(10, Height-h*2, h*2);
	//ugc.DrawString(L"добавить назначение...", 0, r.y+r.height);
	ugc.SetDrawColor(255, 255, 255);
	ugc.SetTextSize(22);
	ugc.DrawString(L"+", 10 + r.height, 10 + r.y + r.height*2-ugc.GetTextHeight() / 2);
	ugc.SetAlign(UGC::LEFT);
	*/
	
	


	/*ugc.SetDrawColor(255,0,0);
	ChartData* ChartData = model->getCurrentChartData();
	if(ChartData)
		ugc.DrawString(ChartData->getName(), 0, 0);*/
}
//------------------------------------------------------------
void CChartView::OnSize(UINT nType, int cx, int cy)
{
	SetBounds();
	
	//table_container->Resize(Rect(0,0,Width, Height));
	table_container->Resize(Rect(0,table_container->getLineHeight(), Width, Height-table_container->getLineHeight()));
		
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
	if(x >= 0 && x <= Width && y >= table_container->getLineHeight() && y <= Height)
	{
		if(table_container->OnLButtonUp(x,y))
			return;
		if(y>Height-100)
			main_controller->addDrug();
	}
}
//------------------------------------------------------------
void CChartView::OnLButtonDown(UINT flags, CPoint point)
{
	int x = point.x;
	int y = point.y;
	if (x >= 0 && x <= Width && y >= table_container->getLineHeight() && y <= Height)
	{
		if (table_container->OnLButtonDown(x, y))
			RedrawWindow();
	}
}
//------------------------------------------------------------
void CChartView::OnMouseMove(UINT nFlags, CPoint point)
{
	
	int x = point.x;
	int y = point.y;
	if (x >= 0 && x <= Width && y >= table_container->getLineHeight() && y <= Height)
	{
		if (table_container->OnMouseMove(x, y) || table_container->IsMoveAborted())
			RedrawWindow();
	}
}
//------------------------------------------------------------

void CChartView::Update(vector<TableCommand_Ptr>& table_commands)
{
	for(size_t i=0; i<table_commands.size(); ++i)
	{
		table_commands[i]->Execute(table_container);
	}
	RedrawWindow();

	

}