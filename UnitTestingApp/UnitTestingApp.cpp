// UnitTestingApp.cpp : Defines the entry point for the console application.
//
//#include <iostream>
//#include <ostream>
#include "stdafx.h"
#include "Tests.h"
#include <iostream>
#include <Windows.h>
using namespace std;

int main()
{
	Tests().Execute();
	Sleep(10000);
	
    return 0;
}

