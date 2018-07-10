#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <atlstr.h>

class PatientInfo : public std::vector<std::wstring>
{

public:
	enum { FIO, AGE, NUM, ST_NUM, CODE, PROF_DEP, DIAGNOS, DOCTOR, WEIGHT, HEIGHT, VISITID};

	explicit PatientInfo()
	{
		resize(11);
	};

	explicit PatientInfo(const std::wstring& fio, const std::wstring& age, const std::wstring& num, const std::wstring& st_num,
		const std::wstring& code, const std::wstring& prof_dep, const std::wstring& diagnos, const std::wstring& doctor, const std::wstring& visit_id)
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
		(*this)[8] = L"70";
		(*this)[9] = L"170";
		(*this)[10] = visit_id;
	};

	bool is_empty()
	{
		return (*this)[FIO].empty();
	}

};