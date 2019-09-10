// SSOWebLib.h: interface for the CSSOWebLib class.
//
/// @file	SSOWebLib.h
/// @brief	SSO WebBrowser Library Header
/// @author	Heo Jaemin<judge@cj.net>
/// @date 	2009.07.30
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SSOWEBLIB_H__B6EC3473_CBDF_4B60_8B92_75E4962873EE__INCLUDED_)
#define AFX_SSOWEBLIB_H__B6EC3473_CBDF_4B60_8B92_75E4962873EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <objbase.h>

/// @cond DISABLE_DOXYGEN

#define SUCCEEDED_SSOWEB(x) ( (x) == ERROR_SSOWEB_SUCCESS )
typedef unsigned long	NMSSOWEB_RETCODE;

#ifndef DECLSPEC_NOVTABLE
#if (_MSC_VER >= 1100) && defined(__cplusplus)
#define DECLSPEC_NOVTABLE   __declspec(novtable)
#else
#define DECLSPEC_NOVTABLE
#endif
#endif

//// @endcond

// SSO Web Library �ڵ��߰�.(Library ��δ� ������Ʈ ������ �߰�)
#ifndef _SSOWEBMODULE
	#if defined(_DLL)
		#ifdef _DEBUG
// Debug Multithread Thread DLL 
#define SSOWEBMODULE_NAME	"SSOWebLib_MDd"
		#else
// Multithread Thread DLL
#define SSOWEBMODULE_NAME	"SSOWebLib_MD"
		#endif
	#else
		#ifdef _DEBUG
// Debug Multithread Thread
#define SSOWEBMODULE_NAME	"SSOWebLib_MTd"
		#else
// Multithread Thread
#define SSOWEBMODULE_NAME	"SSOWebLib_MT"
		#endif
	#endif
// ���̺귯�� ��ũ
#pragma comment(lib, SSOWEBMODULE_NAME)
#undef SSOWEBMODULE_NAME
#endif

