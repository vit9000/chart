#pragma once

#include <vector>
#include <string>
using namespace std;
class Value
{
private:
	vector<double> values;
public:
	Value()
	{
		values.push_back(0);
	}
	Value(double val)
	{
		values.push_back(val);
	}
	Value(const vector<double>& val)
	{
		values = vector<double>(val);
	}

	operator double() const
	{
		if (values.size() > 0)
			return values.at(0);
		return 0.;
	}
	
	double operator[](size_t index) const
	{
		if (index >= values.size())
			throw invalid_argument("Value operator[]");
		return values.at(index);
	}

	Value& operator =(const vector<double>& rhs)
	{
		values = vector<double>(rhs);
		return *this;
	}

	const vector<double>& getValues() const
	{
		return values;
	}

	inline size_t size() const
	{
		return values.size();
	}
};