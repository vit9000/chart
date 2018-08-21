#pragma once
//#include "DrugInfo.h"
#include <sstream>

using namespace std;
class CVString
{
	wchar_t* _str;
	size_t _size;
public:
	CVString() : _str(NULL), _size(0) {}
	CVString(const CVString& new_str)
	{
		_size = wcslen(new_str._str);
		_str = new wchar_t[_size + 1];
		memcpy(_str, new_str._str, _size * sizeof(wchar_t));
		_str[_size] = L'\0';
	}
	CVString(const wchar_t* new_str)
	{
		_size = wcslen(new_str);
		_str = new wchar_t[_size + 1];
		memcpy(_str, new_str, _size * sizeof(wchar_t));
		_str[_size] = L'\0';
	}

	~CVString()
	{
		delete[] _str;
		_str = NULL;
		_size = 0;
	}

	CVString& operator=(const wchar_t* new_str)
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

	CVString& operator=(const CVString& new_str)
	{
		return operator=(new_str._str);
	}

	wchar_t* c_str() { return _str; }
	const wchar_t* c_str() const { return _str; }
	size_t size() const { return _size; }
};
//----------------------------------------------
template<typename T>
class DLLCopier
{
public:
	virtual void push_back_data(const T&) const = 0;
};
//----------------------------------------------
typedef DLLCopier<CVString> StringCopier;
typedef DLLCopier<BOOL> BoolCopier;
typedef DLLCopier<double> DoubleCopier;
//++++++++++++
template<typename T>
class CVCopier
{
	T obj;
public:
	virtual void push_back(const T& res) { obj = res; }
	operator T() { return obj; }
	operator const T&() const { return obj; }
	T& get() { return obj; }
	const T& get() const { return obj; }
	bool operator==(const T& rhs) const { return obj == rhs; }
	friend bool operator==(const T& lhs, const CVCopier& rhs) { return lhs == rhs.obj; }
};

class IDBResult
{
public:
	virtual void GetStrValue(const CVString& param, CVCopier<CVString>& copier) = 0;
	virtual int GetIntValue(const CVString& param) = 0;
	virtual double GetDateValue(const CVString& param) = 0;
	virtual double GetFloatValue(const CVString& param) = 0;

	virtual void Next() = 0;;
	virtual bool Eof() = 0;
};
//----------------------------
class IDBResultCopier
{
public:
	virtual void push_back(IDBResult&) {};
};
//----------------------------
class CQueryParameter
{
	enum TYPE { STRING, DOUBLE, INT };
	int type;
	CVString name;
	CVString variable;
public:
	CQueryParameter() {};
	CQueryParameter(const CVString& Name, const CVString& value) : type(DOUBLE), name(Name), variable(value.c_str()) {}
	CQueryParameter(const CVString& Name, int value) : type(DOUBLE), name(Name)
	{
		wstringstream ss;
		ss << value;
		variable = ss.str().c_str();
	}
	CQueryParameter(const CVString& Name, double value) : type(DOUBLE), name(Name)
	{
		wstringstream ss;
		ss << value;
		variable = ss.str().c_str();
	}

	const CVString& getName() const { return name; }
	const CVString& get() const { return variable; }
};
//--------------------------
class CQueryParameters
{
private:
	CQueryParameter* data;
	int reserved;
	int count;
public:
	CQueryParameters() : reserved(0), count(0), data(NULL) {}
	CQueryParameters(int Reserve) : reserved(0), count(0), data(NULL) 
	{
		reserve(Reserve);
	}

	CQueryParameters(const CQueryParameters& CQueryParameters)
	{
		if (CQueryParameters.reserved <= 0) return;

		reserved = CQueryParameters.reserved;
		CQueryParameter* newdata = new CQueryParameter[reserved];
		if (CQueryParameters.count > 0)
		{
			count = CQueryParameters.count;
			for (int i = 0; i < count; i++)
			{
				newdata[i] = data[i];
			}
		}
		data = newdata;
		newdata = NULL;
	}

	CQueryParameters& operator=(const CQueryParameters& CQueryParameters)
	{
		if (CQueryParameters.data == data) return *this;
		clear();

		if (CQueryParameters.reserved <= 0) return *this;

		reserved = CQueryParameters.reserved;
		CQueryParameter* newdata = new CQueryParameter[reserved];
		if (CQueryParameters.count > 0)
		{
			count = CQueryParameters.count;
			for (int i = 0; i < count; i++)
			{
				newdata[i] = data[i];
			}
		}
		data = newdata;
		newdata = NULL;
		return *this;
	}

	~CQueryParameters()
	{
		clear();
	}

	void reserve(int Reserve)
	{
		if (Reserve <= count) return;

		CQueryParameter* newdata = new CQueryParameter[Reserve];
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

	void push_back(const CQueryParameter& parameter)
	{
		if (reserved <= 0) reserve(1);
		else if (count >= reserved)
			reserve(reserved * 2);
		data[count] = parameter;
		count++;
	}

	CQueryParameter& operator[](int index)
	{
		return data[index];
	}

	const CQueryParameter& operator[](int index) const
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
	virtual void sendQuery(const CVString& query, const CQueryParameters& params, IDBResultCopier& result) = 0;
	virtual void sendQuery(const CVString& query, IDBResultCopier& result) = 0;
	virtual void getDoctor(const DLLCopier<CVString>& copier) const = 0;

	template<typename T> void GetParam(int Code, const DLLCopier<T>&) const {};
	template<> void GetParam<BOOL>(int Code, const DLLCopier<BOOL>& copier) const { GetParamBool(Code, copier); };
	template<> void GetParam<double>(int Code, const DLLCopier<double>& copier) const { GetParamNumber(Code, copier); };
	template<> void GetParam<CVString>(int Code, const DLLCopier<CVString>& copier) const { GetParamText(Code, copier); };

protected: // классические функции скрыты, нужно использовать только специализации шаблоной функции GetParam<>
	virtual void GetParamBool(int Code, const BoolCopier&) const = 0;
	virtual void GetParamNumber(int Code, const DoubleCopier&) const = 0;
	virtual void GetParamText(int Code, const StringCopier&) const = 0;
};

