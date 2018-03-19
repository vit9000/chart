// NewLineDialog.cpp : implementation file
//

#include "stdafx.h"
#include "chart.h"
#include "NewLineDialog.h"


// NewLineDialog dialog

IMPLEMENT_DYNAMIC(NewLineDialog, CDialog)

NewLineDialog::NewLineDialog(CWnd* pParent /*=NULL*/)
	: CDialog(NewLineDialog::IDD, pParent), type(0)
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
}


BEGIN_MESSAGE_MAP(NewLineDialog, CDialog)
	ON_BN_CLICKED(IDOK, &NewLineDialog::OnOKButtonClick)
	ON_CBN_SELCHANGE(IDC_DRUG_COMBO, &NewLineDialog::OnCbnSelchangeDrugCombo)
	ON_EN_CHANGE(IDC_DRUGEDIT, &NewLineDialog::OnEnChangeDrugedit)
END_MESSAGE_MAP()

BOOL NewLineDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	mDrugCombo.AddString(L"В/в капельно");
	mDrugCombo.AddString(L"В/в болюсно");
	mDrugCombo.AddString(L"В/в дозатором");
	mDrugCombo.AddString(L"Таблетированные формы");
	


	return TRUE;  // return TRUE  unless you set the focus to a control
}


// NewLineDialog message handlers
wstring NewLineDialog::getString()
{
	
	return drugName;
}
void NewLineDialog::OnOKButtonClick()
{
	const int size = 128;
	wchar_t buf[size];
	int cur = m_DrugList.GetCurSel();
	m_DrugList.GetText(cur, buf);
	if(wcslen(buf)<=0)
	{
		MessageBox(L"Необходимо выбрать или ввести название препарата", L"Внимание");
		return;
	}
	
	drugName = wstring(buf);

	OnOK();
}



void NewLineDialog::OnCbnSelchangeDrugCombo()
{
	type = mDrugCombo.GetCurSel();
	/*
	DatabaseLoader db;

	vector<wstring> names;
	
	switch (type)
	{
	case 0:
		names = db.getDrugsIVDrops();
		break;
	case 1:
		names = db.getDrugsIVBolus();
		break;
	case 2:
		names = db.getDrugsIVInfusion();
		break;
	case 3:
		names = db.getDrugsTabs();
		break;
	}

	m_DrugList.ResetContent();
	for (const wstring& name : names)
	{
		m_DrugList.AddString(name.c_str());
	}*/
}


void NewLineDialog::OnEnChangeDrugedit()
{
	DatabaseLoader db;
	CString str;
	m_DrugEdit.GetWindowTextW(str);
	db.getDrugs(str.GetBuffer(), &m_DrugList);
}
