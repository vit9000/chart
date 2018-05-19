#pragma once
#include <string>
#include <sstream>
#include "Serializable.h"
using namespace std;

struct DBPatient : public Serializable
{

	wstring name;
	class BloodType
	{
	private:
		byte ABO_;
		byte Rh_;
	public:
		BloodType() : ABO_(0), Rh_(0) {}
		BloodType(byte ABO, byte Rh) : ABO_(ABO), Rh_(Rh) {}
		operator wstring()
		{
			wstringstream ss;
			switch (ABO_)
			{
			case 0:
				ss << L"0(I)";
				break;
			case 1:
				ss << L"A(II)";
				break;
			case 2:
				ss << L"B(III)";
				break;
			case 3:
				ss << L"AB(IV)";
				break;
			}
		
			ss << L"Rh";
			ss << wstring((Rh_ == 1) ? L"+" : L"-");
			
			return ss.str();
		}
	} blood_type;
	wstring age;
	double weight;
	int case_number;
	int patient_number;
	wstring code;

	bool Deserialize(const JSON_Value& value) override
	{
		if (!value.IsObject()) return false;
		name = value[L"fio"].GetString();
		age = value[L"age"].GetString();
		case_number = value[L"st_num"].GetInt();
		patient_number = value[L"num"].GetInt();
		code = value[L"code"].GetString();
		
		weight = 70;
		return true;
	}

	bool Serialize(JSON_Value& value, JSON_Allocator& allocator) override
	{
		return false;
	}
};