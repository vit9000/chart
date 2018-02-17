// ValueInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chart.h"
#include "ValueInputDlg.h"
#include "afxdialogex.h"


// ValueInputDlg dialog

IMPLEMENT_DYNAMIC(ValueInputDlg, CDialogEx)

ValueInputDlg::ValueInputDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INSERT_UNIT_DIALOG, pParent)
{

}

ValueInputDlg::~ValueInputDlg()
{
}

void ValueInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_value);
	DDX_Control(pDX, IDC_EDIT_DURATION, m_duration);

}




BEGIN_MESSAGE_MAP(ValueInputDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ValueInputDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// ValueInputDlg message handlers

#include <sstream>
#include <string>
void ValueInputDlg::OnBnClickedOk()
{
	CString temp;
	
	m_value.GetWindowTextW(temp);
	std::wstring res(temp.GetBuffer());

	m_duration.GetWindowTextW(temp);
	res += L" " + std::wstring(temp.GetBuffer());

	std::wstringstream ss(res);
	double value = 0;
	int duration = 0;
	ss >> value >> duration;

	result = std::make_pair(value, duration);

	CDialogEx::OnOK();
}
