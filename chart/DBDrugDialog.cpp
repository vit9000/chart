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
	//DDX_Control(pDX, IDC_LIST_WAYS, m_ListWays);
}


BEGIN_MESSAGE_MAP(DBDrugDialog, CDialog)
	ON_BN_CLICKED(IDOK, &DBDrugDialog::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &DBDrugDialog::OnCbnSelchangeComboType)
END_MESSAGE_MAP()

BOOL DBDrugDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(!header.empty())
		this->SetWindowTextW(header.c_str());

	

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
	
	/*m_ListWays.Create(LBS_HASSTRINGS | LBS_OWNERDRAWFIXED | WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_BORDER, rect, this, IDC_LIST_WAYS);
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
	m_ListWays.ResetContent();*/
	m_ListWays.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | WS_BORDER | LBS_NOTIFY, rect, this, IDC_LIST_WAYS);
	//main_list.SetFocus();
	m_ListWays.ModifyStyle(LVS_LIST, LVS_REPORT, 0); //- ставим режим таблицы
	m_ListWays.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	
	SQL sql;
	sql.Connect();
	sql.SendRequest(L"SELECT * FROM admin_ways;");
	m_ListWays.InsertColumn(0, L"", LVCFMT_CENTER, dpix.getIntegerValue(20));//добавляем колонки
	m_ListWays.InsertColumn(1, L"Параметр", LVCFMT_CENTER, dpix.getIntegerValue(215));//добавляем колонки
	for (int i = 0; i < sql.CountStrings(); i++)
	{
		auto temp = sql.RecieveNextData()[1];
		int index = m_ListWays.InsertItem(LVIF_TEXT, i, L"", 0, 0, 0, NULL);
		m_ListWays.SetItemText(index, 1, temp.c_str());//time
		
	}

	sql.SendRequest(L"SELECT * FROM types;");
	for (int i = 0; i < sql.CountStrings(); i++)
		m_ComboType.AddString(sql.RecieveNextData()[1].c_str());
	
	sql.SendRequest(L"SELECT * FROM units;");
	for (int i = 0; i < sql.CountStrings(); i++)
		m_ComboUnit.AddString(sql.RecieveNextData()[1].c_str());
	
	OnCbnSelchangeComboType();
	return TRUE;
}

void DBDrugDialog::Init(const wstring& name, DrugInfo& drugInfo)
{
	drugInfo_ = &drugInfo;
	header = name;
}


void DBDrugDialog::OnBnClickedOk()
{
	// сохраняем данные из полей в drugInfo_
	CString temp;
	drugInfo_->dbname = header;

	m_EditName.GetWindowTextW(temp);
	if (!IsFieldValid(temp.GetBuffer()))
		return;
	drugInfo_->name = temp.GetBuffer();
	

	m_ComboType.GetWindowTextW(temp);
	if (!IsFieldValid(temp.GetBuffer()))
		return;
	drugInfo_->type = temp.GetBuffer();
	

	if (m_EditPercent.IsWindowEnabled())
	{
		m_EditPercent.GetWindowTextW(temp);
		if (!IsFieldValid(temp.GetBuffer()))
			return;
		drugInfo_->percent = temp.GetBuffer();
		
	}

	m_EditDose.GetWindowTextW(temp);
	if (!IsFieldValid(temp.GetBuffer()))
		return;
	drugInfo_->dose = temp.GetBuffer();
	
	m_ComboUnit.GetWindowTextW(temp);
	if (!IsFieldValid(temp.GetBuffer()))
		return;
	drugInfo_->ED = temp.GetBuffer();
	
	wstringstream aw;
	int c = m_ListWays.GetItemCount();//GetCount();
	for (int i = 0; i < c; i++)
	{
		if (m_ListWays.GetCheck(i))
			aw << i + 1 << " ";
	}
	if (!IsFieldValid(aw.str()))
		return;
	drugInfo_->admin_ways = aw.str();

	// делаем запросы в базу данных и сохраняем данные
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


void DBDrugDialog::OnCbnSelchangeComboType()
{
	CString temp;
	m_ComboType.GetWindowTextW(temp);
	m_EditPercent.EnableWindow((temp == L"раствор" || temp == L"крем"));
}
