#pragma once

#include "stdafx.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "utils.h"
#include "Unit.h"
#include "DrugInfo.h"
#include "ValueInputDlg.h"


#include "LogCommand_Units.h"


/*#define PARAMETER__TEXT			L"text"
#define PARAMETER__NUMBER		L"number"

#define DRUG__IV_DROPS			L"iv drops"
#define DRUG__INFUSION			L"infusion"
#define DRUG__IV_BOLUS			L"iv bolus"
#define DRUG__IM				L"im"
#define DRUG__SUBCUTANEUSLY		L"subcuraneusly"
#define DRUG__DEFAULT			L"default"*/
#include <memory>
using namespace std;

class ContainerUnit
{
public:
	
protected:
	//for serialize
	ID id;
	//ID parent_id;
	map<int, Unit> units;
	DrugInfo drugInfo;
	wstring type;//тип для сериализации

	//not for serialize
	ContainerUnit* parent;
	vector<shared_ptr<ContainerUnit>> childs;
	bool changeStatusAvailable;
	double summ;

	void MakeSolution(const wstring& dilution_volume)
	{
		
		wstring ED = drugInfo.getPercentString() + drugInfo.ED;
		


		Value dose;
		Value volume;
		while (1)
		{
			ValueInputDlg dlg;
			dlg.Init(drugInfo.name, { L"Доза", L"Объем разведения" }, { ED, L"мл" }, { ToString(drugInfo.dose), dilution_volume });
			auto result = dlg.DoModal();
			if (result == IDOK)
			{
				vector<Value> val = dlg.getValue();
				dose = val[0];
				volume = val[1];
				if (drugInfo.isSolution())
				{
					if (dose.getDoubleValue() > volume.getDoubleValue())
					{
						MessageBox(0, L"Объем разведения не может быть больше объема разводимого раствора", L"Внимание", MB_OK);
						continue;
					}
					else if (volume.isEmpty() || volume.getDoubleValue() < 0.01)
					{
						MessageBox(0, L"Необходимо ввести объем для разведения раствора", L"Внимание", MB_OK);
						continue;
					}
					else break;
				}
				else break;

			}
			else
			{
				MessageBox(0, L"Необходимо ввести данные для разведения раствора", L"Внимание", MB_OK);
				continue;
			}
		}


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



		drugInfo.drug_form = L"[" + drugInfo.getPercentString() + wstring(dose) + L" " + drugInfo.ED + L"]/" + wstring(volume) + L" мл";

		wstringstream ss;
		double temp = d * (drugInfo.isSolution() ? drugInfo.getPercentNumber() : 1);
		temp /= v;
		if (drugInfo.ED == L"г")
			temp *= 100;
		else if (drugInfo.ED == L"мг")
			temp /= 10;
		else if (drugInfo.ED == L"мкг")
			temp /= 10000;
		drugInfo.percent = temp;

		drugInfo.ED = L"мл";
		drugInfo.dose = v;
	}

	void replaceDB_ID(const Unit& src, Unit& dest)
	{
		dest.setDB_ID(src.getDB_ID());
	}
public:

	virtual void calculateSumm()
	{
		summ = 0;
		for (auto& unit : units)
		{
			double temp = unit.second.getValue().getDoubleValue();
			if (temp != Value::EMPTY)
				summ += unit.second.getValue().getDoubleValue();
		}
	}

	bool isUnitNumberValid(int unit_number) const
	{
		//return (unit_number >=0 && unit_number < static_cast<int>(units.size()));	
		return (units.count(unit_number) > 0);
	}

	bool isDrugContainer() const { return drugInfo.selected_adminWayCode >= 0; }



	ContainerUnit(const ID& _id, const DrugInfo& drug_Info)
		: id(_id),
		parent(nullptr),
		drugInfo(drug_Info),
		summ (0.),
		changeStatusAvailable(false)

		//,type(PARAMETER__NUMBER)
		
	{
	}

	void replaceID(const ID& new_id)
	{
		id = std::move(new_id);
	}

	size_t getChildsCount() const { return childs.size(); }
	inline vector<shared_ptr<ContainerUnit>> getChilds() { return childs; } // копируем

	virtual bool isDigit() const { return true; }
	

	virtual ~ContainerUnit() 
	{
		
	};

	void deleteFromParent()
	{
		if (parent)
		{
			auto& p_childs = parent->childs;
			for (int i = 0; i < static_cast<int>(p_childs.size()); i++)
			{
				if (id == p_childs[i]->getID())
				{
					p_childs.erase(p_childs.begin() + i);
					break;
				}
			}

		}
	}

	void addContainerUnit(const shared_ptr<ContainerUnit>& childContainerUnit)
	{
		if (!childContainerUnit) return;

		childContainerUnit->parent = this;//у дочерних
		childContainerUnit->units.clear();
		childs.push_back(childContainerUnit);
	}

//	inline void rename(const wstring& NewName) { name = NewName; }

