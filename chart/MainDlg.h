// MainDlg.h : header file
//

#pragma once

#include "CChart.h"

// CMainDlg dialog
class CMainDlg : public CDialog
{
// Construction
public:
	CMainDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CMainDlg();
// Dialog Data
	enum { IDD = IDD_CHART_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	CChart* chart;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};
