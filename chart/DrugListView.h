#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <utility>
#include <functional>
#include <map>
#include <mutex>
#include "ugc.h"
#include "DrugInfo.h"

using namespace std;
class DrugListView : public CWnd
{
public:
	DrugListView();
	~DrugListView();
	void Init(const vector<const DrugInfo*>* Items, function<void()> CallBack);


	int GetContentHeight();
	int GetCurSel();
	bool GetText(int index, wstring& str);
	inline void ResetCursor()
	{
		cursor = -1;
	}
protected:
	int scroll;
	int cursor;
	int Width;
	int Height;
	int LineHeight;
	//void ClearTableObjects();
	//void SetBounds(bool OnSize = false);
	inline void setScroll(int new_value)
	{
		scroll = new_value;
	}
	
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT flags, CPoint point);
	afx_msg void OnLButtonDown(UINT flags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	DECLARE_MESSAGE_MAP();
private:
	const vector<const DrugInfo*>* items;
	function<void()> callBack;
	

};
