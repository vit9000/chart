
// MFCApplication.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif
#include "resource.h"		// основные символы   
#include "WinAppBase.h"
#include "LogonDlg.h"
//#include "MainFrm.h"


// CMFCApplicationApp:
// Сведения о реализации этого класса: MFCApplication.cpp
//

class CMFCApplicationApp : public CWinAppBase//CWinApp
{
public:
	CMFCApplicationApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CMFCApplicationApp theApp;
