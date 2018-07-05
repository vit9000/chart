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
//typedef map<wstring, vector<ContainerUnit_Ptr>> Data;
typedef key_vector<wstring, vector<ContainerUnit_Ptr>> Data;
 //		<block_name, <id, container_unit>>

extern CWnd * parentDlg;

class ChartData
{
private:
	wstring date;

	Data administrations;
	map<wstring, int> block_types;

private:
	int insertIntoAdministrations(int pos, const ContainerUnit_Ptr& item);
public:
	ChartData()
	{
	}
	ChartData(const wstring& Date)
		: date(Date)
	{}
	
	std::vector<ContainerUnit_Ptr>::iterator find(const ID& id);
	inline const Data& getAdministrations() const { return administrations; }
	inline const wstring& getDate() const { return date; }
	// функции для формирования массива данных
	ID getNewID(const wstring& BlockName, const wstring& DB_ID = L"");
	void addBlock(const wstring& BlockName); // блоки
	std::pair<ContainerUnit_Ptr, int> addDrug(int pos, const ID& id, const wstring& BlockName, int type, const DrugInfo& drugInfo, const PatientInfo& patientInfo);// просто лекарство
	std::pair<ContainerUnit_Ptr, int> addChildDrug(const ID& id, const ID& host_id,const DrugInfo& drugInfo, const PatientInfo& patientInfo);// составные капельницы
	std::pair<ContainerUnit_Ptr, int> addParameter(int pos, const wstring& BlockName, const wstring& ParameterName, int type, const COLORREF& color, int LegendMark); // обычный показатель
	LogCommandPtr deleteDrug(const ID& id);
	LogCommandPtr deleteChildDrug(const ID& id);
	
	LogCommandPtr updateDrugPos(const ID& id, int new_pos);


	ContainerUnit_Ptr getContainerUnit(const ID& id);// получение строки по ID
	//bool addUnit(const ID& id, const Unit& unit);// добавление данных в строку
	//bool updateUnit(const ID& id, const Unit& unit);// добавление данных в строку
	
	int getBlockType(const wstring& BlockName) const;
	wstring getAdministrationsBlockName() const;
	// Serializable
	
	bool loadChartTemplate();
};