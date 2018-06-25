#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "type_defines.h"
#include "IDBConnector.h"


extern "C" __declspec(dllexport) int ShowDialog(IDBConnector * db_connector);
extern "C" __declspec(dllexport) void GetHBITMAP(HBITMAP *hbitmap, const wchar_t* fileJSON);



class GdiPlusInitializer
{
	ULONG_PTR gdiplusToken;
public:
	GdiPlusInitializer()
	{
		Gdiplus::GdiplusStartupInput gdiplusInput;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusInput, NULL);
	}
	~GdiPlusInitializer()
	{
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}
};
