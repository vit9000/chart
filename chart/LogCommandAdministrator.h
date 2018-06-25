#pragma once
#include <vector>
#include <memory>
#include "ILogCommand.h"



using namespace std;

typedef shared_ptr<ILogCommand> LogCommandPtr;

class LogCommandAdministrator
{
private:
	vector<LogCommandPtr> commands;
public:
	void push_back(const LogCommandPtr& command);

};
