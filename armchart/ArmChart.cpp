// ArmStacDoctor.cpp : Defines the class behaviors for the application.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ArmChart.h"
//#include "MainFrm.h"
#include "Global.h"
#include "Splash.h"
#include "SQL.h"
#include "tags.h"
#include "AppString.h"
#include "LogonDlg.h"
#include "ArmVersion.h"
#include "SelectDepDlg.h"
#include "param.h"
#include "OperPersonalInfoWnd.h"
#include "SqlLogDlg.h"
#include "Query.h"
#include "LabDirectInfoWnd2.h"
#include "HistoryPharmaScheduleDlg.h"
#include "WordArchiever.h"
#include "SelectDiagnosDlg.h"
#include "AboutDlg.h"
#include "tables\SOLUTION_MED\SOLUTION_MED_DEP.h"

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
	//{{AFX_MSG_MAP(CArmStacDoctor)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
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
	if (!InitInstanceBase(APP_CODE_STACDOCTOR, APP_NAME_STACDOCTOR, VERSION_SYS, VERSION, IDR_MAINFRAME, IDB_LOGON))
		return FALSE;


	g_DescProtocolDefault = GetParamBool(303050);
	g_GenerateAddressProtocol = GetParamBool(303002);
	g_StacDoctorUseStorageWnd = GetParamBool(303008);
	g_FormMode = GetParamBool(303003);
	g_DescProtocolSelectByProfileOnly = GetParamBool(303004);
	g_DescProtocolOnly = GetParamBool(303005);
	g_UseSOATOProtocol = GetParamBool(303013);

	g_OperCardAnestHide = GetParamBool(801);
	g_OperCardDiagnosHide = GetParamBool(802);
	g_OperCardMacroHide = GetParamBool(803);
	g_OperCardOperDoctorTimeHide = GetParamBool(804);
	g_OperCardComplexShow = GetParamBool(809);  // показывать осложнения
	g_ByProfDep = GetParamBool(810);
	g_UseInternalDiagnoses = GetParamBool(303007);
	g_ShowDiagnosMES = GetParamBool(303063);
	g_OperCardNoteShow = GetParamBool(303058);

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

	DeptInfo deptInfo;
	ShowDepList(deptInfo);


	FillPatGrid(deptInfo.keyID);

	//InitInstanceMain(new CMainFrame, IDR_MAINFRAME, IDI_ICON_SMALL);
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

/////////////////////////////////////////////////////////////////////////////
// CArmStacDoctor message handlers

void CArmChart::OnAppAbout()
{
	CAboutDlg aboutDlg(IDR_MAINFRAME, VERSION_SYS, VERSION, IDR_MAINFRAME);
	aboutDlg.DoModal();
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

				dept_info_array.push_back({
					rs.GetValue(_T("Status_dep")),
					rs.GetValue(_T("Text")),
					rs.GetValue(_T("Code")),
					rs.GetStrValue(_T("KeyID"))
					});
				

				nRow++;
				rs.Next();
			}
		}
		rs.Close();
	}
	catch (CADOException *pE) { pE->ReportError(); pE->Delete();  return false; }


	if (dept_info_array.size() == 0) return false;
	int selected = 0;// если доступно только одно отделение, тогда просто его и передадим
	if (dept_info_array.size() > 1)// если доступен выбор отделений более 1, тогда покажем окно для выбора
	{
		DepDlg dlg;
		dlg.Init(&dept_info_array);
		auto res = dlg.DoModal();
		if (res == IDCANCEL)
			return false;
		selected = dlg.getSelected();
		if (selected < 0 || selected >= static_cast<int>(dept_info_array.size()))
			return false;
	}
	deptInfo = std::move(dept_info_array.at(selected));
	
	return true;
}

