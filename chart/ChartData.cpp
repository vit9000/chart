#include "stdafx.h"
#include "ChartData.h"
#include "AdminWay.h"



void ChartData::addBlock(const wstring& BlockName)
{
	if (administrations.count(BlockName) > 0) return;
	administrations[BlockName];
}
//--------------------------------------------------------------------------------------------
std::pair<ContainerUnit_Ptr, int> ChartData::addChildDrug(const ID& _id, const ID& host_id, const DrugInfo& drugInfo, const DBPatient& patientInfo)
{
	ID id(_id);
	if (id.isEmpty())
		id = getNewID(host_id.getBlockName());
	ContainerUnit_Ptr new_drug = ContainerUnit_Ptr(new ContainerIVdrops(id, drugInfo, false)); // принудительно запрещаем дополнительное разведение//addDrug(id, host_id.getBlockName(), ADMINWAY::COMBINED_DROPS, drugInfo, patientInfo);
	ContainerUnit_Ptr host_drug = getContainerUnit(host_id);
	host_drug->addContainerUnit(new_drug);
	return make_pair(new_drug, host_drug->getChildsCount()-1);
}
//--------------------------------------------------------------------------------------------
std::pair<ContainerUnit_Ptr, int> ChartData::addDrug(int pos, const ID& _id, const wstring& BlockName, int way, const DrugInfo& drugInfo, const DBPatient& patientInfo)
{
	ContainerUnit_Ptr drug;
	bool allowMakeSolution = false;
	ID id(_id);
	if (id.isEmpty())
	{
		id = getNewID(BlockName);
		allowMakeSolution = true;
	}
	switch (way)
	{
	/*case ADMINWAY::ADMIN_TYPE::COMBINED_DROPS: // drugToDrug IVdrops
		drug = ContainerUnit_Ptr(new ContainerIVdrops(id, drugInfo, false)); // принудительно запрещаем дополнительное разведение
		break;*/
	case ADMINWAY::ADMIN_TYPE::DROPS: // IVdrops host
		drug = ContainerUnit_Ptr(new ContainerIVdrops(id, drugInfo, allowMakeSolution)); // разрешаем разведение препарата, если требуется
		break;
	case ADMINWAY::ADMIN_TYPE::INFUSION: // в/в дозатором, эпидурально дозатором
		drug = ContainerUnit_Ptr(new ContainerInfusion(id, drugInfo, patientInfo.weight, allowMakeSolution));
		break;
	case ADMINWAY::ADMIN_TYPE::BOLUS: // болюсно, в/м, п/к и т.д. - введение раствора
		drug = ContainerUnit_Ptr(new ContainerSolution(id, drugInfo, allowMakeSolution));
		break;
	default:// остальные пути введения
		drug = ContainerUnit_Ptr(new ContainerUnitMovable(id, drugInfo));
		break;
	}
	pos = insertIntoAdministrations(pos, drug);
	return make_pair(drug, pos);
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
		ss << L"N" << i;
		db_id = ss.str();
	}
	return ID(BlockName, db_id);
}
//--------------------------------------------------------------------------------------------
int ChartData::insertIntoAdministrations(int pos, const ContainerUnit_Ptr& item)
{
	const ID& id = item->getID();
	auto& block = administrations[id.getBlockName()];

	if (pos < 0)
	{
		if (item->getAdminWay() < 0)
		{
			block.push_back(item);
			return block.size() - 1;
		}

		auto temp_it = block.end();
		for (auto it = block.begin(); it != block.end(); ++it)
		{
			if ((*it)->getAdminWay() == item->getAdminWay())// если найден, идем дальше до конца
			{
				while (it != block.end() && (*it)->getAdminWay() == item->getAdminWay())
				{
					it++;
				}
				temp_it = it;
				break;
			}
			else if (temp_it == block.end() && (*it)->getAdminWay() > item->getAdminWay())
				temp_it = it;

		}
		pos = std::distance(block.begin(), temp_it);
	}
	block.insert(block.begin()+pos, item);
	return pos;
}
//--------------------------------------------------------------------------------------------
std::vector<ContainerUnit_Ptr>::iterator ChartData::find(const ID& id)
{
	auto& block = administrations[id.getBlockName()];
	auto it = std::find_if(block.begin(), block.end(), [&id](const ContainerUnit_Ptr& rhs) -> bool { return id == rhs->getID(); });
	return it;
}


ContainerUnit_Ptr ChartData::getContainerUnit(const ID& id)
{
	if (administrations.count(id.getBlockName()) == 0) throw out_of_range("getContainerUnit: BlockName is not exists");
	auto& block = administrations[id.getBlockName()];
	
	for (auto& cu : block)
	{
		if (id == cu->getID())
			return cu;
		else if (cu->isParent())
		{
			for (auto& child_cu : cu->getChilds())
			{
				if (id == child_cu->getID())
					return child_cu;
			}
		}
	}
	return nullptr;
}
//--------------------------------------------------------------------------------------------
std::pair<ContainerUnit_Ptr, int> ChartData::addParameter(int pos, const wstring& BlockName, const wstring& ParameterName, int type)
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
	insertIntoAdministrations(pos, param);
	return make_pair(param, administrations[BlockName].size()-1);//administrations.size() - 1;
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
			int pos = 0;
			for (auto lineIt = lines.Begin(); lineIt != lines.End(); ++lineIt)
			{
				wstring param_name = (*lineIt)[0].GetString();
				FIELD_TYPE type = (wstring((*lineIt)[1].GetString()) == wstring(L"number")) ? FIELD_TYPE::NUMERIC : FIELD_TYPE::TEXT;
				addParameter(pos, blockName, param_name, static_cast<int>(type));
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
			containerUnitPtr->Serialize(item, allocator);
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
	auto it = find(id);
	if (it == block.end())
		return nullptr;
	ContainerUnit_Ptr& container = *it;
	int pos = std::distance(block.begin(), it);
	LogCommand_Union* com = new LogCommand_Union();
	com->add(LogCommandPtr(new LogCommand_DeleteDrug(*container, pos)));

	for (const ContainerUnit_Ptr& child_ptr : container->getChilds()) // если есть childs, то их удалить сначала
	{
		com->add(LogCommandPtr(new LogCommand_DeleteChildDrug(*child_ptr)));
		child_ptr->deleteFromParent();
		//block.erase(find(child_ptr->getID()));
	}
	block.erase(find(id)); // затем удаляем parent
	return LogCommandPtr(com);
}
//--------------------------------------------------------------------------------------------
LogCommandPtr ChartData::deleteChildDrug(const ID& id)
{
	auto& block = administrations[id.getBlockName()];
	auto it = find(id);
	if(it==block.end())
		return nullptr;

	LogCommandPtr com (new LogCommand_DeleteChildDrug(**it));
	block.erase(it); // затем удаляем parent
	return com;
}
//--------------------------------------------------------------------------------------------
LogCommandPtr ChartData::updateDrugPos(const ID& id, int new_pos)
{
	auto& block = administrations[id.getBlockName()];
	auto it = find(id);
	if (it == block.end())
		return nullptr;
	int old_pos = (int) std::distance(block.begin(), it);
	ContainerUnit_Ptr backup = *it;
	block.erase(it);
	block.insert(block.begin() + new_pos, backup);

	return LogCommandPtr(new LogCommand_MoveDrug(id, new_pos, old_pos));
}
//--------------------------------------------------------------------------------------------