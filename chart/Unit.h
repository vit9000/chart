#pragma once

#define MAX_MINUTE 1440

#include "Value.h"

class Unit
{
protected:
	Value value;
	int start;
	int duration;
public:
	enum {MIN_DURATION=30, MAX_DURATION=1440};
	Unit(double new_value, int Start, int Duration)
		: value(new_value), start(Start), duration(Duration)
	{}
	Unit(const wstring& new_value, int Start, int Duration)
		: value(new_value), start(Start), duration(Duration)
	{}
	Unit(Value new_value, int Start, int Duration)
		: value(new_value), start(Start), duration(Duration)
	{}
	
	Value getValue() const { return value; }
	void setValue(double NewValue) { value = Value(NewValue); }
	void setValue(Value NewValue) { value = NewValue; }
	
	void setStart(int _start) { start = _start; }
	int getStart() const { return start; }
	int getDuration() const { return duration; }
	void setDuration(int Duration) { duration = Duration; }

	friend bool operator<(const Unit& lhs, const Unit& rhs) 
	{
		return lhs.start < rhs.start;
	}

	friend bool operator>(const Unit& lhs, const Unit& rhs) 
	{
		return lhs.start+lhs.duration > rhs.start+lhs.duration;
	}

	friend bool operator==(const Unit& lhs, const Unit& rhs)
	{
		return (lhs.start == rhs.start) && (lhs.duration == rhs.duration);
	}

	friend bool operator<(int lhs, const Unit& rhs)
	{
		return lhs < rhs.start;
	}
	friend bool operator<(const Unit& lhs, int rhs)
	{
		return lhs.start+lhs.duration < rhs;
	}

	friend bool operator>(int lhs, const Unit& rhs)
	{
		return lhs > rhs.start+rhs.duration;
	}

	friend bool operator>(const Unit& lhs, int rhs)
	{
		return lhs.start > rhs;
	}

	friend bool operator==(int lhs, const Unit& rhs)
	{
		return (lhs >= rhs.start) && (lhs<=rhs.start+rhs.duration);
	}

	friend bool operator==(const Unit& lhs, int rhs)
	{
		return (rhs >= lhs.start) && (rhs <= lhs.start + lhs.duration);
	}

};
