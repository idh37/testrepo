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
		@brif - ����� ���ڿ� �����͸� �����´�.
		@returns ����� ���ڿ� �����͸� ��ȯ�Ѵ�.
		*/
		LPCSTR CNMSSOWEBURLString::GetData()
		{
			return m_pURL->GetData();
		}

		/**
		@brif - �־��� ���ڿ��� �����Ѵ�.
		@param[in] szData - �ش� Ŭ������ ������ ���ڿ�.
		*/
		void CNMSSOWEBURLString::Assign(LPCSTR szData)
		{
			m_pURL->Assign(szData);
		}

		/**
		@brif - ���ڿ� �����͸� Clear�Ѵ�.
		*/
		void CNMSSOWEBURLString::Clear()
		{
			m_pURL->Clear();
		}

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
		void NMPresetSSOWebBrowserSetSiteName(LPCSTR strSiteName)
		{
			PresetSSOWebBrowserSetSiteName(strSiteName);
		}

		/*
		@brif - SSO �������� DLL�� ��ġ�� ��θ� �������� ����. ȣ������ ���� ��� ���� ������ SSO �������� DLL�� ����.
		@note InitSSOWebBrowser() ������ ȣ��Ǿ�� ��
		@param[in] strFolder - SSO �������� DLL�� ��ġ�� ���.
		*/
		void NMPresetSSOWebBrowserBaseFolder(LPCSTR strFolder)
		{
			PresetSSOWebBrowserBaseFolder(strFolder);
		}

		/*
		@brif - SSO �������� DLL ������ ���� ���(���ϸ� ����)�� �����Ѵ�.
		@param[in] szDLLFullFileName - SSO �������� DLL ������ ���� ���(���ϸ� ����)
		*/
		//void NMSetSSOWebBrowserDLLFileName(LPCSTR szDLLFullFileName);
		//{
		//	SetSSOWebBrowserDLLFileName(szDLLFullFileName);
		//}

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
		BOOL NMInitSSOWebBrowser(HWND hWndMain, LPCSTR strAuthCookie, LPCSTR strDataCookie, LPCSTR strCpCookie, BOOL bHook/* = TRUE*/)
		{
			return InitSSOWebBrowser(hWndMain, strAuthCookie, strDataCookie, strCpCookie, bHook);
		}

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
		BOOL NMInitSSOWebBrowser2(LPCSTR strBaseDir, HWND hWndMain, LPCSTR strAuthCookie, LPCSTR strDataCookie, LPCSTR strCpCookie)
		{
			return InitSSOWebBrowser2(strBaseDir, hWndMain, strAuthCookie, strDataCookie, strCpCookie);
		}

		/*
		@brif - SSO �������� ��� �Ҹ� �Լ�
		@note �ݵ�� ���� �����찡 �ı��Ǳ� ���� ȣ���ؾ��Ѵ�.
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@section SECEXAMPLE ���� ����
		���� - @ref SECTIONUNINIT "SSO �������� ��� �Ҹ�" ����
		*/
		BOOL NMUnInitSSOWebBrowser()
		{
			return UnInitSSOWebBrowser();
		}

		/*
		@brif - SSO ���������� ó���ؾ� �� �޽����� ��ŷ�Ͽ� ó������ ����.
		@returns ���� �ֱٿ� ������ SSO �������� �������� ��ȯ�Ѵ�. ��ȯ�� ������ @ref ERROR_SSOWEB �� �����Ѵ�.
		@param[in] bSetHook - �� ���� True�̸� ��ŷ�Ͽ� �޽����� ó���ϰ�, False�̸� ��ŷ�� �ߴ��Ѵ�.
		*/
		BOOL NMSetHOOKPreTranslateMessage(BOOL bSetHook)
		{
			return SetHOOKPreTranslateMessage(bSetHook);
		}

		/*
		@brif - SSO ���������� ó���ؾ� �� �޽��� ó��(Tab, enterŰ ���۵� ���� �ذ�) - TRUE���� �� ó�� �Ϸ� ��
		@note @ref InitSSOWebBrowser API ȣ��ÿ� SSO ���������� ó���ؾ� �� �޽����� ��ŷ ó���Ͽ��ٸ� ȣ�� �ʿ� ����.
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@param[in] pMsg - SSO ���������� ó���ؾ� �� �޽��� ����ü.
		*/
		BOOL NMPreTranslateForSSOWebBrowser(LPMSG pMsg)
		{
			return PreTranslateForSSOWebBrowser(pMsg);
		}

		/*
		@brif - ��Ű Ȱ��ȭ ������ URL�� ��� �Լ�
		@returns ��Ű Ȱ��ȭ ������ URL�� ��ȯ�Ѵ�.
		*/
		LPCSTR NMGetActCookieURL()
		{
			return GetActCookieURL();
		}

		/*
		@brif - ��Ű �������� ������ URL�� ��� �Լ�
		@returns ��Ű �������� ������ URL�� ��ȯ�Ѵ�. 
		*/
		LPCSTR NMGetRefreshCookieURL()
		{
			return GetRefreshCookieURL();
		}

		/*
		@brif - ��Ű ������ �������� ��� �Լ�
		@returns ��Ű ������ �������� ��ȯ�Ѵ�.
		*/
		LPCSTR NMGetCookieDomain()
		{
			return GetCookieDomain();
		}

		/*
		@brif - SSO ���������κ��� ��Ű�� ��� �Լ�<br>
		���������� ��ȯ�Ǳ� ������ ���� �״�� put_cookie�ϸ� �ȴ�.
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@param[out] pstrAuth - ���� �� ��Ű�� ������ ���� ������.
		@param[out] pstrData - ������ �� ��Ű�� ������ ���� ������.
		@param[out] pstrCp - CP �� ��Ű�� ������ ���� ������.
		*/
		BOOL NMGetSSOWebCookieString(LPSTR pstrAuth, LPSTR pstrData, LPSTR pstrCp)
		{
			return GetSSOWebCookieString(pstrAuth, pstrData, pstrCp);
		}

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
		BOOL NMGetSSOWebCookieString(LPSTR pszAuth, DWORD cbAuth, LPSTR pszData, DWORD cbData, LPSTR pszCP, DWORD cbCP)
		{
			return GetSSOWebCookieString(pszAuth, cbAuth, pszData, cbData, pszCP, cbCP);
		}

		/*
		@brif - SSO ���������� ����� ��Ű�� ���̸� ��� �Լ�<br>
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@param[out] cbAuth - ���� �� ��Ű�� �����Ҷ� �ʿ��� ������ ũ��
		@param[out] cbData - ������ �� ��Ű�� �����Ҷ� �ʿ��� ������ ũ��
		@param[out] cbCP - CP �� ��Ű�� �����Ҷ� �ʿ��� ������ ũ��
		*/
		BOOL NMGetSSOWebCookieStringLength(DWORD& cbAuth, DWORD& cbData, DWORD& cbCP)
		{
			return GetSSOWebCookieStringLength(cbAuth, cbData, cbCP);
		}

		/*
		@brif - �ݸ��� �� ��Ű(����, ������, CP)�� ����� Internet Explorer â�� �����Ͽ� IDispatch �����͸� ��ȯ�Ѵ�.
		@note ������ ������ Internet Explorerâ�� ���μ����� ���� ��� �ش� ���μ����� â�� �߰��ȴ�.
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@param[out] ppDisp - ���� ������ Internet Explorer â�� IDispatch ������.
		*/
		BOOL NMSSOWEBLib_CreateNewIEDispatch(IDispatch** ppDisp)
		{
			return SSOWEBLib_CreateNewIEDispatch(ppDisp);
		}

		/*
		@brif - �ݸ��� �� ��Ű(����, ������, CP)�� ����� Internet Explorer â�� �����Ͽ� ������ URL�� Navigate��Ų��.
		@note ������ ������ Internet Explorerâ�� ���μ����� ���� ��� �ش� ���μ����� â�� �߰��ȴ�.
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@param[in] szURL - �ű� ������ Internet Explorer���� Navigate ��ų URL.
		*/
		BOOL NMSSOWEBLib_OpenInternetExplorer(LPCSTR szURL)
		{
			return SSOWEBLib_OpenInternetExplorer(szURL);
		}

		/*
		@brif - ���� �ֱٿ� ������ SSO �������� ���� �ڵ�(NMSSOWEB_RETCODE)�� �����´�.
		@returns ���� �ֱٿ� ������ SSO �������� �������� ��ȯ�Ѵ�. ��ȯ�� ������ @ref ERROR_SSOWEB �� �����Ѵ�.
		*/
		NMSSOWEB_RETCODE NMSSOWebLib_GetLastError()
		{
			return SSOWebLib_GetLastError();
		}

		/*
		@brif - ���� �ֱٿ� ������ SSO �������� ���� �ڵ�(NMSSOWEB_RETCODE)�� ���ڿ� �ڵ�� �����´�.
		@returns ���� �Ͽ��� ��� @ref ERROR_SSOWEB_SUCCESS �� ��ȯ�ϸ�, ������ ��� @ref ERROR_SSOWEB_SUCCESS ���� @ref ERROR_SSOWEB ���� ��ȯ�Ѵ�.
		@param[out] szBuffer - SSO �������� ���� �ڵ带 ���ڿ� �ڵ�� ������ ���� ������.
		@param[in] cbSize - SSO �������� ���� �ڵ带 ���ڿ� ���·� ������ ������ ũ��.
		*/
		NMSSOWEB_RETCODE NMSSOWebLib_GetLastError(LPSTR szBuffer, size_t cbSize)
		{
			UNREFERENCED_PARAMETER( szBuffer );
			UNREFERENCED_PARAMETER( cbSize );

			return SSOWebLib_GetLastError();
		}

		/*
		@brif - �־��� SSO �������� ���� �ڵ带 ���ڿ� ���·� ��ȯ�Ѵ�.
		@returns ���� �Ͽ��� ��� @ref ERROR_SSOWEB_SUCCESS �� ��ȯ�ϸ�, ������ ��� @ref ERROR_SSOWEB_SUCCESS ���� @ref ERROR_SSOWEB ���� ��ȯ�Ѵ�.
		@param[in] hError - ���ڿ� �ڵ�� ��ȯ�� SSO �������� ���� �ڵ�.
		@param[out] szBuffer - �־��� SSO �������� ���� �ڵ带 ���ڿ� �ڵ�� ������ ���� ������.
		@param[in] cbSize - �־��� SSO �������� ���� �ڵ带 ���ڿ� ���·� ������ ������ ũ��.
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
		@brif - @ref CSSOWebBrowser::Navigate() �Լ� ȣ��� URL ������ ���� Hook �Լ��� �����Ѵ�.
		@returns ���� �Ͽ��� ��� @ref ERROR_SSOWEB_SUCCESS �� ��ȯ�ϸ�, ������ ��� @ref ERROR_SSOWEB_SUCCESS ���� @ref ERROR_SSOWEB ���� ��ȯ�Ѵ�.
		@param[in] pFunc - CSSOWebBrowser::Navigate() �Լ� ȣ��� URL ������ ó���� �Լ��� ������.
		@note �ش� �Լ��� ��ŷ �Լ��� ������ ��� ��� CSSOWebBrowser Ŭ������ ����ȴ�.
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
		BOOL CNMSSOWebBrowser::Create(DWORD dwStyle, RECT& rect, HWND hWndParent, UINT nID, LPDISPATCH lpExternalDisp/* = NULL*/)
		{
			return m_pBrowser->Create(dwStyle, rect, hWndParent, nID, lpExternalDisp);
		}

		/**
		@brif - SSO �� ��Ʈ�� �ı�
		@section SECEXAMPLE ���� ����
		���� - @ref SECTIONDESTROY "�� ��Ʈ�� �ı�" ����
		*/
		void CNMSSOWebBrowser::Destroy()
		{
			m_pBrowser->Destroy();
		}

		/**
		@brif - �ش� ��Ʈ�ѿ��� ���������� Navigate �Ǵ� ���ȿ� ȭ��� �ڵ����� ����� �ɼ��� �����Ѵ�.
		@param[in] bEnable - �ɼ��� Ȱ��ȭ�Ұܿ쿡�� TRUE��, ��Ȱ��ȭ �� ��쿡�� FALSE�� �Է��Ѵ�.
		*/
		void CNMSSOWebBrowser::SetAutoHide(BOOL bEnable)
		{
			m_pBrowser->SetAutoHide(bEnable);
		}

		/**
		@brif - �ش� ��Ʈ�ѿ��� ���������� Navigate �ɶ� Local URL �����丮���� ����� �ɼ��� �����Ѵ�.<br>
		�ش� �ɼ��� ������ ��� HTML������ GoFoward()�� GoBack()�� �۵����� �ʴ´�.
		@note �ش� �ɼ��� �����Ǵ��� Global URL �����丮(��� ������)���� ������ ���´�.(IE ���Ȼ� ����)
		@param[in] bEnable - �ɼ��� Ȱ��ȭ�Ұܿ쿡�� TRUE��, ��Ȱ��ȭ �� ��쿡�� FALSE�� �Է��Ѵ�.
		*/
		void CNMSSOWebBrowser::SetNavNoHistory(BOOL bEnable)
		{
			m_pBrowser->SetNavNoHistory(bEnable);
		}

		/**
		@brif - �ش� ��Ʈ�ѿ��� ���������� ���� ��, Scroll�ٸ� �ڵ����� �����ϴ� �ɼ��� �����Ѵ�.
		@param[in] bEnable - �ɼ��� Ȱ��ȭ�Ұܿ쿡�� TRUE��, ��Ȱ��ȭ �� ��쿡�� FALSE�� �Է��Ѵ�.
		*/
		void CNMSSOWebBrowser::SetHideScroll(BOOL bEnable)
		{
			m_pBrowser->SetNavNoHistory(bEnable);
		}

		/**
		@brif - �ش� ��Ʈ�ѿ��� ���������� ���� ��, Border�� �ڵ����� �����ϴ� �ɼ��� �����Ѵ�.
		@param[in] bEnable - �ɼ��� Ȱ��ȭ�Ұܿ쿡�� TRUE��, ��Ȱ��ȭ �� ��쿡�� FALSE�� �Է��Ѵ�.
		*/
		void CNMSSOWebBrowser::SetHideBorder(BOOL bEnable)
		{
			m_pBrowser->SetHideBorder(bEnable);
		}

		/**
		@brif - ���� ���� HTML �������� ��� ���ڿ� ���ڵ� ������ �Ѵ�. 
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@param[in] strCharset - ������ ��� ���ڿ� ���ڵ� �ڵ�.  (���ڵ� �ڵ� �� euc-kr, shift-jis)
		*/
		BOOL CNMSSOWebBrowser::PutCharSet(LPCSTR strCharset)
		{
			return m_pBrowser->PutCharSet(strCharset);
		}

		/**
		@brif - ���� ���� HTML ������������ �־��� HTML �ڵ带 �߰��Ѵ�.
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@param[in] strHtml - 
		*/
		BOOL CNMSSOWebBrowser::InsertHTML(LPCSTR strHtml)
		{
			return m_pBrowser->InsertHTML(strHtml);
		}

		/**
		@brif - �־��� �ڹ� ��ũ��Ʈ �ڵ带 �����Ѵ�.
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@param[in] strScript - ������ �ڹ� ��ũ��Ʈ �ڵ�.
		*/
		BOOL CNMSSOWebBrowser::ExecJavascript(LPCSTR strScript)
		{
			return m_pBrowser->ExecJavascript(strScript);
		}

		/**
		@brif - �־��� URL�� Navigate��Ų��.
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@param[in] strURL - Navigate ��ų URL.
		*/
		BOOL CNMSSOWebBrowser::Navigate(LPCSTR strURL)
		{
			return m_pBrowser->Navigate(strURL);
		}

		/**
		@brif - �־��� ���ҽ� ���̵�� Navigate ��Ų��.
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@param[in] uResID - Navigate ��ų HTML ���ҽ��� ���̵�
		*/
		BOOL CNMSSOWebBrowser::NavigateFromRes(UINT uResID, HINSTANCE hResInstance/* = NULL*/)
		{
			return m_pBrowser->NavigateFromRes(uResID, hResInstance);
		}

		/**
		@brif - �־��� ���ҽ� ���̵�� Navigate ��Ų��.
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ���� �����Ѵ�.
		@param[in] szResID - Navigate ��ų HTML ���ҽ��� ���̵�
		*/
		BOOL CNMSSOWebBrowser::NavigateFromRes(LPCSTR szResID, HINSTANCE hResInstance/* = NULL*/)
		{
			return m_pBrowser->NavigateFromRes(szResID, hResInstance);
		}

		/**
		@brif - ���� SSO �� ��Ʈ�ѿ��� Navigate �� Ƚ���� �����´�.
		@note ����� �������� �ʾ���.
		@returns Navigate �� Ƚ���� ��ȯ�Ѵ�. 
		*/
		int	CNMSSOWebBrowser::GetUserNavigatedCount()
		{
			return m_pBrowser->GetUserNavigatedCount();
		}

		/**
		@brif - �־��� ������ Navigate �� Ƚ���� �����Ѵ�.
		@note ����� �������� �ʾ���.
		@param[in] count - Navigate �� Ƚ���� ������ ��.
		*/
		void CNMSSOWebBrowser::SetUserNavigatedCount(int count)
		{
			m_pBrowser->SetUserNavigatedCount(count);
		}

		/**
		@brif - Navigate �� Ƚ���� �ʱ�ȭ�Ѵ�.
		@note ����� �������� �ʾ���.
		*/
		void CNMSSOWebBrowser::ResetUserNavigatedCount()
		{
			m_pBrowser->ResetUserNavigatedCount();
		}

		/**
		@brif - SSO �� ��Ʈ�� ������ �ڵ鰪�� �����´�.
		@returns SSO �� ��Ʈ�� ������ �ڵ鰪�� ��ȯ�Ѵ�. 
		*/
		HWND CNMSSOWebBrowser::GetHWND()
		{
			return m_pBrowser->GetHWND();
		}

		/**
		@brif - SSO �� ��Ʈ�� �θ� ������ �ڵ鰪�� �����´�.
		@returns SSO �� ��Ʈ�� �θ� ������ �ڵ鰪�� ��ȯ�Ѵ�.
		*/
		HWND CNMSSOWebBrowser::GetParentHWND()
		{
			return m_pBrowser->GetParentHWND();
		}

		/**
		@brif - �ش� ��ǥ�� ����Ʈ���� �̵� �� �������� ��Ų��.
		@param[in] x - �� ��Ʈ���� ���� ��ġ
		@param[in] y - �� ��Ʈ���� ��� ��ġ
		@param[in] nWidth - �� ��Ʈ���� �ʺ�
		@param[in] nHeight - �� ��Ʈ���� ����
		@param[in] bRepaint - �־��� ���� TRUE�̸�, WM_PAINT �޽����� �߻���Ų��.
		*/
		void CNMSSOWebBrowser::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint/* = TRUE*/)
		{
			m_pBrowser->MoveWindow(x, y, nWidth, nHeight, bRepaint);
		}

		/**
		@brif - �ش� �������� ����Ʈ���� �̵� �� �������� ��Ų��.
		@param[in] lpRect - �̵� �� �������� ��ų ����
		@param[in] bRepaint - �־��� ���� TRUE�̸�, WM_PAINT �޽����� �߻���Ų��.
		*/
		void CNMSSOWebBrowser::MoveWindow(LPCRECT lpRect, BOOL bRepaint/* = TRUE*/)
		{
			m_pBrowser->MoveWindow(lpRect, bRepaint);
		}

		/**
		@brif - ���� SSO �� ��Ʈ���� show state�� visible���� �˷��ش�.
		@returns SSO �� ��Ʈ���� show state�� visible�̸� TRUE�� ��ȯ�ϸ�, �ݴ��� ��� FALSE�� ��ȯ�Ѵ�.
		*/
		BOOL CNMSSOWebBrowser::IsWindowVisible()
		{
			return m_pBrowser->IsWindowVisible();
		}

		/**
		@brif - SSO �� ��Ʈ���� show state�� �����Ѵ�.
		@returns �Լ��� ȣ���ϱ����� SSO �� ��Ʈ���� show state�� visible�̸� TRUE�� ��ȯ�ϸ�, �ݴ��� ��� FALSE�� ��ȯ�Ѵ�.
		@param[in] nCmdShow - �����ϰ��� �ϴ� show state ��. (Ex> SW_SHOW, SW_HIDE ��)
		*/
		BOOL CNMSSOWebBrowser::ShowWindow(int nCmdShow)
		{
			return m_pBrowser->ShowWindow(nCmdShow);
		}

		/**
		@brif - @ref SECBeforeNavigate2 "BeforeNavigate2" �̺�Ʈ �߻��ÿ� ȣ��� �ݹ� �Լ��� �����Ѵ�.<br>
		(BeforeNavigate2�� ���ο� URL�� Navigate �Ǳ� ������ �߻��ϴ� �̺�Ʈ.)
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ ���� �����Ѵ�.
		@param[in] pCallback - �̺�Ʈ �߻��ÿ� ȣ��� �ݹ� �Լ� ������.
		@section SECEXAMPLE ���� ����
		���� - @ref PAGEEXAMPLECALLBACK "�� ��Ʈ�� �ݹ� �Լ�" ����
		*/
		BOOL CNMSSOWebBrowser::SetCallback_OnBeforeNavigate2(SWEBCALLBACK_ONBN2* pCallback)
		{
			return m_pBrowser->SetCallback_OnBeforeNavigate2(pCallback);
		}

		/**
		@brif - @ref SECNavigateComplete2 "NavigateComplete2" �̺�Ʈ �߻��ÿ� ȣ��� �ݹ� �Լ��� �����Ѵ�.<br>
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ ���� �����Ѵ�.
		@param[in] pCallback - �̺�Ʈ �߻��ÿ� ȣ��� �ݹ� �Լ� ������.
		@section SECEXAMPLE ���� ����
		���� - @ref PAGEEXAMPLECALLBACK "�� ��Ʈ�� �ݹ� �Լ�" ����
		*/
		BOOL CNMSSOWebBrowser::SetCallback_OnNavigateComplete2(SWEBCALLBACK_ONNC2* pCallback)
		{
			return m_pBrowser->SetCallback_OnNavigateComplete2(pCallback);
		}

		/**
		@brif - @ref SECQuit "Quit" �̺�Ʈ �߻��ÿ� ȣ��� �ݹ� �Լ��� �����Ѵ�.<br>
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ ���� �����Ѵ�.
		@param[in] pCallback - �̺�Ʈ �߻��ÿ� ȣ��� �ݹ� �Լ� ������.
		@section SECEXAMPLE ���� ����
		���� - @ref PAGEEXAMPLECALLBACK "�� ��Ʈ�� �ݹ� �Լ�" ����
		*/
		BOOL CNMSSOWebBrowser::SetCallback_OnQuit(SWEBCALLBACK_ONQUIT* pCallback)
		{
			return m_pBrowser->SetCallback_OnQuit(pCallback);
		}

		/**
		@brif - �־��� Dispatch �������̽��� SSO Web ��Ʈ���� �������̽��� �������� Ȯ���Ѵ�.
		@returns SSO Web ��Ʈ���� �������̽��� �����ϸ� TRUE��, �׷��� �ȴٸ� FALSE�� ��ȯ�Ѵ�.
		@param[in] pDisp - SSO Web ��Ʈ���� �������̽��� �������� Ȯ���� Dispatch �������̽�.
		*/
		BOOL CNMSSOWebBrowser::IsMe(LPDISPATCH pDisp)
		{
			return m_pBrowser->IsMe(pDisp);
		}

		/**
		@brif - �־��� IID�� �´� �������̽��� ��ȯ�Ѵ�.
		@returns �ش� �Լ��� ���� �Ͽ��� ��� TRUE�� ��ȯ�ϸ�, �����Ͽ��� ��� FALSE�� ��ȯ�Ѵ�.<br>
		���н� ���� �������� �����ڵ带 ��������, SSOWebLib_GetLastError() �Լ��� ��ȯ ���� �����Ѵ�.
		@param[in] iid - ������ �������̽� ���̵�(IID)
		@param[out] ppvObject - ������ �������̽� �����͸� ������ ����.
		*/
		BOOL CNMSSOWebBrowser::QueryInterface(REFIID iid, void ** ppvObject)
		{
			return m_pBrowser->QueryInterface(iid, ppvObject);
		}

		//////////////////////////////////////////////////////////////////////////
		// ISSOWebEventsUnknown Interface
		/**
		@brif - �־��� �������̽� �����ڿ� �ش��ϴ� �̺�Ʈ �������̽� �����͸� �����´�.
		@note ��� ���������� ���Ǿ����� �Լ��̺�� �ش� �Լ��� �������� ��� ����� �������� �۵��� �ȵȴ�.
		@returns �����Ұ�� TRUE�� �����Ұ�� FALSE�� ��ȯ�Ѵ�.
		@param[in] eEventVer - ���������� �̺�Ʈ �������̽��� �ش��ϴ� ������.
		@param[out] pVal_Out - ���������� �̺�Ʈ �������̽� ����.
		*/
		//BOOL CNMSSOWebBrowser::SSOWebEventQueryInterface(ENUM_EventVer eEventVer, void** pVal_Out)
		//{
		//	return m_pBrowser->SSOWebEventQueryInterface(eEventVer, pVal_Out);
		//}

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
		void CNMSSOWebBrowser::OnSSOWebEventBN2(LPDISPATCH pDisp, const char* lpszURL, DWORD nFlags, const char* lpszTargetFrameName, const char* lpszPostedData, const char* lpszHeaders, BOOL* pbCancel)
		{
			m_pBrowser->OnSSOWebEventBN2(pDisp, lpszURL, nFlags, lpszTargetFrameName, lpszPostedData, lpszHeaders, pbCancel);
		}

		/**
		@brif - @ref @ref SECNavigateComplete2 "NavigateComplete2" �̺�Ʈ�� �߻��� �� ȣ���.<br>
		@param[in] pDisp - �̺�Ʈ�� �߻��� WebBrowser�� Dispatch �������̽�.
		@param[in] lpszURL - Navigate�� �Ϸ�� URL
		@section SECEXAMPLE ���� ����
		���� - @ref PAGEEXAMPLEEVENTOVERRIDE "�� ��Ʈ�� �̺�Ʈ ��� �Լ� ������" ����
		*/
		void CNMSSOWebBrowser::OnSSOWebEventNC2(LPDISPATCH pDisp, const char* lpszURL)
		{
			m_pBrowser->OnSSOWebEventNC2(pDisp, lpszURL);
		}

		/**
		@brif - @ref SECNewWindow2 "NewWindow2" �̺�Ʈ�� �߻��� �� ȣ���.<br>
		@param[out] ppDisp - ��â�� ������ ��ü�� WebBrowser�� Dispatch �������̽�. ��ü���� ���� ��� ���� �������� �ʵ��� �Ѵ�.
		@param[out] Cancel - ��â�� �����°��� ��� ��ų ��� �ش� ���� ������ TRUE�� ����. �׷��� ���� ��� FALSE�� ����.
		@section SECEXAMPLE ���� ����
		���� - @ref PAGEEXAMPLEEVENTOVERRIDE "�� ��Ʈ�� �̺�Ʈ ��� �Լ� ������" ����
		*/
		void CNMSSOWebBrowser::OnSSOWebEventNW2(LPDISPATCH *ppDisp, BOOL* Cancel)
		{
			m_pBrowser->OnSSOWebEventNW2(ppDisp, Cancel);
		}

		/**
		@brif - @ref @ref SECQuit "Quit" �̺�Ʈ�� �߻��� �� ȣ���.
		@section SECEXAMPLE ���� ����
		���� - @ref PAGEEXAMPLEEVENTOVERRIDE "�� ��Ʈ�� �̺�Ʈ ��� �Լ� ������" ����
		*/
		void CNMSSOWebBrowser::OnSSOWebEventQuit()
		{
			m_pBrowser->OnSSOWebEventQuit();
		}

	}//namespace UTIL

}//namespace NMBASE