#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <utility>
#include "ugc.h"
#include "CInPlaceEditbox.h"
using namespace std;

struct Item
{
	wstring caption, mes_unit, value;
};

class EditableList : public CWnd
{
public:
	EditableList();
	~EditableList();

	void SetCloseDlgFunction(const std::function<void()>& CloseDlg)
	{
		closeDlg = CloseDlg;
	}

	void InsertItem(const wstring& caption, const wstring& value, const wstring& mes_unit)
	{
		items.push_back( { caption, mes_unit, value } );
		RedrawWindow();
	}

	const wstring& GetItemName(size_t index) const
	{
		if (index >= items.size())
			throw invalid_argument("EditableList::GetTextItem - invalid index");
		return items.at(index).caption;
	}

	const wstring& GetItemText(size_t index) const
	{
		if (index >= items.size())
			throw invalid_argument("EditableList::GetTextItem - invalid index");
		return items.at(index).value;
	}
	void SetEditBox(int index);
	int GetContentHeight();
protected:
	std::function<void()> closeDlg;
	int Width;
	int Height;
	int LineHeight;
	//void ClearTableObjects();
	//void SetBounds(bool OnSize = false);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT flags, CPoint point);
	afx_msg void OnLButtonDown(UINT flags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	void Next(int prev_index);
	//afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	DECLARE_MESSAGE_MAP();
private:
	vector<Item> items;
	bool lock_next;
	
	
};
