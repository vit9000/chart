#pragma once

#include <string>
#include <vector>
#include "Unit.h"

using namespace std;

class ContainerUnit
{
protected:
	vector<Unit> units;
	wstring name;
public:
	ContainerUnit(const wstring& Name) : name(Name) {}

	void rename(const wstring& NewName){ name = NewName; }
	void addUnit(const Unit& NewUnit) { units.push_back(Unit(NewUnit)); }
	void removeUnit(size_t) {}

	const wstring& getName() const { return name;}
	const vector<Unit>& getUnits() const { return units; }
};