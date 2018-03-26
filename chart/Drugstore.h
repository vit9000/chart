#pragma once
#include <map>
#include <fstream>
#include <sstream>
#include <future>
#include "utils.h" 

using namespace std;
class Drugstore;

struct DrugInfo
{
	wstring dbname;
	wstring name;
	wstring dose;
	wstring ED;
	wstring type;

	wstring getFullName()
	{
		if (type.empty())
		{
			auto pos = dbname.find(L" №");
			if (pos > 0 && pos < dbname.size())
				return dbname.substr(0, pos);
			return dbname;// +L" " + type;
		}
		wstring full = name + wstring(L" ") + type;
		if(!dose.empty())
			full += wstring(L" (") + dose + wstring(L" ") + ED + wstring(L")");
		return full;
	}

};


class Drugstore
{
private:
	class DrugstoreDestroyer
	{
	private:
		Drugstore* ptr;
	public:
		~DrugstoreDestroyer();
		void init(Drugstore *instance);

	};


	static Drugstore* instance;
	static DrugstoreDestroyer destroyer;

	typedef map<wstring, DrugInfo> DataType;
	DataType data;
	map<wstring, wstring> dict;
	vector<wstring> EDs;
protected:
	Drugstore();
	~Drugstore() {}
	
	
public:
	static Drugstore& getInstance();
	
	
	void find(const wstring& str, vector<wstring>& result);
	inline size_t getSize() const { return data.size(); }
	inline const DataType& getData() { return data; }
	
	wstring ParseName(const wstring& str);
	pair<wstring, wstring> ParseED(const wstring& str);
	wstring ParseType(const wstring& str);
	bool parse(const wstring& input_string,  DrugInfo& drug);
	bool isDose(int letter)
	{
		return (letter >= 48 && letter <= 57) || letter==46 || letter==44;
	}
	bool isDigit(int letter)
	{
		return (letter >= 48 && letter <= 57);
	}
	const DrugInfo& getDrugInfo(const wstring& name) const;
};