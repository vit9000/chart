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
	: CDialog(CMainDlg::IDD, pParent)
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
	DDX_Control(pDX, IDC_PATIENT_LIST, patientList);
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

	CRect rect;
	GetClientRect(&rect);
	chartView = new CChartView();
	rect.top+= static_cast<int>(150 * DPIX());
	rect.left += static_cast<int>(150 * DPIX());
	chartView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, IDC_CHART);


	
	GetClientRect(&rect);
	rect.bottom = static_cast<int>(150 * DPIX());
	header.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, IDC_HEADER);
	header.SetFeadback(this);
	
	//DatabaseLoader::getInstance().LoadDatabase();
	int countPatients = DatabaseLoader::getInstance().countPatients();//chartView->getModel()->getCountPatients();

	for(int i=0; i<countPatients; ++i )
	{
		patientList.AddString(DatabaseLoader::getInstance().getPatient(i).name.c_str());
		
	}
	//if(countPatients>0) 
	//patientList.SetCurSel(0);
	chartView->getModel()->setPatient(0);
	header.LoadPatient(0);
	
	DPIX dpix;
	this->SetWindowPos(this->GetParent(), 0, 0, dpix.getIntegerValue(1024.), dpix.getIntegerValue(600.), NULL);
	//ShowWindow(SW_MAXIMIZE);
	SetPos();
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CMainDlg::SetPos()
{
	if (chartView)
	{
		CRect rect;
		GetClientRect(&rect);
		double dpix = DPIX();
		int top = (int)(dpix * 60);

		::SetWindowPos(GetDlgItem(IDC_HEADER)->m_hWnd, HWND_TOP,
			rect.left, rect.top,
			rect.Width(),
			top, NULL);

		int left = (patientList.IsWindowVisible()) ? (int)(dpix * 150) : 0;

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
	patientList.ShowWindow((visible) ? SW_SHOW : SW_HIDE);
	SetPos();
}

bool CMainDlg::getVisible()
{
	return patientList.IsWindowVisible();
}
