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
	ContainerParameter(const wstring& BlockName, const wstring& Name) 
		: ContainerUnit(BlockName, Name, L"") {}
};
//----------------------------------------------------------------------
class ContainerTextParameter : public ContainerParameter
{
public:
	ContainerTextParameter(const wstring& BlockName, const wstring& Name)
		: ContainerParameter(BlockName, Name)
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
	ContainerIVbolus(const wstring& BlockName, const wstring& Name)
		: ContainerUnitMovable(BlockName, Name, L"мг")
	{}
};
//----------------------------------------------------------------------
class ContainerTabs : public ContainerUnitMovable
{
public:
	ContainerTabs(const wstring& BlockName, const wstring& Name)
		: ContainerUnitMovable(BlockName, Name, L"мг")
	{}
};
/*---------------------------------------------------------------------
-------------------         RESIZABLE           -----------------------
----------------------------------------------------------------------*/
class ContainerIVdrops : public ContainerUnitResizable
{
public:
	ContainerIVdrops(const wstring& BlockName, const wstring& Name)
		: ContainerUnitResizable(BlockName, Name, L"мл")
	{}
};
//----------------------------------------------------------------------
#include "ValueInputDlg.h"
class ContainerIVinfusion : public ContainerUnitResizable
{
public:
	ContainerIVinfusion(const wstring& BlockName, const wstring& Name)
		: ContainerUnitResizable(BlockName, Name, L"мл/час")
	{
		ValueInputDlg dlg;
		dlg.Init(Name, { L"Доза в шприце", L"Объем шприца" }, {L"", L"50"});
		if (dlg.DoModal() == IDOK)
		{
			vector<Value> val = dlg.getValue();
			dose = val[0];
			volume = val[1];
			measure_unit = L"[" + wstring(dose)+L" мг/" + wstring(volume) + L" мл] " + measure_unit;
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
