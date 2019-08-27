#ifndef __Camel_Base_System_Threading_AutoResetEvent__
#define __Camel_Base_System_Threading_AutoResetEvent__

#include "SystemBase.h"
#include <Windows.h>

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace Threading
{
	class CAMEL_BASE_SYSTEM_API AutoResetEvent
	{
	private:
		struct Impl;
		Impl *impl_;

	public:
		AutoResetEvent(bool initialState = false);
		~AutoResetEvent(void);

		bool Set(void);
		bool Reset(void);
		bool Pulse(void);

		bool Wait(unsigned int milliseconds = INFINITE);
	#if defined(_WIN32) || defined(_WIN64)
		bool WaitEx(bool alertable, unsigned int milliseconds = INFINITE);
	#endif

	private:
		AutoResetEvent(const AutoResetEvent &);
		const AutoResetEvent &operator=(const AutoResetEvent &);
	};
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif // __Camel_Base_System_Threading_AutoResetEvent__