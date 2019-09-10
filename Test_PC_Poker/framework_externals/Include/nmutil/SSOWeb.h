// NMSSOWebLib.h: interface for the CSSOWebLib class.
//////////////////////////////////////////////////////////////////////
#pragma once


#ifndef BADFILTER_IMPORT

typedef unsigned long	NMSSOWEB_RETCODE;

/// @brief - SSOWebLib �ݸ��� SSO�������� �����ڵ�
enum ERROR_SSOWEB 
{
	ERROR_SSOWEB_SUCCESS = 0,						///< ����(���� ����).
	ERROR_SSOWEB_FAIL,								///< ����.
	///	@details <b> ERROR_SSOWEB_LIB
	ERROR_SSOWEB_LIB_INVALIDPARAM = 1001,			///< �߸��� �Է� �Ķ����.
	ERROR_SSOWEB_LIB_NOT_INITIALIZED,				///< SSOWebLib�� �ʱ�ȭ���� �ʾ���.
	ERROR_SSOWEB_LIB_ALREADY_INITIALIZED,			///< SSOWebLib�� �̹� �ʱ�ȭ�Ǿ���.
	ERROR_SSOWEB_LIB_CANT_LOAD_DLL,					///< SSOWebDLL �ε� ����.
	ERROR_SSOWEB_LIB_NOT_FOUND_PATH,				///< SSOWebDLL ��� ã�� ����.
	ERROR_SSOWEB_LIB_NOT_FOUND_DLLPROC,				///< SSOWebDLL �Լ� ã�� ����.
	ERROR_SSOWEB_LIB_NOT_LOADED_DLL,				///< SSOWebDLL �ε尡 ���� �ʾ���.
	ERROR_SSOWEB_LIB_WEBCTRL_ALREADY_CREATED,		///< SSO ����Ʈ���� �̹� ������.
	ERROR_SSOWEB_LIB_WEBCTRL_NOT_CREATED,			///< SSO ����Ʈ���� �������� �ʾ���.
	ERROR_SSOWEB_LIB_NOT_EOUNGH_BUFFER,				///< �Է� ������ ���̰� ������� ����.
	ERROR_SSOWEB_LIB_NOT_EXISTS_DLL,					///< DLL�� �������� ����.
	///	@details <b> ERROR_SSOWEB_DLL
	ERROR_SSOWEB_DLL_INVALIDPARAM = 10001,			///< �߸��� �Է� �Ķ����.
	ERROR_SSOWEB_DLL_NOT_INITIALIZED,				///< SSOWebDLL�� �ʱ�ȭ���� �ʾ���.
	ERROR_SSOWEB_DLL_ALREADY_INITIALIZED,			///< SSOWebDLL�� �̹� �ʱ�ȭ�Ǿ���.
	ERROR_SSOWEB_DLL_MISSING_INITIAL_PARAM,			///< SSOWebDLL�� �ʱ�ȭ �Ķ���Ͱ� ������.
	ERROR_SSOWEB_DLL_INVALID_MAIN_HWND,				///< �߸��� Main ������ �ڵ�.
	ERROR_SSOWEB_DLL_CANT_CREATE_MAIN_WEBCTRL,		///< ���� SSO �� ��Ʈ�� ������ ������.
	///	@details <b> ERROR_SSOWEB_DLL_HOOK
	ERROR_SSOWEB_DLL_HOOK_FAIL_SETHOOK = 10101,		///< ������ �޽��� Hook ����
	ERROR_SSOWEB_DLL_HOOK_FAIL_UNHOOK,						///< ������ �޽��� UnHook ����
	///	@details <b> ERROR_SSOWEB_DLL_COOKIE
	ERROR_SSOWEB_DLL_COOKIE_IS_EMPTY = 11001,		///< ����Ű���� ����.
	ERROR_SSOWEB_DLL_COOKIE_NOT_EXISTS,				///< ����Ű���� �������� ����.
	ERROR_SSOWEB_DLL_COOKIE_CANT_GET,				///< ����Ű���� ������ �� ����.
	ERROR_SSOWEB_DLL_COOKIE_NOTENOUGH_BUFFER,		///< ����Ű���� ������ ���۰����� ������� ����.
	///	@details <b> ERROR_SSOWEB_DLL_COOKIE_AUTH
	ERROR_SSOWEB_DLL_COOKIE_AUTH_NOT_FOUND_SEP = 12101,	///< ���� ����Ű ������ �����ڸ� ã������ ����.
	ERROR_SSOWEB_DLL_COOKIE_AUTH_CANT_COPY,				///< ���� ����Ű���� ���۷� �����ϴµ� ������.
	///	@details <b> ERROR_SSOWEB_DLL_WEBCTRL
	ERROR_SSOWEB_DLL_WEBCTRL_NOT_FOUND = 15001,			///< �־��� SSO ����Ʈ���� ã������ ����.
	ERROR_SSOWEB_DLL_WEBCTRL_OUT_OF_MEMORY,				///< �޸� ����.
	ERROR_SSOWEB_DLL_WEBCTRL_CREATE_FAIL,				///< SSO �� ��Ʈ�� ���� ����.
	ERROR_SSOWEB_DLL_WEBCTRL_NOT_CREATED,				///< SSO �� ��Ʈ���� �������� �ʾ���.
	ERROR_SSOWEB_DLL_WEBCTRL_FAIL_QUERYINTERFACE,		///< SSO �� ��Ʈ�� QueryInterface����.
};

