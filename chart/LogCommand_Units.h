#pragma once

#include <vector>

#include "ILogCommand.h"
#include "Unit.h"

using namespace std;




class LogCommand_AddUnit : public ILogCommand
{
	Unit unit;
	
public:
	LogCommand_AddUnit(const ID& _id, const Unit& new_unit)
		: ILogCommand(_id),
		unit (new_unit)
	{}

	void undo(IModel& model) override 
	{ 
		model.deleteUnit(id, unit.getStart());
	}

	void redo(IModel& model) override
	{
		model.addUnit(id, unit);
	}
};
//-----------------------------------------------------
class LogCommand_UpdateUnit : public ILogCommand
{
	Unit backup;
	Unit updated;
public:
	LogCommand_UpdateUnit(const ID& _id, const Unit& backup_unit, const Unit& updated_unit)
		: ILogCommand(_id),
		backup(backup_unit), 
		updated(updated_unit)
	{}

	void undo(IModel& model) override
	{
		model.deleteUnit(id, updated.getStart());
		model.addUnit(id, backup);
	}

	void redo(IModel& model) override
	{
		model.updateUnit(id, backup.getStart(), updated);
	}
};
//-----------------------------------------------------
class LogCommand_DeleteUnit : public ILogCommand
{
	Unit backup;
public:
	LogCommand_DeleteUnit(const ID& _id, const Unit& backup_unit)
		: ILogCommand(_id),
		backup(backup_unit)
	{}

	void undo(IModel& model) override
	{
		model.addUnit(id, backup);
	}

	void redo(IModel& model) override
	{
		model.deleteUnit(id, backup.getStart());
	}
};

// объединение комманд всех типов для нескольких строк
class LogCommand_Union : public ILogCommand
{
	vector<LogCommandPtr> commands;
public:
	LogCommand_Union(const ID& _id)
		: ILogCommand(_id) {}

	inline void add(const LogCommandPtr& command) { commands.push_back(command); }

	void undo(IModel& model) override
	{
		for (auto& command : commands)
			command->undo(model);
	}

	void redo(IModel& model) override
	{
		for (auto& command : commands)
			command->redo(model);
	}

};