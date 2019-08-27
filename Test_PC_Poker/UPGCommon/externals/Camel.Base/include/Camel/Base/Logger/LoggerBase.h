#ifndef __Camel_Base_Logger_LoggerBase__
#define __Camel_Base_Logger_LoggerBase__

#if defined(_WIN32) || defined(_WIN64)
	#ifdef LOGGER_EXPORTS
		#define CAMEL_BASE_LOGGER_API __declspec(dllexport)
	#else
		#define CAMEL_BASE_LOGGER_API __declspec(dllimport)

		#ifndef _DEBUG
			#pragma comment(lib, "Camel.Base.Logger.lib")
		#else
			#pragma comment(lib, "Camel.Base.LoggerD.lib")
		#endif
	#endif
#else
	#define CAMEL_BASE_LOGGER_API
#endif

#define _DECLARE_NAMESPACE_CAMEL_BASE_LOGGER_	namespace Camel \
												{ \
													namespace Base \
													{ \
														namespace Logger \
														{ 

#define _UNDECLARE_NAMESPACE_CAMEL_BASE_LOGGER_	} } }

#include <tchar.h>

#endif // __Camel_Base_Logger_LoggerBase__