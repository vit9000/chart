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
	wstring drugName;
public:
	NewLineDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~NewLineDialog();
	
	wstring getString();
	int getType() { return type; }
// Dialog Data
	enum { IDD = IDD_NEW_LINE_DIALOG };

protected:

	CComboBox mDrugCombo;
	CListBox m_DrugList;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	

public:
	afx_msg void OnOKButtonClick();
	
	
	afx_msg void OnCbnSelchangeDrugCombo();
};
