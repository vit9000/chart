#pragma once

#include "ITableCommand.h"
#include <string>
using std::wstring;
//������ �������� ������ �������, � ������������� ������������
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