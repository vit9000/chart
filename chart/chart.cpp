#include "stdafx.h"
#include "chart.h"
#include "MainDlg.h"
#include "MainBridge.h"

#pragma comment( lib, "gdiplus.lib" )

//callback = nullptr;

bool chart_debug = false;


int ShowDialog(IDBConnector * db_connector)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	GdiPlusInitializer gdiplus; // ������������� GdiPlus, � ����������� Gdiplus::GdiplusShutdown
	
	MainBridge::getInstance().setDBConnector(db_connector);

	CMainDlg dlg;
	return dlg.DoModal();//��������� ����������
}
//-------------------------------------------------------------------------------------------------------
void GetHBITMAP(HBITMAP *hbitmap, const wchar_t* fileJSON_UTF16)
{
	GdiPlusInitializer gdiplus;// ������������� GdiPlus, � ����������� Gdiplus::GdiplusShutdown
	CChartView chartView (true);
	chartView.getModel()->setPatient(fileJSON_UTF16);
	chartView.PrintAll(hbitmap);
}
//-------------------------------------------------------------------------------------------------------
