// chart.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CChartApp:
// See chart.cpp for the implementation of this class
//

class CChartApp : public CWinApp
{
public:
	CChartApp();

// Overrides
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
private:
	ULONG_PTR gdiplusToken;
// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CChartApp theApp;