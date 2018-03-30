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
	dict[L"фл"] = L"раствор";
	dict[L"св"] = L"суппозитории";
	dict[L"супп"] = L"суппозитории";
	dict[L"пор"] = L"порошок";
	dict[L"лиоф"] = L"порошок";
	dict[L"крем"] = L"крем";
	dict[L"гл.гель"] = L"гель";
	dict[L"мазь"] = L"мазь";
	dict[L"туба"] = L"мазь";
	dict[L"гель"] = L"гель";
	dict[L"эмульгель"] = L"гель";
	dict[L"бан"] = L"гель";
	dict[L"драже"] = L"драже"; 
	dict[L"др"] = L"драже";
	dict[L"пакет"] = L"пакет"; 
	dict[L"сироп"] = L"сироп";
	dict[L"аэр"] = L"аэрозоль";
	dict[L"нап"] = L"напиток";
	EDs.push_back(L"мг");
	EDs.push_back(L"мл");
	EDs.push_back(L"мкг");
	EDs.push_back(L"г");
	EDs.push_back(L"%");
	EDs.push_back(L"мг/мл");

	for (const auto& it : dict)
	{
		func_dict[it.first] = &Drugstore::ParseType;
	}
	for (const auto& ed : EDs)
	{
		func_dict[ed] = &Drugstore::ParseED;
	}


	
}
//---------------------------------------------------------------------------------------
vector<wstring> Drugstore::convert(const wstring& str_) const
{
	vector<wstring> result;
	// убираем №10 и т.д.
	wstring str(str_);
	auto pos = str.find(L" №");
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

bool Drugstore::parse(const wstring& input, DrugInfo& drug) const
{


	drug.dbname = input;
	//получаем имя препарата
	ParseName(input, drug);
	if (drug.name.empty())
		return false;
	//далее обрабатываем без имени

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
		// анализируем входные данные
		
		// если тип идет с точкой, убираем точку вконце
		if (*(str.end()-1)==L'.')
			str = str.substr(0, str.size() - 1);
		//если команда найдена, запускаем
		if (func_dict.count(str) > 0)
		{
			(this->*func_dict.at(str))(str, drug);
			continue;
		}
		//если команда не найдена, продолжаем анализировать
		
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
void Drugstore::ParseED(const wstring& str, DrugInfo& drugInfo) const
{

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
				drugInfo.type = L"раствор";
				drugInfo.percent = str.substr(start, pos - start);
			}
			else if (ed == L"мг/мл")
			{
				drugInfo.type = L"раствор";
				wstringstream ss(str.substr(start, pos - start));
				double p;
				ss >> p;
				if (!ss) continue;
				ss.clear();
				ss << p / 10.;
				drugInfo.percent = ss.str();
			}
			else if (ed == L"мл")
			{
				drugInfo.type = L"раствор";
				//drugInfo.volume = str.substr(start, pos - start);
			}
			else
				drugInfo.dose = str.substr(start, pos - start);
			//drugInfo.ED = ed;
			
			return;
		}
	}
		return;
}
//---------------------------------------------------------------------------------------
void Drugstore::ParseType(const wstring& str, DrugInfo& drugInfo)const
{

	
	drugInfo.type = dict.at(str);
	
}
//---------------------------------------------------------------------------------------
void Drugstore::ParseName(const wstring& name, DrugInfo& drugInfo) const
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

	drugInfo.name = result;
}
//---------------------------------------------------------------------------------------
void Drugstore::find(const wstring& str, vector<wstring>& result)
{
	if (str.size() == 0)
		return;
	result.clear();
	

	SQL sql;
	sql.Connect();
	sql.SendRequest(L"SELECT * FROM med122 WHERE name LIKE '" + str  + wstring(L"%';"));
	int count = sql.CountStrings();
	result = vector<wstring>(sql.CountStrings());
	for (auto& s : result)
		s = sql.RecieveNextData()[1];

}

bool Drugstore::getDrugInfo(const wstring& name, DrugInfo& drugInfo) const
{
	if (!isDrugInfoExists(name, drugInfo))
	{
		parse(name, drugInfo);
		DBDrugDialog dlg;
		dlg.Init(name, drugInfo);
		if (dlg.DoModal() == IDOK)
		{
			return true;
		}
		else return false;
	}
	return true;
}

bool Drugstore::isDrugInfoExists(const wstring& name, DrugInfo& drugInfo) const
{
	SQL sql;
	sql.Connect();
	if (!sql.SendRequest(L"SELECT * FROM drugname_linker,druginfo WHERE drugname_linker.name = '" + name + L"' AND drugname_linker.id=druginfo.id;"))
		return false;

	if (sql.CountStrings() == 0)
		return false;
	auto result = sql.RecieveNextData();
	result.erase(result.begin(), result.begin() + 3);
	drugInfo = DrugInfo(name, result);
	return true;
}

vector<wstring> Drugstore::getAllowedAdminWays(const wstring& name) const
{
	vector<wstring> result;
	SQL sql;
	sql.Connect();
	if (!sql.SendRequest(L"SELECT * FROM drugname_linker,druginfo WHERE drugname_linker.name = '" + name + L"' AND drugname_linker.id=druginfo.id;"))
		return result;
	if (sql.CountStrings() == 0)
		return result;
	vector<wstring> data = sql.RecieveNextData();
	wstringstream ss(data[8]);
	while (ss)
	{
		wstring temp;
		ss >> temp;
		if (temp.empty()) continue;
		wstring request = L"SELECT * FROM admin_ways WHERE id = '" + temp + L"';";
		if (!sql.SendRequest(request))
			return result;
		result.push_back(sql.RecieveNextData()[1]);
		
	}
	return result;
}

