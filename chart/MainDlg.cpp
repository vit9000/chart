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


// CMainDlg message handlers

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect;
	GetClientRect(&rect);
	chartView = new CChartView();
	rect.left += static_cast<int>(150 * DPIX());
	chartView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, IDC_CHART);

	size_t countPatients = chartView->getModel()->getCountPatients();
	for(size_t i=0; i<countPatients; ++i )
	{
		patientList.AddString(chartView->getModel()->getPatient(i)->getName().c_str());
		
	}
	if(countPatients>0) 
		patientList.SetCurSel(0);
	chartView->getModel()->setPatient(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	
	CRect rect;
	GetClientRect(&rect);
	int left = static_cast<int>(150 * DPIX());

	
	
	if(chartView)
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
	chartView->getController()->setPatient(index);
}
