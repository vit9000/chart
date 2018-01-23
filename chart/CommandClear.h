#pragma once

#include "ITableCommand.h"

class CommandClear : public ITableCommand
{
public:
	virtual void Execute(CTableContainer* table_container)
	{
		table_container->Clear(); 
	}
};