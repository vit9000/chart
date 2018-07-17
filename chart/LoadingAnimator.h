#pragma once
#include "ugc.h"

class CLoadingAnimator : public CWnd
{
	CWnd * wnd;
	bool loading;
	bool readyToExit;
	
protected:
	CLoadingAnimator() : wnd(this), loading(false), readyToExit(true) {}
public:
	CLoadingAnimator(CWnd* window) : wnd(window), loading(false), readyToExit(true) {}
	void SetLoading(bool status);
	void DrawLoadingAnimation(UGC& ugc, int Width, int Height);
	inline bool isReadyToExit() const { return readyToExit; }
	inline bool isLoading() const { return loading;  }

	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP();
};

