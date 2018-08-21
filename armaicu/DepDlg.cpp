// DepDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "ArmAICU.h"
#include "DepDlg.h"
//#include "afxdialog.h"


// Диалоговое окно DepDlg

IMPLEMENT_DYNAMIC(DepDlg, CDialog)

DepDlg::DepDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DEP_DIALOG, pParent), selected_index(-1), dept_info(NULL)
{

}

DepDlg::~DepDlg()
{
}

void DepDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEP_LIST, m_DepList);
}


BEGIN_MESSAGE_MAP(DepDlg, CDialog)
	
	ON_BN_CLICKED(IDOK, &DepDlg::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_DEP_LIST, &DepDlg::OnLbnSelchangeDepList)
END_MESSAGE_MAP()


// Обработчики сообщений DepDlg
BOOL DepDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (!dept_info) return FALSE;

//	for (const auto& info : *dept_info)
	for(int i=0; i<(int)dept_info->size(); i++)
	{
		const DeptInfo& info = dept_info->at(i);
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
	CDialog::OnOK();
}


void DepDlg::OnLbnSelchangeDepList()
{
	selected_index = m_DepList.GetCurSel();
	// TODO: добавьте свой код обработчика уведомлений
}
