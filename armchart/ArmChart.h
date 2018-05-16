// ArmMoving.h : main header file for the ArmMoving application
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "resource.h"   
#include "WinAppBase.h"

//#define MAXSIZEKEYID 30
//#define NUMBORDTYPE 31



BOOL GetDepInfo(CString & strID, int & nType, CString & strText);
BOOL IsGZ(const ID_TYPE & i);

/////////////////////////////////////////////////////////////////////////////
// ArmMoving:

class CArmChart : public CWinAppBase
{
public:
	CArmChart();

	//{{AFX_VIRTUAL(CArmMoving)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

public:
	//{{AFX_MSG(CArmMoving)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
