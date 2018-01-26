#pragma once

#include "ITableCommand.h"

class Observer
{
public:
	virtual void Update(vector<TableCommand_Ptr>& table_commands){};
};