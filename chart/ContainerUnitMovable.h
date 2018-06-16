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


	bool updateUnit(int unit_number, const Unit& unit) override
	{
		if (units.count(unit_number) == 0) // ���� �� ���� - ���������
			return addUnit(unit);

		// ���� ��� ����
		if (unit.isEmpty())
			return deleteUnit(unit_number);

		// ���� �� ������ - ���������
		int start = unit.getStart() / 60 * 60;
		if (unit.getStart() % 60 > 25)
			start += 60;
		for (auto& unit : units)
		{
			if (unit.first != unit_number && unit.second.getStart() == start)
				start += 60;
		}
		if (start >= 1440) return false;
		
		// ���� ����� �������, �� ������� ������
		if (unit_number != start)
			deleteUnit(unit_number);
		// ��������� �����
		units[start] = std::move(Unit(unit.getValue(), start, 60));
		calculateSumm();

		return true;
	}
};