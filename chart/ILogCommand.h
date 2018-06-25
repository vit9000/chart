#pragma once
#include <functional>
class ILogCommand
{
public:
	virtual void undo() = 0;
};
