

#include "stdafx.h"
#include "resource.h"
#include "BasicListView.h"

BEGIN_MESSAGE_MAP(CBasicListView, CLoadingAnimator)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


CBasicListView::CBasicListView()
	:
	Width(100),
	Height(100),
	cursor(-1),
	scroll(0)
{}
//-------------------------------------------------------------------------
CBasicListView::~CBasicListView()
{

}
//-------------------------------------------------------------------------

void CBasicListView::SetScrollBarSize()
{
	RECT rect;
	GetClientRect(&rect);
	int s = GetContentHeight() - rect.bottom - rect.top;
	if (s < 0) s = 0;
	this->SetScrollRange(SB_VERT, 0, s);
}
//-------------------------------------------------------------------------
void CBasicListView::OnSize(UINT nType, int cx, int cy)
{
	CRect rect;
	GetClientRect(&rect);
	Width = rect.Width();
	Height = rect.Height();
	CWnd::OnSize(nType, cx, cy);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
int CBasicListView::GetCurSel() const
{
	return cursor;
}
//-------------------------------------------------------------------------

void CBasicListView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
			curpos -= dpix.getIntegerValue(5);
		break;

	case SB_LINEDOWN:   // ����a� ��������a.
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

	case SB_THUMBPOSITION: // ��������a � ���������� �������. nPos - �������
		curpos = nPos;      // �� ������� � ����� ������������ ��������.

		break;

	case SB_THUMBTRACK:   // ���������� ������� � ������������ �������. nPos -
		curpos = nPos;     // �������, � ������� ������� ���������.
		break;
	}

	// ���������� ����� ������� ������� 
	this->SetScrollPos(SB_VERT, curpos);
	setNewScrollPos(curpos);
	RedrawWindow();
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CBasicListView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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
	setNewScrollPos(curpos);
	RedrawWindow();

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);;
}