#pragma once

#include <memory>
#include "IModel.h"

class ILogCommand
{
protected:
	ID id;// unitConteinerID
public:
	ILogCommand(const ID& _id) : id(_id) {}
	virtual void undo(IModel& model, bool redraw=true) = 0;
	virtual void redo(IModel& model, bool redraw=true) = 0;
};

typedef std::shared_ptr<ILogCommand> LogCommandPtr;
