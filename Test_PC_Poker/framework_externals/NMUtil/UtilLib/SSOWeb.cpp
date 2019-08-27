// SysUtil.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SSOWeb.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace NMBASE
{
	namespace UTIL
	{

		CNMSSOWEBURLString::CNMSSOWEBURLString(CSSOWEBURLString *pURL/* = NULL*/)
		{
			m_pURL = pURL;
		}

		CNMSSOWEBURLString::~CNMSSOWEBURLString()
		{
		}

		/**
		@brif - 저장된 문자열 포인터를 가져온다.
		@returns 저장된 문자열 포인터를 반환한다.
		*/
		LPCSTR CNMSSOWEBURLString::GetData()
		{
			return m_pURL->GetData();
		}

		/**
		@brif - 주어진 문자열을 저장한다.
		@param[in] szData - 해당 클래스에 저장할 문자열.
		*/
		void CNMSSOWEBURLString::Assign(LPCSTR szData)
		{
			m_pURL->Assign(szData);
		}

		/**
		@brif - 문자열 데이터를 Clear한다.
		*/
		void CNMSSOWEBURLString::Clear()
		{
			m_pURL->Clear();
		}

		//////////////////////////////////////////////////////////////////////
		/*
		@defgroup GROUPEAPIFUNC API 함수
		@{
		*/
		/*
		@brif - 서비스 사이트에 따른 쿠키 이름을 변경하기 위하여 호출하는 함수.<br>
		넷마블인 경우는 호출할 필요가 없음.
		@note InitSSOWebBrowser() 이전에 호출되어야 함.
		@param[in] strSiteName - 다음게임인 경우 "daum", 하나포스게임인 경우 "hanafos"를 넘김.
		*/
		void NMPresetSSOWebBrowserSetSiteName(LPCSTR strSiteName)
		{
			PresetSSOWebBrowserSetSiteName(strSiteName);
		}

		/*
		@brif - SSO 웹브라우져 DLL이 위치한 경로를 수동으로 설정. 호출하지 않을 경우 공통 폴더의 SSO 웹브라우져 DLL을 참조.
		@note InitSSOWebBrowser() 이전에 호출되어야 함
		@param[in] strFolder - SSO 웹브라우져 DLL이 위치한 경로.
		*/
		void NMPresetSSOWebBrowserBaseFolder(LPCSTR strFolder)
		{
			PresetSSOWebBrowserBaseFolder(strFolder);
		}

		/*
		@brif - SSO 웹브라우져 DLL 파일의 절대 경로(파일명 포함)를 설정한다.
		@param[in] szDLLFullFileName - SSO 웹브라우져 DLL 파일의 절대 경로(파일명 포함)
		*/
		//void NMSetSSOWebBrowserDLLFileName(LPCSTR szDLLFullFileName);
		//{
		//	SetSSOWebBrowserDLLFileName(szDLLFullFileName);
		//}

		/*
		@brif - SSO 웹브라우져 모듈 초기화 함수
		@returns 해당 함수가 성공 하였을 경우 TRUE를 반환하며, 실패하였을 경우 FALSE를 반환한다.<br>
		실패시 보다 세부적인 오류코드를 얻으려면, SSOWebLib_GetLastError() 함수의 반환값을 참조한다.
		@param[in] hWndMain - 메인 윈도우의 핸들 값.
		@param[in] strAuthCookie - 인증 웹쿠키 값.
		@param[in] strDataCookie - 데이터 웹쿠키 값.
		@param[in] strCpCookie - CP 웹쿠키값.
		@param[in] bHook - 웹 콘트롤에서 발생하는 WindowMsg를 후킹하여 키보드 입력 정상 처리 할지 여부.
		@section SECEXAMPLE 관련 예제
		예제 - @ref SECTIONINIT "SSO 웹브라우져 모듈 초기화" 참조
		*/
		BOOL NMInitSSOWebBrowser(HWND hWndMain, LPCSTR strAuthCookie, LPCSTR strDataCookie, LPCSTR strCpCookie, BOOL bHook/* = TRUE*/)
		{
			return InitSSOWebBrowser(hWndMain, strAuthCookie, strDataCookie, strCpCookie, bHook);
		}

		/*
		@brif - SSO 웹브라우져 모듈 초기화 함수. (테스트용)<br>SSO 웹브라우져 DLL의 경로를 입력 받는 부분외에 InitSSOWebBrowser()함수와 다른 점은 없다.
		@returns 해당 함수가 성공 하였을 경우 TRUE를 반환하며, 실패하였을 경우 FALSE를 반환한다.<br>
		실패시 보다 세부적인 오류코드를 얻으려면, SSOWebLib_GetLastError() 함수의 반환값을 참조한다.
		@param[in] strBaseDir - SSO 웹브라우져 DLL이 위치한 경로.
		@param[in] hWndMain - 메인 윈도우의 핸들값.
		@param[in] strAuthCookie - 인증 웹쿠키 값.
		@param[in] strDataCookie - 데이터 웹쿠키 값.
		@param[in] strCpCookie - CP 웹쿠키값.
		*/
		BOOL NMInitSSOWebBrowser2(LPCSTR strBaseDir, HWND hWndMain, LPCSTR strAuthCookie, LPCSTR strDataCookie, LPCSTR strCpCookie)
		{
			return InitSSOWebBrowser2(strBaseDir, hWndMain, strAuthCookie, strDataCookie, strCpCookie);
		}

		/*
		@brif - SSO 웹브라우져 모듈 소멸 함수
		@note 반드시 메인 윈도우가 파괴되기 전에 호출해야한다.
		@returns 해당 함수가 성공 하였을 경우 TRUE를 반환하며, 실패하였을 경우 FALSE를 반환한다.<br>
		실패시 보다 세부적인 오류코드를 얻으려면, SSOWebLib_GetLastError() 함수의 반환값을 참조한다.
		@section SECEXAMPLE 관련 예제
		예제 - @ref SECTIONUNINIT "SSO 웹브라우져 모듈 소멸" 참조
		*/
		BOOL NMUnInitSSOWebBrowser()
		{
			return UnInitSSOWebBrowser();
		}

		/*
		@brif - SSO 웹브라우져가 처리해야 할 메시지를 후킹하여 처리할지 설정.
		@returns 가장 최근에 설정된 SSO 웹브라우져 오류값을 반환한다. 반환의 내용은 @ref ERROR_SSOWEB 를 참조한다.
		@param[in] bSetHook - 이 값이 True이면 후킹하여 메시지를 처리하고, False이면 후킹을 중단한다.
		*/
		BOOL NMSetHOOKPreTranslateMessage(BOOL bSetHook)
		{
			return SetHOOKPreTranslateMessage(bSetHook);
		}

		/*
		@brif - SSO 웹브라우져가 처리해야 할 메시지 처리(Tab, enter키 미작동 문제 해결) - TRUE리턴 시 처리 완료 됨
		@note @ref InitSSOWebBrowser API 호출시에 SSO 웹브라우져가 처리해야 할 메시지를 후킹 처리하였다면 호출 필요 없음.
		@returns 해당 함수가 성공 하였을 경우 TRUE를 반환하며, 실패하였을 경우 FALSE를 반환한다.<br>
		실패시 보다 세부적인 오류코드를 얻으려면, SSOWebLib_GetLastError() 함수의 반환값을 참조한다.
		@param[in] pMsg - SSO 웹브라우져가 처리해야 할 메시지 구조체.
		*/
		BOOL NMPreTranslateForSSOWebBrowser(LPMSG pMsg)
		{
			return PreTranslateForSSOWebBrowser(pMsg);
		}

		/*
		@brif - 쿠키 활성화 페이지 URL을 얻는 함수
		@returns 쿠키 활성화 페이지 URL을 반환한다.
		*/
		LPCSTR NMGetActCookieURL()
		{
			return GetActCookieURL();
		}

		/*
		@brif - 쿠키 리프레쉬 페이지 URL을 얻는 함수
		@returns 쿠키 리프레쉬 페이지 URL을 반환한다. 
		*/
		LPCSTR NMGetRefreshCookieURL()
		{
			return GetRefreshCookieURL();
		}

		/*
		@brif - 쿠키 설정용 도메인을 얻는 함수
		@returns 쿠키 설정용 도메인을 반환한다.
		*/
		LPCSTR NMGetCookieDomain()
		{
			return GetCookieDomain();
		}

		/*
		@brif - SSO 웹브라우져로부터 쿠키를 얻는 함수<br>
		내부적으로 변환되기 때문에 얻은 그대로 put_cookie하면 된다.
		@returns 해당 함수가 성공 하였을 경우 TRUE를 반환하며, 실패하였을 경우 FALSE를 반환한다.<br>
		실패시 보다 세부적인 오류코드를 얻으려면, SSOWebLib_GetLastError() 함수의 반환값을 참조한다.
		@param[out] pstrAuth - 인증 웹 쿠키를 저장할 버퍼 포인터.
		@param[out] pstrData - 데이터 웹 쿠키를 저장할 버퍼 포인터.
		@param[out] pstrCp - CP 웹 쿠키를 저장할 버퍼 포인터.
		*/
		BOOL NMGetSSOWebCookieString(LPSTR pstrAuth, LPSTR pstrData, LPSTR pstrCp)
		{
			return GetSSOWebCookieString(pstrAuth, pstrData, pstrCp);
		}

		/*
		@brif - SSO 웹브라우져로부터 쿠키를 얻는 함수<br>
		내부적으로 변환되기 때문에 얻은 그대로 put_cookie하면 된다.
		@returns 해당 함수가 성공 하였을 경우 TRUE를 반환하며, 실패하였을 경우 FALSE를 반환한다.<br>
		실패시 보다 세부적인 오류코드를 얻으려면, SSOWebLib_GetLastError() 함수의 반환값을 참조한다.
		@param[out] pszAuth - 인증 웹 쿠키를 저장할 버퍼 포인터.
		@param[in] cbAuth - 인증 웹 쿠키를 저장할 버퍼의 크기
		@param[out] pszData - 데이터 웹 쿠키를 저장할 버퍼 포인터.
		@param[in] cbData - 데이터 웹 쿠키를 저장할 버퍼의 크기
		@param[out] pszCP - CP 웹 쿠키를 저장할 버퍼 포인터.
		@param[in] cbCP - CP 웹 쿠키를 저장할 버퍼의 크기
		*/
		BOOL NMGetSSOWebCookieString(LPSTR pszAuth, DWORD cbAuth, LPSTR pszData, DWORD cbData, LPSTR pszCP, DWORD cbCP)
		{
			return GetSSOWebCookieString(pszAuth, cbAuth, pszData, cbData, pszCP, cbCP);
		}

		/*
		@brif - SSO 웹브라우져에 저장된 쿠키의 길이를 얻는 함수<br>
		@returns 해당 함수가 성공 하였을 경우 TRUE를 반환하며, 실패하였을 경우 FALSE를 반환한다.<br>
		실패시 보다 세부적인 오류코드를 얻으려면, SSOWebLib_GetLastError() 함수의 반환값을 참조한다.
		@param[out] cbAuth - 인증 웹 쿠키를 저장할때 필요한 버퍼의 크기
		@param[out] cbData - 데이터 웹 쿠키를 저장할때 필요한 버퍼의 크기
		@param[out] cbCP - CP 웹 쿠키를 저장할때 필요한 버퍼의 크기
		*/
		BOOL NMGetSSOWebCookieStringLength(DWORD& cbAuth, DWORD& cbData, DWORD& cbCP)
		{
			return GetSSOWebCookieStringLength(cbAuth, cbData, cbCP);
		}

		/*
		@brif - 넷마블 웹 쿠키(인증, 데이터, CP)가 저장된 Internet Explorer 창을 생성하여 IDispatch 포인터를 반환한다.
		@note 기존에 생성된 Internet Explorer창의 프로세스가 있을 경우 해당 프로세스에 창이 추가된다.
		@returns 해당 함수가 성공 하였을 경우 TRUE를 반환하며, 실패하였을 경우 FALSE를 반환한다.<br>
		실패시 보다 세부적인 오류코드를 얻으려면, SSOWebLib_GetLastError() 함수의 반환값을 참조한다.
		@param[out] ppDisp - 새로 생성된 Internet Explorer 창의 IDispatch 포인터.
		*/
		BOOL NMSSOWEBLib_CreateNewIEDispatch(IDispatch** ppDisp)
		{
			return SSOWEBLib_CreateNewIEDispatch(ppDisp);
		}

		/*
		@brif - 넷마블 웹 쿠키(인증, 데이터, CP)가 저장된 Internet Explorer 창을 생성하여 지정된 URL로 Navigate시킨다.
		@note 기존에 생성된 Internet Explorer창의 프로세스가 있을 경우 해당 프로세스에 창이 추가된다.
		@returns 해당 함수가 성공 하였을 경우 TRUE를 반환하며, 실패하였을 경우 FALSE를 반환한다.<br>
		실패시 보다 세부적인 오류코드를 얻으려면, SSOWebLib_GetLastError() 함수의 반환값을 참조한다.
		@param[in] szURL - 신규 생성될 Internet Explorer에서 Navigate 시킬 URL.
		*/
		BOOL NMSSOWEBLib_OpenInternetExplorer(LPCSTR szURL)
		{
			return SSOWEBLib_OpenInternetExplorer(szURL);
		}

		/*
		@brif - 가장 최근에 설정된 SSO 웹브라우져 오류 코드(NMSSOWEB_RETCODE)를 가져온다.
		@returns 가장 최근에 설정된 SSO 웹브라우져 오류값을 반환한다. 반환의 내용은 @ref ERROR_SSOWEB 를 참조한다.
		*/
		NMSSOWEB_RETCODE NMSSOWebLib_GetLastError()
		{
			return SSOWebLib_GetLastError();
		}

		/*
		@brif - 가장 최근에 설정된 SSO 웹브라우져 오류 코드(NMSSOWEB_RETCODE)를 문자열 코드로 가져온다.
		@returns 성공 하였을 경우 @ref ERROR_SSOWEB_SUCCESS 를 반환하며, 실패할 경우 @ref ERROR_SSOWEB_SUCCESS 외의 @ref ERROR_SSOWEB 값을 반환한다.
		@param[out] szBuffer - SSO 웹브라우져 오류 코드를 문자열 코드로 저장할 버퍼 포인터.
		@param[in] cbSize - SSO 웹브라우져 오류 코드를 문자열 형태로 저장할 버퍼의 크기.
		*/
		NMSSOWEB_RETCODE NMSSOWebLib_GetLastError(LPSTR szBuffer, size_t cbSize)
		{
			UNREFERENCED_PARAMETER( szBuffer );
			UNREFERENCED_PARAMETER( cbSize );

			return SSOWebLib_GetLastError();
		}

		/*
		@brif - 주어진 SSO 웹브라우져 오류 코드를 문자열 형태로 변환한다.
		@returns 성공 하였을 경우 @ref ERROR_SSOWEB_SUCCESS 를 반환하며, 실패할 경우 @ref ERROR_SSOWEB_SUCCESS 외의 @ref ERROR_SSOWEB 값을 반환한다.
		@param[in] hError - 문자열 코드로 변환할 SSO 웹브라우져 오류 코드.
		@param[out] szBuffer - 주어진 SSO 웹브라우져 오류 코드를 문자열 코드로 저장할 버퍼 포인터.
		@param[in] cbSize - 주어진 SSO 웹브라우져 오류 코드를 문자열 형태로 저장할 버퍼의 크기.
		*/
		NMSSOWEB_RETCODE NMSSOWebLib_ErrorCode2String(NMSSOWEB_RETCODE hError, LPSTR szBuffer, size_t cbSize)
		{
			return SSOWebLib_ErrorCode2String(hError, szBuffer, cbSize);
		}

		NMSWEBHOOKFUNCPTR_NAVIGATE s_pSSOWEBHookFuncPtr = NULL;

		bool __fastcall NMFuncHookNavigate(const char* lpszURL, CSSOWEBURLString& strURL_Out)
		{
			if(s_pSSOWEBHookFuncPtr)
			{
				CNMSSOWEBURLString cURl(&strURL_Out);
				return (*s_pSSOWEBHookFuncPtr)(lpszURL, cURl);
			}

			return true;
		}

		/*
		@brif - @ref CSSOWebBrowser::Navigate() 함수 호출시 URL 변경을 위한 Hook 함수를 설정한다.
		@returns 성공 하였을 경우 @ref ERROR_SSOWEB_SUCCESS 를 반환하며, 실패할 경우 @ref ERROR_SSOWEB_SUCCESS 외의 @ref ERROR_SSOWEB 값을 반환한다.
		@param[in] pFunc - CSSOWebBrowser::Navigate() 함수 호출시 URL 변경을 처리할 함수의 포인터.
		@note 해당 함수로 후킹 함수를 설정할 경우 모든 CSSOWebBrowser 클래스에 적용된다.
		*/
		NMSSOWEB_RETCODE NMSSOWebLib_SetHookFuncForNavigate(NMSWEBHOOKFUNCPTR_NAVIGATE pFunc)
		{
			s_pSSOWEBHookFuncPtr = pFunc;
			return SSOWebLib_SetHookFuncForNavigate(NMFuncHookNavigate);
		}

		CNMSSOWebBrowser::CNMSSOWebBrowser()
		{
			m_pBrowser = new CSSOWebBrowser;
		}

		CNMSSOWebBrowser::~CNMSSOWebBrowser()
		{
			if(m_pBrowser) delete m_pBrowser;
			m_pBrowser = NULL;
		}

		/**
		@brif - SSO 웹 콘트롤 초기화 및 생성
		@returns 해당 함수가 성공 하였을 경우 TRUE를 반환하며, 실패하였을 경우 FALSE를 반환한다.<br>
		실패시 보다 세부적인 오류코드를 얻으려면, SSOWebLib_GetLastError() 함수의 반환값을 참조한다.
		@param[in] dwStyle - 생성될 SSO 웹 콘트롤 윈도우의 스타일.
		@param[in] rect - 생성될 SSO 웹 콘트롤 윈도우의 영역.
		@param[in] hWndParent - 생성될 SSO 웹 콘트롤의 부모 윈도우 핸들.
		@param[in] nID - 생성될 SSO 웹 콘트롤의 콘트롤 ID
		@param[in] lpExternalDisp - 생성될 SSO 웹 콘트롤의 HTML내에서 사용될 외부 객체(window.external)의 포인터.
		@section SECEXAMPLE 관련 예제
		예제 - @ref SECTIONCREATE "웹 콘트롤 생성" 참조<br>
		예제 - @ref PAGEEXAMPLEEXTERNALCREATE "외부 객체 지원 웹 콘트롤 생성" 참조
		*/
		BOOL CNMSSOWebBrowser::Create(DWORD dwStyle, RECT& rect, HWND hWndParent, UINT nID, LPDISPATCH lpExternalDisp/* = NULL*/)
		{
			return m_pBrowser->Create(dwStyle, rect, hWndParent, nID, lpExternalDisp);
		}

		/**
		@brif - SSO 웹 콘트롤 파괴
		@section SECEXAMPLE 관련 예제
		예제 - @ref SECTIONDESTROY "웹 콘트롤 파괴" 참조
		*/
		void CNMSSOWebBrowser::Destroy()
		{
			m_pBrowser->Destroy();
		}

		/**
		@brif - 해당 콘트롤에서 웹페이지가 Navigate 되는 동안에 화면상에 자동으로 숨기는 옵션을 설정한다.
		@param[in] bEnable - 옵션을 활성화할겨우에는 TRUE를, 비활성화 할 경우에는 FALSE를 입력한다.
		*/
		void CNMSSOWebBrowser::SetAutoHide(BOOL bEnable)
		{
			m_pBrowser->SetAutoHide(bEnable);
		}

		/**
		@brif - 해당 콘트롤에서 웹페이지가 Navigate 될때 Local URL 히스토리에서 숨기는 옵션을 설정한다.<br>
		해당 옵션이 설정될 경우 HTML에서의 GoFoward()와 GoBack()은 작동되지 않는다.
		@note 해당 옵션이 설정되더라도 Global URL 히스토리(열어본 페이지)에는 무조건 남는다.(IE 보안상 제약)
		@param[in] bEnable - 옵션을 활성화할겨우에는 TRUE를, 비활성화 할 경우에는 FALSE를 입력한다.
		*/
		void CNMSSOWebBrowser::SetNavNoHistory(BOOL bEnable)
		{
			m_pBrowser->SetNavNoHistory(bEnable);
		}

		/**
		@brif - 해당 콘트롤에서 웹페이지가 열릴 때, Scroll바를 자동으로 제거하는 옵션을 설정한다.
		@param[in] bEnable - 옵션을 활성화할겨우에는 TRUE를, 비활성화 할 경우에는 FALSE를 입력한다.
		*/
		void CNMSSOWebBrowser::SetHideScroll(BOOL bEnable)
		{
			m_pBrowser->SetNavNoHistory(bEnable);
		}

		/**
		@brif - 해당 콘트롤에서 웹페이지가 열릴 때, Border를 자동으로 제거하는 옵션을 설정한다.
		@param[in] bEnable - 옵션을 활성화할겨우에는 TRUE를, 비활성화 할 경우에는 FALSE를 입력한다.
		*/
		void CNMSSOWebBrowser::SetHideBorder(BOOL bEnable)
		{
			m_pBrowser->SetHideBorder(bEnable);
		}

		/**
		@brif - 현재 열린 HTML 웹페이지 언어 문자열 인코딩 설정을 한다. 
		@returns 해당 함수가 성공 하였을 경우 TRUE를 반환하며, 실패하였을 경우 FALSE를 반환한다.<br>
		실패시 보다 세부적인 오류코드를 얻으려면, SSOWebLib_GetLastError() 함수의 반환값을 참조한다.
		@param[in] strCharset - 변경할 언어 문자열 인코딩 코드.  (인코딩 코드 예 euc-kr, shift-jis)
		*/
		BOOL CNMSSOWebBrowser::PutCharSet(LPCSTR strCharset)
		{
			return m_pBrowser->PutCharSet(strCharset);
		}

		/**
		@brif - 현재 열린 HTML 웹페이지끝에 주어진 HTML 코드를 추가한다.
		@returns 해당 함수가 성공 하였을 경우 TRUE를 반환하며, 실패하였을 경우 FALSE를 반환한다.<br>
		실패시 보다 세부적인 오류코드를 얻으려면, SSOWebLib_GetLastError() 함수의 반환값을 참조한다.
		@param[in] strHtml - 
		*/
		BOOL CNMSSOWebBrowser::InsertHTML(LPCSTR strHtml)
		{
			return m_pBrowser->InsertHTML(strHtml);
		}

		/**
		@brif - 주어진 자바 스크립트 코드를 실행한다.
		@returns 해당 함수가 성공 하였을 경우 TRUE를 반환하며, 실패하였을 경우 FALSE를 반환한다.<br>
		실패시 보다 세부적인 오류코드를 얻으려면, SSOWebLib_GetLastError() 함수의 반환값을 참조한다.
		@param[in] strScript - 실행할 자바 스크립트 코드.
		*/
		BOOL CNMSSOWebBrowser::ExecJavascript(LPCSTR strScript)
		{
			return m_pBrowser->ExecJavascript(strScript);
		}

		/**
		@brif - 주어진 URL로 Navigate시킨다.
		@returns 해당 함수가 성공 하였을 경우 TRUE를 반환하며, 실패하였을 경우 FALSE를 반환한다.<br>
		실패시 보다 세부적인 오류코드를 얻으려면, SSOWebLib_GetLastError() 함수의 반환값을 참조한다.
		@param[in] strURL - Navigate 시킬 URL.
		*/
		BOOL CNMSSOWebBrowser::Navigate(LPCSTR strURL)
		{
			return m_pBrowser->Navigate(strURL);
		}

		/**
		@brif - 주어진 리소스 아이디로 Navigate 시킨다.
		@returns 해당 함수가 성공 하였을 경우 TRUE를 반환하며, 실패하였을 경우 FALSE를 반환한다.<br>
		실패시 보다 세부적인 오류코드를 얻으려면, SSOWebLib_GetLastError() 함수의 반환값을 참조한다.
		@param[in] uResID - Navigate 시킬 HTML 리소스의 아이디
		*/
		BOOL CNMSSOWebBrowser::NavigateFromRes(UINT uResID, HINSTANCE hResInstance/* = NULL*/)
		{
			return m_pBrowser->NavigateFromRes(uResID, hResInstance);
		}

		/**
		@brif - 주어진 리소스 아이디로 Navigate 시킨다.
		@returns 해당 함수가 성공 하였을 경우 TRUE를 반환하며, 실패하였을 경우 FALSE를 반환한다.<br>
		실패시 보다 세부적인 오류코드를 얻으려면, SSOWebLib_GetLastError() 함수의 반환값을 참조한다.
		@param[in] szResID - Navigate 시킬 HTML 리소스의 아이디
		*/
		BOOL CNMSSOWebBrowser::NavigateFromRes(LPCSTR szResID, HINSTANCE hResInstance/* = NULL*/)
		{
			return m_pBrowser->NavigateFromRes(szResID, hResInstance);
		}

		/**
		@brif - 현재 SSO 웹 콘트롤에서 Navigate 된 횟수를 가져온다.
		@note 현재는 구현되지 않았음.
		@returns Navigate 된 횟수를 반환한다. 
		*/
		int	CNMSSOWebBrowser::GetUserNavigatedCount()
		{
			return m_pBrowser->GetUserNavigatedCount();
		}

		/**
		@brif - 주어진 값으로 Navigate 된 횟수를 변경한다.
		@note 현재는 구현되지 않았음.
		@param[in] count - Navigate 된 횟수를 변경할 값.
		*/
		void CNMSSOWebBrowser::SetUserNavigatedCount(int count)
		{
			m_pBrowser->SetUserNavigatedCount(count);
		}

		/**
		@brif - Navigate 된 횟수를 초기화한다.
		@note 현재는 구현되지 않았음.
		*/
		void CNMSSOWebBrowser::ResetUserNavigatedCount()
		{
			m_pBrowser->ResetUserNavigatedCount();
		}

		/**
		@brif - SSO 웹 콘트롤 윈도우 핸들값을 가져온다.
		@returns SSO 웹 콘트롤 윈도우 핸들값을 반환한다. 
		*/
		HWND CNMSSOWebBrowser::GetHWND()
		{
			return m_pBrowser->GetHWND();
		}

		/**
		@brif - SSO 웹 콘트롤 부모 윈도우 핸들값을 가져온다.
		@returns SSO 웹 콘트롤 부모 윈도우 핸들값을 반환한다.
		*/
		HWND CNMSSOWebBrowser::GetParentHWND()
		{
			return m_pBrowser->GetParentHWND();
		}

		/**
		@brif - 해당 좌표로 웹콘트롤을 이동 및 리사이즈 시킨다.
		@param[in] x - 웹 콘트롤의 좌측 위치
		@param[in] y - 웹 콘트롤의 상단 위치
		@param[in] nWidth - 웹 콘트롤의 너비
		@param[in] nHeight - 웹 콘트롤의 높이
		@param[in] bRepaint - 주어진 값이 TRUE이면, WM_PAINT 메시지를 발생시킨다.
		*/
		void CNMSSOWebBrowser::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint/* = TRUE*/)
		{
			m_pBrowser->MoveWindow(x, y, nWidth, nHeight, bRepaint);
		}

		/**
		@brif - 해당 영역으로 웹콘트롤을 이동 및 리사이즈 시킨다.
		@param[in] lpRect - 이동 및 리사이즈 시킬 영역
		@param[in] bRepaint - 주어진 값이 TRUE이면, WM_PAINT 메시지를 발생시킨다.
		*/
		void CNMSSOWebBrowser::MoveWindow(LPCRECT lpRect, BOOL bRepaint/* = TRUE*/)
		{
			m_pBrowser->MoveWindow(lpRect, bRepaint);
		}

		/**
		@brif - 현재 SSO 웹 콘트롤의 show state가 visible한지 알려준다.
		@returns SSO 웹 콘트롤의 show state가 visible이면 TRUE를 반환하며, 반대의 경우 FALSE를 반환한다.
		*/
		BOOL CNMSSOWebBrowser::IsWindowVisible()
		{
			return m_pBrowser->IsWindowVisible();
		}

		/**
		@brif - SSO 웹 콘트롤의 show state를 변경한다.
		@returns 함수를 호출하기전에 SSO 웹 콘트롤의 show state가 visible이면 TRUE를 반환하며, 반대의 경우 FALSE를 반환한다.
		@param[in] nCmdShow - 변경하고자 하는 show state 값. (Ex> SW_SHOW, SW_HIDE 등)
		*/
		BOOL CNMSSOWebBrowser::ShowWindow(int nCmdShow)
		{
			return m_pBrowser->ShowWindow(nCmdShow);
		}

		/**
		@brif - @ref SECBeforeNavigate2 "BeforeNavigate2" 이벤트 발생시에 호출될 콜백 함수를 설정한다.<br>
		(BeforeNavigate2는 새로운 URL로 Navigate 되기 직전에 발생하는 이벤트.)
		@returns 해당 함수가 성공 하였을 경우 TRUE를 반환하며, 실패하였을 경우 FALSE를 반환한다.<br>
		실패시 보다 세부적인 오류코드를 얻으려면, SSOWebLib_GetLastError() 함수의 반환 값을 참조한다.
		@param[in] pCallback - 이벤트 발생시에 호출될 콜백 함수 포인터.
		@section SECEXAMPLE 관련 예제
		예제 - @ref PAGEEXAMPLECALLBACK "웹 콘트롤 콜백 함수" 참조
		*/
		BOOL CNMSSOWebBrowser::SetCallback_OnBeforeNavigate2(SWEBCALLBACK_ONBN2* pCallback)
		{
			return m_pBrowser->SetCallback_OnBeforeNavigate2(pCallback);
		}

		/**
		@brif - @ref SECNavigateComplete2 "NavigateComplete2" 이벤트 발생시에 호출될 콜백 함수를 설정한다.<br>
		@returns 해당 함수가 성공 하였을 경우 TRUE를 반환하며, 실패하였을 경우 FALSE를 반환한다.<br>
		실패시 보다 세부적인 오류코드를 얻으려면, SSOWebLib_GetLastError() 함수의 반환 값을 참조한다.
		@param[in] pCallback - 이벤트 발생시에 호출될 콜백 함수 포인터.
		@section SECEXAMPLE 관련 예제
		예제 - @ref PAGEEXAMPLECALLBACK "웹 콘트롤 콜백 함수" 참조
		*/
		BOOL CNMSSOWebBrowser::SetCallback_OnNavigateComplete2(SWEBCALLBACK_ONNC2* pCallback)
		{
			return m_pBrowser->SetCallback_OnNavigateComplete2(pCallback);
		}

		/**
		@brif - @ref SECQuit "Quit" 이벤트 발생시에 호출될 콜백 함수를 설정한다.<br>
		@returns 해당 함수가 성공 하였을 경우 TRUE를 반환하며, 실패하였을 경우 FALSE를 반환한다.<br>
		실패시 보다 세부적인 오류코드를 얻으려면, SSOWebLib_GetLastError() 함수의 반환 값을 참조한다.
		@param[in] pCallback - 이벤트 발생시에 호출될 콜백 함수 포인터.
		@section SECEXAMPLE 관련 예제
		예제 - @ref PAGEEXAMPLECALLBACK "웹 콘트롤 콜백 함수" 참조
		*/
		BOOL CNMSSOWebBrowser::SetCallback_OnQuit(SWEBCALLBACK_ONQUIT* pCallback)
		{
			return m_pBrowser->SetCallback_OnQuit(pCallback);
		}

		/**
		@brif - 주어진 Dispatch 인터페이스가 SSO Web 콘트롤의 인터페이스와 동일한지 확인한다.
		@returns SSO Web 콘트롤의 인터페이스와 동일하면 TRUE를, 그렇지 안다면 FALSE를 반환한다.
		@param[in] pDisp - SSO Web 콘트롤의 인터페이스와 동일한지 확인할 Dispatch 인터페이스.
		*/
		BOOL CNMSSOWebBrowser::IsMe(LPDISPATCH pDisp)
		{
			return m_pBrowser->IsMe(pDisp);
		}

		/**
		@brif - 주어진 IID에 맞는 인터페이스로 변환한다.
		@returns 해당 함수가 성공 하였을 경우 TRUE를 반환하며, 실패하였을 경우 FALSE를 반환한다.<br>
		실패시 보다 세부적인 오류코드를 얻으려면, SSOWebLib_GetLastError() 함수의 반환 값을 참조한다.
		@param[in] iid - 가져올 인터페이스 아이디(IID)
		@param[out] ppvObject - 가져올 인터페이스 포인터를 저장할 버퍼.
		*/
		BOOL CNMSSOWebBrowser::QueryInterface(REFIID iid, void ** ppvObject)
		{
			return m_pBrowser->QueryInterface(iid, ppvObject);
		}

		//////////////////////////////////////////////////////////////////////////
		// ISSOWebEventsUnknown Interface
		/**
		@brif - 주어진 인터페이스 열거자에 해당하는 이벤트 인터페이스 포인터를 가져온다.
		@note 모듈 내부적으로 사용되어지는 함수이브로 해당 함수를 재정의할 경우 모듈의 정상적인 작동이 안된다.
		@returns 성공할경우 TRUE를 실패할경우 FALSE를 반환한다.
		@param[in] eEventVer - 가져오려는 이벤트 인터페이스에 해당하는 열거자.
		@param[out] pVal_Out - 가져오려는 이벤트 인터페이스 버퍼.
		*/
		//BOOL CNMSSOWebBrowser::SSOWebEventQueryInterface(ENUM_EventVer eEventVer, void** pVal_Out)
		//{
		//	return m_pBrowser->SSOWebEventQueryInterface(eEventVer, pVal_Out);
		//}

		//////////////////////////////////////////////////////////////////////////
		// ISSOWebEventsVer1 Interface
		/**
		@brif - @ref SECBeforeNavigate2 "BeforeNavigate2" 이벤트가 발생될 때 호출됨.<br>
		@param[in] pDisp - 이벤트가 발생한 WebBrowser의 Dispatch 인터페이스.
		@param[in] lpszURL - Navigate 될 URL
		@param[in] nFlags - 추후를 위해 예약됨.
		@param[in] lpszTargetFrameName - Navigate 될 Frame 이름 
		@param[in] lpszPostedData - Navigate 될 URL에 전달될 Post 데이터
		@param[in] lpszHeaders - Navigate 될 URL에 전달될 Header 데이터
		@param[out] pbCancel - Navigate를 취소 시킬 경우 해당 값의 내용을 TRUE로 설정. 그렇지 않을 경우 FALSE로 설정.
		@section SECEXAMPLE 관련 예제
		예제 - @ref PAGEEXAMPLEEVENTOVERRIDE "웹 콘트롤 이벤트 멤버 함수 재정의" 참조
		*/
		void CNMSSOWebBrowser::OnSSOWebEventBN2(LPDISPATCH pDisp, const char* lpszURL, DWORD nFlags, const char* lpszTargetFrameName, const char* lpszPostedData, const char* lpszHeaders, BOOL* pbCancel)
		{
			m_pBrowser->OnSSOWebEventBN2(pDisp, lpszURL, nFlags, lpszTargetFrameName, lpszPostedData, lpszHeaders, pbCancel);
		}

		/**
		@brif - @ref @ref SECNavigateComplete2 "NavigateComplete2" 이벤트가 발생될 때 호출됨.<br>
		@param[in] pDisp - 이벤트가 발생한 WebBrowser의 Dispatch 인터페이스.
		@param[in] lpszURL - Navigate가 완료된 URL
		@section SECEXAMPLE 관련 예제
		예제 - @ref PAGEEXAMPLEEVENTOVERRIDE "웹 콘트롤 이벤트 멤버 함수 재정의" 참조
		*/
		void CNMSSOWebBrowser::OnSSOWebEventNC2(LPDISPATCH pDisp, const char* lpszURL)
		{
			m_pBrowser->OnSSOWebEventNC2(pDisp, lpszURL);
		}

		/**
		@brif - @ref SECNewWindow2 "NewWindow2" 이벤트가 발생될 때 호출됨.<br>
		@param[out] ppDisp - 새창이 열릴때 대체할 WebBrowser의 Dispatch 인터페이스. 대체하지 않을 경우 값을 변경하지 않도록 한다.
		@param[out] Cancel - 새창이 열리는것을 취소 시킬 경우 해당 값의 내용을 TRUE로 설정. 그렇지 않을 경우 FALSE로 설정.
		@section SECEXAMPLE 관련 예제
		예제 - @ref PAGEEXAMPLEEVENTOVERRIDE "웹 콘트롤 이벤트 멤버 함수 재정의" 참조
		*/
		void CNMSSOWebBrowser::OnSSOWebEventNW2(LPDISPATCH *ppDisp, BOOL* Cancel)
		{
			m_pBrowser->OnSSOWebEventNW2(ppDisp, Cancel);
		}

		/**
		@brif - @ref @ref SECQuit "Quit" 이벤트가 발생될 때 호출됨.
		@section SECEXAMPLE 관련 예제
		예제 - @ref PAGEEXAMPLEEVENTOVERRIDE "웹 콘트롤 이벤트 멤버 함수 재정의" 참조
		*/
		void CNMSSOWebBrowser::OnSSOWebEventQuit()
		{
			m_pBrowser->OnSSOWebEventQuit();
		}

	}//namespace UTIL

}//namespace NMBASE