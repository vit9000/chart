// PatientListDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "ArmChart.h"
#include "PatientListDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "dpix.h"

// Диалоговое окно PatientListDlg

IMPLEMENT_DYNAMIC(PatientListDlg, CDialogEx)

PatientListDlg::PatientListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PATIENTLIST_DIALOG, pParent),
	patient_array(nullptr),
	selected(-1)
{

}

PatientListDlg::~PatientListDlg()
{
}

void PatientListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PATIENT_LIST, m_PatientList);
}


BEGIN_MESSAGE_MAP(PatientListDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PATIENT_LIST, &PatientListDlg::OnLvnItemchangedPatientList)
END_MESSAGE_MAP()


// Обработчики сообщений PatientListDlg

// Обработчики сообщений DepDlg
BOOL PatientListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(L"Пациенты отделения \"" + dept_name + "\"");

	m_PatientList.ModifyStyle(LVS_LIST, LVS_REPORT, 0); //- ставим режим таблицы
	m_PatientList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	if (!patient_array) return FALSE;

	DPIX dpix;
	std::vector<std::pair<CString, int> > headers{ 
		{L"ФИО",					200},
		{L"Возраст",				80}, 
		{L"NN",						80}, 
		{L"NN истории",				80},
		{L"Шифр",					80}, 
		{L"Профильное отделение",	80}, 
		{L"Диагноз",				80}, 
		{L"Врач",					80} };
	

	for(int i=0; i<static_cast<int>(headers.size()); i++)
		m_PatientList.InsertColumn(i, headers[i].first, LVCFMT_CENTER,  dpix(headers[i].second), i);//добавляем колонки

	for (int i = 0; i < static_cast<int>(patient_array->size()); i++)
	{
		auto& patient = (*patient_array)[i];
		int index = m_PatientList.InsertItem(LVIF_TEXT, i, patient[0]/*.fio*/, 0, 0, 0, NULL);
		for(int column=1; column < static_cast<int>(patient.size()-1); column++)
			m_PatientList.SetItemText(index, column, patient[column]);
		
	}

	return TRUE;
}

void PatientListDlg::Init(const CString& deptName, const PatientInfoArrayPtr PatientArray)
{
	patient_array = PatientArray;
	dept_name = deptName;
}


void PatientListDlg::OnLvnItemchangedPatientList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;

	selected = m_PatientList.GetNextItem(-1, LVNI_FOCUSED);;
}
