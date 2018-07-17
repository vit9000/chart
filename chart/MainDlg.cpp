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
extern bool chart_debug;
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
	ON_LBN_SELCHANGE(IDC_PATIENT_LIST,OnLbnSelchangePatientList)
	ON_COMMAND(ID_MENU_ABOUT, OnAppAbout)
	ON_COMMAND(ID_MENU_QUIT, OnQuitApp)
	ON_COMMAND(ID_MENU_CHANGE_DEPT, OnChangeDept)
	ON_COMMAND(ID_MENU_UNDO, OnUndo)
	ON_COMMAND(ID_MENU_REDO, OnRedo)
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

	// �������� ����� ����������
	m_ChartView = new CChartView();
	m_ChartView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(0,0,0,0), this, IDC_CHART);

	//�������� ��������� ����� ����������
	header.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, IDC_HEADER);
	header.SetFeadback(this);
	header.SetModel(m_ChartView->getModel());
	
	
	CRect rect;
	int dutyPickerHeight = dpix.getIntegerValue(60);
	

	//�������� �������� ���������� ����� � �������� ���������
	GetClientRect(&rect);
	rect.bottom = dutyPickerHeight;
	rect.right = patientListWidth;
	wstring startDutyTime;
	MainBridge::getInstance().getDBParam<wstring>(PARAM_BGN_TIME, startDutyTime); // �������� ��������� PARAM_BGN_TIME=42 ("Tags.h") �� ���� ������
	m_DutyDatePicker.ParseDateTime(startDutyTime);
	m_DutyDatePicker.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, IDC_DUTY_PICKER);	
	
	//�������� ���� ������ ��������� �� ���������
	GetClientRect(&rect);
	rect.top = dutyPickerHeight;
	rect.right = patientListWidth;
	m_PatientList.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, IDC_PATIENT_LIST);
	m_PatientList.SetCustomizations(false);

	//������� ������� ���� ���������� � ����� �������
	parentDlg = this;
	CMenu * pMenu = GetMenu();
	MainBridge::getInstance().setAppMenu(pMenu);

	CMenu * editMenu = pMenu->GetSubMenu(1);
	if (m_ChartView) m_ChartView->getModel()->setEditMenu (editMenu);
	//���������� ������ ��������� � ��������� �� �� ��
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

	MainBridge& main_bridge = MainBridge::getInstance();
	COleDateTime date = m_DutyDatePicker.getStartDutyDateTime();
	int time_type = TIME_TYPE::ICU_CHART;
	auto& visitid = main_bridge.getPatientList(date)[index][PatientInfo::VISITID];

	auto func = [this, index, &main_bridge, &time_type, &date, &visitid](IDBResult& rs)
	{
		struct ChartDB { wstring keyid; wstring text; };
		vector<ChartDB> charts;
		// ��������� ���������� � ������
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

		/* ����� ����� ���������� ���� ��� ������ �������� 
		- ������� ����� ���������� ��� ���������
		- ��������� ��������� ����� (���� ���� � ���� ����)
		- ������� ��������� �����
		*/
		wstring chart_id;
		if (charts.size() == 0)
		{
			if (MessageBox(L"����� ���������� �� ������ ����� �� �����������. ������� �����?", L"�������������", MB_YESNO) == IDYES)
			{
				main_bridge.createNewChart(time_type, date, visitid, chart_id);
				OnLbnSelchangePatientList();
				return;
			}
			else return;
		}
		else chart_id = charts[0].keyid;
		if (chart_id.empty())
		{
			MessageBox(L"������ ������������� �����", L"������", MB_OK | MB_ICONERROR);
			return;
		}
		m_ChartView->getModel()->setPatient(index, chart_id);
		header.LoadPatient();
		setVisible(false);
	};


	vector<QueryParameter> params;
	params.push_back(QueryParameter(L"VISIT_ID", visitid));
	params.push_back(QueryParameter(L"DAT", DateToString(date)));
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
		m_ChartView->getModel()->undo();
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnRedo()
{
	if (m_ChartView)
		m_ChartView->getModel()->redo();
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnUpdateUndoMenuItem(CCmdUI *pCmdUI)
{
	//bool Enabled = (m_ChartView && m_ChartView->getModel()->isUndoAvailable());
	//pCmdUI->Enable(Enabled);
}
//------------------------------------------------------------------------------------------------
void CMainDlg::OnUpdateRedoMenuItem(CCmdUI *pCmdUI)
{
	//bool Enabled = (m_ChartView && m_ChartView->getModel()->isRedoAvailable());
	
	//pCmdUI->Enable(Enabled);
	//pCmdUI->m_pParentMenu->EnableMenuItem(pCmdUI->m_nID, MF_BYPOSITION | MF_GRAYED); //(m, 1, MF_BYPOSITION | MF_GRAYED); )
}
//------------------------------------------------------------------------------------------------