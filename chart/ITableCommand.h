#pragma once

#include "CTableContainer.h"
#include <boost/shared_ptr.hpp>


class ITableCommand
{
public:
	virtual void Execute(CTableContainer* table_container)
	{}
};

typedef boost::shared_ptr<ITableCommand> TableCommand_Ptr;