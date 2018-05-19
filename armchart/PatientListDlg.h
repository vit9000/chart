#pragma once
#include <vector>
#include "PatientInfo.h"


// Диалоговое окно PatientListDlg
typedef std::vector<PatientInfo>* PatientInfoArrayPtr;

class PatientListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PatientListDlg)

public:
	PatientListDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	~PatientListDlg() override;
	void Init(const CString& deptName, const PatientInfoArrayPtr PatienArray);
	inline int getSelected() { return selected; }
// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATIENTLIST_DIALOG };
#endif
private:
	CListCtrl m_PatientList;
	PatientInfoArrayPtr patient_array;
	CString dept_name;
	int selected;
protected:
	void DoDataExchange(CDataExchange* pDX) override;    // поддержка DDX/DDV
	BOOL OnInitDialog() override;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedPatientList(NMHDR *pNMHDR, LRESULT *pResult);
};
