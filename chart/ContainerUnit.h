#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include "Unit.h"



using namespace std;

class ContainerUnit
{
protected:
	vector<Unit> units;
	wstring name;
	wstring measure_unit;

	void sort()
	{
		std::sort(units.begin(), units.end());
	}
public:
	ContainerUnit(const wstring& Name, const wstring& Measure_unit) : name(Name), measure_unit(Measure_unit){}
	virtual ~ContainerUnit() {};
	void rename(const wstring& NewName){ name = NewName; }
	virtual void addUnit(const Unit& NewUnit) 
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
	void removeUnit(size_t) {}

	const wstring& getMeasureUnit() const {
		return measure_unit;
	}

	const wstring& getName() const { return name;}
	const vector<Unit>& getUnits() const { return units; }
	const Unit& getUnit(int index) const 
	{ 
		if (index >= static_cast<int>(units.size()))
			throw invalid_argument("getUnit invalid argument");
		return units.at(index);
	}
	virtual void updateUnit(int index, const Unit& unit)
	{
		int start = unit.getStart();
		int duration = unit.getDuration();
		
		allocateUnit(index, start, duration);
		if (start >= 1440) return;

		units[index] = unit;
		units[index].setStart(start);
		units[index].setDuration(duration);
		sort();
	}
	virtual void allocateUnit(size_t index, int& start, int& duration)
	{
		if (duration < Unit::MIN_DURATION)
			duration = Unit::MIN_DURATION;
		else if (duration > Unit::MAX_DURATION)
			duration = Unit::MAX_DURATION - 1;
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

	int find(int minute) const
	{
		for (size_t i=0; i<units.size(); ++i)
		{
			if (units.at(i) == minute)
				return static_cast<int>(i);
		}
		return -1;
	}
};