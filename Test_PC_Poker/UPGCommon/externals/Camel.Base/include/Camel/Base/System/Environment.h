#ifndef __Camel_Base_System_Environment__
#define __Camel_Base_System_Environment__

#include "SystemBase.h"

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace Environment
{
	CAMEL_BASE_SYSTEM_API size_t __stdcall GetProcessorCount(void);
	CAMEL_BASE_SYSTEM_API bool   __stdcall IsWindows2000(void);
	CAMEL_BASE_SYSTEM_API bool   __stdcall GetOSVersion(unsigned long *ulplatformId, unsigned long *ulMajorVersion, unsigned long *ulMinorVersion);
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif // __Camel_Base_System_Environment__
