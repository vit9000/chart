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

	bool addUnit(const Unit& NewUnit) override
	{
		
		if (NewUnit.isEmpty())
			return false;

		if (addUnitLikeTemplate(NewUnit))
				return true;


		Unit unit(NewUnit);
		int start = unit.getStart();
		int duration = unit.getDuration();
		allocateUnit(units.size(), start, duration);
		if (start >= 1440) return false;
		if (units.count(start) != 0)
			return false;

		unit.setStart(start);
		unit.setDuration(duration);

		units[start] = std::move(unit);
		calculateSumm();

		return true;
	}

	bool addUnitLikeTemplate(const Unit& NewUnit)
	{
		int unit_number = NewUnit.getStart();
		if (parent)
		{
			if (parent->isUnitNumberValid(unit_number))
			{
				Unit unit = parent->getUnit(unit_number);
				unit.setValue(NewUnit.getValue());
				units[unit_number] = std::move(unit);
				return true;
			}
		}
		else if(childs.size()>0)
		{
			for (auto& child : childs)
			{
				if (child && child->isUnitNumberValid(unit_number))
				{
					Unit unit = parent->getUnit(unit_number);
					unit.setValue(NewUnit.getValue());
					units[unit_number] = std::move(unit);
					return true;
				}
			}
		}
		return false;
	}

	bool updateUnit(int unit_number, const Unit& unit) override
	{
		// если не было - добавляем
		if (units.count(unit_number) == 0) 
			return addUnit(unit);
		// если был юнит
		if (unit.isEmpty() && childs.size()==0)
			return deleteUnit(unit_number);

		// если не пустой - обновляем
		int start = unit.getStart();
		int duration = unit.getDuration();

		allocateUnit(unit_number, start, duration);
		if (start > 1440 - Unit::MIN_DURATION) return false;
		if (start + duration >= 1440) duration = 1440 - start;
		if (start < 0) start = 0;
		
		// если новая позиция, то удаляем старую
		if (unit_number != start)
			deleteUnit(unit_number);
		// добавляем новую
		units[start] = std::move(unit);
		units[start].setStart(start);
		units[start].setDuration(duration);
		calculateSumm();

		return true;
	}
protected:
	void allocateUnit(size_t index, int& start, int& duration)
	{
		if (duration < Unit::MIN_DURATION)
			duration = Unit::MIN_DURATION;
		else if (duration > Unit::MAX_DURATION)
			duration = Unit::MAX_DURATION;
		// выравнивание в длительность MIN_DURATION
		else if (duration % Unit::MIN_DURATION > 13)
			duration += Unit::MIN_DURATION;
		duration = duration / Unit::MIN_DURATION * Unit::MIN_DURATION;

		// выравнимание начала кратного MIN_DURATION
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