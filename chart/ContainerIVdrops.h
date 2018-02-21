#pragma once
#include "ContainerUnit.h"

using namespace std;
class ContainerIVdrops : public ContainerUnit
{
public:
	ContainerIVdrops(const wstring& Name)
		: ContainerUnit(Name, L"мл")
	{}
};