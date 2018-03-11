#pragma once

#include "ITableCommand.h"
#include <string>


class CommandAddBlock : public ITableCommand
{
private:
	const std::wstring& blockName;
public:
	CommandAddBlock(const wstring& BlockName)
		: blockName(BlockName)
	{}

	virtual void Execute(CTableContainer* table_container)
	{
		table_container->AddBlock(blockName);
	}
};
