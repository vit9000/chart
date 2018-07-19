#include "stdafx.h"
#include "LogCommandAdministrator.h"
#include "LogCommand_Units.h"
#include "LogCommand_ContainerUnits.h"

void LogCommandAdministrator::trim()
{
	if (cursor + 1 != static_cast<int>(commands.size()))
		commands.erase(commands.begin() + (cursor + 1), commands.end());
}

void LogCommandAdministrator::push_back(const LogCommandPtr& command)
{
	if (command->isEmpty()) return;
	trim();
	commands.push_back(command);
	cursor++;
	setEnabled();
}

bool LogCommandAdministrator::undo(IModel& model)
{
	if (commands.size() == 0) return false;
	if (cursor < 0) return false;

	commands[cursor]->undo(model);
	cursor--;
	setEnabled();
	return true;
}

bool LogCommandAdministrator::redo(IModel& model)
{
	if (cursor+1 >= static_cast<int>(commands.size()))
		return false;
	cursor++;
	commands[cursor]->redo(model);
	setEnabled();
	return true;
}

bool LogCommandAdministrator::isUndoAvailable()
{
	return (cursor >= 0);
}

bool LogCommandAdministrator::isRedoAvailable()
{
	return (cursor < static_cast<int>(commands.size())-1);
}

void LogCommandAdministrator::setEnabled()
{
	if (!editMenu || editMenu->GetMenuItemCount() < 2) return;
	editMenu->EnableMenuItem(0, MF_BYPOSITION | (isUndoAvailable() ? (MF_ENABLED) : (MF_DISABLED | MF_GRAYED)));
	editMenu->EnableMenuItem(1, MF_BYPOSITION | (isRedoAvailable() ? (MF_ENABLED) : (MF_DISABLED | MF_GRAYED)));
}

void LogCommandAdministrator::reset()
{
	commands.clear();
	cursor = -1;
	setEnabled();
}

void LogCommandAdministrator::getUpdatedUnitsIDs(set<wstring>& containers_ids, set<wstring>& unit_ids)
{
	trim();
	for (auto& command : commands)
	{
		if (LogCommand_Union* union_command = dynamic_cast<LogCommand_Union*>(command.get()))
		{
			for (auto& command : union_command->commands)
			{
				buildUpdatedUnits(unit_ids, command);
			}
		}
		else if (LogCommand_MoveDrug* moveCommand = dynamic_cast<LogCommand_MoveDrug*>(command.get()))
		{
			containers_ids.emplace(moveCommand->id.getIndex());
		}
		else
			buildUpdatedUnits(unit_ids, command);
	}
}

void LogCommandAdministrator::buildUpdatedUnits(set<wstring>& ids, LogCommandPtr& command)
{
	if (LogCommand_UpdateUnit* com = dynamic_cast<LogCommand_UpdateUnit*>(command.get()))
	{
		ids.emplace(com->updated.getDB_ID());
	}
}