/**
@defgroup GROUPERRCODE �����ڵ�
@{
*/
/// @brief - SSOWebLib �ݸ��� SSO�������� �����ڵ�
enum ERROR_SSOWEB {
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
/**
@}
*/

//////////////////////////////////////////////////////////////////////
/// @brief - SSO Web URL ��Ʈ�� ���� Ŭ����.
class CSSOWEBURLString
{
public:
	/**
	@brif - SSO Web URL ��Ʈ�� ���� Ŭ���� ������
	@param[in] szData - �ش� Ŭ������ ������ ���ڿ�.
	*/
	CSSOWEBURLString(LPCSTR szData = NULL);
	/// @brif - SSO Web URL ��Ʈ�� ���� Ŭ���� �Ҹ���
	~CSSOWEBURLString();
	
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
protected:
	char*	m_pchData; ///< ���ڿ��� ����� ���� ������
};

//////////////////////////////////////////////////////////////////////
/**
@defgroup GROUPEHOOKFUNC �Լ� Ÿ�� ����
@{
*/

/**
@brif - @ref CSSOWebBrowser::Navigate() �Լ� ȣ��� URL ������ ���� Hook �Լ��� Ÿ�� ����.
@returns ���������� �����Ͽ��� ��쿡 true���� �׷��� ���� ��� false�� ��ȯ�ϵ��� ����.
@param[in] lpszURL - CSSOWebBrowser::Navigate() ȣ��� ���޵� URL ��Ʈ��.
@param[out] strURL_Out - ������ URL ��Ʈ���� ����� ���� Ŭ����.
@section SEC_EXAMPLE �Լ� ���� ����
@code
bool __fastcall MyFuncHookNavigate(const char* lpszURL, CSSOWEBURLString& strURL_Out)
{
	// �߸��� URL ���̸� ����.
	if ( lpszURL == NULL || *lpszURL == '\0' ) 
		return false;
	CString strURL = lpszURL;
	// www.netmarble.net �������� global.netmarble.com ���������� ����.
	strURL.Replace("www.netmarble.net", "global.netmarble.com");
	// ���� ���� ������ �� ����.
	strURL_Out.Assign(strURL);

	return true;
}

@endcode
*/
typedef bool (__fastcall* SWEBHOOKFUNCPTR_NAVIGATE)(const char* lpszURL, CSSOWEBURLString& strURL_Out);

/**
@}
*/

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
/**
@}
*/

//////////////////////////////////////////////////////////////////////
/**
@defgroup GROUPEAPIFUNC API �Լ�
@{
*/
/**
@brif - ���� ����Ʈ�� ���� ��Ű �̸��� �����ϱ� ���Ͽ� ȣ���ϴ� �Լ�.<br>
		�ݸ����� ���� ȣ���� �ʿ䰡 ����.
@note InitSSOWebBrowser() ������ ȣ��Ǿ�� ��.
@param[in] strSiteName - ���������� ��� "daum", �ϳ����������� ��� "hanafos"�� �ѱ�.
*/
void __cdecl PresetSSOWebBrowserSetSiteName(LPCSTR strSiteName);

/**
@brif - SSO �������� DLL�� ��ġ�� ��θ� �������� ����. ȣ������ ���� ��� ���� ������ SSO �������� DLL�� ����.
@note InitSSOWebBrowser() ������ ȣ��Ǿ�� ��
@param[in] strFolder - SSO �������� DLL�� ��ġ�� ���.
*/
void __cdecl PresetSSOWebBrowserBaseFolder(LPCSTR strFolder);

/**
@brif - SSO �������� DLL ������ ���� ���(���ϸ� ����)�� �����Ѵ�.
@param[in] szDLLFullFileName - SSO �������� DLL ������ ���� ���(���ϸ� ����)
*/
void __cdecl SetSSOWebBrowserDLLFileName(LPCSTR szDLLFullFileName);

/**
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
BOOL __cdecl InitSSOWebBrowser(HWND hWndMain, LPCSTR strAuthCookie, LPCSTR strDataCookie, LPCSTR strCpCookie, BOOL bHook = TRUE);

/**
@brif - SSO �������� ��� �ʱ�ȭ �Լ�. (�׽�Ʈ��)<br>SSO �������� DLL�� ��θ� �Է� �޴� �κпܿ� InitSSOWebBrowser()�Լ��� �ٸ� ���� ����.
@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
@param[in] strBaseDir - SSO �������� DLL�� ��ġ�� ���.
@param[in] hWndMain - ���� �������� �ڵ鰪.
@param[in] strAuthCookie - ���� ����Ű ��.
@param[in] strDataCookie - ������ ����Ű ��.
@param[in] strCpCookie - CP ����Ű��.
*/
BOOL __cdecl InitSSOWebBrowser2(LPCSTR strBaseDir, HWND hWndMain, LPCSTR strAuthCookie, LPCSTR strDataCookie, LPCSTR strCpCookie);


/**
@brif - SSO �������� ��� �Ҹ� �Լ�
@note �ݵ�� ���� �����찡 �ı��Ǳ� ���� ȣ���ؾ��Ѵ�.
@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
@section SECEXAMPLE ���� ����
	���� - @ref SECTIONUNINIT "SSO �������� ��� �Ҹ�" ����
*/
BOOL __cdecl UnInitSSOWebBrowser();

/**
@brif - SSO ���������� ó���ؾ� �� �޽����� ��ŷ�Ͽ� ó������ ����.
@returns ���� �ֱٿ� ������ SSO �������� �������� ��ȯ�Ѵ�. ��ȯ�� ������ @ref ERROR_SSOWEB �� �����Ѵ�.
@param[in] bSetHook - �� ���� True�̸� ��ŷ�Ͽ� �޽����� ó���ϰ�, False�̸� ��ŷ�� �ߴ��Ѵ�.
*/
BOOL __cdecl SetHOOKPreTranslateMessage(BOOL bSetHook);

/**
@brif - SSO ���������� ó���ؾ� �� �޽��� ó��(Tab, enterŰ ���۵� ���� �ذ�) - TRUE���� �� ó�� �Ϸ� ��
@note @ref InitSSOWebBrowser API ȣ��ÿ� SSO ���������� ó���ؾ� �� �޽����� ��ŷ ó���Ͽ��ٸ� ȣ�� �ʿ� ����.
@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
@param[in] pMsg - SSO ���������� ó���ؾ� �� �޽��� ����ü.
*/
BOOL __cdecl PreTranslateForSSOWebBrowser(LPMSG pMsg);

/**
@brif - ��Ű Ȱ��ȭ ������ URL�� ��� �Լ�
@returns ��Ű Ȱ��ȭ ������ URL�� ��ȯ�Ѵ�.
*/
LPCSTR __cdecl GetActCookieURL();

/**
@brif - ��Ű �������� ������ URL�� ��� �Լ�
@returns ��Ű �������� ������ URL�� ��ȯ�Ѵ�. 
*/
LPCSTR __cdecl GetRefreshCookieURL();

/**
@brif - ��Ű ������ �������� ��� �Լ�
@returns ��Ű ������ �������� ��ȯ�Ѵ�.
*/
LPCSTR __cdecl GetCookieDomain();

/**
@brif - SSO ���������κ��� ��Ű�� ��� �Լ�<br>
���������� ��ȯ�Ǳ� ������ ���� �״�� put_cookie�ϸ� �ȴ�.
@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
@param[out] pstrAuth - ���� �� ��Ű�� ������ ���� ������.
@param[out] pstrData - ������ �� ��Ű�� ������ ���� ������.
@param[out] pstrCp - CP �� ��Ű�� ������ ���� ������.
*/
BOOL __cdecl GetSSOWebCookieString(LPSTR pstrAuth, LPSTR pstrData, LPSTR pstrCp);
/**
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
BOOL __cdecl GetSSOWebCookieString(LPSTR pszAuth, DWORD cbAuth, LPSTR pszData, DWORD cbData, LPSTR pszCP, DWORD cbCP);
/**
@brif - SSO ���������� ����� ��Ű�� ���̸� ��� �Լ�<br>
@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
@param[out] cbAuth - ���� �� ��Ű�� �����Ҷ� �ʿ��� ������ ũ��
@param[out] cbData - ������ �� ��Ű�� �����Ҷ� �ʿ��� ������ ũ��
@param[out] cbCP - CP �� ��Ű�� �����Ҷ� �ʿ��� ������ ũ��
*/
BOOL __cdecl GetSSOWebCookieStringLength(DWORD& cbAuth, DWORD& cbData, DWORD& cbCP);

/**
@brif - �ݸ��� �� ��Ű(����, ������, CP)�� ����� Internet Explorer â�� �����Ͽ� IDispatch �����͸� ��ȯ�Ѵ�.
@note ������ ������ Internet Explorerâ�� ���μ����� ���� ��� �ش� ���μ����� â�� �߰��ȴ�.
@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
@param[out] ppDisp - ���� ������ Internet Explorer â�� IDispatch ������.
*/
BOOL __cdecl SSOWEBLib_CreateNewIEDispatch(IDispatch** ppDisp);
/**
@brif - �ݸ��� �� ��Ű(����, ������, CP)�� ����� Internet Explorer â�� �����Ͽ� ������ URL�� Navigate��Ų��.
@note ������ ������ Internet Explorerâ�� ���μ����� ���� ��� �ش� ���μ����� â�� �߰��ȴ�.
@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
@param[in] szURL - �ű� ������ Internet Explorer���� Navigate ��ų URL.
*/
BOOL __cdecl SSOWEBLib_OpenInternetExplorer(LPCSTR szURL);

/**
@brif - ���� �ֱٿ� ������ SSO �������� ���� �ڵ�(NMSSOWEB_RETCODE)�� �����´�.
@returns ���� �ֱٿ� ������ SSO �������� �������� ��ȯ�Ѵ�. ��ȯ�� ������ @ref ERROR_SSOWEB �� �����Ѵ�.
*/
NMSSOWEB_RETCODE __cdecl SSOWebLib_GetLastError();
/**
@brif - ���� �ֱٿ� ������ SSO �������� ���� �ڵ�(NMSSOWEB_RETCODE)�� ���ڿ� �ڵ�� �����´�.
@returns ���� �Ͽ��� ��� @ref ERROR_SSOWEB_SUCCESS �� ��ȯ�ϸ�, ������ ��� @ref ERROR_SSOWEB_SUCCESS ���� @ref ERROR_SSOWEB ���� ��ȯ�Ѵ�.
@param[out] szBuffer - SSO �������� ���� �ڵ带 ���ڿ� �ڵ�� ������ ���� ������.
@param[in] cbSize - SSO �������� ���� �ڵ带 ���ڿ� ���·� ������ ������ ũ��.
*/
NMSSOWEB_RETCODE __cdecl SSOWebLib_GetLastError(LPSTR szBuffer, size_t cbSize);
/**
@brif - �־��� SSO �������� ���� �ڵ带 ���ڿ� ���·� ��ȯ�Ѵ�.
@returns ���� �Ͽ��� ��� @ref ERROR_SSOWEB_SUCCESS �� ��ȯ�ϸ�, ������ ��� @ref ERROR_SSOWEB_SUCCESS ���� @ref ERROR_SSOWEB ���� ��ȯ�Ѵ�.
@param[in] hError - ���ڿ� �ڵ�� ��ȯ�� SSO �������� ���� �ڵ�.
@param[out] szBuffer - �־��� SSO �������� ���� �ڵ带 ���ڿ� �ڵ�� ������ ���� ������.
@param[in] cbSize - �־��� SSO �������� ���� �ڵ带 ���ڿ� ���·� ������ ������ ũ��.
*/
NMSSOWEB_RETCODE __cdecl SSOWebLib_ErrorCode2String(NMSSOWEB_RETCODE hError, LPSTR szBuffer, size_t cbSize);

/**
@brif - @ref CSSOWebBrowser::Navigate() �Լ� ȣ��� URL ������ ���� Hook �Լ��� �����Ѵ�.
@returns ���� �Ͽ��� ��� @ref ERROR_SSOWEB_SUCCESS �� ��ȯ�ϸ�, ������ ��� @ref ERROR_SSOWEB_SUCCESS ���� @ref ERROR_SSOWEB ���� ��ȯ�Ѵ�.
@param[in] pFunc - CSSOWebBrowser::Navigate() �Լ� ȣ��� URL ������ ó���� �Լ��� ������.
@note �ش� �Լ��� ��ŷ �Լ��� ������ ��� ��� CSSOWebBrowser Ŭ������ ����ȴ�.
*/
NMSSOWEB_RETCODE __cdecl SSOWebLib_SetHookFuncForNavigate(SWEBHOOKFUNCPTR_NAVIGATE pFunc);

/**
@}
*/
/// @cond DISABLE_DOXYGEN
extern "C++"
{
	//////////////////////////////////////////////////////////////////////
	/// @brief - SSO �� �̺�Ʈ �ڵ鷯 Ŭ���� ���� (��� ���������� ���)
	class DECLSPEC_NOVTABLE ISSOWebEventsUnknown
	{
	public:
		enum ENUM_EventVer {
			ENUM_EventVer1 = 1,
		};
		/// @cond DISABLE_DOXYGEN
		BEGIN_INTERFACE
		/// @endcond

		/**
		@brif - �־��� �������̽� �����ڿ� �ش��ϴ� �̺�Ʈ �������̽� �����͸� �����´�.
		@note ��� ���������� ���Ǿ����� �Լ��̺�� �ش� �Լ��� �������� ��� ����� �������� �۵��� �ȵȴ�.
		@returns �����Ұ�� TRUE�� �����Ұ�� FALSE�� ��ȯ�Ѵ�.
		@param[in] eEventVer - ���������� �̺�Ʈ �������̽��� �ش��ϴ� ������.
		@param[out] pVal_Out - ���������� �̺�Ʈ �������̽� ����.
		*/
		virtual BOOL __stdcall SSOWebEventQueryInterface(ENUM_EventVer eEventVer, void** pVal_Out) = 0;

		/// @cond DISABLE_DOXYGEN
		END_INTERFACE
		/// @endcond
	};

	//////////////////////////////////////////////////////////////////////
	/// @brief - SSO �� �̺�Ʈ �ڵ鷯 Ŭ���� (Version 1)
	class DECLSPEC_NOVTABLE ISSOWebEventsVer1 : public ISSOWebEventsUnknown
	{
	public:
		/// @cond DISABLE_DOXYGEN
		BEGIN_INTERFACE
		/// @endcond

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
		virtual void __stdcall OnSSOWebEventBN2(LPDISPATCH pDisp, const char* lpszURL, DWORD nFlags, const char* lpszTargetFrameName, const char* lpszPostedData, const char* lpszHeaders, BOOL* pbCancel) = 0;
		/**
		@brif - @ref @ref SECNavigateComplete2 "NavigateComplete2" �̺�Ʈ�� �߻��� �� ȣ���.<br>
		@param[in] pDisp - �̺�Ʈ�� �߻��� WebBrowser�� Dispatch �������̽�.
		@param[in] lpszURL - Navigate�� �Ϸ�� URL
		@section SECEXAMPLE ���� ����
		���� - @ref PAGEEXAMPLEEVENTOVERRIDE "�� ��Ʈ�� �̺�Ʈ ��� �Լ� ������" ����
		*/
		virtual void __stdcall OnSSOWebEventNC2(LPDISPATCH pDisp, const char* lpszURL) = 0;
		/**
		@brif - @ref SECNewWindow2 "NewWindow2" �̺�Ʈ�� �߻��� �� ȣ���.<br>
		@param[out] ppDisp - ��â�� ������ ��ü�� WebBrowser�� Dispatch �������̽�. ��ü���� ���� ��� ���� �������� �ʵ��� �Ѵ�.
		@param[out] Cancel - ��â�� �����°��� ��� ��ų ��� �ش� ���� ������ TRUE�� ����. �׷��� ���� ��� FALSE�� ����.
		@section SECEXAMPLE ���� ����
		���� - @ref PAGEEXAMPLEEVENTOVERRIDE "�� ��Ʈ�� �̺�Ʈ ��� �Լ� ������" ����
		*/
		virtual void __stdcall OnSSOWebEventNW2(LPDISPATCH *ppDisp, BOOL* Cancel) = 0;
		/**
		@brif - @ref @ref SECQuit "Quit" �̺�Ʈ�� �߻��� �� ȣ���.
		@section SECEXAMPLE ���� ����
		���� - @ref PAGEEXAMPLEEVENTOVERRIDE "�� ��Ʈ�� �̺�Ʈ ��� �Լ� ������" ����
		*/
		virtual void __stdcall OnSSOWebEventQuit() = 0;

		/// @cond DISABLE_DOXYGEN
		END_INTERFACE
		/// @endcond
	};
}
/// @endcond
//////////////////////////////////////////////////////////////////////
/// @brief - SSO �� ��Ʈ�� Ŭ����
class CSSOWebBrowser : public ISSOWebEventsVer1
{
public:
	/**
	@brif - SSO �� ��Ʈ�� Ŭ���� ������
	*/
	CSSOWebBrowser();
	/**
	@brif - SSO �� ��Ʈ�� Ŭ���� �Ҹ���
	*/
	~CSSOWebBrowser();

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
	BOOL	__cdecl Create(DWORD dwStyle, RECT& rect, HWND hWndParent, UINT nID, LPDISPATCH lpExternalDisp = NULL);
	/**
	@brif - SSO �� ��Ʈ�� �ı�
	@section SECEXAMPLE ���� ����
		���� - @ref SECTIONDESTROY "�� ��Ʈ�� �ı�" ����
	*/
	void	__cdecl Destroy();

