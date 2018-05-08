#pragma once

namespace Callback
{
	template <typename T>
	class CallBackFunction
	{
	protected:
		T* obj;
	public:
		CallBackFunction(T* object) : obj(object) {}
		virtual void execute()=0;
		void operator()() { execute(); };
	};
}