#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols



extern "C" __declspec(dllexport) void ShowDialog();
extern "C" __declspec(dllexport) void GetHBITMAP(HBITMAP *hbitmap);

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
