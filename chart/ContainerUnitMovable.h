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


	LogCommandPtr updateUnit(int unit_number, const Unit& updated_unit, bool create_log = true) override
	{
		if (units.count(unit_number) == 0) // если не было - добавляем
			return addUnit(updated_unit);

		// если был юнит
		if (updated_unit.isEmpty())
		{
			return deleteUnit(unit_number);
		}

		// если не пустой - обновляем
		int start = updated_unit.getStart() / 60 * 60;
		if (updated_unit.getStart() % 60 > 25)
			start += 60;
		for (auto& unit : units)
		{
			if (unit.first != unit_number && unit.second.getStart() == start)
				start += 60;
		}
		if (start >= 1440) return nullptr;

		Unit copy_updated_unit(std::move(updated_unit));
		copy_updated_unit.setStart(start);
		
		LogCommandPtr log_command = (!create_log) ? nullptr : createLogCommandUpdateUnit(units[unit_number], copy_updated_unit);
		// если новая позиция, то удаляем старую
		if (unit_number != start)
			deleteUnit(unit_number);
		// добавляем новую
		units[start] = std::move(copy_updated_unit);
		calculateSumm();

		return log_command;
	}
};