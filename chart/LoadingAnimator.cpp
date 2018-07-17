#include "stdafx.h"
#include "LoadingAnimator.h"
#include <thread>


BEGIN_MESSAGE_MAP(CLoadingAnimator, CWnd)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


using namespace std;
void CLoadingAnimator::SetLoading(bool status)
{
	if (status != loading && status)
	{
		loading = status;
		if (loading)
		{
			thread t(
				[this]()
			{
				readyToExit = false;
				while (loading)
				{
					wnd->RedrawWindow();
					std::this_thread::sleep_for(30ms);
				}
				readyToExit = true;
			}
			);
			t.detach();
		}
		
	}
	loading = status;
	wnd->RedrawWindow();
}
//-------------------------------------------------------------------------
void CLoadingAnimator::DrawLoadingAnimation(UGC& ugc, int Width, int Height)
{
	if (!loading) return;
	
	static int angle_start = 0;
	static int angle_end = 90;
	static bool t = false;

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
void CLoadingAnimator::OnDestroy()
{
	while (!readyToExit) {} // для синхронизации с детачед процессом
	std::this_thread::sleep_for(10ms);
	CWnd::OnDestroy();
}
//-------------------------------------------------------------------------