

#include "stdafx.h"
#include "resource.h"
#include "CustomListView.h"

BEGIN_MESSAGE_MAP(CCustomListView, CBasicListView)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


CCustomListView::CCustomListView()
	: CBasicListView(),
	highlightColor(UGC::convertColor(GetSysColor(COLOR_MENUHILIGHT))),
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
	int one = static_cast<int>(1 * DPIX());;
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
