#pragma once

#define MAX_MINUTE 1440



class Unit
{
protected:
	double value;
	int start;
	int duration;
public:
	Unit(double Value, int Start, int Duration)
		: value(Value), start(Start), duration(Duration)
	{}
	double getValue() const { return value; }
	void setValue(double NewValue) { value = NewValue; }
	int getStart() const { return start; }
	int getDuration() const { return duration; }

};
