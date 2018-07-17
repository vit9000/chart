#pragma once

#include <windows.h>
#include <vector>
#include <string>
#include <utility>
#include <functional>
#include <map>
#include <thread>
#include <mutex>
#include "ugc.h"

#include "CustomListViewItem.h"
#include "LoadingAnimator.h"


//-----------
class CCustomListView : public CLoadingAnimator
{
public:
	CCustomListView();
	~CCustomListView();
	void AddItem(CCustomListViewItem* item);
	
	const CCustomListViewItem* GetItem(int index);

	int GetContentHeight() const;
	void SetCurSel(int index);
	int GetCurSel() const;
	inline void ResetCursor() { cursor = -1; }
	void Clear();
	void SetCustomizations(bool DrawRect) { drawRect = DrawRect; }
protected:
	bool drawRect;
	Color highlightColor;
	int scroll;
	int cursor;
	int Width;
	int Height;
	
	

	// functions
	void SetScrollBarSize();
	inline void setNewScrollPos(int new_value) { scroll = new_value; }
	void setCursor(const CPoint& point);
	afx_msg virtual void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT flags, CPoint point);
	afx_msg void OnLButtonDown(UINT flags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	DECLARE_MESSAGE_MAP();
private:
	std::vector<CCustomListViewItem *> items;
};
