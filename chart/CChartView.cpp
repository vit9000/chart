#include "stdafx.h"
#include "CChartView.h"

BEGIN_MESSAGE_MAP(CChartView, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
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
void CChartView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// Получите минимальные и максимальные позиции слайдера.
	int minpos;
	int maxpos;

	
	this->GetScrollRange(SB_VERT, &minpos, &maxpos);
	maxpos = this->GetScrollLimit(SB_VERT);
	// Получите текущую позицию бегунка.
	int curpos = this->GetScrollPos(SB_VERT);
	// Определите новую позицию бегунка.
	switch (nSBCode)
	{
	case SB_BOTTOM:      // Прокрутка  далеко в лево.
		curpos = minpos;
		break;

	case SB_TOP:      // Прокрутка  далеко в право.
		curpos = maxpos;
		break;

	case SB_ENDSCROLL:   // Конечная прокрутка.
		break;

	case SB_LINEUP:      // Левая Прокрутка.
		if (curpos > minpos)
			curpos--;
		break;

	case SB_LINEDOWN:   // Правaя Прокруткa.
		if (curpos < maxpos)
			curpos++;
		break;

	/*case SB_PAGEDOWN:    
	{
		// Получите размер страницы. 
		SCROLLINFO   info;
		this->GetScrollInfo(SB_VERT,&info, SIF_ALL);

		if (curpos > minpos)
			curpos = max(minpos, curpos - (int)info.nPage);
	}
	break;

	case SB_PAGEUP:      
	{
		// Получите размер страницы. 
		SCROLLINFO   info;
		this->GetScrollInfo(SB_VERT,&info, SIF_ALL);

		if (curpos < maxpos)
			curpos = min(maxpos, curpos + (int)info.nPage);
	}
	break;*/

	case SB_THUMBPOSITION: // Прокруткa к абсолютной позиции. nPos - позиция
		curpos = nPos;      // Из бегунка в конце перетащенной операции.

		break;

	case SB_THUMBTRACK:   // Перетащите бегунок к определенной позиции. nPos -
		curpos = nPos;     // Позиция, к которой бегунок перемещен.
		break;
	}

	// Установите новую позицию бегунка 
	this->SetScrollPos(SB_VERT,curpos);
	if (table_container)
		table_container->setScroll(curpos);
	RedrawWindow();
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CChartView::OnPaint()
{
	CWnd::OnPaint();

	

	CRect rect;
	GetClientRect(&rect);
	UGC ugc(GetDC(), Width, Height);
	ugc.SetDrawColor(255,255,255);
	ugc.Clear();
	
	

	ugc.SetAlign(UGC::LEFT);

	table_container->OnPaint(ugc);

	ugc.SetAlign(UGC::CENTER);
	

	
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
	GetClientRect(&rect);
	
	Width = rect.Width();
	Height = rect.Height();

	int contentHeight = table_container->getContentHeight();
	int scroll = Height - contentHeight;
	if (scroll < 0) scroll = -scroll;
	else
	{
		int minpos, maxpos;
		this->GetScrollRange(SB_VERT, &minpos, &maxpos);
		if (maxpos == 0) return;
		scroll = 0;
	}
	this->SetScrollRange(SB_VERT, 0, scroll);
	this->SetScrollPos(SB_VERT, 0);
	table_container->setScroll(0);

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
	SetBounds();
	RedrawWindow();

	

}