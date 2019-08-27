// BadFilter.h
//
//////////////////////////////////////////////////////////////////////
#pragma once


#ifndef BADFILTER_IMPORT
enum BADFILTER_ERROR
{
	SUCCESS = 0,
	INITVALUE_FAIL = 1,		//- 초기화 변수 Error
	DLLLOAD_FAIL = 2,		//- Dll load error
	FUNCLOAD_FAIL = 3,		//- Dll Function Load error
	FILELOAD_FAIL = 4,		//- File Load error
};
#else
#include <BadFilterLoader.h>
#endif

#include "UtilDefine.h"

namespace NMBASE
{
	namespace UTIL
	{

		//- Common Function !!!
		//- Filter Level 변경 ( 1번은 기존방식, 2번은 추가된 방식(권장) )
		NMUTIL_API void				NMSetFilterLevel( int nLevel );
		//- Filter 기능 사용여부
		NMUTIL_API void				NMSetFilterEnable( BOOL bEnable );
		NMUTIL_API BOOL				NMIsFilterEnable();

		//////-------------------------------------------------------------------------//
		//////------------------------ UNICODE FUNCTION -------------------------------//
		//////_________________________________________________________________________//
		// [9/30/2008 Bad Filter]
		//- BadFilter 초기화 함수
		//- shared라면, szDllName을  BadFilterSHD.dll로 할것
		//- Static이라면, szDllName을  BadFilterSTD.dll로 할것
		NMUTIL_API extern BADFILTER_ERROR		NMInitFilterW( wchar_t* szAvatar );
		//extern "C"	BADFILTER_ERROR		InitFilter( wchar_t*  szAvatar );
		//- Dll Path 변경 ( 해당 Path에는 Bad Filter File도 존재해야 한다. )
		NMUTIL_API extern void					NMSetFilterDllPathW( wchar_t* szDllFullPath );
		//- Filter File 변경 ( 경로와 함께 넣어주어야 한다. )
		NMUTIL_API extern void					NMSetFilterFileW( wchar_t* szFilterFile );

		//- Bad Word의 존재 여부를 Return
		NMUTIL_API extern BOOL					NMIsBadW( wchar_t* str );
		//- Bad Word를 rep로 대체 하는 함수 ( strOut으로 return )
		NMUTIL_API extern BOOL					NMIsBadToRepW( wchar_t* str, wchar_t* strOut, DWORD nOut, wchar_t rep = '*' );


		//////-------------------------------------------------------------------------//
		//////------------------------ MULTIBYTE FUNCTION -------------------------------//
		//////_________________________________________________________________________//
		// Multibyte
		NMUTIL_API extern BADFILTER_ERROR		NMInitFilter( char* szAvatar );
		//- Dll Path 변경 ( 해당 Path에는 Bad Filter File도 존재해야 한다. )
		NMUTIL_API extern void					NMSetFilterDllPath( char* szDllFullPath );
		//- Filter File 변경 ( 경로와 함께 넣어주어야 한다. )
		NMUTIL_API extern void					NMSetFilterFile( char* szFilterFile );

		//- Bad Word의 존재 여부를 Return
		NMUTIL_API extern BOOL					NMIsBad( char* str );
		//- Bad Word를 rep로 대체 하는 함수 ( strOut으로 return )
		NMUTIL_API extern BOOL					NMIsBadToRep( char* str, char* strOut, DWORD nOut, char rep = '*' );

	}//namespace UTIL

}//namespace NMBASE
