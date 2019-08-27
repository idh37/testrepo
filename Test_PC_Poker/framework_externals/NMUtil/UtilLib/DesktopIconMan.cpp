l// DesktopIconMan.cpp: implementation of the CDesktopIconMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DesktopIconMan.h"
#include "ShortCut.h"
#include "Atlbase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace NMBASE
{
	namespace UTIL
	{
		//////////////////////////////////////////////////////////////////////
		// Construction/Destruction
		//////////////////////////////////////////////////////////////////////

		CDesktopIconMan::CDesktopIconMan()
		{
			m_hkey = NULL;
		}

		CDesktopIconMan::~CDesktopIconMan()
		{

		}


		BOOL CDesktopIconMan::CheckSetVersion(const char *pProgName,  int nSiteCode)
		{
			CRegKey Key;
			int ShortCutVersion = 0;
			int RegVersion = 0;
			CString strVersion;

			// 바로가기 아이콘의 버전을 읽어옴
			char strkey[500]={0,};
			::wsprintf(strkey, "Software\\Netmarble\\%s", pProgName);
			int rtn = Key.Open(HKEY_LOCAL_MACHINE, strkey); 

			if(rtn == ERROR_SUCCESS) 
			{
				char verstr[100]={0,};
				unsigned long vstrlen = 100;

				#if _MSC_VER >= 1300
					if(ERROR_SUCCESS == Key.QueryStringValue( _T("Version"), verstr, &vstrlen)) 
					{
						RegVersion = ::atoi(verstr);
					}
				#else	// VS 6.0 ++
					rtn = Key.QueryValue(verstr, _T("Version"), &vstrlen);
					if(rtn == ERROR_SUCCESS)
					{
						RegVersion = ::atoi(verstr);
					}
				#endif

				strVersion.Format(_T("ShortCutVersion_SC%02d"), nSiteCode);

				#if _MSC_VER >= 1300
					if(ERROR_SUCCESS == Key.QueryStringValue( strVersion, verstr, &vstrlen)) 
					{
						RegVersion = ::atoi(verstr);
					}
				#else	// VS 6.0 ++
					rtn = Key.QueryValue(verstr, strVersion, &vstrlen);
					if(rtn == ERROR_SUCCESS) 
					{
						ShortCutVersion = ::atoi(verstr);
					}
				#endif

						
			}

			if( ShortCutVersion != RegVersion && RegVersion != 0 )
			{
				// 레지스트리에 바로가기 아이콘의 버전을 설정(키가 없을땐 키를 생성)
				strVersion.Format("ShortCutVersion_SC%02d", nSiteCode);

				TCHAR verstr[100]={0,};
				wsprintf(verstr, "%d", RegVersion);

				#if _MSC_VER >= 1300
					Key.SetStringValue(strVersion.GetBuffer(), verstr);
				#else	// VS 6.0 ++
					Key.SetValue(verstr, strVersion.GetBuffer());		
				#endif


				return TRUE;
			}
			return FALSE;
		}

		BOOL CDesktopIconMan::MakeIcon(  CString url, CString ShortcutName, CString Desc, const char *pIconFilePath, int nIconIndex )
		{
			// URL
			LPCSTR pszURL = url;
			// 바로가기 이름
			LPCSTR pszlnkName = ShortcutName;
			// 바로가기 설명
			LPCSTR szDescription = Desc;

			char szFileName[MAX_PATH]={0,};
			if(pIconFilePath)
			{
				// 실행파일명을 얻음(실행파일의 아이콘을 사용하기 위함)			
				strncpy_s(szFileName, MAX_PATH, pIconFilePath, MAX_PATH-1);
			}
			else
			{
				GetModuleFileName(NULL, szFileName, MAX_PATH-1);
			}


			// 인터넷 바로가기 아이콘을 바탕화면에 생성
			//return 	CreateInternetShortCutToDesktopFolder(pszURL, pszlnkName, szDescription, szFileName, nIconIndex);

			NMBASE::UTIL::CreateShortCutToDesktopFolder(pszlnkName, NMBASE::UTIL::GetiExplorerPath().c_str(), pszURL, szDescription, szFileName, 0);

			//unicon7 : 몇달뒤 지울것~! 예전의 바로가기.url화일을 지우기 위해 임시로 만들어 놨다!(2017.4.11)
			char strkey[500] = {0,};
			NMBASE::UTIL::GetDeskTopFolder(strkey);
			strcat_s(strkey, 500, "\\");
			strcat_s(strkey, 500, pszlnkName);
			strcat_s(strkey, 500, ".url");
			if(PathFileExists(strkey))
			{
			    DeleteFile(strkey);
			}
			//unicon7 : 몇달뒤 지울것~! 여기까지~

			return TRUE;
		}

		void CDesktopIconMan::MakeIconFile(ENUM_SCUT_SITE nSite)
		{
			// 알아서 바꿔야 할 부분..
			std::string strURL = "";

			switch(nSite)
			{
			case SCS_NEWPOKER:
				strURL = "http://game1.netmarble.net/newpoker";
				break;
			case SCS_CHAMPIONSHIP:
				strURL = "http://game1.netmarble.net/event/agame/pokerChampionship_New/";
				break;
			default:
				ASSERT(FALSE);
				break;
			}

			Copy( "넷마블.url", "netmarble.ico", strURL.c_str() );
		}

		void CDesktopIconMan::Copy( LPCTSTR str, LPCTSTR icon, LPCTSTR url )
		{
			char PathBuf[400];
			ZeroMemory( PathBuf, sizeof(PathBuf) );

			FILE* destfp = NULL;
			CString dest;

			if( SHGetSpecialFolderPath( NULL, PathBuf, CSIDL_DESKTOPDIRECTORY, 0 ) != NOERROR ) 
				return;

			if( strlen(PathBuf) <= 0 )
				return;

			dest.Format( "%s\\%s", PathBuf, str );
			fopen_s(&destfp, dest.GetBuffer(), "r" );
			if( destfp != NULL )
				return;

			ZeroMemory( PathBuf, 400 );
			if( SHGetSpecialFolderPath( NULL, PathBuf, CSIDL_COMMON_DESKTOPDIRECTORY, 0 ) != NOERROR )
				return;

			if( strlen(PathBuf) <= 0 ) 
				return;

			dest.Format( "%s\\%s", PathBuf, str );
			fopen_s(&destfp, dest.GetBuffer(), "r" );
			if( destfp != NULL )
				return;

			dest.Format( "%s\\%s", PathBuf, str );
			fopen_s(&destfp, dest.GetBuffer(), "wt" );
			if( destfp == NULL ) 
				return;

			CString scut_str;
			GetCurrentDirectory( 400, PathBuf );
			scut_str.Format( "[InternetShortcut]\nURL=%s\nIconFile=%s\\%s\nIconIndex=0", url, PathBuf, icon );
			fprintf( destfp, "%s", (LPCTSTR)scut_str );
			fclose( destfp );
		}

	}//namespace UTIL

}//namespace NMBASE