	/**
	@brif - �ش� ��Ʈ�ѿ��� ���������� Navigate �Ǵ� ���ȿ� ȭ��� �ڵ����� ����� �ɼ��� �����Ѵ�.
	@param[in] bEnable - �ɼ��� Ȱ��ȭ�Ұܿ쿡�� TRUE��, ��Ȱ��ȭ �� ��쿡�� FALSE�� �Է��Ѵ�.
	*/
	void	__cdecl SetAutoHide(BOOL bEnable);
	/**
	@brif - �ش� ��Ʈ�ѿ��� ���������� Navigate �ɶ� Local URL �����丮���� ����� �ɼ��� �����Ѵ�.<br>
			�ش� �ɼ��� ������ ��� HTML������ GoFoward()�� GoBack()�� �۵����� �ʴ´�.
	@note �ش� �ɼ��� �����Ǵ��� Global URL �����丮(��� ������)���� ������ ���´�.(IE ���Ȼ� ����)
	@param[in] bEnable - �ɼ��� Ȱ��ȭ�Ұܿ쿡�� TRUE��, ��Ȱ��ȭ �� ��쿡�� FALSE�� �Է��Ѵ�.
	*/
	void	__cdecl SetNavNoHistory(BOOL bEnable);
	/**
	@brif - �ش� ��Ʈ�ѿ��� ���������� ���� ��, Scroll�ٸ� �ڵ����� �����ϴ� �ɼ��� �����Ѵ�.
	@param[in] bEnable - �ɼ��� Ȱ��ȭ�Ұܿ쿡�� TRUE��, ��Ȱ��ȭ �� ��쿡�� FALSE�� �Է��Ѵ�.
	*/
	void	__cdecl SetHideScroll(BOOL bEnable);
	/**
	@brif - �ش� ��Ʈ�ѿ��� ���������� ���� ��, Border�� �ڵ����� �����ϴ� �ɼ��� �����Ѵ�.
	@param[in] bEnable - �ɼ��� Ȱ��ȭ�Ұܿ쿡�� TRUE��, ��Ȱ��ȭ �� ��쿡�� FALSE�� �Է��Ѵ�.
	*/
	void	__cdecl SetHideBorder(BOOL bEnable);

