#pragma once

#include "Resource.h"
#include <string>
#include "afxwin.h"
#include "DatabaseLoader.h"
using std::wstring;

// NewLineDialog dialog

class NewLineDialog : public CDialog
{

	DECLARE_DYNAMIC(NewLineDialog)
	int type;
	DrugInfo drugInfo;
	bool ready;
public:
	DatabaseLoader db;
	NewLineDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~NewLineDialog();
	
	const DrugInfo& getDrugInfo();
	int getType() { return type; }
// Dialog Data
	enum { IDD = IDD_NEW_LINE_DIALOG };

protected:

	CComboBox mDrugCombo;
	CListBox m_DrugList;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	void updateOkButton();

public:
	afx_msg void OnOKButtonClick();
	
	
	afx_msg void OnCbnSelchangeDrugCombo();
	CEdit m_DrugEdit;
	afx_msg void OnEnChangeDrugedit();
	CButton m_OkButton;
	afx_msg void OnLbnSelchangeDrugList();
};
