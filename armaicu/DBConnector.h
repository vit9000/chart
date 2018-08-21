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
	CVString docName;
	CCommonMenu m_CommonMenu;
public:
	DBConnector(const CVString& dept_id, const CVString& doc_name) : deptID(dept_id), docName(doc_name)  {}

	void getPatientList(double DATETIME, IDBResultCopier& copier) override;
	void setAppMenu(CMenu * menu) override;
	void executeApp(UINT nID) override;
	void showAboutDlg() override;
	void showLogDialog() override;

	void sendQuery(const CVString& query, const CQueryParameters& params, IDBResultCopier& result) override;
	void sendQuery(const CVString& query, IDBResultCopier& result) override;
	void getDoctor(const DLLCopier<CVString>& copier) const override;
protected:
	void GetParamBool(int Code, const BoolCopier&) const override;
	void GetParamNumber(int Code, const DoubleCopier&) const override;
	void GetParamText(int Code, const StringCopier&) const override;
private:
	
};


