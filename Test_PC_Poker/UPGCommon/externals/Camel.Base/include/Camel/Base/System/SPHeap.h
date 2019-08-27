#ifndef __Camel_Base_System_SPHeap__
#define __Camel_Base_System_SPHeap__

#include "SystemBase.h"
#include "SystemHandles.h"
#include "PlatformConfig.h"

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace SPHeap
{
	typedef void (__stdcall *Event)(const Block ptr, void *context);

	CAMEL_BASE_SYSTEM_API Block __stdcall Alloc(size_t size);
	CAMEL_BASE_SYSTEM_API Block __stdcall Alloc(size_t size, Event onFree, void *context);
	CAMEL_BASE_SYSTEM_API void __stdcall Free(Block ptr);
	CAMEL_BASE_SYSTEM_API Block __stdcall Realloc(Block ptr, size_t newSize);
	CAMEL_BASE_SYSTEM_API Block __stdcall Realloc(Block ptr, size_t newSize, Event onFree, void *context);
	CAMEL_BASE_SYSTEM_API size_t __stdcall GetSize(const Block ptr);
	CAMEL_BASE_SYSTEM_API Block __stdcall Copy(const Block ptr);
	CAMEL_BASE_SYSTEM_API Block __stdcall Clone(const Block ptr);
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif // __Camel_Base_System_SPHeap__
