// GameTest.cpp: implementation of the CGameTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./webcontrol/GameTest.h"
#include "SSOWebLib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace NMBASE
{
	namespace UTIL
	{

		extern URL_CONVERT g_URLConverType=URL_CONVERT_NONE;

		char * __fastcall Test_URLConvert(const char *pURL_Original)
		{	
			if(g_URLConverType == URL_CONVERT_NONE)
				return (char*)pURL_Original;

			CString strURL = pURL_Original;
			static char pURL_Converted[4096]={0,};

			if(g_URLConverType == URL_CONVERT_DEV)
			{
				//캐쉬 URL은 예외처리
				if(strURL.Find("cash.netmarble.net")>0 || strURL.Find("cashtest.netmarble.net") >0)
				{
					strURL.Replace("cash.netmarble.net", "cashtest.netmarble.net");			
				}		
				else
				{
					strURL.Replace("-dev.netmarble.net", ".netmarble.net");	
					strURL.Replace(".netmarble.net", "-dev.netmarble.net");	
				}
			}
			else if(g_URLConverType == URL_CONVERT_STAGE)
			{
				strURL.Replace("-dev.netmarble.net", ".netmarble.net");	
			}

			strcpy_s(pURL_Converted, 4096, strURL);
			return pURL_Converted;
		}

		bool __fastcall MyFuncHookNavigate(const char* lpszURL, CSSOWEBURLString& strURL_Out)
		{
			// 잘못된 URL 값이면 종료.
			if( lpszURL == NULL || *lpszURL == '\0' ) 
				return false;

			if(g_URLConverType == URL_CONVERT_NONE)
				return false;

			CString strURL = Test_URLConvert(lpszURL);	
			strURL_Out.Assign(strURL);

			return true;
		}



		void Test_Init()
		{

			g_URLConverType = (AfxMessageBox("DEV로 연결할까요?(YES) Stage로 연결할까요?(NO)",  MB_YESNO)==IDYES)?URL_CONVERT_DEV:URL_CONVERT_STAGE;	

			//SSO 웹브라우저 URL자동변경(데브/스테이지) 콜백함수 설정
			SSOWebLib_SetHookFuncForNavigate(MyFuncHookNavigate);
		}

	}//namespace UTIL
}//namespace NMBASE
