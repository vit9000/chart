#pragma once
#include <map>
#include <string>
#include <vector>
#include <regex>
#include "DrugInfo.h"

#define rectal_suppositories L"����.����."
#define tab L"��."
#define flacon L"����."
#define salve L"����/����/����"

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
		{ L'm', L'�' },
		{ L'k', L'�' },
		{ L'c', L'�' },
		{ L'a', L'�' },
		{ L'p', L'�' },
		{ L's', L'�' },
		{ L'g', L'�' },
		{ L'r', L'�' },
		{ L'n', L'�' },
		{ L'l', L'�' },
		{ L'L', L'�' },
		{ L'M', L'�' },
		{ L'-', L' ' },
		//{ L'+', L' ' },
		{ L'\\', L'/' }
		};

		data[tab] = wregex(L".*[�t��c]{1}[��ab�]{1}[��\.bp]?[�\.s]?.*");//��., ���., ��., ����
		data[flacon] = wregex(L".*[�]{1}[�]{1}[.�]?[�]?.*"); //
		data[rectal_suppositories] = wregex(L".*[�]{1}[��]{1}[.�]{1}[�]?.*"); //
		data[salve] = wregex(L".*[���]{1}[���]{1}[���]{1}[��]?.*");

		adminways[tab].setOn(ADMINWAY::ENTERAL);
		adminways[flacon].setAllOn();
		adminways[rectal_suppositories].setOn(ADMINWAY::RECTAL);
		adminways[salve].setOn(ADMINWAY::EXTERNAL);
		adminways[L"�/�"].setOn(ADMINWAY::INTRAVENOUS_DROPS);
		adminways[L"�/�"].setOn(ADMINWAY::INTRAVENOUS_BOLUS);
		adminways[L"�/�"].setOn(ADMINWAY::INTRAVENOUS_INFUSION);
		adminways[L"�/�"].setOn(ADMINWAY::INTRAMUSCULAR);
		adminways[L"�/�"].setOn(ADMINWAY::SUBCUTANEOUS);
		
	}

	
	ParserDrugFrom(int ID, const wstring& Name, const wstring& DrugForm)
		: drug(ID, Name)
	{
		buidDicts();

		drug.temp = wstring(DrugForm);

		GetReservedED(DrugForm);
		wstring lu;
		wstring clearedString = DrugForm;
		ClearFromGarbage(clearedString); // ������� ����� �� ������ ����������
		preprocess(clearedString, lu); // ������ ������ � �������� �������� � ����������� ����� ������ ��� ����� �������

		if (IsNotNeedParsingDrug()) // ������ ������� �� ������� �������� - ����, �����
			return;

		map<wstring, vector<double>> result;
		parse_prerocessed_string(lu, result);

		if (result.size() == 0 && !reservedED.empty())
		{
			SetAsReserved();
			return;
		}
		// ������ ��������� ������� ��
		else if (Volume(result))
			return;
		//����� ������������� �� ��������
		else if (OtherDrug(result))
			return;
		else if (MgDrug(result))
			return;
		else // ���� ������ ���� "���. 0.05 **"
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
						drug.ED = L"��";
						drug.dose = it->second[0] * 1000;
					}
					else
					{
						drug.ED = L"�";
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
		

		// ��������� ������
		size_t i = 0;
		preprocessed_string.reserve(DrugForm.size() * 2);
		drug.drug_form.reserve(DrugForm.size());
		wstring& lu = preprocessed_string;
		for (i; i<DrugForm.size(); i++)
		{
			auto& c = DrugForm[i];
			if (c == L'�') break; // ����� ����� � ���� �������������� ��� ����������

			wchar_t new_c = c;
			if (replacement.count(c) > 0)
				new_c = replacement.at(c);
			drug.drug_form += new_c;

			if (new_c == L'/')// ���� ����� /, �� ���� ��������� ���� �� ����� �����, �� ���� ��������� 1
			{
				wchar_t next_char = L' ';
				size_t j = i;
				while (j++ < DrugForm.size() && DrugForm[j]==L' ')
				{}
				next_char = (replacement.count(DrugForm[j]) > 0)? replacement[DrugForm[j]] : DrugForm[j];
				
				//if (!isDigit(next_char))
				if(next_char == L'�' || next_char == L'�')
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
		//������ ������ �� ������� (��, ��, ��/��)
		const wstring& lu = preprocessed_string;

		double combined_val = -1;
		for (size_t i = 0; i < lu.size(); i++)
		{
			double val = 0;
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

			while (i < lu.size() && (lu[i] == L' ' || lu[i] == L'+')) 
			{
				if (lu[i] == L'+') 
					combined_val = val;
				i++;
			}
				
			if (!isDigit(lu[i]))// ����� �������� ������ (��, ��, ��/��)
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
			if (MgDrug(result) && drug.dose > 0)
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
	bool MgDrug(map<wstring, vector<double>>& result)
	{
		#pragma warning(push)	
		#pragma warning(disable: 4129)
		wregex r_dry(L"[���]{1}[���]?[�]?"); // mg, gr,
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
		if (drug_it.second.size() > 0)
		{
			if (drug_it.first[0] == L'�' && drug_it.first[1] == L'�')
				drug_it.first = L"�";

			if (drug_it.second.size() > 1)
			{
				// ��������������� �������� (10 �� + 20 ��)
				// ������� ��������� ����� ����, ����
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
		// ����� ��������� 
		#pragma warning(push)	
		#pragma warning(disable: 4129)
		wregex r_dry(L"[���]{1}[���]{1}[���]?"); // ���, ���, �, ���
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
		// ���� �����, �� ����������
		if (reservedED == rectal_suppositories)
		{
			SetAsReserved();
			return true;
		}
		else if (reservedED == salve)
		{
			reservedED = L"�";
			SetAsReserved();
			return true;
		}
		return false;
	}
	//---------------------------------------------------
	void GetReservedED(const wstring& DrugForm) // ���������� ��� �������������� ����� ���� ����� ������� - ��������, � �������� � ��������������� ���������
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
		vector<wstring> garbage{ L"*", L"�/�", L"�/�", L"�/�", L" � " };
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
*/
