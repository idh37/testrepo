#ifndef __Camel_Base_Network_VersionInfo_API__
#define __Camel_Base_Network_VersionInfo_API__

#include "NetworkBase.h"

_DECLARE_NAMESPACE_CAMEL_BASE_NETWORK_
	CAMEL_BASE_NETWORK_API void __stdcall GetVersion(unsigned long *majorVersion, unsigned long *minorVersion, unsigned long *buildVersion1, unsigned long *buildVersion2);
	CAMEL_BASE_NETWORK_API void __stdcall GetProductVersion(unsigned long *majorVersion, unsigned long *minorVersion, unsigned long *buildVersion1, unsigned long *buildVersion2);
_UNDECLARE_NAMESPACE_CAMEL_BASE_NETWORK_
#endif // __Camel_Base_Network_VersionInfo_API__
