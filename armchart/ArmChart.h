// ArmStacDoctor.h : main header file for the ArmStacDoctor application
/////////////////////////////////////////////////////////////////////////////
#pragma warning( disable: 4786 )

#pragma once

#include "resource.h"       
#include "CoolCtrls.h"
#include "SpeedButton.h" 
#include "WinAppBase.h"
#include "DepDlg.h"
/////////////////////////////////////////////////////////////////////////////
// ArmStacDoctor:

class CArmChart : public CWinAppBase
{
public:
	CArmChart();

	//{{AFX_VIRTUAL(CArmStacDoctor)
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void FillPatGrid();
	//}}AFX_VIRTUAL
	bool ShowDepList(DeptInfo& deptInfo);

public:
	//{{AFX_MSG(CArmStacDoctor)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

