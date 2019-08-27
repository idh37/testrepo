#ifndef __Camel_Base_Network_Link__
#define __Camel_Base_Network_Link__

#include "NetworkBase.h"
#include "NetworkHandles.h"
#include <Camel/Base/System/System.h>

_DECLARE_NAMESPACE_CAMEL_BASE_NETWORK_
namespace Link
{
	typedef void (__stdcall *Event)(Handle, void *);
	
	CAMEL_BASE_NETWORK_API bool    __stdcall SetMaxAsyncUserBufferSize(Handle link, size_t maxBufferSize);
	CAMEL_BASE_NETWORK_API int  __stdcall GetMaxAsyncUserBufferSize(Handle link);

	CAMEL_BASE_NETWORK_API int __stdcall AddRef(Handle link);
	CAMEL_BASE_NETWORK_API int __stdcall Release(Handle link);

	CAMEL_BASE_NETWORK_API bool __stdcall Close(Handle link);

	CAMEL_BASE_NETWORK_API Address::Handle __stdcall GetLocalAddress(Handle link);
	CAMEL_BASE_NETWORK_API Address::Handle __stdcall GetRemoteAddress(Handle link);

	CAMEL_BASE_NETWORK_API bool __stdcall Send(Handle link, const void *data, size_t len);
	CAMEL_BASE_NETWORK_API bool __stdcall Send(Handle link, const System::SPHeap::Block data);
	CAMEL_BASE_NETWORK_API bool __stdcall Send(Handle link, const System::SPHeap::Block data, size_t len);
	CAMEL_BASE_NETWORK_API bool __stdcall Recv(Handle link, size_t len);

	CAMEL_BASE_NETWORK_API bool __stdcall SetData(Handle link, void *data);
	CAMEL_BASE_NETWORK_API bool __stdcall SetData(Handle link, void *data, const Event &onDestroy, void *context);
	CAMEL_BASE_NETWORK_API void * __stdcall GetData(Handle link);

	CAMEL_BASE_NETWORK_API bool __stdcall AddAttribute(Handle link, const char *name, void *data);
	CAMEL_BASE_NETWORK_API bool __stdcall RemoveAttribute(Handle link, const char *name);
	CAMEL_BASE_NETWORK_API void * __stdcall GetAttribute(Handle link, const char *name);

	CAMEL_BASE_NETWORK_API System::Stream::Handle __stdcall LockReadStream(Handle link);
	CAMEL_BASE_NETWORK_API bool __stdcall UnlockReadStream(Handle link);

	CAMEL_BASE_NETWORK_API LinkConfig::Handle GetConfig(Handle link);

	CAMEL_BASE_NETWORK_API bool __stdcall SetSendBufferSize(Handle link, int value);
	CAMEL_BASE_NETWORK_API int __stdcall GetSendBufferSize(Handle link);
	CAMEL_BASE_NETWORK_API bool __stdcall SetRecvBufferSize(Handle link, int value);
	CAMEL_BASE_NETWORK_API int __stdcall GetRecvBufferSize(Handle link);

	CAMEL_BASE_NETWORK_API bool __stdcall SetNoDelay(Handle link, bool value);
	CAMEL_BASE_NETWORK_API bool __stdcall GetNoDelay(Handle link, bool *value);
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_NETWORK_
#endif // __Camel_Base_Network_Link__
