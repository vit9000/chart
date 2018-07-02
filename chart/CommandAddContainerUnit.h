#pragma once
#include "ITableCommand.h"
#include <string>


class CommandAddContainerUnit : public ITableCommand
{
private:

	const ContainerUnit& contUnit;
	const std::wstring& blockName;
	int pos;
public:
	CommandAddContainerUnit(const wstring& BlockName,const ContainerUnit& containerUnit, int Pos)
		: blockName(BlockName), contUnit(containerUnit), pos(Pos)
	{}

	virtual void Execute(CTableContainer* table_container)
	{
		table_container->AddToBlock(blockName, &contUnit, pos);
	}
};