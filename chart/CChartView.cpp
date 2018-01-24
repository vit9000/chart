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
	
	ugc.SetDrawColor(0,0,0);
	int linesCount = Height / table_container->LINE_HEIGHT;
	for(int i=0; i<linesCount; ++i)
		ugc.DrawLine(0,i*table_container->LINE_HEIGHT, Width, i*table_container->LINE_HEIGHT);

	ugc.SetAlign(CENTER);
	ugc.SetTextSize(12);
	int headerWidth = table_container->getHeaderWidth();
	int columnWidth = table_container->getColumnWidth();
	for(int i=0; i<=table_container->HOUR_COUNT; ++i)
	{
		int x = headerWidth + i*columnWidth;
		ugc.DrawLine(x,0,x,Height);

		if(i!=table_container->HOUR_COUNT)
		{
			int number = 9+i;
			if(number>=24) number-=24;
			ugc.DrawNumber(number, x+columnWidth/2, table_container->LINE_HEIGHT/2 - ugc.GetTextHeight()/2);
		}
	}
	ugc.SetAlign(LEFT);

	table_container->OnPaint(ugc);

	// Draw round with +
	ugc.SetDrawColor(155,155,155);
	int y = static_cast<int>(table_container->getCount()) * table_container->LINE_HEIGHT;
	ugc.FillEllipse(0, 
					y+static_cast<int>(table_container->LINE_HEIGHT*1.1), 
					static_cast<int>(table_container->LINE_HEIGHT*0.8));
	ugc.SetDrawColor(255,255,255);
	ugc.SetAlign(CENTER);
	ugc.SetTextSize(20);
	ugc.DrawString(L"+", static_cast<int>(table_container->LINE_HEIGHT*0.4), y+static_cast<int>(table_container->LINE_HEIGHT*1.5)-ugc.GetTextHeight()/2);
	ugc.SetAlign(LEFT);


	/*ugc.SetDrawColor(255,0,0);
	Patient* patient = model->getCurrentPatient();
	if(patient)
		ugc.DrawString(patient->getName(), 0, 0);*/
}
//------------------------------------------------------------
void CChartView::OnSize(UINT nType, int cx, int cy)
{
	SetBounds();
	
	//table_container->Resize(Rect(0,0,Width, Height));
	table_container->Resize(Rect(0,table_container->LINE_HEIGHT, Width, Height-table_container->LINE_HEIGHT));
		
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
	if(x >= 0 && x <= Width && y >= table_container->LINE_HEIGHT && y <= Height)
	{
		if(table_container->OnLButtonUp(x,y))
			return;
		main_controller->addDrug();
	}
}
//------------------------------------------------------------
void CChartView::OnMouseMove(UINT nFlags, CPoint point)
{
	
	int x = point.x;
	int y = point.y;
}
//------------------------------------------------------------

void CChartView::Update(vector<shared_ptr<ITableCommand> >& table_commands)
{
	for(size_t i=0; i<table_commands.size(); ++i)
	{
		table_commands[i]->Execute(table_container);
	}
	RedrawWindow();

	

}