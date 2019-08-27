#ifndef __Camel_Base_System_Stream__
#define __Camel_Base_System_Stream__

#include "SystemBase.h"
#include "SystemHandles.h"
#include "SPHeap.h"

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace Stream
{
	typedef enum _HeapManager
	{
		USE_DEFAULT_HEAP = 0,
		USE_SPHEAP = 1,
		USE_LFHEAP = 2
	} HeapManager;

	CAMEL_BASE_SYSTEM_API Handle __stdcall CreateHandle(void);
	CAMEL_BASE_SYSTEM_API Handle __stdcall CreateHandle(HeapManager heapManager);
	CAMEL_BASE_SYSTEM_API void __stdcall DestroyHandle(Handle stream);

	CAMEL_BASE_SYSTEM_API size_t __stdcall Read(Handle stream, void *p, size_t size);
	CAMEL_BASE_SYSTEM_API size_t __stdcall Write(Handle stream, const void *p, size_t size);
	CAMEL_BASE_SYSTEM_API size_t __stdcall Peek(Handle stream, void *p, size_t size);
	CAMEL_BASE_SYSTEM_API size_t __stdcall RemoveLeft(Handle stream, size_t size);
	CAMEL_BASE_SYSTEM_API size_t __stdcall RemoveRight(Handle stream, size_t size);
	CAMEL_BASE_SYSTEM_API bool __stdcall Clear(Handle stream);
	CAMEL_BASE_SYSTEM_API size_t __stdcall GetLength(Handle stream);
	
	CAMEL_BASE_SYSTEM_API bool __stdcall Attach(Handle stream, SPHeap::Block p);
	CAMEL_BASE_SYSTEM_API SPHeap::Block __stdcall Detach(Handle stream);
	CAMEL_BASE_SYSTEM_API SPHeap::Block __stdcall GetRawPtr(Handle stream);
	
	CAMEL_BASE_SYSTEM_API Handle __stdcall Clone(Handle stream);
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif // __Camel_Base_System_Stream__
