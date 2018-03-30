#pragma once
#include <string>
#include <vector>
using namespace std;

struct DrugInfo
{
	DrugInfo() {}

	DrugInfo(const wstring& Name)
		:name(Name)
	{
	}

	DrugInfo(const wstring& DBname, const vector<wstring>& init) 
		: dbname(DBname),
		name (init[0]),
		type (init[1]),
		percent (init[2]),
		dose (init[3]),
		ED (init[4]),
		admin_ways(init[5])
	{
		
	}

	

	vector<wstring> getVector()
	{
		vector<wstring> v;
		v.push_back(name);
		v.push_back(type);
		v.push_back(percent);
		v.push_back(dose);
		v.push_back(ED);
		v.push_back(admin_ways);
		return v;
	}

	wstring dbname;
	wstring name;
	wstring type;
	wstring percent;
	wstring dose;
	wstring ED;
	wstring admin_ways;

	wstring volume;
	
	
	

	wstring getFullName()
	{
		if (type.empty())
		{
			auto pos = dbname.find(L" ¹");
			if (pos > 0 && pos < dbname.size())
				return dbname.substr(0, pos);
			return dbname;// +L" " + type;
		}
		wstring full = name + wstring(L" ") + type;
		if (!dose.empty())
			full += wstring(L" (") + ((!percent.empty()) ? (percent + wstring(L"% ")) : L"") + dose + wstring(L" ") + ED + wstring(L")");
		return full;
	}

};