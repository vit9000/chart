#pragma once

#include "ContainerUnit.h"

using namespace std;
class ContainerTabs : public ContainerUnit
{
public:
	ContainerTabs(const wstring& Name)
		: ContainerUnit(Name, L"ìã")
	{}
};