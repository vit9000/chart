#include "stdafx.h"
#include "LoadingAnimator.h"

BEGIN_MESSAGE_MAP(CLoadingAnimator, CWnd)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CLoadingAnimator::setLoading(bool status)
{
	if (status != loading)
	{
		loading = status;
		if(loading)
			SetTimer(REDRAW_TIMER, 20, NULL);
		else KillTimer(REDRAW_TIMER);
	}

	RedrawWindow();
}
//-------------------------------------------------------------------------
void CLoadingAnimator::OnPaint()
{
	CWnd::OnPaint();
	CRect rect;
	GetClientRect(&rect);
	int Width = rect.Width();
	int Height = rect.Height();

	UGC ugc(GetDC(), Width, Height);
	ugc.SetDrawColor(255, 255, 255);
	ugc.Clear();

	OnPaint(ugc, Width, Height);
}
//-------------------------------------------------------------------------
void CLoadingAnimator::OnPaint(UGC& ugc, int Width, int Height)
{
	if (!loading) return;

	static int angle_start = 0;
	static int angle_end = 90;
	static bool t = false;

	if (!message.empty())
	{
		ugc.SetBold(true);
		ugc.SetAlign(UGC::CENTER);
		ugc.SetDrawColor(180, 100, 100, 100);
		ugc.DrawString(message, Width / 2, ugc.getDPIX()(10));
		ugc.SetAlign(UGC::LEFT);
		ugc.SetBold(false);
	}

	int w = static_cast<int>(60 * ugc.getDPIX());
	ugc.SetDrawColor(120, 100, 100, 100);
	ugc.DrawArc(Width / 2 - w / 2, Height / 2 - w / 2, w, angle_start, angle_end, static_cast<int>(ugc.getDPIX() * 8));
	angle_start += 10;

	if (angle_start > 360) angle_start -= 360;

	if (t) angle_end += 7;
	else
	{
		angle_start += 7;
		angle_end -= 7;
	}

	if (angle_end > 360 || angle_end < 0) t = !t;
}
//-------------------------------------------------------------------------
void CLoadingAnimator::OnTimer(UINT uTime)
{
	//CWnd::OnTimer(uTime);
	if (uTime == REDRAW_TIMER)
	{
		RedrawWindow();
		/*if(!loading)
			KillTimer(REDRAW_TIMER);*/
	}
}
//-------------------------------------------------------------------------
void CLoadingAnimator::OnDestroy()
{
	setLoading(false);
	CWnd::OnDestroy();
}
//-------------------------------------------------------------------------