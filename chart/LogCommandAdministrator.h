#pragma once
#include <vector>
#include <memory>
#include "ILogCommand.h"



using namespace std;


class LogCommandAdministrator
{
private:
	vector<LogCommandPtr> commands;
public:
	void push_back(const LogCommandPtr& command);
	bool undo() 
	{
		if (commands.size() == 0) return false;
		commands.back()->undo();
		commands.pop_back();
		return true;
	}
};
