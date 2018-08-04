#pragma once
#include "ugc.h"

#include "CustomListViewItem.h"
#include "LoadingAnimator.h"


//-----------
class CBasicListView : public CLoadingAnimator
{
public:
	CBasicListView();
	~CBasicListView();
	int GetCurSel() const;
	inline void ResetCursor() { cursor = -1; }

	virtual void Clear() = 0;
	virtual size_t Size()=0;
	virtual int GetContentHeight() const = 0;
	virtual void SetCurSel(int index, bool execute = true) = 0;
protected:
	int scroll;
	int cursor;
	int Width;
	int Height;

	// functions
	void SetScrollBarSize();
	inline void setNewScrollPos(int new_value) { scroll = new_value; }
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	DECLARE_MESSAGE_MAP();

};
