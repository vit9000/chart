#pragma once

#include "ITableCommand.h"
#include <string>


class CommandSetTimes : public ITableCommand
{
private:
	double begin_date;
	double end_date;
public:
	CommandSetTimes(double BeginDate, double EndDate)
		: begin_date(BeginDate), end_date(EndDate)
	{}

	virtual void Execute(CTableContainer* table_container)
	{
		table_container->setTimes(begin_date, end_date);
	}
};
