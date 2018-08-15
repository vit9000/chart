#pragma once


#include "ContainerUnit.h"

using namespace std;

class ContainerUnitResizable : public ContainerUnit
{
public:
	ContainerUnitResizable(const ID& _id, const DrugInfo& drug_Info, int BalanceType)
		: ContainerUnit(_id, drug_Info, BalanceType)
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
		if (start >= config->getMaxMinute()) return nullptr;
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

		// ���� ���� ����
		Unit& _unit = units[unit_number];
		Value value = updated_unit.getValue();
		if (value.isEmpty() || value.getDoubleValue()==0)
		{
			if (childs.size() > 0) // �� ���� ���� �������� parent - �� ����� ���� ������
			{
				value = Value(0); // ����� ���� 0
			}
			else if (_unit.getDB_ID().empty())
				return deleteUnit(unit_number, create_log);
			else 
				return nullptr;
		}

		// ���� �� ������ - ���������
		int start = updated_unit.getStart();
		int duration = updated_unit.getDuration();

		allocateUnit(unit_number, start, duration);
		if (start > config->getMaxMinute() - config->getMinDuration()) return nullptr;
		if (start + duration >= config->getMaxMinute()) duration = config->getMaxMinute() - start;
		if (start < 0) start = 0;
		
		Unit copy_updated_unit(std::move(updated_unit)); // �� �����, ��� ���������� ����������
		copy_updated_unit.setStart(start);
		copy_updated_unit.setDuration(duration);
		
		
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
protected:
	void allocateUnit(size_t index, int& start, int& duration)
	{
		int MIN_DURATION = config->getMinDuration();
		int MAX_DURATION = config->getMaxMinute();

		if (duration < MIN_DURATION)
			duration = MIN_DURATION;
		else if (duration > MAX_DURATION)
			duration = MAX_DURATION;
		// ������������ � ������������ MIN_DURATION
		else if (duration % MIN_DURATION > 13)
			duration += MIN_DURATION;
		duration = duration / MIN_DURATION * MIN_DURATION;

		// ������������ ������ �������� MIN_DURATION
		int old_start = start;
		start = start / MIN_DURATION * MIN_DURATION;
		int r = old_start-start;
		if (r > MIN_DURATION/2)
			start += MIN_DURATION;
		

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
					temp = unit.getStart() - start;
				if (temp < MIN_DURATION)
				{
					start = unit.getStart() + unit.getDuration();
					it = units.begin();
				}
				else
					duration = temp;
			}
		}
	}

};