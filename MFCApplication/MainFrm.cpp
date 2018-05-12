// MainFrm.cpp : implementation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MainFrm.h"
#include "resource.h"



IMPLEMENT_DYNAMIC(CMainFrame, CMainFrmBase)



CMainFrame::CMainFrame() : CMainFrmBase(IDR_MAINFRAME, IDB_LOGON)
{

}

CMainFrame::~CMainFrame() {}

BOOL CMainFrame::_CreateUnic()
{


	return TRUE;
}
