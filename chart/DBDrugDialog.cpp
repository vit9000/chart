// DBDrugDialog.cpp : implementation file
//

#include "stdafx.h"
#include "chart.h"
#include "DBDrugDialog.h"
#include "afxdialogex.h"



// DBDrugDialog dialog

IMPLEMENT_DYNAMIC(DBDrugDialog, CDialog)

DBDrugDialog::DBDrugDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_DRUG, pParent)
{

}

DBDrugDialog::~DBDrugDialog()
{
}

void DBDrugDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_EditName);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_ComboType);
	DDX_Control(pDX, IDC_EDIT_PERCENT, m_EditPercent);
	DDX_Control(pDX, IDC_EDIT_DOSE, m_EditDose);
	DDX_Control(pDX, IDC_COMBO_UNIT, m_ComboUnit);
	//DDX_Control(pDX, IDC_LIST_WAYS, m_ListWays);
}


BEGIN_MESSAGE_MAP(DBDrugDialog, CDialog)
	ON_BN_CLICKED(IDOK, &DBDrugDialog::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL DBDrugDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(!header.empty())
		this->SetWindowTextW(header.c_str());

	m_ComboType.AddString(L"таблетки");
	m_ComboType.AddString(L"раствор");
	m_ComboType.AddString(L"суппозитории");
	m_ComboType.AddString(L"мазь");
	m_ComboType.AddString(L"капсулы");

	m_ComboUnit.AddString(L"г");
	m_ComboUnit.AddString(L"мг");
	m_ComboUnit.AddString(L"мкг");
	m_ComboUnit.AddString(L"мл");

	if (drugInfo_)
	{
		m_EditName.SetWindowTextW(drugInfo_->name.c_str());
		m_ComboType.SetWindowTextW(drugInfo_->type.c_str());
		m_EditPercent.SetWindowTextW(drugInfo_->percent.c_str());
		m_EditDose.SetWindowTextW(drugInfo_->dose.c_str());
		m_ComboUnit.SetWindowTextW(drugInfo_->ED.c_str());
		//m_ListWays.SetWindowTextW(drugInfo_->name.c_str());
	}
	CRect rect;
	GetClientRect(&rect);
	DPIX dpix;
	rect.top += dpix.getIntegerValue(155);
	rect.bottom -= dpix.getIntegerValue(40);
	rect.left += dpix.getIntegerValue(10);
	rect.right -= dpix.getIntegerValue(10);
	
	m_ListWays.Create(LBS_HASSTRINGS | LBS_OWNERDRAWFIXED | WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_BORDER, rect, this, IDC_LIST_WAYS);
	//checklist.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES| WS_BORDER);

	CFont* pFont = new CFont;
	VERIFY(pFont->CreateFont(
		14 * (double)DPIX(),                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,//ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH,  // nPitchAndFamily
		L"Arial"));                 // lpszFacename

	m_ListWays.SetFont(pFont);
	m_ListWays.ResetContent();
	SQL sql;
	sql.Connect();
	sql.SendRequest(L"SELECT * FROM admin_ways;");
	for (int i = 0; i < sql.CountStrings(); i++)
	{
		m_ListWays.AddString(sql.RecieveNextData()[1].c_str());
	}

	return TRUE;
}

void DBDrugDialog::Init(const wstring& name, DrugInfo& drugInfo)
{
	drugInfo_ = &drugInfo;
	header = name;
}


void DBDrugDialog::OnBnClickedOk()
{
	CString temp;

	m_EditName.GetWindowTextW(temp);
	drugInfo_->name = temp.GetBuffer();

	m_ComboType.GetWindowTextW(temp);
	drugInfo_->type = temp.GetBuffer();

	m_EditPercent.GetWindowTextW(temp);
	drugInfo_->percent = temp.GetBuffer();

	m_EditDose.GetWindowTextW(temp);
	drugInfo_->dose = temp.GetBuffer();

	m_ComboUnit.GetWindowTextW(temp);
	drugInfo_->ED = temp.GetBuffer();

	
	wstringstream aw;
	int c = m_ListWays.GetCount();
	for (int i = 0; i < c; i++)
	{
		if (m_ListWays.GetCheck(i))
			aw << i + 1 << " ";
	}
	drugInfo_->admin_ways = aw.str();


	vector<wstring> value = drugInfo_->getVector();
	
	wstring request = L"INSERT INTO druginfo (name, type, percent, dose, unit, admin_ways) VALUES(";
	for (auto& v : value)
	request += wstring(L"'") + wstring(v)+ wstring(L"'") + wstring((v!=value[value.size()-1])?L",":L");");
	SQL sql;
	sql.Connect();
	if (!sql.SendRequest(request))
		return;
	auto id = sql.getInsertID();
	wstringstream ss;
	ss << L"INSERT INTO drugname_linker (name, id) VALUES('" << header << L"'," << id << L");";
	if (!sql.SendRequest(ss.str()))
		return;
	// TODO: добавьте свой код обработчика уведомлений
	CDialog::OnOK();
}
