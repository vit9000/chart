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
#include <locale>
#include <codecvt>
#include <fstream>
#include "ChartDLLFunction.h"
#include "ParserDrugForm.h"
#include "Tests.h"
#include "AdminWay.h"


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


	if (!InitInstanceBase(APP_CODE_STACDOCTOR, APP_NAME_STACDOCTOR, VERSION_SYS, VERSION, IDR_MAINFRAME, IDB_LOGON))
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
	
	
	DeptInfo deptInfo;
	if (!ShowDepList(deptInfo))
		return FALSE;

	
	DBConnector db_connector(deptInfo.keyID.GetBuffer());
	

	//while (1)
	{

		ChartDLL::function<void(IDBConnector*)> ShowDialog("ShowDialog");
		if (ShowDialog)
			ShowDialog(&db_connector);
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
	m_DepID = deptInfo.keyID;
	
	return true;
}

//-----------------------------------------------------------------

void DBConnector::getChartJSON(const PatientInfo& patient) const

{
	wifstream wif(L"c:/ariadna/app/structure_json.txt");
	wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

	wstringstream wss;
	wss << wif.rdbuf();

	wstring patientJSON = patient.getJSONBlock();
	wstring fileJSON_UTF16 = wss.str();
	fileJSON_UTF16.insert(fileJSON_UTF16.begin() + 1, patientJSON.begin(), patientJSON.end());

	copier->push_back_data(&fileJSON_UTF16);
}

//-----------------------------------------------------------------
void DBConnector::getDrugList(const std::wstring& drug) const
{
	//std::wstring request = L"EXECUTE solution_apteka.pkg_select_list.select_prod_name_form_existing\n  ";
	//request += L"'"+ deptID + L"',";//'65'\n,
	//request += L"'2018-05-21 00:00:00'\n, ''\n, '";
	//request += drug;
	//request += L"%'\n, NULL\n, ''\n, ''\n, 0";
	/*
	SELECT text 
FROM SOLUTION_APTEKA.LU
WHERE ID IN
(

SELECT (form_lu_id || dosage_lu_id) as lu
            FROM SOLUTION_APTEKA.PRODUCT_FORM 
            WHERE (form_lu_id IS NOT NULL OR dosage_lu_id IS NOT NULL)
                                 AND product_name_id IN                    
                                  (SELECT id, name
                                  FROM SOLUTION_APTEKA.PRODUCT_NAME 
                                  WHERE NAME LIKE 'Кето%' )
)
	*/
	
	std::wstring query = 
		L"SELECT product_name_id as id, UPPER(SOLUTION_APTEKA.PRODUCT_NAME.NAME) as name, LOWER(solution_apteka.lu.text) as lu \
		FROM solution_apteka.lu, solution_apteka.product_form, SOLUTION_APTEKA.PRODUCT_NAME \
		WHERE(solution_apteka.product_form.form_lu_id || solution_apteka.product_form.dosage_lu_id) = solution_apteka.lu.id \
		AND solution_apteka.product_form.product_name_id = SOLUTION_APTEKA.PRODUCT_NAME.ID \
		AND solution_apteka.product_form.product_name_id \
		IN (SELECT id \
			FROM SOLUTION_APTEKA.PRODUCT_NAME \
			WHERE UPPER(NAME) LIKE UPPER('" + drug + L"%'))";
	
	try {
		CADOResult rs = g_lpConn->Execute(query.c_str());
		while (!rs.Eof()) 
		{
			int count = rs.GetColCount();
			CString name = rs.GetStrValue(L"NAME");
			int id = rs.GetIntValue(L"ID");
			CString lu = rs.GetStrValue(L"LU");
			copier->push_back_data(&ParserDrugFrom(id, name.GetBuffer(), lu.GetBuffer()));

			rs.Next();
		}
		rs.Close();
	}
	catch (...) {
		AfxMessageDlg(_T("Ошибка формирования списка !"), MB_ICONSTOP);
	}
}
//-----------------------------------------------------------------


