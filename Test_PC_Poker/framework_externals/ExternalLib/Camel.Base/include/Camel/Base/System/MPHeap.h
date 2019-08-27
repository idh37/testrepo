#ifndef __Camel_Base_System_MPHeap__
#define __Camel_Base_System_MPHeap__

#include "SystemBase.h"
#include "SystemHandles.h"
#include "PlatformConfig.h"

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace MPHeap
{
	CAMEL_BASE_SYSTEM_API Handle __stdcall CreateHandle(size_t numberOfConcurrentThreads);
	CAMEL_BASE_SYSTEM_API void __stdcall DestroyHandle(Handle heap);

	CAMEL_BASE_SYSTEM_API void * __stdcall Alloc(Handle heap, size_t size);
	CAMEL_BASE_SYSTEM_API void __stdcall Free(Handle heap, void *ptr);
	CAMEL_BASE_SYSTEM_API void * __stdcall Realloc(Handle heap, void *ptr, size_t newSize);
	CAMEL_BASE_SYSTEM_API size_t __stdcall GetSize(Handle heap, const void *ptr);
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif
