#ifndef __Camel_Base_Network_FilterChain__
#define __Camel_Base_Network_FilterChain__

#include "NetworkBase.h"
#include "NetworkHandles.h"
#include <Camel/Base/System/System.h>

_DECLARE_NAMESPACE_CAMEL_BASE_NETWORK_
namespace FilterChain
{
	CAMEL_BASE_NETWORK_API Handle __stdcall CreateHandle(void);
	CAMEL_BASE_NETWORK_API void __stdcall DestroyHandle(Handle filterChain);

	CAMEL_BASE_NETWORK_API bool __stdcall Add(Handle filterChain, const char *name, Filter::Handle filter);
	CAMEL_BASE_NETWORK_API bool __stdcall Remove(Handle filterChain, const char *name);
	CAMEL_BASE_NETWORK_API Filter::Handle __stdcall Find(Handle filterChain, const char *name);
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_NETWORK_
#endif