	const ID& getID() const
	{
		return id;
	}
	ID getParentID() const
	{
		if(parent) 
			return parent->getID();
		return ID();
	}
	bool isChild() const
	{
		return !(getParentID().isEmpty());
	}

	bool isParent() const
	{
		return childs.size()>0;
	}

	virtual wstring getUnitDetails(int unit_number) const
	{
		if (!isUnitNumberValid(unit_number)) return L"";

		wstringstream ss;
		ss << getUnit(unit_number).getValue().getString() << L"" << drugInfo.ED;
		return ss.str();
	}

	Unit getUnit(int unit_number) const noexcept
	{
		if (units.count(unit_number) > 0)
			return units.at(unit_number);
		else 
			return Unit(L"", unit_number, 1);
	}

	inline bool isChangeStatusAvailable() const
	{
		return changeStatusAvailable;
	}

	void setCompleted(int unit_number, bool status)
	{
		if (!changeStatusAvailable || !isUnitNumberValid(unit_number)) return;
		if (units.count(unit_number) == 0) return;
		units[unit_number].setCompleted(status);
	}

	inline size_t size() const { return units.size(); }

	LogCommandPtr deleteUnit(int unit_number, bool create_log = true)
	{
		if (units.count(unit_number) == 0) return nullptr;
	
		// создаем комманду для бэкапа, если требуется - create_log
		LogCommandPtr log_command = (!create_log) ? nullptr 
			: LogCommandPtr(new LogCommand_DeleteUnit(id, units.at(unit_number)));

		units.erase(unit_number);
		calculateSumm();
		return log_command;
	}

	void addUnits(const map<int, Unit> _units)
	{
		units.insert(_units.begin(), _units.end());
	}

	void loadUnit(const Unit& newUnit)
	{
		units[newUnit.getStart()] = std::move(newUnit);
	}

	virtual LogCommandPtr addUnit(const Unit& NewUnit, bool create_log = true)
	{
		if (NewUnit.isEmpty())
			return nullptr;
		int STEP = config->getStep();
		int start = NewUnit.getStart() / STEP * STEP;
		if (units.count(start) != 0) 
			return nullptr;
		
		Unit&  _unit = units[start];
		_unit = std::move(Unit(NewUnit.getValue(), start, STEP));
		calculateSumm();

		// создаем лог и обратное дейтсвие - удаление
		LogCommandPtr log_command = (!create_log) ? nullptr : createLogCommandAddUnit(_unit);
			
		return log_command;
	}

	

	virtual LogCommandPtr updateUnit(int unit_number, const Unit& updated_unit, bool create_log = true)
	{
		if (units.count(unit_number) == 0) // если не было - добавляем
			return addUnit(updated_unit, create_log);

		// если был юнит
		if (updated_unit.isEmpty())
		{
			if(updated_unit.getDB_ID().empty())
				return deleteUnit(unit_number, create_log);
			else return nullptr;
		}
		
		Unit& _unit = units[unit_number];
		if (_unit.isFullyEqual(updated_unit))
			return nullptr;

		Unit& unit = units[unit_number];
		Unit copy_updated_unit = std::move(updated_unit);
		replaceDB_ID(unit, copy_updated_unit);

		LogCommandPtr log_command = (!create_log) ? nullptr : createLogCommandUpdateUnit(_unit, copy_updated_unit);
		// если не пустой - обновляем
		unit = copy_updated_unit;
		calculateSumm();
		return log_command;
	}
	protected:

		LogCommandPtr createLogCommandAddUnit(const Unit& new_unit)
		{
			return LogCommandPtr(new LogCommand_AddUnit(id, new_unit));
		}

		LogCommandPtr createLogCommandUpdateUnit(const Unit& old_unit, const Unit& updated_unit)
		{
			return LogCommandPtr(new LogCommand_UpdateUnit(id, old_unit, updated_unit));
		}
	public:


	virtual wstring getSumm() const 
	{
		wstringstream ss;
		ss << summ;
		return ss.str();
	}

	const wstring& getMeasureUnit() const { return drugInfo.ED; }
	const DrugInfo& getDrugInfo() const { return drugInfo; }
	int getAdminWay() const { return drugInfo.selected_adminWayCode; }
	const wstring& getName() const { return drugInfo.name;}
	const map<int, Unit> getMapUnits() const
	{
		return units;
	}
	vector<Unit> getUnits() const 
	{
		vector<Unit> vec;
		vec.reserve(units.size());
		for (auto& unit : units)
		{
			vec.push_back(unit.second);
		}
		return vec; 
	}
	
	
	int find(int minute) const
	{
		for (auto& u : units)
		{
			if (u.second == minute)
				return static_cast<int>(u.first);
		}
		return -1;
	}
	//-------------------------------------------------------------------------------------------
	

};