//////////////////////////////////////////////////////////////////////
/**
@defgroup GROUPECALLBACKFUNC CallBack �Լ� ����
@{
*/
/// @brief - OnBerforeNavigate2 �ݹ� �Լ� ����

/**
@brif - @ref SECBeforeNavigate2 "BeforeNavigate2" �̺�Ʈ �� �����Ǵ� �ݹ� �Լ�
@param[in] lpszURL - Navigate �� URL
@param[in] nFlags - ���ĸ� ���� �����.
@param[in] lpszTargetFrameName - Navigate �� Frame �̸� 
@param[in] lpszPostedData - Navigate �� URL�� ���޵� Post ������
@param[in] lpszHeaders - Navigate �� URL�� ���޵� Header ������
@param[out] pbCancel - Navigate�� ��� ��ų ��� �ش� ���� ������ TRUE�� ����. �׷��� ���� ��� FALSE�� ����.
*/
typedef void (SWEBCALLBACK_ONBN2)(const char* lpszURL, DWORD nFlags, const char* lpszTargetFrameName, const char* lpszPostedData, const char* lpszHeaders, BOOL* pbCancel);
/**
@brif - @ref @ref SECNavigateComplete2 "NavigateComplete2" �̺�Ʈ �� �����Ǵ� �ݹ� �Լ�<br>
@param[in] lpszURL - Navigate�� �Ϸ�� URL
*/
typedef void (SWEBCALLBACK_ONNC2)(const char* lpszURL);
/**
@brif - @ref @ref SECQuit "Quit" �̺�Ʈ �� �����Ǵ� �ݹ� �Լ�
*/
typedef void (SWEBCALLBACK_ONQUIT)();
#else
#include <SSOWebLib.h>
#pragma message( "================================ <SSOWebLib.h> Include ==============================" )
#endif


#include "UtilDefine.h"

class CSSOWEBURLString;
class CSSOWebBrowser;

namespace NMBASE
{
	namespace UTIL
	{
		//////////////////////////////////////////////////////////////////////
		/// @brief - SSO Web URL ��Ʈ�� ���� Ŭ����.
		class NMUTIL_CLASS CNMSSOWEBURLString
		{
		protected:
			CSSOWEBURLString	*m_pURL;
		public:
			/**
			@brif - SSO Web URL ��Ʈ�� ���� Ŭ���� ������
			@param[in] szData - �ش� Ŭ������ ������ ���ڿ�.
			*/
			CNMSSOWEBURLString(CSSOWEBURLString *pURL = NULL);
			/// @brif - SSO Web URL ��Ʈ�� ���� Ŭ���� �Ҹ���
			virtual ~CNMSSOWEBURLString();

			/**
			@brif - ����� ���ڿ� �����͸� �����´�.
			@returns ����� ���ڿ� �����͸� ��ȯ�Ѵ�.
			*/
			LPCSTR	GetData();
			/**
			@brif - �־��� ���ڿ��� �����Ѵ�.
			@param[in] szData - �ش� Ŭ������ ������ ���ڿ�.
			*/
			void	Assign(LPCSTR szData);
			/**
			@brif - ���ڿ� �����͸� Clear�Ѵ�.
			*/
			void	Clear();
		};

		typedef bool (__fastcall* NMSWEBHOOKFUNCPTR_NAVIGATE)(const char* lpszURL, CNMSSOWEBURLString& strURL_Out);

		//////////////////////////////////////////////////////////////////////
		/*
		@defgroup GROUPEAPIFUNC API �Լ�
		@{
		*/
		/*
		@brif - ���� ����Ʈ�� ���� ��Ű �̸��� �����ϱ� ���Ͽ� ȣ���ϴ� �Լ�.<br>
		�ݸ����� ���� ȣ���� �ʿ䰡 ����.
		@note InitSSOWebBrowser() ������ ȣ��Ǿ�� ��.
		@param[in] strSiteName - ���������� ��� "daum", �ϳ����������� ��� "hanafos"�� �ѱ�.
		*/
		NMUTIL_CLASS void NMPresetSSOWebBrowserSetSiteName(LPCSTR strSiteName);

