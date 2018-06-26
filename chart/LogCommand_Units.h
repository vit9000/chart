#pragma once

#include <functional>

#include "ILogCommand.h"
#include "Unit.h"

typedef std::function<void(const Unit&)> UNIT_UNDO_FUNCTION;

class LogCommand_UnitUndo : public ILogCommand
{
protected:
	UNIT_UNDO_FUNCTION undo_function;
public:
	LogCommand_UnitUndo(const UNIT_UNDO_FUNCTION& UndoFunction)
		: undo_function(UndoFunction) {}
};


class LogCommand_AddUnit : public LogCommand_UnitUndo
{
	Unit unit;
	
public:
	LogCommand_AddUnit(const Unit& new_unit, const UNIT_UNDO_FUNCTION& UndoFunction)
		: 
		unit (new_unit),
		LogCommand_UnitUndo(UndoFunction)
	{}

	void undo() override { undo_function(unit); }
};

class LogCommand_UpdateUnit : public LogCommand_UnitUndo
{
	Unit backup;
	Unit updated;
	
public:
	LogCommand_UpdateUnit(const Unit& backup_unit, const Unit& updated_unit, const UNIT_UNDO_FUNCTION& UndoFunction)
		: 
		backup(backup_unit), 
		updated(updated_unit),
		LogCommand_UnitUndo(UndoFunction)
	{}

	void undo() override { undo_function(backup); }
};

class LogCommand_DeleteUnit : public LogCommand_UnitUndo
{
	Unit backup;
public:
	LogCommand_DeleteUnit(const Unit& backup_unit, const UNIT_UNDO_FUNCTION& UndoFunction)
		:
		backup(backup_unit),
		LogCommand_UnitUndo(UndoFunction)
	{}

	void undo() override { undo_function(backup); }
};
