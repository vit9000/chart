#pragma once

#include "Resource.h"
#include <string>
#include "afxwin.h"
#include "MainBridge.h"
#include "DrugListView.h"
#include "AdminWay.h"
using std::wstring;

// NewLineDialog dialog
extern CWnd* parentDlg;

class NewLineDialog : public CDialog
{

	DECLARE_DYNAMIC(NewLineDialog)
	int type;
	DrugInfo drugInfo;
	bool ready;
	bool allowToChangeAdminWay;
public:
	NewLineDialog(bool AllowToChangeAdminWay,CWnd* pParent = parentDlg);   // standard constructor
	
	virtual ~NewLineDialog();
	
	const DrugInfo& getDrugInfo();
	int getType() { return type; }
// Dialog Data
	enum { IDD = IDD_NEW_LINE_DIALOG };

protected:

	CComboBox m_DrugCombo;
	DrugListView m_DrugList;
	CEdit m_DrugEdit;
	CButton m_OkButton;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	void updateOkButton();
	afx_msg void OnOKButtonClick();
	afx_msg void OnCbnSelchangeDrugCombo();
	afx_msg void OnEnChangeDrugedit();
	afx_msg void OnLbnSelchangeDrugList();
	void LoadWaysToDrugCombo();
public:
	afx_msg void OnBnClickedCancel();
};
