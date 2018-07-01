#include "stdafx.h"
#include "ChartData.h"
#include "AdminWay.h"



void ChartData::addBlock(const wstring& BlockName)
{
	if (administrations.count(BlockName) > 0) return;
	administrations[BlockName];
}
//--------------------------------------------------------------------------------------------
ContainerUnit_Ptr ChartData::addChildDrug(const ID& id, const ID& host_id, const DrugInfo& drugInfo, const DBPatient& patientInfo)
{
	ContainerUnit_Ptr new_drug = addDrug(id, host_id.getBlockName(), ADMINWAY::COMBINED_DROPS, drugInfo, patientInfo);
	ContainerUnit_Ptr host_drug = getContainerUnit(host_id);
	host_drug->linkContainerUnit(new_drug.get());
	return new_drug;
}
//--------------------------------------------------------------------------------------------
ContainerUnit_Ptr ChartData::addDrug(const ID& _id, const wstring& BlockName, int way, const DrugInfo& drugInfo, const DBPatient& patientInfo)
{
	ContainerUnit_Ptr drug;
	ID id(_id);
	if(id.isEmpty())
		id = getNewID(BlockName);
	switch (way)
	{
	case ADMINWAY::ADMIN_TYPE::COMBINED_DROPS: // drugToDrug IVdrops
		drug = ContainerUnit_Ptr(new ContainerIVdrops(id, drugInfo, false)); // принудительно запрещаем дополнительное разведение
		break;
	case ADMINWAY::ADMIN_TYPE::DROPS: // IVdrops host
		drug = ContainerUnit_Ptr(new ContainerIVdrops(id, drugInfo, true)); // разрешаем разведение препарата, если требуется
		break;
	case ADMINWAY::ADMIN_TYPE::INFUSION: // в/в дозатором, эпидурально дозатором
		drug = ContainerUnit_Ptr(new ContainerInfusion(id, drugInfo, patientInfo.weight));
		break;
	case ADMINWAY::ADMIN_TYPE::BOLUS: // болюсно, в/м, п/к и т.д. - введение раствора
		drug = ContainerUnit_Ptr(new ContainerSolution(id, drugInfo));
		break;
	default:// остальные пути введения
		drug = ContainerUnit_Ptr(new ContainerUnitMovable(id, drugInfo));
		break;
	}

	insertIntoAdministrations(drug);
	return drug;
}
//--------------------------------------------------------------------------------------------
ID ChartData::getNewID(const wstring& BlockName, const wstring& DB_ID)
{
	wstring db_id(DB_ID);
	if (db_id.empty())
	{
		static int i = 0;
		i++;
		std::wstringstream ss;
		ss << i;
		db_id = ss.str();
	}
	return ID(BlockName, db_id);
}
//--------------------------------------------------------------------------------------------
void ChartData::insertIntoAdministrations(const ContainerUnit_Ptr& item)
{
	const auto& id = item->getID();
	administrations[id.getBlockName()][id.getIndex()] = (item);
}
//--------------------------------------------------------------------------------------------
const ContainerUnit_Ptr& ChartData::getContainerUnit(const ID& id)
{
	if (administrations.count(id.getBlockName()) == 0) throw out_of_range("getContainerUnit: BlockName is not exists");
	auto& block = administrations[id.getBlockName()];
	if (block.count(id.getIndex())==0) 
		throw out_of_range("getContainerUnit out of range map<int, ContainerUnit_Ptr>");

	return block[id.getIndex()];
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
/*bool ChartData::addUnit(const ID& id, const Unit& unit)
{
	ContainerUnit_Ptr containerUnit = getContainerUnit(id);
	if (!containerUnit->addUnit(unit))
	{
		MessageBox(parentDlg->m_hWnd, L"Допускается введение только числовых значений", L"Внимание", MB_OK);
		return false;
	}
	return true;
}*/
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
	// читаем массив блоков
	for (auto blockIt = value.Begin(); blockIt != value.End(); ++blockIt)
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
				FIELD_TYPE type = (wstring((*lineIt)[1].GetString()) == wstring(L"number")) ? FIELD_TYPE::NUMERIC : FIELD_TYPE::TEXT;
				addParameter(blockName, param_name, static_cast<int>(type));
			}
		}
	}
	return true;
}
//--------------------------------------------------------------------------------------------
bool ChartData::Serialize(JSON_Value& value, JSON_Allocator& allocator)
{
	using jvalue = JSON_Value;
	using namespace rapidjson;
	
	for (size_t i = 0; i < administrations.size(); i++)
	{
		const auto& blockName = administrations.first(i);
		int type = block_types[blockName];

		jvalue block(kObjectType);
		block.AddMember(L"name", jvalue().SetString(blockName.c_str(), allocator), allocator);
		block.AddMember(L"type", jvalue().SetInt(type), allocator);

		jvalue lines(kArrayType);
		for (const auto& containerUnitPtr : administrations.second(i))
		{
			jvalue item(kArrayType);
			containerUnitPtr.second->Serialize(item, allocator);
			lines.PushBack(item, allocator);
		}
		block.AddMember(L"lines",lines, allocator);

		
		value.PushBack(block, allocator);
	}
	
	return true;
}
//--------------------------------------------------------------------------------------------
LogCommandPtr ChartData::deleteDrug(const ID& id)
{

	auto& block = administrations[id.getBlockName()];
	auto& container = block[id.getIndex()];
	if (!container) return nullptr;

	LogCommand_Union* com = new LogCommand_Union();
	com->add(LogCommandPtr(new LogCommand_DeleteDrug(*container)));

	for (const ContainerUnit* child_ptr : container->getChilds()) // если есть childs, то их удалить сначала
	{
		com->add(LogCommandPtr(new LogCommand_DeleteChildDrug(*child_ptr)));
		block.erase(child_ptr->getID().getIndex());
	}

	administrations[id.getBlockName()].erase(id.getIndex()); // затем удаляем parent
	return LogCommandPtr(com);
}

LogCommandPtr ChartData::deleteChildDrug(const ID& id)
{
	auto& block = administrations[id.getBlockName()];
	auto& container = block[id.getIndex()];
	if (!container) return nullptr;

	LogCommandPtr com (new LogCommand_DeleteChildDrug(*container));
	administrations[id.getBlockName()].erase(id.getIndex()); // затем удаляем parent
	return com;
}

LogCommandPtr ChartData::moveDrug(const ID& id, int new_pos)
{
	/*auto& block = administrations[id.getBlockName()];
	auto container = block[id.getIndex()];
	block.erase(id.getIndex());
	block.insert(new_pos, id.getIndex(), container);*/
	
	return nullptr;
}