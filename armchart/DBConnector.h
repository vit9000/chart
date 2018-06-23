#pragma once
#include "IDBConnector.h"
#include "CommonMenu.h"
#include "AboutDlg.h"
#include <atlstr.h> 
//#include "Global.h"

class DBConnector : public IDBConnector
{
private:
	std::wstring deptID;
	CCommonMenu m_CommonMenu;
public:
	DBConnector(const std::wstring dept_id) : deptID(dept_id) {}

	void getPatientList(const PatientInfoCopier&) const override;
	void getDrugList(const wstring&, const DrugInfoExCopier&) const override;
	void getChartJSON(const PatientInfo& patient, const StringCopier& data_copier) const override;
	//void getAdminWays(const Push_Back_AdminWay& push_back) const override;
	void getAdminWays(const PairCopier& data_copier) const override;
	void setAppMenu(CMenu * menu) override;
	void executeApp(UINT nID) override;
	void showAboutDlg() override;

	void GetParamBool(int Code, const BoolCopier&) const;
	void GetParamNumber(int Code, const DoubleCopier&) const;
	void GetParamText(int Code, const StringCopier&) const;
private:
	
};


