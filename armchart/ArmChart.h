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
/////////////////////////////////////////////////////////////////////////////
// ArmStacDoctor:
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

	friend const wchar_t* CallbackForDrugs(const wchar_t* request);
	std::wstring GetDrugsFromApteka(const std::wstring& drug);

	void GetDrugList(const TCHAR * sql, std::vector<CString>& drug_list);
};

