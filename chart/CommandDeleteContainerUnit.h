#pragma once

#include "ITableCommand.h"
#include "ID.h"


class CommandDeleteContainerUnit : public ITableCommand
{
private:
	const ID& id;
public:
	CommandDeleteContainerUnit(const ID& _id)
		: id(_id)
	{}

	virtual void Execute(CTableContainer* table_container)
	{
		table_container->DeleteContainerUnit(id);
	}
};