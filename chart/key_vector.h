#pragma once
#include <vector>
#include <map>


template <class KEY, class VALUE>
class key_vector : public vector<VALUE>
{
	typedef vector<VALUE> PARENT;

	map<KEY, size_t> dict;
	vector<const KEY*> keys;
public:

	void erase(const KEY& key)
	{
		size_t pos = dict.at(key);
		PARENT::erase(PARENT::begin() + pos);
		keys.erase(keys.begin() + pos);
		dict.erase(key);
	}

	void insert(size_t pos, const KEY& key, const VALUE& val)
	{
		if (pos >= size()) push_back(key, val);
		if (dict.count(key) > 0) return;

		PARENT::insert(PARENT::begin()+pos, val);
		dict[key] = this->size() - 1;
		keys.insert(keys.begin()+pos, &(dict.find(key)->first));
	}


	void push_back(const KEY& key, const VALUE& val)
	{
		if (dict.count(key) > 0) return;

		PARENT::push_back(val);
		dict[key] = this->size() - 1;
		keys.push_back(&(dict.find(key)->first));
	}

	VALUE& operator[] (const KEY& key)
	{
		if (dict.count(key) == 0)
			push_back(key, VALUE());
		return this->at(dict[key]);
	}

	const VALUE& operator[] (const KEY& key) const
	{
		if (dict.count(key) == 0)
			throw out_of_range("key_vector");
		return this->at(dict[key]);
	}

	VALUE& at (size_t index)
	{
		check_valid_index(index);
		return PARENT::at(index);
	}

	const VALUE& at (size_t index) const
	{
		check_valid_index(index);
		return PARENT::at(index);
	}
	

	VALUE& operator[] (size_t index)
	{
		return at(index);
	}

	const VALUE& operator[] (size_t index) const
	{
		return at(index);
	}

	const VALUE& second (size_t index) const
	{
		return operator[](index);
	}

	const KEY& first(size_t index) const
	{
		check_valid_index(index);
		return *(keys[index]);
	}

	int getIndex(const KEY& key) const
	{
		if (dict.count(key) == 0) return -1;
		return static_cast<int>(dict.at(key));
	}

	const pair<KEY, VALUE>& get_pair(size_t index) const
	{
		check_valid_index(index);
		return male_pair(*(keys[index]), this->at(index));
	}
	
	

	void clear()
	{
		this->clear();
		dict.clear();
		keys.clear();
	}

	size_t count(const KEY& key)
	{
		return dict.count(key);
	}
private:
	inline void check_valid_index(size_t index) const
	{
		if (index >= this->size())
			throw out_of_range("key_vector");
	}
};