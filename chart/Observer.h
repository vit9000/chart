#pragma once

#include "ITableCommand.h"

class Observer
{
public:
	virtual void Update(vector<ITableCommand*>& table_commands){};
};