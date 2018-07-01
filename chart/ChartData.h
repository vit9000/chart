#pragma once

#include <memory>
#include <map>
#include <string>
#include <vector>

#include "ID.h"
#include "DBPatient.h"
#include "ContainerUnit.h"
#include "Constants.h"
#include "ModelContainers.h"
#include "Serializable.h"
#include "key_vector.h"
#include "LogCommand_ContainerUnits.h"
#include "LogCommand_Units.h"

using namespace std;

typedef shared_ptr<ContainerUnit> ContainerUnit_Ptr;
//typedef map<wstring, vector<ContainerUnit_Ptr>> Data;
typedef key_vector<wstring, map<wstring, ContainerUnit_Ptr>> Data;
 //		<block_name, <id, container_unit>>

extern CWnd * parentDlg;

class ChartData : public Serializable
{
private:
	wstring date;

	Data administrations;
	map<wstring, int> block_types;

private:
	void insertIntoAdministrations(const ContainerUnit_Ptr& item);
public:
	ChartData()
	{
	}
	ChartData(const wstring& Date)
		: date(Date)
	{}
	
	inline const Data& getAdministrations() const { return administrations; }
	inline const wstring& getDate() const { return date; }
	// функции для формирования массива данных
	ID getNewID(const wstring& BlockName, const wstring& DB_ID = L"");
	void addBlock(const wstring& BlockName); // блоки
	ContainerUnit_Ptr addDrug(const ID& id, const wstring& BlockName, int type, const DrugInfo& drugInfo, const DBPatient& patientInfo);// просто лекарство
	ContainerUnit_Ptr addChildDrug(const ID& id, const ID& host_id,const DrugInfo& drugInfo, const DBPatient& patientInfo);// составные капельницы
	ContainerUnit_Ptr addParameter(const wstring& BlockName, const wstring& ParameterName, int type); // обычный показатель
	LogCommandPtr deleteDrug(const ID& id);
	LogCommandPtr deleteChildDrug(const ID& id);
	
	LogCommandPtr moveDrug(const ID& id, int new_pos);


	const ContainerUnit_Ptr& getContainerUnit(const ID& id);// получение строки по ID
	//bool addUnit(const ID& id, const Unit& unit);// добавление данных в строку
	//bool updateUnit(const ID& id, const Unit& unit);// добавление данных в строку
	
	int getBlockType(const wstring& BlockName) const;
	wstring getAdministrationsBlockName() const;
	// Serializable
	bool Deserialize(const JSON_Value& value) override;
	bool Serialize(JSON_Value& value, JSON_Allocator& allocator) override;

};