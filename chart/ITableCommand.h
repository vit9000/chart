#pragma once

#include "CTableContainer.h"

class ITableCommand
{
public:
	virtual void Execute(CTableContainer* table_container)
	{}
};