		/*
		@brif - SSO �������� DLL�� ��ġ�� ��θ� �������� ����. ȣ������ ���� ��� ���� ������ SSO �������� DLL�� ����.
		@note InitSSOWebBrowser() ������ ȣ��Ǿ�� ��
		@param[in] strFolder - SSO �������� DLL�� ��ġ�� ���.
		*/
		NMUTIL_CLASS void NMSSOWebLib_SetHookFuncForNavigate(LPCSTR strFolder);

		/*
		@brif - SSO �������� DLL ������ ���� ���(���ϸ� ����)�� �����Ѵ�.
		@param[in] szDLLFullFileName - SSO �������� DLL ������ ���� ���(���ϸ� ����)
		*/
		//NMUTIL_CLASS void NMSetSSOWebBrowserDLLFileName(LPCSTR szDLLFullFileName);

		/*
		@brif - SSO �������� ��� �ʱ�ȭ �Լ�
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@param[in] hWndMain - ���� �������� �ڵ� ��.
		@param[in] strAuthCookie - ���� ����Ű ��.
		@param[in] strDataCookie - ������ ����Ű ��.
		@param[in] strCpCookie - CP ����Ű��.
		@param[in] bHook - �� ��Ʈ�ѿ��� �߻��ϴ� WindowMsg�� ��ŷ�Ͽ� Ű���� �Է� ���� ó�� ���� ����.
		@section SECEXAMPLE ���� ����
		���� - @ref SECTIONINIT "SSO �������� ��� �ʱ�ȭ" ����
		*/
		NMUTIL_CLASS BOOL NMInitSSOWebBrowser(HWND hWndMain, LPCSTR strAuthCookie, LPCSTR strDataCookie, LPCSTR strCpCookie, BOOL bHook = TRUE);

		/*
		@brif - SSO �������� ��� �ʱ�ȭ �Լ�. (�׽�Ʈ��)<br>SSO �������� DLL�� ��θ� �Է� �޴� �κпܿ� InitSSOWebBrowser()�Լ��� �ٸ� ���� ����.
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@param[in] strBaseDir - SSO �������� DLL�� ��ġ�� ���.
		@param[in] hWndMain - ���� �������� �ڵ鰪.
		@param[in] strAuthCookie - ���� ����Ű ��.
		@param[in] strDataCookie - ������ ����Ű ��.
		@param[in] strCpCookie - CP ����Ű��.
		*/
		NMUTIL_CLASS BOOL NMInitSSOWebBrowser2(LPCSTR strBaseDir, HWND hWndMain, LPCSTR strAuthCookie, LPCSTR strDataCookie, LPCSTR strCpCookie);


		/*
		@brif - SSO �������� ��� �Ҹ� �Լ�
		@note �ݵ�� ���� �����찡 �ı��Ǳ� ���� ȣ���ؾ��Ѵ�.
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@section SECEXAMPLE ���� ����
		���� - @ref SECTIONUNINIT "SSO �������� ��� �Ҹ�" ����
		*/
		NMUTIL_CLASS BOOL NMUnInitSSOWebBrowser();

		/*
		@brif - SSO ���������� ó���ؾ� �� �޽����� ��ŷ�Ͽ� ó������ ����.
		@returns ���� �ֱٿ� ������ SSO �������� �������� ��ȯ�Ѵ�. ��ȯ�� ������ @ref ERROR_SSOWEB �� �����Ѵ�.
		@param[in] bSetHook - �� ���� True�̸� ��ŷ�Ͽ� �޽����� ó���ϰ�, False�̸� ��ŷ�� �ߴ��Ѵ�.
		*/
		NMUTIL_CLASS BOOL NMSetHOOKPreTranslateMessage(BOOL bSetHook);

		/*
		@brif - SSO ���������� ó���ؾ� �� �޽��� ó��(Tab, enterŰ ���۵� ���� �ذ�) - TRUE���� �� ó�� �Ϸ� ��
		@note @ref InitSSOWebBrowser API ȣ��ÿ� SSO ���������� ó���ؾ� �� �޽����� ��ŷ ó���Ͽ��ٸ� ȣ�� �ʿ� ����.
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@param[in] pMsg - SSO ���������� ó���ؾ� �� �޽��� ����ü.
		*/
		NMUTIL_CLASS BOOL NMPreTranslateForSSOWebBrowser(LPMSG pMsg);

