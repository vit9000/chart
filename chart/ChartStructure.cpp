
#include "stdafx.h"
#include "ChartStructure.h"

ChartStructure * ChartStructure::instance = nullptr;
ChartStructure::Destroyer ChartStructure::destroyer;

ChartStructure::ChartStructure()
{
	Ini ini(L"structure.txt");
	int countBlocks = ini.Read(L"blocks", L"count", 0);
	for (int i = 1; i <= countBlocks; i++)
	{
		vector<pair<wstring, int>> block;

		wstring blockName = ini.Read(L"blocks", to_wstring(i), L"");

		int type = ini.Read(blockName, L"type", 0);
		block_types[blockName] = type;

		int countInBlock = ini.Read(blockName, L"count", 0);
		for (int j = 1; j <= countInBlock; j++)
		{
			pair<wstring, int> temp = ini.ReadPair(blockName, to_wstring(j), pair<wstring, int>(L"", 0));
			block.push_back(temp);
		}
		blocks.push_back(blockName);
		data[blockName] = block;
	}

}


ChartStructure * ChartStructure::getInstance()
{
	if (!instance)
	{
		instance = new ChartStructure;
		destroyer.init(instance);
	}
	return instance;
}




vector<pair<wstring, int>> ChartStructure::getBlockParameters(const wstring& BlockName) const
{
	if (data.count(BlockName) == 0)
	{
		vector<pair<wstring, int>> params;
		return params;
	}
	return data.at(BlockName);
}
const vector<wstring>& ChartStructure::getBlocks() const
{
	return blocks;
}


int ChartStructure::getBlockType(const wstring& BlockName) const
{
	if (block_types.count(BlockName) == 0) return 0;
	return block_types.at(BlockName);
}

wstring ChartStructure::getAdministrationsBlockName()
{
	for (auto& it : block_types)
		if (it.second == ADMINISTRATIONS)
			return it.first;

	return L"";

}