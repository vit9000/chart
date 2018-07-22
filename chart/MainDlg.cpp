// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chart.h"
#include "MainDlg.h"
#include "common_cmd.h"
#include "Tags.h"
#include "ChartConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CChartConfig* config;
CWnd *parentDlg = nullptr;
extern bool chart_debug;

// CMainDlg dialog
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent), 
	IShowHide(), 
	patientListWidth(DPIX()(300))
{
	m_ChartView = nullptr;
	config = new CChartConfig();
}

CMainDlg::~CMainDlg()
{
	if(m_ChartView) delete m_ChartView;
	if (config) delete config;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_PATIENT_LIST, patientList);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_PATIENT_LIST,OnLbnSelchangePatientList)
	ON_COMMAND(ID_MENU_ABOUT, OnAppAbout)
	ON_COMMAND(ID_MENU_QUIT, OnQuitApp)
	ON_COMMAND(ID_MENU_CHANGE_DEPT, OnChangeDept)
	ON_COMMAND(ID_MENU_UNDO, OnUndo)
	ON_COMMAND(ID_MENU_REDO, OnRedo)
	ON_COMMAND_RANGE(CM_COMMON_APP_MENU, CM_COMMON_APP_MENU + 100, OnExecuteApp)
	ON_COMMAND(ID_MENU_ICUMODE, &CMainDlg::OnMenuICU_Mode)
	ON_COMMAND(ID_MENU_ANESTHMODE, &CMainDlg::OnMenuAnesth_Mode)
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
		else if (pMsg->wParam == VK_CONTROL)
		{
			chart_debug = !chart_debug;
			if (m_ChartView) m_ChartView->getModel()->redrawView();
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
	header.SetModel(m_ChartView->getModel());
	
	
	CRect rect;
	int dutyPickerHeight = dpix.getIntegerValue(60);
	

	//создание элемента управлени€ датой и временем дежурства
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

	//создаем главное меню приложени€ в стиле јриадны
	parentDlg = this;
	CMenu * pMenu = GetMenu();
	MainBridge::getInstance().setAppMenu(pMenu);

	CMenu * editMenu = pMenu->GetSubMenu(1);
	if (m_ChartView) m_ChartView->getModel()->setEditMenu (editMenu);
	//отображаем список пациентов и загружаем их из Ѕƒ
	setVisible(true);
	return TRUE; 
}
//------------------------------------------------------------------------------------------------
void CMainDlg::UpdatePatientList()
{
	std::thread t([this]()
				{
					m_PatientList.setLoading(true);
					DPIX dpix;
					m_PatientList.Clear();
					double dutyDateTime = static_cast<double>(m_DutyDatePicker.getStartDutyDateTime());
					for (const auto& pat : MainBridge::getInstance().getPatientList(dutyDateTime, true))
					{
						m_PatientList.AddItem(new CPatientListItem(&pat, dpix(40), [this]() {OnLbnSelchangePatientList(); }));
					}
					m_PatientList.setLoading(false);
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
void CMainDlg::SaveAndCloseChart()
{
	if (!m_ChartView->getModel()->isChartLoaded()) return;

	CWaitDlg dlg(this, L"—охран€ютс€ изменени€...", [this]() { m_ChartView->getModel()->SaveAndCloseChart(); });
	dlg.DoModal();
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnLbnSelchangePatientList()
{
	SaveAndCloseChart();

	size_t index = static_cast<int>(m_PatientList.GetCurSel());

	MainBridge& main_bridge = MainBridge::getInstance();
	COleDateTime date = m_DutyDatePicker.getStartDutyDateTime();

	// определ€ем режим работы карты по состо€нию меню
	int time_type = TIME_TYPE::ICU_CHART;
	CMenu *pMenu = GetMenu();
	if (pMenu != NULL)
	{
		UINT state = pMenu->GetMenuState(ID_MENU_ANESTHMODE, MF_BYCOMMAND);
		if (state & MF_CHECKED)
			time_type = TIME_TYPE::ANESTH_CHART;
	}// определили режим работы карты

	auto& visitid = main_bridge.getPatientList(date)[index][PatientInfo::VISITID];

	auto func = [this, index, &main_bridge, &time_type, &date, &visitid](IDBResult& rs)
	{
		struct ChartDB { wstring keyid; wstring text; };
		vector<ChartDB> charts;
		// загружаем информацию о картах
		if (!rs.Eof())
		{
			ChartDB chart;
			VCopier<wstring> vsc;
			rs.GetStrValue(L"ID", vsc);
			chart.keyid = std::move(vsc);
			rs.GetStrValue(L"TEXT", vsc);
			chart.text = std::move(vsc);
			charts.push_back(std::move(chart));

			rs.Next();
		}

		/* здесь будет диалоговое окно дл€ выбора действий 
		- создать карту назначений или загрузить
		- загрузить наркозную карту (если есть в этот день)
		- создать наркозную карту
		*/
		wstring chart_id;
		if (charts.size() == 0)
		{
			wstring msg = (time_type == TIME_TYPE::ICU_CHART) ? L" арта ведени€ реанимационного пациента" : L"Ќаркозна€ карта";
			msg += L" на данные сутки не создавалась. —оздать новую?";
			if (MessageBox(msg.c_str(), L"ѕодтверждение", MB_YESNO) == IDYES)
			{
				main_bridge.createNewChart(time_type, date, visitid, chart_id);
			}
			else return;
		}
		else chart_id = charts[0].keyid;
		if (chart_id.empty())
		{
			MessageBox(L"ќшибка идентификации карты", L"ќшибка", MB_OK | MB_ICONERROR);
			return;
		}
		MainBridge::getInstance().setLoading(true);
		m_ChartView->getModel()->setPatient(index, chart_id);
		header.LoadPatient();
		setVisible(false);
		MainBridge::getInstance().setLoading(false);
		m_ChartView->ResetCurPos();
	};


	vector<QueryParameter> params;
	params.push_back(QueryParameter(L"VISIT_ID", visitid));
	params.push_back(QueryParameter(L"DAT", DateToString(date)));
	params.push_back(QueryParameter(L"TIME_TYPE", time_type));
	main_bridge.sendSQLRequest(L"sql_GetChartList", params, func);
	
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
	OnCancel();
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnChangeDept()
{
	SaveAndCloseChart();
	OnOK();
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnUndo()
{
	if (m_ChartView)
		m_ChartView->getModel()->undo();
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnRedo()
{
	if (m_ChartView)
		m_ChartView->getModel()->redo();
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnCancel()
{
	SaveAndCloseChart();
	CDialog::OnCancel();
}
//------------------------------------------------------------------------------------------------


void CMainDlg::OnMenuICU_Mode()
{
	// TODO: добавьте свой код обработчика команд
	CMenu *pMenu = GetMenu();
	if (pMenu != NULL)
	{
		pMenu->CheckMenuItem(ID_MENU_ICUMODE, MF_CHECKED | MF_BYCOMMAND);
		pMenu->CheckMenuItem(ID_MENU_ANESTHMODE, MF_UNCHECKED | MF_BYCOMMAND);
	}
}


void CMainDlg::OnMenuAnesth_Mode()
{
	// TODO: добавьте свой код обработчика команд
	CMenu *pMenu = GetMenu();
	if (pMenu != NULL)
	{
		pMenu->CheckMenuItem(ID_MENU_ANESTHMODE, MF_CHECKED | MF_BYCOMMAND);
		pMenu->CheckMenuItem(ID_MENU_ICUMODE, MF_UNCHECKED | MF_BYCOMMAND);
	}
}