		/*
		@brif - ��Ű Ȱ��ȭ ������ URL�� ��� �Լ�
		@returns ��Ű Ȱ��ȭ ������ URL�� ��ȯ�Ѵ�.
		*/
		NMUTIL_CLASS LPCSTR NMGetActCookieURL();

		/*
		@brif - ��Ű �������� ������ URL�� ��� �Լ�
		@returns ��Ű �������� ������ URL�� ��ȯ�Ѵ�. 
		*/
		NMUTIL_CLASS LPCSTR NMGetRefreshCookieURL();

		/*
		@brif - ��Ű ������ �������� ��� �Լ�
		@returns ��Ű ������ �������� ��ȯ�Ѵ�.
		*/
		NMUTIL_CLASS LPCSTR NMGetCookieDomain();

		/*
		@brif - SSO ���������κ��� ��Ű�� ��� �Լ�<br>
		���������� ��ȯ�Ǳ� ������ ���� �״�� put_cookie�ϸ� �ȴ�.
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@param[out] pstrAuth - ���� �� ��Ű�� ������ ���� ������.
		@param[out] pstrData - ������ �� ��Ű�� ������ ���� ������.
		@param[out] pstrCp - CP �� ��Ű�� ������ ���� ������.
		*/
		NMUTIL_CLASS BOOL NMGetSSOWebCookieString(LPSTR pstrAuth, LPSTR pstrData, LPSTR pstrCp);
		/*
		@brif - SSO ���������κ��� ��Ű�� ��� �Լ�<br>
		���������� ��ȯ�Ǳ� ������ ���� �״�� put_cookie�ϸ� �ȴ�.
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@param[out] pszAuth - ���� �� ��Ű�� ������ ���� ������.
		@param[in] cbAuth - ���� �� ��Ű�� ������ ������ ũ��
		@param[out] pszData - ������ �� ��Ű�� ������ ���� ������.
		@param[in] cbData - ������ �� ��Ű�� ������ ������ ũ��
		@param[out] pszCP - CP �� ��Ű�� ������ ���� ������.
		@param[in] cbCP - CP �� ��Ű�� ������ ������ ũ��
		*/
		NMUTIL_CLASS BOOL NMGetSSOWebCookieString(LPSTR pszAuth, DWORD cbAuth, LPSTR pszData, DWORD cbData, LPSTR pszCP, DWORD cbCP);
		/*
		@brif - SSO ���������� ����� ��Ű�� ���̸� ��� �Լ�<br>
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@param[out] cbAuth - ���� �� ��Ű�� �����Ҷ� �ʿ��� ������ ũ��
		@param[out] cbData - ������ �� ��Ű�� �����Ҷ� �ʿ��� ������ ũ��
		@param[out] cbCP - CP �� ��Ű�� �����Ҷ� �ʿ��� ������ ũ��
		*/
		NMUTIL_CLASS BOOL NMGetSSOWebCookieStringLength(DWORD& cbAuth, DWORD& cbData, DWORD& cbCP);

		/*
		@brif - �ݸ��� �� ��Ű(����, ������, CP)�� ����� Internet Explorer â�� �����Ͽ� IDispatch �����͸� ��ȯ�Ѵ�.
		@note ������ ������ Internet Explorerâ�� ���μ����� ���� ��� �ش� ���μ����� â�� �߰��ȴ�.
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@param[out] ppDisp - ���� ������ Internet Explorer â�� IDispatch ������.
		*/
		NMUTIL_CLASS BOOL NMSSOWEBLib_CreateNewIEDispatch(IDispatch** ppDisp);
		/*
		@brif - �ݸ��� �� ��Ű(����, ������, CP)�� ����� Internet Explorer â�� �����Ͽ� ������ URL�� Navigate��Ų��.
		@note ������ ������ Internet Explorerâ�� ���μ����� ���� ��� �ش� ���μ����� â�� �߰��ȴ�.
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@param[in] szURL - �ű� ������ Internet Explorer���� Navigate ��ų URL.
		*/
		NMUTIL_CLASS BOOL NMSSOWEBLib_OpenInternetExplorer(LPCSTR szURL);

