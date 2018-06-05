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
public:
	
	ParserDrugFrom(int ID, const wstring& Name, const wstring& DrugForm)
		: drug(ID, Name)
	{
		wstring lu;
		preprocess(DrugForm, lu);
		map<wstring, vector<double>> result;
		parse_prerocessed_string(lu, result);

		// ������ ��������� ������� ��
		if (Volume(result))
			return;
		//����� ������������� �� ��������
		else
			DryDrug(result);

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
			if (c == L'�') break; // ����� ����� � ���� �������������� ��� ����������

			wchar_t new_c = c;
			if (replacement.count(c) > 0)
				new_c = replacement.at(c);

			if (new_c == L'/')// ���� ����� /, �� ���� ��������� ���� �� ����� �����, �� ���� ��������� 1
			{
				wchar_t next_char = L' ';
				int j = i;
				while (j++ < DrugForm.size() && (next_char = DrugForm[j]) == L' ')
				{
				}
				if (!isDigit(next_char))
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
		//������ ������ �� ������� (��, ��, ��/��)
		const wstring& lu = preprocessed_string;

		for (size_t i = 0; i < lu.size(); i++)
		{
			double val;
			wstring ed;
		RESTART:
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

			while (i < lu.size() && lu[i] == L' ') { i++; }
				
			if (!isDigit(lu[i]))// ����� �������� ������ (��, ��, ��/��)
			{

				size_t start = i;
				while (i < lu.size() && lu[i] != L' ')
				{
					i++;
				}
				ed = lu.substr(start, i - start);
			}
			else goto RESTART;
			if (ed.size()>0)
			{
				result[ed].push_back(val);
			}

		}
	}
	//---------------------------------------------------
	bool Volume(map<wstring, vector<double>>& result)
	{
		// ���� % - ������ �������
		wstring perc_str = L"%";
		if (result.count(perc_str)>0)
		{
			auto& p = result.at(perc_str);
			if (p.size() > 0)
				drug.percent = p.at(0);
			result.erase(perc_str);
		}
		// ������ � ��� ��
		#pragma warning(push)	
		#pragma warning(disable: 4129)
		wregex r_ml(L"[�]?[�]{1}"); // L, mL
		#pragma warning(pop)
		pair<wstring, vector<double>> volume;
		for (auto& it : result)
		{
			if (std::regex_match(it.first, r_ml))
			{
				volume = it;
				result.erase(it.first);// ������� ������
				break;
			}
		}

		if (drug.percent > 0)// ���� ������ ������� - �� ��� �������
		{
			//���� �������������� �����
			if (volume.second.size() > 0)
				drug.dose = volume.second[0];
			else drug.dose = 1.0;// ����� ����� 1 ��, ���� ����� ��� � ������ ������
			drug.ED = volume.first; // ��� ��� ������� - ������� ��������� ��������
			return true; // ������� ������ ����������� - ������� � % � ��
		}
		else if (volume.second.size() > 0) //���� ������� ��,� - ���� �������
		{
			// �������� ���������� �������
			std::sort(volume.second.begin(), volume.second.end());
			double& min_ml = volume.second[0];
			double& max_ml = volume.second[volume.second.size() - 1];
			// �������� �� � ������������ �������
			if (DryDrug(result) && drug.dose > 0)
			{
				double temp_dry = drug.dose;

				if (drug.ED == L"�")
					temp_dry *= 1000;
				else if (drug.ED == L"���")
					temp_dry /= 1000;

				double temp_volume = min_ml;
				if (volume.first == L"�")
					temp_volume *= 1000;

				drug.percent = temp_dry / temp_volume * 0.1;/// ���� ������ ��, � � �.�.
			}
			//���������� ������������ ����� ��� ����
			drug.dose = max_ml;


			drug.ED = volume.first; // ��� ��� ������� - ������� ��������� ��������
			return true;
		}
		return false;
	}
	//---------------------------------------------------
	bool DryDrug(map<wstring, vector<double>>& result)
	{
#pragma warning(push)	
#pragma warning(disable: 4129)
		wregex r_dry(L"[���]?[��]{1}[�r�]?"); // mg, gr,
#pragma warning(pop)
		pair<wstring, vector<double>> drug_it;
		for (auto& it : result)
		{
			if (regex_match(it.first, r_dry))
			{
				drug_it = it;
				result.erase(it.first);// ������� ������
				break;
			}

		}
		if (drug_it.second.size() > 1)
		{
			// ��������������� �������� (10 �� + 20 ��)
			// ������� ��������� ����� ����, ����
			drug.dose = 0;
			//ED = L"����.";//!!!!!!!!!!!!!!!!!!!!!!!!!
			//return true;
		}
		else if (drug_it.second.size() > 0)
		{
			drug.dose = drug_it.second[0];
			drug.ED = drug_it.first;
			return true;
		}

		for (auto& it : result)
		{
			if (it.second.size()>0)
				drug.dose = it.second[0];
			drug.ED = it.first;
			result.erase(it.first);// ������� ������
			break;
		}

		return false;
	}
	//---------------------------------------------------
	
};

