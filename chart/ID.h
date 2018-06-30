#pragma once
#include <string>
using std::wstring;
class ID
{
	wstring block_name;
	wstring db_id;
public:
	ID()
	{}

	ID(const wstring& BlockName, wstring DB_ID)
		: block_name(BlockName), db_id(DB_ID)
	{}

	ID(const ID& id)
		: block_name (id.block_name),
		db_id(id.db_id)
	{
		
	}

	ID operator=(const ID& id)
	{
		block_name = id.block_name;
		db_id = id.db_id;
		return *this;
	}

	wstring getIndex() const 
	{ 
		return db_id;
	}
	const wstring& getBlockName() const { return block_name; }

	friend bool operator== (const ID& lhs, const ID& rhs)
	{
		return ((lhs.block_name == rhs.block_name) && (lhs.db_id == rhs.db_id));
	}

	bool isEmpty()
	{
		return db_id.empty();
	}
};