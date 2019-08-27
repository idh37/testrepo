#ifndef __Camel_Base_Network_NetworkBase__
#define __Camel_Base_Network_NetworkBase__

#if defined(_WIN32) || defined(_WIN64)
	#ifdef NETWORK_EXPORTS
		#define CAMEL_BASE_NETWORK_API __declspec(dllexport)
	#else
		#define CAMEL_BASE_NETWORK_API __declspec(dllimport)

		#ifndef _DEBUG
			#pragma comment(lib, "Camel.Base.Network.lib")
		#else
			#pragma comment(lib, "Camel.Base.NetworkD.lib")
		#endif
	#endif
#else
	#define CAMEL_BASE_NETWORK_API
#endif

#define _DECLARE_NAMESPACE_CAMEL_BASE_NETWORK_	namespace Camel \
												{ \
													namespace Base \
													{ \
														namespace Network \
														{ 

#define _UNDECLARE_NAMESPACE_CAMEL_BASE_NETWORK_	} } }

#ifndef _WINSOCK2API_
#include <WinSock2.h>
#include <Windows.h>
#endif // _WINSOCK2API_

#endif // __Camel_Base_Network_NetworkBase__
