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

/*
thread t([this]()
{
	SQL sql;
	sql.Connect();
	//vector<wstring> result;
	//if (sql.SendRequest(L"SELECT * FROM admin_ways"))

	//	sql.RecieveNextData(result);
	setlocale(LC_ALL, "UTF-8");
	ifstream in;

	wstring prev;
	in.open("drugs_122.txt");

	int i = 0;
	while (in)
	{

		char c_str[256];
		in.getline(c_str, 256);

		wstring str(std::move(StringConverter(c_str)));
		if (str == prev)
			continue;
		prev = str;

		wstringstream ss;
		i++;
		ss << L"INSERT INTO med122 VALUES (" << i << L", '" << str << L"');";
		wstring request = ss.str();

		if (!sql.SendRequest(request))
			continue;


		DrugInfo drug;
		if (!parse(str, drug))
			continue;
		data[drug.getFullName()] = drug;


	}
});
t.detach();
*/