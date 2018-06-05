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
				Etalon(1, 1, L"��"),
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

		
		//ParserDrugFrom d2(2, L"������", L"���. 4��+�-�� 5�� �1");
		//ParserDrugFrom d3(3, L"��������", L"�-� �/���. 0,25��+0,5��/�� 20��");
		//ParserDrugFrom d4(4, L"��������� ����������", L"160+4,5���/120���");
		//ParserDrugFrom d5(5, L"�������������", L"����. �\��. 125��/5�� �1");
		//ParserDrugFrom d6(6, L"�������", L"** ��. 5���/�� 5�� �5");
	}
};
