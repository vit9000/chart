#pragma once
#include <string>
#include <vector>
#include "PatientInfo.h"
#include <functional>
#include <memory>

typedef std::vector<std::wstring> DrugList;

typedef std::function<void(const std::wstring&)> Push_Back_String;
#define GET_STRING_LAMBDA(str) [&str](const wstring& result) { str = result; } 
#define PUSH_BACK_LAMBDA(vec) [&vec](const wstring& result) { vec.push_back(result); } 
