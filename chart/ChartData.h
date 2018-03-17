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

typedef shared_ptr<ContainerUnit> ContainerUnit_Ptr;
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

	inline ID getID(const wstring& BlockName)
	{
		static int index = 0;
		index++;
		return ID(BlockName, index);
	}

	ContainerUnit_Ptr addDrug(const wstring& BlockName, int type, const wstring& DrugName)
	{
		ID id = getID(BlockName);
		ContainerUnit_Ptr drug;
		switch (type)
		{
		default:
		case 0:
			drug = ContainerUnit_Ptr((ContainerUnit*)new ContainerIVdrops(id, DrugName));
			break;
		case 1:
			drug = ContainerUnit_Ptr((ContainerUnit*)new ContainerIVbolus(id, DrugName));
			break;
		case 2:
			drug = ContainerUnit_Ptr((ContainerUnit*)new ContainerIVinfusion(id, DrugName));
			break;
		case 3:
			drug = ContainerUnit_Ptr((ContainerUnit*)new ContainerTabs(id, DrugName));
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
		ID id = getID(BlockName);
		ContainerUnit_Ptr param;
		switch (type)
		{
			default:
			case ChartStructure::NUMERIC:
				param = ContainerUnit_Ptr(new ContainerParameter(id, ParameterName));
				break;
			case ChartStructure::TEXT:
				param = ContainerUnit_Ptr(new ContainerTextParameter(id, ParameterName));
				break;
		}
		administrations[BlockName][param->getID().getIndex()] = param;
		return param;//administrations.size() - 1;
	}

	bool addUnit(const ID& id, const Unit& unit)
	{
		auto& containerUnit = getContainerUnit(id);
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