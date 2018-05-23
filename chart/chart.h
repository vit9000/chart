#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "type_defines.h"


extern "C" __declspec(dllexport) void ShowDialog(const wchar_t* fileJSON);
extern "C" __declspec(dllexport) void GetHBITMAP(HBITMAP *hbitmap, const wchar_t* fileJSON);
//extern "C" __declspec(dllexport) void SetFunc(void(*Func)(const std::wstring& request, std::vector<std::wstring>& result));
extern "C" __declspec(dllexport) void SetFunc(GetDrugFunction);




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
