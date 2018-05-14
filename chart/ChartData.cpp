#include "stdafx.h"
#include "ChartData.h"

void ChartData::addBlock(const wstring& BlockName)
{
	if (administrations.count(BlockName) > 0) return;
	administrations[BlockName];
	//blocks.push_back(&(administrations.find(BlockName)->first));
}
//--------------------------------------------------------------------------------------------
ContainerUnit_Ptr ChartData::addDrugToDrug(const ID& host_id, int type, const DrugInfo& drugInfo, const DBPatient& patientInfo)
{
	ContainerUnit_Ptr new_drug = addDrug(host_id.getBlockName(), type, drugInfo, patientInfo);
	ContainerUnit_Ptr host_drug = getContainerUnit(host_id);
	host_drug->linkContainerUnit(new_drug.get());
	return new_drug;
}
//--------------------------------------------------------------------------------------------
ContainerUnit_Ptr ChartData::addDrug(const wstring& BlockName, int type, const DrugInfo& drugInfo, const DBPatient& patientInfo)
{
	ContainerUnit_Ptr drug;
	ID id = getNewID(BlockName);
	switch (type)
	{
	case 0: // drugToDrug IVdrops
	case 1: // IVdrops host
		drug = ContainerUnit_Ptr(new ContainerIVdrops(id, drugInfo, type));
		break;
	case 2: // �/� ���������
	case 10: // ����������� ���������

		drug = ContainerUnit_Ptr(new ContainerInfusion(id, drugInfo, patientInfo.weight));
		break;
	case 3:
		drug = ContainerUnit_Ptr(new ContainerIVbolus(id, drugInfo));
		break;
	case 4:
		drug = ContainerUnit_Ptr(new ContainerIM(id, drugInfo));
		break;
	case 5:
		drug = ContainerUnit_Ptr(new ContainerSubcutaneusly(id, drugInfo));
		break;
	default:
		drug = ContainerUnit_Ptr(new ContainerUnitMovable(id, drugInfo));
		break;
	}

	insertIntoAdministrations(drug);
	return drug;
}
//--------------------------------------------------------------------------------------------
ID ChartData::getNewID(const wstring& BlockName)
{
	return ID(BlockName, administrations[BlockName].size());
}
//--------------------------------------------------------------------------------------------
void ChartData::insertIntoAdministrations(const ContainerUnit_Ptr& item)
{
	const auto& id = item->getID();
	administrations[id.getBlockName()].push_back(item);
}
//--------------------------------------------------------------------------------------------
const ContainerUnit_Ptr& ChartData::getContainerUnit(const ID& id)
{
	if (administrations.count(id.getBlockName()) == 0) throw out_of_range("getContainerUnit: BlockName is not exists");
	auto& block = administrations[id.getBlockName()];
	//if (block.count(id.getIndex())==0) 

	if (id.getIndex() >= static_cast<int>(block.size()))
		throw out_of_range("getContainerUnit out of range map<int, ContainerUnit_Ptr>");

	return block.at(id.getIndex());
}
//--------------------------------------------------------------------------------------------
ContainerUnit_Ptr ChartData::addParameter(const wstring& BlockName, const wstring& ParameterName, int type)
{
	ContainerUnit_Ptr param;
	ID id = getNewID(BlockName);
	switch (static_cast<FIELD_TYPE>(type))
	{
	default:
	case FIELD_TYPE::NUMERIC:
		param = ContainerUnit_Ptr(new ContainerParameter(id, ParameterName));
		break;
	case FIELD_TYPE::TEXT:
		param = ContainerUnit_Ptr(new ContainerTextParameter(id, ParameterName));
		break;
	}
	//administrations[BlockName][param->getID().getIndex()] = param;
	insertIntoAdministrations(param);
	return param;//administrations.size() - 1;
}
//--------------------------------------------------------------------------------------------
bool ChartData::addUnit(const ID& id, const Unit& unit)
{
	ContainerUnit_Ptr containerUnit = getContainerUnit(id);
	containerUnit->addUnit(unit);
	return true;
}
//--------------------------------------------------------------------------------------------
int ChartData::getBlockType(const wstring& BlockName) const
{
	if (block_types.count(BlockName) == 0) return 0;
	return block_types.at(BlockName);
}
//--------------------------------------------------------------------------------------------
wstring ChartData::getAdministrationsBlockName() const
{
	for (auto& it : block_types)
		if (it.second == static_cast<int>(BLOCK_TYPE::ADMINISTRATIONS))
			return it.first;

	return L"";
}
//--------------------------------------------------------------------------------------------
bool ChartData::Deserialize(const JSON_Value& value)
{
	// ������ ������ ������
	for (auto blockIt = value.Begin(); blockIt != value.End(); ++blockIt)
	{
		wstring blockName = (*blockIt)[L"name"].GetString();
		block_types[blockName] = (*blockIt)[L"type"].GetInt();
		const auto& lines = (*blockIt)[L"lines"];

		//addBlock(blockName); // - �� �����������, ��� ��� � addParameter ��������� ������ � key_vector
		if (lines.IsArray())
		{
			for (auto lineIt = lines.Begin(); lineIt != lines.End(); ++lineIt)
			{
				wstring param_name = (*lineIt)[0].GetString();
				FIELD_TYPE type = ((*lineIt)[1].GetString() == wstring(L"number")) ? FIELD_TYPE::NUMERIC : FIELD_TYPE::TEXT;
				addParameter(blockName, param_name, static_cast<int>(type));
			}
		}
	}
	return true;
}
//--------------------------------------------------------------------------------------------
bool ChartData::Serialize(JSON_Value& value)
{
	/*JSON_Value val;
	JSON_Document document;
	auto& allocator = document.GetAllocator();

	document.SetObject();

	val.SetArray();

	// ����������
	JSON_StringBuffer buffer;
	JSON_Writer writer(buffer);
	document.Accept(writer);
	json = buffer.GetString();*/
	return true;
}