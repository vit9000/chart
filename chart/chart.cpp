#include "stdafx.h"
#include "chart.h"
#include "MainDlg.h"

#pragma comment( lib, "gdiplus.lib" )

void ShowDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	GdiPlusInitializer gdiplus; // ������������� GdiPlus, � ����������� Gdiplus::GdiplusShutdown
	
	CMainDlg dlg;
	dlg.DoModal();//��������� ����������
}
//-------------------------------------------------------------------------------------------------------
void GetHBITMAP(HBITMAP *hbitmap)
{
	GdiPlusInitializer gdiplus;// ������������� GdiPlus, � ����������� Gdiplus::GdiplusShutdown
	CChartView chartView (true);
	chartView.getModel()->setPatient(0);
	chartView.PrintAll(hbitmap);
}
//-------------------------------------------------------------------------------------------------------
