#pragma once
#include <vector>
#include "IObserver.h"

using std::vector;

class IObservable
{
protected:
	vector<IObserver*> observer_list;
public:
	void Register(IObserver* observer)
	{
		observer_list.push_back(observer);
	}

	void Unregister(size_t index)
	{
		observer_list.erase(observer_list.begin()+index);
	}

	void Notify()
	{
		for(vector<IObserver*>::iterator it=observer_list.begin();
			it != observer_list.end();
			++it)
		{
			(*it)->Update();
		}
	}
};