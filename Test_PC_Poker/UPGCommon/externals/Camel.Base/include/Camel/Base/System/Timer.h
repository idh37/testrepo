#ifndef __Camel_Base_System_Timers__
#define __Camel_Base_System_Timers__

#include "SystemBase.h"
#include "SystemHandles.h"
//#include "ThreadPool.h"

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace Timers
{
	namespace TimerQueue
	{
		CAMEL_BASE_SYSTEM_API Handle __stdcall CreateHandle(ThreadPool::Handle threadPool);
		CAMEL_BASE_SYSTEM_API void __stdcall DestroyHandle(Handle timerQueue);
	}

	namespace Timer
	{
		typedef void (__stdcall *Event)(void *context);

		CAMEL_BASE_SYSTEM_API Handle __stdcall CreateHandle(TimerQueue::Handle timerQueue, unsigned int interval, Event callback, void *context);
		CAMEL_BASE_SYSTEM_API void __stdcall DestroyHandle(Handle timer);
		CAMEL_BASE_SYSTEM_API bool __stdcall ResumeTimer(Handle timer);
		CAMEL_BASE_SYSTEM_API bool __stdcall StopTimer(Handle timer);
	}
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif
