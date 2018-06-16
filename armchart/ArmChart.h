// ArmStacDoctor.h : main header file for the ArmStacDoctor application
/////////////////////////////////////////////////////////////////////////////
#pragma warning( disable: 4786 )

#pragma once

#include "resource.h"       
#include "CoolCtrls.h"
#include "SpeedButton.h" 
#include "WinAppBase.h"
#include "DepDlg.h"
#include "PatientListDlg.h"
#include "PatientInfo.h"

#include "type_defines.h"
/////////////////////////////////////////////////////////////////////////////
// 
#include "MainFrm.h"
#include <string>

#include "DBConnector.h"



class CArmChart : public CWinAppBase
{
public:
	CArmChart();
	
private:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	bool ShowDepList(DeptInfo& deptInfo);
	
	
	CString m_DepID;

	
	DECLARE_MESSAGE_MAP()



	//friend class DBConnector;
	
	
	
	//void GetDrugList(const TCHAR * sql, std::vector<std::wstring>& drug_list);
};

