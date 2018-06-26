#pragma once

#define MAX_MINUTE 1440

#include "Value.h"

class Unit
{
protected:
	std::wstring id;
	Value value;
	int start;
	int duration;
	bool completed;
public:
	enum {MIN_DURATION=30, MAX_DURATION=1440};
	Unit()
		: value(L""), start(0), duration(MIN_DURATION), completed(false)
	{}
	Unit(double new_value, int Start, int Duration)
		: value(new_value), start(Start), duration(Duration), completed(false)
	{}
	Unit(const wstring& new_value, int Start, int Duration)
		: value(new_value), start(Start), duration(Duration), completed(false)
	{}
	Unit(Value new_value, int Start, int Duration)
		: value(new_value), start(Start), duration(Duration), completed(false)
	{}
	inline const wstring& getID() const { return id; }
	inline Value getValue() const { return value; }
	inline void setValue(double NewValue) { value = Value(NewValue); }
	inline void setValue(Value NewValue) { value = NewValue; }
	
	inline void setCompleted(bool status) { completed = status; }
	inline bool isCompleted() const { return completed; }

	inline void setStart(int _start) { start = _start; }
	inline int getStart() const { return start; }
	inline int getDuration() const { return duration; }
	inline void setDuration(int Duration) { duration = Duration; }
	inline bool isEmpty() const { return value.getString().empty(); }

	inline friend bool operator<(const Unit& lhs, const Unit& rhs)
	{
		return lhs.start < rhs.start;
	}

	inline friend bool operator>(const Unit& lhs, const Unit& rhs)
	{
		return lhs.start+lhs.duration > rhs.start+lhs.duration;
	}

	inline friend bool operator==(const Unit& lhs, const Unit& rhs)
	{
		return (lhs.start == rhs.start) && (lhs.duration == rhs.duration);
	}

	inline friend bool operator<(int lhs, const Unit& rhs)
	{
		return lhs < rhs.start;
	}
	inline friend bool operator<(const Unit& lhs, int rhs)
	{
		return lhs.start+lhs.duration < rhs;
	}

	inline friend bool operator>(int lhs, const Unit& rhs)
	{
		return lhs > rhs.start+rhs.duration;
	}

	inline friend bool operator>(const Unit& lhs, int rhs)
	{
		return lhs.start > rhs;
	}

	inline friend bool operator==(int lhs, const Unit& rhs)
	{
		return (lhs >= rhs.start) && (lhs<=rhs.start+rhs.duration);
	}

	inline friend bool operator==(const Unit& lhs, int rhs)
	{
		return (rhs >= lhs.start) && (rhs <= lhs.start + lhs.duration);
	}

};
