#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <atlstr.h>

class PatientInfo : public std::vector<std::wstring>
{

public:
	enum { FIO, AGE, NUM, ST_NUM, CODE, PROF_DEP, DIAGNOS, DOCTOR };

	explicit PatientInfo()
	{
		resize(8);
	};


	

	explicit PatientInfo(const std::wstring& fio, const std::wstring& age, const std::wstring& num, const std::wstring& st_num,
		const std::wstring& code, const std::wstring& prof_dep, const std::wstring& diagnos, const std::wstring& doctor)
	{
		resize(8);
		(*this)[0] = fio;
		(*this)[1] = age;
		(*this)[2] = num;
		(*this)[3] = st_num;
		(*this)[4] = code;
		(*this)[5] = prof_dep;
		(*this)[6] = diagnos;
		(*this)[7] = doctor;
	};

	bool is_empty()
	{
		return (*this)[FIO].empty();
	}

	std::wstring getJSONBlock() const
	{
		std::wstringstream wss;
		wss << L"\"patient\" : \n{";
		wss << L"\"fio\" : \""		<<	(*this)[FIO].c_str()		<< "\",\n";
		wss << L"\"age\" : \""		<<	(*this)[AGE].c_str()		<< "\",\n";
		wss << L"\"num\" : "		<<	(*this)[NUM].c_str()		<< ",\n";
		wss << L"\"st_num\" : "		<<	(*this)[ST_NUM].c_str()		<< ",\n";
		wss << L"\"code\" : \""		<<	(*this)[CODE].c_str()		<< "\",\n";
		wss << L"\"diagnos\" : \""	<<	(*this)[DIAGNOS].c_str()	<< "\",\n";
		wss << L"\"doctor\" : \""	<<	(*this)[DOCTOR].c_str()		<< "\"";
		wss << L"},\n";

		return wss.str();
	}
};