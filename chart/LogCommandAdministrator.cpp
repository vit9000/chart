#include "stdafx.h"
#include "LogCommandAdministrator.h"


void LogCommandAdministrator::push_back(const LogCommandPtr& command)
{
	commands.push_back(command);
}