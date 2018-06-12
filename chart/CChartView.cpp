#include "stdafx.h"
#include "CChartView.h"
#include "CPrintController.h"
#include "CNurseController.h"

BEGIN_MESSAGE_MAP(CChartView, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYUP()
END_MESSAGE_MAP()


CChartView::CChartView(bool _print_mode)
	:	Width(300),
		Height(300),
		cursor_type(0),
		print_mode(_print_mode)
{
	model = new CMainModel();
	model->Register(this);
	main_controller = new CMainController(model, this);


	table_container = new CTableContainer(&main_controller, Rect(0,0,Width, Height));	
}
//------------------------------------------------------------
CChartView::~CChartView()
{
	delete table_container;
	delete main_controller;	
	delete model;
	DestroyWindow();
}

void CChartView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
	if (nChar != 77) // m
	{
		CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
		return;
	}
	if (!main_controller) return;

	IChartController* new_controller;
	switch (main_controller->MODE)
	{

	case ACCESS::FULL_ACCESS:
		new_controller = new CNurseController(model, this);
//		MessageBox(L"Включен режим мед.сестры");
		break;
	case ACCESS::NURSE_ACCESS:
		new_controller = new CPrintController(model);
//		MessageBox(L"Включен режим просмотра");
		break;
	
	case ACCESS::VIEW_ACCESS:
		new_controller = new CMainController(model, this);
//		MessageBox(L"Включен режим врача");
		break;
	}
	delete main_controller;

	main_controller = new_controller;
	new_controller = nullptr;
	table_container->SetButtonsVisible();

	RedrawWindow();
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
	DPIX dpix;
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
			curpos-= dpix.getIntegerValue(5);
		break;

	case SB_LINEDOWN:   // Правaя Прокруткa.
		if (curpos < maxpos)
			curpos+= dpix.getIntegerValue(5);
		break;

	case SB_PAGEDOWN:    
	{
		if (curpos < maxpos)
			curpos+= dpix.getIntegerValue(10);
		if (curpos > maxpos) curpos = maxpos;
		

		
	}
	break;

	case SB_PAGEUP:      
	{
		if (curpos > minpos)
			curpos -= dpix.getIntegerValue(10);
		if (curpos < minpos) curpos = minpos;

	}
	break;

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

BOOL CChartView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int minpos;
	int maxpos;
	this->GetScrollRange(SB_VERT, &minpos, &maxpos);
	maxpos = this->GetScrollLimit(SB_VERT);
	// Получите текущую позицию бегунка.
	int curpos = this->GetScrollPos(SB_VERT);
	DPIX dpix;
	if (zDelta < 0 && curpos < maxpos)
		curpos+= dpix.getIntegerValue(10);
	else if (zDelta > 0 && curpos > minpos)
		curpos-= dpix.getIntegerValue(10);

	if (curpos > maxpos) curpos = maxpos;
	else if (curpos < minpos) curpos = minpos;
	

	this->SetScrollPos(SB_VERT, curpos);
	if (table_container)
		table_container->setScroll(curpos);
	RedrawWindow();

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);;
}
//------------------------------------------------------------
void CChartView::PrintAll(HBITMAP *hbitmap)
{
	DPIX dpix;
	int width = dpix.getIntegerValue(1024);
	int height = table_container->getContentHeight();
	SetBounds(width, height, true);// выставляем размеры окна

	using Gdiplus::Bitmap;
	Bitmap bitmap(width, height);
	UGC ugc(&bitmap);
	table_container->OnPaint(ugc);// прорисовка всей таблицы

	//---> просто комментарий, что это изображение
	ugc.SetDrawColor(255, 0, 0);
	ugc.SetTextSize(20);
	ugc.SetBold(true);
	ugc.DrawString(L"ИЗОБРАЖЕНИЕ HBITMAP", 200,10);
	ugc.SetBold(false);
	//---<

	bitmap.GetHBITMAP(Gdiplus::Color::White, hbitmap);//создаем требуемый HBITMAP
}
//------------------------------------------------------------
void CChartView::OnPaint()
{
	CWnd::OnPaint();

	UGC ugc(GetDC(), Width, Height);
	if (model->isChartLoaded())
		table_container->OnPaint(ugc);
	else
	{
		ugc.SetDrawColor(0, 0, 0);
		ugc.SetAlign(UGC::CENTER);
		ugc.DrawString(L"Карта ведения пациента не выбрана", Width / 3, Height / 2);
	}
}
//------------------------------------------------------------
void CChartView::OnSize(UINT nType, int cx, int cy)
{
	SetBounds(true);
	RedrawWindow();
	CWnd::OnSize(nType, cx, cy);
}
//------------------------------------------------------------
void CChartView::SetBounds(bool OnSize)
{
	CRect rect;
	GetClientRect(&rect);

	SetBounds(rect.Width(), rect.Height(), OnSize);
}
//------------------------------------------------------------
void CChartView::SetBounds(int width, int height, bool OnSize)
{
	Width = width;
	Height = height;

	int contentHeight = table_container->getContentHeight();
	int new_max = Height - contentHeight;
	if (new_max < 0) new_max = -new_max;
	else
	{
		int curr_minpos, curr_maxpos;
		this->GetScrollRange(SB_VERT, &curr_minpos, &curr_maxpos);
		if (curr_maxpos == 0) goto END;
		new_max = 0;
	}
	this->SetScrollRange(SB_VERT, 0, new_max);
END:
	if (OnSize)
	{
		table_container->setScroll(0, false);
		table_container->Resize(Rect(0, static_cast<int>(TableObject::LINE_HEIGHT*DPIX()), Width, Height));
	}
	else
		table_container->Resize();
}
//------------------------------------------------------------
void CChartView::OnLButtonUp(UINT flags, CPoint point)
{
	int x = point.x;
	int y = point.y;
	if(x >= 0 && x <= Width && y >= TableObject::LINE_HEIGHT && y <= Height)
	{
		if(table_container->OnLButtonUp(x,y))
			return;
		//if(y>Height-100)
			//main_controller->addDrug();
	}
	
}
//------------------------------------------------------------
void CChartView::OnLButtonDown(UINT flags, CPoint point)
{
	int x = point.x;
	int y = point.y;
	if (x >= 0 && x <= Width && y >= TableObject::LINE_HEIGHT && y <= Height)
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
	if (x >= 0 && x <= Width && y >= TableObject::LINE_HEIGHT && y <= Height)
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
	if(!print_mode) 
	{
		SetBounds();
		RedrawWindow();
	}
	
	
	
}
//------------------------------------------------------------
void CChartView::OnRButtonUp(UINT flags, CPoint point)
{
	int x = point.x;
	int y = point.y;
	if (x >= 0 && x <= Width && y >= TableObject::LINE_HEIGHT && y <= Height)
	{
		if (table_container->OnRButtonUp(x, y))
			return;
		//if(y>Height-100)
		//main_controller->addDrug();
	}
}
//------------------------------------------------------------

