// UnitTestingApp.cpp : Defines the entry point for the console application.
//
//#include <iostream>
//#include <ostream>
#include "stdafx.h"
#include "UnitTest.h"
#include <iostream>
using namespace std;
int main()
{
	
	{
			UnitTest({
			L"10mg/ ml",
			L"10mg/ 1ml",
			L"10mg/1ml",
			L"10mg/ml"
		})();

	}

	int i;
	cin >> i;
    return 0;
}

