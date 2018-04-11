#pragma once

#include <vector>
#include <string>
#include <sstream>
using namespace std;

class Value
{
private:
	wstring value;
public:
	enum {EMPTY=-1000};
	Value()
	{
		//values.push_back(L"");
	}
	Value(double val)
	{
		wstringstream ss;
		ss << val;
		value = ss.str();
	}
	Value(const wstring& val)
	{
		value = val;
	}
	

	const wstring& getValue() const
	{
		return value;
	}

	const wstring& getString() const
	{
		return value;
	}

	operator wstring() const
	{
		return getValue();
	}

	


	Value& operator =(const wstring& rhs)
	{
		value = rhs;
		return *this;
	}


	double getDoubleValue() const
	{
		if (value.empty()) return EMPTY;
		double temp = 0;
		wstringstream ss(value);
		ss >> temp;
		return temp;
	}

	inline size_t size() const
	{
		return 1;
	}
	
	friend bool operator==(const Value& lhs, const Value& rhs)
	{
		return lhs.value == rhs.value;
	}

	friend bool operator!=(const Value& lhs, const Value& rhs)
	{
		return lhs.value != rhs.value;
	}

};

/*
class Value
{
private:
	vector<wstring> values;
public:
	Value()
	{
		values.push_back(L"");
	}
	Value(double val)
	{
		wstringstream ss;
		ss << val;
		values.push_back(ss.str());
	}
	Value(const wstring& val)
	{
		values.push_back(val);
	}
	Value(const vector<wstring>& val)
	{
		values = vector<wstring>(val);
	}

	wstring getValue() const
	{
		if (values.size() > 0)
			return values.at(0);
		return L"";
	}

	operator wstring() const
	{
		return getValue();
	}
	
	wstring operator[](size_t index) const
	{
		if (index >= values.size())
			throw invalid_argument("Value operator[]");
		return values.at(index);
	}

	

	Value& operator =(const vector<wstring>& rhs)
	{
		values = vector<wstring>(rhs);
		return *this;
	}

	const vector<wstring>& getValues() const
	{
		return values;
	}

	vector<double> getDoubleValues() const
	{
		vector<double> result;
		for (const wstring& str : values)
		{
			wstringstream ss(str);
			double temp=0;
			ss >> temp;
			result.push_back(temp);
		}
		return result;
	}

	double getDoubleValue() const
	{
		double temp = 0;
		wstringstream ss (getValue());
		ss >> temp;
		return temp;
	}

	inline size_t size() const
	{
		return values.size();
	}

	
};*/