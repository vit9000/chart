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
			{ L'm', L'�' },
			{ L'k', L'�' },
			{ L'g', L'�' },
			{ L'n', L'�' },
			{ L'l', L'�' },
			{ L'L', L'�' },
			{ L'M', L'�' },
			{ L'-', L' ' },
			{ L'+', L' ' },
			{ L'\\', L'/' }
		};

		// ��������� ������
		size_t i = 0;
		preprocessed_string.reserve(DrugForm.size() * 2);
		wstring& lu = preprocessed_string;
		for (i; i<DrugForm.size(); i++)
		{
			auto& c = DrugForm[i];
			if (c == L'�')
			{

				break;
			}
			auto new_c = c;
			if (replacement.count(c) > 0)
				new_c = replacement[c];

			if (new_c == L'/')
			{
				if (i + 1 < DrugForm.size() && ((DrugForm[i + 1] == L'�') || (DrugForm[i + 1] == L'�')))
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
		//������ ������ �� ������� (��, ��, ��/��)
		const wstring& lu = preprocessed_string;
		
		for (size_t i = 0; i < lu.size(); i++)
		{
			double val;
			wstring ed;
			if (isDigit(lu[i]))// ����� �������� ��������
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
			if (!isDigit(lu[i]))// ����� �������� ������ (��, ��, ��/��)
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
		���������
		%
		��/��
		��
		����
		��
		*/
		
		// ������ ��������� ������� ��
		// ���� % - ������ �������
		wstring perc_str = L"%";
		if(result.count(perc_str)>0)
		{
			auto& p = result.at(perc_str);
			if (p.size() > 0)
				percent = p.at(0);
			result.erase(perc_str);
		}
		// ������ � ��� ��
		{
			wsmatch m;
			#pragma warning(push)
			#pragma warning(disable: 4129)
			wregex r_ml(L"[m�]?[l�]{1}"); // L, mL
			#pragma warning(pop)
			pair<wstring, vector<double>> volume;
			for(auto& it : result)
			{	
				if (regex_match(it.first, r_ml))
				{
					volume = it;
					result.erase(it.first);// ������� ������
				}
			}

			if (percent > 0)// ���� ������ ������� - �� ��� �������
			{
				//���� �������������� �����
				if(volume.second.size() > 0)
					dose = volume.second[0];
				else dose = 1.0;// ����� ����� 1 ��, ���� ����� ��� � ������ ������
				ED = volume.first; // ��� ��� ������� - ������� ��������� ��������
				return; // ������� ������ ����������� - ������� � % � ��
			}
			else if (volume.second.size() > 0) //���� ������� ��,� - ���� �������
			{
				if (volume.second.size() > 1) // ���� ��������� �������� ��,� -�� ����� ��������� %
				{
					// �������� ���������� �������

					// �������� ��

					// ����������� %
					//percent = ;
					
					//���������� ������������ ����� ��� ����
					//dose = max;//volume.second[0];
				}
				else
				{
					dose = volume.second[0];
				}
				
				ED = volume.first; // ��� ��� ������� - ������� ��������� ��������

			}
			
			
		}
		//����� ������������� �� ���������





		/*
		wsmatch m;
		#pragma warning(push)
		#pragma warning(disable: 4129)
		wregex r_mg(L"[0-9]+[\.,]?[0-9]*.?[m��]?[�]?[g�]{1}"); // g, mg, mcg
		wregex r_ml(L"[0-9]+[\.,]?[0-9]*.?[m�]?[l�]{1}"); // L, mL
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
	//��� - ��������, ��������, ����
	wstring type;
	//����� �������� �� ����� �������
	double percent;
	double dose;
	wstring ED;
	wstring drug_form;
	//���� �������� ���������
	int selected_way;
	wstring selected_way_name;
	//���������� 
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