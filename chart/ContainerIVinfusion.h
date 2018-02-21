#pragma once


#include "ContainerUnit.h"

using namespace std;
class ContainerIVinfusion : public ContainerUnit
{
public:
	ContainerIVinfusion(const wstring& Name)
		: ContainerUnit(Name, L"мл/час")
	{}
};