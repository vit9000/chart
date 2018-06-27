#include "stdafx.h"
#include "LogCommandAdministrator.h"


void LogCommandAdministrator::push_back(const LogCommandPtr& command)
{
	if (cursor + 1 != static_cast<int>(commands.size()))
	{
		commands.erase(commands.begin() + (cursor+1), commands.end());
	}
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
	editMenu->EnableMenuItem(0, MF_BYPOSITION | (isUndoAvailable() ? (MF_ENABLED) : (MF_DISABLED | MF_GRAYED)));
	editMenu->EnableMenuItem(1, MF_BYPOSITION | (isRedoAvailable() ? (MF_ENABLED) : (MF_DISABLED | MF_GRAYED)));
}