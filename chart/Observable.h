#pragma once
#include <vector>
#include "Observer.h"
#include "ITableCommand.h"

using std::vector;

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

	void Notify(vector<ITableCommand*>& table_commands)
	{
		for(vector<Observer*>::iterator it=observer_list.begin();
			it != observer_list.end();
			++it)
		{
			(*it)->Update(table_commands);
		}
	}
};