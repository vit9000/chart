#pragma once


class MouseShift
{
public:

	MouseShift(int pos) : start_pos(pos), end_pos(pos), action(-1), direction(0) {}
	int getShift() { if (action == -1) return 0; return end_pos - start_pos; }
	void setStart(int x, int action_, int new_index = -1) { start_pos = x; end_pos = x; action = action_; index = new_index;  }
	void setEnd(int pos)
	{
		if (action != -1)
			end_pos = pos;
		if (start_pos > end_pos)
			direction = 1;
		else if (start_pos < end_pos)
			direction = -1;
		else direction = 0;
	}
	void reset() { start_pos = 0; end_pos = 0; action = -1; index = -1; }
	void resetShift() { start_pos = end_pos; }
	bool is_action() const { return action != -1; }
	int getAction() const { return action; }
	int getIndex() const { return index; }
	void setIndex(int new_index) { index = new_index; }
	int getDirection() const { return direction; }

	void assignPosition(int& start, int& duration)
	{
		switch (getAction())
		{

		case 0:// MOVE
			duration -= getShift();
		case 1: // START
			start += getShift();
			break;
		case 2: // DURATION
			duration += getShift();
		}
	}


private:
	int start_pos;
	int end_pos;
	int action;
	int index;
	int direction;
};