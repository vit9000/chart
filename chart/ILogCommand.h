#pragma once

#include <memory>
#include "IModel.h"

class ILogCommand
{
protected:
	ID id;// unitConteinerID
public:
	ILogCommand(const ID& _id) : id(_id) {}
	virtual void undo(IModel& model) = 0;
	virtual void redo(IModel& model) = 0;
};

typedef std::shared_ptr<ILogCommand> LogCommandPtr;
