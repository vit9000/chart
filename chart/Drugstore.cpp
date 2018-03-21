#include "stdafx.h"
#include "Drugstore.h"


/*Singleton * Singleton::p_instance = 0;
SingletonDestroyer Singleton::destroyer;

SingletonDestroyer::~SingletonDestroyer() {
	delete p_instance;
}
void SingletonDestroyer::initialize(Singleton* p) {
	p_instance = p;
}
Singleton& Singleton::getInstance() {
	if (!p_instance) {
		p_instance = new Singleton();
		destroyer.initialize(p_instance);
	}
	return *p_instance;
}*/

Drugstore* Drugstore::instance = nullptr;
Drugstore::DrugstoreDestroyer Drugstore::destroyer;


Drugstore::DrugstoreDestroyer::~DrugstoreDestroyer()
{
	delete ptr;
}
void Drugstore::DrugstoreDestroyer::init(Drugstore *instance)
{
	ptr = instance;
}

Drugstore& Drugstore::getInstance()
{
	if (!instance)
	{
		instance = new Drugstore();
		destroyer.init(instance);
	}
	return *instance;
}

Drugstore::Drugstore()
{
	thread t([this]()
	{
		setlocale(LC_ALL, "UTF-8");
		ifstream in;

		in.open("drugs_122.txt");
		while (in)
		{
			char c_str[256];
			//char c_str[256];
			in.getline(c_str, 256);
			//in >> c_str;

			wstring str (std::move(StringConverter(c_str)));
			if (str[0] == L'\"' || str[0] == L' ' || isLower(str[0]))
				continue;
			wstring name;
			getName(str, name);
			if (name.size() <= 1)
				continue;
			data[name] = str;
		}
	});
	t.detach();


}
bool Drugstore::isDigit(int letter)
{
	return (letter >= 48 && letter <= 57);
}

void Drugstore::getName(const wstring& source, wstring& name)
{


	/*auto it = find_if(source.begin(), source.end(),
	[this](const wchar_t letter)
	{
	return !isUpper(static_cast<int>(letter));
	});
	name = source.substr(0, it - source.begin());*/
	wstringstream ss(source);

	int i = 0;
	while (ss)
	{
		wstring temp;
		ss >> temp;;
		if (temp.size() == 0)
			break;
		if (isUpper(temp[0]))// || isDigit(temp[0]))
		{
			name += wstring((i == 0) ? L"" : L" ") + temp;
			++i;
		}
		else return;
	}

}

void Drugstore::find(const wstring& str, vector<wstring>& result)
{
	if (str.size() == 0)
		return;


	result.clear();
	auto itlow = data.lower_bound(str);  // itlow points to b
	wstring s(str);
	s.at(s.size() - 1)++;
	auto itup = data.upper_bound(s);   // itup points to e (not d!)

	for (auto& it = itlow; it != itup; it++)
	{
		result.push_back(it->first);
	}


}


bool Drugstore::isUpper(int letter)
{
	return ((letter >= 1040 && letter <= 1071) || letter == 1025);
}

bool Drugstore::isLower(int letter)
{
	return !isUpper(letter);
}

