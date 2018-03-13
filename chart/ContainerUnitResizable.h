#pragma once


#include "ContainerUnit.h"
using namespace std;

class ContainerUnitResizable : public ContainerUnit
{
public:
	ContainerUnitResizable(const wstring& Name, const wstring& Measure_unit)
		: ContainerUnit(Name, Measure_unit)
	{}

	void addUnit(const Unit& NewUnit) override
	{
		Unit unit(NewUnit);

		int start = unit.getStart();
		int duration = unit.getDuration();
		allocateUnit(units.size(), start, duration);
		if (start >= 1440) return;

		unit.setStart(start);
		unit.setDuration(duration);

		units.push_back(unit);
		sort();
	}

	void updateUnit(int index, const Unit& unit) override
	{
		int start = unit.getStart();
		int duration = unit.getDuration();

		allocateUnit(index, start, duration);
		if (start > 1440 - Unit::MIN_DURATION) return;
		if (start + duration >= 1440) duration = 1440 - start;
		if (start < 0) start = 0;
		
		

		units[index] = unit;
		units[index].setStart(start);
		units[index].setDuration(duration);
		sort();
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

		for (size_t i = 0; i < units.size(); i++)
		{
			if (i == index) continue;
			if (units[i] == start)
				start = units[i].getStart() + units[i].getDuration() + 1;
			else
			{
				int temp = duration;
				if (start < units[i].getStart() && units[i].getStart() < start + duration)
					temp = units[i].getStart() - start - 1;
				if (temp < Unit::MIN_DURATION)
				{
					start = units[i].getStart() + units[i].getDuration() + 1;
					i = 0;
				}
				else
					duration = temp;
			}
		}
	}

};