void CArmChart::FillPatGrid(const CString& m_DepID)
{
	
	CMacroQuery query;
	CADOResult rs;

	//if (NOT_VALID(m_DepID))
	//	m_DepID = g_DepID;

	int old_keyid = 0;

	try {
		if (IsRightForUser(FORBID_TO_VIEW_PATHISTORY_OTHER_DOCTORS)) // запрет на просмотр других врачей
		{
			query.SQL = GetSql (_T("sql_SelDepDocPats"));
			query.ParamByName(_T("DocID")).AsString = g_DocdepID;
		}
			else
				query.SQL = GetSql(_T("sql_SelDepPats"));
		
		COleDateTime m_dCurrDate;
		m_dCurrDate = COleDateTime::GetCurrentTime();
		query.ParamByName(_T("DepID")).AsString = m_DepID;
		query.ParamByName(_T("Dat")).AsDate = m_dCurrDate;

		std::vector<CString> names;
		rs = g_lpConn->Execute(query.SQL);
		if (rs != NULL && !rs.Eof()) {
			int nRow = 1;
			while (!rs.Eof()) {
				BOOL bSetImage = FALSE;

				names.push_back(rs.GetStrValue(_T("Fio")));
				/*int const& colnum = rs.GetIntValue(_T("colnum"));
				FuncForPaintGrid(&grd.Cell(nRow, PATCOLUMN_PLAN_DAT).Color, rs.GetStrValue(_T("rgb")));
				FuncForPaintGrid(&grd.Cell(nRow, colnum == 0 ? PATCOLUMN_FIO : colnum).Color, rs.GetStrValue(_T("rgb1")));
				FuncForPaintGrid(&grd.Cell(nRow, PATCOLUMN_NUM).Color, rs.GetStrValue(_T("rgb2")));

				grd.CellImage[nRow][PATCOLUMN_INFO] = 2;
				grd.CellAsInteger[nRow][PATCOLUMN_INFO] = IDB_COMMON_INFO_BMP;

				grd.CellText[nRow][PATCOLUMN_FIO] = rs.GetStrValue(_T("Fio"));
				grd.CellText[nRow][PATCOLUMN_AGE] = rs.GetStrValue(_T("Age"));
				grd.CellText[nRow][PATCOLUMN_NUM] = rs.GetStrValue(_T("Num"));
				grd.CellText[nRow][PATCOLUMN_TYPE] = rs.GetStrValue(_T("Agr"));
				grd.CellText[nRow][PATCOLUMN_ST_NUM] = rs.GetStrValue(_T("st_num"));
				grd.CellText[nRow][PATCOLUMN_CATEG] = rs.GetStrValue(_T("bcateg"));
				grd.CellText[nRow][PATCOLUMN_PROF] = rs.GetStrValue(_T("prof"));
				grd.CellText[nRow][PATCOLUMN_BED] = rs.GetStrValue(_T("bed"));
				grd.CellText[nRow][PATCOLUMN_DEP_FROM] = rs.GetStrValue(_T("from_dep"));
				grd.CellText[nRow][PATCOLUMN_DEP_TO] = rs.GetStrValue(_T("to_dep"));
				grd.CellText[nRow][PATCOLUMN_DEP_PROF] = rs.GetStrValue(_T("dep_prof"));
				grd.CellText[nRow][PATCOLUMN_COMPLEX] = rs.GetStrValue(_T("complexlu"));
				grd.CellText[nRow][PATCOLUMN_DOCTOR] = rs.GetStrValue(_T("doctor"));
				grd.CellText[nRow][PATCOLUMN_DIAG] = rs.GetStrValue(_T("diagnos"));

				grd.CellText[nRow][PATCOLUMN_IF] = rs.GetStrValue(_T("FinList"));
				grd.CellAsInteger[nRow][LAST_STATUS] = rs.GetIntValue(_T("Status"));
				grd.CellText[nRow][PATCOLUMN_PATKEYID] = rs.GetStrValue(_T("PatKeyID"));
				grd.CellText[nRow][PATCOLUMN_VISKEYID] = rs.GetStrValue(_T("VisKeyID"));
				grd.CellText[nRow][PATCOLUMN_ROOTKEYID] = rs.GetStrValue(_T("RootKeyID"));
				grd.CellText[nRow][PATCOLUMN_DEP1ID] = rs.GetStrValue(_T("Dep1ID"));
				grd.CellText[nRow][PATCOLUMN_PROFDEPKEYID] = rs.GetStrValue(_T("prof_dep_id"));

				grd.CellText[nRow][PATCOLUMN_BEDKEYID] = rs.GetStrValue(_T("BedID"));
				grd.CellText[nRow][PATCOLUMN_PROFKEYID] = rs.GetStrValue(_T("ProfID"));
				grd.CellText[nRow][PATCOLUMN_BCATEGKEYID] = rs.GetStrValue(_T("BcategID"));
				grd.CellText[nRow][PATCOLUMN_DOCTORID] = rs.GetStrValue(_T("DoctorID"));
				grd.CellText[nRow][PATCOLUMN_BCOMPLEXLUID] = rs.GetStrValue(_T("COMPLEXLU_ID"));
				grd.CellAsDate[nRow][PATCOLUMN_DAT_FROM] = rs.GetDateValue(_T("dat")) == NULL ? NULL_DATE : rs.GetDateValue(_T("dat"));
				grd.CellAsDate[nRow][PATCOLUMN_DAT_TO] = rs.GetDateValue(_T("dat1")) == NULL ? NULL_DATE : rs.GetDateValue(_T("dat1"));
				grd.CellAsInteger[nRow][PATCOLUMN_VISTYPE] = rs.GetIntValue(_T("VisitType"));

				grd.CellText[nRow][PATCOLUMN_DEP_PROF_NAME] = rs.GetStrValue(_T("dep_prof_name"));

				if (m_StatusDep == _T("101") || m_StatusDep == _T("100"))
					grd.CellAsInteger[nRow][PATCOLUMN_STATUSDEP] = rs.GetIntValue(_T("dep_out_status"));
				else
					grd.CellAsInteger[nRow][PATCOLUMN_STATUSDEP] = rs.GetIntValue(_T("status_dep"));

				grd.CellImage[nRow][PATCOLUMN_CLOSESTATUS] = 2;
				grd.CellAsInteger[nRow][PATCOLUMN_CLOSESTATUS] = ClosedStatusToBitmap(rs.GetIntValue("CloseStatus"));
				grd.CellAsDate[nRow][PATCOLUMN_PLAN_DAT] = rs.GetDateValue(_T("PlanDat")) == NULL ? NULL_DATE : rs.GetDateValue(_T("PlanDat"));

				if (m_Mode == MODE_PO)
					ColorRowsPO(nRow);
				else
					ColorRows(nRow);
				*/
				nRow++;
				rs.Next();
			}
		}
		rs.Close();
	}
	catch (CADOException *pE) { pE->ReportError(); pE->Delete(); }

	
}
