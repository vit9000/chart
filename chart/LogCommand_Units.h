#pragma once

#include <functional>

#include "ILogCommand.h"
#include "Unit.h"

using namespace std;



class LogCommand_AddUnit : public ILogCommand
{
	Unit unit;
	function<void(const Unit&)> undo_function;
public:
	LogCommand_AddUnit(const Unit& new_unit, const function<void(const Unit&)>& UndoFunction)
		: 
		unit (new_unit),
		undo_function(UndoFunction)
	{}

	void undo() override { undo_function(unit); }
};
//-----------------------------------------------------
class LogCommand_UpdateUnit : public ILogCommand
{
	Unit backup;
	Unit updated;
	function<void(const Unit& oldUnit, const Unit& newUnit)> undo_function;
public:
	LogCommand_UpdateUnit(const Unit& backup_unit, const Unit& updated_unit, const function<void(const Unit&, const Unit&)>& UndoFunction)
		: 
		backup(backup_unit), 
		updated(updated_unit),
		undo_function(UndoFunction)
	{}

	void undo() override { undo_function(backup, updated); }
};
//-----------------------------------------------------
class LogCommand_DeleteUnit : public ILogCommand
{
	Unit backup;
	function<void(const Unit&)> undo_function;
public:
	LogCommand_DeleteUnit(const Unit& backup_unit, const function<void(const Unit&)>& UndoFunction)
		:
		backup(backup_unit),
		undo_function(UndoFunction)
	{}

	void undo() override { undo_function(backup); }
};
