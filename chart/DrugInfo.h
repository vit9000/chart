#pragma once
#include <string>
#include <vector>
#include <sstream>
using namespace std;

struct DrugInfo
{
	DrugInfo() {}

	DrugInfo(const wstring& Name)
		:name(Name)
	{
	}


	DrugInfo(const wstring& Name, const wstring& lu)
		: name(Name)
	{
		// parsing
		/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		 ����� ����������� ������
		 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
		percent = L"50";
		dose = L"2";
		ED = L"��";
	}


	DrugInfo(const wstring& DBname, const vector<wstring>& init) 
		:
		name (init[0]),
		type (init[1]),
		percent (init[2]),
		dose (init[3]),
		ED (init[4])
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
		return v;
	}

	//name
	wstring name;
	//��� - ��������, ��������, ����
	wstring type;
	//����� �������� �� ����� �������
	wstring percent;
	wstring dose;
	wstring ED;
	//���� �������� ���������
	int selected_way;
	wstring selected_way_name;
	//���������� 
	wstring dilution;
	
	bool isExistsInDB() const
	{
		return !type.empty();
	}

	bool isSolution() const
	{
		if (!percent.empty() && percent != L"0")
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
		return (isSolution()) ? percent + L"% " : L"";;
	}

	double getPercentNumber() const
	{
		wstringstream ss(percent);
		double temp = 0;
		ss >> temp;
		return temp;
	}

	double getDoseNumber() const
	{
		wstringstream ss(dose);
		double temp=0;
		ss >> temp;
		return temp;
	}

	wstring getFullName() const
	{
		if (!isExistsInDB())
		{	
			return name;// +L" " + type;
		}
		wstring full = name + wstring(L" ") + type;
		if (!dose.empty())
			full += wstring(L" (") + (isSolution() ? (percent + wstring(L"% ")) : L"") + dose + wstring(L" ") + ED + wstring(L")");
		return full;
	}

};