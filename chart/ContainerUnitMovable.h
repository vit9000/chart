#pragma once

#include "ContainerUnit.h"
using namespace std;

class ContainerUnitMovable : public ContainerUnit
{
public:
	ContainerUnitMovable(const wstring& Name, const wstring& Measure_unit)
		: ContainerUnit(Name, Measure_unit)
	{}


	void updateUnit(int index, const Unit& NewUnit) override
	{
		int start = NewUnit.getStart() / 60 * 60;
		if (NewUnit.getStart() % 60 > 25)
			start += 60;
		for (size_t i = 0; i < units.size(); i++)
		{
			if (i != index && units[i].getStart() == start)
				start += 60;
		}
		if (start >= 1440) return;
		Unit unit(NewUnit.getValue(), start, 60);
		units[index] = unit;
		sort();
	}
};