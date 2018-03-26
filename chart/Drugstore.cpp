#include "stdafx.h"
#include "Drugstore.h"



Drugstore* Drugstore::instance = nullptr;
Drugstore::DrugstoreDestroyer Drugstore::destroyer;


Drugstore::DrugstoreDestroyer::~DrugstoreDestroyer()
{
	delete ptr;
}
void Drugstore::DrugstoreDestroyer::init(Drugstore *instance)
{
	ptr = instance;
}

Drugstore& Drugstore::getInstance()
{
	if (!instance)
	{
		instance = new Drugstore();
		destroyer.init(instance);
	}
	return *instance;
}

Drugstore::Drugstore()
{
	dict[L"таб"] = L"таблетки";
	dict[L"тб"] = L"таблетки";
	dict[L"капс"] = L"капсулы";
	dict[L"амп"] = L"раствор";
	dict[L"раст"] = L"раствор";
	dict[L"р-р"] = L"раствор";
	dict[L"р/р"] = L"раствор";
	dict[L"в/в"] = L"раствор";
	dict[L"фл"] = L"флаконы";
	dict[L"св"] = L"суппозитории";
	dict[L"супп"] = L"суппозитории";
	dict[L"пор"] = L"порошок";
	dict[L"лиоф"] = L"порошок";
	dict[L"крем"] = L"крем";
	dict[L"мазь"] = L"мазь";
	dict[L"туба"] = L"мазь";
	dict[L"гель"] = L"гель";
	dict[L"бан"] = L"гель";
	dict[L"драже"] = L"драже"; 
	dict[L"др"] = L"драже";
	dict[L"пакет"] = L"пакет"; 
	dict[L"аэр"] = L"аэрозоль";
	dict[L"нап."] = L"напиток";
	EDs.push_back(L"мг");
	EDs.push_back(L"мл");
	EDs.push_back(L"мкг");
	EDs.push_back(L"г");




	thread t([this]()
	{
		setlocale(LC_ALL, "UTF-8");
		ifstream in;

		wstring prev;
		in.open("drugs_122.txt");
		while (in)
		{
			char c_str[256];
			in.getline(c_str, 256);
			
			wstring str (std::move(StringConverter(c_str)));
			if (str == prev)
				continue;
			prev = str;

			DrugInfo drug;
			if (!parse(str, drug))
				continue;
			data[drug.getFullName()] = drug;

		}
	});
	t.detach();


}
//---------------------------------------------------------------------------------------
bool Drugstore::parse(const wstring& input, DrugInfo& drug)
{
	drug.dbname = input;
	drug.name = ParseName(input);
	if (drug.name.empty()) 
		return false;
	drug.type = ParseType(input);
	
	auto p = ParseED(input);
	drug.dose = p.first;
	drug.ED = p.second;
	return true;
}
//---------------------------------------------------------------------------------------
pair<wstring, wstring> Drugstore::ParseED(const wstring& str)
{
	
	for (const auto& ed : EDs)
	{
		auto pos = str.find(ed);
		if (pos >= 0 && pos < str.size())
		{
			
			auto start = pos;
			while (--start > 0)
			{
				if (!isDose(str[start]))
					break;
			}

			start++;
			if (!isDigit(str[start]))
				start++;
			return { str.substr(start, pos-start), ed };
		}
	}
		return { L"",L""};
}
//---------------------------------------------------------------------------------------
wstring Drugstore::ParseType(const wstring& str)
{
	for (const auto& d : dict)
	{
		auto pos = str.find(d.first);
		if (pos >= 0 && pos < str.size())
			return d.second;
	}
	return L"";
}
//---------------------------------------------------------------------------------------
wstring Drugstore::ParseName(const wstring& name)
{
	auto isValidString = [this](const wstring& str) -> bool
	{
		for (const auto& letter : str)
		{
			if (!((letter >= 47 && letter <= 57) ||
				(letter >= 1040 && letter <= 1071) ||
				letter == 1025))
				return false;

		}
		return true;

	};
	wstringstream ss(name);
	wstring result;
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

	return result;
}
//---------------------------------------------------------------------------------------
void Drugstore::find(const wstring& str, vector<wstring>& result)
{
	if (str.size() == 0)
		return;
	result.clear();
	auto itlow = data.lower_bound(str);  // itlow points to b
	wstring s(str);
	s.at(s.size() - 1)++;
	auto itup = data.upper_bound(s);   // itup points to e (not d!)

	for (auto& it = itlow; it != itup; it++)
	{
		result.push_back(it->first);
	}


}

const DrugInfo& Drugstore::getDrugInfo(const wstring& name) const
{
	return data.at(name);
}

