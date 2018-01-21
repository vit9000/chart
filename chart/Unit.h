#pragma once

class Unit
{
private:
	double dose;
	int start_pos; // in min
	int duration; // in min
public:
	Unit(double Dose, int StartPos, int Duration)
		: dose(Dose), start_pos(StartPos), duration(Duration)
	{}


};