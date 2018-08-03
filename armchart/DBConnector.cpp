
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
	DBResult(const VString& query)
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
	virtual void GetStrValue(const VString& param, VCopier<VString>& copier)
	{ 
		copier.push_back(rs.GetStrValue(param.c_str()).GetBuffer()); 
	};
	//--------------------------------------------------------------------
	virtual int GetIntValue(const VString& param)
	{
		return rs.GetIntValue(param.c_str());
	};
	//--------------------------------------------------------------------
	virtual double GetDateValue(const VString& param)
	{
		return rs.GetDateValue(param.c_str());
	};
	//--------------------------------------------------------------------
	virtual double GetFloatValue(const VString& param)
	{
		return rs.GetFloatValue(param.c_str());
	};
	
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void DBConnector::sendQuery(const VString& query_name, const QueryParameters& params, IDBResultCopier& copier)
{
	try
	{
		CMacroQuery query;
		query.SQL = GetSql(query_name.c_str());
		for (int i = 0; i < params.size(); i++)
		{
			query.ParamByName(params[i].getName().c_str()).AsString = params[i].get().c_str();
		}
		VString request = query.SQL.GetBuffer();
		DBResult result(request);
		copier.push_back(result);
	}
	//catch (CADOException *pE) { pE->ReportError(); pE->Delete(); }
	catch (...) {
		AfxMessageDlg(_T("������ ������������ ������ !"), MB_ICONSTOP);
	}
}
//--------------------------------------------------------------------
void DBConnector::sendQuery(const VString& query, IDBResultCopier& copier)
{
	try
	{
		DBResult result(query);
		copier.push_back(result);
	}
	//catch (CADOException *pE) { pE->ReportError(); pE->Delete(); }
	catch (...) {
		AfxMessageDlg(_T("������ ������������ ������ !"), MB_ICONSTOP);
	}
}
//--------------------------------------------------------------------
void DBConnector::getPatientList(double DATETIME, IDBResultCopier& copier)
{
	CMacroQuery query;
	int old_keyid = 0;
	if (IsRightForUser(FORBID_TO_VIEW_PATHISTORY_OTHER_DOCTORS)) // ������ �� �������� ������ ������
	{
		query.SQL = GetSql(_T("sql_SelDepDocPats"));
		query.ParamByName(_T("DocID")).AsString = g_DocdepID;
	}
	else
		query.SQL = GetSql(_T("sql_SelDepPats"));

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
	VString result = ::GetParamText(Code).GetBuffer();
	data_copier.push_back_data(result);
}
//--------------------------------------------------------------------
void DBConnector::showLogDialog()
{
	ShowLogDialog();
}
//--------------------------------------------------------------------
