#ifndef __Camel_Base_Network_Filter__
#define __Camel_Base_Network_Filter__

#include "NetworkBase.h"
#include "NetworkHandles.h"
#include <Camel/Base/System/System.h>

_DECLARE_NAMESPACE_CAMEL_BASE_NETWORK_
namespace Filter
{
	typedef void (__stdcall *Event)(Handle, Link::Handle, void *);
	typedef void (__stdcall *IOEvent)(Handle, Link::Handle, System::SPHeap::Block, size_t, void *);

	CAMEL_BASE_NETWORK_API Handle __stdcall CreateHandle(void);
	CAMEL_BASE_NETWORK_API void __stdcall DestroyHandle(Handle filter);

	CAMEL_BASE_NETWORK_API Handle __stdcall GetPrev(Handle filter);
	CAMEL_BASE_NETWORK_API Handle __stdcall GetNext(Handle filter);

	CAMEL_BASE_NETWORK_API bool __stdcall SetOnLinkOpened(Handle filter, const Event &callback, void *context);
	CAMEL_BASE_NETWORK_API bool __stdcall SetOnLinkClosed(Handle filter, const Event &callback, void *context);

	CAMEL_BASE_NETWORK_API bool __stdcall SetOnSend(Handle filter, const IOEvent &callback, void *context);
	CAMEL_BASE_NETWORK_API bool __stdcall SetOnReceived(Handle filter, const IOEvent &callback, void *context);

	CAMEL_BASE_NETWORK_API bool __stdcall DoOnLinkOpened(Handle filter, Link::Handle link);
	CAMEL_BASE_NETWORK_API bool __stdcall DoOnLinkClosed(Handle filter, Link::Handle link);

	CAMEL_BASE_NETWORK_API bool __stdcall DoOnSend(Handle filter, Link::Handle link, System::SPHeap::Block buffer, size_t size);
	CAMEL_BASE_NETWORK_API bool __stdcall DoOnReceived(Handle filter, Link::Handle link, System::SPHeap::Block buffer, size_t size);
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_NETWORK_
#endif // __Camel_Base_Network_Filter__
