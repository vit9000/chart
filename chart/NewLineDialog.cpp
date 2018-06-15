// NewLineDialog.cpp : implementation file
//

#include "stdafx.h"
#include "chart.h"
#include "NewLineDialog.h"


// NewLineDialog dialog

IMPLEMENT_DYNAMIC(NewLineDialog, CDialog)

NewLineDialog::NewLineDialog(bool AllowToChangeAdminWay, CWnd* pParent /* NULL */)
	: CDialog(NewLineDialog::IDD, pParent), allowToChangeAdminWay(AllowToChangeAdminWay), type(0), ready(false)
{

}

NewLineDialog::~NewLineDialog()
{
}

void NewLineDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DRUG_COMBO, m_DrugCombo);
	//DDX_Control(pDX, IDC_DRUG_LIST, m_DrugList);
	DDX_Control(pDX, IDC_DRUGEDIT, m_DrugEdit);
	DDX_Control(pDX, IDOK, m_OkButton);
}


BEGIN_MESSAGE_MAP(NewLineDialog, CDialog)
	ON_BN_CLICKED(IDOK, &NewLineDialog::OnOKButtonClick)
	ON_CBN_SELCHANGE(IDC_DRUG_COMBO, &NewLineDialog::OnCbnSelchangeDrugCombo)
	ON_EN_CHANGE(IDC_DRUGEDIT, &NewLineDialog::OnEnChangeDrugedit)
	//ON_LBN_SELCHANGE(IDC_DRUG_LIST, &NewLineDialog::OnLbnSelchangeDrugList)
	ON_BN_CLICKED(IDCANCEL, &NewLineDialog::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL NewLineDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	DatabaseLoader::getInstance().resetBufferedDrugs();
	CRect rect;
	GetClientRect(&rect);
	double dpix = DPIX();
	rect.left += static_cast<int>(10. * dpix);
	rect.right -= static_cast<int>(10. * dpix);
	rect.top += static_cast<int>(40. * dpix);
	rect.bottom -= static_cast<int>(60. * dpix);
	m_DrugList.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, IDC_DRUG_LIST);
	m_DrugList.Init(DatabaseLoader::getInstance().getDrugsPtr(), [this]() { this->OnLbnSelchangeDrugList(); });
	
	if(!allowToChangeAdminWay)
		m_DrugCombo.ShowWindow(false);
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
	wstring buf;
	int cur = m_DrugList.GetCurSel();
	m_DrugList.GetText(cur, buf);
	if(buf.empty())
	{
		MessageBox(L"Необходимо выбрать название препарата", L"Внимание");
		return;
	}
	
	if (DatabaseLoader::getInstance().getDrugInfo(buf, drugInfo))
	{
		if (!ready)
		{
			ready = true;
			LoadWaysToDrugCombo();
			m_DrugList.RedrawWindow();
			updateOkButton();
			return;
		}
		
	}
	else return;

	if(allowToChangeAdminWay && type<0)
	{
		MessageBox(L"Необходимо выбрать путь введения препарата", L"Внимание");
		return;
	}

	CString temp;
	m_DrugCombo.GetWindowTextW(temp);
	if (allowToChangeAdminWay)
		drugInfo.selected_admin_way = type;
	else
		drugInfo.selected_admin_way = ADMINWAY::DROPS;
	
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
	DatabaseLoader::getInstance().getDrugNames(
		str.GetBuffer(), 
		[this](bool loading) { 
			m_DrugList.ResetCursor(); 
			m_DrugList.setLoading(loading);
			if(!loading)
				m_DrugList.RedrawWindow(); 
		}, 
		!allowToChangeAdminWay);
	ready = false;
	updateOkButton();
}


void NewLineDialog::OnLbnSelchangeDrugList()
{
	const DrugInfoEx* di = m_DrugList.getSelectedDrugInfo();
	std::wstringstream ss;
	ss << di->percent << L"% " << di->dose << L" " << di->ED;
	MessageBox(ss.str().c_str(), L"Info", MB_OK);

	LoadWaysToDrugCombo();
	updateOkButton();
}

void NewLineDialog::LoadWaysToDrugCombo()
{
	if (!allowToChangeAdminWay) return;
	wstring buf;
	
	const DrugInfoEx* selectedDrugInfoEx = m_DrugList.getSelectedDrugInfo();
	if (!selectedDrugInfoEx) return;

	vector<wstring> list;
	DatabaseLoader::getInstance().getAllowedAdminWays(*selectedDrugInfoEx, list);
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
}

void NewLineDialog::updateOkButton()
{
	if (allowToChangeAdminWay)
		m_DrugCombo.ShowWindow(ready);
	else ready = true;
	m_OkButton.SetWindowTextW((ready ? L"OK" : L"Редактировать"));
	
	
}

void NewLineDialog::OnBnClickedCancel()
{
	
	CDialog::OnCancel();
}
