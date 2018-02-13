#pragma once

#define MAX_MINUTE 1440



class Unit
{
protected:
	double value;
public:
	Unit(double Value)
		: value(Value)
	{}
	double getValue() const { return value; }
	void setValue(double NewValue) { value = NewValue; }
};
