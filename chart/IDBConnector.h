#pragma once
#include "type_defines.h"
#include "DrugInfo.h"


template<typename T>
class DLLCopier
{
public:
	virtual void push_back_data(const T&) const  =0;
};






typedef DLLCopier<wstring> StringCopier;
typedef DLLCopier<BOOL> BoolCopier;
typedef DLLCopier<double> DoubleCopier;
//typedef DLLCopier<DrugInfoEx> DrugInfoExCopier;
//typedef DLLCopier<PatientInfo> PatientInfoCopier;

//++++++++++++
template<typename T>
class VCopier
{
	T obj;
public:
	virtual void push_back(const T& res) { obj = res; }
	operator T() { return obj; }
	bool operator==(const T& rhs) const { return obj == rhs; }
	friend bool operator==(const T& lhs, const VCopier& rhs) { return lhs == rhs.obj; }
};

class IDBResult
{
public:
	virtual void GetStrValue(const std::wstring& param, VCopier<wstring>& copier) = 0;
	virtual int GetIntValue(const std::wstring& param) = 0;
	virtual double GetDateValue(const std::wstring& param) = 0;
	virtual double GetFloatValue(const std::wstring& param) = 0;

	virtual void Next() = 0;;
	virtual bool Eof() = 0;
};

class IDBResultCopier
{
public:
	virtual void push_back(IDBResult&) {};
};

#include <cstdint>
class QueryParameter
{
	enum TYPE {STRING, DOUBLE, INT};
	int type;
	wstring name;
	wstring variable;
public:
	/*QueryParameter(const wstring& Name, int value) : type(INT), name(Name), variable((long) value) {}
	QueryParameter(const wstring& Name, double value) : type(DOUBLE), name(Name), variable(value) {}*/
	QueryParameter(const wstring& Name, const wstring& value) : type(DOUBLE), name(Name), variable(value.c_str()) {}

	const wstring& getName() const { return name; }
	const wstring& get() const { return variable; }
};

//++++++++++++++



class IDBConnector
{

public:
	IDBConnector() {}
	virtual void getPatientList(double DATETIME, IDBResultCopier&) = 0;
	virtual void setAppMenu(CMenu * menu) = 0;
	virtual void executeApp(UINT nID) = 0;
	virtual void showAboutDlg() = 0;
	virtual void showLogDialog() = 0;
	virtual void sendQuery(const wstring& query, const vector<QueryParameter>& params, IDBResultCopier& result) =0;
	virtual void sendQuery(const wstring& query, IDBResultCopier& result) = 0;

	template<typename T> void GetParam(int Code, const DLLCopier<T>&) const {};
	template<> void GetParam<BOOL>(int Code, const DLLCopier<BOOL>& copier) const { GetParamBool(Code, copier); };
	template<> void GetParam<double>(int Code, const DLLCopier<double>& copier) const { GetParamNumber(Code, copier); };
	template<> void GetParam<wstring>(int Code, const DLLCopier<wstring>& copier) const { GetParamText(Code, copier); };

protected: // классические функции скрыты, нужно использовать только специализации шаблоной функции GetParam<>
	virtual void GetParamBool(int Code, const BoolCopier&) const = 0;
	virtual void GetParamNumber(int Code, const DoubleCopier&) const = 0;
	virtual void GetParamText(int Code, const StringCopier&) const = 0;;

};