#pragma once
#include "ITableCommand.h"
#include <string>


class CommandMoveDrug : public ITableCommand
{
private:

	const ID& _id;
	int pos;
public:
	CommandMoveDrug(const ID& id, int new_pos)
		: _id(id), pos(new_pos)
	{}

	virtual void Execute(CTableContainer* table_container)
	{
		table_container->moveDrug(_id, pos);
	}
}; 
