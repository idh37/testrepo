// ShellMan.h: interface for the CShellMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHELLMAN_H__8DD27A39_C287_43BE_80FB_0C92E4A00500__INCLUDED_)
#define AFX_SHELLMAN_H__8DD27A39_C287_43BE_80FB_0C92E4A00500__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UtilDefine.h"
#include <string>

namespace NMBASE
{
	namespace UTIL
	{

		typedef struct
		{	
			int nHeight;
			int nWidth;
			BOOL bAddressBar;
			BOOL bMenuBar;
			BOOL bResizable;
			BOOL bStatusBar;
			BOOL bToolBar;
		}BROWSEROPTION,*PBROWSEROPTION;

		class NMUTIL_CLASS CShellMan  
		{
		public:
			CShellMan();
			~CShellMan();

			char m_szAuth[1024];
			char m_szData[1024];
			char m_szCp[1024];

			//bool ConnectToShell( CDialog* pDlg );
			void ShowExplorer( const BOOL bShow, const char* pFindURL = "netmarble", BOOL bRefresh = TRUE );
			void ShowRefresh(const char* pURL = "netmarble");
			//void MakeExplorer();

			void SetCookie(const char *szAuth, const char *szData, const char *szCp);
			HWND OpenNewBrowser(const char *pURL, BROWSEROPTION *pOpiton=NULL);
			void OpenNewBrowser_Refresh();

			static BOOL CALLBACK EnumWindowsCloseNetmarbleIEProc (HWND hwnd, LPARAM lParam);
			static bool IsStringEqualToProcessModuleName( DWORD dwPID, CString strProcessName);

			BOOL OpenNetmarbleWebpage(LPCTSTR szMyUniqNo, LPCTSTR szTargetURL);


			//yoo
			int EnumProcessCnt(char* pProcess);
		protected:

			//CCmdTarget m_target;

		};

		//yoo
		class NMUTIL_CLASS CFileMapping 
		{
		public:
			HANDLE m_hfMemMap;
		public:
			static CFileMapping* GetInstance(void);
		public:
			CFileMapping(void){ InitFileMapping(); }
			virtual ~CFileMapping(void){ FreeFileMapping(); }
			void InitFileMapping(void);
			void FreeFileMapping(void);
		public:
			bool MappingWrite(std::string sName, std::string sMap);
			bool MappingRead(std::string sName, std::string &sMap);
		};

	}//namespace UTIL

}//namespace NMBASE


#endif // !defined(AFX_SHELLMAN_H__8DD27A39_C287_43BE_80FB_0C92E4A00500__INCLUDED_)