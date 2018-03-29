#pragma once
#include <sstream>
#include <vector>
#include <string>
#include "afxwin.h"
#include "Resource.h"
#include "dpix.h"
#include "SQL.h"
#include "DrugInfo.h"

using namespace std;
// DBDrugDialog dialog

class DBDrugDialog : public CDialog
{
	DECLARE_DYNAMIC(DBDrugDialog)

public:
	DBDrugDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~DBDrugDialog();


	enum { IDD = IDD_DIALOG_DRUG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	DrugInfo* drugInfo_ = nullptr;
	wstring header;
public:
	void Init(const wstring& name, DrugInfo& drugInfo);

	CEdit m_EditName;
	CComboBox m_ComboType;
	CEdit m_EditPercent;
	CEdit m_EditDose;
	CComboBox m_ComboUnit;
	CCheckListBox m_ListWays;
	afx_msg void OnBnClickedOk();
};
