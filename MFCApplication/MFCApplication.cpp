
// MFCApplication.cpp: определяет поведение классов для приложения.
//

#include "stdafx.h"
#include "MFCApplication.h"
#include "MFCApplicationDlg.h"
#include "ADOConnection.h"
#include "MainFrm.h"
#include "DiagServSubsidiaries.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment (lib, "I:/lib/_UNICODE/debug/common_vs2017.lib")
#pragma comment (lib, "I:/lib/_UNICODE/debug/commondb_vs2017.lib")
#pragma comment (lib, "I:/lib/_UNICODE/debug/commonmed_vs2017.lib")
//#pragma comment (lib, "I:/lib/_UNICODE/debug/common.lib")
//#pragma comment (lib, "I:/lib/_UNICODE/debug/commondb.lib")
//#pragma comment (lib, "I:/lib/_UNICODE/debug/commonmed.lib")
// CMFCApplicationApp



BEGIN_MESSAGE_MAP(CMFCApplicationApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()






CMFCApplicationApp::CMFCApplicationApp()
{
	// TODO: добавьте код создания,
	// Размещает весь важный код инициализации в InitInstance
}


// Единственный объект CMFCApplicationApp

CMFCApplicationApp theApp;


// Инициализация CMFCApplicationApp

BOOL CMFCApplicationApp::InitInstance()
{
	/*
	int AppCode = 0;
	CString sAppName;
	UINT idb = 0;


#ifndef _POSTOVAYA
	AppCode = APP_CODE_MOVING;
	sAppName = APP_NAME_MOVING;
	idb = IDB_LOGON;
#else
	AppCode = APP_CODE_MOVING_POST;
	sAppName = APP_NAME_MOVING_POST;
	idb = IDB_POSTOVAYA;
#endif 

	if (!InitInstanceBase(AppCode, sAppName, VERSION_SYS, VERSION, IDR_MAINFRAME, idb))
		return FALSE;

	FillRightAllForUser(g_UserKeyID);


	
	g_HandReceptionTime = GetParamNumber(15012);

	

	InitInstanceMain(new CMainFrame, IDR_MAINFRAME, IDI_ICON_SMALL);
	//проверка при входе в АРМ
	CQuery query;

	CString new_query;
	CString new_query1;

	CString new_command;
	CString command;
	command = _T("sql_CheckMovingErrors");
	new_command = GetSql(L"sql_CheckMovingErrors");

	query.SQL = new_command;

	query.ParamByName(_T("UserID")).AsInteger = g_UserKeyID;
	new_query1 = query.ParamByName(_T("UserID")).AsString;
	new_query = _T("UserID");

	int TAG = LU_TAG_MOVING_CHECK;

	CheckErrorsClose(TAG, new_query1, command, new_query);


	return TRUE;




	//----------
	*/
	CWinApp::InitInstance();





	// Создать диспетчер оболочки, в случае, если диалоговое окно содержит
	// представление дерева оболочки или какие-либо его элементы управления.
	CShellManager *pShellManager = new CShellManager;

	// Активация визуального диспетчера "Классический Windows" для включения элементов управления MFC
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Стандартная инициализация
	// Если эти возможности не используются и необходимо уменьшить размер
	// конечного исполняемого файла, необходимо удалить из следующих
	// конкретных процедур инициализации, которые не требуются
	// Измените раздел реестра, в котором хранятся параметры
	// TODO: следует изменить эту строку на что-нибудь подходящее,
	// например на название организации
	SetRegistryKey(_T("Локальные приложения, созданные с помощью мастера приложений"));

	CLogonDlg dlg(L"CHART", VERSION_SYS, VERSION, L"");
	//CString s; s.LoadString(iIDRMainFrm);
	dlg.InitLogon(IDB_LOGON, L"Карта назначений");

	//dlg.EnableDisplayButtonText(FALSE);
	auto res = dlg.DoModal();
	if (res == IDCANCEL)
		return FALSE;
	else if(res == -1)
	{
		TRACE(traceAppMsg, 0, "Не удалось создать CLogonDlg\n");
	}
	
	CMFCApplicationDlg dlgapp;
	m_pMainWnd = &dlgapp;
	INT_PTR nResponse = dlgapp.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Введите код для обработки закрытия диалогового окна
		//  с помощью кнопки "ОК"
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Введите код для обработки закрытия диалогового окна
		//  с помощью кнопки "Отмена"
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Предупреждение. Не удалось создать диалоговое окно, поэтому работа приложения неожиданно завершена.\n");
		TRACE(traceAppMsg, 0, "Предупреждение. При использовании элементов управления MFC для диалогового окна невозможно #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Удалить диспетчер оболочки, созданный выше.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Поскольку диалоговое окно закрыто, возвратите значение FALSE, чтобы можно было выйти из
	//  приложения вместо запуска генератора сообщений приложения.
	return FALSE;
}

