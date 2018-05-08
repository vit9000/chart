#pragma once
#include <map>
#include <fstream>
#include <sstream>
#include <future>
#include "utils.h" 

using namespace std;


#include "SQL.h"
#include "DBDrugDialog.h"
#include "DrugInfo.h"

class Parser
{
private:
	map<wstring, wstring> dict;
	vector<wstring> EDs;

	map<wstring, void (Parser::*)(const wstring& str, DrugInfo& drugInfo)const> func_dict;
	
public:
	Parser();
	~Parser() {}
	
	
public:
	
	void load();
	void ParseName(const wstring& str, DrugInfo& drugInfo) const;
	void ParseName(const wstring& str, wstring& result) const;
	void ParseED(const wstring& str, DrugInfo& drugInfo) const;
	void ParseType(const wstring& str, DrugInfo& drugInfo) const;
	bool parse(const wstring& input_string,  DrugInfo& drug) const;

	bool isValidString(const wstring& str) const
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

	bool isDose(int letter) const
	{
		return (letter >= 48 && letter <= 57) || letter==46 || letter==44;
	}
	bool isDigit(int letter) const
	{
		return (letter >= 48 && letter <= 57);
	}
	
	vector<wstring> convert(const wstring& string) const;

	
	

};

