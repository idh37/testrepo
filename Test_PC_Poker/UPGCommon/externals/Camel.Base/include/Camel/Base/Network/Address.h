#ifndef __Camel_Base_Network_Address__
#define __Camel_Base_Network_Address__

#include "NetworkBase.h"
#include "NetworkHandles.h"
#include "Consts.h"

_DECLARE_NAMESPACE_CAMEL_BASE_NETWORK_
namespace Address
{
    CAMEL_BASE_NETWORK_API Handle __stdcall CreateHandle(const char *ipAddress, unsigned short port, Family::Type addressFamily = Family::IPv4);
    CAMEL_BASE_NETWORK_API Handle __stdcall CreateHandle(unsigned long ipAddress, unsigned short port, Family::Type addressFamily = Family::IPv4);
    CAMEL_BASE_NETWORK_API void __stdcall DestroyHandle(Handle address);

	CAMEL_BASE_NETWORK_API bool __stdcall GetIPAddress(Handle addr, char* ip, size_t len);
	CAMEL_BASE_NETWORK_API bool __stdcall GetPort(Handle addr, unsigned short* port);
	CAMEL_BASE_NETWORK_API bool __stdcall GetFamily(Handle address, Family::Type *family);
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_NETWORK_
#endif // __Camel_Base_Network_Address__
