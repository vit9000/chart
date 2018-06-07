#include "stdafx.h"
#include "DatabaseLoader.h"

DatabaseLoader * DatabaseLoader::p_instance = 0;
DatabaseLoader::DatabaseLoaderDestroyer DatabaseLoader::destroyer;


DatabaseLoader::DatabaseLoader()
	: db_connector(nullptr)
{
	
	loadAllowedAdminWays();
	
	//LoadPatientChart(0);// �������� ������ �������� �� ������
}
//--------------------------------------------------------------------------------------------------------
DatabaseLoader& DatabaseLoader::getInstance() 
{
	if (!p_instance) {
		p_instance = new DatabaseLoader();
		destroyer.initialize(p_instance);
	}
	return *p_instance;
}
//--------------------------------------------------------------------------------------------------------
void DatabaseLoader::LoadPatientChartByIndex(int index)
{
	if (patientList.size() == 0) return;

	std::wstring fileJSON;
	db_connector->getChartJSON(patientList[index], GET_STRING_LAMBDA(fileJSON));//[&fileJSON](const wstring& result) { fileJSON = result; });
	LoadPatientChartJSON(fileJSON);
}

void DatabaseLoader::LoadPatientChartJSON(const std::wstring& fileJSON)
{
	//auto patient = getPatient(index);
	//auto med_card_ID = patient.case_number;
	/*
	����� ����������� �������� ����� �� ���� ������,
	� ���� ����������� �������� ���������� �����
	*/
	
	
	
	administrations = ChartData(patient.name);
	{//��������������
		JSON_Document document;
		document.Parse(fileJSON.c_str());
		if (document.IsObject())
		{
			DBPatient p;
			p.Deserialize(document[L"patient"]);
			patient = std::move(p);
			administrations.Deserialize(document[L"blocks"]);
		}
		else
			MessageBox(0, L"�������� ������ �����", L"������", MB_OK | MB_ICONERROR);
	}
	

	{// ������������
		using namespace rapidjson;
		using jvalue = JSON_Value;
		JSON_Document document;
		
		auto& allocator = document.GetAllocator();
		document.SetObject();

		jvalue blocks(kArrayType);
		administrations.Serialize(blocks, allocator);

		document.AddMember(L"blocks", blocks, allocator);
		
		// ����������
		JSON_StringBuffer buffer;
		JSON_Writer writer(buffer);
		document.Accept(writer);
		// ��������� ������
		wstring json = buffer.GetString(); 
		
		wstring temp = json;
	}
	
}
//--------------------------------------------------------------------------------------------------------
int DatabaseLoader::countPatients() const
{
	return static_cast<int>(patientList.size()); // ����� �� ���� ������ ���������
}
//--------------------------------------------------------------------------------------------------------
DBPatient DatabaseLoader::getPatient(int index) const
{	
	// ����� �������� �� ���� ������
	return patient;//{ { L"������ ��������� ��������" },{ DBPatient::BloodType(1,1) },{ 40 },{ 90 },{ L"1223" },{ L"100628" } };
}
//--------------------------------------------------------------------------------------------------------
const ChartData& DatabaseLoader::getAdministrations() const
{
	return administrations;
}
//--------------------------------------------------------------------------------------------------------
void DatabaseLoader::saveAdministrations(int index)
{
	//	����������� ���������� ChartData ����� ������������
}
//--------------------------------------------------------------------------------------------------------
const vector<const DrugInfo*>* DatabaseLoader::getDrugsPtr()
{
	return &selectedDrugs;
}
//--------------------------------------------------------------------------------------------------------
void DatabaseLoader::resetBufferedDrugs()
{
	selectedDrugs.clear();
	bufferedDrugs.clear();
}
//--------------------------------------------------------------------------------------------------------
void DatabaseLoader::getDrugNames(const wstring& str, const function<void(bool)>& callBack, bool OnlyIV)
{
	selectedDrugs.clear();
	if (str.size() < 1)
	{
		drugFinder.find_str.clear();
		bufferedDrugs.clear();
		if (callBack)
			callBack(false);
		return;
	}

	if (str == drugFinder.find_str)
		return;
	drugFinder.find_str = str;

	// ������� ��� �������
	auto fiterBuffered = [this, callBack, OnlyIV]()
	{
		wstring str2(this->drugFinder.find_str);
		str2[str2.size() - 1]++;
		
		std:mutex mute;
		mute.lock();
		auto startIt = bufferedDrugs.lower_bound(this->drugFinder.find_str);
		auto endIt = bufferedDrugs.lower_bound(str2);
		mute.unlock();

		selectedDrugs.clear();
		for (startIt; startIt != endIt; ++startIt)
		{
			//if (!OnlyIV || (*startIt).second.isIVallowed())
			//������ ���� �������� �� ����������� �������� ��������� �����������!!!!!
				selectedDrugs.push_back(&(*startIt).second);
		}
		
	};

	
	// ��������� ���������� �������� � ��
	if (bufferedDrugs.empty() && !drugFinder.working)// ���� ����������� �����, ����� ��������� ������ �� ��
	{
		
		// �������� ���� ���������� � ���������� �� ������ ������
		thread t(
			[this, str, callBack, OnlyIV, fiterBuffered]()
		{
			if (!db_connector) return;

			if (callBack)
				callBack(true);
			this->drugFinder.working = true;
			selectedDrugs.clear();
			bufferedDrugs.clear();


			db_connector->getDrugList(str, 
				[this](const DrugInfo& newDrugInfo)
				{
					std::mutex mute;
					auto& drug_name = newDrugInfo.name;
					mute.lock();
					bufferedDrugs[drug_name] = newDrugInfo;
					selectedDrugs.push_back(&bufferedDrugs[drug_name]);
					mute.unlock();
				}
			);
			
			if (callBack)
				callBack(false);
			this->drugFinder.working = false;

		}
		);
		t.detach();
	}
	else if(!drugFinder.working)//���� �����������
	{
		fiterBuffered();

		if (callBack)
			callBack(false);
	}


}
//--------------------------------------------------------------------------------------------------------
bool DatabaseLoader::getDrugInfo(const wstring& name, DrugInfo& drugInfo)
{
	if (bufferedDrugs.count(name) == 0)
		return false;

	drugInfo = bufferedDrugs.at(name);

	return true;
}
//--------------------------------------------------------------------------------------------------------
vector<wstring> DatabaseLoader::getAllowedAdminWays(const wstring& name) const
{
	
	vector<wstring> result;
	result.reserve(allowedAdminWays.size());
	for (const auto& way : allowedAdminWays)
	{
		result.push_back(way.first);
	}
	
	return result;

}
//--------------------------------------------------------------------------------------------------------

