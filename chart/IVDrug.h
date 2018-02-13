#pragma once

#include "Unit.h"

enum STATUS_STATE {UNAVALABLE=-1, UNCOMPLETE=0, COMPLETE=1};

class IVDrug : public Unit
{
protected:
	size_t start; // in min
	size_t duration; // in min
	int status; // 0 - uncomplete, 1 - complete, -1 - unavalable
public:
	IVDrug(double Value, size_t Start, size_t Duration)
		: Unit(Value), start(Start), duration(Duration), status(UNCOMPLETE)
	{}
	
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