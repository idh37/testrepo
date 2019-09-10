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
		//- Filter Level ���� ( 1���� �������, 2���� �߰��� ���(����) )
		void NMSetFilterLevel( int nLevel )
		{
			SetFilterLevel(nLevel);
		}

		//- Filter ��� ��뿩��
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
		//- BadFilter �ʱ�ȭ �Լ�
		//- shared���, szDllName��  BadFilterSHD.dll�� �Ұ�
		//- Static�̶��, szDllName��  BadFilterSTD.dll�� �Ұ�
		BADFILTER_ERROR	NMInitFilterW( wchar_t* szAvatar )
		{
			return InitFilterW(szAvatar);
		}

		//extern "C"	BADFILTER_ERROR		InitFilter( wchar_t*  szAvatar );
		//- Dll Path ���� ( �ش� Path���� Bad Filter File�� �����ؾ� �Ѵ�. )
		void NMSetFilterDllPathW( wchar_t* szDllFullPath )
		{
			SetFilterDllPathW(szDllFullPath);
		}

		//- Filter File ���� ( ��ο� �Բ� �־��־�� �Ѵ�. )
		void NMSetFilterFileW( wchar_t* szFilterFile )
		{
			SetFilterFileW(szFilterFile);
		}

		//- Bad Word�� ���� ���θ� Return
		BOOL NMIsBadW( wchar_t* str )
		{
			return IsBadW(str);
		}

		//- Bad Word�� rep�� ��ü �ϴ� �Լ� ( strOut���� return )
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

		//- Dll Path ���� ( �ش� Path���� Bad Filter File�� �����ؾ� �Ѵ�. )
		void NMSetFilterDllPath( char* szDllFullPath )
		{
			SetFilterDllPath(szDllFullPath);
		}

		//- Filter File ���� ( ��ο� �Բ� �־��־�� �Ѵ�. )
		void NMSetFilterFile( char* szFilterFile )
		{
			SetFilterFile(szFilterFile);
		}

		//- Bad Word�� ���� ���θ� Return
		BOOL NMIsBad( char* str )
		{
			return IsBad(str);
		}

		//- Bad Word�� rep�� ��ü �ϴ� �Լ� ( strOut���� return )
		BOOL NMIsBadToRep( char* str, char* strOut, DWORD nOut, char rep/* = '*' */)
		{
			return IsBadToRep(str, strOut, nOut, rep);
		}

	}//namespace UTIL

}//namespace NMBASE
