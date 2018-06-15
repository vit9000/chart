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


using namespace std;
class DrugListView : public CWnd
{
public:
	DrugListView();
	~DrugListView();
	void Init(const vector<const DrugInfoEx*>* Items, const function<void()>& CallBack);

	void setLoading(bool status);
	int GetContentHeight() const;
	int GetCurSel() const;
	bool GetText(int index, wstring& str);
	const DrugInfoEx* getSelectedDrugInfo() const;
	inline void ResetCursor()
	{
		cursor = -1;
	}
	/*const DrugInfoEx& getSelectedDrugInfo() const
	{
		auto i = GetCurSel();
		return (*(*items)[i]);

	}*/
	
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
	inline void setScroll(int new_value)
	{
		scroll = new_value;
	}
	void setCursor(const CPoint& point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT flags, CPoint point);
	afx_msg void OnLButtonDown(UINT flags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
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
	const vector<const DrugInfoEx*>* items;
	function<void()> callBack;
	

};
