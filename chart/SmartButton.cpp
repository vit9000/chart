#include "stdafx.h"
#include "SmartButton.h"


BEGIN_MESSAGE_MAP(CSmartButton, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()


CSmartButton::CSmartButton(CToolBarBase* Parent, int ID, const wstring& Text, CRect& rect, const function<void()>& Func, int Group)
	: parent(Parent), id(ID-2000), text(Text), width(rect.Width()), height(rect.Height()), func(Func), status(NORMAL), checked(false), group(Group), m_bMouseTracking(false)
{
	Create(NULL, NULL, WS_VISIBLE | WS_CHILDWINDOW, rect, parent, ID);
}

void CSmartButton::OnPaint()
{
	CWnd::OnPaint();
	CRect rect;
	GetClientRect(&rect);
	
	UGC ugc(GetDC(), width, height);
	ugc.SetDrawColor(255, 255, 135);
	ugc.Clear();
	switch (status)
	{
	case NORMAL: 
		DrawNormal(ugc);
		break;
	case HOVER:
		DrawHover(ugc);
		break;
	case DOWN:
		DrawDown(ugc);
		break;
	}
	
	ugc.SetDrawColor(0, 0, 0);
	ugc.SetAlign(UGC::CENTER);
	ugc.SetTextSize(12);
	ugc.DrawString(text, width / 2, height / 2 - ugc.GetTextHeight() / 2);
	ugc.SetAlign(UGC::LEFT);
}


void CSmartButton::OnLButtonUp(UINT flags, CPoint point)
{
	
	if (group == -1)
	{
		status = NORMAL;
	}
	else
	{
		parent->setPressed(id);
	}
	RedrawWindow();
	if (func) func();
}
//-------------------------------------------------------------------------
void CSmartButton::OnLButtonDown(UINT flags, CPoint point)
{
	status = DOWN;
	RedrawWindow();
}
//-------------------------------------------------------------------------
void CSmartButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bMouseTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = this->m_hWnd;

		if (::_TrackMouseEvent(&tme))
		{
			m_bMouseTracking = true;
		}

		status = HOVER;
		RedrawWindow();
	}

}
//-------------------------------------------------------------------------
void CSmartButton::OnMouseLeave()
{
	m_bMouseTracking = false;
	status = NORMAL;
	RedrawWindow();
}
//-------------------------------------------------------------------------

void CSmartButton::OnSize(UINT nType, int cx, int cy)
{
	CRect rect;
	GetClientRect(&rect);
	width = rect.Width();
	height = rect.Height();
	CWnd::OnSize(nType, cx, cy);
}

void CSmartButton::DrawNormal(UGC& ugc)
{
	if (checked)
		DrawDown(ugc);
}

void CSmartButton::DrawHover(UGC& ugc)
{
	if (checked)
	{
		ugc.SetDrawColor(30, 100, 100, 100);
		ugc.FillRectangle(0, 0, width, height);
		DrawDownRect(ugc);
	}
	else
	{
		ugc.SetDrawColor(100, 255, 255, 255);
		ugc.FillRectangle(0, 0, width, height);
		DrawHoverRect(ugc);
	}
}

void CSmartButton::DrawDown(UGC& ugc)
{
	ugc.SetDrawColor(45, 100, 100, 100);
	ugc.FillRectangle(0, 0, width, height);
	DrawDownRect(ugc);
}
void CSmartButton::DrawDownRect(UGC& ugc)
{
	ugc.SetDrawColor(150, 255, 255, 255);
	ugc.DrawLine(width-1, height-1, width-1, 0);
	ugc.DrawLine(0, height-1, width-1, height-1);

	ugc.SetDrawColor(140, 140, 140);
	ugc.DrawLine(0, height, 0, 0);
	ugc.DrawLine(0, 0, width, 0);	
}

void CSmartButton::DrawHoverRect(UGC& ugc)
{
	ugc.SetDrawColor(140, 140, 140);
	ugc.DrawLine(width - 1, height - 1, width - 1, 0);
	ugc.DrawLine(0, height - 1, width - 1, height - 1);

	ugc.SetDrawColor(150, 255, 255, 255);
	ugc.DrawLine(0, height, 0, 0);
	ugc.DrawLine(0, 0, width, 0);
}