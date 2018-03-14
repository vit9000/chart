#pragma once
#include <vector>
#include <Windows.h>
#include <functional>
#include "Rect.h"

class CursorHandler
{
protected:
	size_t current;
	std::vector<HCURSOR> cursors;
	std::function<void()> callBack;
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

	virtual void setEditBox(const Rect& rect, function<void(const std::wstring&)> callBack, const wstring& defaultValue) = 0;


};