
#include "stdafx.h"
#include "ChartStructure.h"

ChartStructure * ChartStructure::instance = nullptr;
ChartStructure::Destroyer ChartStructure::destroyer;

ChartStructure::ChartStructure()
{
	/*Ini ini(L"structure.txt");
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
	}*/

	// Read file in UTF-8
	std::wifstream wif(L"structure_json.txt");
	wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

	std::wstringstream wss;
	wss << wif.rdbuf();

	
	WDocument document;
	document.Parse(wss.str().c_str());
//	assert(document.IsObject());
//	assert(document.HasMember("name"));
//	assert(document["name"].IsString());
	//printf("hello = %s\n", );
	//wstring s = document[L"name"].GetString();
	//wstring temp = s;
	const auto& blocksJson = document[L"blocks"];
	// читаем массив блоков
	for (auto blockIt = blocksJson.Begin(); blockIt != blocksJson.End(); ++blockIt)
	{
		vector<pair<wstring, int>> block;
		wstring blockName = (*blockIt)[L"name"].GetString();
		block_types[blockName] = (*blockIt)[L"type"].GetInt();

		const auto& lines = (*blockIt)[L"lines"];
		
		
		if (lines.IsArray())
		{
			for (auto lineIt = lines.Begin(); lineIt != lines.End(); ++lineIt)
			{
				wstring name = (*lineIt)[0].GetString();
				int type = ((*lineIt)[1].GetString() == L"number") ? 0 : 1;
				block.push_back(make_pair(name, type));
			}
		}
		blocks.push_back(blockName);
		data[blockName] = block;

	}

}
//-----------------------------------------------------------------------------------------------
ChartStructure * ChartStructure::getInstance()
{
	if (!instance)
	{
		instance = new ChartStructure;
		destroyer.init(instance);
	}
	return instance;
}
//-----------------------------------------------------------------------------------------------
vector<pair<wstring, int>> ChartStructure::getBlockParameters(const wstring& BlockName) const
{
	if (data.count(BlockName) == 0)
	{
		vector<pair<wstring, int>> params;
		return params;
	}
	return data.at(BlockName);
}
//-----------------------------------------------------------------------------------------------
const vector<wstring>& ChartStructure::getBlocks() const
{
	return blocks;
}
//-----------------------------------------------------------------------------------------------
int ChartStructure::getBlockType(const wstring& BlockName) const
{
	if (block_types.count(BlockName) == 0) return 0;
	return block_types.at(BlockName);
}
//-----------------------------------------------------------------------------------------------
wstring ChartStructure::getAdministrationsBlockName()
{
	for (auto& it : block_types)
		if (it.second == ADMINISTRATIONS)
			return it.first;

	return L"";

}
//-----------------------------------------------------------------------------------------------