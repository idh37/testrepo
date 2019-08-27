#ifndef __Camel_Base_Network_ActiveLinkList__
#define __Camel_Base_Network_ActiveLinkList__

#include "NetworkBase.h"
#include "NetworkHandles.h"
#include <cstddef>

_DECLARE_NAMESPACE_CAMEL_BASE_NETWORK_
namespace ActiveLinkList
{
	CAMEL_BASE_NETWORK_API Link::Handle __stdcall GetLink(Handle activeLinkList, size_t index);
	CAMEL_BASE_NETWORK_API size_t __stdcall GetSize(Handle activeLinkList);
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_NETWORK_
#endif // __Camel_Base_Network_ActiveLinkList__
