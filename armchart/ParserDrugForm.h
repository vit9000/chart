#pragma once
#include <map>
#include <string>
#include <vector>
#include <regex>
#include "DrugInfo.h"

#define rectal_suppositories L"супп.рект."
#define tab L"шт."
#define flacon L"флак."
#define salve L"мазь/гель/крем"

using namespace std;

class ParserDrugFrom
{
	DrugInfoEx drug;
	wstring reservedED;
	map<wchar_t, wchar_t> replacement;
	map<wstring, wregex> data;
	map<wstring, ADMINWAY> adminways;
	
public:
	void buidDicts()
	{
		replacement =
		{
			{ L',', L'.' },
		{ L'm', L'м' },
		{ L'k', L'к' },
		{ L'c', L'к' },
		{ L'a', L'а' },
		{ L'p', L'п' },
		{ L's', L'с' },
		{ L'g', L'г' },
		{ L'r', L'р' },
		{ L'n', L'н' },
		{ L'l', L'л' },
		{ L'L', L'л' },
		{ L'M', L'М' },
		{ L'-', L' ' },
		//{ L'+', L' ' },
		{ L'\\', L'/' }
		};

		data[tab] = wregex(L".*[тtдкc]{1}[абabр]{1}[пб\.bp]?[с\.s]?.*");//тб., таб., др., капс
		data[flacon] = wregex(L".*[ф]{1}[л]{1}[.а]?[к]?.*"); //
		data[rectal_suppositories] = wregex(L".*[с]{1}[ув]{1}[.п]{1}[п]?.*"); //
		data[salve] = wregex(L".*[мгк]{1}[аер]{1}[зле]{1}[ьм]?.*");

		adminways[tab].setOn(ADMINWAY::ENTERAL);
		adminways[flacon].setAllOn();
		adminways[rectal_suppositories].setOn(ADMINWAY::RECTAL);
		adminways[salve].setOn(ADMINWAY::EXTERNAL);
		adminways[L"в/в"].setOn(ADMINWAY::INTRAVENOUS_DROPS);
		adminways[L"в/в"].setOn(ADMINWAY::INTRAVENOUS_BOLUS);
		adminways[L"в/в"].setOn(ADMINWAY::INTRAVENOUS_INFUSION);
		adminways[L"в/м"].setOn(ADMINWAY::INTRAMUSCULAR);
		adminways[L"п/к"].setOn(ADMINWAY::SUBCUTANEOUS);
		
	}

	
	ParserDrugFrom(int ID, const wstring& Name, const wstring& DrugForm)
		: drug(ID, Name)
	{
		buidDicts();

		drug.temp = wstring(DrugForm);

		GetReservedED(DrugForm);
		wstring lu;
		wstring clearedString = DrugForm;
		ClearFromGarbage(clearedString); // очистка строк от лишней информации
		preprocess(clearedString, lu); // привод строки к удобному парсингу и копирование части строки как форму выпуска

		if (IsNotNeedParsingDrug()) // строки которые не требуют парсинга - крем, свечи
			return;

		map<wstring, vector<double>> result;
		parse_prerocessed_string(lu, result);

		if (result.size() == 0 && !reservedED.empty())
		{
			SetAsReserved();
			return;
		}
		// сперва оцениваем раствор ли
		else if (Volume(result))
			return;
		//далее рассматриваеи не растворы
		else if (OtherDrug(result))
			return;
		else if (MgDrug(result))
			return;
		else // если запись типа "таб. 0.05 **"
		{
			if (result.size() == 1)
			{
				const std::map<std::wstring, std::vector<double>>::iterator& it = result.begin();
				if (it->second.size() > 0)
				{
					if (it->second.size() > 1)
					{
						SetAsReserved();
						return;
					}
					drug.percent = 0.;
					if (it->second[0] < 1)
					{
						drug.ED = L"мг";
						drug.dose = it->second[0] * 1000;
					}
					else
					{
						drug.ED = L"г";
						drug.dose = it->second[0];
					}
				}
				
			}
			else
			{
				//SetAsReserved();
				return;
			}
			

		

		}

		int temp = 1;

	}
	//-----------------------------------------------------
	operator DrugInfoEx()
	{
		return drug;
	}
	//-----------------------------------------------------
	inline bool isDigit(const wchar_t& c)
	{
		return ((int)c >= L'0' && (int)c <= L'9');
	}
	//-----------------------------------------------------
	void preprocess(const wstring& DrugForm, wstring& preprocessed_string)
	{
		

		// обработка строки
		size_t i = 0;
		preprocessed_string.reserve(DrugForm.size() * 2);
		drug.drug_form.reserve(DrugForm.size());
		wstring& lu = preprocessed_string;
		for (i; i<DrugForm.size(); i++)
		{
			auto& c = DrugForm[i];
			if (c == L'№') break; // после знака № идет неинтересующая нас информация

			wchar_t new_c = c;
			if (replacement.count(c) > 0)
				new_c = replacement.at(c);
			drug.drug_form += new_c;

			if (new_c == L'/')// если стоит /, то надо проверить есть не число после, то надо подствить 1
			{
				wchar_t next_char = L' ';
				size_t j = i;
				while (j++ < DrugForm.size() && DrugForm[j]==L' ')
				{}
				next_char = (replacement.count(DrugForm[j]) > 0)? replacement[DrugForm[j]] : DrugForm[j];
				
				//if (!isDigit(next_char))
				if(next_char == L'м' || next_char == L'л')
				{
					lu += L" 1";
				}
				new_c = L' ';
			}

			lu += new_c;
		}
	}
	//---------------------------------------------------
	void parse_prerocessed_string(const wstring& preprocessed_string, map<wstring, vector<double>>& result)
	{
		//разбор данных по группам (мг, мл, мг/мл)
		const wstring& lu = preprocessed_string;

		double combined_val = -1;
		for (size_t i = 0; i < lu.size(); i++)
		{
			double val = 0;
			wstring ed;
		RESTART:
			if (isDigit(lu[i]))// поиск числовых значений
			{
				size_t start = i;
				while (i < lu.size() && (isDigit(lu[i]) || lu[i] == L'.'))
				{
					i++;
				}
				val = _wtof(lu.substr(start, i - start).c_str());
			}
			else continue;

			while (i < lu.size() && (lu[i] == L' ' || lu[i] == L'+')) 
			{
				if (lu[i] == L'+') 
					combined_val = val;
				i++;
			}
				
			if (!isDigit(lu[i]))// поиск названия группы (мг, мл, мг/мл)
			{

				size_t start = i;
				while (i < lu.size() && lu[i] != L' ' && lu[i] != L'+')
				{
					i++;
				}
				ed = lu.substr(start, i - start);
			}
			else
			{
				combined_val = val;
				goto RESTART;
			}
			//if (ed.size()>0)
			{
				if(combined_val>0) result[ed].push_back(combined_val);
				combined_val = -1;
				result[ed].push_back(val);
			}

		}
	}
	//---------------------------------------------------
	bool Volume(map<wstring, vector<double>>& result)
	{
		// есть % - значит раствор
		wstring perc_str = L"%";
		if (result.count(perc_str)>0)
		{
			auto& p = result.at(perc_str);
			if (p.size() > 0)
				drug.percent = p.at(0);
			result.erase(perc_str);
		}
		// поищем л или мл
		#pragma warning(push)	
		#pragma warning(disable: 4129)
		wregex r_ml(L"[м]?[л]{1}"); // L, mL
		#pragma warning(pop)
		pair<wstring, vector<double>> volume;
		for (auto& it : result)
		{
			if (std::regex_match(it.first, r_ml))
			{
				volume = it;
				result.erase(it.first);// очищаем строку
				break;
			}
		}

		if (drug.percent > 0)// если указан процент - то это раствор
		{
			//ищем соответсвующий объем
			if (volume.second.size() > 0)
				drug.dose = volume.second[0];
			else drug.dose = 1.0;// пусть будет 1 мл, если вдруг нет в записи объема
			drug.ED = volume.first; // так как раствор - единицы измерения объемные
			return true; // найдено полное соотвествие - раствор с % и мл
		}
		else if (volume.second.size() > 0) //если найдены мл,л - тоже раствор
		{
			// получаем наименьший раствор
			std::sort(volume.second.begin(), volume.second.end());
			double& min_ml = volume.second[0];
			double& max_ml = volume.second[volume.second.size() - 1];
			// получаем мг и рассчитываем процент
			if (MgDrug(result) && drug.dose > 0)
			{
				double temp_dry = drug.dose;

				if (drug.ED == L"г")
					temp_dry *= 1000;
				else if (drug.ED == L"мкг")
					temp_dry /= 1000;

				double temp_volume = min_ml;
				if (volume.first == L"л")
					temp_volume *= 1000;

				drug.percent = temp_dry / temp_volume * 0.1;/// надо учесть мг, г и т.д.
			}
			//записываем максимальный объем как доза
			drug.dose = max_ml;
			drug.ED = volume.first; // так как раствор - единицы измерения объемные
			return true;
		}
		return false;
	}
	//---------------------------------------------------
	bool MgDrug(map<wstring, vector<double>>& result)
	{
		#pragma warning(push)	
		#pragma warning(disable: 4129)
		wregex r_dry(L"[мнг]{1}[гкр]?[г]?"); // mg, gr,
		#pragma warning(pop)
		pair<wstring, vector<double>> drug_it;
		for (auto& it : result)
		{
			if (regex_match(it.first, r_dry))
			{
				drug_it = it;
				result.erase(it.first);// очищаем строку
				break;
			}

		}
		if (drug_it.second.size() > 0)
		{
			if (drug_it.first[0] == L'г' && drug_it.first[1] == L'р')
				drug_it.first = L"г";

			if (drug_it.second.size() > 1)
			{
				// комбинированный препарат (10 мг + 20 мг)
				// единица измерения будет табл, флак
				SetAsReserved();
				return false;
			}
			drug.dose = drug_it.second[0];
			drug.ED = drug_it.first;
			return true;
		}
		
		return false;
	}
	//---------------------------------------------------
	bool OtherDrug(map<wstring, vector<double>>& result)
	{
		// далее проверяем 
		#pragma warning(push)	
		#pragma warning(disable: 4129)
		wregex r_dry(L"[дмт]{1}[олы]{1}[знс]?"); // доз, млн, т, тыс
		#pragma warning(pop)
		pair<wstring, vector<double>> drug_it;
		for (auto& it : result)
		{
			if (regex_match(it.first, r_dry))
			{
				drug_it = it;
				result.erase(it.first);// очищаем строку
				break;
			}

		}
		if (drug_it.second.size() > 1)
		{
			SetAsReserved();
		}
		else if (drug_it.second.size() > 0)
		{
			drug.dose = drug_it.second[0];
			drug.ED = drug_it.first;
			return true;
		}

		return false;
	}
	//---------------------------------------------------
	void SetAsReserved()
	{
		drug.dose = 1;
		drug.ED = reservedED;
	}
	//---------------------------------------------------
	bool IsNotNeedParsingDrug()
	{
		// если свечи, то количество
		if (reservedED == rectal_suppositories)
		{
			SetAsReserved();
			return true;
		}
		else if (reservedED == salve)
		{
			reservedED = L"г";
			SetAsReserved();
			return true;
		}
		return false;
	}
	//---------------------------------------------------
	void GetReservedED(const wstring& DrugForm) // вызывается для резервирования такой типы формы выпуска - например, у таблеток с комбинированным введением
	{
		#pragma warning(push)	
		#pragma warning(disable: 4129)
		
		#pragma warning(pop)
		pair<wstring, vector<double>> volume;
		for (auto& it : data)
		{
			if (std::regex_match(DrugForm, it.second))
			{
				reservedED = it.first;
				if(adminways.count(reservedED))
					drug.setADMINWAY(adminways.at(reservedED));
				break;
			}
		}
	}
	//----------------------------------------------------------------
	void ClearFromGarbage(wstring& DrugForm)
	{
		vector<wstring> garbage{ L"*", L"в/в", L"в/м", L"п/к", L" и " };
		for (const auto str : garbage)
		{
			auto pos = DrugForm.find(str, 0);
			while (pos != -1)
			{
				DrugForm.erase(pos, str.size());
				pos = DrugForm.find(str, 0);
				if (adminways.count(str) > 0)
					drug.addADMINWAY(adminways.at(str));
			}
			
		}
	}

};

/*
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
*/
