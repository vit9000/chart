#pragma once

#include <string>
#include <Windows.h>

class StringConverter
{
	std::wstring local_string;
public:
	StringConverter(const std::string& string)
	{
		//CP_ACP 
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, NULL, 0);
		wchar_t *pwText;
		pwText = new wchar_t[dwNum];
		MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, pwText, dwNum);
		local_string = pwText;
		delete[] pwText;
		
	}
	operator std::wstring()
	{
		return local_string;
	}
};