#pragma once
#include <vector>
#include <Windows.h>

class CursorHandler
{
protected:
	size_t current;
	std::vector<HCURSOR> cursors;
	enum { MOVE = 0, START, DURATION };
public:
	CursorHandler()
		: cursors({ LoadCursor(NULL,IDC_HAND), LoadCursor(NULL, IDC_SIZEWE), LoadCursor(NULL, IDC_SIZEWE) })
	{
		
	}

	virtual void SetMouseCursor(size_t index)
	{
		current = index;
	}


};