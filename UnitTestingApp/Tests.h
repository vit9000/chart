#pragma once
#include "UnitTest.h"
#include <cassert>
class Tests
{
	DrugInfo Etalon(double percent, double dose, const wstring& ED)
	{
		DrugInfo d;
		d.percent = percent;
		d.dose = dose;
		d.ED = ED;
		return d;
	}
public:
	void Execute()
	{
		
		{
			assert (UnitTest(
				Etalon(1, 1, L"ЛК"),
				{
					L"10mg/ ml",
					L"10mg/ 1ml",
					L"10mg/1ml",
					L"10mg/ml",
					L"10 mg/ml",
					L"10mg /ml",
				})());

		}

		{
			assert (!UnitTest({
				L"10mg/ ml",
				L"10mg/ 1ml",
				L"10mg/1ml",
				L"10mg/ml"
			})());

		}

		
		//ParserDrugFrom d2(2, L"гнлерю", L"ОНП. 4ЛЦ+П-КЭ 5ЛК ╧1");
		//ParserDrugFrom d3(3, L"аепндсюк", L"П-П Д/ХМЦ. 0,25ЛЦ+0,5ЛЦ/ЛК 20ЛК");
		//ParserDrugFrom d4(4, L"яхлахйнпр рспасуюкеп", L"160+4,5ЛЙЦ/120ДНГ");
		//ParserDrugFrom d5(5, L"цхдпнйнпрхгнм", L"ЯСЯО. Д\ХМ. 125ЛЦ/5ЛК ╧1");
		//ParserDrugFrom d6(6, L"цеоюпхм", L"** ТК. 5Ред/ЛК 5ЛК ╧5");
	}
};
