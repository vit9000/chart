#pragma once
#include <map>
#include <string>
#include <vector>
#include <regex>
#include "DrugInfo.h"

using namespace std;

class ParserDrugFrom
{
	DrugInfo drug;
	wstring reservedED;

public:
	
	ParserDrugFrom(int ID, const wstring& Name, const wstring& DrugForm)
		: drug(ID, Name)
	{
		
		GetReservedED(DrugForm);
		wstring lu;
		//надо добавить поиск путей введения "в/в" "в/м" с удалением их из строки
		// ...
		preprocess(DrugForm, lu);
		map<wstring, vector<double>> result;
		parse_prerocessed_string(lu, result);

		// сперва оцениваем раствор ли
		if (Volume(result))
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
				SetAsReserved();
				return;
			}
			

		

		}

		int temp = 1;

	}
	//-----------------------------------------------------
	operator DrugInfo()
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
		map<wchar_t, wchar_t> replacement
		{
			{ L',', L'.' },
		{ L'm', L'м' },
		{ L'k', L'к' },
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

		// обработка строки
		size_t i = 0;
		preprocessed_string.reserve(DrugForm.size() * 2);
		wstring& lu = preprocessed_string;
		for (i; i<DrugForm.size(); i++)
		{
			auto& c = DrugForm[i];
			if (c == L'№') break; // после знака № идет неинтересующая нас информация

			wchar_t new_c = c;
			if (replacement.count(c) > 0)
				new_c = replacement.at(c);

			if (new_c == L'/')// если стоит /, то надо проверить есть не число после, то надо подствить 1
			{
				wchar_t next_char = L' ';
				int j = i;
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
		drug.drug_form = DrugForm.substr(0, i);
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
		if (drug_it.second.size() > 1)
		{
			// комбинированный препарат (10 мг + 20 мг)
			// единица измерения будет табл, флак
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
	bool OtherDrug(map<wstring, vector<double>>& result)
	{
		#pragma warning(push)	
		#pragma warning(disable: 4129)
		wregex r_dry(L"[дмт]{1}[олы]{1}[знс]?"); // доз, млн
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

	void GetReservedED(const wstring& DrugForm) // вызывается для резервирования такой типы формы выпуска - например, у таблеток с комбинированным введением
	{
		#pragma warning(push)	
		#pragma warning(disable: 4129)
		map<wstring, wregex> data;
		data[L"таб."] = wregex(L".*[тt]{1}[абab]{1}[б\.b]?[\.]?.*");//wregex (L"[тt]{1}[абab]{1}[б.b]?[.]?"); //
		data[L"флак."] = wregex(L".*[ф]{1}[л]{1}[.а]?[к]?.*"); //
		#pragma warning(pop)
		pair<wstring, vector<double>> volume;
		for (auto& it : data)
		{
			if (std::regex_match(DrugForm, it.second))
			{
				reservedED = it.first;
				break;
			}
		}
	}

};

