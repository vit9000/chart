#include "stdafx.h"
#include "utils.h"

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

