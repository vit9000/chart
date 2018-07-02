#include "stdafx.h"
#include "resource.h"
#include "EditableList.h"

BEGIN_MESSAGE_MAP(EditableList, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//ON_WM_VSCROLL()
	//ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


EditableList::EditableList()
	:
	Width(100),
	Height(100),
	LineHeight(static_cast<int>(22*DPIX())),
	lock_next(false)
{}
//-------------------------------------------------------------------------
EditableList::~EditableList()
{}
//-------------------------------------------------------------------------

void EditableList::OnPaint()
{
	CWnd::OnPaint();

	UGC ugc(GetDC(), Width, Height);
	ugc.SetDrawColor(255, 255, 255);
	ugc.Clear();
	ugc.SetDrawColor(Gdiplus::Color::Gray);
	ugc.DrawRectangle(0, 0, Width-1, Height-1);

	ugc.SetTextSize(10);
	int x1 = static_cast<int>(5 * ugc.getDPIX());

	int header_width = Width * 3 / 4;
	ugc.DrawDotLine(header_width, 0, header_width, items.size()*LineHeight);
	int x2 = (Width-header_width)/2+header_width;

	int y = 0;
	int yi = LineHeight / 2 - ugc.GetTextHeight() / 2;
	const DPIX& dpix = ugc.getDPIX();
	for (const auto& item : items)
	{
		ugc.SetDrawColor(0, 0, 0);
		ugc.SetAlign(UGC::LEFT);
		int mes_unit_width = (item.mes_unit.empty())? 0 : ugc.GetTextWidth(item.mes_unit) + dpix(8);
		ugc.DrawStringInWidth(item.caption, x1, y + yi, header_width-mes_unit_width);
		ugc.DrawString(item.mes_unit, x1 + header_width - mes_unit_width + dpix(4), y + yi);
		ugc.SetAlign(UGC::CENTER);
		ugc.DrawString(item.value, x2, y + LineHeight / 2 - ugc.GetTextHeight() / 2);
		ugc.SetAlign(UGC::LEFT);
		y += LineHeight;
		ugc.SetDrawColor(Gdiplus::Color::Gray);
		ugc.DrawDotLine(0, y, Width, y);
	}
}
//-------------------------------------------------------------------------
int EditableList::GetContentHeight()
{
	return LineHeight*items.size();
}
//-------------------------------------------------------------------------
void EditableList::OnSize(UINT nType, int cx, int cy)
{
	CRect rect;
	GetClientRect(&rect);
	Width = rect.Width();
	Height = rect.Height();
	CWnd::OnSize(nType, cx, cy);
}
//-------------------------------------------------------------------------
void EditableList::OnLButtonUp(UINT flags, CPoint point)
{
	int x = point.x;
	int y = point.y;
	int index = y / LineHeight;
	if (index >= static_cast<int>(items.size()))
		return;
	lock_next = true;
	SetEditBox(index);
	lock_next = false;
}
//-------------------------------------------------------------------------
void EditableList::OnLButtonDown(UINT flags, CPoint point)
{

}
//-------------------------------------------------------------------------
void EditableList::OnMouseMove(UINT nFlags, CPoint point)
{

}
//-------------------------------------------------------------------------
void EditableList::Next(int prev_index)
{
	if(!lock_next)
		SetEditBox(prev_index + 1);
}
//-------------------------------------------------------------------------
void EditableList::SetEditBox(int index)
{
	if (index >= static_cast<int>(items.size()))
	{
		if (closeDlg)
			closeDlg();
		return;
	}

	function<void(const wstring&)> callBack = [this, index](const wstring& new_value)
	{
		items.at(index).value = new_value;
	};
	function<void()> next = [this, index]()
	{
		Next(index);
	};

	CEdit * pEdit = new CInPlaceEditbox(callBack, items.at(index).value, true, next);
	DWORD dwStyle = ES_CENTER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
	//dwStyle |= WS_BORDER;
	RECT r;
	double dpix = DPIX();
	int border = static_cast<int>(2 * dpix);
	int h = static_cast<int>(18. * dpix);
	r.left = Width * 3 / 4 + border;
	r.right = Width - border * 2;
	int temp = (LineHeight - h) / 2;
	r.top = index*LineHeight + temp;
	r.bottom = r.top + h;
	pEdit->Create(dwStyle, r, this, IDC_EDIT_VALUE);
}