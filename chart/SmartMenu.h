#pragma once
#include <vector>
#include <string>
#include <functional>
#include <thread>
#include "Resource.h"
#include "afxwin.h"
#include "ugc.h"

#include "IChartController.h"



// SmartMenu dialog
using namespace std;

class CSmartMenu : public CDialog
{
	DECLARE_DYNAMIC(CSmartMenu)
	int ITEM_HEIGHT;
	int x, y, selected;
	int width, height;
	MENU menu;
	Color bgColor;//GDIPLUS_COLOR
	Color highlightColor;
	Color notselectedColor;
	bool mouseTracked;
public:
	enum { IDD = IDD_SMART_MENU };

	CSmartMenu(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSmartMenu();

	void Init(int X, int Y, const MENU& Menu);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNcDestroy();

	afx_msg BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT flags, CPoint point);
	afx_msg void OnLButtonDown(UINT flags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM WParam, LPARAM LParam);
	
};
