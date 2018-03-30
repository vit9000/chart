#pragma once

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "Unit.h"
#include "DrugInfo.h"



using namespace std;

class ContainerUnit
{
protected:
	ID id;
	ID parent_id;
	//map<int, ID> child_ids;
	vector<Unit> units;
	
	DrugInfo drugInfo;

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
	ContainerUnit(const wstring& BlockName, const DrugInfo& drug_Info)
		: id(getNewID(BlockName)),
		parent_id(ID(BlockName, -1)),
		drugInfo(drug_Info),
		summ (0.)
		
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
		//const ID& id = containerUnit->getID();
		//child_ids.insert(make_pair(id.getIndex(), ID(id)));// 
		containerUnit->parent_id = id;//у дочерних
		containerUnit->units.clear();
		for(const Unit& unit : units)
			containerUnit->units.push_back(Unit(0., unit.getStart(), unit.getDuration()));
	}

//	inline void rename(const wstring& NewName) { name = NewName; }

	const ID& getID() const
	{
		return id;
	}
	const ID& getParentID() const
	{
		return parent_id;
	}
	bool isChild() const
	{
		return (parent_id.getIndex() != -1);
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
	const wstring& getMeasureUnit() const { return drugInfo.ED; }
	const wstring& getName() const { return drugInfo.name;}
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