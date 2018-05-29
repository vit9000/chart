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
#include "DrugInfo.h"
#include "Parser.h"
#include "AdditionalFeatures.h"



class CCustomListViewItem
{
	int height;
	function<void()> callBack;
public:
	CCustomListViewItem(int Height, const function<void()>& CallBack) : height(Height), callBack(CallBack) {}
	virtual void OnPaint(UGC& ugc) = 0;

	inline int getHeight() const { return height; }
	inline bool isExecutable() const { return callBack ? true : false; }
	inline void execute() { if(callBack) callBack(); }
};
//-----------
class CCustomListView : public CWnd
{
public:
	CCustomListView();
	~CCustomListView();
	inline void AddItem(CCustomListViewItem* item) { items.push_back(item); }

	void SetLoading(bool status);
	int GetContentHeight();
	int GetCurSel();
	const CCustomListViewItem* GetItem(int index);
	inline void ResetCursor() { cursor = -1; }


protected:

	Color highlightColor;
	int scroll;
	int cursor;
	int Width;
	int Height;
	int LineHeight;
	bool loading;
	bool readyToExit;
	//void ClearTableObjects();
	//void SetBounds(bool OnSize = false);
	inline void setScroll(int new_value) { scroll = new_value; }
	void setCursor(const CPoint& point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT flags, CPoint point);
	afx_msg void OnLButtonDown(UINT flags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnDestroy()
	{
		loading = false;
		while (!readyToExit) {} // для синхронизации с детачед процессом
		std::this_thread::sleep_for(10ms);
		CWnd::OnDestroy();
	}

	DECLARE_MESSAGE_MAP();
private:
	std::vector<CCustomListViewItem *> items;

	void DrawLoadingAnimation(UGC& ugc, RECT& rect);
};
