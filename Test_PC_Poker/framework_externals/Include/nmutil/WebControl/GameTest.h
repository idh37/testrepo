// GameTest.h: interface for the CGameTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMETEST_H__0ED283BE_6EB3_4FF4_AAAB_7C3B17203917__INCLUDED_)
#define AFX_GAMETEST_H__0ED283BE_6EB3_4FF4_AAAB_7C3B17203917__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../UtilDefine.h"

namespace NMBASE
{
	namespace UTIL
	{

		enum URL_CONVERT
		{
			URL_CONVERT_NONE,
			URL_CONVERT_STAGE,
			URL_CONVERT_DEV,	
		};

		extern URL_CONVERT g_URLConverType;
		//void Test_mymemcpy(void *lpDest, void* lpSrc, int len);
		NMUTIL_CLASS char * __fastcall Test_URLConvert(const char *pURL_Original);

		NMUTIL_CLASS void Test_Init();

	}//namespace UTIL

}	//namespace NMBASE


#endif // !defined(AFX_GAMETEST_H__0ED283BE_6EB3_4FF4_AAAB_7C3B17203917__INCLUDED_)
