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

	void getPatientList() const override;
	void getDrugList(const std::wstring& drug) const override;
	void getChartJSON(const PatientInfo& patient) const override;
	//void getAdminWays(const Push_Back_AdminWay& push_back) const override;
	void getAdminWays(PairCopier * data_copier) const override;
	void setAppMenu(CMenu * menu) override;
	void executeApp(UINT nID) override;
	void showAboutDlg() override;
private:
	
};


