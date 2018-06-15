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

class DrugInfo
{
public: // variables
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
public: // functions

	DrugInfo()
		:percent(0),
		dose(0),
		selected_admin_way(-1)
	{}
	//--------------------------------------------------------------------------
	DrugInfo(const wstring& Name)
		:name(Name),
		percent(0),
		dose(0),
		selected_admin_way(-1)
	{
	}
	//--------------------------------------------------------------------------
	DrugInfo(int ID, const wstring& Name)
		:id(ID), name(Name),
		percent(0),
		dose(0),
		selected_admin_way(-1)
	{
	}
	//--------------------------------------------------------------------------
	virtual ~DrugInfo() {}
	//--------------------------------------------------------------------------
	bool operator==(const DrugInfo& d)
	{
		
		return 
			(
			(int(this->percent * 1000) == int(d.percent * 1000)) &&
			(this->ED == d.ED) && 
			(this->dose == d.dose));
	}
	//--------------------------------------------------------------------------
	bool isSolution() const
	{
		//if (!percent.empty() && percent != L"0")
		if(percent>0)
			return true;
		return false;
	}
	//--------------------------------------------------------------------------
	wstring getPercentString() const
	{
		return (isSolution()) ? ToString(percent) + L"% " : L"";;
		
	}
	//--------------------------------------------------------------------------
	double getPercentNumber() const
	{
	
		return percent;
	}
	//--------------------------------------------------------------------------
	double getDoseNumber() const
	{
		return dose;
	}
	//--------------------------------------------------------------------------
	bool isEnoughInfo() const
	{
		return (dose !=0 && !ED.empty());
	}

};

class DrugInfoEx : public DrugInfo
{
private:
	ADMINWAY allowedAdminWays;
public:
	DrugInfoEx()
		:DrugInfo(), allowedAdminWays(){}
	//--------------------------------------------------------------------------
	DrugInfoEx(const wstring& Name)
		:DrugInfo(Name), allowedAdminWays() {}
	//--------------------------------------------------------------------------
	DrugInfoEx(int ID, const wstring& Name)
		:DrugInfo(ID, Name), allowedAdminWays()
	{}
	//--------------------------------------------------------------------------
	virtual ~DrugInfoEx() {}
	//--------------------------------------------------------------------------
	void SetAllowedAdminWays(const std::vector<int>& switch_on_list)
	{
		allowedAdminWays.setAllOff();
		for (size_t i = 0; i < switch_on_list.size(); i++)
		{
			allowedAdminWays.setOn(i);
		}
	}
	//--------------------------------------------------------------------------
	void GetAllowedAdminWays(std::vector<int>& switched_on_list) const
	{
		switched_on_list.clear();
		for (int i = 0; i < sizeof(allowedAdminWays); i++)
		{
			if (allowedAdminWays.getStatus(i))
				switched_on_list.push_back(i); // возвращаем доступный путь по номеру бита в allowedAdminWays
		}
	}
	//--------------------------------------------------------------------------
	bool GetAllowedAdminWay(int WAY) const
	{
		return allowedAdminWays.getStatus(WAY);
	}
	//--------------------------------------------------------------------------
	inline bool IsAdminWaysExists() const
	{
		return !allowedAdminWays.IsNull();
	}
	//--------------------------------------------------------------------------
	inline void setADMINWAY(const ADMINWAY& new_aminway)
	{
		allowedAdminWays = new_aminway;
	}
	inline void addADMINWAY(const ADMINWAY& new_aminway)
	{
		allowedAdminWays.add(new_aminway);
	}

};

