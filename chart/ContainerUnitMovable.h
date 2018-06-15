#pragma once

#include "ContainerUnit.h"
using namespace std;

class ContainerUnitMovable : public ContainerUnit
{
public:
	ContainerUnitMovable(const ID& _id, const DrugInfo& drug_Info)
		: ContainerUnit(_id, drug_Info)
	{
		changeStatusAvailable = true;
		//type = DRUG__DEFAULT;
	}


	bool updateUnit(int index, const Unit& NewUnit) override
	{
		if (isDigit() && !isInputDataValid(NewUnit.getValue()))
			return false;
		int start = NewUnit.getStart() / 60 * 60;
		if (NewUnit.getStart() % 60 > 25)
			start += 60;
		for (size_t i = 0; i < units.size(); i++)
		{
			if (i != index && units[i].getStart() == start)
				start += 60;
		}
		if (start >= 1440) return false;
		Unit unit(NewUnit.getValue(), start, 60);
		units[index] = unit;
		sort();

		return true;
	}
};