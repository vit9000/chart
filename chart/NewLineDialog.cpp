// NewLineDialog.cpp : implementation file
//

#include "stdafx.h"
#include "chart.h"
#include "NewLineDialog.h"


// NewLineDialog dialog

IMPLEMENT_DYNAMIC(NewLineDialog, CDialog)

NewLineDialog::NewLineDialog(CWnd* pParent /*=NULL*/)
	: CDialog(NewLineDialog::IDD, pParent), type(0), ready(false)
{

}

NewLineDialog::~NewLineDialog()
{
}

void NewLineDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DRUG_COMBO, mDrugCombo);
	DDX_Control(pDX, IDC_DRUG_LIST, m_DrugList);
	DDX_Control(pDX, IDC_DRUGEDIT, m_DrugEdit);
	DDX_Control(pDX, IDOK, m_OkButton);
}


BEGIN_MESSAGE_MAP(NewLineDialog, CDialog)
	ON_BN_CLICKED(IDOK, &NewLineDialog::OnOKButtonClick)
	ON_CBN_SELCHANGE(IDC_DRUG_COMBO, &NewLineDialog::OnCbnSelchangeDrugCombo)
	ON_EN_CHANGE(IDC_DRUGEDIT, &NewLineDialog::OnEnChangeDrugedit)
	ON_LBN_SELCHANGE(IDC_DRUG_LIST, &NewLineDialog::OnLbnSelchangeDrugList)
END_MESSAGE_MAP()

BOOL NewLineDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	/*mDrugCombo.AddString(L"�/� ��������");
	mDrugCombo.AddString(L"�/� �������");
	mDrugCombo.AddString(L"�/� ���������");
	mDrugCombo.AddString(L"��������������� �����");*/
	
	updateOkButton();


	return TRUE;  // return TRUE  unless you set the focus to a control
}


// NewLineDialog message handlers
const DrugInfo& NewLineDialog::getDrugInfo()
{	
	return drugInfo;
}
void NewLineDialog::OnOKButtonClick()
{
	const int size = 128;
	wchar_t buf[size];
	int cur = m_DrugList.GetCurSel();
	m_DrugList.GetText(cur, buf);
	if(wcslen(buf)<=0)
	{
		MessageBox(L"���������� ������� ��� ������ �������� ���������", L"��������");
		return;
	}
	
	if (db.getDrugInfo(wstring(buf), drugInfo))
	{
		if (!ready)
		{
			ready = true;
			updateOkButton();
			return;
		}
		
	}
	else return;

	OnOK();
}



void NewLineDialog::OnCbnSelchangeDrugCombo()
{
	type = mDrugCombo.GetCurSel();
	
	
}


void NewLineDialog::OnEnChangeDrugedit()
{
	
	CString str;
	m_DrugEdit.GetWindowTextW(str);
	db.getDrugNames(str.GetBuffer(), &m_DrugList);
	ready = false;
	updateOkButton();
}


void NewLineDialog::OnLbnSelchangeDrugList()
{
	const int size = 128;
	wchar_t buf[size];
	type = m_DrugList.GetCurSel();
	m_DrugList.GetText(type, buf);
	DrugInfo drugInfo;
	ready = Drugstore::getInstance().isDrugInfoExists(wstring(buf), drugInfo);
	auto list = Drugstore::getInstance().getAllowedAdminWays(wstring(buf));
	mDrugCombo.ResetContent();
	for (const auto& l : list)
		mDrugCombo.AddString(l.c_str());
	if (mDrugCombo.GetCount() > 0)
		mDrugCombo.SetCurSel(0);
	updateOkButton();
}

void NewLineDialog::updateOkButton()
{
	m_OkButton.SetWindowTextW((ready ? L"OK" : L"�������������"));
	mDrugCombo.ShowWindow(ready);

	
}