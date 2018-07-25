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
	patientListWidth(DPIX()(300)),
	ready(false)
{
	config = new CChartConfig();
}

CMainDlg::~CMainDlg()
{
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
			m_ChartView.getModel()->redrawView();
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
	// создание карты назначений
	m_ChartView.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(0,0,0,0), this, IDC_CHART);

	//создание заголовка карты назначений
	m_Header.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, IDC_HEADER);
	m_Header.SetFeadback(this);
	m_Header.SetModel(m_ChartView.getModel());
	
	CRect rect;
	int headerHeight = dpix.getIntegerValue(80);
	
	//создание панели инструментов для переключения режима
	rect.top = 0;
	rect.left = 0;
	rect.bottom = headerHeight / 2;
	rect.right = patientListWidth;
	m_modeToolBar.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, IDC_MODE_TOOLBAR);
	m_modeToolBar.addButton(this, L"Наркозная карта", [this]() {SetAnesth_Mode(); }, 1);
	m_modeToolBar.addButton(this, L"Реанимац.карта", [this]() {SetICU_Mode(); }, 1);
	m_modeToolBar.setPressed(TIME_TYPE::ICU_CHART);


	//создание элемента управления датой и временем дежурства
	rect.top = headerHeight/2;
	rect.left = 0;
	rect.bottom = headerHeight;
	rect.right = patientListWidth;
	wstring startDutyTime;
	MainBridge::getInstance().getDBParam<wstring>(PARAM_BGN_TIME, startDutyTime); // загрузка параметра PARAM_BGN_TIME=42 ("Tags.h") из базы данных
	m_DutyDatePicker.ParseDateTime(startDutyTime);
	m_DutyDatePicker.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, IDC_DUTY_PICKER);	

	//создание меню списка пациентов на отделении
	GetClientRect(&rect);
	rect.top = headerHeight;
	rect.right = patientListWidth;
	m_PatientList.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, IDC_PATIENT_LIST);
	m_PatientList.SetCustomizations(false);

	this->SetWindowPos(this->GetParent(), 0, 0, dpix(1024), dpix(600), NULL);
	//создаем главное меню приложения в стиле Ариадны
	parentDlg = this;
	CMenu * pMenu = GetMenu();
	MainBridge::getInstance().setAppMenu(pMenu);

	CMenu * editMenu = pMenu->GetSubMenu(1);
	m_ChartView.getModel()->setEditMenu (editMenu);
	//отображаем список пациентов и загружаем их из БД
	setMode();
	ready = true;
	return TRUE; 
}
//------------------------------------------------------------------------------------------------
void CMainDlg::UpdatePatientList()
{
	//SaveAndCloseChart();
	
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
	CRect rect;
	GetClientRect(&rect);
	DPIX dpix;
	int top = dpix.getIntegerValue(80);

	int left = (getVisible()) ? patientListWidth : 0;
	
	::SetWindowPos(GetDlgItem(IDC_HEADER)->m_hWnd, HWND_TOP,
		rect.left + left, rect.top,
		rect.Width(),
		top, NULL);


	::SetWindowPos(GetDlgItem(IDC_PATIENT_LIST)->m_hWnd, HWND_TOP,
		rect.left, rect.top + top,
		left,
		rect.Height(), NULL);


	::SetWindowPos(GetDlgItem(IDC_CHART)->m_hWnd, HWND_TOP,
		rect.left + left, rect.top + top,
		rect.Width(),
		rect.Height() - top, NULL);
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	if(ready)SetPos();
	CDialog::OnSize(nType, cx, cy);
}
//------------------------------------------------------------------------------------------------
void CMainDlg::SaveAndCloseChart()
{
	if (!m_ChartView.getModel()->isChartLoaded()) return;
	
	m_Header.Clear();
	CWaitDlg dlg(this, L"Сохраняются изменения...", [this]() { m_ChartView.getModel()->SaveAndCloseChart(); });
	dlg.DoModal();
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnLbnSelchangePatientList()
{
	SaveAndCloseChart();

	size_t index = static_cast<int>(m_PatientList.GetCurSel());

	MainBridge& main_bridge = MainBridge::getInstance();
	COleDateTime date = m_DutyDatePicker.getStartDutyDateTime();
	

	
	auto& visitid = main_bridge.getPatientList(date)[index][PatientInfo::VISITID];

	int time_type = config->getChartType();
	

	auto func = [this, index, &main_bridge, &time_type, &date, &visitid](IDBResult& rs)
	{
		struct ChartDB { wstring keyid; wstring text; double bgnDate; double endDate; };
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
			chart.bgnDate = rs.GetDateValue(L"BGNDAT");
			chart.endDate = rs.GetDateValue(L"ENDDAT");
			charts.push_back(std::move(chart));

			rs.Next();
		}

		/* здесь будет диалоговое окно для выбора действий 
		- создать карту назначений или загрузить
		- загрузить наркозную карту (если есть в этот день)
		- создать наркозную карту
		*/
		if (charts.size() == 0)
		{
			wstring msg = (time_type == TIME_TYPE::ICU_CHART) ? L"Карта ведения реанимационного пациента" : L"Наркозная карта";
			msg += L" на данные сутки не создавалась.";
			if (time_type == TIME_TYPE::ANESTH_CHART)
			{
				COleDateTime enddate = m_DutyDatePicker.getEndDutyDateTime();
				COleDateTime dt = COleDateTime::GetCurrentTime();
				if (dt < date || dt >= enddate)
				{
					MessageBox(msg.c_str(), L"Внимание", MB_OK | MB_ICONINFORMATION);
					return;
				}
			}
			msg += L" Создать новую?";
			
			if (MessageBox(msg.c_str(), L"Подтверждение", MB_YESNO) == IDYES)
			{
				ChartDB chart;
				if (time_type == TIME_TYPE::ANESTH_CHART) // если наркозная карта, то время должно быть текущим. Разве что с точностью до 
				{
					COleDateTime dt = COleDateTime::GetCurrentTime();
					int step = config->getStep();
					date = (double)COleDateTime(dt.GetYear(), dt.GetMonth(), dt.GetDay(), dt.GetHour(), dt.GetMinute() / step * step, 0);
				}
				chart.bgnDate = date;
				main_bridge.createNewChart(time_type, chart.bgnDate, chart.endDate, visitid, chart.keyid);
				charts.push_back(std::move(chart));
			}
			else return;
		}
		
		const ChartDB& loadedChart = charts[0];
		if (loadedChart.keyid.empty())
		{
			MessageBox(L"Ошибка идентификации карты", L"Ошибка", MB_OK | MB_ICONERROR);
			return;
		}
		MainBridge::getInstance().setLoading(true);
		m_ChartView.getModel()->setPatient(index, loadedChart.keyid, loadedChart.bgnDate, loadedChart.endDate);
		m_Header.LoadPatient();
		setVisible(false);
		MainBridge::getInstance().setLoading(false);
		m_ChartView.ResetCurPos();
	};


	vector<QueryParameter> params;
	params.push_back(QueryParameter(L"VISIT_ID", visitid));
	params.push_back(QueryParameter(L"DAT", DateToString(date)));
	params.push_back(QueryParameter(L"TIME_TYPE", time_type));
	main_bridge.sendSQLRequest(L"sql_GetChartList_2", params, func);
	
}
//------------------------------------------------------------------------------------------------
void CMainDlg::setVisible(bool visible)
{
	IShowHide::setVisible(visible);

	if (visible) UpdatePatientList();
	m_PatientList.ShowWindow((visible) ? SW_SHOW : SW_HIDE);
	m_DutyDatePicker.ShowWindow((visible) ? SW_SHOW : SW_HIDE);
	m_modeToolBar.ShowWindow((visible) ? SW_SHOW : SW_HIDE);
	(visible) ? m_PatientList.SetFocus() : m_ChartView.SetFocus();
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
	m_ChartView.getModel()->undo();
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnRedo()
{
	m_ChartView.getModel()->redo();
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnCancel()
{
	SaveAndCloseChart();
	CDialog::OnCancel();
}
//------------------------------------------------------------------------------------------------
void CMainDlg::setMode(int MODE)
{
	SaveAndCloseChart();
	if(MODE == TIME_TYPE::ICU_CHART)
		config->setTimes(TIME_TYPE::ICU_CHART, 1440, 60, 30);
	else
		config->setTimes(TIME_TYPE::ANESTH_CHART, 180, 10, 10); // в наркозной карте максимальное время сдвинется в зависимости от данных из БД
	m_Header.RedrawWindow();
	m_ChartView.RedrawWindow();
	setVisible(true);
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnMenuICU_Mode()
{
	SetICU_Mode();
	m_modeToolBar.setPressed(TIME_TYPE::ICU_CHART);
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnMenuAnesth_Mode()
{
	SetAnesth_Mode();
	m_modeToolBar.setPressed(TIME_TYPE::ANESTH_CHART);
}
//------------------------------------------------------------------------------------------------
void CMainDlg::SetICU_Mode()
{
	CMenu *pMenu = GetMenu();
	if (pMenu != NULL)
	{
		UINT state = pMenu->GetMenuState(ID_MENU_ICUMODE, MF_BYCOMMAND);
		if (state & MF_CHECKED)
			return;

		pMenu->CheckMenuItem(ID_MENU_ICUMODE, MF_CHECKED | MF_BYCOMMAND);
		pMenu->CheckMenuItem(ID_MENU_ANESTHMODE, MF_UNCHECKED | MF_BYCOMMAND);
		setMode(TIME_TYPE::ICU_CHART);
	}
}
//------------------------------------------------------------------------------------------------
void CMainDlg::SetAnesth_Mode()
{
	// TODO: добавьте свой код обработчика команд
	CMenu *pMenu = GetMenu();
	if (pMenu != NULL)
	{
		UINT state = pMenu->GetMenuState(ID_MENU_ANESTHMODE, MF_BYCOMMAND);
		if (state & MF_CHECKED)
			return;

		pMenu->CheckMenuItem(ID_MENU_ANESTHMODE, MF_CHECKED | MF_BYCOMMAND);
		pMenu->CheckMenuItem(ID_MENU_ICUMODE, MF_UNCHECKED | MF_BYCOMMAND);
		setMode(TIME_TYPE::ANESTH_CHART);
	}
}
//------------------------------------------------------------------------------------------------