		/*
		@brif - ���� �ֱٿ� ������ SSO �������� ���� �ڵ�(NMSSOWEB_RETCODE)�� �����´�.
		@returns ���� �ֱٿ� ������ SSO �������� �������� ��ȯ�Ѵ�. ��ȯ�� ������ @ref ERROR_SSOWEB �� �����Ѵ�.
		*/
		NMUTIL_CLASS NMSSOWEB_RETCODE NMSSOWebLib_GetLastError();
		/*
		@brif - ���� �ֱٿ� ������ SSO �������� ���� �ڵ�(NMSSOWEB_RETCODE)�� ���ڿ� �ڵ�� �����´�.
		@returns ���� �Ͽ��� ��� @ref ERROR_SSOWEB_SUCCESS �� ��ȯ�ϸ�, ������ ��� @ref ERROR_SSOWEB_SUCCESS ���� @ref ERROR_SSOWEB ���� ��ȯ�Ѵ�.
		@param[out] szBuffer - SSO �������� ���� �ڵ带 ���ڿ� �ڵ�� ������ ���� ������.
		@param[in] cbSize - SSO �������� ���� �ڵ带 ���ڿ� ���·� ������ ������ ũ��.
		*/
		NMUTIL_CLASS NMSSOWEB_RETCODE NMSSOWebLib_GetLastError(LPSTR szBuffer, size_t cbSize);

		/*
		@brif - �־��� SSO �������� ���� �ڵ带 ���ڿ� ���·� ��ȯ�Ѵ�.
		@returns ���� �Ͽ��� ��� @ref ERROR_SSOWEB_SUCCESS �� ��ȯ�ϸ�, ������ ��� @ref ERROR_SSOWEB_SUCCESS ���� @ref ERROR_SSOWEB ���� ��ȯ�Ѵ�.
		@param[in] hError - ���ڿ� �ڵ�� ��ȯ�� SSO �������� ���� �ڵ�.
		@param[out] szBuffer - �־��� SSO �������� ���� �ڵ带 ���ڿ� �ڵ�� ������ ���� ������.
		@param[in] cbSize - �־��� SSO �������� ���� �ڵ带 ���ڿ� ���·� ������ ������ ũ��.
		*/
		NMUTIL_CLASS NMSSOWEB_RETCODE NMSSOWebLib_ErrorCode2String(NMSSOWEB_RETCODE hError, LPSTR szBuffer, size_t cbSize);

		/*
		@brif - @ref CSSOWebBrowser::Navigate() �Լ� ȣ��� URL ������ ���� Hook �Լ��� �����Ѵ�.
		@returns ���� �Ͽ��� ��� @ref ERROR_SSOWEB_SUCCESS �� ��ȯ�ϸ�, ������ ��� @ref ERROR_SSOWEB_SUCCESS ���� @ref ERROR_SSOWEB ���� ��ȯ�Ѵ�.
		@param[in] pFunc - CSSOWebBrowser::Navigate() �Լ� ȣ��� URL ������ ó���� �Լ��� ������.
		@note �ش� �Լ��� ��ŷ �Լ��� ������ ��� ��� CSSOWebBrowser Ŭ������ ����ȴ�.
		*/
		NMUTIL_CLASS NMSSOWEB_RETCODE NMSSOWebLib_SetHookFuncForNavigate(NMSWEBHOOKFUNCPTR_NAVIGATE pFunc);

		//////////////////////////////////////////////////////////////////////
		/// @brief - SSO �� ��Ʈ�� Ŭ����
		class NMUTIL_CLASS CNMSSOWebBrowser
		{
		protected:
			CSSOWebBrowser	*m_pBrowser;
		public:
			CNMSSOWebBrowser();
			virtual ~CNMSSOWebBrowser();

			/**
			@brif - SSO �� ��Ʈ�� �ʱ�ȭ �� ����
			@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
			@param[in] dwStyle - ������ SSO �� ��Ʈ�� �������� ��Ÿ��.
			@param[in] rect - ������ SSO �� ��Ʈ�� �������� ����.
			@param[in] hWndParent - ������ SSO �� ��Ʈ���� �θ� ������ �ڵ�.
			@param[in] nID - ������ SSO �� ��Ʈ���� ��Ʈ�� ID
			@param[in] lpExternalDisp - ������ SSO �� ��Ʈ���� HTML������ ���� �ܺ� ��ü(window.external)�� ������.
			@section SECEXAMPLE ���� ����
			���� - @ref SECTIONCREATE "�� ��Ʈ�� ����" ����<br>
			���� - @ref PAGEEXAMPLEEXTERNALCREATE "�ܺ� ��ü ���� �� ��Ʈ�� ����" ����
			*/
			BOOL Create(DWORD dwStyle, RECT& rect, HWND hWndParent, UINT nID, LPDISPATCH lpExternalDisp = NULL);
			/**
			@brif - SSO �� ��Ʈ�� �ı�
			@section SECEXAMPLE ���� ����
			���� - @ref SECTIONDESTROY "�� ��Ʈ�� �ı�" ����
			*/
			void Destroy();

