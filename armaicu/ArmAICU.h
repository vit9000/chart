// ArmAICU.h : main header file for the ArmAICU application
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "resource.h"       
#include "WinAppBase.h"
#include "DepDlg.h"


class CArmAICU : public CWinAppBase
{
public:
	CArmAICU();
	
private:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	bool ShowDepList(DeptInfo& deptInfo);
	DECLARE_MESSAGE_MAP()
};

