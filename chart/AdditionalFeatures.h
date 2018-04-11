#pragma once
#include <string>
#include <sstream>
#include <Windows.h>
using namespace std;

class AdditionalFeatures
{
public:
	void RunDrugInfo(const wstring& name)
	{
		wstringstream ss;
		ss << L"https://www.vidal.ru/search?t=product&q=" << name;
		ShellExecute(0, NULL, ss.str().c_str(), NULL, NULL, SW_RESTORE);
	}
};