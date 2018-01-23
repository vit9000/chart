#include "ITableCommand.h"
#include <string>


class CommandAddUnitContainer : public ITableCommand
{
private:

	const UnitContainer& drug;
public:
	CommandAddUnitContainer(const UnitContainer& Drug): drug(Drug)
	{}

	virtual void Execute(CTableContainer* table_container)
	{
		table_container->Add(drug); 
	}
};