			/**
			@brif - �ش� ��Ʈ�ѿ��� ���������� Navigate �Ǵ� ���ȿ� ȭ��� �ڵ����� ����� �ɼ��� �����Ѵ�.
			@param[in] bEnable - �ɼ��� Ȱ��ȭ�Ұܿ쿡�� TRUE��, ��Ȱ��ȭ �� ��쿡�� FALSE�� �Է��Ѵ�.
			*/
			void SetAutoHide(BOOL bEnable);
			/**
			@brif - �ش� ��Ʈ�ѿ��� ���������� Navigate �ɶ� Local URL �����丮���� ����� �ɼ��� �����Ѵ�.<br>
			�ش� �ɼ��� ������ ��� HTML������ GoFoward()�� GoBack()�� �۵����� �ʴ´�.
			@note �ش� �ɼ��� �����Ǵ��� Global URL �����丮(��� ������)���� ������ ���´�.(IE ���Ȼ� ����)
			@param[in] bEnable - �ɼ��� Ȱ��ȭ�Ұܿ쿡�� TRUE��, ��Ȱ��ȭ �� ��쿡�� FALSE�� �Է��Ѵ�.
			*/
			void SetNavNoHistory(BOOL bEnable);
			/**
			@brif - �ش� ��Ʈ�ѿ��� ���������� ���� ��, Scroll�ٸ� �ڵ����� �����ϴ� �ɼ��� �����Ѵ�.
			@param[in] bEnable - �ɼ��� Ȱ��ȭ�Ұܿ쿡�� TRUE��, ��Ȱ��ȭ �� ��쿡�� FALSE�� �Է��Ѵ�.
			*/
			void SetHideScroll(BOOL bEnable);
			/**
			@brif - �ش� ��Ʈ�ѿ��� ���������� ���� ��, Border�� �ڵ����� �����ϴ� �ɼ��� �����Ѵ�.
			@param[in] bEnable - �ɼ��� Ȱ��ȭ�Ұܿ쿡�� TRUE��, ��Ȱ��ȭ �� ��쿡�� FALSE�� �Է��Ѵ�.
			*/
			void SetHideBorder(BOOL bEnable);

			/**
			@brif - ���� ���� HTML �������� ��� ���ڿ� ���ڵ� ������ �Ѵ�. 
			@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
			@param[in] strCharset - ������ ��� ���ڿ� ���ڵ� �ڵ�.  (���ڵ� �ڵ� �� euc-kr, shift-jis)
			*/
			BOOL PutCharSet(LPCSTR strCharset);
			/**
			@brif - ���� ���� HTML ������������ �־��� HTML �ڵ带 �߰��Ѵ�.
			@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
			@param[in] strHtml - 
			*/
			BOOL InsertHTML(LPCSTR strHtml);
			/**
			@brif - �־��� �ڹ� ��ũ��Ʈ �ڵ带 �����Ѵ�.
			@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
			@param[in] strScript - ������ �ڹ� ��ũ��Ʈ �ڵ�.
			*/
			BOOL ExecJavascript(LPCSTR strScript);

			/**
			@brif - �־��� URL�� Navigate��Ų��.
			@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
			@param[in] strURL - Navigate ��ų URL.
			*/
			BOOL Navigate(LPCSTR strURL);

			/**
			@brif - �־��� ���ҽ� ���̵�� Navigate ��Ų��.
			@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
			@param[in] uResID - Navigate ��ų HTML ���ҽ��� ���̵�
			*/
			BOOL NavigateFromRes(UINT uResID, HINSTANCE hResInstance = NULL);
			/**
			@brif - �־��� ���ҽ� ���̵�� Navigate ��Ų��.
			@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
			@param[in] szResID - Navigate ��ų HTML ���ҽ��� ���̵�
			*/
			BOOL NavigateFromRes(LPCSTR szResID, HINSTANCE hResInstance = NULL);

			/**
			@brif - ���� SSO �� ��Ʈ�ѿ��� Navigate �� Ƚ���� �����´�.
			@note ����� �������� �ʾ���.
			@returns Navigate �� Ƚ���� ��ȯ�Ѵ�. 
			*/
			int	GetUserNavigatedCount();
			/**
			@brif - �־��� ������ Navigate �� Ƚ���� �����Ѵ�.
			@note ����� �������� �ʾ���.
			@param[in] count - Navigate �� Ƚ���� ������ ��.
			*/
			void SetUserNavigatedCount(int count);
			/**
			@brif - Navigate �� Ƚ���� �ʱ�ȭ�Ѵ�.
			@note ����� �������� �ʾ���.
			*/
			void ResetUserNavigatedCount();

			/**
			@brif - SSO �� ��Ʈ�� ������ �ڵ鰪�� �����´�.
			@returns SSO �� ��Ʈ�� ������ �ڵ鰪�� ��ȯ�Ѵ�. 
			*/
			HWND GetHWND();

