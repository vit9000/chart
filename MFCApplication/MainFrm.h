// MainFrm.h : interface of the CMainFrame class
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "MainFrmBase.h"
/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

class CMainFrame : public CMainFrmBase
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	~CMainFrame();
	BOOL _CreateUnic();

	LRESULT CMainFrame::OnConnectionFailed(WPARAM wParam, LPARAM lParam);
};
