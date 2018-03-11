#pragma once

#include "ITableCommand.h"
#include <string>


class CommandAddBlock : public ITableCommand
{
private:
	const std::wstring& blockName;
	int type;
public:
	CommandAddBlock(const wstring& BlockName, int Type)
		: blockName(BlockName), type(Type)
	{}

	virtual void Execute(CTableContainer* table_container)
	{
		table_container->AddBlock(blockName, type);
	}
};
