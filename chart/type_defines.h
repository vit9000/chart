#pragma once
#include <string>
#include <vector>
#include "PatientInfo.h"
#include <functional>
#include <memory>

typedef std::vector<std::wstring> DrugList;

typedef std::function<void(const std::wstring&)> Push_Back_String;
