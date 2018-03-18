#pragma once
#include <string>
using std::wstring;
class ID
{
	wstring block_name;
	int index;
public:
	ID(const wstring& BlockName, int Index)
		: block_name(BlockName), index(Index)
	{}

	ID(const ID& id)
		: block_name (id.block_name),
		index(id.index)
	{
		
	}

	ID operator=(const ID& id)
	{
		block_name = id.block_name;
		index = id.index;
		return *this;
	}

	int getIndex() const { return index; }
	const wstring& getBlockName() const { return block_name; }

	friend bool operator== (const ID& lhs, const ID& rhs)
	{
		return ((lhs.block_name == rhs.block_name) && (lhs.index && rhs.index));
	}

};