#pragma once
#include <string>
#include <vector>

typedef std::vector<std::wstring> DrugList;
typedef const DrugList& (*GetDrugFunction)(const std::wstring& drug);