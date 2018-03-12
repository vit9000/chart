#pragma once

#include "InPlaceEdit.h"
// CMyListCtrl dialog

class EditableListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(EditableListCtrl)

public:
	EditableListCtrl();   // standard constructor
	virtual ~EditableListCtrl();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = undefined };
#endif

protected:
	int HitTestEx(CPoint &point, int *col) const;
	CEdit* EditSubLabel(int nItem, int nCol);
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()




};
