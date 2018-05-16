// Global.cpp : implementation file
//

#include "stdafx.h"
#include "Global.h"
#include "AppString.h"
#include <atlbase.h>

//////////////////////////////////////////////////////
//  global variables

CString g_def_dep_out;

//CString gDayChangeTime = "";
CADOConnection* g_lpConn = NULL;

//struct DepInfo g_DepInfo = {0};

//#define _FORMED

//////////////////////////////////////////////////////
//  global functions

void CheckStrLength(CString& str)
{
	if (str.GetLength() == 1) str = "0" + str;
}

CString DateToStr(DATE dt)
{
	COleDateTime oleDate = dt;
	CString strDate = oleDate.Format(_T("%Y-%m-%d"));
	return strDate;
}

CString OleDateTimeToStr(COleDateTime& dt)
{
	CString strDate = dt.Format(_T("%Y-%m-%d %H:%M:%S"));
	return strDate;
}

CString GetDayChangeTime() { return GetParamText(PARAM_BGN_TIME); };
CString GetStacOutTime() { return GetParamText(103); }; //время выписки из стационара
