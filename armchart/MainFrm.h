#pragma once
// MainFrm.h : interface of the CMainFrame class
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "ArmVersion.h"
/*#include "Tabs.h"
#include "CoolCtrls.h"
#include "SpravToolBar.h"

#include "HistoryWnd.h"
#include "HistoryProfileTypWnd.h"
#include "HistoryRecordTypeWnd.h"
#include "DescTmplWnd.h"
#include "HistorySearchWnd.h"
#include "FormPanelWnd.h"
#include "JournalWnd.h"
#include "HospitalizationListWnd.h"
#include "Schedule/DocVedomFrm.h"*/
#include "MainFrmBase.h"
#include "Global.h"
#include "LogonDlg.h"


/////////////////////////////////////////////////////////////////////////////

class CMainFrame : public CMainFrmBase
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	~CMainFrame();

protected:
	CString         ms_bgn_date;
	CString         ms_end_date;

public:
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

protected:
	
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg BOOL  _CreateUnic();
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnClose();
	BOOL IsDocVedMode();
	LRESULT OnConnectionFailed(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	CWnd * GetCurrPane();
	void SetCurrPane(CWnd * pWnd);

public:
	
};
