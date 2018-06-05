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
			di.push_back(ParserDrugFrom(1, L"1", test));
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

		for (size_t i = 0; i < di.size() - 2; i++)
		{
			if (!(di[i] == di[i + 1]))
			{
				cout << "error case #" << i << endl;
				
				success = false;
			}
		}
		cout << ((success) ? "SUCCESS" : "FAILED") << endl << "------------------------"<< endl;
		return success;

	}
	//-------------------------------------------------------------------------
};