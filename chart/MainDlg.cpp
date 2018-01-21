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
	chart = NULL;
}

CMainDlg::~CMainDlg()
{
	if(chart) delete chart;
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


// CMainDlg message handlers

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect;
	GetClientRect(&rect);
	chart = new CChart();
	rect.left += static_cast<int>(150 * DPIX());
	chart->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, IDC_CHART);
	for(size_t i=0; i<chart->getModel()->getCountPatients(); ++i )
	{
		patientList.AddString(chart->getModel()->getPatient(i)->getName().c_str());
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	
	CRect rect;
	GetClientRect(&rect);
	int left = static_cast<int>(150 * DPIX());

	
	
	if(chart)
	{
		/*::SetWindowPos(GetDlgItem(IDC_PATIENT_LIST)->m_hWnd, HWND_TOP,
											rect.left, rect.top, 
											rect.Width()- left,
											rect.Height(), NULL);*/
		rect.left += left;
		::SetWindowPos(GetDlgItem(IDC_CHART)->m_hWnd, HWND_TOP,
											rect.left, rect.top, 
											rect.Width(),
											rect.Height(), NULL);
	}

	CDialog::OnSize(nType, cx, cy);
}

void CMainDlg::OnLbnSelchangePatientList()
{
	size_t index = static_cast<int>(patientList.GetCurSel());
	chart->setDatabase(index);
}
