#pragma once


#include "ContainerUnit.h"
using namespace std;

class ContainerUnitResizable : public ContainerUnit
{
public:
	ContainerUnitResizable(const ID& _id, const DrugInfo& drug_Info)
		: ContainerUnit(_id, drug_Info)
	{
		changeStatusAvailable = true;
		//type = DRUG__DEFAULT;
	}

	LogCommandPtr addUnit(const Unit& NewUnit, bool create_log = true) override
	{
		Value value = NewUnit.getValue();
		if (value.isEmpty())
		{
			if (childs.size() > 0) // �� ���� ���� �������� parent - �� ����� ���� ������
			{
				value = Value(0); // ����� ���� 0
			}
			else // ��� ���������
				return nullptr;
			
		}
		

		Unit unit(NewUnit);
		if (unit.isEmpty()) unit.setValue(0);
		int start = unit.getStart();
		int duration = unit.getDuration();
		allocateUnit(units.size(), start, duration);
		if (start >= 1440) return nullptr;
		if (units.count(start) != 0)
			return nullptr;


		unit.setStart(start);
		unit.setDuration(duration);

		Unit& _unit = units[start];
		_unit = std::move(unit);
		calculateSumm();

		// ������� ��� � �������� �������� - ��������
		LogCommandPtr log_command = (!create_log) ? nullptr : createLogCommandAddUnit(_unit);

		return log_command;
	}


	LogCommandPtr updateUnit(int unit_number, const Unit& updated_unit, bool create_log = true) override
	{
		// ���� �� ���� - ���������
		if (units.count(unit_number) == 0) 
			return addUnit(updated_unit, create_log);
		// ���� ��� ����
		Value value = updated_unit.getValue();
		if (value.isEmpty())
		{
			if (childs.size() > 0) // �� ���� ���� �������� parent - �� ����� ���� ������
			{
				value = Value(0); // ����� ���� 0
			}
			else // ��� ���������
				return deleteUnit(unit_number, create_log);

		}


		// ���� �� ������ - ���������
		int start = updated_unit.getStart();
		int duration = updated_unit.getDuration();

		allocateUnit(unit_number, start, duration);
		if (start > 1440 - Unit::MIN_DURATION) return nullptr;
		if (start + duration >= 1440) duration = 1440 - start;
		if (start < 0) start = 0;
		
		Unit copy_updated_unit(updated_unit.getValue(), start, duration);
		

		Unit& _unit = units[start];
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
protected:
	void allocateUnit(size_t index, int& start, int& duration)
	{
		if (duration < Unit::MIN_DURATION)
			duration = Unit::MIN_DURATION;
		else if (duration > Unit::MAX_DURATION)
			duration = Unit::MAX_DURATION;
		// ������������ � ������������ MIN_DURATION
		else if (duration % Unit::MIN_DURATION > 13)
			duration += Unit::MIN_DURATION;
		duration = duration / Unit::MIN_DURATION * Unit::MIN_DURATION;

		// ������������ ������ �������� MIN_DURATION
		int tt = start % Unit::MIN_DURATION;
		if (tt > 13)
			start += Unit::MIN_DURATION;
		start = start / Unit::MIN_DURATION * Unit::MIN_DURATION;

		for (map<int, Unit>::iterator it = units.begin(); 
			it!= units.end(); ++it)
		{
			if (it->first == index) continue;
			auto& unit = it->second;
			if (unit == start)
				start = unit.getStart() + unit.getDuration() + 1;
			else
			{
				int temp = duration;
				if (start < unit.getStart() && unit.getStart() < start + duration)
					temp = unit.getStart() - start - 1;
				if (temp < Unit::MIN_DURATION)
				{
					start = unit.getStart() + unit.getDuration() + 1;
					it = units.begin();
				}
				else
					duration = temp;
			}
		}
	}

};