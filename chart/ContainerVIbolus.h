#pragma once

#pragma once
#include "ContainerUnit.h"

using namespace std;
class ContainerIVbolus : public ContainerUnit
{
public:
	ContainerIVbolus(const wstring& Name)
		: ContainerUnit(Name, L"��")
	{}

	void addUnit(const Unit& NewUnit) override
	{
		int start = NewUnit.getStart() / 60 * 60;
		Unit unit(NewUnit.getValue(), start, 60);
		units.push_back(unit);
		sort();
	}

	void updateUnit(int index, const Unit& NewUnit) override
	{
		int start = NewUnit.getStart() / 60 * 60;
		if (NewUnit.getStart() % 60 > 25)
			start += 60;
		Unit unit(NewUnit.getValue(), start, 60);
		units[index] = unit;
		sort();
	}
};