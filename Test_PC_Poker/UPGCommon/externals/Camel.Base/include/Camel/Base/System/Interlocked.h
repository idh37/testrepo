#ifndef __Camel_Base_System_Threading_Interlocked__
#define __Camel_Base_System_Threading_Interlocked__

#include "SystemBase.h"

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace Threading
{
	namespace Interlocked
	{
		CAMEL_BASE_SYSTEM_API int __stdcall Increment(volatile int *value);
		CAMEL_BASE_SYSTEM_API unsigned int __stdcall Increment(volatile unsigned int *value);

		CAMEL_BASE_SYSTEM_API int __stdcall Decrement(volatile int *value);
		CAMEL_BASE_SYSTEM_API unsigned int __stdcall Decrement(volatile unsigned int *value);

		CAMEL_BASE_SYSTEM_API int __stdcall Exchange(volatile int *dest, int value);
		CAMEL_BASE_SYSTEM_API unsigned int __stdcall Exchange(volatile unsigned int *dest, unsigned int value);

		CAMEL_BASE_SYSTEM_API int __stdcall CompareExchange(volatile int *dest, int value, int comperand);
		CAMEL_BASE_SYSTEM_API unsigned int __stdcall CompareExchange(volatile unsigned int *dest, unsigned int value, unsigned int comperand);
	} // namespace Interlocked
} // namespace Threading
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif //__Camel_Base_System_Threading_Interlocked__
