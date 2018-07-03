
#include "stdafx.h"
#include "DBConnector.h"

#include "Global.h"
#include "ArmVersion.h"

#include "tags.h"
//#include "AppString.h"
#include "param.h"
#include "SqlLogDlg.h"
#include "AboutDlg.h"

#include <vector>
#include <locale>
#include <codecvt>
#include <fstream>

#include "ChartDLLFunction.h"
#include "ParserDrugForm.h"
#include "Tests.h"
#include "AdminWay.h"


class DBResult : public IDBResult
{
	CADOResult rs;
public:
	DBResult(const wstring& query)
	{
		rs = g_lpConn->Execute(query.c_str());
	}
	//--------------------------------------------------------------------
	~DBResult()
	{
		rs.Close();
	}
	//--------------------------------------------------------------------
	virtual void Next()
	{
		rs.Next();
	}
	//--------------------------------------------------------------------
	virtual bool Eof()
	{
		return rs.Eof();
	}
	//--------------------------------------------------------------------
	virtual void GetStrValue(const std::wstring& param, VCopier<wstring>& copier)
	{ 
		copier.push_back(rs.GetStrValue(param.c_str()).GetBuffer()); 
	};
	//--------------------------------------------------------------------
	virtual int GetIntValue(const std::wstring& param)
	{
		return rs.GetIntValue(param.c_str());
	};
	//--------------------------------------------------------------------
	virtual double GetDateValue(const std::wstring& param)
	{
		return rs.GetDateValue(param.c_str());
	};
	//--------------------------------------------------------------------
	virtual double GetFloatValue(const std::wstring& param)
	{
		return rs.GetFloatValue(param.c_str());
	};
	
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void DBConnector::sendQuery(const wstring& query, IDBResultCopier& copier)
{
	try
	{
		DBResult result(query);
		copier.push_back(result);
	}
	//catch (CADOException *pE) { pE->ReportError(); pE->Delete(); }
	catch (...) {
		AfxMessageDlg(_T("Ошибка формирования списка !"), MB_ICONSTOP);
	}
}
//--------------------------------------------------------------------
void DBConnector::getPatientList(double DATETIME, IDBResultCopier& copier)
{
	CMacroQuery query;
	int old_keyid = 0;
	if (IsRightForUser(FORBID_TO_VIEW_PATHISTORY_OTHER_DOCTORS)) // запрет на просмотр других врачей
	{
		query.SQL = GetSql(_T("sql_SelDepDocPats"));
		query.ParamByName(_T("DocID")).AsString = g_DocdepID;
	}
	else
		query.SQL = GetSql(_T("sql_SelDepPats"));

	//COleDateTime m_dCurrDate = COleDateTime::GetCurrentTime();
	query.ParamByName(_T("DepID")).AsString = deptID.c_str();
	query.ParamByName(_T("Dat")).AsDate = DATETIME;

	sendQuery(query.SQL.GetBuffer(), copier);
}



void DBConnector::getChartJSON(const PatientInfo& patient, const StringCopier& data_copier) const

{
	wifstream wif(L"structure_json.txt");
	wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

	wstringstream wss;
	wss << wif.rdbuf();

	wstring patientJSON = patient.getJSONBlock();
	wstring fileJSON_UTF16 = wss.str();
	fileJSON_UTF16.insert(fileJSON_UTF16.begin() + 1, patientJSON.begin(), patientJSON.end());

	data_copier.push_back_data(fileJSON_UTF16);
}
//-----------------------------------------------------------------
void DBConnector::getDrugList(const std::wstring& drug, IDBResultCopier& copier)
{
	//std::wstring request = L"EXECUTE solution_apteka.pkg_select_list.select_prod_name_form_existing\n  ";
	//request += L"'"+ deptID + L"',";//'65'\n,
	//request += L"'2018-05-21 00:00:00'\n, ''\n, '";
	//request += drug;
	//request += L"%'\n, NULL\n, ''\n, ''\n, 0";
	std::wstring query =
		L"SELECT product_name_id as id, UPPER(solution_apteka.product_name.name) as name, LOWER(solution_apteka.lu.text) as lu \
		FROM solution_apteka.lu, solution_apteka.product_form, solution_apteka.product_name \
		WHERE(solution_apteka.product_form.form_lu_id || solution_apteka.product_form.dosage_lu_id) = solution_apteka.lu.id \
		AND solution_apteka.product_form.product_name_id = solution_apteka.product_name.id \
		AND solution_apteka.product_form.product_name_id \
		IN (SELECT id \
			FROM SOLUTION_APTEKA.PRODUCT_NAME \
			WHERE UPPER(NAME) LIKE UPPER('" + drug + L"%'))";

	sendQuery(query, copier);
}
//-----------------------------------------------------------------

//--------------------------------------------------------------------
void DBConnector::getAdminWays(const PairCopier& data_copier) const
{

	// сделать загрузку путей введения из базы данных
	std::map<int, std::wstring> allowedAdminWays
	{
		{ ADMINWAY::INTRAVENOUS_DROPS,		L"внутривенно капельно" },
	{ ADMINWAY::INTRAVENOUS_BOLUS,		L"внутривенно болюсно" },
	{ ADMINWAY::INTRAVENOUS_INFUSION,	L"внутривенно микроструйно" },

	{ ADMINWAY::INTRAMUSCULAR,			L"внутримышечно" },
	{ ADMINWAY::SUBCUTANEOUS,			L"подкожно" },
	{ ADMINWAY::ENTERAL,				L"энтерально" },
	{ ADMINWAY::RECTAL,					L"ректально" },
	{ ADMINWAY::SPINAL,					L"спинальное пространство" },
	{ ADMINWAY::EPIDURAL_BOLUS,			L"эпидурально болюсно" },
	{ ADMINWAY::EPIDURAL_INFUSION,		L"эпидурально микроструйно" },
	{ ADMINWAY::EXTERNAL,				L"наружное применение" },
	{ ADMINWAY::INHALATION,				L"ингаляция" },
	{ ADMINWAY::NASAL,					L"назально" },
	{ ADMINWAY::EYE_DROPS,				L"ушные капли" },
	{ ADMINWAY::EAR_DROPS,				L"глазные капли" }
	};

	for (map<int, wstring>::iterator it = allowedAdminWays.begin();
		it != allowedAdminWays.end(); ++it)

	{
		data_copier.push_back_data(make_pair(it->first, it->second));
	}
}
//--------------------------------------------------------------------
void DBConnector::setAppMenu(CMenu * menu)
{
	if (menu)
	{
		m_CommonMenu.setMainMenu(menu);
		m_CommonMenu.InitializeAppsMenu();
	}
}
//--------------------------------------------------------------------
void DBConnector::executeApp(UINT nID)
{
	m_CommonMenu.ProcessAppCmd(nID);
}
//--------------------------------------------------------------------
void DBConnector::showAboutDlg()
{
	CAboutDlg aboutDlg(IDS_APPNAME, VERSION_SYS, VERSION, IDR_MAINFRAME);
	aboutDlg.DoModal();
}
//--------------------------------------------------------------------
void DBConnector::GetParamBool(int Code, const BoolCopier& data_copier) const
{
	BOOL result = ::GetParamBool(Code);
	data_copier.push_back_data(result);
}
//--------------------------------------------------------------------
void DBConnector::GetParamNumber(int Code, const DoubleCopier& data_copier) const
{
	double result = ::GetParamNumber(Code);
	data_copier.push_back_data(result);
}
//--------------------------------------------------------------------
void DBConnector::GetParamText(int Code, const StringCopier& data_copier) const
{
	wstring result = ::GetParamText(Code).GetBuffer();
	data_copier.push_back_data(result);
}
//--------------------------------------------------------------------
void DBConnector::showLogDialog()
{
	ShowLogDialog();
}
//--------------------------------------------------------------------