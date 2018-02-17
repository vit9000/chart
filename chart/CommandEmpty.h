#pragma once

#include "ITableCommand.h"
#include <string>
using std::wstring;
//просто пошлется пустая команда, а Представление перерисуется
class CommandEmpty : public ITableCommand
{
private:
	wstring drugName;
public:
	CommandEmpty()
	{}

	virtual void Execute(CTableContainer* table_container)
	{
		
	}
};