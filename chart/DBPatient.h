#pragma once
#include <string>
#include <sstream>
using namespace std;

struct DBPatient
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
	int age;
	double weight;
	int case_number;
	int patient_number;


};