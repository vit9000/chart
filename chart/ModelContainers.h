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
	{}
};
//----------------------------------------------------------------------

/*---------------------------------------------------------------------
-------------------         RESIZABLE           -----------------------
----------------------------------------------------------------------*/
class ContainerIVdrops : public ContainerUnitResizable
{
public:
	ContainerIVdrops(const wstring& BlockName, const DrugInfo& drug_Info)
		: ContainerUnitResizable(BlockName, drug_Info)
	{}
};
//----------------------------------------------------------------------
#include "ValueInputDlg.h"
class ContainerInfusion : public ContainerUnitResizable
{
public:
	ContainerInfusion(const wstring& BlockName, const DrugInfo& drug_Info)
		: ContainerUnitResizable(BlockName, drug_Info)
	{
		ValueInputDlg dlg;
		dlg.Init(drugInfo.name, { wstring(L"Äמחא (")+drugInfo.ED+wstring(L")"), L"Îבתול (לכ)" }, {L"", L"50"});
		if (dlg.DoModal() == IDOK)
		{
			vector<Value> val = dlg.getValue();
			dose = val[0];
			volume = val[1];
			drugInfo.ED = L"לכ"; 
			drugInfo.dilution =  L"[" + wstring(dose) + L" לד/" + wstring(volume) + L" לכ]";
			drugInfo.dose = volume;
			wstringstream ss(wstring(dose)+wstring(L" ")+wstring(volume));
			double d(-1), v(-1);
			ss >> d >> v;
			if (d > 0 && v > 0)
			{
				ss = wstringstream();
				ss << d / v/ 10.;
				drugInfo.percent = ss.str();
			}
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
