#ifndef __Camel_Base_Database_DatabaseBase__
#define __Camel_Base_Database_DatabaseBase__

#if defined(_WIN32) || defined(_WIN64)
	#ifdef DATABASE_EXPORTS
		#define CAMEL_BASE_DATABASE_API __declspec(dllexport)
	#else
		#define CAMEL_BASE_DATABASE_API __declspec(dllimport)

		#ifndef _DEBUG
			#pragma comment(lib, "Camel.Base.Database.lib")
		#else
			#pragma comment(lib, "Camel.Base.DatabaseD.lib")
		#endif
	#endif
#else
	#define CAMEL_BASE_DATABASE_API
#endif

#define _DECLARE_NAMESPACE_CAMEL_BASE_DATABASE_	namespace Camel \
												{ \
													namespace Base \
													{ \
														namespace Database \
														{ 

#define _UNDECLARE_NAMESPACE_CAMEL_BASE_DATABASE_	} } }

#endif // __Camel_Base_Network_NetworkBase__
