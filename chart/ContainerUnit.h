#pragma once

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "Unit.h"



using namespace std;

class ContainerUnit
{
protected:
	ID id;
	vector<Unit> units;
	map<int, ID> linkedContainers;
	wstring name;
	wstring measure_unit;

	double summ;
	void sort()
	{
		std::sort(units.begin(), units.end());
		calculateSumm();
	}
	virtual void calculateSumm()
	{
		summ = 0;
		for (const Unit& unit : units)
		{
			double temp = unit.getValue().getDoubleValue();
			if(temp != Value::EMPTY)
				summ += unit.getValue().getDoubleValue();
		}
	}
public:
	ContainerUnit(const wstring& BlockName, const wstring& Name, const wstring& Measure_unit) 
		: name(Name), 
		measure_unit(Measure_unit), 
		summ (0.),
		id(getNewID(BlockName))
	{
	}
	inline ID getNewID(const wstring& BlockName)
	{
		static int index = 0;
		index++;
		return ID(BlockName, index);
	}

	virtual ~ContainerUnit() {};

	void linkContainerUnit(ContainerUnit* containerUnit)
	{
		if (!containerUnit) return;

		const ID& id = containerUnit->getID();
		linkedContainers.insert(make_pair(id.getIndex(), ID(id)));
		
		containerUnit->units.clear();
		for(const Unit& unit : units)
			containerUnit->units.push_back(Unit(0., unit.getStart(), unit.getDuration()));
	}

	void rename(const wstring& NewName) { name = NewName; }
	const ID& getID() const
	{
		return id;
	}
	virtual void addUnit(const Unit& NewUnit)
	{
		int start = NewUnit.getStart() / 60 * 60;
		Unit unit(NewUnit.getValue(), start, 60);
		units.push_back(unit);
		sort();
	}
	virtual void updateUnit(int index, const Unit& unit)
	{
		units[index] = unit;
		sort();
	}
	virtual wstring getSumm() const 
	{
		wstringstream ss;
		ss << summ;
		return ss.str();
	}
	void removeUnit(size_t) {}
	const wstring& getMeasureUnit() const { return measure_unit; }
	const wstring& getName() const { return name;}
	const vector<Unit>& getUnits() const { return units; }
	const Unit& getUnit(int index) const 
	{ 
		if (index >= static_cast<int>(units.size()))
			throw invalid_argument("getUnit invalid argument");
		return units.at(index);
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