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

void ValueInputDlg::Init(const vector<wstring>& header_string, const vector<wstring>& editbox_content)
{
	header = CString(header_string[0].c_str());
	content = CString(editbox_content[0].c_str());
	count = header_string.size();
}

void ValueInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_value);
}

BOOL ValueInputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->SetWindowTextW(header);
	m_value.SetWindowTextW(content);
	HWND hwndControl;
	GetDlgItem(IDC_EDIT_VALUE, &hwndControl);
	PostMessage(WM_NEXTDLGCTL, (WPARAM)hwndControl, TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
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
	/*
	std::wstringstream ss(res);
	double value = 0;
	ss >> value;
	*/
	for(size_t i=0; i<count; i++)
		result.push_back(res);
	
	CDialogEx::OnOK();
}


