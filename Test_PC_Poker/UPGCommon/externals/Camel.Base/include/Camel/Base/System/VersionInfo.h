#ifndef __Camel_Base_System_VersionInfo_API__
#define __Camel_Base_System_VersionInfo_API__

#include "SystemBase.h"

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
	CAMEL_BASE_SYSTEM_API void __stdcall GetVersion(unsigned long *majorVersion, unsigned long *minorVersion, unsigned long *buildVersion1, unsigned long *buildVersion2);
	CAMEL_BASE_SYSTEM_API void __stdcall GetProductVersion(unsigned long *majorVersion, unsigned long *minorVersion, unsigned long *buildVersion1, unsigned long *buildVersion2);
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif // __Camel_Base_System_VersionInfo_API__
