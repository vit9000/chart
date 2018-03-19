#pragma once
#include <map>
#include <fstream>
#include <sstream>
#include <future>

using namespace std;

class Drugstore
{
private:
	typedef map<wstring, wstring> DataType;
	DataType data;

	Drugstore()
	{
		thread t([this]() 
		{
			setlocale(LC_ALL, "UTF-8");
			ifstream in;

			in.open("drugs_122.txt");
			while (in)
			{
				char c_str[256];
				//char c_str[256];
				in.getline(c_str, 256);
				//in >> c_str;
				
				wstring str = StringToWString(c_str);
				if (str[0] == L'\"' || str[0] == L' ' || isLower(str[0]))
					continue;
				wstring name;
				getName(str, name);
				if (name.size() <= 1)
					continue;
				data[name] = str;
			}
		});
		t.detach();
		
		
	}
	bool isDigit(int letter)
	{
		return (letter >= 48 && letter <= 57);
	}

	void getName(const wstring& source, wstring& name)
	{
		int t0 = (int)L'0';
		int t = (int)L'1';
		int t2 = (int)L'9';


		/*auto it = find_if(source.begin(), source.end(), 
			[this](const wchar_t letter) 
		{
			return !isUpper(static_cast<int>(letter)); 
		});
		name = source.substr(0, it - source.begin());*/
		wstringstream ss(source);
		
		int i = 0;
		while (ss)
		{
			wstring temp;
			ss >> temp;;
			if (temp.size() == 0)
				break;
			if (isUpper(temp[0]))// || isDigit(temp[0]))
			{
				name += wstring((i == 0) ? L"" : L" ") + temp;
				++i;
			}
			else return;
		}

	}
public:
	void find(const wstring& str, vector<wstring>& result)
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


	bool isUpper(int letter)
	{
		return ((letter >= 1040 && letter <= 1071) || letter == 1025);
	}
	
	bool isLower(int letter)
	{
		return !isUpper(letter);
	}

	wstring StringToWString(const string& str)
	{
		//CP_ACP 
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		wchar_t *pwText;
		pwText = new wchar_t[dwNum];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pwText, dwNum);
		wstring w = pwText;
		delete[] pwText;
		return w;
	}

public:
	static Drugstore* getInstance()
	{
		static Drugstore drugstore;
		return &drugstore;
	}

	size_t getSize() const
	{
		return data.size();
	}

	const DataType& getData()
	{
		return data;
	}


};