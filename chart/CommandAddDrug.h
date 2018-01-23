#pragma once

#include "ITableCommand.h"
#include <string>
using std::wstring;

class CommandAddDrug : public ITableCommand
{
private:
	wstring drugName;
public:
	CommandAddDrug(const wstring& DrugName):drugName(DrugName)
	{}

	virtual void Execute(CTableContainer* table_container)
	{
		table_container->Add(UnitContainer(drugName)); 
	}
};