			/**
			@brif - SSO �� ��Ʈ�� �θ� ������ �ڵ鰪�� �����´�.
			@returns SSO �� ��Ʈ�� �θ� ������ �ڵ鰪�� ��ȯ�Ѵ�.
			*/
			HWND GetParentHWND();

			/**
			@brif - �ش� ��ǥ�� ����Ʈ���� �̵� �� �������� ��Ų��.
			@param[in] x - �� ��Ʈ���� ���� ��ġ
			@param[in] y - �� ��Ʈ���� ��� ��ġ
			@param[in] nWidth - �� ��Ʈ���� �ʺ�
			@param[in] nHeight - �� ��Ʈ���� ����
			@param[in] bRepaint - �־��� ���� TRUE�̸�, WM_PAINT �޽����� �߻���Ų��.
			*/
			void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);

			/**
			@brif - �ش� �������� ����Ʈ���� �̵� �� �������� ��Ų��.
			@param[in] lpRect - �̵� �� �������� ��ų ����
			@param[in] bRepaint - �־��� ���� TRUE�̸�, WM_PAINT �޽����� �߻���Ų��.
			*/
			void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);

			/**
			@brif - ���� SSO �� ��Ʈ���� show state�� visible���� �˷��ش�.
			@returns SSO �� ��Ʈ���� show state�� visible�̸� TRUE�� ��ȯ�ϸ�, �ݴ��� ��� FALSE�� ��ȯ�Ѵ�.
			*/
			BOOL IsWindowVisible();

			/**
			@brif - SSO �� ��Ʈ���� show state�� �����Ѵ�.
			@returns �Լ��� ȣ���ϱ����� SSO �� ��Ʈ���� show state�� visible�̸� TRUE�� ��ȯ�ϸ�, �ݴ��� ��� FALSE�� ��ȯ�Ѵ�.
			@param[in] nCmdShow - �����ϰ��� �ϴ� show state ��. (Ex> SW_SHOW, SW_HIDE ��)
			*/
			BOOL ShowWindow(int nCmdShow);

			/**
			@brif - @ref SECBeforeNavigate2 "BeforeNavigate2" �̺�Ʈ �߻��ÿ� ȣ��� �ݹ� �Լ��� �����Ѵ�.<br>
			(BeforeNavigate2�� ���ο� URL�� Navigate �Ǳ� ������ �߻��ϴ� �̺�Ʈ.)
			@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ ���� �����Ѵ�.
			@param[in] pCallback - �̺�Ʈ �߻��ÿ� ȣ��� �ݹ� �Լ� ������.
			@section SECEXAMPLE ���� ����
			���� - @ref PAGEEXAMPLECALLBACK "�� ��Ʈ�� �ݹ� �Լ�" ����
			*/
			BOOL SetCallback_OnBeforeNavigate2(SWEBCALLBACK_ONBN2* pCallback);
			/**
			@brif - @ref SECNavigateComplete2 "NavigateComplete2" �̺�Ʈ �߻��ÿ� ȣ��� �ݹ� �Լ��� �����Ѵ�.<br>
			@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ ���� �����Ѵ�.
			@param[in] pCallback - �̺�Ʈ �߻��ÿ� ȣ��� �ݹ� �Լ� ������.
			@section SECEXAMPLE ���� ����
			���� - @ref PAGEEXAMPLECALLBACK "�� ��Ʈ�� �ݹ� �Լ�" ����
			*/
			BOOL SetCallback_OnNavigateComplete2(SWEBCALLBACK_ONNC2* pCallback);
			/**
			@brif - @ref SECQuit "Quit" �̺�Ʈ �߻��ÿ� ȣ��� �ݹ� �Լ��� �����Ѵ�.<br>
			@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ ���� �����Ѵ�.
			@param[in] pCallback - �̺�Ʈ �߻��ÿ� ȣ��� �ݹ� �Լ� ������.
			@section SECEXAMPLE ���� ����
			���� - @ref PAGEEXAMPLECALLBACK "�� ��Ʈ�� �ݹ� �Լ�" ����
			*/
			BOOL SetCallback_OnQuit(SWEBCALLBACK_ONQUIT* pCallback);

			/**
			@brif - �־��� Dispatch �������̽��� SSO Web ��Ʈ���� �������̽��� �������� Ȯ���Ѵ�.
			@returns SSO Web ��Ʈ���� �������̽��� �����ϸ� TRUE��, �׷��� �ȴٸ� FALSE�� ��ȯ�Ѵ�.
			@param[in] pDisp - SSO Web ��Ʈ���� �������̽��� �������� Ȯ���� Dispatch �������̽�.
			*/
			BOOL IsMe(LPDISPATCH pDisp);
			/**
			@brif - �־��� IID�� �´� �������̽��� ��ȯ�Ѵ�.
			@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ ���� �����Ѵ�.
			@param[in] iid - ������ �������̽� ���̵�(IID)
			@param[out] ppvObject - ������ �������̽� �����͸� ������ ����.
			*/
			BOOL QueryInterface(REFIID iid, void ** ppvObject);

