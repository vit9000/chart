// ArmAICU.cpp : Defines the class behaviors for the application.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ArmAICU.h"
#include "ArmVersion.h"
#include "DebugWnd.h"
#include "tables\SOLUTION_MED\SOLUTION_MED_DEP.h"
#include "ChartDLLFunction.h"
#include "DBConnector.h"
#include <vector>

CADOConnection * g_lpConn = NULL;
CArmAICU theApp;
/////////////////////////////////////////////////////////////////////////////
// CArmAICU

BEGIN_MESSAGE_MAP(CArmAICU, CWinApp)
END_MESSAGE_MAP()

//---------------------------------------------------------------------------
CArmAICU::CArmAICU() {}
//---------------------------------------------------------------------------
BOOL CArmAICU::InitInstance()
{
	if (!InitInstanceBase(APP_CODE_AICU, APP_NAME_AICU, VERSION_SYS, VERSION, IDR_MAINFRAME, IDB_LOGON))
		return FALSE;

	while (1)
	{
		DeptInfo deptInfo;
		if (!ShowDepList(deptInfo))
			return FALSE;

		DBConnector db_connector(deptInfo.keyID.GetBuffer(), g_UserNameInitials.GetBuffer());

		ChartDLL::function<int(IDBConnector*)> ShowDialog("ShowDialog");
		if (ShowDialog)
		{
			if (ShowDialog(&db_connector) != IDOK)
				break;
		}
	}
	return TRUE;
}
//---------------------------------------------------------------------------
int CArmAICU::ExitInstance()
{
	if (g_lpConn != NULL)
	{
		if (g_lpConn->IsOpened())
			g_lpConn->Close();
		delete g_lpConn;
		g_lpConn = NULL;
	}

	DestroyLogDialog();

	CDebugWnd::Destroy();
	AfxOleTerm(TRUE);
	return CWinApp::ExitInstance();
}
//---------------------------------------------------------------------------
BOOL CArmAICU::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F12)
	{
		CDebugWnd::Show();
		return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_PAUSE)
	{
		ShowLogDialog();
		return TRUE;
	}

	return CWinApp::PreTranslateMessage(pMsg);
}
//---------------------------------------------------------------------------
bool CArmAICU::ShowDepList(DeptInfo& deptInfo)
{
	CString rootStructSortCode = _T("001");
	if (rootStructSortCode == _T("") && !IsRightForUser(RIGHT_TO_ALL_DEP_STRUCTURES))
		rootStructSortCode = _T("-1");

	CQuery query;
	CADOResult rs;

	std::vector<DeptInfo> dept_info_array;
	try
	{
		query.SQL = GetSql(_T("sql_SelStPoDeps"));
		CString str = query.SQL;
		rs = g_lpConn->Execute(query.SQL);
		if (rs != NULL && !rs.Eof())
		{
			int nRow = 1;
			while (!rs.Eof())
			{

				CString checkSortCode = rs.GetStrValue(_T("SortCode"));
				if (checkSortCode.Left(3) != rootStructSortCode && rootStructSortCode != _T(""))
				{
					rs.Next();
					continue;
				}
				BOOL bSetImage = FALSE;

				DeptInfo di;
				di.status = rs.GetStrValue(_T("Status_dep"));
				di.name = rs.GetStrValue(_T("Text"));
				di.code = rs.GetStrValue(_T("Code"));
				di.keyID = rs.GetStrValue(_T("KeyID"));
				dept_info_array.push_back(di);
				
				nRow++;
				
				rs.Next();
			}
		}
		rs.Close();
	}
	catch (CADOException *pE) { 
		pE->ReportError(); pE->Delete();  return false; 
	}


	if (dept_info_array.size() == 0) return false;
	int selected = 0;// если доступно только одно отделение, тогда просто его и передадим
	if (dept_info_array.size() > 1)// если доступен выбор отделений более 1, тогда покажем окно для выбора
	{
		DepDlg dlg;
		dlg.Init(&dept_info_array);
		int res = dlg.DoModal();
		if (res == IDCANCEL)
			return false;
		selected = dlg.getSelected();
		if (selected < 0 || selected >= static_cast<int>(dept_info_array.size()))
			return false;
	}
	deptInfo = dept_info_array.at(selected);
	return true;
}

//-----------------------------------------------------------------






