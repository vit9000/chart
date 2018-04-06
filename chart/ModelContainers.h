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
		: ContainerUnit(BlockName, DrugInfo(Name)) {}
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
	ContainerIVbolus(const wstring& BlockName, const DrugInfo& drug_Info)
		: ContainerUnitMovable(BlockName, drug_Info)
	{
		if (!drugInfo.isSolution())
			MakeSolution(L"20");
	}
};

class ContainerIM : public ContainerUnitMovable
{
public:
	ContainerIM(const wstring& BlockName, const DrugInfo& drug_Info)
		: ContainerUnitMovable(BlockName, drug_Info)
	{
		if (!drugInfo.isSolution())
			MakeSolution(L"5");
	}
};

class ContainerSubcutaneusly : public ContainerUnitMovable // subcutaneusly
{
public:
	ContainerSubcutaneusly(const wstring& BlockName, const DrugInfo& drug_Info)
		: ContainerUnitMovable(BlockName, drug_Info)
	{
		if (!drugInfo.isSolution())
			MakeSolution(L"1");
	}
};
//----------------------------------------------------------------------

/*---------------------------------------------------------------------
-------------------         RESIZABLE           -----------------------
----------------------------------------------------------------------*/
class ContainerIVdrops : public ContainerUnitResizable
{
public:
	ContainerIVdrops(const wstring& BlockName, const DrugInfo& drug_Info, bool allowedMakeSolution = true)
		: ContainerUnitResizable(BlockName, drug_Info)
	{
		if ((allowedMakeSolution) && (!drugInfo.isSolution() || drugInfo.getDoseNumber() < 100))
			MakeSolution(L"100");
	}
};
//---------------------------------------------------------------------
class ContainerInfusion : public ContainerUnitResizable
{
public:
	ContainerInfusion(const wstring& BlockName, const DrugInfo& drug_Info)
		: ContainerUnitResizable(BlockName, drug_Info)
	{
		MakeSolution(L"50");
	}
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


};
//----------------------------------------------------------------------
