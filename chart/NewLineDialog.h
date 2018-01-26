#pragma once

#include "Resource.h"
#include <string>
#include "afxwin.h"
using std::wstring;

// NewLineDialog dialog

class NewLineDialog : public CDialog
{
	DECLARE_DYNAMIC(NewLineDialog)
	wstring drugName;
public:
	NewLineDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~NewLineDialog();
	
	wstring getString();
// Dialog Data
	enum { IDD = IDD_NEW_LINE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox mDrugCombo;
public:
	afx_msg void OnOKButtonClick();
};
