#pragma once

#include <Windows.h>



#if __cplusplus > 199711L || (defined(_MSC_VER) && _MSC_VER >= 1800) // __cplusplus ������� � VS 2013 ���������� ��� 199711L
#include <cstddef>
#include <type_traits>

template <typename T, typename Enable = void>
class ChartDLLFunction;

template <typename T>
class ChartDLLFunction<T, typename std::enable_if<std::is_function<T>::value>::type>
#else
template <typename T>
class ChartDLLFunction
#endif
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
	operator T*()
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

