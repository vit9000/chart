#pragma once
#include <vector>
#include <string>
#include "Resource.h"
#include "afxwin.h"
#include "ugc.h"

// SmartMenu dialog
using namespace std;

class CSmartMenu : public CDialog
{
	DECLARE_DYNAMIC(CSmartMenu)
	int x, y;
	vector<wstring> info;
public:
	enum { IDD = IDD_SMART_MENU };

	CSmartMenu(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSmartMenu();

	void Init(int X, int Y, const vector<wstring>& Info);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNcDestroy();

	afx_msg BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
