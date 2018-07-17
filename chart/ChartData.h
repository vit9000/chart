#pragma once

#include <memory>
#include <map>
#include <string>
#include <vector>

#include "ID.h"
#include "ContainerUnit.h"
#include "Constants.h"
#include "ModelContainers.h"

#include "key_vector.h"
#include "LogCommand_ContainerUnits.h"
#include "LogCommand_Units.h"
#include "MainBridge.h"
#include "utils.h"

#include "IDBConnector.h"

using namespace std;

typedef shared_ptr<ContainerUnit> ContainerUnit_Ptr;
typedef key_vector<wstring, vector<ContainerUnit_Ptr>> Data;//<block_name, <id, container_unit>>

extern CWnd * parentDlg;

class ChartData
{
private:
	wstring chart_keyid;
	Data administrations;
	map<wstring, int> block_types;
	PatientInfo patientInfo;
private:
	int insertIntoAdministrations(int pos, const ContainerUnit_Ptr& item);
public:
	void setPatient(int index)
	{
		patientInfo = MainBridge::getInstance().getPatientList(NULL)[index];
	}
	const PatientInfo& getPatientInfo() { return patientInfo; }

	ChartData()
	{
	}
	inline void clear()
	{
		chart_keyid.clear();
		administrations.clear();
		block_types.clear();
	}


	std::vector<ContainerUnit_Ptr>::iterator find(const ID& id);
	inline const Data& getAdministrations() const { return administrations; }
	// функции для формирования массива данных
	ID getNewID(const wstring& BlockName, const wstring& DB_ID = L"");
	void addBlock(const wstring& BlockName); // блоки
	std::pair<ContainerUnit_Ptr, int> addDrug(int pos, const ID& id, const wstring& BlockName, int way_type, const DrugInfo& drugInfo);// просто лекарство
	std::pair<ContainerUnit_Ptr, int> addChildDrug(const ID& id, const ID& host_id,const DrugInfo& drugInfo);// составные капельницы
	std::pair<ContainerUnit_Ptr, int> addParameter(int pos, const ID& id, const wstring& ParameterName, const wstring& measure_unit, int type, const COLORREF& color, int LegendMark); // обычный показатель

	LogCommandPtr deleteDrug(const ID& id);
	LogCommandPtr deleteChildDrug(const ID& id);
	LogCommandPtr updateDrugPos(const ID& id, int new_pos);

	ContainerUnit_Ptr getContainerUnit(const ID& id);// получение строки по ID
	
	int getBlockType(const wstring& BlockName) const;
	wstring getAdministrationsBlockName() const;

	// работа с БД
	bool loadChart(const wstring& ChartKEYID);
	void loadUnits(const ContainerUnit_Ptr& cu_ptr);
	void saveChart() const;
	void saveUnits(const ContainerUnit_Ptr& cu_ptr) const;
	void saveUnit(const ID& line_id, const Unit& unit) const;
	void saveLine(const ContainerUnit_Ptr& cu_ptr, const wstring& db_keyid = L"") const;
};