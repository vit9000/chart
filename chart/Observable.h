#pragma once
#include <vector>
using std::vector;

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "Observer.h"
#include "ITableCommand.h"



class Observable
{
protected:
	vector<Observer*> observer_list;
public:
	void Register(Observer* observer)
	{
		observer_list.push_back(observer);
	}

	void Unregister(size_t index)
	{
		observer_list.erase(observer_list.begin()+index);
	}

	void Notify(vector<shared_ptr<ITableCommand> >& table_commands)
	{
		for(vector<Observer*>::iterator it=observer_list.begin();
			it != observer_list.end();
			++it)
		{
			(*it)->Update(table_commands);
		}
	}
};