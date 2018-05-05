#pragma once

#include <Windows.h>

template <class T> 
class ChartDLLFunction
{
private:
	HINSTANCE dllHandle;
	T* FunctionPtr;
public:

	ChartDLLFunction(const char* func_name)
		:dllHandle(NULL),
		FunctionPtr(NULL)
	{
		dllHandle = LoadLibrary(L"chart.dll");
		if (dllHandle)
			FunctionPtr = reinterpret_cast<T*>(GetProcAddress(dllHandle, func_name));
	}
	//-------------------------------------------------------------------
	T* operator()()
	{
		return FunctionPtr;
	}
	//-------------------------------------------------------------------
	~ChartDLLFunction()
	{
		if (dllHandle)
			FreeLibrary(dllHandle);
	}
};