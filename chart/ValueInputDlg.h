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
	const pair<double, int>& getValues() { return result; }
protected:
	pair<double, int> result;
	CEdit m_value;
	CEdit m_duration;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
