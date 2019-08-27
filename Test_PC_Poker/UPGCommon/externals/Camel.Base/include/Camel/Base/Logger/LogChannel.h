#ifndef __Camel_Base_Logger_LogChannel__
#define __Camel_Base_Logger_LogChannel__

#include "LoggerBase.h"
#include "LoggerHandles.h"
#include <Camel/Base/System/Types.h>

#include <string>

_DECLARE_NAMESPACE_CAMEL_BASE_LOGGER_
namespace LogChannel
{
	namespace Type
	{
		enum _Type
		{
			LCCT_Consol		= 1,
			LCCT_File		= 2,
			LCCT_FileShm	= 3,
			LCCT_SysLog		= 4
		};
	}
	namespace Option
	{
		enum _Option
		{
			LCOP_Rotate_None		= 0x8000,
			LCOP_Rotate_T_Hour		= 0x4000,
			LCOP_Rotate_T_Day		= 0x2000,
			LCOP_Rotate_S_100M		= 0x1000,
			LCOP_Rotate_S_1G		= 0x0800,
			LCOP_DIRECT_DISK		= 0x0400,
			LCOP_CONSOL_PRINT		= 0x0200,
		};
	}

	CAMEL_BASE_LOGGER_API Handle __stdcall CreateHandle(Type::_Type type, const std::string& name, const std::string& path, const std::string& prefix, unsigned int rotateflags);
	CAMEL_BASE_LOGGER_API void __stdcall DestroyHandle(Handle handle);
	CAMEL_BASE_LOGGER_API bool __stdcall Open(Handle handle);
	CAMEL_BASE_LOGGER_API void __stdcall Close(Handle handle);
	CAMEL_BASE_LOGGER_API size_t __stdcall Write(Handle handle, const std::string& line);
	CAMEL_BASE_LOGGER_API void __stdcall Flush(Handle handle);
	CAMEL_BASE_LOGGER_API bool __stdcall IsOpen(Handle handle);
	//CAMEL_BASE_LOGGER_API Type::_Type __stdcall GetType(Handle handle);
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_LOGGER_
#endif // __Camel_Base_Logger_LogChannel__