	/**
	@brif - ���� ���� HTML �������� ��� ���ڿ� ���ڵ� ������ �Ѵ�. 
	@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
	@param[in] strCharset - ������ ��� ���ڿ� ���ڵ� �ڵ�.  (���ڵ� �ڵ� �� euc-kr, shift-jis)
	*/
	BOOL	__cdecl PutCharSet(LPCSTR strCharset);
	/**
	@brif - ���� ���� HTML ������������ �־��� HTML �ڵ带 �߰��Ѵ�.
	@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
	@param[in] strHtml - 
	*/
	BOOL	__cdecl InsertHTML(LPCSTR strHtml);
	/**
	@brif - �־��� �ڹ� ��ũ��Ʈ �ڵ带 �����Ѵ�.
	@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
	@param[in] strScript - ������ �ڹ� ��ũ��Ʈ �ڵ�.
	*/
	BOOL	__cdecl ExecJavascript(LPCSTR strScript);

	/**
	@brif - �־��� URL�� Navigate��Ų��.
	@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
	@param[in] strURL - Navigate ��ų URL.
	*/
	BOOL	__cdecl Navigate(LPCSTR strURL);

	/**
	@brif - �־��� ���ҽ� ���̵�� Navigate ��Ų��.
	@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
	@param[in] uResID - Navigate ��ų HTML ���ҽ��� ���̵�
	*/
	BOOL	__cdecl NavigateFromRes(UINT uResID, HINSTANCE hResInstance = NULL);
	/**
	@brif - �־��� ���ҽ� ���̵�� Navigate ��Ų��.
	@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
	@param[in] szResID - Navigate ��ų HTML ���ҽ��� ���̵�
	*/
	BOOL	__cdecl NavigateFromRes(LPCSTR szResID, HINSTANCE hResInstance = NULL);

