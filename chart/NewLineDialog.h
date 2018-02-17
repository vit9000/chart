#pragma once

#include "Resource.h"
#include <string>
#include "afxwin.h"
using std::wstring;

// NewLineDialog dialog

class NewLineDialog : public CDialog
{

	DECLARE_DYNAMIC(NewLineDialog)
	int type;
	wstring drugName;
public:
	NewLineDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~NewLineDialog();
	
	wstring getString();
	int getType() { return type; }
// Dialog Data
	enum { IDD = IDD_NEW_LINE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	void LoadDrugs();
	void LoadParams();
public:
	CComboBox mDrugCombo;
public:
	afx_msg void OnOKButtonClick();
	CButton m_RadioParam;
	CButton m_RadioDrug;
	afx_msg void OnBnClickedRadioDrug();
	afx_msg void OnBnClickedRadioParam();
};
