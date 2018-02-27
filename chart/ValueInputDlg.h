#pragma once


// ValueInputDlg dialog
#include <string>
#include "Value.h"
using std::wstring;

class ValueInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ValueInputDlg)
	CString header;
	CString content;
	int type;
public:
	enum {STANDART=0, HEMODYNAMIC};
	ValueInputDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ValueInputDlg();
	void Init(const wstring& header_string, const wstring& editbox_content, int dialog_type=0);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSERT_UNIT_DIALOG};
#endif
	Value getValue() { return result; }
protected:
	Value result;
	CEdit m_value;
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
