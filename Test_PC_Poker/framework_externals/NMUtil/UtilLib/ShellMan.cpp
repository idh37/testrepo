// ShellMan.cpp: implementation of the CShellMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#pragma warning(disable : 4192)
#pragma warning(disable : 4146)
#import <mshtml.tlb>  rename("TranslateAccelerator", "TranslateAcceleratorA") // Internet Explorer 5
#import <shdocvw.dll> rename("FindText", "FindTextA")

#include "SSOWebLib.h"
#include <afxinet.h>
#include <mshtml.h>
#include <atlconv.h>
#include "ShellMan.h"
#include <Tlhelp32.h>

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

		// 
		static SHDocVw::IShellWindowsPtr spSHWinds = 0; 

		CShellMan::CShellMan()
		{

			ZeroMemory(m_szAuth, sizeof(m_szAuth));
			ZeroMemory(m_szData, sizeof(m_szData));
			ZeroMemory(m_szCp, sizeof(m_szCp));
		}

		CShellMan::~CShellMan()
		{
			if(spSHWinds)
			{
				spSHWinds.Release();
			}
		}



		void CShellMan::ShowExplorer( const BOOL bShow, const char* pFindURL, BOOL bRefresh )
		{
			UNREFERENCED_PARAMETER( bRefresh );

			if( pFindURL == NULL )
			{
				return;
			}

			// Enumerates all top-level windows on the screen by passing the handle to each window,
			// in turn, to an application-defined callback function.
			// EnumWindows continues until the last top-level window is enumerated or the callback function returns FALSE
			EnumWindows(&CShellMan::EnumWindowsCloseNetmarbleIEProc, (LPARAM)bShow);
		}

		BOOL CALLBACK CShellMan::EnumWindowsCloseNetmarbleIEProc (HWND hwnd, LPARAM lParam)
		{
			TCHAR szCaption[255] = {0, };
			TCHAR szCompareString[255] = {0, };
			char *pStartPos = NULL;

			DWORD dwPID = 0;
			HANDLE hProcess = NULL;


			::GetWindowThreadProcessId(hwnd, &dwPID);

			// GetModuleBaseName() 이라는 함수를 이용해서 PID로 프로세스 이름을 얻어올 수 있다던데 컴파일 에러난다...;;
			// 그래서 IsStringEqualToProcessModuleName()로 프로세스에 올라와있는 모든 모듈의 이름을 검사한다.
			if(IsStringEqualToProcessModuleName( dwPID, "iexplore.exe"))
			{
				BOOL bShow = (BOOL)lParam;

				// 넷마블 웹페이지는 caption에 항상 "넷마블" 스트링이 포함되어 있다
				// "넷마블" 스트링으로 검사해서 해당되는 윈도우에 적절한(?) 액션을 한다.
				strcpy_s(szCompareString, 255, "넷마블");
				GetWindowText(hwnd, szCaption, 255);

				pStartPos = strstr(szCaption, szCompareString);

				if(pStartPos != NULL)
				{
					if( bShow )
					{
						//ShowWindow 함수를 이용할 경우 win7에서 이상 동작을 해서 WM_SYSCOMMAND 메시지로 교체
						//::ShowWindow( hwnd, SW_RESTORE );
						::SendMessage( hwnd, WM_SYSCOMMAND, (WPARAM)SC_RESTORE, NULL);

						// 페이지를 새로고침 하기 위해 아래의 메시지들을 보냄
						// 이게 모든 윈도우 환경에서 동작하는지는 QA 검수를 통해 확인하자.
						PostMessage(hwnd, WM_KEYDOWN, VK_F5, 0 );
						PostMessage(hwnd, WM_KEYUP, VK_F5, 0 );
					}
					else
					{
						//ShowWindow 함수를 이용할 경우 win7에서 이상 동작을 해서 WM_SYSCOMMAND 메시지로 교체
						//::ShowWindow( hwnd, SW_SHOWMINIMIZED );
						::SendMessage( hwnd, WM_SYSCOMMAND, (WPARAM)SC_MINIMIZE, NULL);
					}
				}
			}

			return TRUE;
		}

		bool CShellMan::IsStringEqualToProcessModuleName( DWORD dwPID, CString strProcessName)
		{
			HANDLE hModuleSnap = NULL;
			MODULEENTRY32 me32 = {0, };
			hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);

			if(hModuleSnap == (HANDLE)-1)
			{
				return false;
			}

			me32.dwSize = sizeof(MODULEENTRY32);

			// 해달 프로세스의 모듈리스트를 루프로 돌려서 프로세스 이름과 동일하면 true를 리턴한다.
			if(Module32First (hModuleSnap, &me32))
			{
				do 
				{
					if(me32.szModule == strProcessName)
					{
						CloseHandle( hModuleSnap );

						return true;
					}
				}
				while (Module32Next( hModuleSnap, &me32));

			}

			CloseHandle (hModuleSnap);

			return false;
		}


		void CShellMan::ShowRefresh(const char* pFindURL )
		{
			if( pFindURL == NULL )
			{
				return;	
			}

			//	if( spSHWinds == NULL ) return;
			USES_CONVERSION;

			//	SHDocVw::IShellWindowsPtr spSHWinds = 0; 

			HRESULT hr = spSHWinds.CreateInstance (__uuidof(SHDocVw::ShellWindows)); 
			if( FAILED(hr) ) 
			{
				return;
			}

			long nCount = spSHWinds->GetCount();

			CString str = "";

			// Enum all Shell Windows and list them
			BSTR pURL = NULL;
			//CString str = _T("");
			HWND hwnd = NULL;
			IDispatchPtr spDisp = NULL;
			for (long i = 0; i < nCount; ++i)
			{
				if (spSHWinds == NULL)
					break;

				_variant_t va(i, VT_I4);
				spDisp = spSHWinds->Item( va );

				if(NULL == spDisp)
					continue;

				IWebBrowser2 * pWebBrowser = NULL;
				hr = spDisp.QueryInterface (IID_IWebBrowser2, &pWebBrowser);
				//pWebBrowser->put_Visible (VARIANT_TRUE);

				if(pWebBrowser != NULL)
				{
					pURL = NULL;
					pWebBrowser->get_LocationURL( &pURL );
					if( pURL == NULL ) 
					{
						continue;
					}

					str = W2A( pURL );

					if (str.IsEmpty())
						continue;

					if( str.Find( pFindURL ) >= 0 )
					{
						pWebBrowser->Refresh();						
					}

					SysFreeString( pURL );
				}

				if( pWebBrowser )
				{
					pWebBrowser->Release();
				}
			}
		}


		void CShellMan::SetCookie(const char *szAuth, const char *szData, const char *szCp)
		{
			strncpy(m_szAuth, szAuth, sizeof(m_szAuth)-1);
			strncpy(m_szData, szData, sizeof(m_szData)-1);
			strncpy(m_szCp, szCp, sizeof(m_szCp)-1);

			return;
		}


		HWND CShellMan::OpenNewBrowser(const char *pURL, BROWSEROPTION *pOpiton)
		{
			CoInitialize(NULL);			

			IWebBrowser2 *pWebBrowser = NULL;
			HRESULT hr = 0;


			hr = ::CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (void**)&pWebBrowser);   

			if(pWebBrowser == NULL) 
			{
				TRACE("Failed CoCreateInstance \n");		
				return NULL;
			}

			CString strAddress;

			// Navigate() 인자 초기화
			VARIANT vtFlags, vtTarget, vtPostData, vtHeader;
			::VariantInit(&vtFlags);
			::VariantInit(&vtTarget);
			::VariantInit(&vtPostData);
			::VariantInit(&vtHeader);

			// 브라우저의 크기 및 툴바 옵션 세팅
			if( pOpiton ) 
			{ 
				pWebBrowser->put_Height(pOpiton->nHeight); 
				pWebBrowser->put_Width(pOpiton->nWidth); 
				pWebBrowser->put_AddressBar((pOpiton->bAddressBar == TRUE) ? 1 : 0); 
				pWebBrowser->put_MenuBar((pOpiton->bMenuBar == TRUE) ? 1 : 0); 
				pWebBrowser->put_Resizable((pOpiton->bResizable == TRUE) ? 1 : 0); 
				pWebBrowser->put_StatusBar((pOpiton->bStatusBar == TRUE) ? 1 : 0); 
				pWebBrowser->put_ToolBar(pOpiton->bToolBar); 
				//pWebBrowser->put_AddressBar()
			}

			// 웹브라우저 띄우기	
			pWebBrowser->put_Visible(VARIANT_TRUE);

			// 사이트를 연다.
			strAddress.Format("http://login.netmarble.net/game/cookie/actcookie.asp");
			hr = pWebBrowser->Navigate(strAddress.AllocSysString(), &vtFlags, &vtTarget, &vtPostData, &vtHeader);

			READYSTATE rt=READYSTATE_UNINITIALIZED;
			DWORD m_LoadTime = GetTickCount();
			DWORD m_ElapsedTime= m_LoadTime;
			const int TIMEOUT_COMPLETE = 5000;	// 5 sec timout
			while(rt!=READYSTATE_COMPLETE && (m_ElapsedTime-m_LoadTime)<TIMEOUT_COMPLETE)
			{
				pWebBrowser->get_ReadyState(&rt);
				m_ElapsedTime= GetTickCount();
				Sleep(20);
			}

			IDispatchPtr spDisp;
			if(pWebBrowser->get_Document(&spDisp) == S_OK && spDisp!= 0 )
			{
				MSHTML::IHTMLDocument2Ptr spHtmlDocument(spDisp);

				CString strCookie;
				BSTR bstr;

				if (GetSSOWebCookieString (m_szAuth,1024 ,m_szData, 1024 ,m_szCp,1024))
				{  
					if(strlen(m_szAuth)>0)
					{
						strCookie=m_szAuth;
						bstr=strCookie.AllocSysString();
						spHtmlDocument->put_cookie(bstr);
					}

					if(strlen(m_szData)>0)
					{
						strCookie=m_szData;					
						bstr=strCookie.AllocSysString();
						spHtmlDocument->put_cookie(bstr);
					}

					if(strlen(m_szCp)>0)
					{
						strCookie=m_szCp;
						bstr=strCookie.AllocSysString();
						spHtmlDocument->put_cookie(bstr);
					}

				}
				//old

				//if(strlen(m_szAuth)>0)
				//{
				//	strCookie=m_szAuth;
				//	bstr=strCookie.AllocSysString();
				//	spHtmlDocument->put_cookie(bstr);
				//}

				//if(strlen(m_szData)>0)
				//{
				//	strCookie=m_szData;					
				//	bstr=strCookie.AllocSysString();
				//	spHtmlDocument->put_cookie(bstr);
				//}

				//if(strlen(m_szCp)>0)
				//{
				//	strCookie=m_szCp;
				//	bstr=strCookie.AllocSysString();
				//	spHtmlDocument->put_cookie(bstr);
				//}
			}
			else
			{
				TRACE("Failed to put cookies into browser");
				return NULL;
			}

			strAddress.Format(pURL);
			hr = pWebBrowser->Navigate(strAddress.AllocSysString(), &vtFlags, &vtTarget, &vtPostData, &vtHeader);   

			long hwndd = 0;
			pWebBrowser->get_HWND(&hwndd);

			::SetWindowPos((HWND)hwndd, HWND_TOP, 0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
			pWebBrowser->Release();

			CoUninitialize();	

			return (HWND)hwndd;
		}

		void CShellMan::OpenNewBrowser_Refresh()
		{
			CoInitialize(NULL);			
			IWebBrowser2 *pWebBrowser = NULL;
			HRESULT hr = 0;
			hr = ::CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (void**)&pWebBrowser);   
			if(pWebBrowser == NULL){
				TRACE("Failed CoCreateInstance \n");
				CoUninitialize();	
				return;
			}
			CString strAddress;
			// Navigate() 인자 초기화
			VARIANT vtFlags, vtTarget, vtPostData, vtHeader;
			::VariantInit(&vtFlags);
			::VariantInit(&vtTarget);
			::VariantInit(&vtPostData);
			::VariantInit(&vtHeader);
			// 웹브라우저 띄우기	
			pWebBrowser->put_Visible(VARIANT_FALSE);
			// 사이트를 연다.
			strAddress.Format("http://login.netmarble.net/game/cookie/refresh.asp");
			hr = pWebBrowser->Navigate(strAddress.AllocSysString(), &vtFlags, &vtTarget, &vtPostData, &vtHeader);
			pWebBrowser->Release();
			CoUninitialize();
		}

		BOOL CShellMan::OpenNetmarbleWebpage(LPCTSTR szMyUniqNo, LPCTSTR szTargetURL)
		{
			if( NULL == szTargetURL )
				return FALSE;

			SHDocVw::IShellWindowsPtr spWins = NULL;
			HRESULT hr;
			BOOL bSucc = FALSE;

			CoInitialize(NULL);

			hr = spWins.CreateInstance (__uuidof(SHDocVw::ShellWindows));

			if( SUCCEEDED(hr) )
			{
				long nCount = spWins->GetCount();

				if( 0 < nCount )
				{
					IDispatchPtr spDisp = NULL;
					IWebBrowser2Ptr spWeb = NULL;
					IWebBrowser2Ptr spFindWeb = NULL;
					_variant_t vtIdx;

					for (long i = 0; i < nCount ; i++)
					{
						vtIdx = i;

						TRY
						{
							spDisp = spWins->Item( vtIdx );
						}
						CATCH_ALL(e)
						{
							spDisp = NULL;
						}
						END_CATCH_ALL


							if( NULL != spDisp )
							{
								hr = spDisp.QueryInterface (IID_IWebBrowser2, &spWeb);

								if( SUCCEEDED(hr) && NULL != spWeb )
								{
									VARIANT_BOOL bShowAddressBar;

									hr = spWeb->get_AddressBar(&bShowAddressBar);

									// 주소 표시줄이 있는넘만 조사한다!
									if( SUCCEEDED(hr) && VARIANT_TRUE == bShowAddressBar  )
									{
										BSTR bstr;

										hr = spWeb->get_LocationURL(&bstr);

										if ( SUCCEEDED(hr) )
										{
											_bstr_t bstrURL(bstr);
											CString strServer, strObject;
											INTERNET_PORT port;
											DWORD dwServiceType;

											SysFreeString(bstr);

											// 인터넷 서비스이고~
											if ( AfxParseURL(bstrURL, dwServiceType, strServer, strObject, port)
												&& ( AFX_INET_SERVICE_HTTP == dwServiceType || AFX_INET_SERVICE_HTTPS == dwServiceType ) )
											{
												TRACE(_T("URL %s %s\n"), strServer, strObject);
												LPCTSTR szFindDomain = _T(".netmarble.net");

												// 넷마블 도메인이 포함되어있으면~ 찾았다!
												if( strServer.Find( szFindDomain ) >= 0 )
												{
													// 저장하고 루프를 종료한다.
													spFindWeb = spWeb;
													break;
												}
											}
										}
									}
								} // if( SUCCEEDED(hr) )
							} //if( NULL != spDisp )

					} // for (long i = 0; i < nCount && NULL == spWebMatchLogon; i++)

					// 찾은 웹 콘트롤을 네비게이트 한다.
					if ( NULL != spFindWeb )
					{
						//창을 연다!
						COleVariant varURL, varFlag, varTargetFrame, varPostdata, varHeaders;
						varURL = szTargetURL;
						varFlag = (long)(0);
						hr = spFindWeb->Navigate2(varURL, varFlag, varTargetFrame, varPostdata, varHeaders);

						bSucc = SUCCEEDED(hr);

						// 변경된 웹페이지가 화면 앞으로 나오도록 처리
						if ( bSucc )
						{
							HWND hWnd;

							hr = spFindWeb->get_HWND((long *)&hWnd);

							if( SUCCEEDED(hr) && NULL != hWnd )
							{
								::SetForegroundWindow(hWnd);
							}
						}
					}

				}
			}

			if ( !bSucc )
			{
#ifdef _UNICODE
				CStringA strTargetURL(szTargetURL);
				SSOWEBLib_OpenInternetExplorer(strTargetURL);
#else
				SSOWEBLib_OpenInternetExplorer(szTargetURL);
#endif
			}

			CoUninitialize();

			return bSucc;
		}

		int CShellMan::EnumProcessCnt(char* pProcess)
		{
			char cProcess[_MAX_PATH]={0,};
			if(NULL==pProcess){
				char szDrive[_MAX_DRIVE]={0,};
				char szDir[MAX_PATH]={0,};
				char szFileName[_MAX_FNAME]={0,};
				char szExt[_MAX_EXT]={0,};
				::GetModuleFileName(NULL, cProcess, _MAX_PATH);
				_splitpath(cProcess, szDrive, szDir, szFileName, szExt);
				wsprintf(cProcess, "%s%s", szFileName, szExt);
			}
			else{
				strcpy(cProcess, pProcess);
			}
			HANDLE hSnap=0;
			PROCESSENTRY32 pe={0,};
			int nProcessNumber=0;
			hSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if(hSnap==(HANDLE)-1){
				return -1;
			}
			pe.dwSize=sizeof(PROCESSENTRY32);
			if(TRUE==Process32First(hSnap, &pe)){
				do{
					//sprintf(pe.szExeFile, "%s", pTemp);
					if(NULL!=strstr(pe.szExeFile, cProcess)){
						nProcessNumber++;
					}
				}while(TRUE==Process32Next(hSnap, &pe));
			}
			CloseHandle(hSnap);
			return nProcessNumber;
		}

		CFileMapping* CFileMapping::GetInstance(void)
		{
			static CFileMapping cFileMapping;
			return &cFileMapping;
		}
		void CFileMapping::InitFileMapping(void)
		{
			m_hfMemMap=NULL;
		}
		void CFileMapping::FreeFileMapping(void)
		{
			if(NULL!=m_hfMemMap){
				CloseHandle(m_hfMemMap);
				m_hfMemMap=NULL;
			}
		}
		bool CFileMapping::MappingWrite(std::string sName, std::string sMap)
		{
			if(NULL!=m_hfMemMap){
				CloseHandle(m_hfMemMap);
			}
			m_hfMemMap = CreateFileMapping((HANDLE) - 1, NULL, PAGE_READWRITE, 0,  1024, sName.c_str());
			if (m_hfMemMap == NULL){
				return false;
			}
			if (GetLastError() == ERROR_ALREADY_EXISTS){
				//printf("이미 매핑 오브젝트가 있어요.\n");		
			}
			LPSTR lpStr = (LPSTR)MapViewOfFile(m_hfMemMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if (lpStr == NULL){
				return false;
			}
			strcpy(lpStr, sMap.c_str());
			UnmapViewOfFile(lpStr);
			return true;
		}
		bool CFileMapping::MappingRead(std::string sName, std::string &sMap)
		{
			HANDLE hfMapRead;
			hfMapRead = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, sName.c_str());
			LPSTR lpStr=(LPSTR)MapViewOfFile(hfMapRead, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if (lpStr == NULL){
				//printf("읽기 실패\n");
				CloseHandle(hfMapRead);
				return false;
			}
			sMap=std::string(lpStr);
			if (UnmapViewOfFile(lpStr) == 0){
				//printf("읽기용 어드래스 매핑 실패\n");
			} 
			else{
				//printf("읽기용 어드래스 매핑 성공\n");
				lpStr = NULL;
			}
			CloseHandle(hfMapRead);
			return true;
		}

	}//namespace UTIL

}//namespace NMBASE

