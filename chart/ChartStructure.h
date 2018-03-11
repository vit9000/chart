#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Ini.h"


using namespace std;
class ChartStructure
{
	map<wstring, vector<pair<wstring, int>>> data;
	vector<wstring> blocks;
	map<wstring,int> block_types;

public:
	ChartStructure()
	{
		Ini ini(L"structure.txt");
		int countBlocks = ini.Read(L"blocks", L"count", 0);
		for (int i = 1; i <= countBlocks; i++)
		{
			vector<pair<wstring, int>> block;

			wstring blockName = ini.Read(L"blocks", to_wstring(i), L"");

			int type = ini.Read(blockName, L"type", 0);
			block_types[blockName]=type;

			int countInBlock = ini.Read(blockName, L"count", 0);
			for (int j = 1; j <= countInBlock; j++)
			{
				pair<wstring, int> temp = ini.ReadPair(blockName, to_wstring(j), pair<wstring,int>(L"",0));
				block.push_back(temp);
			}
			blocks.push_back(blockName);
			data[blockName] = block;
		}

	};
	

	

	enum {NUMERIC = 1, TEXT = 2};
	enum {STANDART, PLOT, ADMINISTRATIONS};

	vector<pair<wstring, int>> getBlockParameters(const wstring& BlockName) const
	{
		if (data.count(BlockName) == 0)
		{
			vector<pair<wstring, int>> params;
			return params;
		}
		return data.at(BlockName);
	}
	const vector<wstring>& getBlocks() const
	{
		return blocks;
	}

	
	int getBlockType(const wstring& BlockName) const
	{
		if (block_types.count(BlockName) == 0) return 0;
		return block_types.at(BlockName);
	}
	
	wstring getAdministrationsBlockName()
	{
		for (auto& it : block_types)
			if (it.second == ADMINISTRATIONS)
				return it.first;
		
		return L"";
		
	}

	/*wstring getText(int type)
	{
		if (type >= static_cast<int>(blocks.size()))
			return L"";
		return getBlocks()[type];
	}*/
};