			//////////////////////////////////////////////////////////////////////////
			// ISSOWebEventsUnknown Interface
			/**
			@brif - �־��� �������̽� �����ڿ� �ش��ϴ� �̺�Ʈ �������̽� �����͸� �����´�.
			@note ��� ���������� ���Ǿ����� �Լ��̺�� �ش� �Լ��� �������� ��� ����� �������� �۵��� �ȵȴ�.
			@returns �����Ұ�� TRUE�� �����Ұ�� FALSE�� ��ȯ�Ѵ�.
			@param[in] eEventVer - ���������� �̺�Ʈ �������̽��� �ش��ϴ� ������.
			@param[out] pVal_Out - ���������� �̺�Ʈ �������̽� ����.
			*/
			//virtual BOOL SSOWebEventQueryInterface(ENUM_EventVer eEventVer, void** pVal_Out);

			//////////////////////////////////////////////////////////////////////////
			// ISSOWebEventsVer1 Interface
			/**
			@brif - @ref SECBeforeNavigate2 "BeforeNavigate2" �̺�Ʈ�� �߻��� �� ȣ���.<br>
			@param[in] pDisp - �̺�Ʈ�� �߻��� WebBrowser�� Dispatch �������̽�.
			@param[in] lpszURL - Navigate �� URL
			@param[in] nFlags - ���ĸ� ���� �����.
			@param[in] lpszTargetFrameName - Navigate �� Frame �̸� 
			@param[in] lpszPostedData - Navigate �� URL�� ���޵� Post ������
			@param[in] lpszHeaders - Navigate �� URL�� ���޵� Header ������
			@param[out] pbCancel - Navigate�� ��� ��ų ��� �ش� ���� ������ TRUE�� ����. �׷��� ���� ��� FALSE�� ����.
			@section SECEXAMPLE ���� ����
			���� - @ref PAGEEXAMPLEEVENTOVERRIDE "�� ��Ʈ�� �̺�Ʈ ��� �Լ� ������" ����
			*/
			virtual void OnSSOWebEventBN2(LPDISPATCH pDisp, const char* lpszURL, DWORD nFlags, const char* lpszTargetFrameName, const char* lpszPostedData, const char* lpszHeaders, BOOL* pbCancel);
			/**
			@brif - @ref @ref SECNavigateComplete2 "NavigateComplete2" �̺�Ʈ�� �߻��� �� ȣ���.<br>
			@param[in] pDisp - �̺�Ʈ�� �߻��� WebBrowser�� Dispatch �������̽�.
			@param[in] lpszURL - Navigate�� �Ϸ�� URL
			@section SECEXAMPLE ���� ����
			���� - @ref PAGEEXAMPLEEVENTOVERRIDE "�� ��Ʈ�� �̺�Ʈ ��� �Լ� ������" ����
			*/
			virtual void OnSSOWebEventNC2(LPDISPATCH pDisp, const char* lpszURL);
			/**
			@brif - @ref SECNewWindow2 "NewWindow2" �̺�Ʈ�� �߻��� �� ȣ���.<br>
			@param[out] ppDisp - ��â�� ������ ��ü�� WebBrowser�� Dispatch �������̽�. ��ü���� ���� ��� ���� �������� �ʵ��� �Ѵ�.
			@param[out] Cancel - ��â�� �����°��� ��� ��ų ��� �ش� ���� ������ TRUE�� ����. �׷��� ���� ��� FALSE�� ����.
			@section SECEXAMPLE ���� ����
			���� - @ref PAGEEXAMPLEEVENTOVERRIDE "�� ��Ʈ�� �̺�Ʈ ��� �Լ� ������" ����
			*/
			virtual void OnSSOWebEventNW2(LPDISPATCH *ppDisp, BOOL* Cancel);
			/**
			@brif - @ref @ref SECQuit "Quit" �̺�Ʈ�� �߻��� �� ȣ���.
			@section SECEXAMPLE ���� ����
			���� - @ref PAGEEXAMPLEEVENTOVERRIDE "�� ��Ʈ�� �̺�Ʈ ��� �Լ� ������" ����
			*/
			virtual void OnSSOWebEventQuit();
			//////////////////////////////////////////////////////////////////////////
		};

	}//namespace UTIL

}//namespace NMBASE