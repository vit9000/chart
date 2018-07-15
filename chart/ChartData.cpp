#include "stdafx.h"
#include "ChartData.h"
#include "AdminWay.h"



void ChartData::addBlock(const wstring& BlockName)
{
	if (administrations.count(BlockName) > 0) return;
	administrations[BlockName];
}
//--------------------------------------------------------------------------------------------
std::pair<ContainerUnit_Ptr, int> ChartData::addChildDrug(const ID& _id, const ID& host_id, const DrugInfo& drugInfo, const PatientInfo& patientInfo)
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
std::pair<ContainerUnit_Ptr, int> ChartData::addDrug(int pos, const ID& _id, const wstring& BlockName, int way_type, const DrugInfo& drugInfo, const PatientInfo& patientInfo)
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
std::pair<ContainerUnit_Ptr, int> ChartData::addParameter(int pos, const ID& id, const wstring& ParameterName, int type, const COLORREF& color, int LegendMark)
{
	ContainerUnit_Ptr param;
	switch (static_cast<FIELD_TYPE>(type))
	{
	default:
	case FIELD_TYPE::NUMERIC_WITH_SUMM:
		param = ContainerUnit_Ptr(new ContainerParameter(id, ParameterName, color, LegendMark));
		break;
	case FIELD_TYPE::NUMERIC_WITHOUT_SUMM:
		param = ContainerUnit_Ptr(new ContainerNumericWithoutSummParameter(id, ParameterName, color, LegendMark));
		break;
	case FIELD_TYPE::TEXT:
		param = ContainerUnit_Ptr(new ContainerTextParameter(id, ParameterName, color, LegendMark));
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
bool ChartData::loadChart(int time_type, double date, const wstring& visit_id)
{
	saveChart();
	clear();

	//MainBridge::getInstance().showLogDlg();

	auto func = [this](IDBResult& rs)
	{
		wstring old_block_name;
		while (!rs.Eof())
		{
			VCopier<wstring> vsc;
			if (chart_keyid.empty())
			{
				rs.GetStrValue(L"CHART_ID", vsc);
				chart_keyid = std::move(vsc);
			}
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
				addParameter(line_sortcode, line_id, line_text, data_type, color, legend_mark);
				loadUnits(line_id);
			}

			rs.Next();
		}
	};
	vector<QueryParameter> params;
	params.push_back(QueryParameter(L"VISIT_ID", visit_id));
	wstringstream ss;
	ss << time_type;
	params.push_back(QueryParameter(L"TIME_TYPE", ss.str()));
	CString temp = static_cast<COleDateTime>(date).Format(_T("%Y-%m-%d %H:%M:%S"));
	params.push_back(QueryParameter(L"DAT", temp.GetBuffer()));
	MainBridge::getInstance().sendSQLRequest(L"sql_GetChartStructure", params, func);

	MainBridge::getInstance().showLogDlg();
	return true;
}
//--------------------------------------------------------------------------------------------
void ChartData::loadUnits(const ID& line_id)
{
	vector<QueryParameter> params;
	params.push_back(QueryParameter(L"LINE_ID", line_id.getIndex()));
	auto func = [this, &line_id](IDBResult& rs)
	{
		auto cu_ptr = *find(line_id);
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
	};

	
	MainBridge::getInstance().sendSQLRequest(L"sql_LoadUnits", params, func);
}


void ChartData::saveChart() const
{
	for (const auto& block : administrations)
	{
		for (int pos = 0; pos < static_cast<int>(block.size()); pos++)
		{
			const ContainerUnit_Ptr& cu = block[pos];
			saveLine(cu); // сохраняем строку
			if(cu->size() == 0) continue;
			vector<Unit> units = cu->getUnits();
			for (const auto& unit : units)
			{
				
				saveUnit(cu->getID(), unit);
			}
		}
	}
}
void ChartData::saveUnit(const ID& line_id, const Unit& unit) const
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
		query = L"sql_Unit_Update";
		params.push_back(QueryParameter(L"UNIT_ID", unit.getDB_ID()));
	}
	
	//params.push_back(QueryParameter(L"UNIT_ID", unit.getDB_ID()));
	params.push_back(QueryParameter(L"VALUE", unit.getValue()));
	params.push_back(QueryParameter(L"START", unit.getStartStr()));
	params.push_back(QueryParameter(L"DURATION", unit.getDurationStr()));
	params.push_back(QueryParameter(L"STATUS", unit.getStatusStr()));

	MainBridge::getInstance().sendSQLRequest(query, params, nullptr);
}

void  ChartData::saveLine(const ContainerUnit_Ptr& cu_ptr) const
{
	MainBridge& bridge = MainBridge::getInstance();

	const wstring& db_id = cu_ptr->getID().getIndex();
	if (!db_id.empty() && db_id[0] != L'N')
		return;
	wstring query;
	vector<QueryParameter> params;
	params.push_back(QueryParameter(L"CHART_ID", chart_keyid));
	params.push_back(QueryParameter(L"ROOT_LINE_ID", L"-1"));
	const DrugInfo& di = cu_ptr->getDrugInfo();
	params.push_back(QueryParameter(L"DRUG_ID", di.id));
	params.push_back(QueryParameter(L"DRUGNAME", di.name));
	params.push_back(QueryParameter(L"DEFAULT_DOSE", di.dose));
	params.push_back(QueryParameter(L"DOSE_MEASURE_UNIT", di.ED));
	params.push_back(QueryParameter(L"DILUTION_PERC", di.percent));
	params.push_back(QueryParameter(L"ADMIN_TYPE", bridge.getAdminWayType(di.selected_adminWayCode)));
	
	auto func = [](IDBResult& rs)
	{
		if (!rs.Eof())
		{
			VCopier<wstring> vsc;
			rs.GetStrValue(L"ID", vsc);
			wstring parent_id = vsc;
		}
	};

	bridge.sendSQLRequest(L"sql_SaveNewDrugLine", params, func);

}
