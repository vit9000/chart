#pragma once

#include "ITableCommand.h"
#include <string>
#include "Constants.h"


class CommandAddBlock : public ITableCommand
{
private:
	const std::wstring& blockName;
	int type;
	BlockInfo block_info;
public:
	CommandAddBlock(const wstring& BlockName, const BlockInfo& blockInfo)
		: blockName(BlockName), block_info(blockInfo)
	{}

	virtual void Execute(CTableContainer* table_container)
	{
		table_container->AddBlock(blockName, block_info);
	}
};
