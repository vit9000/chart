#pragma once
#include "type_defines.h"
#include "DrugInfo.h"


class VString
{
	wchar_t* _str;
	size_t _size;
public:
	VString() : _str(NULL), _size(0) {}
	VString(const VString& new_str)
	{
		_size = wcslen(new_str._str);
		_str = new wchar_t[_size + 1];
		memcpy(_str, new_str._str, _size * sizeof(wchar_t));
		_str[_size] = L'\0';
	}
	VString(const wchar_t* new_str)
	{
		_size = wcslen(new_str);
		_str = new wchar_t[_size + 1];
		memcpy(_str, new_str, _size * sizeof(wchar_t));
		_str[_size] = L'\0';
	}

	~VString()
	{
		delete[] _str;
		_str = NULL;
		_size = 0;
	}

	VString& operator=(const wchar_t* new_str)
	{
		if (_str == new_str)
			return *this;
		delete[] _str;
		_size = wcslen(new_str);
		_str = new wchar_t[_size + 1];
		memcpy(_str, new_str, _size * sizeof(wchar_t));
		_str[_size] = L'\0';
		return *this;
	}

	VString& operator=(const VString& new_str)
	{
		return operator=(new_str._str);
	}

	wchar_t* c_str() { return _str; }
	const wchar_t* c_str() const { return _str; }
	size_t size() const { return _size; }
};


template<typename T>
class DLLCopier
{
public:
	virtual void push_back_data(const T&) const = 0;
};






typedef DLLCopier<VString> StringCopier;
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
	operator const T&() const { return obj; }
	bool operator==(const T& rhs) const { return obj == rhs; }
	friend bool operator==(const T& lhs, const VCopier& rhs) { return lhs == rhs.obj; }
};

class IDBResult
{
public:
	virtual void GetStrValue(const VString& param, VCopier<VString>& copier) = 0;
	virtual int GetIntValue(const VString& param) = 0;
	virtual double GetDateValue(const VString& param) = 0;
	virtual double GetFloatValue(const VString& param) = 0;

	virtual void Next() = 0;;
	virtual bool Eof() = 0;
};

class IDBResultCopier
{
public:
	virtual void push_back(IDBResult&) {};
};

//#include <cstdint>
class QueryParameter
{
	enum TYPE { STRING, DOUBLE, INT };
	int type;
	VString name;
	VString variable;
public:
	/*QueryParameter(const VString& Name, int value) : type(INT), name(Name), variable((long) value) {}
	QueryParameter(const VString& Name, double value) : type(DOUBLE), name(Name), variable(value) {}*/
	QueryParameter() {};
	QueryParameter(const VString& Name, const VString& value) : type(DOUBLE), name(Name), variable(value.c_str()) {}
	QueryParameter(const VString& Name, int value) : type(DOUBLE), name(Name)
	{
		wstringstream ss;
		ss << value;
		variable = ss.str().c_str();
	}
	QueryParameter(const VString& Name, double value) : type(DOUBLE), name(Name)
	{
		wstringstream ss;
		ss << value;
		variable = ss.str().c_str();
	}

	const VString& getName() const { return name; }
	const VString& get() const { return variable; }
};

class QueryParameters
{
private:
	QueryParameter* data;
	int reserved;
	int count;
public:
	QueryParameters() : reserved(0), count(0), data(NULL) {}
	QueryParameters(int Reserve) : reserved(0), count(0), data(NULL) 
	{
		reserve(Reserve);
	}

	QueryParameters(const QueryParameters& queryParameters)
	{
		if (queryParameters.reserved <= 0) return;

		reserved = queryParameters.reserved;
		QueryParameter* newdata = new QueryParameter[reserved];
		if (queryParameters.count > 0)
		{
			count = queryParameters.count;
			for (int i = 0; i < count; i++)
			{
				newdata[i] = data[i];
			}
		}
		data = newdata;
		newdata = NULL;
	}

	QueryParameters& operator=(const QueryParameters& queryParameters)
	{
		if (queryParameters.data == data) return *this;
		clear();

		if (queryParameters.reserved <= 0) return *this;

		reserved = queryParameters.reserved;
		QueryParameter* newdata = new QueryParameter[reserved];
		if (queryParameters.count > 0)
		{
			count = queryParameters.count;
			for (int i = 0; i < count; i++)
			{
				newdata[i] = data[i];
			}
		}
		data = newdata;
		newdata = NULL;
		return *this;
	}

	~QueryParameters()
	{
		clear();
	}

	void reserve(int Reserve)
	{
		if (Reserve <= count) return;

		QueryParameter* newdata = new QueryParameter[Reserve];
		reserved = Reserve;
		if (newdata)
		{
			for (int i = 0; i < count; i++)
				newdata[i] = data[i];
			delete[] data;
			data = newdata;
			newdata = NULL;
		}
	}

	void clear()
	{
		if (data)
			delete[] data;
		count = 0;
		reserved = 0;
	}

	void push_back(const QueryParameter& parameter)
	{
		if (reserved <= 0) reserve(1);
		else if (count >= reserved)
			reserve(reserved * 2);
		data[count] = parameter;
		count++;
	}

	QueryParameter& operator[](int index)
	{
		return data[index];
	}

	const QueryParameter& operator[](int index) const
	{
		return data[index];
	}

	int size() const
	{
		return count;
	}

};

//++++++++++++++

enum TIME_TYPE { ANESTH_CHART = 0, ICU_CHART = 1 };

class IDBConnector
{

public:
	IDBConnector() { }
	virtual void getPatientList(double DATETIME, IDBResultCopier&) = 0;
	virtual void setAppMenu(CMenu * menu) = 0;
	virtual void executeApp(UINT nID) = 0;
	virtual void showAboutDlg() = 0;
	virtual void showLogDialog() = 0;
	virtual void sendQuery(const VString& query, const QueryParameters& params, IDBResultCopier& result) = 0;
	virtual void sendQuery(const VString& query, IDBResultCopier& result) = 0;

	template<typename T> void GetParam(int Code, const DLLCopier<T>&) const {};
	template<> void GetParam<BOOL>(int Code, const DLLCopier<BOOL>& copier) const { GetParamBool(Code, copier); };
	template<> void GetParam<double>(int Code, const DLLCopier<double>& copier) const { GetParamNumber(Code, copier); };
	template<> void GetParam<VString>(int Code, const DLLCopier<VString>& copier) const { GetParamText(Code, copier); };

protected: // классические функции скрыты, нужно использовать только специализации шаблоной функции GetParam<>
	virtual void GetParamBool(int Code, const BoolCopier&) const = 0;
	virtual void GetParamNumber(int Code, const DoubleCopier&) const = 0;
	virtual void GetParamText(int Code, const StringCopier&) const = 0;



};


/*#pragma once
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
	QueryParameter(const wstring& Name, const wstring& value) : type(DOUBLE), name(Name), variable(value.c_str()) {}
	QueryParameter(const wstring& Name, int value) : type(DOUBLE), name(Name) 
	{
		std::wstringstream ss;
		ss << value;
		variable = ss.str();
	}
	QueryParameter(const wstring& Name, double value) : type(DOUBLE), name(Name)
	{
		std::wstringstream ss;
		ss << value;
		variable = ss.str();
	}

	const wstring& getName() const { return name; }
	const wstring& get() const { return variable; }
};

//++++++++++++++

enum TIME_TYPE { ANESTH_CHART=0, ICU_CHART=1 };

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
	virtual void GetParamText(int Code, const StringCopier&) const = 0;

	

};
*/