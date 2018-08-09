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
double DPIX::dpix = 1.;

// CMainDlg dialog
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent), 
	IShowHide(), 
	patientListWidth(DPIX()(300)),
	ready(false)
{
	config = new CChartConfig();
	DPIX::SetDisplayDPI();
	DPIX dpix;
	headerHeight = dpix.getIntegerValue(60);
	toolbarHeight = dpix.getIntegerValue(27);
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
	//создание панели инструментов для переключения режима
	rect.top = 0;
	rect.left = 0;
	rect.bottom = toolbarHeight;
	rect.right = patientListWidth;
	m_modeToolBar.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, IDC_MODE_TOOLBAR);
	m_modeToolBar.SetParam(ST_FILL_ALL_WIDTH);
	m_modeToolBar.addButton(this, L"Наркозная карта", [this]() {SetAnesth_Mode(); }, 1);
	m_modeToolBar.addButton(this, L"Реанимац.карта", [this]() {SetICU_Mode(); }, 1);
	m_modeToolBar.setPressed(TIME_TYPE::ICU_CHART);

	//создание панели инструментов для управления картой (печать и т.д.)
	GetClientRect(&rect);
	rect.top = headerHeight;
	rect.left = patientListWidth;
	rect.bottom = headerHeight + toolbarHeight;
	rect.right += patientListWidth;
	m_chartToolBar.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, IDC_CHART_TOOLBAR);
	m_chartToolBar.addButton(this, L"Параметры пациента", [this]() { ShowPatientParametersDlg(); });
	m_chartToolBar.addButton(this, L"Печать", [this]() { Print(); });
	m_chartToolBar.setEnabled(false);

	//создание элемента управления датой и временем дежурства
	rect.top = toolbarHeight;
	rect.left = 0;
	rect.bottom = toolbarHeight+headerHeight;
	rect.right = patientListWidth;
	VString startDutyTime;
	MainBridge::getInstance().getDBParam<VString>(PARAM_BGN_TIME, startDutyTime); // загрузка параметра PARAM_BGN_TIME=42 ("Tags.h") из базы данных
	m_DutyDatePicker.ParseDateTime(startDutyTime.c_str());
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
					const PatientInfo& curPatient = m_ChartView.getModel()->getPatient();
					
					for (const auto& pat : MainBridge::getInstance().getPatientList(dutyDateTime, true))
					{
						m_PatientList.AddItem(new CPatientListItem(&pat, dpix(40), [this]() {OnLbnSelchangePatientList(); }));
						if (pat == curPatient)
						{
							int count = static_cast<int>(m_PatientList.Size());
							m_PatientList.SetCurSel(count - 1, false);
						}
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

	int left = (getVisible()) ? patientListWidth : 0;
	
	::SetWindowPos(GetDlgItem(IDC_CHART_TOOLBAR)->m_hWnd, HWND_TOP,
		rect.left + left, rect.top,
		rect.Width(),
		toolbarHeight, NULL);

	::SetWindowPos(GetDlgItem(IDC_HEADER)->m_hWnd, HWND_TOP,
		rect.left + left, rect.top+toolbarHeight,
		rect.Width(),
		headerHeight, NULL);

	

	::SetWindowPos(GetDlgItem(IDC_CHART)->m_hWnd, HWND_TOP,
		rect.left + left, rect.top + headerHeight + toolbarHeight,
		rect.Width(),
		rect.Height() - (headerHeight + toolbarHeight), NULL);

	

	::SetWindowPos(GetDlgItem(IDC_PATIENT_LIST)->m_hWnd, HWND_TOP,
		rect.left, rect.top + headerHeight + toolbarHeight,
		left,
		rect.Height() - (headerHeight + toolbarHeight), NULL);
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
	
	m_chartToolBar.setEnabled(false);
	m_Header.Clear();
	CWaitDlg dlg(this, L"Сохраняются изменения...", [this]() { m_ChartView.getModel()->SaveAndCloseChart(); });
	dlg.DoModal();
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnLbnSelchangePatientList()
{
	
	MainBridge& main_bridge = MainBridge::getInstance();
	COleDateTime date = m_DutyDatePicker.getStartDutyDateTime();
	size_t index = static_cast<int>(m_PatientList.GetCurSel());
	const PatientInfo& selectedPatient = main_bridge.getPatientList(date)[index];

	if (m_ChartView.getModel()->getPatient() == selectedPatient)
	{
		setVisible(false);
		return;
	}

	SaveAndCloseChart();

	auto& visitid = selectedPatient[PatientInfo::VISITID];

	int time_type = config->getChartType();
	

	auto func = [this, index, &main_bridge, &time_type, &date, &visitid](IDBResult& rs)
	{
		struct ChartDB { wstring keyid; wstring text; double bgnDate; double endDate; };
		vector<ChartDB> charts;
		
		// загружаем информацию о картах
		if (!rs.Eof())
		{
			ChartDB chart;
			VCopier<VString> vsc;
			rs.GetStrValue(L"ID", vsc);
			VString temp_str = vsc.get();
			chart.keyid = temp_str.c_str();//std::move(vsc);
			rs.GetStrValue(L"TEXT", vsc);
			temp_str = vsc.get();
			chart.text = temp_str.c_str();//std::move(vsc);
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
		m_chartToolBar.setEnabled(true);
		MainBridge::getInstance().setLoading(false);
		m_ChartView.ResetCurPos();
	};


	
	QueryParameters params(3);
	params.push_back(QueryParameter(L"VISIT_ID", visitid.c_str()));
	params.push_back(QueryParameter(L"DAT", DateToString(date).c_str()));
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
void CMainDlg::ShowPatientParametersDlg()
{
	const PatientInfo& pi = m_ChartView.getModel()->getPatient();
	ValueInputDlg dlg;
	dlg.Init(L"Параметры пациента", { L"Рост", L"Вес" }, {L"см", L"кг"}, { pi[PatientInfo::HEIGHT], pi[PatientInfo::WEIGHT] });
	if (dlg.DoModal() == IDOK)
	{
		PatientInfo updatedPI(pi);
		const vector<Value>& values = dlg.getValue();
		updatedPI[PatientInfo::HEIGHT] = values[0];
		updatedPI[PatientInfo::WEIGHT] = values[1];
		m_ChartView.getModel()->updatePatient(updatedPI);
		m_Header.LoadPatient();
		/*
		далее сохраняем в базе данных
		*/

	}
}
//------------------------------------------------------------------------------------------------
#include <GdiPlus.h> 
void CMainDlg::Print()
{
	CPrintDialog dlg(FALSE);


	/*if (!dlg.GetDefaults())
	{
		AfxMessageBox(_T("You have no default printer!"));
	}
	else
	{
		// attach to the DC we were given
		CDC dc;
		dc.Attach(dlg.m_pd.hDC);

		// ask for the measurements
		int nHorz = dc.GetDeviceCaps(LOGPIXELSX);
		int nVert = dc.GetDeviceCaps(LOGPIXELSY);

		// almost always the same in both directions, but sometimes not!
		CString str;
		if (nHorz == nVert)
		{
			str.Format(_T("Your printer supports %d pixels per inch"), nHorz);
		}
		else
		{
			str.Format(_T("Your printer supports %d pixels per inch ")
				_T("horizontal resolution, and %d pixels per inch vertical ")
				_T("resolution"), nHorz, nVert);
		}

		// tell the user
		AfxMessageBox(str);

		// Note: no need to call Detach() because we want the CDC destructor
		// to call FreeDC() on the DC we borrowed from the common dialog
	}*/


	if (dlg.DoModal() == IDOK)
	{
		CPrintDialog dlg(FALSE);
		dlg.GetDefaults();
		// is a default printer set up?
		HDC hdcPrinter = dlg.GetPrinterDC();
		if (hdcPrinter == NULL)
		{
			//MessageBox(_T("Buy a printer!"));
		}
		else
		{
			// create a CDC and attach it to the default printer
			CDC dcPrinter;
			dcPrinter.Attach(hdcPrinter);

			int scr_xLogPPI = dcPrinter.GetDeviceCaps(LOGPIXELSX);
			int scr_yLogPPI = dcPrinter.GetDeviceCaps(LOGPIXELSY);


			// call StartDoc() to begin printing
			DOCINFO docinfo;
			memset(&docinfo, 0, sizeof(docinfo));
			docinfo.cbSize = sizeof(docinfo);
			docinfo.lpszDocName = _T("CDC::StartDoc() Code Fragment");

			// if it fails, complain and exit gracefully
			if (dcPrinter.StartDoc(&docinfo) < 0)
			{
				//MessageBox(_T("Printer wouldn't initalize"));
			}
			else
			{
				// start a page
				if (dcPrinter.StartPage() < 0)
				{
					//MessageBox(_T("Could not start page"));
					dcPrinter.AbortDoc();
				}
				else
				{
					//dcPrinter.SetMapMode(MM_HIENGLISH);
					dcPrinter.SetMapMode(MM_ANISOTROPIC);
					
					UGC ugc(dcPrinter.m_hDC, 0, 0);

					m_ChartView.PrintAll(ugc);
					/*Gdiplus::Graphics g (dcPrinter.m_hDC);
					
					Gdiplus::SolidBrush mySolidBrush(Color(255, 255, 0, 0));

					g.FillRectangle(&mySolidBrush, 0, 0, 500, 800);
					*/
					//m_ChartView.PrintAll(ugc);
					
					/*HBITMAP hbitmap;
					m_ChartView.PrintAll(&hbitmap);
					CBitmap *pBitmap = CBitmap::FromHandle(hbitmap);
					BITMAP bm;
					pBitmap->GetBitmap(&bm);
					CDC MemDC;
					MemDC.CreateCompatibleDC(&dcPrinter);
					MemDC.SelectObject(pBitmap);
					dcPrinter.BitBlt(0, 0, bm.bmWidth*10, bm.bmHeight, &MemDC, 0, 0, SRCCOPY);
					*/
					/*CBrush brush;
					brush.CreateSolidBrush(RGB(50, 151, 151));
					
					RECT rect = { 30, 50, 2000, 5000 };
					dcPrinter.FillRect(&rect, &brush);
					*/

					/*auto font = CreateFont(
						3'000,                        // nHeight
						1'500,                         // nWidth
						0,                         // nEscapement
						0,                         // nOrientation
						FW_NORMAL,                 // nWeight
						FALSE,                     // bItalic
						FALSE,                     // bUnderline
						0,                         // cStrikeOut
						ANSI_CHARSET,              // nCharSet
						OUT_DEFAULT_PRECIS,        // nOutPrecision
						CLIP_DEFAULT_PRECIS,       // nClipPrecision
						DEFAULT_QUALITY,           // nQuality
						DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
						_T("Arial"));                 // lpszFacename
					dcPrinter.SelectObject(&font);
					dcPrinter.TextOut(450, 450, _T("Hello World!"), 12);*/
					dcPrinter.EndPage();
					dcPrinter.EndDoc();
					//dcPrinter.SelectObject(pOldFont);
				}
			}
			CDC::FromHandle(hdcPrinter)->DeleteDC();
		}
		
	}
}