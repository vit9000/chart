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
#include "BasicListView.h"


//-----------
class CCustomListView : public CBasicListView
{
public:
	CCustomListView();
	~CCustomListView();
	void AddItem(CCustomListViewItem* item);
	const CCustomListViewItem* GetItem(int index);
	void SetCustomizations(bool DrawRect) { drawRect = DrawRect; }
	
	
	void Clear() override;
	size_t Size() override { return items.size(); }
	int GetContentHeight() const override;
	void SetCurSel(int index, bool execute = true) override;

	
protected:
	bool mouseDown;
	bool drawRect;
	Color highlightColor;
	

	// functions
	void setCursor(const CPoint& point);
	afx_msg virtual void OnPaint();
	afx_msg void OnLButtonUp(UINT flags, CPoint point);
	afx_msg void OnLButtonDown(UINT flags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	DECLARE_MESSAGE_MAP();
private:
	std::vector<CCustomListViewItem *> items;
};
