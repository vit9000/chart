#include "stdafx.h"
#include "Parser.h"




Parser::Parser()
{
}
void Parser::load()
{
	dict[L"���"] = L"��������";
	dict[L"��"] = L"��������";
	dict[L"����"] = L"�������";
	dict[L"���"] = L"�������";
	dict[L"����"] = L"�������";
	dict[L"�-�"] = L"�������";
	dict[L"�/�"] = L"�������";
	dict[L"�/�"] = L"�������";
	dict[L"��"] = L"�������";
	dict[L"��"] = L"������������";
	dict[L"����"] = L"������������";
	dict[L"���"] = L"�������";
	dict[L"����"] = L"�������";
	dict[L"����"] = L"����";
	dict[L"��.����"] = L"����";
	dict[L"����"] = L"����";
	dict[L"����"] = L"����";
	dict[L"����"] = L"����";
	dict[L"���������"] = L"����";
	dict[L"���"] = L"����";
	dict[L"�����"] = L"�����"; 
	dict[L"��"] = L"�����";
	dict[L"�����"] = L"�����"; 
	dict[L"�����"] = L"�����";
	dict[L"���"] = L"��������";
	dict[L"���"] = L"�������";
	EDs.push_back(L"��");
	EDs.push_back(L"��");
	EDs.push_back(L"���");
	EDs.push_back(L"�");
	EDs.push_back(L"%");
	EDs.push_back(L"��/��");

	for (const auto& it : dict)
	{
		func_dict[it.first] = &Parser::ParseType;
	}
	for (const auto& ed : EDs)
	{
		func_dict[ed] = &Parser::ParseED;
	}


	
}
//---------------------------------------------------------------------------------------
vector<wstring> Parser::convert(const wstring& str_) const
{
	vector<wstring> result;
	// ������� �10 � �.�.
	wstring str(str_);
	auto pos = str.find(L" �");
	if (pos > 0 && pos < str.size())
		str = str.substr(0, pos);

	size_t start = 0;
	auto end = start;
	for (auto i = start+2; i < str.size()-1; i++)
	{
		if (isDigit(str[i]) != isDigit(str[i + 1]))
		{
			end = i;
		}
		if (start != end)
		{
			wstring substr = str.substr(start, end - start);
			result.emplace_back(substr);
			start = end;
			end = start;
		}
		
	}
	wstring substr = str.substr(start, str.size() - start);
	result.emplace_back(substr);
	return result;

}

bool Parser::parse(const wstring& input, DrugInfo& drug) const
{


//	drug.dbname = input;
	//�������� ��� ���������
	ParseName(input, drug);
	if (drug.name.empty())
		return false;
	//����� ������������ ��� �����

	if (drug.name.size() > input.size()) return true;


	wstring updated = input.substr(drug.name.size(), input.size());
	convert(updated);
	//drug.name = updated;// temp
	return true;

	/*
	wstringstream ss(updated);
	
	const size_t not_set = 100000;

	while (ss)
	{
		wstring str;
		ss >> str;
		if (str.size() == 0) continue;
		// ����������� ������� ������
		
		// ���� ��� ���� � ������, ������� ����� ������
		if (*(str.end()-1)==L'.')
			str = str.substr(0, str.size() - 1);
		//���� ������� �������, ���������
		if (func_dict.count(str) > 0)
		{
			(this->*func_dict.at(str))(str, drug);
			continue;
		}
		//���� ������� �� �������, ���������� �������������
		
		for (const auto& ed : EDs)
		{
			auto pos = str.find(ed);
			if (pos > 0 && pos <= str.size())
			{
				wstring com = str.substr(0, pos+ed.size());
				(this->*func_dict.at(ed))(com, drug);
			}
		}
		
		
		
		//wstring g = drug.ED;
	}*/
	return true;
}
//---------------------------------------------------------------------------------------
void Parser::ParseED(const wstring& str, DrugInfo& drugInfo) const
{
	/*
	for (const auto& ed : EDs)
	{
		auto pos = str.find(ed);
		if (pos >= 0 && pos < str.size())
		{
			
			auto start = pos;
			while (start-- > 0)
			{
				
				if (!isDose(str[start]))
					break;
				
			}
			if (start >= str.size()) start=0;
			//start++;
			if (!isDigit(str.at(start)))
				start++;
			if (ed == L"%")
			{
				drugInfo.type = L"�������";
				drugInfo.percent = str.substr(start, pos - start);
			}
			else if (ed == L"��/��")
			{
				drugInfo.type = L"�������";
				wstringstream ss(str.substr(start, pos - start));
				double p;
				ss >> p;
				if (!ss) continue;
				ss.clear();
				ss << p / 10.;
				drugInfo.percent = ss.str();
			}
			else if (ed == L"��")
			{
				drugInfo.type = L"�������";
				//drugInfo.volume = str.substr(start, pos - start);
			}
			else
				drugInfo.dose = str.substr(start, pos - start);
			//drugInfo.ED = ed;
			
			return;
		}
	}
	*/
		return;
}
//---------------------------------------------------------------------------------------
void Parser::ParseType(const wstring& str, DrugInfo& drugInfo)const
{

	
	drugInfo.type = dict.at(str);
	
}
//---------------------------------------------------------------------------------------
void Parser::ParseName(const wstring& name, wstring& result) const
{
	auto isValidString = [this](const wstring& str) -> bool
	{
		for (const auto& letter : str)
		{
			if (!((letter >= 47 && letter <= 57) ||
				(letter >= 1040 && letter <= 1071) ||
				letter == 1025 || letter == 45))
				return false;

		}
		return true;

	};
	wstringstream ss(name);
	wstring temp;
	bool first_run = true;
	while (ss)
	{
		ss >> temp;

		if (!isValidString(temp))
			break;
		if (!first_run)
			result += L" ";
		result += temp;
		first_run = false;
	}
}
void Parser::ParseName(const wstring& name, DrugInfo& drugInfo) const
{
	ParseName(name, drugInfo.name);
}
//---------------------------------------------------------------------------------------




