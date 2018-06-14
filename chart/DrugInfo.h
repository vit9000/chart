#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "AdminWay.h"
using namespace std;

//enum DRUG_TYPE {SOLUTION, };

template <typename T>
wstring ToString(const T& t)
{
	wstringstream ss;
	ss << t;
	return ss.str();
}

struct DrugInfo
{
	DrugInfo()
		:percent(0),
		dose(0),
		selected_admin_way(-1)
		//dilution_dose(0)
	{}

	DrugInfo(const wstring& Name)
		:name(Name),
		percent(0),
		dose(0),
		selected_admin_way(-1)
		//dilution_dose(0)
	{
	}

	DrugInfo(int ID, const wstring& Name)
		:id(ID), name(Name),
		percent(0),
		dose(0),
		selected_admin_way(-1)
		//dilution_dose(0)
	{
	}


	bool operator==(const DrugInfo& d)
	{
		
		return 
			(
			(int(this->percent * 1000) == int(d.percent * 1000)) &&
			(this->ED == d.ED) && 
			(this->dose == d.dose));
	}

	wstring temp;
	
	int id;
	//name
	wstring name;
	//можно получить из формы выпуска
	double percent;
	double dose;
	wstring ED;
	wstring drug_form;
	//путь введения выбранный
	int selected_admin_way;
	

	
	

	bool isSolution() const
	{
		//if (!percent.empty() && percent != L"0")
		if(percent>0)
			return true;
		return false;
	}

	/*bool isIVallowed() const
	{
		wstringstream ss(admin_ways);
		int t(0);
		ss >> t;
		return t == 1;
	}*/

	wstring getPercentString() const
	{
		return (isSolution()) ? ToString(percent) + L"% " : L"";;
		
	}

	double getPercentNumber() const
	{
	
		return percent;
	}

	double getDoseNumber() const
	{
		return dose;
	}

	/*

	void SetAllowedAdminWays(const std::vector<int>& switch_on_list)
	{
		allowedAdminWays.setAllOff();
		for (size_t i = 0; i < switch_on_list.size(); i++)
		{
			allowedAdminWays.setOn(i);
		}
	}
	void SetAllowedAdminWays(std::vector<int>& switched_on_list)
	{
		switched_on_list.clear();
		for (int i = 0; i < sizeof(allowedAdminWays); i++)
		{
			if (allowedAdminWays.getStatus(i))
				switched_on_list.push_back(i);
		}
	}*/

};