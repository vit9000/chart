// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chart.h"
#include "MainDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainDlg dialog
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent), IShowHide(), patientListWidth(DPIX()(300))
{
	chartView = NULL;
}

CMainDlg::~CMainDlg()
{
	if(chartView) delete chartView;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_PATIENT_LIST, patientList);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_SIZE()
	
	ON_LBN_SELCHANGE(IDC_PATIENT_LIST, &CMainDlg::OnLbnSelchangePatientList)
END_MESSAGE_MAP()


BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;                // Do not process further
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

// CMainDlg message handlers

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	DPIX dpix;
	
	chartView = new CChartView();
	chartView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(0,0,0,0), this, IDC_CHART);

	header.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, IDC_HEADER);
	header.SetFeadback(this);
	


	CRect rect;
	GetClientRect(&rect);
	rect.right = patientListWidth;
	patientList.Create(NULL, NULL,WS_VISIBLE| WS_CHILD, rect, this, IDC_PATIENT_LIST);
	patientList.SetCustomizations(false);
	//UpdatePatientList();
	//patientList.SetCurSel(0);
	
	
	
	
	this->SetWindowPos(this->GetParent(), 0, 0, dpix(1024.), dpix(600.), NULL);
	//ShowWindow(SW_MAXIMIZE);
	//SetPos();
	setVisible(true);
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CMainDlg::UpdatePatientList()
{
	DPIX dpix;
	patientList.Clear();
	for (const auto& pat : DatabaseLoader::getInstance().getPatientList(true))
	{
		patientList.AddItem(new CPatientListItem(&pat, dpix(40), [this]() {OnLbnSelchangePatientList(); }));
	}
}

void CMainDlg::SetPos()
{
	if (chartView)
	{
		CRect rect;
		GetClientRect(&rect);
		double dpix = DPIX();
		int top = (int)(dpix * 60);

		int left = (getVisible()) ? patientListWidth : 0;

		::SetWindowPos(GetDlgItem(IDC_HEADER)->m_hWnd, HWND_TOP,
			rect.left+left, rect.top,
			rect.Width(),
			top, NULL);

		/*::SetWindowPos(GetDlgItem(IDC_PATIENT_LIST)->m_hWnd, HWND_TOP,
			rect.left, rect.top+top,
			left,
			rect.Height(), NULL);
			*/
		
		::SetWindowPos(GetDlgItem(IDC_CHART)->m_hWnd, HWND_TOP,
			rect.left+left, rect.top+top,
			rect.Width(),
			rect.Height()-top, NULL);
	}
}
void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	
	SetPos();
	

	CDialog::OnSize(nType, cx, cy);
}

void CMainDlg::OnLbnSelchangePatientList()
{
	size_t index = static_cast<int>(patientList.GetCurSel());
	chartView->getController()->setPatient(index);
	header.LoadPatient(index);
	setVisible(false);
}

void CMainDlg::setVisible(bool visible)
{
	IShowHide::setVisible(visible);

	if (visible) UpdatePatientList();
	patientList.ShowWindow((visible) ? SW_SHOW : SW_HIDE);

	SetPos();
}