/*bool DatabaseLoader::getExistsDrugInfo(SQL& sql, const wstring& name, DrugInfo& drugInfo) const
{
	if (!sql.SendRequest(L"SELECT * FROM drugname_linker,druginfo WHERE drugname_linker.name = '" + name + L"' AND drugname_linker.id=druginfo.id;"))
		return false;

	auto count = sql.CountStrings();
	if (count == 0)
		return false;
	auto result = sql.RecieveNextData();
	result.erase(result.begin(), result.begin() + 3);
	drugInfo = DrugInfo(name, result);
	return true;
}
//--------------------------------------------------------------------------------------------------------
bool DatabaseLoader::getExistsDrugInfo(const wstring& name, DrugInfo& drugInfo) const
{
	if (bufferedDrugs.count(name)>0)
	{
		if (!bufferedDrugs.at(name).isExistsInDB())
			return false;
		drugInfo = bufferedDrugs.at(name);
		return true;
	}
	SQL sql;
	sql.Connect();
	return getExistsDrugInfo(sql, name, drugInfo);
}
//--------------------------------------------------------------------------------------------------------*/



int DatabaseLoader::getAdminWayType(const wstring& adminway)
{
	
	return (allowedAdminWays.count(adminway)>0) ? allowedAdminWays.at(adminway) : -1;
}
//--------------------------------------------------------------------------------------------------------
void DatabaseLoader::loadAllowedAdminWays()
{
	/* ��������� � ���� ������ */
	/*allowedAdminWays = {
		{ L"� / � ��������", 1},
		{L"� / � ������������",2},
		{L"� / � �������",3},
		{L"� / �",4},
		{L"� / �",5},
		{L"����������",6},
		{L"���������",7},
		{L"���������� ������������",8},
		{L"������������ ������������",9 },
		{L"����������� ������������", 10},
		{L"�������� ����������",11},
		{L"���������",12},
		{L"��������",13},
		{L"����� �����",14},
		{L"������� �����",15}
	};*/
	db_connector->getAdminWays([this](const std::pair<std::wstring, int>& result) { allowedAdminWays.insert(result);} );
}