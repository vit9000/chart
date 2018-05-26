// MainDlg.h : header file
//

#pragma once

#include "CChartView.h"
#include "DatabaseLoader.h"
#include "CHeader.h"
#include "IShowHide.h"

// CMainDlg dialog
class CMainDlg : public CDialog, public IShowHide
{
// Construction
public:
	CMainDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CMainDlg();
// Dialog Data
	enum { IDD = IDD_CHART_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	BOOL PreTranslateMessage(MSG* pMsg);
	void SetPos();

// Implementation
protected:

	CChartView* chartView;// View
	CListBox patientList;
	CHeader header;
	
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

	
public:
	void setVisible(bool visible) override;
	bool getVisible() override;
	afx_msg void OnLbnSelchangePatientList();
};
