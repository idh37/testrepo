// SysUtil.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BadFilter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
 
namespace NMBASE
{
	namespace UTIL
	{
		//- Common Function !!!
		//- Filter Level 변경 ( 1번은 기존방식, 2번은 추가된 방식(권장) )
		void NMSetFilterLevel( int nLevel )
		{
			SetFilterLevel(nLevel);
		}

		//- Filter 기능 사용여부
		void NMSetFilterEnable( BOOL bEnable )
		{
			SetFilterEnable(bEnable);
		}

		BOOL NMIsFilterEnable()
		{
			return IsFilterEnable();
		}

		//////-------------------------------------------------------------------------//
		//////------------------------ UNICODE FUNCTION -------------------------------//
		//////_________________________________________________________________________//
		// [9/30/2008 Bad Filter]
		//- BadFilter 초기화 함수
		//- shared라면, szDllName을  BadFilterSHD.dll로 할것
		//- Static이라면, szDllName을  BadFilterSTD.dll로 할것
		BADFILTER_ERROR	NMInitFilterW( wchar_t* szAvatar )
		{
			return InitFilterW(szAvatar);
		}

		//extern "C"	BADFILTER_ERROR		InitFilter( wchar_t*  szAvatar );
		//- Dll Path 변경 ( 해당 Path에는 Bad Filter File도 존재해야 한다. )
		void NMSetFilterDllPathW( wchar_t* szDllFullPath )
		{
			SetFilterDllPathW(szDllFullPath);
		}

		//- Filter File 변경 ( 경로와 함께 넣어주어야 한다. )
		void NMSetFilterFileW( wchar_t* szFilterFile )
		{
			SetFilterFileW(szFilterFile);
		}

		//- Bad Word의 존재 여부를 Return
		BOOL NMIsBadW( wchar_t* str )
		{
			return IsBadW(str);
		}

		//- Bad Word를 rep로 대체 하는 함수 ( strOut으로 return )
		BOOL NMIsBadToRepW( wchar_t* str, wchar_t* strOut, DWORD nOut, wchar_t rep/* = '*' */)
		{
			return IsBadToRepW(str, strOut, nOut, rep);
		}


		//////-------------------------------------------------------------------------//
		//////------------------------ MULTIBYTE FUNCTION -------------------------------//
		//////_________________________________________________________________________//
		// Multibyte
		BADFILTER_ERROR	NMInitFilter( char* szAvatar )
		{
			return InitFilter(szAvatar);
		}

		//- Dll Path 변경 ( 해당 Path에는 Bad Filter File도 존재해야 한다. )
		void NMSetFilterDllPath( char* szDllFullPath )
		{
			SetFilterDllPath(szDllFullPath);
		}

		//- Filter File 변경 ( 경로와 함께 넣어주어야 한다. )
		void NMSetFilterFile( char* szFilterFile )
		{
			SetFilterFile(szFilterFile);
		}

		//- Bad Word의 존재 여부를 Return
		BOOL NMIsBad( char* str )
		{
			return IsBad(str);
		}

		//- Bad Word를 rep로 대체 하는 함수 ( strOut으로 return )
		BOOL NMIsBadToRep( char* str, char* strOut, DWORD nOut, char rep/* = '*' */)
		{
			return IsBadToRep(str, strOut, nOut, rep);
		}

	}//namespace UTIL

}//namespace NMBASE
