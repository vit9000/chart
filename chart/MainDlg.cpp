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
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_SIZE()
	
END_MESSAGE_MAP()


// CMainDlg message handlers

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect;
	GetClientRect(&rect);
	chart = new CChart();
	chart->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, IDC_CHART);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	
	CRect rect;
	GetClientRect(&rect);
	if(chart)
	{
		::SetWindowPos(GetDlgItem(IDC_CHART)->m_hWnd, HWND_TOP,
											0, 0, rect.Width(),
											rect.Height(), NULL);
	}

	CDialog::OnSize(nType, cx, cy);
}
