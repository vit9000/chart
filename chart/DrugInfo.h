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
		#pragma warning(suppress: 4129)
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

	inline bool isDigit(const wchar_t& c)
	{
		return ((int)c >= L'0' && (int)c <= L'9');	
	}


	void parse(const wstring& str, double& value, wstring& ed)
	{
		size_t i = str.size()-1;
		for (i; i >= 0; i--)
		{
			auto& c = str[i];
			if (isDigit(c) || c == L'\\' || c == L'/' || c == L' ')
				break;
		}
		
		ed = str.substr(i+1);
		if (str[i] != L' ') i++;
		wstring temp = str.substr(0, i);
		wstringstream ss(temp);
		ss >> value;
		if (value<=0)
			value = 1;

	}



	DrugInfo(int ID, const wstring& Name, const wstring& lu)
		: name(Name), id(ID)
	{
		
		map<wstring, double> res;

		for (size_t i = 0; i < lu.size(); i++)
		{
			double val;
			wstring ed;
			if (isDigit(lu[i]))
			{
				size_t start = i;
				while (i < lu.size() && (isDigit(lu[i]) || lu[i] == L'.' || lu[i] == L','))
				{
					i++;
				}
				val = _wtof(lu.substr(start, i-start).c_str());
			}
			else continue;

			if (!isDigit(lu[i]))
			{
				if (lu[i] == L'№')
					break;
				if(lu[i]==L' ')
					i++;
				size_t start = i;
				while (i < lu.size() && lu[i] != L' ')
				{
					if (isDigit(lu[i]))/*обработка 125мг/5мл */
					{
						i--;
						if (lu[i] == L'\\')
							i--;
						break;
					}
					i++;
				}
				ed = lu.substr(start, i-start);
			}
			if(ed.size()>0)
				res[ed] = val;
		}
		int temp = 1;



		/*
		wsmatch m;
		#pragma warning(push)
		#pragma warning(disable: 4129)
		wregex r_mg(L"[0-9]+[\.,]?[0-9]*.?[mмн]?[к]?[gг]{1}"); // g, mg, mcg
		wregex r_ml(L"[0-9]+[\.,]?[0-9]*.?[mм]?[lл]{1}"); // L, mL
		wregex r_perc(L"[0-9]+[\.,]?[0-9]*.?\%"); // %
		#pragma warning(pop)
		
		
		double ml(0), mg(0);
		wstring mg_ED, ml_ED;

		if (std::regex_search(lu, m, r_perc) && m.size()>0)
		{
			wstring temp;
			parse(m[0].str(), percent, temp);
			//percent = (m[0].str());	
		}

		if(std::regex_search(lu, m, r_mg) && m.size()>0)
		{ 
			//mg = extractValue(m[0].str());
			parse(m[0].str(), mg, mg_ED);
		}

		if (std::regex_search(lu, m, r_ml) && m.size()>0)
		{
			//ml = extractValue(m[0].str());
			parse(m[0].str(), ml, ml_ED);
		}
		if (ml > 0)
		{
			if (mg > 0)
			{
				percent = mg / ml * 0.1;
				// добавить проверку на граммы и литры. По умолчанию мг и мл
			}
			dose = ml;
			ED = ml_ED;//!!!!!!
		}
		else ED = mg_ED;
		
		*/
	}

	
	int id;
	//name
	wstring name;
	//тип - таблетки, растворы, мазь
	wstring type;
	//можно получить из формы выпуска
	double percent;
	double dose;
	wstring ED;
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