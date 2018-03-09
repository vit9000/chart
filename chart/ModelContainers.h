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
ContainerHemodynamics ContainerIVbolus ContainerTabs	ContainerIVdrops	ContainerIVinfusion
*/



//----------------------------------------------------------------------
class ContainerParameter : public ContainerUnit
{
public:
	ContainerParameter(const wstring& Name) : ContainerUnit(Name, L"") {}
};
//----------------------------------------------------------------------
class ContainerHemodynamic : public ContainerParameter
{
public:
	ContainerHemodynamic(const wstring& Name)
		: ContainerParameter(Name)
	{}
};

class ContainerTextParameter : public ContainerParameter
{
public:
	ContainerTextParameter(const wstring& Name)
		: ContainerParameter(Name)
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
	ContainerIVbolus(const wstring& Name)
		: ContainerUnitMovable(Name, L"לד")
	{}
};
//----------------------------------------------------------------------
class ContainerTabs : public ContainerUnitMovable
{
public:
	ContainerTabs(const wstring& Name)
		: ContainerUnitMovable(Name, L"לד")
	{}
};
/*---------------------------------------------------------------------
-------------------         RESIZABLE           -----------------------
----------------------------------------------------------------------*/
class ContainerIVdrops : public ContainerUnitResizable
{
public:
	ContainerIVdrops(const wstring& Name)
		: ContainerUnitResizable(Name, L"לכ")
	{}
};
//----------------------------------------------------------------------
class ContainerIVinfusion : public ContainerUnitResizable
{
protected:
	void calculateSumm() override
	{
		summ = 0;
		for (const Unit& unit : units)
		{
			summ += (unit.getValue().getDoubleValue()/60.)*unit.getDuration();
		}
		summ = std::round(summ * 10) / 10;
	}
public:
	ContainerIVinfusion(const wstring& Name)
		: ContainerUnitResizable(Name, L"לכ/קאס")
	{}

};
//----------------------------------------------------------------------
