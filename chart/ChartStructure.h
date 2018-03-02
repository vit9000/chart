#pragma once
#include <string>
#include <vector>

using namespace std;
class ChartStructure
{
public:
	enum { HEMODYNAMIC = 0, BALANS, BREATH, ADMINISTRATIONS };
	enum {PLOT, NUMERIC, TEXT};

	vector<pair<wstring, int>> getBlockParameters(const wstring& BlockName)
	{
		vector<pair<wstring, int>> params;
		if (BlockName == L"������������")
			//params = { L"������������� ��",L"�������������� ��", L"", L"������", L"�� ��������",L"������" };
			params = { {L"������������", PLOT } };
		else if (BlockName == L"������")
			params = { {L"Per os/� ����", NUMERIC }, {L"�� �����/�����",NUMERIC }, {L"����", TEXT}, {L"������", NUMERIC }, {L"�� ��������", NUMERIC}
	};
		else if (BlockName == L"�������")
			params = {
				{L"������� ���/���", TEXT},
				{L"����� ���/���", TEXT},
				{L"�� ���./������.", TEXT},
				{L"�� ���./������.", TEXT},
				{L"���",TEXT },
				{L"P �������", TEXT},
				{L"����", TEXT },
				{L"FiO2", TEXT },
				{L"FetO2",TEXT },
				{L"FetCO2",TEXT},
				{L"SpO2", TEXT }};

		return params;
	}
	vector<wstring> getBlocks()
	{
		vector<wstring> blocks{ L"������������",L"������",L"�������", L"����������" };
		return blocks;
	}

	wstring getText(int type)
	{
		return getBlocks()[type];
	}
};