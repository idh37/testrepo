
#ifndef __Camel_Base_Logger_Wtrace__
#define __Camel_Base_Logger_Wtrace__

#if defined(_WIN32) || defined(_WIN64)
	#include <crtdbg.h>
	#include <stdio.h>

	#ifdef _DEBUG
		////// WTRACE 루틴 
		inline void __cdecl DbgTrace(const char * format, ...)
		{
			char buf[2048];
			char * arg = (char *)&format + sizeof(format);
			if( _vsnprintf_s(buf, sizeof(buf)-1, format, arg) == -1 )
				buf[sizeof(buf)-1] = '\0';

			_RPT0(_CRT_WARN, buf);
		}
		#undef	WTRACE
		#define WTRACE DbgTrace
		#undef	WTRACE2
		#define WTRACE2 WTRACE("%s(%d) - ", __FUNCTION__, __LINE__); WTRACE

		////// PROFILE 루틴

		#define RDTSC(var) \
				{\
				_asm _emit 0x0F \
				_asm _emit 0x31 \
				_asm mov DWORD PTR var,eax \
				_asm mov DWORD PTR var+4,edx \
				}

		// 사용예)	PROFILE( nTimespan, DoAnyThing(...))
		#define	WPROFILE(destvar,func)	\
				{\
					__int64	i64_start, i64_end;\
					RDTSC(i64_start) \
					(func); \
					RDTSC(i64_end) \
					(destvar) = (int)(i64_end-i64_start);\
				}
	#else
		#undef  WTRACE

		#if (_MSC_VER >= 1310)
			#define WTRACE	__noop
		#else
			#define WTRACE	((void)0)
		#endif

		#define	WPROFILE(destvar, func)		(func);
	#endif // _DEBUG
#else // _WIN32 || _WIN64
	#ifdef _DEBUG
		#undef	WTRACE
		#define WTRACE printf
	#else
		#undef  WTRACE
		#define WTRACE

	#endif // _DEBUG
#endif // _WIN32 || _WIN64

#endif // __Camel_Base_Logger_Wtrace__