void DBConnector::getPatientList() const
{
	CMacroQuery query;
	CADOResult rs;

	int old_keyid = 0;
	//std::vector<PatientInfo> patients;
	try
	{
		if (IsRightForUser(FORBID_TO_VIEW_PATHISTORY_OTHER_DOCTORS)) // запрет на просмотр других врачей
		{
			query.SQL = GetSql(_T("sql_SelDepDocPats"));
			query.ParamByName(_T("DocID")).AsString = g_DocdepID;
		}
		else
			query.SQL = GetSql(_T("sql_SelDepPats"));

		COleDateTime m_dCurrDate;
		m_dCurrDate = COleDateTime::GetCurrentTime();
		query.ParamByName(_T("DepID")).AsString = deptID.c_str();
		query.ParamByName(_T("Dat")).AsDate = m_dCurrDate;


		rs = g_lpConn->Execute(query.SQL);
		if (rs!=NULL)// && !rs.Eof())
		{
			while (!rs.Eof())
			{
				PatientInfo pi
				(
					rs.GetStrValue(_T("Fio")).GetBuffer(),
					rs.GetStrValue(_T("Age")).GetBuffer(),
					rs.GetStrValue(_T("Num")).GetBuffer(),
					rs.GetStrValue(_T("st_num")).GetBuffer(),
					rs.GetStrValue(_T("Agr")).GetBuffer(),
					rs.GetStrValue(_T("dep_prof")).GetBuffer(),
					rs.GetStrValue(_T("diagnos")).GetBuffer(),
					rs.GetStrValue(_T("doctor")).GetBuffer()
				);
				copier->push_back_data(&pi);
				rs.Next();
			}
		}
		rs.Close();
	}
	catch (CADOException *pE) { pE->ReportError(); pE->Delete(); }
	
}


void DBConnector::getAdminWays(const PairCopier& data_copier) const
{
	
	// сделать загрузку путей введения из базы данных
	std::map<int, std::wstring> allowedAdminWays 
	{ 
	{ ADMINWAY::INTRAVENOUS_DROPS,		L"внутривенно капельно"		},
	{ ADMINWAY::INTRAVENOUS_BOLUS,		L"внутривенно болюсно"		},
	{ ADMINWAY::INTRAVENOUS_INFUSION,	L"внутривенно микроструйно" },

	{ ADMINWAY::INTRAMUSCULAR,			L"внутримышечно" },
	{ ADMINWAY::SUBCUTANEOUS,			L"подкожно"		 },
	{ ADMINWAY::ENTERAL,				L"энтерально"				 },
	{ ADMINWAY::RECTAL,					L"ректально"				 },
	{ ADMINWAY::SPINAL,					L"спинальное пространство" },
	{ ADMINWAY::EPIDURAL_BOLUS,			L"эпидурально болюсно"		 },
	{ ADMINWAY::EPIDURAL_INFUSION,		L"эпидурально микроструйно"	 },
	{ ADMINWAY::EXTERNAL,				L"наружное применение"		 },
	{ ADMINWAY::INHALATION,				L"ингаляция"					 },
	{ ADMINWAY::NASAL,					L"назально"					 },
	{ ADMINWAY::EYE_DROPS,				L"ушные капли"			 },
	{ ADMINWAY::EAR_DROPS,				L"глазные капли"				 }
	};
	
	/*for (const auto& it : allowedAdminWays)

	{
		pair<int, wstring> p = make_pair(it.first, it.second);
		data_copier->push_back_data(&p);
	}*/
	for (map<int, wstring>::iterator it = allowedAdminWays.begin();
		it != allowedAdminWays.end(); ++it)

	{
		data_copier.push_back_data(make_pair(it->first, it->second));
	}
}


void DBConnector::setAppMenu(CMenu * menu)
{
	if (menu)
	{
		m_CommonMenu.setMainMenu(menu);
		m_CommonMenu.InitializeAppsMenu();
	}
}

void DBConnector::executeApp(UINT nID)
{
	m_CommonMenu.ProcessAppCmd(nID);
}

void DBConnector::showAboutDlg()
{
	CAboutDlg aboutDlg(IDS_APPNAME, VERSION_SYS, VERSION, IDR_MAINFRAME);
	aboutDlg.DoModal();
}

