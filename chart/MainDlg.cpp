// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chart.h"
#include "MainDlg.h"
#include "common_cmd.h"
#include "Tags.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWnd *parentDlg = nullptr;

// CMainDlg dialog
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent), 
	IShowHide(), 
	patientListWidth(DPIX()(300))
{
	m_ChartView = nullptr;
}

CMainDlg::~CMainDlg()
{
	if(m_ChartView) delete m_ChartView;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_PATIENT_LIST, patientList);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_PATIENT_LIST, &CMainDlg::OnLbnSelchangePatientList)
	ON_COMMAND(ID_MENU_ABOUT, OnAppAbout)
	ON_COMMAND(ID_MENU_QUIT, OnQuitApp)
	ON_COMMAND(ID_MENU_CHANGE_DEPT, OnChangeDept)
	ON_COMMAND(ID_MENU_UNDO, OnUndo)
	ON_COMMAND_RANGE(CM_COMMON_APP_MENU, CM_COMMON_APP_MENU + 100, OnExecuteApp)
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------------
BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;                // Do not process further
		}
		else if (pMsg->wParam == VK_PAUSE)
		{
			MainBridge::getInstance().showLogDlg();
			return TRUE;
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}
//------------------------------------------------------------------------------------------------
BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	DPIX dpix;
	this->SetWindowPos(this->GetParent(), 0, 0, dpix(1024), dpix(600), NULL);

	// создание карты назначений
	m_ChartView = new CChartView();
	m_ChartView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(0,0,0,0), this, IDC_CHART);

	//создание заголовка карты назначений
	header.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, IDC_HEADER);
	header.SetFeadback(this);
	
	
	CRect rect;
	int dutyPickerHeight = dpix.getIntegerValue(60);
	

	//создание элемента управления датой и временем дежурства
	GetClientRect(&rect);
	rect.bottom = dutyPickerHeight;
	rect.right = patientListWidth;
	wstring startDutyTime;
	MainBridge::getInstance().getDBParam<wstring>(PARAM_BGN_TIME, startDutyTime); // загрузка параметра PARAM_BGN_TIME=42 ("Tags.h") из базы данных
	m_DutyDatePicker.ParseDateTime(startDutyTime);
	m_DutyDatePicker.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, IDC_DUTY_PICKER);	
	
	//создание меню списка пациентов на отделении
	GetClientRect(&rect);
	rect.top = dutyPickerHeight;
	rect.right = patientListWidth;
	m_PatientList.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, IDC_PATIENT_LIST);
	m_PatientList.SetCustomizations(false);

	//создаем главное меню приложения в стиле Ариадны
	parentDlg = this;
	CMenu *pMenu = GetMenu();
	MainBridge::getInstance().setAppMenu(pMenu);

	//отображаем список пациентов и загружаем их из БД
	setVisible(true);
	return TRUE; 
}
//------------------------------------------------------------------------------------------------
void CMainDlg::UpdatePatientList()
{
	std::thread t([this]()
				{
					m_PatientList.SetLoading(true);
					DPIX dpix;
					m_PatientList.Clear();
					double dutyDateTime = static_cast<double>(m_DutyDatePicker.getStartDutyDateTime());
					for (const auto& pat : MainBridge::getInstance().getPatientList(dutyDateTime, true))
					{
						m_PatientList.AddItem(new CPatientListItem(&pat, dpix(40), [this]() {OnLbnSelchangePatientList(); }));
					}
					m_PatientList.SetLoading(false);
					//patientList.RedrawWindow();
				});
	t.detach();
}
//------------------------------------------------------------------------------------------------
void CMainDlg::SetPos()
{
	if (m_ChartView)
	{
		CRect rect;
		GetClientRect(&rect);
		DPIX dpix;
		int top = dpix.getIntegerValue(60);

		int left = (getVisible()) ? patientListWidth : 0;

		::SetWindowPos(GetDlgItem(IDC_HEADER)->m_hWnd, HWND_TOP,
			rect.left+left, rect.top,
			rect.Width(),
			top, NULL);

		::SetWindowPos(GetDlgItem(IDC_PATIENT_LIST)->m_hWnd, HWND_TOP,
			rect.left, rect.top+top,
			left,
			rect.Height(), NULL);

		
		::SetWindowPos(GetDlgItem(IDC_CHART)->m_hWnd, HWND_TOP,
			rect.left+left, rect.top+top,
			rect.Width(),
			rect.Height()-top, NULL);


	}
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	SetPos();
	CDialog::OnSize(nType, cx, cy);
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnLbnSelchangePatientList()
{
	size_t index = static_cast<int>(m_PatientList.GetCurSel());
	m_ChartView->getController()->setPatient(index);
	header.LoadPatient(index);
	setVisible(false);
}
//------------------------------------------------------------------------------------------------
void CMainDlg::setVisible(bool visible)
{
	IShowHide::setVisible(visible);

	if (visible) UpdatePatientList();
	m_PatientList.ShowWindow((visible) ? SW_SHOW : SW_HIDE);
	m_DutyDatePicker.ShowWindow((visible) ? SW_SHOW : SW_HIDE);
	(visible) ? m_PatientList.SetFocus() : m_ChartView->SetFocus();
	SetPos();
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnExecuteApp(UINT nID)
{
	MainBridge::getInstance().executeApp(nID);
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnAppAbout()
{
	MainBridge::getInstance().showAboutDlg();
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnQuitApp()
{
	CDialog::OnCancel();
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnChangeDept()
{
	CDialog::OnOK();
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnUndo()
{
	if (m_ChartView)
		m_ChartView->undo();
}
