#pragma once
#include <vector>
#include <string>
//#include <boost/shared_ptr.hpp>
//using boost::shared_ptr
#include <memory>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <locale>
#include <codecvt>
#include "ID.h"
#include "DBPatient.h"
#include "ContainerUnit.h"
#include "Constants.h"
#include "ModelContainers.h"



using std::shared_ptr;
using namespace std;

#include "rapidjson/document.h"     // rapidjson's DOM-style API
typedef rapidjson::GenericDocument<rapidjson::UTF16<> > WDocument;
typedef rapidjson::GenericValue<rapidjson::UTF16<> > WValue;

typedef ContainerUnit* ContainerUnit_Ptr;
typedef map<wstring, map<int, ContainerUnit_Ptr>> Data;





class ChartData
{
private:
	wstring date;
	Data administrations;
	vector<const wstring*> blocks;//храним последовательность блоков
	map<wstring, int> block_types;
public:
	ChartData()
	{
	}
	ChartData(const wstring& Date)
		: date(Date)
	{}
	
	const Data& getAdministrations() const 
	{ 
		return administrations;
	}

	const wstring& getDate() const
	{ return date; }

	void addBlock(const wstring& BlockName)
	{
		if (administrations.count(BlockName) > 0) return;
		administrations[BlockName];
		blocks.push_back(&(administrations.find(BlockName)->first));
	}

	ContainerUnit_Ptr addDrugToDrug(const ID& host_id, int type, const DrugInfo& drugInfo, const DBPatient& patientInfo)
	{
		ContainerUnit_Ptr new_drug = addDrug(host_id.getBlockName(), type, drugInfo, patientInfo);
		ContainerUnit_Ptr host_drug = getContainerUnit(host_id);
		//host_drug->linkContainerUnit(new_drug.get());
		host_drug->linkContainerUnit(new_drug);
		return new_drug;
	}

	ContainerUnit_Ptr addDrug(const wstring& BlockName, int type, const DrugInfo& drugInfo, const DBPatient& patientInfo)
	{
		ContainerUnit_Ptr drug;
		switch (type)
		{
		case 0: // drugToDrug IVdrops
		case 1: // IVdrops host
			drug = ContainerUnit_Ptr((ContainerUnit*)new ContainerIVdrops(BlockName, drugInfo, type));
			break;
		case 2: // в/в дозатором
		case 10: // эпидурально дозатором
			
			drug = ContainerUnit_Ptr((ContainerUnit*)new ContainerInfusion(BlockName, drugInfo, patientInfo.weight));
			break;
		case 3:
			drug = ContainerUnit_Ptr((ContainerUnit*)new ContainerIVbolus(BlockName, drugInfo));
			break;
		case 4:
			drug = ContainerUnit_Ptr((ContainerUnit*)new ContainerIM(BlockName, drugInfo));
			break;
		case 5:
			drug = ContainerUnit_Ptr((ContainerUnit*)new ContainerSubcutaneusly(BlockName, drugInfo));
			break;
		default:
			drug = ContainerUnit_Ptr((ContainerUnit*)new ContainerUnitMovable(BlockName, drugInfo));
			break;
		}
		
		
		
		//administrations[BlockName].push_back(drug);
		administrations[BlockName][drug->getID().getIndex()] = drug;
		//return ID(BlockName,administrations[BlockName].size() - 1);
		return drug;
	}

	ContainerUnit_Ptr getContainerUnit(const ID& id)
	{
		if (administrations.count(id.getBlockName()) == 0) throw out_of_range("getContainerUnit: BlockName is not exists");
		auto& block = administrations[id.getBlockName()];
		if (block.count(id.getIndex())==0) 
			throw out_of_range("getContainerUnit out of range map<int, ContainerUnit_Ptr>");
		return block.at(id.getIndex());
	}

	ContainerUnit_Ptr addParameter(const wstring& BlockName, const wstring& ParameterName, int type)
	{
		ContainerUnit_Ptr param;
		switch (static_cast<FIELD_TYPE>(type))
		{
			default:
			case FIELD_TYPE::NUMERIC:
				param = ContainerUnit_Ptr(new ContainerParameter(BlockName, ParameterName));
				break;
			case FIELD_TYPE::TEXT:
				param = ContainerUnit_Ptr(new ContainerTextParameter(BlockName, ParameterName));
				break;
		}
		administrations[BlockName][param->getID().getIndex()] = param;
		return param;//administrations.size() - 1;
	}

	bool addUnit(const ID& id, const Unit& unit)
	{
		ContainerUnit_Ptr containerUnit = getContainerUnit(id);
		containerUnit->addUnit(unit);
		return true;
		/*if (administrations.count(BlockName) == 0)
		{
			blocks.push_back(BlockName);
			administrations[BlockName];
		}
		//if(i>=administrations[BlockName].size()) return false;
		administrations[BlockName][i]->addUnit(unit);
		return true;*/
	}

	void loadFromJSON(const wstring& json)
	{
		WDocument document;
		document.Parse(json.c_str());
		//assert(document.IsObject());
		//assert(document.HasMember("name"));
		//assert(document["name"].IsString());
		const auto& blocksJson = document[L"blocks"];
		// читаем массив блоков
		for (auto blockIt = blocksJson.Begin(); blockIt != blocksJson.End(); ++blockIt)
		{
			wstring blockName = (*blockIt)[L"name"].GetString();
			block_types[blockName] = (*blockIt)[L"type"].GetInt();

			const auto& lines = (*blockIt)[L"lines"];

			addBlock(blockName);
			if (lines.IsArray())
			{
				for (auto lineIt = lines.Begin(); lineIt != lines.End(); ++lineIt)
				{
					wstring param_name = (*lineIt)[0].GetString();
					int type = ((*lineIt)[1].GetString() == L"number") ? 0 : 1;
					addParameter(blockName, param_name, type);
				}
			}
		}

	}


	inline const vector<const wstring*>& getBlockNames() const
	{	
		auto a = sizeof(blocks[0]);
		return blocks;
	}

	int getBlockType(const wstring& BlockName) const
	{
		if (block_types.count(BlockName) == 0) return 0;
		return block_types.at(BlockName);
	}

	wstring getAdministrationsBlockName() const
	{
		for (auto& it : block_types)
			if (it.second == static_cast<int>(BLOCK_TYPE::ADMINISTRATIONS))
				return it.first;

		return L"";
	}
	

};