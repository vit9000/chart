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

using namespace std;

typedef shared_ptr<ContainerUnit> ContainerUnit_Ptr;
typedef map<wstring, vector<ContainerUnit_Ptr>> Data;
 //		<block_name, <id, container_unit>>


class ChartData : public Serializable
{
private:
	wstring date;
	Data administrations;
	vector<const wstring*> blocks;//������ ������������������ ������
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
	// ������� ��� ������������ ������� ������
	void addBlock(const wstring& BlockName); // �����
	ContainerUnit_Ptr addDrug(const wstring& BlockName, int type, const DrugInfo& drugInfo, const DBPatient& patientInfo);// ������ ���������
	ContainerUnit_Ptr addDrugToDrug(const ID& host_id, int type, const DrugInfo& drugInfo, const DBPatient& patientInfo);// ��������� ����������
	ContainerUnit_Ptr addParameter(const wstring& BlockName, const wstring& ParameterName, int type); // ������� ����������
	
	const ContainerUnit_Ptr& getContainerUnit(const ID& id);// ��������� ������ �� ID
	bool addUnit(const ID& id, const Unit& unit);// ���������� ������ � ������
	
	inline const vector<const wstring*>& getBlockNames() const { return blocks; }//!!!!!!!!!!!! ����������� �� ��������
	int getBlockType(const wstring& BlockName) const;
	wstring getAdministrationsBlockName() const;
	// Serializable
	bool Deserialize(const JSON_Value& value) override;
	bool Serialize(JSON_Value& value) override;

};