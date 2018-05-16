// ArmMoving.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ArmChart.h"
#include "ArmVersion.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL g_PerinatalMode		= FALSE;  // не проверять привязку стац. услуг периода госпитализации к визитам госпитализации
/////////////////////////////////////////////////////////////////////////////
// CArmMoving

BEGIN_MESSAGE_MAP(CArmChart, CWinApp)
	//{{AFX_MSG_MAP(CArmMoving)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CArmChart::CArmChart(){}

CArmChart theApp;

void SaveDepInfo()
{
	CString sub_key;
	sub_key.Format(_T("%s\\DepInfo"), GetRegKeyName());
    SetRegValue(sub_key, "dep_info", &g_DepInfo, sizeof(g_DepInfo));
}

void LoadDepInfo()
{
	CString sub_key;
	sub_key.Format(_T("%s\\DepInfo"), GetRegKeyName());
	DWORD size = sizeof(g_DepInfo);
	GetRegValue(sub_key, "dep_info", &g_DepInfo, &size);
	if (size != sizeof(g_DepInfo) || _tcslen(g_DepInfo.Text) == 0 || g_DepInfo.KeyID == 0) {
		g_DepInfo.KeyID = -1;
		g_DepInfo.Type = -1;
		_tcscpy(g_DepInfo.Text, _T(""));
	}
}

BOOL IsGZ(const ID_TYPE & i)
{
    return !i.IsEmpty() && -1 == i.Find('-') && i != "0";
}

//упаковываем данные в строку
BOOL SetDepInfo(const CString & strID, int nType, const CString & strText)
{	   
	g_DepInfo.KeyID = _ttoi(strID);
	g_DepInfo.Type = nType;
	_tcscpy(g_DepInfo.Text,strText);
	return TRUE;
   /* int nTmp;
	LPSTR szTmp = g_DepInfo.Text;
    int nSizeBuf = sizeof(g_DepInfo.Text)/sizeof (g_DepInfo.Text[0]);
        
    nTmp  = strID.GetLength();                           if (nSizeBuf <= 0 || MAXSIZEKEYID > nSizeBuf){AfxMessageDlg("34802: Размер буффера не соответствует.");return FALSE;}
    szTmp = g_DepInfo.Text;                              if (!szTmp) {AfxMessageDlg("34803: Буффер пуст."); return FALSE;}
    strcpy(szTmp, strID);
    szTmp += NUMBORDTYPE;
    *szTmp = nType;
    ++szTmp;
    nTmp  = strText.GetLength();                         if ( nTmp <= 0 || nTmp >= nSizeBuf - NUMBORDTYPE - 2) {AfxMessageDlg("34804: Размер буффера не соответствует."); return FALSE;}
    strcpy(szTmp, strText);
    return TRUE;*/
}

BOOL GetDepInfo(CString & strID, int & nType, CString & strText)
{	  
	strID = IntToStr(g_DepInfo.KeyID);
	nType = g_DepInfo.Type;
	strText = g_DepInfo.Text;
	return TRUE;
}



BOOL SelectGroupDep() {
	
	/*if(IsRightForUser(RIGHT_TO_VIEW_ALL_DEPS))
		return FALSE;

	if(NOT_VALID(g_DepID))
		return TRUE;
	
	CQuery query;
	query.SQL = GetSql("sql_SelDepAndGroupByDep");
	query.ParamByName("DEPID").AsString = g_DepID;
	
	int dep_qty = 0;
    CADOResult rs = g_lpConn->Execute(query.SQL);
	if(rs != NULL)
		while (!rs.Eof())
		{
			dep_qty++;
			if(dep_qty>1)
				break;
			rs.Next();
		}
		rs.Close();   
		
		if(dep_qty <= 1) // выбор не нужен
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
			AfxMessageDlg( _T("Ошибка при выборе отделений. Обратитесь к администратору."),
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
		*/
		
		return TRUE;
}

BOOL CArmChart::InitInstance()
{
	int AppCode = 0;
	CString sAppName;
	UINT idb = 0;


#ifndef _POSTOVAYA
	AppCode		= APP_CODE_MOVING;
	sAppName	= APP_NAME_MOVING;
	idb			= IDB_LOGON;
#else
	AppCode		= APP_CODE_MOVING_POST;
	sAppName	= APP_NAME_MOVING_POST;
	idb			= IDB_POSTOVAYA;
#endif 

	if(!InitInstanceBase(AppCode,sAppName,VERSION_SYS,VERSION,IDR_MAINFRAME,idb))
		return FALSE;

	/*FillRightAllForUser(g_UserKeyID);


	// временно оставляю обработку неактивного параметра 11040 для поддержки старых настроек, т.к. в последствии переводим на права
	g_PerinatalMode		=	(IsRightForUser(RIGHT_MOVING_SHOW_PERINATAL) || GetParamBool(11040));  // Ведение родов в режиме "Перевод пациентов"

	g_HandReceptionTime	=	GetParamNumber(15012);

	// новый выбор групп по отделению, если параметр работы с группами отделений включен
	if(GetParamBool(11014))
	{
		if(!SelectGroupDep())
		{
			SOLUTION_MED::DEP dep(g_DepID);
			SetDepInfo(g_DepID, DEPARTMENT, dep.TEXT);
		}
	}
	else
	{
		SOLUTION_MED::DEP dep(g_DepID);
		SetDepInfo(g_DepID, DEPARTMENT, dep.TEXT);		
	}*/

//nitInstanceMain(new CMainFrame,IDR_MAINFRAME,IDI_ICON_SMALL);
	//проверка при входе в АРМ
	CQuery query;

	CString new_query;
	CString new_query1;
	
	CString new_command;
	CString command;
	command = _T("sql_CheckMovingErrors");
	new_command = GetSql("sql_CheckMovingErrors");
		
	query.SQL = new_command;
	
	query.ParamByName(_T("UserID")).AsInteger = g_UserKeyID;
	new_query1 = query.ParamByName(_T("UserID")).AsString;
	new_query = _T("UserID");
	
	int TAG = LU_TAG_MOVING_CHECK;
	
	//CheckErrorsClose(TAG,new_query1,command, new_query);
	

	return TRUE;
}

int CArmChart::ExitInstance()
{
	DestroyLogDialog();
	if (g_lpConn != NULL)
	{
		if (g_lpConn->IsOpened())
			g_lpConn->Close();
		delete g_lpConn;
		g_lpConn = NULL;
	}

	CDebugWnd::Destroy();
	AfxOleTerm(TRUE);
	return CWinApp::ExitInstance();
}

BOOL CArmChart::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN &&	pMsg->wParam == VK_PAUSE)
	{
		ShowLogDialog();
		return TRUE;
	}
	
	return CWinApp::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CArmMoving message handlers

void CArmChart::OnAppAbout()
{
	

}


