#include "stdafx.h"
#include "utils.h"


#pragma warning( disable: 4129 )

bool isInputDataValid(const std::wstring& str)
{
	if (str.empty()) return true;
	std::wregex r = std::wregex(L"[0-9]{1}[\.,]?[0-9]*");
	if (std::regex_match(str, r))
	{
		return true;
	}
	return false;
}

COLORREF textToColor(const std::wstring& str)
{
	std::wstring temp(str);
	COLORREF crefColor;
	//COLORREF crefColor2;
	swscanf_s(temp.c_str(), L"%x", &crefColor); // should return the same as strtol
												//crefColor2 = strtol(pszTmp, NULL, 16); // should return the same as sscanf_s
												//ASSERT(crefColor == crefColor2); // just to be sure
	INT nR = GetRValue(crefColor);
	INT nG = GetGValue(crefColor);
	INT nB = GetBValue(crefColor);
	return crefColor = RGB(nB, nG, nR);
}

std::wstring DateToString(double date)
{
	return static_cast<COleDateTime>(date).Format(L"%Y-%m-%d %H:%M:%S").GetBuffer();
}

