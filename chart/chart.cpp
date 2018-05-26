#include "stdafx.h"
#include "chart.h"
#include "MainDlg.h"
#include "DatabaseLoader.h"

#pragma comment( lib, "gdiplus.lib" )

//callback = nullptr;




void ShowDialog(IDBConnector * db_connector)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	GdiPlusInitializer gdiplus; // инициализация GdiPlus, в деструкторе Gdiplus::GdiplusShutdown
	
	DatabaseLoader::getInstance().setDBConnector(db_connector);

	CMainDlg dlg;
	dlg.DoModal();//запускаем приложение
}
//-------------------------------------------------------------------------------------------------------
void GetHBITMAP(HBITMAP *hbitmap, const wchar_t* fileJSON_UTF16)
{
	GdiPlusInitializer gdiplus;// инициализация GdiPlus, в деструкторе Gdiplus::GdiplusShutdown
	CChartView chartView (true);
	//chartView.getModel()->setPatient(0, fileJSON_UTF16);
	chartView.PrintAll(hbitmap);
}
//-------------------------------------------------------------------------------------------------------
