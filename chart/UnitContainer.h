#pragma once

#include <string>
#include <vector>
#include "Unit.h"

using namespace std;

class UnitContainer
{
private:
	vector<Unit> units;
	wstring name;
public:
	UnitContainer(const wstring& Name) : name(Name) {}

	void rename(const wstring& NewName){ name = NewName; }
	void addUnit(const Unit& NewUnit) { units.push_back(Unit(NewUnit)); }
	void removeUnit(size_t) {}
};