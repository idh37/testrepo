#ifndef __Camel_Base_System_ThreadPool__
#define __Camel_Base_System_ThreadPool__

#include "SystemBase.h"
#include "SystemHandles.h"
#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif
#include <cstddef>

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace ThreadPool
{
	namespace ThreadStatus
	{
        enum Type
        {
            IDLE = 0,
            RUN,
        };
	}
	typedef unsigned int (__stdcall *ThreadEvent)(void *context);
	typedef void (__stdcall *OverlappedCompletionEvent)(unsigned int errorCode, unsigned int numberOfBytesTransferred, void *context);

	CAMEL_BASE_SYSTEM_API Handle __stdcall CreateHandle(size_t size);
	CAMEL_BASE_SYSTEM_API void __stdcall DestroyHandle(Handle threadPool);

	CAMEL_BASE_SYSTEM_API size_t __stdcall GetPoolSize(Handle threadPool);
	CAMEL_BASE_SYSTEM_API size_t __stdcall GetUsingPoolSize(Handle threadPool);
	CAMEL_BASE_SYSTEM_API size_t __stdcall GetMaxUsedPoolSize(Handle threadPool);
	CAMEL_BASE_SYSTEM_API size_t __stdcall GetUsedPoolSize(Handle threadPool);
	CAMEL_BASE_SYSTEM_API void   __stdcall GetUsingPoolSizeEx(Handle threadPool, size_t* usingSize, size_t* usingQueuedDefaultSize, size_t* usingQueuedFuncSize, size_t* usingQueuedBindFuncSize);

#if defined(_WIN32) || defined(_WIN64)
	CAMEL_BASE_SYSTEM_API bool __stdcall BindCompletionCallback(Handle threadPool, HANDLE handle, OverlappedCompletionEvent completionCallback);
#endif

	CAMEL_BASE_SYSTEM_API bool __stdcall QueueWorkItem(Handle threadPool, ThreadEvent callback, void *context);
	CAMEL_BASE_SYSTEM_API bool __stdcall QueueWorkItem(Handle threadPool, OverlappedCompletionEvent callback, unsigned int errorCode, unsigned int numberOfBytesTransferred, void *context);
	
    CAMEL_BASE_SYSTEM_API bool __stdcall GetThreadID(Handle threadPool, size_t index, unsigned int* threadID);
    CAMEL_BASE_SYSTEM_API bool __stdcall GetThreadStatus(Handle threadPool, size_t index, ThreadStatus::Type* status);
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif // __Camel_Base_System_ThreadPool__