	/**
	@brif - ���� SSO �� ��Ʈ�ѿ��� Navigate �� Ƚ���� �����´�.
	@note ����� �������� �ʾ���.
	@returns Navigate �� Ƚ���� ��ȯ�Ѵ�. 
	*/
	int		__cdecl GetUserNavigatedCount();
	/**
	@brif - �־��� ������ Navigate �� Ƚ���� �����Ѵ�.
	@note ����� �������� �ʾ���.
	@param[in] count - Navigate �� Ƚ���� ������ ��.
	*/
	void	__cdecl SetUserNavigatedCount(int count);
	/**
	@brif - Navigate �� Ƚ���� �ʱ�ȭ�Ѵ�.
	@note ����� �������� �ʾ���.
	*/
	void	__cdecl ResetUserNavigatedCount();

	/**
	@brif - SSO �� ��Ʈ�� ������ �ڵ鰪�� �����´�.
	@returns SSO �� ��Ʈ�� ������ �ڵ鰪�� ��ȯ�Ѵ�. 
	*/
	HWND	__cdecl GetHWND();

	/**
	@brif - SSO �� ��Ʈ�� �θ� ������ �ڵ鰪�� �����´�.
	@returns SSO �� ��Ʈ�� �θ� ������ �ڵ鰪�� ��ȯ�Ѵ�.
	*/
	HWND	__cdecl GetParentHWND();

