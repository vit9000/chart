#pragma once
#include "type_defines.h"
#include "DrugInfo.h"


template<typename T>
class DLLCopier
{
public:
	virtual void push_back_data(const T&) const  =0;
};

typedef DLLCopier<pair<int, wstring>> PairCopier;
typedef DLLCopier<wstring> StringCopier;
typedef DLLCopier<BOOL> BoolCopier;
typedef DLLCopier<double> DoubleCopier;
typedef DLLCopier<DrugInfoEx> DrugInfoExCopier;
typedef DLLCopier<PatientInfo> PatientInfoCopier;



class IDBConnector
{

public:
	IDBConnector() {}
	virtual void getPatientList(const PatientInfoCopier&) const = 0;
	virtual void getDrugList(const wstring&, const DrugInfoExCopier&) const = 0;
	virtual void getChartJSON(const PatientInfo&, const StringCopier&) const = 0;
	virtual void getAdminWays(const PairCopier&) const = 0;
	virtual void setAppMenu(CMenu * menu) = 0;
	virtual void executeApp(UINT nID) = 0;
	virtual void showAboutDlg() = 0;

	virtual void GetParamBool(int Code, const BoolCopier&) const = 0;
	virtual void GetParamNumber(int Code, const DoubleCopier&) const = 0;
	virtual void GetParamText(int Code, const StringCopier&) const = 0;;

};