#pragma once
#include <string>
using std::wstring;
class ID
{
	const wstring block_name;
	const int index;
public:
	ID(const wstring& BlockName, int Index)
		: block_name(BlockName), index(Index)
	{}

	ID(const ID& id)
		: block_name (id.block_name),
		index(id.index)
	{
		
	}

	int getIndex() const { return index; }
	const wstring& getBlockName() const { return block_name; }

};