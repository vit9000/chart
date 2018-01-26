// NewLineDialog.cpp : implementation file
//

#include "stdafx.h"
#include "chart.h"
#include "NewLineDialog.h"


// NewLineDialog dialog

IMPLEMENT_DYNAMIC(NewLineDialog, CDialog)

NewLineDialog::NewLineDialog(CWnd* pParent /*=NULL*/)
	: CDialog(NewLineDialog::IDD, pParent)
{

}

NewLineDialog::~NewLineDialog()
{
}

void NewLineDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DRUG_COMBO, mDrugCombo);
}


BEGIN_MESSAGE_MAP(NewLineDialog, CDialog)
	ON_BN_CLICKED(IDOK, &NewLineDialog::OnOKButtonClick)
END_MESSAGE_MAP()

BOOL NewLineDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	mDrugCombo.AddString(L"NaCl 0.9%");
	mDrugCombo.AddString(L"Glucosae 5%");
	mDrugCombo.AddString(L"Glucosae 10%");
	mDrugCombo.AddString(L"Венофундин");
	mDrugCombo.AddString(L"Альбумин");
	mDrugCombo.AddString(L"Аминоплазмаль");
	mDrugCombo.AddString(L"KCl 4%");
	mDrugCombo.AddString(L"MgSO4 25%");
	mDrugCombo.AddString(L"Insulini");




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
