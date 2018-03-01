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
		if (BlockName == L"������������")
			//params = { L"������������� ��",L"�������������� ��", L"", L"������", L"�� ��������",L"������" };
			params = { L"������������" };
		else if (BlockName == L"������")
			params = { L"Per os/� ����",L"�� �����/�����", L"������", L"�� ��������" };
		return params;
	}
	vector<wstring> getBlocks()
	{
		vector<wstring> blocks{ L"������������",L"������", L"����������" };
		return blocks;
	}

	wstring getText(int type)
	{
		return getBlocks()[type];
	}
};