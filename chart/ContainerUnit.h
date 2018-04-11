#pragma once

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "Unit.h"
#include "DrugInfo.h"
#include "ValueInputDlg.h"



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

	void MakeSolution(const wstring& dilution_volume)
	{
		ValueInputDlg dlg;
		wstring ED = drugInfo.getPercentString() + drugInfo.ED;
		dlg.Init(drugInfo.name, { wstring(L"Доза (") + ED + wstring(L")"), L"Объем разведения (мл)" }, { drugInfo.dose, dilution_volume });
		if (dlg.DoModal() == IDOK)
		{
			vector<Value> val = dlg.getValue();
			Value dose = val[0];
			Value volume = val[1];

			// значение должно быть больше 0
			double d = dose.getDoubleValue();
			if (d <= 0)
			{
				dose = drugInfo.dose;
				d = dose.getDoubleValue();
			}
			// значение должно быть больше 0
			double v = volume.getDoubleValue();
			if (v <= 0)
			{
				volume = dilution_volume;
				v = volume.getDoubleValue();
			}
			
			

			drugInfo.dilution = L"[" + drugInfo.getPercentString() + wstring(dose) + L" " + drugInfo.ED + L"/" + wstring(volume) + L" мл]";
			drugInfo.ED = L"мл";
			drugInfo.dose = volume;

			wstringstream ss;
			double temp = d * (drugInfo.isSolution()) ? drugInfo.getPercentNumber() : 1;
			temp /= v;
			if (drugInfo.ED == L"г")
				temp *= 100;
			else if (drugInfo.ED == L"мг")
				temp /= 10;
			else if (drugInfo.ED == L"мкг")
				temp /= 10000;
			ss << temp;
			drugInfo.percent = ss.str();
			
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

	virtual void deleteUnit(int number)
	{
		units.erase(units.begin() + number);
		calculateSumm();
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
	const wstring& getAdminWayName() const { return drugInfo.selected_way_name; }
	const DrugInfo& getDrugInfo() const { return drugInfo; }
	int getAdminWay() const { return drugInfo.selected_way; }
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