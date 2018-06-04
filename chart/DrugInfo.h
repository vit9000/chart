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

	void preprocess(const wstring& DrugForm, wstring& preprocessed_string)
	{
		map<wchar_t, wchar_t> replacement
		{
			{ L',', L'.' },
			{ L'm', L'м' },
			{ L'k', L'к' },
			{ L'g', L'г' },
			{ L'n', L'н' },
			{ L'l', L'л' },
			{ L'L', L'л' },
			{ L'M', L'М' },
			{ L'-', L' ' },
			{ L'+', L' ' },
			{ L'\\', L'/' }
		};

		// обработка строки
		size_t i = 0;
		preprocessed_string.reserve(DrugForm.size() * 2);
		wstring& lu = preprocessed_string;
		for (i; i<DrugForm.size(); i++)
		{
			auto& c = DrugForm[i];
			if (c == L'№')
			{

				break;
			}
			auto new_c = c;
			if (replacement.count(c) > 0)
				new_c = replacement[c];

			if (new_c == L'/')
			{
				if (i + 1 < DrugForm.size() && ((DrugForm[i + 1] == L'м') || (DrugForm[i + 1] == L'л')))
				{
					lu += L" 1";
					
				}
				new_c = L' ';
			}

			lu += new_c;
		}
		drug_form = DrugForm.substr(0,i);
	}
	//---------------------------------------------------
	void parse_prerocessed_string(const wstring& preprocessed_string, map<wstring, vector<double>>& result)
	{
		//разбор данных по группам (мг, мл, мг/мл)
		const wstring& lu = preprocessed_string;
		
		for (size_t i = 0; i < lu.size(); i++)
		{
			double val;
			wstring ed;
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

			if (lu[i] == L' ')
				i++;
			if (!isDigit(lu[i]))// поиск названия группы (мг, мл, мг/мл)
			{

				size_t start = i;
				while (i < lu.size() && lu[i] != L' ')
				{
					i++;
				}
				ed = lu.substr(start, i - start);
			}
			if (ed.size()>0)
			{
				result[ed].push_back(val);
			}

		}
	}
	//---------------------------------------------------
	DrugInfo(int ID, const wstring& Name, const wstring& DrugForm)
		: name(Name), id(ID)
	{
		wstring lu;
		preprocess(DrugForm, lu);
		map<wstring, vector<double>> result;
		parse_prerocessed_string(lu, result);
		
		/*
		приоритет
		%
		мг/мл
		мл
		дозы
		мг
		*/
		
		// сперва оцениваем раствор ли
		// есть % - значит раствор
		wstring perc_str = L"%";
		if(result.count(perc_str)>0)
		{
			auto& p = result.at(perc_str);
			if (p.size() > 0)
				percent = p.at(0);
			result.erase(perc_str);
		}
		// поищем л или мл
		{
			wsmatch m;
			#pragma warning(push)
			#pragma warning(disable: 4129)
			wregex r_ml(L"[mм]?[lл]{1}"); // L, mL
			#pragma warning(pop)
			pair<wstring, vector<double>> volume;
			for(auto& it : result)
			{	
				if (regex_match(it.first, r_ml))
				{
					volume = it;
					result.erase(it.first);// очищаем строку
				}
			}

			if (percent > 0)// если указан процент - то это раствор
			{
				//ищем соответсвующий объем
				if(volume.second.size() > 0)
					dose = volume.second[0];
				else dose = 1.0;// пусть будет 1 мл, если вдруг нет в записи объема
				ED = volume.first; // так как раствор - единицы измерения объемные
				return; // найдено полное соотвествие - раствор с % и мл
			}
			else if (volume.second.size() > 0) //если найдены мл,л - тоже раствор
			{
				if (volume.second.size() > 1) // если несколько значений мл,л -то можно расчитать %
				{
					// получаем наименьший раствор

					// получаем мг

					// расчитываем %
					//percent = ;
					
					//записываем максимальный объем как доза
					//dose = max;//volume.second[0];
				}
				else
				{
					dose = volume.second[0];
				}
				
				ED = volume.first; // так как раствор - единицы измерения объемные

			}
			
			
		}
		//далее рассматриваеи не расстворы





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
	wstring drug_form;
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