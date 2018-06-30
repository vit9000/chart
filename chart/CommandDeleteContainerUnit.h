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

class CommandDeleteChildContainerUnit : public ITableCommand
{
private:
	const ID& parent_id;
	const ID& id;

public:
	CommandDeleteChildContainerUnit(const ID& _parent, const ID& _id)
		: parent_id(_parent), id(_id)
	{}

	virtual void Execute(CTableContainer* table_container)
	{
		table_container->DeleteChildContainerUnit(parent_id, id);
	}
};