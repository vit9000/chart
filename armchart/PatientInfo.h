#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <atlstr.h>

class PatientInfo : public std::vector<CString>
{

public:
	enum { FIO, AGE, NUM, ST_NUM, CODE, PROF_DEP, DIAGNOS, DOCTOR };

	explicit PatientInfo()
	{
		resize(8);
	};


	explicit PatientInfo(const CString& fio, const CString& age, const CString& num, const CString& st_num,
		const CString& code, const CString& prof_dep, const CString& diagnos, const CString& doctor)
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
		return (*this)[FIO].IsEmpty();
	}

	std::wstring getJSONBlock()
	{
		std::wstringstream wss;
		wss << L"\"patient\" : \n{";
		wss << L"\"fio\" : \""		<<	(*this)[FIO].GetBuffer()		<< "\",\n";
		wss << L"\"age\" : \""		<<	(*this)[AGE].GetBuffer()		<< "\",\n";
		wss << L"\"num\" : \""		<<	(*this)[NUM].GetBuffer()		<< "\",\n";
		wss << L"\"st_num\" : \""	<<	(*this)[ST_NUM].GetBuffer()		<< "\",\n";
		wss << L"\"code\" : \""		<<	(*this)[CODE].GetBuffer()		<< "\",\n";
		wss << L"\"diagnos\" : \""	<<	(*this)[DIAGNOS].GetBuffer()	<< "\",\n";
		wss << L"\"doctor\" : \""	<<	(*this)[DOCTOR].GetBuffer()		<< "\"";
		wss << L"},\n";

		return wss.str();
	}
};