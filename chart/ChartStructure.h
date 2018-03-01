#pragma once
#include <string>
#include <vector>

using namespace std;
class ChartStructure
{
public:
	enum { HEMODYNAMIC = 0, BALANS, ADMINISTRATIONS };

	vector<wstring> getBlockParameters(const wstring& BlockName)
	{
		vector<wstring> params;
		if (BlockName == L"Гемодинамика")
			//params = { L"Систолическое АД",L"Диастолическое АД", L"", L"Диурез", L"По дренажам",L"Баланс" };
			params = { L"Гемодинамика" };
		else if (BlockName == L"Баланс")
			params = { L"Per os/в зонд",L"По зонду/рвота", L"Диурез", L"По дренажам" };
		return params;
	}
	vector<wstring> getBlocks()
	{
		vector<wstring> blocks{ L"Гемодинамика",L"Баланс", L"Назначения" };
		return blocks;
	}

	wstring getText(int type)
	{
		return getBlocks()[type];
	}
};