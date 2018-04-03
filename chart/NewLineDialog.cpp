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
	DDX_Control(pDX, IDC_DRUG_COMBO, m_DrugCombo);
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

	/*mDrugCombo.AddString(L"В/в капельно");
	mDrugCombo.AddString(L"В/в болюсно");
	mDrugCombo.AddString(L"В/в дозатором");
	mDrugCombo.AddString(L"Таблетированные формы");*/
	
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
		MessageBox(L"Необходимо выбрать название препарата", L"Внимание");
		return;
	}
	
	if (DatabaseLoader::getInstance().getDrugInfo(wstring(buf), drugInfo))
	{
		if (!ready)
		{
			ready = true;
			updateOkButton();
			return;
		}
		
	}
	else return;

	if(type<0)
	{
		MessageBox(L"Необходимо выбрать путь введения препарата", L"Внимание");
		return;
	}

	CString temp;
	m_DrugCombo.GetWindowTextW(temp);
	drugInfo.selected_way_name = wstring(temp.GetBuffer());
	drugInfo.selected_way = type;
	
	OnOK();
}

void NewLineDialog::OnCbnSelchangeDrugCombo()
{
	int index = m_DrugCombo.GetCurSel();
	CString temp;
	m_DrugCombo.GetWindowTextW(temp);
	type = DatabaseLoader::getInstance().getAdminWayType(temp.GetBuffer());
}


void NewLineDialog::OnEnChangeDrugedit()
{
	CString str;
	m_DrugEdit.GetWindowTextW(str);
	DatabaseLoader::getInstance().getDrugNames(str.GetBuffer(), &m_DrugList);
	ready = false;
	updateOkButton();
}


void NewLineDialog::OnLbnSelchangeDrugList()
{
	const int size = 128;
	wchar_t buf[size];
	int cur = m_DrugList.GetCurSel();
	m_DrugList.GetText(cur, buf);
	DrugInfo drugInfo;
	//ready = db.getExistsDrugInfo(wstring(buf), drugInfo);
	auto list = DatabaseLoader::getInstance().getAllowedAdminWays(wstring(buf));
	ready = (!list.empty()) ? true : false;
	m_DrugCombo.ResetContent();
	for (const auto& l : list)
		m_DrugCombo.AddString(l.c_str());
	if (m_DrugCombo.GetCount() > 0)
	{
		m_DrugCombo.SetCurSel(0);
		type = -1;
		OnCbnSelchangeDrugCombo();
		
	}
	updateOkButton();
}

void NewLineDialog::updateOkButton()
{
	m_OkButton.SetWindowTextW((ready ? L"OK" : L"Редактировать"));
	m_DrugCombo.ShowWindow(ready);
	
	
}