#include "stdafx.h"
#include "chart.h"
#include "MainDlg.h"

#pragma comment( lib, "gdiplus.lib" )

void ShowDialog(const wchar_t* fileJSON_UTF16)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	GdiPlusInitializer gdiplus; // ������������� GdiPlus, � ����������� Gdiplus::GdiplusShutdown
	
	CMainDlg dlg(fileJSON_UTF16);
	dlg.DoModal();//��������� ����������
}
//-------------------------------------------------------------------------------------------------------
void GetHBITMAP(HBITMAP *hbitmap, const wchar_t* fileJSON_UTF16)
{
	GdiPlusInitializer gdiplus;// ������������� GdiPlus, � ����������� Gdiplus::GdiplusShutdown
	CChartView chartView (true);
	chartView.getModel()->setPatient(0, fileJSON_UTF16);
	chartView.PrintAll(hbitmap);
}
//-------------------------------------------------------------------------------------------------------
void SetFunc(const wchar_t* (*f)(const wchar_t* request))
{
	wstring str = (*f)(L"Hello from DLL");

}
