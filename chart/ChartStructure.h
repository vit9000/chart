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
		if (BlockName == L"Гемодинамика")
			//params = { L"Систолическое АД",L"Диастолическое АД", L"", L"Диурез", L"По дренажам",L"Баланс" };
			params = { {L"Гемодинамика", PLOT } };
		else if (BlockName == L"Баланс")
			params = { {L"Per os/в зонд", NUMERIC }, {L"По зонду/рвота",NUMERIC }, {L"Стул", TEXT}, {L"Диурез", NUMERIC }, {L"По дренажам", NUMERIC}
	};
		else if (BlockName == L"Дыхание")
			params = {
				{L"Аппарат ИВЛ/ВВЛ", TEXT},
				{L"Режим ИВЛ/ВВЛ", TEXT},
				{L"ЧД апп./самост.", TEXT},
				{L"ДО апп./самост.", TEXT},
				{L"МОД",TEXT },
				{L"P пиковое", TEXT},
				{L"ПДКВ", TEXT },
				{L"FiO2", TEXT },
				{L"FetO2",TEXT },
				{L"FetCO2",TEXT},
				{L"SpO2", TEXT }};

		return params;
	}
	vector<wstring> getBlocks()
	{
		vector<wstring> blocks{ L"Гемодинамика",L"Баланс",L"Дыхание", L"Назначения" };
		return blocks;
	}

	wstring getText(int type)
	{
		return getBlocks()[type];
	}
};