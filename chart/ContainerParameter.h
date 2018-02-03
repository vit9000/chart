#pragma once

#include "ContainerUnit.h"

class ContainerParameter : public ContainerUnit
{
public:
	ContainerParameter(const wstring& Name) : ContainerUnit(Name) {}
}