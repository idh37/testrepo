#ifndef __Camel_Base_SystemBase__
#define __Camel_Base_SystemBase__

#if defined(_WIN32) || defined(_WIN64)
    #ifdef SYSTEM_EXPORTS
        #define CAMEL_BASE_SYSTEM_API __declspec(dllexport)
    #else
        #define CAMEL_BASE_SYSTEM_API __declspec(dllimport)

        #ifndef _DEBUG
            #pragma comment(lib, "Camel.Base.System.lib")
        #else
            #pragma comment(lib, "Camel.Base.SystemD.lib")
        #endif
    #endif
#else
    #define SYSTEM_API
#endif

#define _DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_	namespace Camel \
												{ \
													namespace Base \
													{ \
														namespace System \
														{ 

#define _UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_	} } }

#endif // __Camel_Base_SystemBase__
