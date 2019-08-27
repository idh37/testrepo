#ifndef __Camel_Base_Logger__
#define __Camel_Base_Logger__

#include "Wtrace.h"
//#include "LogFile.h"
#include "LogChannel.h"
#include "LoggerBase.h"
#include "LoggerHandles.h"

#include <Camel/Base/System/Types.h>
#include <string>

_DECLARE_NAMESPACE_CAMEL_BASE_LOGGER_
	namespace Priority
	{
		enum Type
		{
			PRIO_NULL = 0,
			PRIO_FATAL = 1,
			PRIO_CRITICAL,
			PRIO_ERROR,
			PRIO_WARNING,
			PRIO_NOTICE,
			PRIO_INFORMATION,
			PRIO_DEBUG,
			PRIO_TRACE
		};
	}
	CAMEL_BASE_LOGGER_API Handle __stdcall CreateHandle();
	CAMEL_BASE_LOGGER_API Handle __stdcall CreateHandle(const std::string& key, LogChannel::Handle loghandle, Priority::Type priority);
	CAMEL_BASE_LOGGER_API void __stdcall DestroyHandle(Handle handle);
	CAMEL_BASE_LOGGER_API void __stdcall Log(Handle handle, Priority::Type priority, const char* fmt, ...);
	CAMEL_BASE_LOGGER_API void __stdcall vLog(Handle handle, Priority::Type priority, const char* fmt, va_list ap);
	CAMEL_BASE_LOGGER_API void __stdcall Fatal(Handle handle, const char* fmt, ...);
	CAMEL_BASE_LOGGER_API void __stdcall Critical(Handle handle, const char* fmt, ...);
	CAMEL_BASE_LOGGER_API void __stdcall Error(Handle handle, const char* fmt, ...);
	CAMEL_BASE_LOGGER_API void __stdcall Warning(Handle handle, const char* fmt, ...);
	CAMEL_BASE_LOGGER_API void __stdcall Notice(Handle handle, const char* fmt, ...);
	CAMEL_BASE_LOGGER_API void __stdcall Information(Handle handle, const char* fmt, ...);
	CAMEL_BASE_LOGGER_API void __stdcall Debug(Handle handle, const char* fmt, ...);
	CAMEL_BASE_LOGGER_API void __stdcall Trace(Handle handle, const char* fmt, ...);
		
	CAMEL_BASE_LOGGER_API bool __stdcall Log(Handle handle, const std::string& key, Priority::Type priority, const char* fmt, ...);
	CAMEL_BASE_LOGGER_API bool __stdcall vLog(Handle handle, const std::string& key, Priority::Type priority, const char* fmt, va_list ap);
	CAMEL_BASE_LOGGER_API bool __stdcall Fatal(Handle handle, const std::string& key, const char* fmt, ...);
	CAMEL_BASE_LOGGER_API bool __stdcall Critical(Handle handle, const std::string& key, const char* fmt, ...);
	CAMEL_BASE_LOGGER_API bool __stdcall Error(Handle handle, const std::string& key, const char* fmt, ...);
	CAMEL_BASE_LOGGER_API bool __stdcall Warning(Handle handle, const std::string& key, const char* fmt, ...);
	CAMEL_BASE_LOGGER_API bool __stdcall Notice(Handle handle, const std::string& key, const char* fmt, ...);
	CAMEL_BASE_LOGGER_API bool __stdcall Information(Handle handle, const std::string& key, const char* fmt, ...);
	CAMEL_BASE_LOGGER_API bool __stdcall Debug(Handle handle, const std::string& key, const char* fmt, ...);
	CAMEL_BASE_LOGGER_API bool __stdcall Trace(Handle handle, const std::string& key, const char* fmt, ...);

	CAMEL_BASE_LOGGER_API Priority::Type __stdcall GetPriority(Handle handle);

	CAMEL_BASE_LOGGER_API bool __stdcall AddLogChannel(Handle handle, const std::string& key, LogChannel::Handle loghandle, Priority::Type priority);
	CAMEL_BASE_LOGGER_API bool __stdcall ReleaseLogChannel(Handle handle, const std::string& key);

_UNDECLARE_NAMESPACE_CAMEL_BASE_LOGGER_

#endif //  __Camel_Base_Logger__