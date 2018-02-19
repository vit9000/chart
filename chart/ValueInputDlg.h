#pragma once


// ValueInputDlg dialog
#include <utility>
using std::pair;

class ValueInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ValueInputDlg)

public:
	ValueInputDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ValueInputDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSERT_UNIT_DIALOG};
#endif
	double getValue() { return result; }
protected:
	double result;
	CEdit m_value;
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
