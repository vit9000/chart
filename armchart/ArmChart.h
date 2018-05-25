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
#include "DrugStore.h"
#include "type_defines.h"
/////////////////////////////////////////////////////////////////////////////
// 
#include "MainFrm.h"
#include <string>



class CArmChart : public CWinAppBase
{
public:
	CArmChart();
	wchar_t* DLLResult;
	std::wstring DLLres;
	//{{AFX_VIRTUAL(CArmStacDoctor)
private:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	bool ShowPatientList(const DeptInfo& deptInfo, PatientInfo& patientInfo);
	//}}AFX_VIRTUAL
	bool ShowDepList(DeptInfo& deptInfo);
	std::wstring LoadFile();
	

	//{{AFX_MSG(CArmStacDoctor)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	DrugStore drug_store;

	friend const DrugList& CallbackForDrugs(const std::wstring& drug);
	
	//void GetDrugList(const TCHAR * sql, std::vector<std::wstring>& drug_list);
};

