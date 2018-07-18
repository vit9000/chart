#pragma once
#include "ugc.h"

#define REDRAW_TIMER 1
class CLoadingAnimator : public CWnd
{
	bool loading;
protected:
	wstring message;
public:
	CLoadingAnimator() : loading(false) {}
	inline void setMessage(const wstring& Message) { message = Message; }
	void setLoading(bool status);
	inline bool isLoading() const { return loading;  }
	void stop() { loading = false; }

	afx_msg void OnTimer(UINT uTime);
	afx_msg void OnPaint(UGC& ugc, int Width, int Height);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP();
};

