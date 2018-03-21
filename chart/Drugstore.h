#pragma once
#include <map>
#include <fstream>
#include <sstream>
#include <future>
#include "utils.h" 

using namespace std;
class Drugstore;




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

	typedef map<wstring, wstring> DataType;
	DataType data;
protected:
	Drugstore();
	~Drugstore() {}
	
	
public:
	static Drugstore& getInstance();
	
	
	void find(const wstring& str, vector<wstring>& result);
	bool isUpper(int letter);
	bool isLower(int letter);
	inline size_t getSize() const { return data.size(); }
	inline const DataType& getData() { return data; }
	bool isDigit(int letter);
	void getName(const wstring& source, wstring& name);

};