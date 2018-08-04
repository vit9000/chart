// ArmStacDoctor.cpp : Defines the class behaviors for the application.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ArmChart.h"

#include "Global.h"
#include "ArmVersion.h"
#include "WordArchiever.h"
#include "AppString.h"
#include "tables\SOLUTION_MED\SOLUTION_MED_DEP.h"

#include "ChartDLLFunction.h"
#include "Tests.h"
#include <vector>


CADOConnection * g_lpConn = NULL;
BOOL g_GenerateAddressProtocol = FALSE;
BOOL g_DescProtocolDefault = FALSE;
BOOL g_ByProfDep = TRUE;
//BOOL g_StacDoctorUseStorageWnd = FALSE;
BOOL g_DescProtocolSelectByProfileOnly = TRUE;
extern BOOL g_FormMode;

/////////////////////////////////////////////////////////////////////////////
// CArmStacDoctor

BEGIN_MESSAGE_MAP(CArmChart, CWinApp)
	
END_MESSAGE_MAP()


CArmChart::CArmChart() {}

CArmChart theApp;

BOOL SetDepInfo(const CString & strID, int nType, const CString & strText)
{
	g_DepInfo.KeyID = _ttol(strID);
	g_DepInfo.Type = nType;
	_tcscpy(g_DepInfo.Text, strText);
	return TRUE;
}

BOOL GetDepInfo(CString & strID, int & nType, CString & strText)
{
	strID = IntToStr(g_DepInfo.KeyID);
	nType = g_DepInfo.Type;
	strText = g_DepInfo.Text;
	return TRUE;
}

BOOL SelectGroupDep() {

	if (IsRightForUser(RIGHT_TO_VIEW_ALL_DEPS) || NOT_VALID(g_DepID))
		return TRUE;

	CQuery query;
	query.SQL = GetSql("sql_SelDepAndGroupByDep");
	query.ParamByName("DEPID").AsString = g_DepID;

	int dep_qty = 0;
	CADOResult rs = g_lpConn->Execute(query.SQL);
	if (rs != NULL)
		while (!rs.Eof())
		{
			dep_qty++;
			if (dep_qty>1)
				break;
			rs.Next();
		}
	rs.Close();

	if (dep_qty <= 1) // выбор не нужен
		return FALSE;

	CSelectDlg dlgSelGroup(AfxGetMainWnd());
	dlgSelGroup.SetDlgType(SELECT_DLG_TYPE_SIMPLE);
	dlgSelGroup.SetSqlCmd(query.SQL);
	//	dlgSelGroup.SetAutoChoose(TRUE);
	dlgSelGroup.SetHiddenColCount(2);

	if (dlgSelGroup.DoModal() == IDCANCEL)
		return FALSE;

	CSelectItem * SelectItem = dlgSelGroup.GetSelectItem();
	if (!SelectItem || SelectItem->GetSize() < 4)
	{
		AfxMessageDlg(_T("Ошибка при выборе отделений. Обратитесь к администратору."),
			MB_ICONEXCLAMATION);
		return FALSE;
	}
	int nType = _ttoi(SelectItem->GetAt(1));

	if (!SetDepInfo(SelectItem->GetAt(0), nType, SelectItem->GetAt(4)))
		return FALSE;
	//SaveDepInfo();

	//        szTmp = "Все отделения";
	//		if (0 == strcmp(SelectItem->GetAt(4), szTmp))
	//			nType = ALL_DEPS;      


	return TRUE;
}


BOOL CArmChart::InitInstance()
{
	#ifdef DEBUG
	//	Tests().Execute(); // юнит тесты для парсер формы выпуска препарата
	#endif // DEBUG
	//APP_CODE_STACDOCTOR

	if (!InitInstanceBase(APP_CODE_AICU, APP_NAME_STACDOCTOR, VERSION_SYS, VERSION, IDR_MAINFRAME, IDB_LOGON))
		return FALSE;


	// новый выбор групп по отделению, если параметр работы с группами отделений включен
	if (GetParamBool(11014))
	{
		if (!SelectGroupDep())
		{
			SOLUTION_MED::DEP dep(g_DepID);
			SetDepInfo(g_DepID, DEPARTMENT, dep.TEXT);
		}
		
	}
	else
	{	
		SOLUTION_MED::DEP dep(g_DepID);
		SetDepInfo(g_DepID, DEPARTMENT, dep.TEXT);
	}

	//InitInstanceMain(new CMainFrame, IDR_MAINFRAME, NULL);
	//return TRUE;
	
	


	while (1)
	{
		
		DeptInfo deptInfo;
		if (!ShowDepList(deptInfo))
			return FALSE;
	

		DBConnector db_connector(deptInfo.keyID.GetBuffer());


		ChartDLL::function<int(IDBConnector*)> ShowDialog("ShowDialog");
		if (ShowDialog)
		{

			if (ShowDialog(&db_connector) != IDOK)
				break;
		}
	}
	
	return TRUE;
}

int CArmChart::ExitInstance()
{

	if (g_WordInstances.GetSize() > 0)
	{
		for (int i = g_WordInstances.GetSize() - 1; i >= 0; i--)
		{
			((CWord*)g_WordInstances.GetAt(i)->m_pWord)->EnableMessageHandling(FALSE);
			delete ((CWord*)g_WordInstances.GetAt(i)->m_pWord);
			delete g_WordInstances.GetAt(i);
			g_WordInstances.GetAt(i)->m_pWord = NULL;
			g_WordInstances.RemoveAt(i);
		}
	}

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

BOOL CArmChart::PreTranslateMessage(MSG* pMsg)
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


bool CArmChart::ShowDepList(DeptInfo& deptInfo)
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
	m_DepID = deptInfo.keyID;
	
	return true;
}

//-----------------------------------------------------------------






