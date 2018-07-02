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
	ContainerSolution(const ID& _id, const DrugInfo& drug_Info, bool allowMakeSolution=true)
		: ContainerUnitMovable(_id, drug_Info)
	{
		//type = DRUG__IV_BOLUS;
		if (allowMakeSolution && !drugInfo.isSolution())
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
		if (allowedMakeSolution && (!drugInfo.isSolution() || drugInfo.getDoseNumber() < 100))
			MakeSolution(L"100");
	}
	wstring getUnitDetails(int unit_number) const override
	{
		if (!isUnitNumberValid(unit_number)) return L"";

		wstringstream ss;
		auto& unit = getUnit(unit_number);
		double dose_vol = unit.getValue().getDoubleValue();
		for (auto& c : childs)
		{
			if(c->getDrugInfo().ED == L"��")
				dose_vol+=c->getUnit(unit_number).getValue().getDoubleValue();
		}

		double rate = dose_vol / unit.getDuration();
		
		ss << setprecision(2) << rate << L" " << drugInfo.ED << L"/���. (" << static_cast<int>(rate*20.) << L" ���./���.)";
		return ss.str();
	}
};
//---------------------------------------------------------------------
class ContainerInfusion : public ContainerUnitResizable
{
public:
	ContainerInfusion(const ID& _id, const DrugInfo& drug_Info, double patientWeight, bool allowedMakeSolution = true)
		: ContainerUnitResizable(_id, drug_Info), weight(patientWeight)
	{
		//type = DRUG__INFUSION;
		if (allowedMakeSolution)
		{
			MakeSolution(L"50");
			drugInfo.ED += L"/���";
			changeStatusAvailable = false;
		}
	}

	virtual wstring getUnitDetails(int unit_number) const
	{
		if (!isUnitNumberValid(unit_number)) return L"";

		wstringstream ss;
		auto& unit = getUnit(unit_number);
		double rate = drugInfo.getPercentNumber() * 10.;// �������� ������� � �� � 1 ��
		rate *= 1000.; // �������� � ��� � 1 ��
		double ml = unit.getValue().getDoubleValue();
		rate *= ml / 60. ; // �������� � ��� � N ��/���
		rate /= weight;
		
		ss << ml << L" ��/���";
		if(rate > 0.001)
			ss << L" (" << fixed << setprecision(2) << rate << L" ���/��/���.)";
		return ss.str();
	}
private:
	double weight;
protected:


	void calculateSumm() override
	{
		summ = 0;
		for (auto& unit : units)
		{
			summ += (unit.second.getValue().getDoubleValue()/60.)*unit.second.getDuration();
		}
		summ = std::round(summ * 10) / 10;
	}


};
//----------------------------------------------------------------------
