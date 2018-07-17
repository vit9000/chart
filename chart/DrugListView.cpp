#include "stdafx.h"
#include "resource.h"
#include "DrugListView.h"

BEGIN_MESSAGE_MAP(DrugListView, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


DrugListView::DrugListView()
	:
	Width(100),
	Height(100),
	LineHeight(static_cast<int>(28 * DPIX())),
	cursor(-1),
	scroll(0)	
{}
//-------------------------------------------------------------------------
DrugListView::~DrugListView()
{}
//-------------------------------------------------------------------------
void DrugListView::Init(const vector<const DrugInfoEx*>* Items, const function<void()>& CallBack)
{
	items = Items;
	callBack = CallBack;

	highlightColor = convertColor(GetSysColor(COLOR_MENUHILIGHT));
}
//-------------------------------------------------------------------------
void DrugListView::OnPaint()
{
	RECT rect;
	GetClientRect(&rect);
	
	CWnd::OnPaint();

	UGC ugc(GetDC(), Width, Height);
	ugc.SetDrawColor(255, 255, 255);
	ugc.Clear();
	ugc.SetDrawColor(Gdiplus::Color::Gray);
	ugc.DrawRectangle(0, 0, Width - 1, Height - 1);

	ugc.SetTextSize(10);
	int x1 = static_cast<int>(5 * ugc.getDPIX());


	int d = static_cast<int>(8 * ugc.getDPIX());
	
	int x2 = static_cast<int>(x1*2+d);
	std::mutex mute;
	mute.lock();
	int s = GetContentHeight() - rect.bottom - rect.top;
	if (s < 0) s = 0;
	this->SetScrollRange(SB_VERT, 0, s);
	int y = -scroll;
	//ugc.SetAlign(UGC::LEFT);

	
	size_t size = items->size();

	int one = static_cast<int>(1 * ugc.getDPIX());;

	auto triangle = [this, &ugc, &x1, &y, &d]() {
		ugc.FillTriangle(x1, y + LineHeight / 2 + d / 2,
			x1 + d / 2, y + LineHeight / 2 - d / 2,
			x1 + d, y + LineHeight / 2 + d / 2);
	};

	for (size_t i=0; i<size; i++)
	{
		if(y>-LineHeight)
		{
			if (static_cast<int>(i) == cursor)
			{

				ugc.SetDrawColor(highlightColor);
				ugc.FillRectangle(1, y + 1, Width - 2, LineHeight - 1);
			}

			if (items->at(i)->isEnoughInfo())
			{
				if (!items->at(i)->IsAdminWaysExists())
				{
					ugc.SetDrawColor(255,165,0);
					triangle();
				}
				ugc.SetDrawColor(0, 0, 0);
			}
			else
			{
				ugc.SetDrawColor(255,0,0);
				triangle();
				ugc.SetDrawColor(Gdiplus::Color::Gray);
			}
			if (static_cast<int>(i) == cursor)
			{
				ugc.SetDrawColor(255, 255, 255);
			}
			ugc.SetBold(true);
			ugc.DrawString(items->at(i)->name, x2, y);
			ugc.SetBold(false);
			ugc.DrawString(items->at(i)->drug_form, x2, y + LineHeight - ugc.GetTextHeight());
			
			ugc.SetDrawColor(Gdiplus::Color::Gray);
			ugc.DrawDotLine(0, y + LineHeight, Width, y + LineHeight);
		}
		

		y += LineHeight;
		
		if (y > Height)
			break;
	}
	mute.unlock();

	DrawLoadingAnimation(ugc, Width, Height);

}
//-------------------------------------------------------------------------
int DrugListView::GetContentHeight() const
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
void DrugListView::setCursor(const CPoint& point)
{
	int x = point.x;
	int y = point.y + scroll;
	int index = y / LineHeight;
	if (index < 0 || index >= static_cast<int>(items->size()))
		index = -1;
	cursor = index;
}
void DrugListView::OnLButtonUp(UINT flags, CPoint point)
{
	setCursor(point);
	RedrawWindow();
	if (callBack)
		callBack();
}
void DrugListView::OnLButtonDblClk(UINT flags, CPoint point)
{
	setCursor(point);
	wstring name;
	
	for (auto& c : name)
		if (c == L' ') c = L'+';
	AdditionalFeatures().RunDrugInfo(name);
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
int DrugListView::GetCurSel() const
{
	return cursor;
}
//-------------------------------------------------------------------------
const DrugInfoEx* DrugListView::getSelectedDrugInfo() const
{
	int index = GetCurSel();
	if (static_cast<size_t>(index) >= items->size())
		return nullptr;
	return items->at(index);
}
//-------------------------------------------------------------------------
bool DrugListView::GetText(int index, wstring& str)
{
	if (static_cast<size_t>(index) >= items->size())
		return false;
	str = items->at(index)->name;
	return true;
}
//-------------------------------------------------------------------------

void DrugListView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// �������� ����������� � ������������ ������� ��������.
	int minpos;
	int maxpos;
	this->GetScrollRange(SB_VERT, &minpos, &maxpos);
	maxpos = this->GetScrollLimit(SB_VERT);
	// �������� ������� ������� �������.
	int curpos = this->GetScrollPos(SB_VERT);
	DPIX dpix;
	// ���������� ����� ������� �������.
	switch (nSBCode)
	{
	case SB_BOTTOM:      // ���������  ������ � ����.
		curpos = minpos;
		break;

	case SB_TOP:      // ���������  ������ � �����.
		curpos = maxpos;
		break;

	case SB_ENDSCROLL:   // �������� ���������.
		break;

	case SB_LINEUP:      // ����� ���������.
		if (curpos > minpos)
			curpos-=dpix.getIntegerValue(5);
		break;

	case SB_LINEDOWN:   // ����a� ��������a.
		if (curpos < maxpos)
			curpos+= dpix.getIntegerValue(5);
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

	case SB_THUMBPOSITION: // ��������a � ���������� �������. nPos - �������
		curpos = nPos;      // �� ������� � ����� ������������ ��������.

		break;

	case SB_THUMBTRACK:   // ���������� ������� � ������������ �������. nPos -
		curpos = nPos;     // �������, � ������� ������� ���������.
		break;
	}

	// ���������� ����� ������� ������� 
	this->SetScrollPos(SB_VERT, curpos);
	setScroll(curpos);
	RedrawWindow();
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL DrugListView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int minpos;
	int maxpos;
	this->GetScrollRange(SB_VERT, &minpos, &maxpos);
	maxpos = this->GetScrollLimit(SB_VERT);
	// �������� ������� ������� �������.
	int curpos = this->GetScrollPos(SB_VERT);
	DPIX dpix;
	if (zDelta < 0 && curpos < maxpos)
		curpos += dpix.getIntegerValue(10);
	else if (zDelta > 0 && curpos > minpos)
		curpos -= dpix.getIntegerValue(10);

	if (curpos > maxpos) curpos = maxpos;
	else if (curpos < minpos) curpos = minpos;


	this->SetScrollPos(SB_VERT, curpos);
	setScroll(curpos);
	RedrawWindow();

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);;
}