#pragma once
#include "DrugInfo.h"
#include "ParserDrugForm.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class UnitTest
{
private: // variables
	int test_number;
	vector<DrugInfo> di;
	//-------------------------------------------------------------------------
private: // functions
	void fill(const vector<wstring>& test_list)
	{
		static int test = 1;
		test_number = test;
		test++;

		for (const wstring& test : test_list)
		{
			di.push_back(ParserDrugFrom(L"1", L"1", test));
		}
	}
public:
	UnitTest(const vector<wstring>& test_list)
	{
		fill(test_list);
	}
	//-------------------------------------------------------------------------
	UnitTest(const DrugInfo& etalon, const vector<wstring>& test_list)
	{
		di.push_back(etalon);
		fill(test_list);
	}
	//-------------------------------------------------------------------------
	bool operator()()
	{
		bool success = true;
		cout << "Test #" << test_number << endl;

		for (size_t i = 0; i < di.size() - 1; i++)
		{
			if (!(di[i] == di[i + 1]))
			{
				cout << "------------------------" << endl << "error case #" << i << endl;
				wcout << L"DrugInfo:"
					<< L"percent = " << di[i].percent << L" - " << di[i + 1].percent << endl
					<< L"dose = " << di[i].dose << L" - " << di[i + 1].dose << endl;
					//<< L"ED = " << di[i].ED << L" - " << di[i + 1].ED << endl;
				
				success = false;
			}
		}
		cout << ((success) ? "SUCCESS" : "FAILED") << endl << "======================================"<< endl;
		return success;

	}
	//-------------------------------------------------------------------------
};