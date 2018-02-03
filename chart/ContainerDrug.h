#pragma once

#include "ContainerUnit.h"

class ContainerDrug : public ContainerUnit
{
public:
	ContainerDrug(const wstring& Name) : ContainerUnit(Name) {}
}