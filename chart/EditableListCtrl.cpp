// MyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "chart.h"
#include "EditableListCtrl.h"
#include "afxdialogex.h"


// CMyListCtrl dialog

IMPLEMENT_DYNAMIC(EditableListCtrl, CListCtrl)

EditableListCtrl::EditableListCtrl()
{

}

EditableListCtrl::~EditableListCtrl()
{
}


BEGIN_MESSAGE_MAP(EditableListCtrl, CWnd)
	ON_WM_LBUTTONDOWN()
	//ON_NOTIFY(LVN_ENDLABELEDIT, 1000, OnEndLabelEdit)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndLabelEdit)
END_MESSAGE_MAP()

// HitTestEx - Determine the row index and column index for a point
// Returns	- the row index or -1 if point is not over a row
// point	- point to be tested.
// col		- to hold the column index
int EditableListCtrl::HitTestEx(CPoint &point, int *col) const
{
	int colnum = 0;
	int row = HitTest(point, NULL);

	if (col) *col = 0;

	// Make sure that the ListView is in LVS_REPORT
	if ((GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
		return row;

	// Get the top and bottom row visible
	row = GetTopIndex();
	int bottom = row + GetCountPerPage();
	if (bottom > GetItemCount())
		bottom = GetItemCount();

	// Get the number of columns
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();

	// Loop through the visible rows
	for (; row <= bottom; row++)
	{
		// Get bounding rect of item and check whether point falls in it.
		CRect rect;
		GetItemRect(row, &rect, LVIR_BOUNDS);
		if (rect.PtInRect(point))
		{
			// Now find the column
			for (colnum = 0; colnum < nColumnCount; colnum++)
			{
				int colwidth = GetColumnWidth(colnum);
				if (point.x >= rect.left
					&& point.x <= (rect.left + colwidth))
				{
					if (col) *col = colnum;
					return row;
				}
				rect.left += colwidth;
			}
		}
	}
	return -1;
}


// EditSubLabel		- Start edit of a sub item label
// Returns		- Temporary pointer to the new edit control
// nItem		- The row index of the item to edit
// nCol			- The column of the sub item.
CEdit* EditableListCtrl::EditSubLabel(int nItem, int nCol)
{
	// The returned pointer should not be saved

	// Make sure that the item is visible
	if (!EnsureVisible(nItem, TRUE)) return NULL;

	// Make sure that nCol is valid
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	if (nCol >= nColumnCount || GetColumnWidth(nCol) < 5)
		return NULL;

	// Get the column offset
	int offset = 0;
	for (int i = 0; i < nCol; i++)
		offset += GetColumnWidth(i);

	CRect rect;
	GetItemRect(nItem, &rect, LVIR_BOUNDS);

	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect(&rcClient);
	if (offset + rect.left < 0 || offset + rect.left > rcClient.right)
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll(size);
		rect.left -= size.cx;
	}

	// Get Column alignment
	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	GetColumn(nCol, &lvcol);
	DWORD dwStyle;
	if ((lvcol.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
	else if ((lvcol.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
	else dwStyle = ES_CENTER;

	rect.left += offset + 4;
	rect.right = rect.left + GetColumnWidth(nCol) - 3;
	if (rect.right > rcClient.right) rect.right = rcClient.right;

	dwStyle |= WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
	CEdit *pEdit = new CInPlaceEdit(nItem, nCol, GetItemText(nItem, nCol));
	pEdit->Create(dwStyle, rect, this, IDC_EDIT_VALUE);// IDC_IPEDIT);


	return pEdit;
}

void EditableListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (GetFocus() != this) SetFocus();
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void EditableListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (GetFocus() != this) SetFocus();
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void EditableListCtrl::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO *plvDispInfo = (LV_DISPINFO *)pNMHDR;
	LV_ITEM	*plvItem = &plvDispInfo->item;

	if (plvItem->pszText != NULL)
	{
		SetItemText(plvItem->iItem, plvItem->iSubItem, plvItem->pszText);
	}
	*pResult = FALSE;
}

void EditableListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	int index;
	CListCtrl::OnLButtonDown(nFlags, point);

	int colnum;
	if ((index = HitTestEx(point, &colnum)) != -1)
	{
		EditSubLabel(index, 1);
	}
}