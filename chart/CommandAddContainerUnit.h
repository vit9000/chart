#include "ITableCommand.h"
#include <string>


class CommandAddContainerUnit : public ITableCommand
{
private:

	const ContainerUnit& contUnit;
	const std::wstring& blockName;
public:
	CommandAddContainerUnit(const wstring& BlockName,const ContainerUnit& containerUnit)
		: blockName(BlockName), contUnit(containerUnit)
	{}

	virtual void Execute(CTableContainer* table_container)
	{
		table_container->Add(blockName, &contUnit);
	}
};