#pragma once

#include "ITableCommand.h"
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

class Observer
{
public:
	virtual void Update(vector<shared_ptr<ITableCommand> >& table_commands){};
};