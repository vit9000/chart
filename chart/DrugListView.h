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
#include "BasicListView.h"


using namespace std;
class DrugListView : public CBasicListView
{
public:
	DrugListView();
	~DrugListView();
	void Init(const vector<const DrugInfoEx*>* Items, const function<void()>& CallBack);

	bool GetText(int index, wstring& str);
	const DrugInfoEx* getSelectedDrugInfo() const;
	inline void ResetCursor(){ cursor = -1;}
	void Clear() override {};
	size_t Size() override { if (items) return items->size(); return 0; };
	int GetContentHeight() const override;
	void SetCurSel(int index, bool execute = true) override;
	
protected:
	Color highlightColor;
	int LineHeight;
	void setCursor(const CPoint& point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT flags, CPoint point);
	afx_msg void OnLButtonDown(UINT flags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	
	DECLARE_MESSAGE_MAP();
private:
	const vector<const DrugInfoEx*>* items;
	function<void()> callBack;
	

};
