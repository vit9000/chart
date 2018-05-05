#include "stdafx.h"
#include "chart.h"
#include "MainDlg.h"

#pragma comment( lib, "gdiplus.lib" )

void ShowDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	GdiPlusInitializer gdiplus; // инициализация GdiPlus, в деструкторе Gdiplus::GdiplusShutdown
	
	CMainDlg dlg;
	dlg.DoModal();//запускаем приложение
}
//-------------------------------------------------------------------------------------------------------
void GetHBITMAP(HBITMAP *hbitmap)
{
	GdiPlusInitializer gdiplus;// инициализация GdiPlus, в деструкторе Gdiplus::GdiplusShutdown
	CChartView chartView (true);
	chartView.getModel()->setPatient(0);
	chartView.PrintAll(hbitmap);
}
//-------------------------------------------------------------------------------------------------------
