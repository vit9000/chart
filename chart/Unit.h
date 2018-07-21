#pragma once

#define MAX_MINUTE 1440

#include "Value.h"

class Unit
{
protected:
	std::wstring db_id;
	Value value;
	int start;
	int duration;
	int status;
public:
	enum {NOT_COMPLETE=0, COMPLETE=1, CANCELLED=2};
	enum {MIN_DURATION=30, MAX_DURATION=1440};
	Unit()
		: value(L""), start(0), duration(MIN_DURATION), status(0)
	{}
	Unit(double new_value, int Start, int Duration)
		: value(new_value), start(Start), duration(Duration), status(0)
	{}
	Unit(const wstring& new_value, int Start, int Duration)
		: value(new_value), start(Start), duration(Duration), status(0)
	{}
	Unit(Value new_value, int Start, int Duration)
		: value(new_value), start(Start), duration(Duration), status(0)
	{}
	Unit(wstring unit_id, Value new_value, int Start, int Duration, int Status)
		: db_id(unit_id), value(new_value), start(Start), duration(Duration), status(Status)
	{}
	inline const wstring& getDB_ID() const { return db_id; }
	inline void setDB_ID(const wstring& new_id) { db_id = new_id; }
	inline Value getValue() const { return value; }
	inline void setValue(double NewValue) { value = Value(NewValue); }
	inline void setValue(Value NewValue) { value = NewValue; }
	
	inline void setCompleted(int _status) { status = _status; }
	wstring getStatusStr() const { wstringstream ss; ss << status; return ss.str(); }
	inline int getStatus() const { return status; }
	inline bool isCompleted() const { return status==COMPLETE; }

	inline void setStart(int _start) { start = _start; }
	inline int getStart() const { return start; }
	wstring getStartStr() const { wstringstream ss; ss << start; return ss.str(); }
	inline int getDuration() const { return duration; }
	wstring getDurationStr() const { wstringstream ss; ss << duration; return ss.str(); }
	inline void setDuration(int Duration) { duration = Duration; }
	inline bool isEmpty() const { return value.isEmpty(); }

	bool isFullyEqual(const Unit& rhs)
	{
		return ((*this)==rhs && value==rhs.getValue());
	}

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
