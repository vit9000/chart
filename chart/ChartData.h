#pragma once
#include <vector>
#include <string>
//#include <boost/shared_ptr.hpp>
//using boost::shared_ptr
#include <memory>
#include <map>
#include "ID.h"
using std::shared_ptr;
using namespace std;

#include "ModelContainers.h"
#include "ChartStructure.h"

//typedef shared_ptr<ContainerUnit> ContainerUnit_Ptr;
typedef ContainerUnit* ContainerUnit_Ptr;
typedef map<wstring, map<int, ContainerUnit_Ptr>> BlockVector;

class ChartData
{
private:
	wstring date;
	BlockVector administrations;
	vector<wstring> blocks;//храним последовательность блоков
public:
	ChartData()
	{
	}
	ChartData(const wstring& Date)
		: date(Date)
	{}
	
	const BlockVector& getAdministrations() const 
	{ 
		return administrations;
	}

	const wstring& getDate() const
	{ return date; }

	void addBlock(const wstring& BlockName)
	{
		if (administrations.count(BlockName) > 0) return;
		administrations[BlockName];
	}

	ContainerUnit_Ptr addDrugToDrug(const ID& host_id, int type, const DrugInfo& drugInfo)
	{
		ContainerUnit_Ptr new_drug = addDrug(host_id.getBlockName(), type, drugInfo);
		ContainerUnit_Ptr host_drug = getContainerUnit(host_id);
		//host_drug->linkContainerUnit(new_drug.get());
		host_drug->linkContainerUnit(new_drug);
		return new_drug;
	}

	ContainerUnit_Ptr addDrug(const wstring& BlockName, int type, const DrugInfo& drugInfo)
	{
		
		ContainerUnit_Ptr drug;
		switch (type)
		{
		
		case 1:
			drug = ContainerUnit_Ptr((ContainerUnit*)new ContainerIVdrops(BlockName, drugInfo));
			break;
		case 2: // в/в дозатором
		case 10: // эпидурально дозатором
			drug = ContainerUnit_Ptr((ContainerUnit*)new ContainerInfusion(BlockName, drugInfo));
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
		switch (type)
		{
			default:
			case ChartStructure::NUMERIC:
				param = ContainerUnit_Ptr(new ContainerParameter(BlockName, ParameterName));
				break;
			case ChartStructure::TEXT:
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

	
	

};