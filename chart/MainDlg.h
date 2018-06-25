// MainDlg.h : header file
//

#pragma once

#include <thread>

#include "CChartView.h"
#include "MainBridge.h"
#include "CHeader.h"
#include "IShowHide.h"
#include "CustomListView.h"
#include "PatientListItem.h"
#include "DutyDatePicker.h"



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
	void OnExecuteApp(UINT nID);
	void UpdatePatientList();
	CChartView* m_ChartView;// View
	CCustomListView m_PatientList;
	CDutyDatePicker m_DutyDatePicker;


	CHeader header;
	int patientListWidth;
	
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAppAbout();
	afx_msg void OnQuitApp();
	DECLARE_MESSAGE_MAP()

	
public:
	void setVisible(bool visible) override;
	afx_msg void OnLbnSelchangePatientList();
};
