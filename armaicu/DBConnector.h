#pragma once
#include "IDBConnector.h"
#include "CommonMenu.h"
#include "AboutDlg.h"
#include "Query.h"
#include <atlstr.h> 
//#include "Global.h"

class DBConnector : public IDBConnector
{
private:
	CVString deptID;
	CCommonMenu m_CommonMenu;
public:
	DBConnector(const CVString dept_id) : deptID(dept_id) {}

	void getPatientList(double DATETIME, IDBResultCopier& copier) override;
	void setAppMenu(CMenu * menu) override;
	void executeApp(UINT nID) override;
	void showAboutDlg() override;
	void showLogDialog() override;

	void sendQuery(const CVString& query, const CQueryParameters& params, IDBResultCopier& result) override;
	void sendQuery(const CVString& query, IDBResultCopier& result) override;
	
protected:
	void GetParamBool(int Code, const BoolCopier&) const override;
	void GetParamNumber(int Code, const DoubleCopier&) const override;
	void GetParamText(int Code, const StringCopier&) const override;
private:
	
};


