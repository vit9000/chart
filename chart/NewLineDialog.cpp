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
	DDX_Control(pDX, IDC_RADIO_PARAM, m_RadioParam);
	DDX_Control(pDX, IDC_RADIO_DRUG, m_RadioDrug);
}


BEGIN_MESSAGE_MAP(NewLineDialog, CDialog)
	ON_BN_CLICKED(IDOK, &NewLineDialog::OnOKButtonClick)
	ON_BN_CLICKED(IDC_RADIO_DRUG, &NewLineDialog::OnBnClickedRadioDrug)
	ON_BN_CLICKED(IDC_RADIO_PARAM, &NewLineDialog::OnBnClickedRadioParam)
END_MESSAGE_MAP()

BOOL NewLineDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_RadioDrug.SetCheck(1);
	LoadDrugs();




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
	mDrugCombo.GetWindowTextW(buf, size);
	if(wcslen(buf)<=0)
	{
		MessageBox(L"Необходимо выбрать или ввести название препарата", L"Внимание");
		return;
	}
	
	drugName = wstring(buf);

	OnOK();
}


void NewLineDialog::LoadDrugs()
{
	type = 0;
	mDrugCombo.ResetContent();
	mDrugCombo.AddString(L"NaCl 0.9%");
	mDrugCombo.AddString(L"Glucosae 5%");
	mDrugCombo.AddString(L"Glucosae 10%");
	mDrugCombo.AddString(L"Венофундин");
	mDrugCombo.AddString(L"Альбумин");
	mDrugCombo.AddString(L"Аминоплазмаль");
	mDrugCombo.AddString(L"KCl 4%");
	mDrugCombo.AddString(L"MgSO4 25%");
	mDrugCombo.AddString(L"Insulini");
}

void NewLineDialog::LoadParams()
{
	type = 1;
	mDrugCombo.ResetContent();
	mDrugCombo.AddString(L"Систолическое АД");
	mDrugCombo.AddString(L"Диастолическое АД");
	mDrugCombo.AddString(L"ЧСС");
	mDrugCombo.AddString(L"В зонд");
	mDrugCombo.AddString(L"По дрежанам");
	mDrugCombo.AddString(L"Диурез");
	
}

void NewLineDialog::OnBnClickedRadioDrug()
{
	LoadDrugs();
}


void NewLineDialog::OnBnClickedRadioParam()
{
	LoadParams();
}
