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
		if (units.count(unit_number) == 0) // ���� �� ���� - ���������
			return addUnit(updated_unit);

		// ���� ��� ����
		if (updated_unit.isEmpty())
		{
			return deleteUnit(unit_number);
		}

		// ���� �� ������ - ���������
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
		
		Unit& _unit = units[unit_number];
		replaceDB_ID(_unit, copy_updated_unit);//��������� DB_ID ���� �� ����

		if (_unit.isFullyEqual(copy_updated_unit))
			return nullptr;

		LogCommandPtr log_command = (!create_log) ? nullptr : createLogCommandUpdateUnit(units[unit_number], copy_updated_unit);
		// ���� ����� �������, �� ������� ������
		if (unit_number != start)
			deleteUnit(unit_number);
		// ��������� �����
		units[start] = std::move(copy_updated_unit);
		calculateSumm();

		return log_command;
	}
};