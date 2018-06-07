#pragma once
#include <string>
#include <vector>
#include "PatientInfo.h"
#include "DrugInfo.h"
#include <functional>
#include <memory>

typedef std::vector<std::wstring> DrugList;

typedef std::function<void(const std::wstring&)> Push_Back_String;
typedef std::function<void(const std::pair<std::wstring, int>&)> Push_Back_AdminWay;
typedef std::function<void(const DrugInfo&)> Push_Back_DrugInfo;
typedef std::function<void(const PatientInfo&)> Push_Back_PatientInfo;
#define GET_STRING_LAMBDA(str) [&str](const wstring& result) { str = result; } 
#define PUSH_BACK_LAMBDA(vec) [&vec](const wstring& result) { vec.push_back(result); } 
