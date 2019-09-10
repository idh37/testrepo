// BadFilter.h
//
//////////////////////////////////////////////////////////////////////
#pragma once


#ifndef BADFILTER_IMPORT
enum BADFILTER_ERROR
{
	SUCCESS = 0,
	INITVALUE_FAIL = 1,		//- �ʱ�ȭ ���� Error
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
		//- Filter Level ���� ( 1���� �������, 2���� �߰��� ���(����) )
		NMUTIL_API void				NMSetFilterLevel( int nLevel );
		//- Filter ��� ��뿩��
		NMUTIL_API void				NMSetFilterEnable( BOOL bEnable );
		NMUTIL_API BOOL				NMIsFilterEnable();

		//////-------------------------------------------------------------------------//
		//////------------------------ UNICODE FUNCTION -------------------------------//
		//////_________________________________________________________________________//
		// [9/30/2008 Bad Filter]
		//- BadFilter �ʱ�ȭ �Լ�
		//- shared���, szDllName��  BadFilterSHD.dll�� �Ұ�
		//- Static�̶��, szDllName��  BadFilterSTD.dll�� �Ұ�
		NMUTIL_API extern BADFILTER_ERROR		NMInitFilterW( wchar_t* szAvatar );
		//extern "C"	BADFILTER_ERROR		InitFilter( wchar_t*  szAvatar );
		//- Dll Path ���� ( �ش� Path���� Bad Filter File�� �����ؾ� �Ѵ�. )
		NMUTIL_API extern void					NMSetFilterDllPathW( wchar_t* szDllFullPath );
		//- Filter File ���� ( ��ο� �Բ� �־��־�� �Ѵ�. )
		NMUTIL_API extern void					NMSetFilterFileW( wchar_t* szFilterFile );

		//- Bad Word�� ���� ���θ� Return
		NMUTIL_API extern BOOL					NMIsBadW( wchar_t* str );
		//- Bad Word�� rep�� ��ü �ϴ� �Լ� ( strOut���� return )
		NMUTIL_API extern BOOL					NMIsBadToRepW( wchar_t* str, wchar_t* strOut, DWORD nOut, wchar_t rep = '*' );


		//////-------------------------------------------------------------------------//
		//////------------------------ MULTIBYTE FUNCTION -------------------------------//
		//////_________________________________________________________________________//
		// Multibyte
		NMUTIL_API extern BADFILTER_ERROR		NMInitFilter( char* szAvatar );
		//- Dll Path ���� ( �ش� Path���� Bad Filter File�� �����ؾ� �Ѵ�. )
		NMUTIL_API extern void					NMSetFilterDllPath( char* szDllFullPath );
		//- Filter File ���� ( ��ο� �Բ� �־��־�� �Ѵ�. )
		NMUTIL_API extern void					NMSetFilterFile( char* szFilterFile );

		//- Bad Word�� ���� ���θ� Return
		NMUTIL_API extern BOOL					NMIsBad( char* str );
		//- Bad Word�� rep�� ��ü �ϴ� �Լ� ( strOut���� return )
		NMUTIL_API extern BOOL					NMIsBadToRep( char* str, char* strOut, DWORD nOut, char rep = '*' );

	}//namespace UTIL

}//namespace NMBASE
