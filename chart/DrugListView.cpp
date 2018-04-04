#include "stdafx.h"
#include "resource.h"
#include "DrugListView.h"

BEGIN_MESSAGE_MAP(DrugListView, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//ON_WM_VSCROLL()
	//ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


DrugListView::DrugListView()
	:
	Width(100),
	Height(100),
	LineHeight(static_cast<int>(22 * DPIX()))
{}
//-------------------------------------------------------------------------
DrugListView::~DrugListView()
{}
//-------------------------------------------------------------------------
void DrugListView::Init(const vector<const DrugInfo*>* Items, function<void()> CallBack)
{
	items = Items;
	callBack = CallBack;
}
//-------------------------------------------------------------------------

void DrugListView::OnPaint()
{
	CWnd::OnPaint();

	UGC ugc(GetDC(), Width, Height);
	ugc.SetDrawColor(255, 255, 255);
	ugc.Clear();
	ugc.SetDrawColor(Gdiplus::Color::Gray);
	ugc.DrawRectangle(0, 0, Width - 1, Height - 1);

	ugc.SetTextSize(10);
	int x1 = static_cast<int>(5 * ugc.getDPIX());

	int temp =Width/10;
	ugc.DrawDotLine(temp, 0, temp, items->size()*LineHeight);
	int x2 = static_cast<int>(temp*1.25);

	int y = 0;
	//ugc.SetAlign(UGC::LEFT);
	std::mutex mute;
	mute.lock();
	size_t size = items->size();
	mute.unlock();
	for (size_t i=0; i<size; i++)
	{
		ugc.SetDrawColor(0, 0, 0);
		ugc.DrawString((items->at(i)->isExistsInDB())?L"OK" : L"NO", x1, y + LineHeight / 2 - ugc.GetTextHeight() / 2);
		//ugc.SetAlign(UGC::CENTER);
		ugc.DrawString(items->at(i)->dbname, x2, y + LineHeight / 2 - ugc.GetTextHeight() / 2);
		
		y += LineHeight;
		ugc.SetDrawColor(Gdiplus::Color::Gray);
		ugc.DrawDotLine(0, y, Width, y);
	}
}
//-------------------------------------------------------------------------
int DrugListView::GetContentHeight()
{
	return LineHeight*items->size();
}
//-------------------------------------------------------------------------
void DrugListView::OnSize(UINT nType, int cx, int cy)
{
	CRect rect;
	GetClientRect(&rect);
	Width = rect.Width();
	Height = rect.Height();
	CWnd::OnSize(nType, cx, cy);
}
//-------------------------------------------------------------------------
void DrugListView::OnLButtonUp(UINT flags, CPoint point)
{
	int x = point.x;
	int y = point.y;
	int index = y / LineHeight;
	if (index >= static_cast<int>(items->size()))
		return;
	
}
//-------------------------------------------------------------------------
void DrugListView::OnLButtonDown(UINT flags, CPoint point)
{

}
//-------------------------------------------------------------------------
void DrugListView::OnMouseMove(UINT nFlags, CPoint point)
{

}
//-------------------------------------------------------------------------
int DrugListView::GetCurSel()
{
	return cursor;
}
//-------------------------------------------------------------------------

bool DrugListView::GetText(int index, wstring& str)
{
	if (static_cast<size_t>(index) >= items->size())
		return false;
	str = items->at(index)->dbname;
	return true;
}
//-------------------------------------------------------------------------

