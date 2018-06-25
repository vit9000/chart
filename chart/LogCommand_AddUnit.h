#pragma once

#include "ILogCommand.h"
#include "ContainerUnit.h"

class LogCommand_AddUnit : public ILogCommand
{
	const Unit* _unit;
	function<void()> _undo;
public:
	LogCommand_AddUnit(const Unit * new_unit, const function<void()>& UndoFunction) : _unit (new_unit), _undo(UndoFunction)
	{}

	void undo() override { _undo; }

};
