
#pragma once
#include <utility>
#include <string>
#include <sstream>
#include <Windows.h>
using std::wstring;
using std::wstringstream;
using std::pair;

class Ini
{
private:
	wstring file_name_;
public:
	Ini(wstring file_name)
	{	
		wchar_t buffer[MAX_PATH];
		GetCurrentDirectory(sizeof(buffer), buffer);
		file_name_ = (wstring)buffer + (wstring)L"\\" + file_name;
	}

	void Delete()
	{
		DeleteFile(file_name_.c_str());
	}

	/*bool IsExists()
	{
		return PathFileExists(file_name_.c_str());
	}*/

	void Write(const wstring& section, const wstring& field, const wstring& value)
	{
		WritePrivateProfileString(section.c_str(), field.c_str(), value.c_str(), file_name_.c_str());
	}

	template <typename T>
	void Write(const wstring& section, const wstring& field, const T& value)
	{
		stringstream ss;
		ss << value;
		WritePrivateProfileString(section.c_str(), field.c_str(), ss.str().c_str(), file_name_.c_str());
	}

	wstring Read(const wstring& section, const wstring& field, const wstring& default_value)
	{
		wchar_t out[256];
		
		DWORD dd = GetPrivateProfileString(
				section.c_str(), field.c_str(), default_value.c_str(),
				out,
				256,
				file_name_.c_str());
		return wstring(out);
	}

	
	template <typename T>
	T Read(const wstring& section, const wstring& field, const T& default_value)
	{
		wstringstream ss;
		ss << default_value;// конвертируем T в строку 
		wstring str_result = Read(section, field, ss.str());
		if (ss.str() == str_result) return default_value;//если совпало

		ss = wstringstream(str_result);
		double result;
		ss >> result;// string to T
		if (ss)// если без ошибок
			return static_cast<T>(result);
		else return default_value;//если с ошибкой, то возвращаем значение по умолчанию
	};

	pair<wstring, int> ReadPair(const wstring& section, const wstring& field, pair<wstring, int>& default_value)
	{
		wstringstream ss;
		ss << default_value.first << L"," << default_value.second;// конвертируем T в строку 
		wstring str_result = Read(section, field, ss.str());
		if (ss.str() == str_result) return default_value;//если совпало

		ss = wstringstream(str_result);
		pair<wstring, int> result;

		wchar_t t[100];
		if (ss.getline(t, 100, L','))
			result.first = t;
		else
			return default_value;
		if (ss.getline(t, 100, L','))
		{
			wstringstream s(t);
			s >> result.second;
		}
		else
			return default_value;
		return result;
	}


	

};