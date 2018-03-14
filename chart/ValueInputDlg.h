#pragma once


// ValueInputDlg dialog
#include <string>
#include "Value.h"
#include "afxcmn.h"
#include "EditableList.h"
using std::wstring;

class ValueInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ValueInputDlg)
	CString header;
	vector<wstring> param;
	vector<wstring> content;
	vector<Value> result;
	size_t count;
	EditableList main_list;
public:
	enum {STANDART=0, HEMODYNAMIC};
	ValueInputDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ValueInputDlg();
	void Init(const wstring& block_name, const vector<wstring>& parameters, const vector<wstring>& editbox_content);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSERT_UNIT_DIALOG};
#endif
	vector<Value> getValue() { return  result; }
protected:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	
};
