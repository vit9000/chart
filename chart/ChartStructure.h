#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <locale>
#include <codecvt>
#include "Ini.h"


#include "rapidjson/document.h"     // rapidjson's DOM-style API
typedef rapidjson::GenericDocument<rapidjson::UTF16<> > WDocument;
typedef rapidjson::GenericValue<rapidjson::UTF16<> > WValue;

using namespace std;
class ChartStructure
{
private:
	class Destroyer
	{
	private:
		ChartStructure* ptr;
	public:
		void init(ChartStructure* instance)
		{
			ptr = instance;
		}
		~Destroyer()
		{
			delete ptr;
		}
	};

	static ChartStructure* instance;
	static Destroyer destroyer;

	map<wstring, vector<pair<wstring, int>>> data;
	vector<wstring> blocks;
	map<wstring,int> block_types;


protected:
	ChartStructure();
	~ChartStructure() {}
public:
	enum { NUMERIC = 1, TEXT = 2 };
	enum { STANDART, ADMINISTRATIONS, PLOT, PLOT_PA };

	static ChartStructure * getInstance();
	vector<pair<wstring, int>> getBlockParameters(const wstring& BlockName) const;
	const vector<wstring>& getBlocks() const;
	int getBlockType(const wstring& BlockName) const;
	wstring getAdministrationsBlockName();

};