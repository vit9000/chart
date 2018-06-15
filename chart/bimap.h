#pragma once
#include <map>
#include <vector>

using namespace std;

template <class T1, class T2> 
class bimap
{
private:
	map<T1, T2> master;
	map<T2, T1> slave;
public:
	bimap() {}
	bimap(const pair<T1, T2>& PAIR)
	{
		insert(PAIR.first, PAIR.second);
	}

	bimap(const pair<T2, T1>& PAIR)
	{
		insert(PAIR.second, PAIR.first);
	}

	bimap(const map<T1, T2>& MAP)
	{
		for (const auto& PAIR : MAP)
		{
			insert(PAIR.first, PAIR.second);
		}
	}

	bimap(const map<T2, T1>& MAP)
	{
		for (const auto& PAIR : MAP)
		{
			insert(PAIR.second, PAIR.first);
		}
	}

	void insert(const pair<T1, T2>& PAIR)
	{
		insert(PAIR.first, PAIR.second);
	}

	void insert(const T1& key, const T2& value)
	{
		master[key] = value;
		slave[value] = key;
	}

	void insert(const T2& key, const T1& value)
	{
		slave[key] = value;
		master[value] = key;
	}

	size_t size() const { return master.size(); }

	void clear() { master.clear(); slave.clear(); }

	const T2& at(const T1& key) const { return master.at(key); }
	T2& at(const T1& key) { return master.at(key); }

	const T1& at(const T2& key) const { return slave.at(key); }
	T1& at(const T2& key) { return slave.at(key); }


	
	const map<T1, T2>& getMap() const { return master; }

	void getVector(vector<T1>& result) const
	{
		result.clear();
		result.reserve(master.size());
		for (const auto& it : master)
			result.push_back(it.first);
	}

	void getVector(vector<T2>& result) const
	{
		result.clear();
		result.reserve(master.size());
		for (const auto& it : master)
			result.push_back(it.second);
	}

	size_t count(const T1& key) const
	{
		return master.count(key);
	}

	size_t count(const T2& key) const
	{
		return slave.count(key);
	}

};