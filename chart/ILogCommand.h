#pragma once

#include <memory>


class ILogCommand
{
public:
	
	virtual void undo() = 0;
};

typedef std::shared_ptr<ILogCommand> LogCommandPtr;
