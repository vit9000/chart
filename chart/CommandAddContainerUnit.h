#include "ITableCommand.h"
#include <string>


class CommandAddContainerUnit : public ITableCommand
{
private:

	const ContainerUnit& drug;
public:
	CommandAddContainerUnit(const ContainerUnit& Drug): drug(Drug)
	{}

	virtual void Execute(CTableContainer* table_container)
	{
		table_container->Add(drug); 
	}
};