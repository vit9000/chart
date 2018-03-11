#pragma once
#include <vector>
#include <string>
//#include <boost/shared_ptr.hpp>
//using boost::shared_ptr
#include <memory>
#include <map>
using std::shared_ptr;
using namespace std;

#include "ModelContainers.h"
#include "ChartStructure.h"

typedef shared_ptr<ContainerUnit> ContainerUnit_Ptr;
typedef map<wstring, vector<ContainerUnit_Ptr>> BlockVector;

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

	size_t addDrug(const wstring& BlockName, int type, const wstring& DrugName)
	{
		
		ContainerUnit_Ptr drug;
		switch (type)
		{
		default:
		case 0:
			drug = ContainerUnit_Ptr((ContainerUnit*)new ContainerIVdrops(DrugName));
			break;
		case 1:
			drug = ContainerUnit_Ptr((ContainerUnit*)new ContainerIVbolus(DrugName));
			break;
		case 2:
			drug = ContainerUnit_Ptr((ContainerUnit*)new ContainerIVinfusion(DrugName));
			break;
		case 3:
			drug = ContainerUnit_Ptr((ContainerUnit*)new ContainerTabs(DrugName));
			break;
		}
		
		
		
		administrations[BlockName].push_back(drug);
		return administrations[BlockName].size() - 1;
	}

	

	ContainerUnit_Ptr getContainerUnit(const wstring& BlockName, size_t index)
	{
		if (administrations.count(BlockName) == 0) throw out_of_range("getContainerUnit: BlockName is not exists");
		if (index >= administrations[BlockName].size()) throw out_of_range("getContainerUnit out_of_range vector");
		return administrations[BlockName].at(index);
	}

	size_t addParameter(const wstring& BlockName, const wstring& ParameterName, int type)
	{
		
		switch (type)
		{
			case ChartStructure::NUMERIC:
				administrations[BlockName].push_back(ContainerUnit_Ptr(new ContainerParameter(ParameterName)));
				break;
			case ChartStructure::TEXT:
				administrations[BlockName].push_back(ContainerUnit_Ptr(new ContainerTextParameter(ParameterName)));
				break;
		}
		
		return administrations.size() - 1;
	}

	bool addUnit(const wstring& BlockName, size_t i, const Unit& unit)
	{
		if (administrations.count(BlockName) == 0)
		{
			blocks.push_back(BlockName);
			administrations[BlockName];
		}
		if(i>=administrations[BlockName].size()) return false;
		administrations[BlockName][i]->addUnit(unit);
		return true;
	}

	
	

};