	/**
	@brif - �ش� ��ǥ�� ����Ʈ���� �̵� �� �������� ��Ų��.
	@param[in] x - �� ��Ʈ���� ���� ��ġ
	@param[in] y - �� ��Ʈ���� ��� ��ġ
	@param[in] nWidth - �� ��Ʈ���� �ʺ�
	@param[in] nHeight - �� ��Ʈ���� ����
	@param[in] bRepaint - �־��� ���� TRUE�̸�, WM_PAINT �޽����� �߻���Ų��.
	*/
	void	__cdecl MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);

	/**
	@brif - �ش� �������� ����Ʈ���� �̵� �� �������� ��Ų��.
	@param[in] lpRect - �̵� �� �������� ��ų ����
	@param[in] bRepaint - �־��� ���� TRUE�̸�, WM_PAINT �޽����� �߻���Ų��.
	*/
	void	__cdecl MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);

	/**
	@brif - ���� SSO �� ��Ʈ���� show state�� visible���� �˷��ش�.
	@returns SSO �� ��Ʈ���� show state�� visible�̸� TRUE�� ��ȯ�ϸ�, �ݴ��� ��� FALSE�� ��ȯ�Ѵ�.
	*/
	BOOL	__cdecl IsWindowVisible();

	/**
	@brif - SSO �� ��Ʈ���� show state�� �����Ѵ�.
	@returns �Լ��� ȣ���ϱ����� SSO �� ��Ʈ���� show state�� visible�̸� TRUE�� ��ȯ�ϸ�, �ݴ��� ��� FALSE�� ��ȯ�Ѵ�.
	@param[in] nCmdShow - �����ϰ��� �ϴ� show state ��. (Ex> SW_SHOW, SW_HIDE ��)
	*/
	BOOL	__cdecl ShowWindow(int nCmdShow);

	/**
	@brif - @ref SECBeforeNavigate2 "BeforeNavigate2" �̺�Ʈ �߻��ÿ� ȣ��� �ݹ� �Լ��� �����Ѵ�.<br>
			(BeforeNavigate2�� ���ο� URL�� Navigate �Ǳ� ������ �߻��ϴ� �̺�Ʈ.)
	@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ ���� �����Ѵ�.
	@param[in] pCallback - �̺�Ʈ �߻��ÿ� ȣ��� �ݹ� �Լ� ������.
	@section SECEXAMPLE ���� ����
		���� - @ref PAGEEXAMPLECALLBACK "�� ��Ʈ�� �ݹ� �Լ�" ����
	*/
	BOOL	__cdecl SetCallback_OnBeforeNavigate2(SWEBCALLBACK_ONBN2* pCallback);
	/**
	@brif - @ref SECNavigateComplete2 "NavigateComplete2" �̺�Ʈ �߻��ÿ� ȣ��� �ݹ� �Լ��� �����Ѵ�.<br>
	@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ ���� �����Ѵ�.
	@param[in] pCallback - �̺�Ʈ �߻��ÿ� ȣ��� �ݹ� �Լ� ������.
	@section SECEXAMPLE ���� ����
		���� - @ref PAGEEXAMPLECALLBACK "�� ��Ʈ�� �ݹ� �Լ�" ����
	*/
	BOOL	__cdecl SetCallback_OnNavigateComplete2(SWEBCALLBACK_ONNC2* pCallback);
	/**
	@brif - @ref SECQuit "Quit" �̺�Ʈ �߻��ÿ� ȣ��� �ݹ� �Լ��� �����Ѵ�.<br>
	@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ ���� �����Ѵ�.
	@param[in] pCallback - �̺�Ʈ �߻��ÿ� ȣ��� �ݹ� �Լ� ������.
	@section SECEXAMPLE ���� ����
		���� - @ref PAGEEXAMPLECALLBACK "�� ��Ʈ�� �ݹ� �Լ�" ����
	*/
	BOOL	__cdecl SetCallback_OnQuit(SWEBCALLBACK_ONQUIT* pCallback);

	/**
	@brif - �־��� Dispatch �������̽��� SSO Web ��Ʈ���� �������̽��� �������� Ȯ���Ѵ�.
	@returns SSO Web ��Ʈ���� �������̽��� �����ϸ� TRUE��, �׷��� �ȴٸ� FALSE�� ��ȯ�Ѵ�.
	@param[in] pDisp - SSO Web ��Ʈ���� �������̽��� �������� Ȯ���� Dispatch �������̽�.
	*/
	BOOL	__cdecl IsMe(LPDISPATCH pDisp);
	/**
	@brif - �־��� IID�� �´� �������̽��� ��ȯ�Ѵ�.
	@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
			���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ ���� �����Ѵ�.
	@param[in] iid - ������ �������̽� ���̵�(IID)
	@param[out] ppvObject - ������ �������̽� �����͸� ������ ����.
	*/
	BOOL	__cdecl QueryInterface(REFIID iid, void ** ppvObject);

	//////////////////////////////////////////////////////////////////////////
	// ISSOWebEventsUnknown Interface
	/**
		@brif - �־��� �������̽� �����ڿ� �ش��ϴ� �̺�Ʈ �������̽� �����͸� �����´�.
		@note ��� ���������� ���Ǿ����� �Լ��̺�� �ش� �Լ��� �������� ��� ����� �������� �۵��� �ȵȴ�.
		@returns �����Ұ�� TRUE�� �����Ұ�� FALSE�� ��ȯ�Ѵ�.
		@param[in] eEventVer - ���������� �̺�Ʈ �������̽��� �ش��ϴ� ������.
		@param[out] pVal_Out - ���������� �̺�Ʈ �������̽� ����.
	*/
	virtual BOOL __stdcall SSOWebEventQueryInterface(ENUM_EventVer eEventVer, void** pVal_Out);

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
	virtual void __stdcall OnSSOWebEventBN2(LPDISPATCH pDisp, const char* lpszURL, DWORD nFlags, const char* lpszTargetFrameName, const char* lpszPostedData, const char* lpszHeaders, BOOL* pbCancel);
	/**
		@brif - @ref @ref SECNavigateComplete2 "NavigateComplete2" �̺�Ʈ�� �߻��� �� ȣ���.<br>
		@param[in] pDisp - �̺�Ʈ�� �߻��� WebBrowser�� Dispatch �������̽�.
		@param[in] lpszURL - Navigate�� �Ϸ�� URL
		@section SECEXAMPLE ���� ����
		���� - @ref PAGEEXAMPLEEVENTOVERRIDE "�� ��Ʈ�� �̺�Ʈ ��� �Լ� ������" ����
		*/
	virtual void __stdcall OnSSOWebEventNC2(LPDISPATCH pDisp, const char* lpszURL);
	/**
		@brif - @ref SECNewWindow2 "NewWindow2" �̺�Ʈ�� �߻��� �� ȣ���.<br>
		@param[out] ppDisp - ��â�� ������ ��ü�� WebBrowser�� Dispatch �������̽�. ��ü���� ���� ��� ���� �������� �ʵ��� �Ѵ�.
		@param[out] Cancel - ��â�� �����°��� ��� ��ų ��� �ش� ���� ������ TRUE�� ����. �׷��� ���� ��� FALSE�� ����.
		@section SECEXAMPLE ���� ����
		���� - @ref PAGEEXAMPLEEVENTOVERRIDE "�� ��Ʈ�� �̺�Ʈ ��� �Լ� ������" ����
	*/
	virtual void __stdcall OnSSOWebEventNW2(LPDISPATCH *ppDisp, BOOL* Cancel);
	/**
		@brif - @ref @ref SECQuit "Quit" �̺�Ʈ�� �߻��� �� ȣ���.
		@section SECEXAMPLE ���� ����
		���� - @ref PAGEEXAMPLEEVENTOVERRIDE "�� ��Ʈ�� �̺�Ʈ ��� �Լ� ������" ����
	*/
	virtual void __stdcall OnSSOWebEventQuit();
	//////////////////////////////////////////////////////////////////////////
protected:
	/**
	@brif - SSO �� ��Ʈ�� ������ �ڵ鰪
	*/
	HWND	m_hwndWeb;
};

#endif // !defined(AFX_SSOWEBLIB_H__B6EC3473_CBDF_4B60_8B92_75E4962873EE__INCLUDED_)
