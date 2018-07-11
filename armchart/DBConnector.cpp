
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
		if (rs == NULL) return false;
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
void DBConnector::sendQuery(const wstring& query_name, const vector<QueryParameter>& params, IDBResultCopier& copier)
{
	try
	{
		CMacroQuery query;
		query.SQL = GetSql(query_name.c_str());
		for (size_t i = 0; i < params.size(); i++)
		{
			query.ParamByName(params[i].getName().c_str()).AsString = params[i].get().c_str();
		}
		wstring request = query.SQL.GetBuffer();
		DBResult result(request);
		copier.push_back(result);
	}
	//catch (CADOException *pE) { pE->ReportError(); pE->Delete(); }
	catch (...) {
		AfxMessageDlg(_T("Ошибка формирования списка !"), MB_ICONSTOP);
	}
}
//--------------------------------------------------------------------
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
void DBConnector::createNewChart(int time_type, double date, const wstring& visit_id)
{
	try
	{
		COleDateTime enddate = (COleDateTime)date;
		if (time_type == TIME_TYPE::ICU_CHART)
			enddate += COleDateTimeSpan(1, 0, 0, 0);
		else if (time_type = TIME_TYPE::ANESTH_CHART)
			enddate += COleDateTimeSpan(0, 3, 0, 0);
		else
		{
			MessageBox(0, L"time_type указан не верно", L"Ошибка", MB_ICONERROR | MB_OK);
			return;
		}
		

		// CREATING NEW CHART
		CMacroQuery query;
		//query.SQL = GetSql(L"sql_NewChart_Add");
		query.SQL = GetSql(L"sql_NewChart");
		query.ParamByName(L"TIME_TYPE").AsInteger = time_type;
		query.ParamByName(L"VISIT_ID").AsString = visit_id.c_str();
		query.ParamByName(L"BGNDAT").AsDate = date;
		query.ParamByName(L"ENDDAT").AsDate = date;
		CADOResult rs = g_lpConn->Execute(query.SQL);
		rs.Close();
	}
	//catch (CADOException *pE) { pE->ReportError(); pE->Delete(); }
	catch (...) {
		AfxMessageDlg(_T("Ошибка формирования списка !"), MB_ICONSTOP);
	}
}

int DBConnector::countCharts(int time_type, double date, const wstring& visit_id)
{
	try
	{
		COleDateTime enddate = (COleDateTime)date;
		if (time_type == TIME_TYPE::ICU_CHART)
			enddate += COleDateTimeSpan(1, 0, 0, 0);
		else if (time_type = TIME_TYPE::ANESTH_CHART)
			enddate += COleDateTimeSpan(0, 3, 0, 0);
		else
		{
			return 0;
		}

		CMacroQuery query;
		query.SQL = GetSql(L"sql_CountCharts");
		query.ParamByName(L"TIME_TYPE").AsInteger = time_type;
		query.ParamByName(L"VISIT_ID").AsString = visit_id.c_str();
		query.ParamByName(L"DAT").AsDate = date;
		CADOResult rs = g_lpConn->Execute(query.SQL);
		
		if (rs != NULL && !rs.Eof())
		{
			return rs.GetIntValue(L"COUNTCHARTS");
		}
		return 0;
		
	}
	//catch (CADOException *pE) { pE->ReportError(); pE->Delete(); }
	catch (...) {
		AfxMessageDlg(_T("Ошибка формирования списка !"), MB_ICONSTOP);
	}
	return 0;
}
