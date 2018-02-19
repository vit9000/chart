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
