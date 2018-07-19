#include "stdafx.h"
#include "ChartData.h"
#include "AdminWay.h"



void ChartData::addBlock(const wstring& BlockName)
{
	if (administrations.count(BlockName) > 0) return;
	administrations[BlockName];
}
//--------------------------------------------------------------------------------------------
std::pair<ContainerUnit_Ptr, int> ChartData::addChildDrug(const ID& _id, const ID& host_id, const DrugInfo& drugInfo)
{
	ID id(_id);
	if (id.isEmpty())
		id = getNewID(host_id.getBlockName());
	
	ContainerUnit_Ptr host_drug = getContainerUnit(host_id);
	DrugInfo di(std::move(drugInfo));
	di.selected_adminWayCode = host_drug->getDrugInfo().selected_adminWayCode;
	ContainerUnit_Ptr new_drug = ContainerUnit_Ptr(new ContainerIVdrops(id, di, false)); // принудительно запрещаем дополнительное разведение//addDrug(id, host_id.getBlockName(), ADMINWAY::COMBINED_DROPS, drugInfo, patientInfo);
	host_drug->addContainerUnit(new_drug);
	return make_pair(new_drug, host_drug->getChildsCount()-1);
}
//--------------------------------------------------------------------------------------------
std::pair<ContainerUnit_Ptr, int> ChartData::addDrug(int pos, const ID& _id, const wstring& BlockName, int way_type, const DrugInfo& drugInfo)
{
	ContainerUnit_Ptr drug;
	bool allowMakeSolution = false;
	ID id(_id);
	if (id.isEmpty())
	{
		id = getNewID(BlockName);
		allowMakeSolution = true;
	}
	switch (way_type)
	{
	/*case ADMINWAY::ADMIN_TYPE::COMBINED_DROPS: // drugToDrug IVdrops
		drug = ContainerUnit_Ptr(new ContainerIVdrops(id, drugInfo, false)); // принудительно запрещаем дополнительное разведение
		break;*/
	case ADMINWAY::ADMIN_TYPE::DROPS: // IVdrops host
		drug = ContainerUnit_Ptr(new ContainerIVdrops(id, drugInfo, allowMakeSolution)); // разрешаем разведение препарата, если требуется
		break;
	case ADMINWAY::ADMIN_TYPE::INFUSION: // в/в дозатором, эпидурально дозатором
		drug = ContainerUnit_Ptr(new ContainerInfusion(id, drugInfo, 70., allowMakeSolution));
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
std::pair<ContainerUnit_Ptr, int> ChartData::addParameter(int pos, const ID& id, const wstring& ParameterName, const wstring& measure_unit, int type, const COLORREF& color, int LegendMark)
{
	ContainerUnit_Ptr param;
	switch (static_cast<FIELD_TYPE>(type))
	{
	default:
	case FIELD_TYPE::NUMERIC_WITH_SUMM:
		param = ContainerUnit_Ptr(new ContainerParameter(id, ParameterName, measure_unit, color, LegendMark));
		break;
	case FIELD_TYPE::NUMERIC_WITHOUT_SUMM:
		param = ContainerUnit_Ptr(new ContainerNumericWithoutSummParameter(id, ParameterName, measure_unit, color, LegendMark));
		break;
	case FIELD_TYPE::TEXT:
		param = ContainerUnit_Ptr(new ContainerTextParameter(id, ParameterName, measure_unit, color, LegendMark));
		break;
	}
	//administrations[BlockName][param->getID().getIndex()] = param;
	insertIntoAdministrations(pos, param);
	return make_pair(param, administrations[id.getBlockName()].size()-1);//administrations.size() - 1;
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
bool ChartData::loadChart(const wstring& ChartKEYID)
{
	chart_keyid = std::move(ChartKEYID);

	//MainBridge::getInstance().showLogDlg();

	auto func = [this](IDBResult& rs)
	{
		wstring old_block_name;
		while (!rs.Eof())
		{
			VCopier<wstring> vsc;
			rs.GetStrValue(L"SECTION_TEXT", vsc);
			wstring blockName = std::move(vsc);
			if (blockName != old_block_name)
			{
				old_block_name = blockName;
				addBlock(blockName);
				block_types[blockName] = rs.GetIntValue(L"SECTION_TYPE");
			}

			rs.GetStrValue(L"LINE_TEXT", vsc);
			wstring line_text = std::move(vsc);
			if (!line_text.empty())
			{
				rs.GetStrValue(L"LINE_ID", vsc);
				wstring section_id = vsc;
				int data_type = rs.GetIntValue(L"DATA_TYPE");
				int line_sortcode = rs.GetIntValue(L"LINE_SORTCODE");
				rs.GetStrValue(L"COLOR", vsc);
				COLORREF color = textToColor(vsc);
				int legend_mark = rs.GetIntValue(L"LEGEND_MARK");
				
				ID line_id(blockName, section_id);
				rs.GetStrValue(L"PRODUCT_FORM_ID", vsc);
				pair<ContainerUnit_Ptr, int> pair_cu_ptr;
				if (static_cast<wstring>(vsc).empty()) // значит это параметр
				{
					rs.GetStrValue(L"MEASURE_UNIT", vsc);
					pair_cu_ptr = addParameter(line_sortcode, line_id, line_text, vsc, data_type, color, legend_mark);
				}
				else // если препарат, то загружаем всю нужную информацию
				{
					DrugInfo di;
					di.id = std::move(vsc);
					rs.GetStrValue(L"ROOT_LINE_ID", vsc);
					wstring parent_id = std::move(vsc);
					
					int admin_type = rs.GetIntValue(L"ADMIN_TYPE");

					di.dose = rs.GetFloatValue(L"DOSE");
					
					rs.GetStrValue(L"DOSE_MEASURE_UNIT", vsc);
					di.ED = std::move(vsc);
					
					di.percent = rs.GetFloatValue(L"DILUTION_PERC");

					di.name = line_text;
					
					rs.GetStrValue(L"PRODUCT_FORM", vsc);
					di.drug_form = std::move(vsc);
					
					di.selected_adminWayCode = rs.GetIntValue(L"ADMIN_CODE");

					if (parent_id.empty())
					{
						pair_cu_ptr = addDrug(line_sortcode, line_id, line_id.getBlockName(), admin_type, di);
					}
					else
					{
						pair_cu_ptr = addChildDrug(line_id, ID(line_id.getBlockName(), parent_id), di);
					}

				}
				loadUnits(pair_cu_ptr.first);
			}
			rs.Next();
		}
	};
	vector<QueryParameter> params;
	params.push_back(QueryParameter(L"CHART_ID", chart_keyid));
	MainBridge::getInstance().sendSQLRequest(L"sql_GetChartStructure", params, func);

	//MainBridge::getInstance().showLogDlg();
	return true;
}
//--------------------------------------------------------------------------------------------
void ChartData::loadUnits(const ContainerUnit_Ptr& cu_ptr)
{
	vector<QueryParameter> params;
	params.push_back(QueryParameter(L"LINE_ID", cu_ptr->getID().getIndex()));
	auto func = [this, &cu_ptr](IDBResult& rs)
	{
		while (!rs.Eof())
		{
			VCopier<wstring> vsc;
			rs.GetStrValue(L"UNIT_ID", vsc);
			wstring unit_id = std::move(vsc);

			rs.GetStrValue(L"UNIT_VALUE", vsc);
			wstring value = std::move(vsc);

			int start = rs.GetIntValue(L"START_FROM");
			int duration = rs.GetIntValue(L"DURATION");
			int status = rs.GetIntValue(L"STATUS");

			cu_ptr->loadUnit(Unit(unit_id, value, start, duration, status));

			rs.Next();
		}
		cu_ptr->calculateSumm();
	};
	MainBridge::getInstance().sendSQLRequest(L"sql_LoadUnits", params, func);
}
//--------------------------------------------------------------------------------------------
void ChartData::saveChart(LogCommandAdministrator& logger) const
{
	set<wstring> updated_containers_ids, updated_units_ids;
	logger.getUpdatedUnitsIDs(updated_containers_ids, updated_units_ids); // получаем список ID

	for (const auto& block : administrations)
	{
		for (int pos = 0; pos < static_cast<int>(block.size()); pos++)
		{
			const ContainerUnit_Ptr& cu_ptr = block[pos];
			saveLine(updated_containers_ids, cu_ptr, pos); // сохраняем строку, а также дочерние строки
			saveUnits(updated_units_ids, cu_ptr); // сохраняем юниты строк parent и child
		}
	}
}
//--------------------------------------------------------------------------------------------
void ChartData::saveUnits(const set<wstring>& updated_units_ids, const ContainerUnit_Ptr& cu_ptr) const
{
	if (cu_ptr->size() == 0) return;

	vector<Unit> units = cu_ptr->getUnits();
	for (const auto& unit : units)
	{
		saveUnit(updated_units_ids, cu_ptr->getID(), unit);
	}
	if (cu_ptr->isParent())
	{
		for (const auto& child_cu_ptr : cu_ptr->getChilds())
		{
			saveUnits(updated_units_ids, child_cu_ptr); // рекурсия
		}
	}
}
//--------------------------------------------------------------------------------------------
void ChartData::saveUnit(const set<wstring>& updated_units_ids, const ID& line_id, const Unit& unit) const
{
	wstring query;
	vector<QueryParameter> params;
	params.reserve(6);
	if (unit.getDB_ID().empty())
	{
		query = L"sql_Unit_New";
		params.push_back(QueryParameter(L"LINE_ID", line_id.getIndex()));
	}
	else
	{
		const wstring& unit_db_id = unit.getDB_ID();
		if (updated_units_ids.count(unit_db_id) == 0) return;// если нет id в списке измененных, то обновлять его не надо
		// иначе отправляем запрос на обновление
		query = L"sql_Unit_Update";
		params.push_back(QueryParameter(L"UNIT_ID", unit_db_id));
	}
	
	//params.push_back(QueryParameter(L"UNIT_ID", unit.getDB_ID()));
	params.push_back(QueryParameter(L"VALUE", unit.getValue()));
	params.push_back(QueryParameter(L"START", unit.getStartStr()));
	params.push_back(QueryParameter(L"DURATION", unit.getDurationStr()));
	params.push_back(QueryParameter(L"STATUS", unit.getStatusStr()));

	MainBridge::getInstance().sendSQLRequest(query, params, nullptr);
}

void ChartData::saveLine(set<wstring>& updated_containers_ids, const ContainerUnit_Ptr& cu_ptr, int sortcode, const wstring& db_keyid) const
{
	if (!cu_ptr) return;
	// данная функция с рекурсией
	
	MainBridge& bridge = MainBridge::getInstance();

	// если родитель сохранен, то запускается эта же функция для деток 
	const wstring& db_id = cu_ptr->getID().getIndex();
	if (!db_id.empty() && db_id[0] != L'N') // если сохранено, то проверяем родитель или нет
	{
		if (cu_ptr->isParent())
		{
			if (cu_ptr->isParent())
			{
				int pos = 1000;
				for (const auto& child : cu_ptr->getChilds())
				{
					saveLine(updated_containers_ids, child, pos, db_id);
					pos++;
				}
			}
		}
		// обновляем позицию только у лекарственных строк, так как именно они двигаются и добавляются
		if (cu_ptr->isDrugContainer())
			updateLinePos(updated_containers_ids, cu_ptr, sortcode);
		return;
	}
	
	// если не сохранен, то сохраняется он, получается parent_id и запускается эта же функция с parent_id 
	//wstring query;
	vector<QueryParameter> params;
	params.push_back(QueryParameter(L"CHART_ID", chart_keyid));
	params.push_back(QueryParameter(L"SORTCODE", sortcode));
	params.push_back(QueryParameter(L"ROOT_LINE_ID", (!db_keyid.empty() && cu_ptr->isChild())? db_keyid : L"-1"));
	const DrugInfo& di = cu_ptr->getDrugInfo();
	params.push_back(QueryParameter(L"DRUG_ID", di.id));
	params.push_back(QueryParameter(L"DRUGNAME", di.name));
	params.push_back(QueryParameter(L"DEFAULT_DOSE", di.dose));
	params.push_back(QueryParameter(L"DOSE_MEASURE_UNIT", di.ED));
	params.push_back(QueryParameter(L"DILUTION_PERC", di.percent));
	params.push_back(QueryParameter(L"PRODUCT_FORM_TEXT", di.drug_form));
	params.push_back(QueryParameter(L"ADMIN_TYPE", bridge.getAdminWayType(di.selected_adminWayCode)));
	params.push_back(QueryParameter(L"ADMIN_CODE", di.selected_adminWayCode));
	
	auto func = [this, &cu_ptr, &updated_containers_ids](IDBResult& rs)
	{
		if (!rs.Eof())
		{
			VCopier<wstring> created_line_id;
			rs.GetStrValue(L"ID", created_line_id);
			ContainerUnit& cu = *cu_ptr;
			cu.replaceID(ID(cu.getID().getBlockName(), created_line_id));
			if (cu_ptr->isParent())
			{
				int pos = 1000;
				for (const auto& child : cu_ptr->getChilds())
				{
					saveLine(updated_containers_ids, child, pos, created_line_id);
					pos++;
				}
			}
		}
	};

	bridge.sendSQLRequest(L"sql_SaveNewDrugLine", params, func);

}
//-----------------------------------------------------
void ChartData::updateLinePos(set<wstring>& updated_containers_ids, const ContainerUnit_Ptr& cu_ptr, int sortcode) const
{
	const wstring& db_id = cu_ptr->getID().getIndex();
	if (updated_containers_ids.count(db_id) == 0) return; // если нет id в списке измененных, то обновлять его не надо
	// иначе запрос на обновление
	vector<QueryParameter> params;
	params.push_back(QueryParameter(L"SORTCODE", sortcode));
	params.push_back(QueryParameter(L"LINE_ID", db_id));
	MainBridge::getInstance().sendSQLRequest(L"sql_UpdateLinePos", params, nullptr);
}
