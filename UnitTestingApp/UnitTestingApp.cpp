// UnitTestingApp.cpp : Defines the entry point for the console application.
//
//#include <iostream>
//#include <ostream>
#include "stdafx.h"
#include "DrugInfo.h"
#include "ParserDrugForm.h"
#include <iostream>
using namespace std;
int main()
{
	bool success = true;
	vector<DrugInfo> d;
	{
		d.push_back(ParserDrugFrom(1, L"1", L"10mg/ ml"));
		d.push_back(ParserDrugFrom(1, L"1", L"10mg/ 1ml"));
		d.push_back(ParserDrugFrom(1, L"1", L"10mg/1ml"));
		d.push_back(ParserDrugFrom(1, L"1", L"10mg/ml"));

		
		for (size_t i = 0; i < d.size() - 2; i++)
		{
			if (!(d[i] == d[i + 1]))
			{
				
				cout << "error case #" << i << endl;
				success = false;
			}

		}

	}


	cout << ((success) ? "SUCCESS" : "ERRORS OCCURED") << endl;

	int i;
	cin >> i;
    return 0;
}

