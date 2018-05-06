#pragma once

#include <Windows.h>

namespace ChartDLL
{

#if __cplusplus > 199711L || (defined(_MSC_VER) && _MSC_VER >= 1800) // __cplusplus начиная с VS 2013 определено как 199711L
#include <cstddef>
#include <type_traits>

	template <typename T, typename Enable = void>
	class function;

	template <typename T>
	class function<T, typename std::enable_if<std::is_function<T>::value>::type>
#else
	template <typename T>
	class function
#endif
	{
	private:
		HINSTANCE dllHandle;
		T* FunctionPtr;
	public:

		function(const char* func_name)
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
		~function()
		{
			if (dllHandle)
				FreeLibrary(dllHandle);
		}
	};

};