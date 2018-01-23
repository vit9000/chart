#pragma once

#define MAX_MINUTE 1440

enum STATUS_STATE {UNAVALABLE=-1, UNCOMPLETE=0, COMPLETE=1};

class Unit
{
private:
	double value;
	size_t start; // in min
	size_t duration; // in min
	int status; // 0 - uncomplete, 1 - complete, -1 - unavalable
public:
	Unit(double Value, size_t Start, size_t Duration)
		: value(Value), start(Start), duration(Duration), status(UNCOMPLETE)
	{}
	
	double getValue() const { return value; }
	void setValue(double NewValue) { value = NewValue; }
	
	size_t getStart() const { return start; }
	void setStart(size_t NewStart) 
	{ 
		if(NewStart >= MAX_MINUTE) return; 
		start = NewStart;
	}

	size_t getDuration() const { return duration; }
	void setDuration(size_t NewDuration) 
	{ 
		if(NewDuration >= MAX_MINUTE) return; 
		duration = NewDuration;
	}

	int getStatus() const { return status; }
	void setStatus(int NewStatus) 
	{
		if(NewStatus<-1 || NewStatus>1) return; 
		status = NewStatus;
	}
};