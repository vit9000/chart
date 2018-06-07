#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <regex>
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
		selected_way(-1),
		dilution_dose(0)
	{}

	DrugInfo(const wstring& Name)
		:name(Name),
		percent(0),
		dose(0),
		selected_way(-1),
		dilution_dose(0)
	{
	}

	DrugInfo(int ID, const wstring& Name)
		:id(ID), name(Name),
		percent(0),
		dose(0),
		selected_way(-1),
		dilution_dose(0)
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
	//тип - таблетки, растворы, мазь
	wstring type;
	//можно получить из формы выпуска
	double percent;
	double dose;
	wstring ED;
	wstring drug_form;
	//путь введения выбранный
	int selected_way;
	wstring selected_way_name;
	//разведение 
	double dilution_dose;
	wstring dilution_ed;

	wstring dilution;

	
	
	bool isExistsInDB() const
	{
		return !type.empty();
	}

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

	wstring getFullName() const
	{
		if (!isExistsInDB())
		{	
			return name;// +L" " + type;
		}
		wstring full = name + wstring(L" ") + type;
		
		if (dose>0)
			full += wstring(L" (") + (isSolution() ? (ToString(percent) + wstring(L"% ")) : L"") + ToString(dose) + wstring(L" ") + ED + wstring(L")");
		return full;
	}

};