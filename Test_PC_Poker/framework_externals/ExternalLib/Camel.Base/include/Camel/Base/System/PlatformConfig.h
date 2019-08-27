#ifndef __Camel_Base_System_PlatformConfig__
#define __Camel_Base_System_PlatformConfig__

#if defined(_WIN32) || defined(_WIN64)
	typedef unsigned __int64 __uint64;
#else
	#include <stdint.h>

	#define _UNIX_

	#define __stdcall
	#define INFINITE    0

	typedef int64_t __int64;
	typedef uint64_t __uint64;

	template<typename T>
	inline T __max(T a, T b)
	{
		return (((a) > (b)) ? (a) : (b));
	}

	template<typename T>
	inline T __min(T a, T b)
	{
		return (((a) < (b)) ? (a) : (b));
	}
#endif

#endif // __Camel_Base_System_PlatformConfig__
