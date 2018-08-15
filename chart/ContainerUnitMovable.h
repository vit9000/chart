#pragma once

#include "ContainerUnit.h"
using namespace std;

class ContainerUnitMovable : public ContainerUnit
{
public:
	ContainerUnitMovable(const ID& _id, const DrugInfo& drug_Info)
		: ContainerUnit(_id, drug_Info, 1)
	{
		changeStatusAvailable = true;
		//type = DRUG__DEFAULT;
		if (drug_Info.ED != L"мл")
			balance = 0;

	}


	LogCommandPtr updateUnit(int unit_number, const Unit& updated_unit, bool create_log = true) override
	{
		if (units.count(unit_number) == 0) // если не было - добавляем
			return addUnit(updated_unit);

		// если есть юнит
		Unit& _unit = units[unit_number];
		Value value = updated_unit.getValue();
		if (value.isEmpty() || value.getDoubleValue() == 0)
		{
			if (_unit.getDB_ID().empty())
				return deleteUnit(unit_number, create_log);
			else 
				return nullptr;
		}

		// если не пустой - обновляем
		int STEP = config->getStep();
		int start = updated_unit.getStart() / STEP * STEP;
		if (updated_unit.getStart() % STEP > config->getCountSteps()+1)
			start += STEP;
		for (auto& unit : units)
		{
			if (unit.first != unit_number && unit.second.getStart() == start)
				start += STEP;
		}
		if (start >= config->getMaxMinute()) return nullptr;

		Unit copy_updated_unit(std::move(updated_unit));
		copy_updated_unit.setStart(start);
		
		replaceDB_ID(_unit, copy_updated_unit);//сохраняем DB_ID если он есть

		if (_unit.isFullyEqual(copy_updated_unit))
			return nullptr;

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