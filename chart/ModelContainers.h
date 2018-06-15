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
	ContainerParameter(const ID& _id, const wstring& Name)
		: ContainerUnit(_id, DrugInfo(Name)) {
		//type = PARAMETER__NUMBER;
	}
};
//----------------------------------------------------------------------
class ContainerTextParameter : public ContainerParameter
{
public:
	ContainerTextParameter(const ID& _id, const wstring& Name)
		: ContainerParameter(_id, Name)
	{
		//type = PARAMETER__TEXT;
	}
	wstring getSumm() const override
	{
		return L"";
	}

	virtual bool isDigit() const override { return false; }
};
/*---------------------------------------------------------------------
-------------------         MOVABLE         ---------------------------
----------------------------------------------------------------------*/
class ContainerSolution : public ContainerUnitMovable
{
public:
	ContainerSolution(const ID& _id, const DrugInfo& drug_Info)
		: ContainerUnitMovable(_id, drug_Info)
	{
		//type = DRUG__IV_BOLUS;
		if (!drugInfo.isSolution())
			MakeSolution(L"2");
	}
};
/*
class Container : public ContainerUnitMovable
{
public:
	ContainerIM(const ID& _id, const DrugInfo& drug_Info)
		: ContainerUnitMovable(_id, drug_Info)
	{
		//type = DRUG__IM;
		if (!drugInfo.isSolution())
			MakeSolution(L"5");
	}
};

class ContainerSubcutaneusly : public ContainerUnitMovable // subcutaneusly
{
public:
	ContainerSubcutaneusly(const ID& _id, const DrugInfo& drug_Info)
		: ContainerUnitMovable(_id, drug_Info)
	{
		//type = DRUG__SUBCUTANEUSLY;
		if (!drugInfo.isSolution())
			MakeSolution(L"1");
	}
};*/
//----------------------------------------------------------------------

/*---------------------------------------------------------------------
-------------------         RESIZABLE           -----------------------
----------------------------------------------------------------------*/
class ContainerIVdrops : public ContainerUnitResizable
{
public:
	ContainerIVdrops(const ID& _id, const DrugInfo& drug_Info, bool allowedMakeSolution = true)
		: ContainerUnitResizable(_id, drug_Info)
	{
		//type = DRUG__IV_DROPS;
		if ((allowedMakeSolution) && (!drugInfo.isSolution() || drugInfo.getDoseNumber() < 100))
			MakeSolution(L"100");
	}
	wstring getUnitDetails(int unit_number) const override
	{
		if (!isUnitNumberValid(unit_number)) return L"";

		wstringstream ss;
		auto& unit = units[unit_number];
		double dose_vol = unit.getValue().getDoubleValue();
		for (auto& c : childs)
		{
			if(c->getDrugInfo().ED == L"мл")
				dose_vol+=c->getUnit(unit_number).getValue().getDoubleValue();
		}

		double rate = dose_vol / unit.getDuration();
		
		ss << setprecision(2) << rate << L" " << drugInfo.ED << L"/мин. (" << static_cast<int>(rate*20.) << L" кап./мин.)";
		return ss.str();
	}
};
//---------------------------------------------------------------------
class ContainerInfusion : public ContainerUnitResizable
{
public:
	ContainerInfusion(const ID& _id, const DrugInfo& drug_Info, double patientWeight)
		: ContainerUnitResizable(_id, drug_Info), weight(patientWeight)
	{
		//type = DRUG__INFUSION;
		MakeSolution(L"50");
		drugInfo.ED += L"/час";
		changeStatusAvailable = false;
	}

	virtual wstring getUnitDetails(int unit_number) const
	{
		if (!isUnitNumberValid(unit_number)) return L"";

		wstringstream ss;
		auto& unit = units[unit_number];
		double rate = drugInfo.getPercentNumber() * 10.;// перевели процент в мг в 1 мл
		rate *= 1000.; // перевели в мкг в 1 мл
		double ml = unit.getValue().getDoubleValue();
		rate *= ml / 60. ; // перевели в мкг в N мл/мин
		rate /= weight;
		
		ss << ml << L" мл/час (" << fixed << setprecision(2) << rate << L" " << L"мкг/кг/мин.)";
		return ss.str();
	}
private:
	double weight;
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
