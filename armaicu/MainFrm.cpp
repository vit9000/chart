
#include "stdafx.h"
#include <atlconv.h>
//#include <fstream.h>

#include "commondb_rc.h"
#include "resource.h"

#include "MainFrm.h"
/*#include "CommondbGlobal.h"
#include "ImageDlg.h"
#include "OptionDlg.h"
#include "ReportMaker.h"
#include "ExcelCash.h"

#include "ADOConnection.h"
#include "ProcDlg.h"
#include "Query.h"

#include "SQL.h"
#include "univ.h"
#include "ArmVersion.h"
#include "PropertiesDlg.h"
#include "ChangePasswordDlg.h"
#include "commondb_rc.h"
#include "univ.h"*/
////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMainFrmBase)

BEGIN_MESSAGE_MAP(CMainFrame, CMainFrmBase)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()

	ON_MESSAGE(WM_CONNECTION_FAILED, OnConnectionFailed)

	//	ON_COMMAND(ID_COMMON_CM_REPORTS, OnCommonReports)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame() : CMainFrmBase(IDR_MAINFRAME, IDB_LOGON) {

	ms_bgn_date = "";
	ms_end_date = "";
	
}
CMainFrame::~CMainFrame() {}

BOOL CMainFrame::_CreateUnic()
{
	LoadProperties();

	CRect min_width;
	GetDesktopWindow()->GetWindowRect(&min_width);
	
	//протоколируем вход в задачу
	//================================================
	//CQuery query;
	//query.SetQuery("EXECUTE GSP_PROTOCOL_INPUT_APP");
	//g_lpConn->Execute(query.SQL, adExecuteNoRecords);
	//================================================


	return TRUE;
}

void CMainFrame::OnClose()
{
	SaveProperties();

	CWnd* pWnd = GetFocus();

	if (AfxMessageDlg(_T("Вы действительно хотите закрыть программу?"), MB_YESNO) != IDYES)
	{
		pWnd->SetFocus();
		return;
	}

	/*
	//протоколируем выход из задачи
	CQuery query;
	query.SetQuery("EXECUTE GSP_PROTOCOL_OUTPUT_APP");
	g_lpConn->Execute(query.SQL, adExecuteNoRecords);

	CFrameWnd::OnClose();
	*/
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



LRESULT CMainFrame::OnConnectionFailed(WPARAM wParam, LPARAM lParam)
{
	AfxMessageDlg(_T("Отсутствует связь с БД. Для продолжения\nнеобходимо повторно пройти регистрацию."), MB_ICONERROR);
	g_lpConn->Close();
	//g_lpConn->m_pConn = NULL;
	CLogonDlg dlg("ArmStacDoctor", VERSION_SYS, VERSION);
	CString s; s.LoadString(IDR_MAINFRAME);
	dlg.InitLogon(IDB_LOGON, s);

	dlg.EnableDisplayButtonText(FALSE);
	if (dlg.DoModal() == IDCANCEL)
	{
		if (IDYES == AfxMessageDlg(_T("Завершить работу программы?"), MB_YESNO))
			PostMessage(WM_QUIT);
	}
	else
	{
		CString* str = (CString*)wParam;
		g_FailoverResultSet = g_lpConn->Execute(*str);
		return TRUE;
	}
	return TRUE;
}
