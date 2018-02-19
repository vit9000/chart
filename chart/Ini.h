
#pragma once
#include <string>
#include <sstream>
#include <Windows.h>
using std::wstring;
using std::wstringstream;

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
	T Read(const string& section, const wstring& field, const T& default_value)
	{
		wstringstream ss;
		ss << default_value;// ������������ T � ������ 
		string str_result = Read(section, field, ss.str());
		if (ss.str() == str_result) return default_value;//���� �������

		ss = stringstream(str_result);
		double result;
		ss >> result;// string to T
		if (ss)// ���� ��� ������
			return result;
		else return default_value;//���� � �������, �� ���������� �������� �� ���������
	}
};