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
		units.push_back(Unit(NewUnit)); 
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
		units[index] = unit;
		sort();
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