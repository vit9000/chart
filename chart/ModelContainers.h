#pragma once
#include <cmath>
#include "ContainerUnitMovable.h"
#include "ContainerUnitResizable.h"
/*

			_________________ContainerUnit ________________
		  /							|						\
		 /							|						 \
		/							|					      \
ContainerParameter			ContainerMovable				ContainerResizable
		|						/       \						/			\
					 ContainerIVbolus ContainerTabs	ContainerIVdrops	ContainerIVinfusion
*/



//----------------------------------------------------------------------
class ContainerParameter : public ContainerUnit
{
public:
	ContainerParameter(const ID& _id, const wstring& Name) : ContainerUnit(_id, Name, L"") {}
};
//----------------------------------------------------------------------
class ContainerTextParameter : public ContainerParameter
{
public:
	ContainerTextParameter(const ID& _id, const wstring& Name)
		: ContainerParameter(_id, Name)
	{}
	wstring getSumm() const override
	{
		return L"";
	}
};
/*---------------------------------------------------------------------
-------------------         MOVABLE         ---------------------------
----------------------------------------------------------------------*/
class ContainerIVbolus : public ContainerUnitMovable
{
public:
	ContainerIVbolus(const ID& _id, const wstring& Name)
		: ContainerUnitMovable(_id, Name, L"��")
	{}
};
//----------------------------------------------------------------------
class ContainerTabs : public ContainerUnitMovable
{
public:
	ContainerTabs(const ID& _id, const wstring& Name)
		: ContainerUnitMovable(_id, Name, L"��")
	{}
};
/*---------------------------------------------------------------------
-------------------         RESIZABLE           -----------------------
----------------------------------------------------------------------*/
class ContainerIVdrops : public ContainerUnitResizable
{
public:
	ContainerIVdrops(const ID& _id, const wstring& Name)
		: ContainerUnitResizable(_id, Name, L"��")
	{}
};
//----------------------------------------------------------------------
#include "ValueInputDlg.h"
class ContainerIVinfusion : public ContainerUnitResizable
{
public:
	ContainerIVinfusion(const ID& _id, const wstring& Name)
		: ContainerUnitResizable(_id, Name, L"��/���")
	{
		ValueInputDlg dlg;
		dlg.Init(Name, { L"���� � ������", L"����� ������" }, {L"", L"50"});
		if (dlg.DoModal() == IDOK)
		{
			vector<Value> val = dlg.getValue();
			dose = val[0];
			volume = val[1];
			measure_unit = L"[" + wstring(dose)+L" ��/" + wstring(volume) + L" ��] " + measure_unit;
		}
	}
protected:
	Value dose;
	Value volume;

	void calculateSumm() override
	{
		summ = 0;
		for (const Unit& unit : units)
		{
			summ += (unit.getValue().getDoubleValue()/60.)*unit.getDuration();
		}
		summ = std::round(summ * 10) / 10;
	}


};
//----------------------------------------------------------------------
