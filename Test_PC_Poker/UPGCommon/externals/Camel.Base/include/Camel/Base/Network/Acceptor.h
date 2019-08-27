#ifndef __Camel_Base_Network_Acceptor__
#define __Camel_Base_Network_Acceptor__

#include "NetworkBase.h"
#include "NetworkHandles.h"
#include <Camel/Base/System/System.h>

_DECLARE_NAMESPACE_CAMEL_BASE_NETWORK_
namespace Acceptor
{
	// typedef struct _Handle {} *Handle;
	// typedef void (__stdcall *Event)(Handle, void *, void *);
	typedef void (__stdcall *Event)(Handle, Link::Handle, void *);
	

	CAMEL_BASE_NETWORK_API Handle __stdcall CreateHandle(System::ThreadPool::Handle threadPool);
	CAMEL_BASE_NETWORK_API Handle __stdcall CreateHandle(System::ThreadPool::Handle threadPool, FilterChain::Handle filterChain);
	CAMEL_BASE_NETWORK_API void __stdcall DestroyHandle(Handle acceptor);

	CAMEL_BASE_NETWORK_API bool __stdcall Start(Handle acceptor);
	CAMEL_BASE_NETWORK_API bool __stdcall Stop(Handle acceptor);

	CAMEL_BASE_NETWORK_API bool __stdcall SetDefaultPort(Handle acceptor, unsigned short port, bool reuse = true);
	CAMEL_BASE_NETWORK_API unsigned short __stdcall GetDefaultPort(Handle acceptor);

//	CAMEL_BASE_NETWORK_API bool __stdcall AddBinding(Handle acceptor, const char *ipAddress, unsigned short port);
//	CAMEL_BASE_NETWORK_API bool __stdcall AddBinding(Handle acceptor, const char *ipAddress, unsigned short port, bool reuse);
//	CAMEL_BASE_NETWORK_API bool __stdcall RemoveBinding(Handle acceptor, const char *ipAddress, unsigned short port);
//	CAMEL_BASE_NETWORK_API bool __stdcall ClearBindings(Handle acceptor);

//	CAMEL_BASE_NETWORK_API ActiveLinkList::Handle __stdcall LockActiveLinks(Handle acceptor);
//	CAMEL_BASE_NETWORK_API bool __stdcall UnlockActiveLinks(Handle acceptor);

	CAMEL_BASE_NETWORK_API bool __stdcall SetOnAccepted(Handle acceptor, const Event &callback, void *context);
	CAMEL_BASE_NETWORK_API bool __stdcall SetOnReceived(Handle acceptor, const Event &callback, void *context);
	CAMEL_BASE_NETWORK_API bool __stdcall SetOnClosed(Handle acceptor, const Event &callback, void *context);
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_NETWORK_
#endif // __Camel_Base_Network_Acceptor__
