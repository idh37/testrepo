#ifndef __Camel_Base_Network_Connector__
#define __Camel_Base_Network_Connector__

#include "NetworkBase.h"
#include "NetworkHandles.h"
#include <Camel/Base/System/System.h>

_DECLARE_NAMESPACE_CAMEL_BASE_NETWORK_
namespace Connector
{
	typedef void (__stdcall *Event)(Handle, Link::Handle, void *);
	typedef void (__stdcall *EventEx)(Handle, Link::Handle, void *, void *);

	CAMEL_BASE_NETWORK_API Handle __stdcall CreateHandle(System::ThreadPool::Handle threadPool);
	CAMEL_BASE_NETWORK_API Handle __stdcall CreateHandle(System::ThreadPool::Handle threadPool, FilterChain::Handle filterChain);
	CAMEL_BASE_NETWORK_API void __stdcall DestroyHandle(Handle connector);

	CAMEL_BASE_NETWORK_API bool __stdcall Connect(Handle connector, const char *ipAddress, unsigned short port);
	CAMEL_BASE_NETWORK_API bool __stdcall Connect(Handle connector, const char *ipAddress, unsigned short port, int timeout);
	CAMEL_BASE_NETWORK_API bool __stdcall Connect(Handle connector, const char *ipAddress, unsigned short port, int timeout, Interrupter::Handle interrupter);

	CAMEL_BASE_NETWORK_API bool __stdcall ConnectEx(Handle connector, const char *ipAddress, unsigned short port, void *contextEx);
	CAMEL_BASE_NETWORK_API bool __stdcall ConnectEx(Handle connector, const char *ipAddress, unsigned short port, int timeout, void *contextEx);
	CAMEL_BASE_NETWORK_API bool __stdcall ConnectEx(Handle connector, const char *ipAddress, unsigned short port, int timeout, Interrupter::Handle interrupter, void *contextEx);

	CAMEL_BASE_NETWORK_API ActiveLinkList::Handle __stdcall LockActiveLinks(Handle connector);
	CAMEL_BASE_NETWORK_API bool __stdcall UnlockActiveLinks(Handle connector);

	CAMEL_BASE_NETWORK_API bool __stdcall SetOnConnected(Handle connector, const Event &callback, void *context);
	CAMEL_BASE_NETWORK_API bool __stdcall SetOnConnectedEx(Handle connector, const EventEx &callback, void *context);
	CAMEL_BASE_NETWORK_API bool __stdcall SetOnReceived(Handle connector, const Event &callback, void *context);
	CAMEL_BASE_NETWORK_API bool __stdcall SetOnDisconnected(Handle connector, const Event &callback, void *context);
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_NETWORK_
#endif // __Camel_Base_Network_Connector__