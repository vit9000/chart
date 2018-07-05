#pragma once

#include <string>
#include <Windows.h>

#include <string>
#include <regex>

bool isInputDataValid(const std::wstring& str);

COLORREF textToColor(const std::wstring& str);

class StringConverter
{
	std::wstring local_string;
public:
	StringConverter(const std::string& str)
	{
		//CP_ACP 
		/*DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, NULL, 0);
		wchar_t *pwText;
		pwText = new wchar_t[dwNum];
		MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, pwText, dwNum);
		local_string = pwText;
		delete[] pwText;*/
		int size_needed = MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
		local_string = wstrTo;
		
	}

	StringConverter(const std::wstring& str)
	{
		local_string = str;
	}

	operator std::wstring()
	{
		return local_string;
	}

	operator std::string ()
	{
		if (local_string.empty()) return std::string();
		int size_needed = WideCharToMultiByte(CP_ACP, 0, &local_string[0], (int)local_string.size(), NULL, 0, NULL, NULL);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(CP_ACP, 0, &local_string[0], (int)local_string.size(), &strTo[0], size_needed, NULL, NULL);
		return strTo;
	}
};