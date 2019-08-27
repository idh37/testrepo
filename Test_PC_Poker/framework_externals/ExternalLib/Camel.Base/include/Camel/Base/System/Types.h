#ifndef __Camel_Base_System_Types__
#define __Camel_Base_System_Types__

namespace Camel
{
#if defined(_MSC_VER)
	// Windows/Visual C++
	typedef unsigned char			byte;
	typedef signed char				int8;
	typedef unsigned char			uint8;
	typedef signed short			int16;
	typedef unsigned short			uint16;
	typedef signed int				int32;
	typedef unsigned int			uint32;
	typedef signed __int64			int64;
	typedef unsigned __int64		uint64;
	#if defined(_WIN64)
		#define CAMEL_PTR_IS_64_BIT 1
		typedef signed __int64     IntPtr;
		typedef unsigned __int64   UIntPtr;
	#else
		typedef signed long        intPtr;
		typedef unsigned long      uintPtr;
	#endif
	#define CAMEL_HAVE_INT64 1
#elif defined(__GNUC__)
	// Unix/GCC
	typedef unsigned char			byte;
	typedef signed char				int8;
	typedef unsigned char			uint8;
	typedef signed short			int16;
	typedef unsigned short			uint16;
	typedef signed int				int32;
	typedef unsigned int			uint32;
	typedef signed long				intPtr;
	typedef unsigned long			uintPtr;
	#if defined(__LP64__)
		#define CAMEL_PTR_IS_64_BIT 1
		#define CAMEL_LONG_IS_64_BIT 1
		typedef signed long        Int64;
		typedef unsigned long      UInt64;
	#else
		typedef signed long long   Int64;
		typedef unsigned long long UInt64;
	#endif
	#define CAMEL_HAVE_INT64 1
#endif // defined(_MSC_VER)

#ifdef _UNICODE
#define tstring std::wstring
#define tofstream std::wofstream
#define tifstream std::wifstream 
#define tostream  std::wostream
#define tistream  std::wifstream
#define tostringstream std::wostringstream
#else
#define tstring std::string
#define tofstream std::ofstream
#define tifstream std::ifstream
#define tostream  std::ostream
#define tistream  std::ifstream
#define tostringstream std::ostringstream
#endif // _UNICODE
}
#endif // __Camel_Base_System_Types__