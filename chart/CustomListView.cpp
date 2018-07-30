

#include "stdafx.h"
#include "resource.h"
#include "CustomListView.h"

BEGIN_MESSAGE_MAP(CCustomListView, CLoadingAnimator)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


CCustomListView::CCustomListView()
	:
	Width(100),
	Height(100),
	cursor(-1),
	scroll(0),
	highlightColor(convertColor(GetSysColor(COLOR_MENUHILIGHT))),
	drawRect(true),
	mouseDown(false)

{}
//-------------------------------------------------------------------------
CCustomListView::~CCustomListView()
{
	Clear();
}
//-------------------------------------------------------------------------

void CCustomListView::Clear()
{
	for (auto& item_ptr : items)
		delete item_ptr;
	items.clear();
	cursor = -1;
}
//-------------------------------------------------------------------------
void CCustomListView::AddItem(CCustomListViewItem* item)
{
	items.push_back(item);
	SetScrollBarSize();
}
//-------------------------------------------------------------------------
void CCustomListView::SetScrollBarSize()
{
	RECT rect;
	GetClientRect(&rect);
	int s = GetContentHeight() - rect.bottom - rect.top;
	if (s < 0) s = 0;
	this->SetScrollRange(SB_VERT, 0, s);
}
//-------------------------------------------------------------------------
void CCustomListView::OnPaint()
{
	CWnd::OnPaint();

	UGC ugc(GetDC(), Width, Height);
	ugc.SetDrawColor(255, 255, 255);
	ugc.Clear();
	if (drawRect)
	{
		ugc.SetDrawColor(Gdiplus::Color::Gray);
		ugc.DrawRectangle(0, 0, Width - 1, Height - 1);
	}
	ugc.SetTextSize(10);
	
	int y = -scroll;
	size_t size = items.size();
	int one = static_cast<int>(1 * ugc.getDPIX());;
	for (size_t i = 0; i < size; i++)
	{
		auto& item = items.at(i);
		int LineHeight = item->getHeight();
		if (y > -LineHeight)
		{
			if (static_cast<int>(i) == cursor)
			{

				ugc.SetDrawColor(highlightColor);
				ugc.FillRectangle(1, y + 1, Width - 2, LineHeight - 2);
				ugc.SetDrawColor(255, 255, 255);
			}
			else
				ugc.SetDrawColor(10, 10, 10);
			item->OnPaint(ugc, y, Width);
			
			ugc.SetDrawColor(Gdiplus::Color::Gray);
			ugc.DrawDotLine(0, y + LineHeight, Width, y + LineHeight);
		}

		y += LineHeight;

		if (y > Height)
			break;
	}
	CLoadingAnimator::OnPaint(ugc, Width, Height);
}
//-------------------------------------------------------------------------
int CCustomListView::GetContentHeight() const
{
	int res = 0;
	for (auto& item : items)
		res += item->getHeight();
	return res;
	//return LineHeight * items.size();
}
//-------------------------------------------------------------------------
void CCustomListView::OnSize(UINT nType, int cx, int cy)
{
	CRect rect;
	GetClientRect(&rect);
	Width = rect.Width();
	Height = rect.Height();
	CWnd::OnSize(nType, cx, cy);
}
//-------------------------------------------------------------------------
void CCustomListView::setCursor(const CPoint& point)
{
	int x = point.x;
	int y = point.y + scroll;

	int index = 0;
	for (auto& item : items)
	{
		y -= item->getHeight();
		if (y<0)
			break;
		index++;
	}
	if (index >= static_cast<int>(items.size()))
		return;
	cursor = index;
}
void CCustomListView::OnLButtonUp(UINT flags, CPoint point)
{
	if (!mouseDown) return;
	mouseDown = false;
	if (cursor < 0 || cursor >= static_cast<int>(items.size()))
		return;
	auto& item = items.at(cursor);
	item->execute();
}
//-------------------------------------------------------------------------
void CCustomListView::OnLButtonDown(UINT flags, CPoint point)
{
	mouseDown = true;
	this->SetFocus();
	setCursor(point);
	RedrawWindow();
	
}
//-------------------------------------------------------------------------
void CCustomListView::OnMouseMove(UINT nFlags, CPoint point)
{

}
//-------------------------------------------------------------------------
int CCustomListView::GetCurSel() const
{
	return cursor;
}
//-------------------------------------------------------------------------
void CCustomListView::SetCurSel(int index, bool execute) 
{
	if (index < 0 || index >= static_cast<int>(items.size()))
		return;
	cursor = index;
	if(execute)
		items.at(index)->execute();
}
//-------------------------------------------------------------------------
const CCustomListViewItem* CCustomListView::GetItem(int index)
{
	if (index >= static_cast<int>(items.size()))
		return nullptr;
	return items.at(index);
}
//-------------------------------------------------------------------------

void CCustomListView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
			curpos -= dpix.getIntegerValue(5);
		break;

	case SB_LINEDOWN:   // Правaя Прокруткa.
		if (curpos < maxpos)
			curpos += dpix.getIntegerValue(5);
		break;

	case SB_PAGEDOWN:
	{
		if (curpos < maxpos)
			curpos += dpix.getIntegerValue(10);
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
	this->SetScrollPos(SB_VERT, curpos);
	setNewScrollPos(curpos);
	RedrawWindow();
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CCustomListView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int minpos;
	int maxpos;
	this->GetScrollRange(SB_VERT, &minpos, &maxpos);
	maxpos = this->GetScrollLimit(SB_VERT);
	// Получите текущую позицию бегунка.
	int curpos = this->GetScrollPos(SB_VERT);
	DPIX dpix;
	if (zDelta < 0 && curpos < maxpos)
		curpos += dpix.getIntegerValue(10);
	else if (zDelta > 0 && curpos > minpos)
		curpos -= dpix.getIntegerValue(10);

	if (curpos > maxpos) curpos = maxpos;
	else if (curpos < minpos) curpos = minpos;


	this->SetScrollPos(SB_VERT, curpos);
	setNewScrollPos(curpos);
	RedrawWindow();

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);;
}