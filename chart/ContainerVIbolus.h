#pragma once

#pragma once
#include "ContainerUnit.h"

using namespace std;
class ContainerIVbolus : public ContainerUnit
{
public:
	ContainerIVbolus(const wstring& Name)
		: ContainerUnit(Name, L"ìã")
	{}
};