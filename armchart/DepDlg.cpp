// DepDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "ArmChart.h"
#include "DepDlg.h"
#include "afxdialogex.h"


// Диалоговое окно DepDlg

IMPLEMENT_DYNAMIC(DepDlg, CDialogEx)

DepDlg::DepDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DEP_DIALOG, pParent), selected_index(-1), dept_info(nullptr)
{

}

DepDlg::~DepDlg()
{
}

void DepDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEP_LIST, m_DepList);
}


BEGIN_MESSAGE_MAP(DepDlg, CDialogEx)
	
	ON_BN_CLICKED(IDOK, &DepDlg::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_DEP_LIST, &DepDlg::OnLbnSelchangeDepList)
END_MESSAGE_MAP()


// Обработчики сообщений DepDlg
BOOL DepDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (!dept_info) return FALSE;

	for (const auto& info : *dept_info)
	{
		m_DepList.AddString(info.name);
	}

	return TRUE;
}

void DepDlg::Init(const DeptInfoArrayPtr deptInfo)
{
	dept_info = deptInfo;
}



void DepDlg::OnBnClickedOk()
{
	
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnOK();
}


void DepDlg::OnLbnSelchangeDepList()
{
	selected_index = m_DepList.GetCurSel();
	// TODO: добавьте свой код обработчика уведомлений
}
