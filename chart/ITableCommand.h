#pragma once

#include "CTableContainer.h"
//#include <boost/shared_ptr.hpp>
#include <memory>
using std::shared_ptr;

class ITableCommand
{
public:
	virtual void Execute(CTableContainer* table_container)
	{}
};

typedef shared_ptr<ITableCommand> TableCommand_Ptr;