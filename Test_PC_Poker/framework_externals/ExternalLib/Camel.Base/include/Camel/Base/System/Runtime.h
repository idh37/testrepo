#ifndef __Camel_Base_System_Runtime__
#define __Camel_Base_System_Runtime__

#include "SystemBase.h"
#include "PlatformConfig.h"
//#include <cstddef>

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace Runtime
{
	CAMEL_BASE_SYSTEM_API void __stdcall SetLastError(unsigned int errorCode);
	CAMEL_BASE_SYSTEM_API unsigned int __stdcall GetLastError(void);
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif
