#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <regex>
using namespace std;


template <typename T>
wstring ToString(const T& t)
{
	wstringstream ss;
	ss << t;
	return ss.str();
}

struct DrugInfo
{
	DrugInfo() {}

	DrugInfo(const wstring& Name)
		:name(Name)
	{
	}


	double extractValue(const wstring& str)
	{
		wregex r(L"[0-9]+[\.,]?[0-9]*");
		wsmatch m;
		if (regex_search(str, m, r) && m.size()>0)
		{
			wstringstream ss(m[0].str());
			double temp=0;
			ss >> temp;
			return temp;
		}
		else return 1.0;
	}

	DrugInfo(int ID, const wstring& Name, const wstring& lu)
		: name(Name), id(ID)
	{
		// parsing
		/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		 Ќ”∆Ќќ –≈јЋ»«ќ¬ј“№ ѕј–—≈–
		 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
		wsmatch m;
		wregex r_mg(L"[0-9]*[\.,]?[0-9]*.?[mмн]?[к]?[gг]{1}"); // g, mg, mcg
		wregex r_ml(L"[0-9]*[\.,]?[0-9]*.?[mм]?[lл]{1}"); // L, mL
		wregex r_perc(L"[0-9]+[\.,]?[0-9]*.?\%"); // %
		
		double ml, mg;

		if (std::regex_search(lu, m, r_perc) && m.size()>0)
		{
			percent = extractValue(m[0].str());	
		}

		if(std::regex_search(lu, m, r_mg) && m.size()>0)
		{ 
			mg = extractValue(m[0].str());
			
		}

		if (std::regex_search(lu, m, r_ml) && m.size()>0)
		{
			ml = extractValue(m[0].str());
			
		}
		if (ml > 0)
		{
			if (mg > 0)
			{
				percent = mg / ml * 0.1;
				// добавить проверку на граммы и литры. ѕо умолчанию мг и мл
			}
			dose = ml;
			ED = L"мл";//!!!!!!
		}


//		percent = L"50";
//		dose = L"2";
		ED = L"мл";
	}

	
	int id;
	//name
	wstring name;
	//тип - таблетки, растворы, мазь
	wstring type;
	//можно получить из формы выпуска
	double percent=0;
	double dose;
	wstring ED;
	//путь введени€ выбранный
	int selected_way;
	wstring selected_way_name;
	//разведение 
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