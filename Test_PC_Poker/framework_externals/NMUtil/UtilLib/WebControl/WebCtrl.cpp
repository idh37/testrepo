// WebCtrl.cpp : implementation file
//

#include "stdafx.h"
#include <Mshtml.h>
#include "./webcontrol/WebCtrl.h"
#include "./webcontrol/GameTest.h"
// For _bstr_t
#include <comdef.h>
#include "SSOWebLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace NMBASE
{
	namespace UTIL
	{
		/*
		// for mshtml
		#pragma warning(push)
		#pragma warning(disable: 4099)
		#pragma warning(disable: 4146)
		#pragma warning(disable: 4192)
		#import <mshtml.tlb>
		#pragma warning(default: 4192)
		#pragma warning(default: 4049)
		#pragma warning(default: 4146)
		#pragma warning(pop)
		*/

		/////////////////////////////////////////////////////////////////////////////
		// CWebCtrl

		IMPLEMENT_DYNCREATE(CWebCtrl, CHtmlView)

			CWebCtrl::CWebCtrl()
		{
			//{{AFX_DATA_INIT(CWebCtrl)
			// NOTE: the ClassWizard will add member initialization here
			//}}AFX_DATA_INIT

			m_bHideBorder = FALSE;
			m_pfnWebCallBack_ONBN2 = NULL;
			m_pfnWebCallBack_ONNC2 = NULL;
			m_pfnWebCallBack_OnQUIT = NULL;
		}

		CWebCtrl::~CWebCtrl()
		{
		}

		void CWebCtrl::DoDataExchange(CDataExchange* pDX)
		{
			CHtmlView::DoDataExchange(pDX);
			//{{AFX_DATA_MAP(CWebCtrl)
			// NOTE: the ClassWizard will add DDX and DDV calls here
			//}}AFX_DATA_MAP
		}


		BEGIN_MESSAGE_MAP(CWebCtrl, CHtmlView)
			//{{AFX_MSG_MAP(CWebCtrl)
			ON_WM_MOUSEACTIVATE()
			//}}AFX_MSG_MAP
		END_MESSAGE_MAP()

		BEGIN_EVENTSINK_MAP(CWebCtrl, CHtmlView)
			ON_EVENT(CWebCtrl, AFX_IDW_PANE_FIRST, 263, OnWebClose, VTS_BOOL VTS_PBOOL)
		END_EVENTSINK_MAP()

		/////////////////////////////////////////////////////////////////////////////
		// CWebCtrl diagnostics

#ifdef _DEBUG
		void CWebCtrl::AssertValid() const
		{
			CHtmlView::AssertValid();
		}

		void CWebCtrl::Dump(CDumpContext& dc) const
		{
			CHtmlView::Dump(dc);
		}
#endif //_DEBUG

		void CWebCtrl::PostNcDestroy()
		{
		}

		void CWebCtrl::SetOption(BOOL bHideBorder)
		{
			m_bHideBorder = bHideBorder;
		}

		void CWebCtrl::SetBorderNone()
		{
			HRESULT hr;
			IHTMLDocument2* pDoc = NULL;
			IDispatch *pDispatch = NULL;
			IHTMLElement *pHtml = NULL;
			IHTMLStyle *pStyle = NULL;
			pDispatch = GetHtmlDocument();

			if(pDispatch &&
				SUCCEEDED(hr = pDispatch->QueryInterface(IID_IHTMLDocument2, (void**)&pDoc)))
			{
				if(pDoc &&
					SUCCEEDED(hr = pDoc->get_body(&pHtml)))
				{
					if(pHtml &&
						SUCCEEDED(hr = pHtml->get_style(&pStyle)))
					{
						if(pStyle)
						{
							_bstr_t bstr;
							bstr = _T("none");
							pStyle->put_border(bstr);

							pStyle->Release();
						}
						pHtml->Release();
					}
					pDoc->Release();
				}
			}
		}

		/////////////////////////////////////////////////////////////////////////////
		// CWebCtrl message handlers

		void CWebCtrl::OnNavigateComplete2( LPCTSTR strURL )
		{
			m_nNavigatedCount++;

			if(m_pfnWebCallBack_ONNC2)
			{
				m_pfnWebCallBack_ONNC2(strURL);
			}

			CHtmlView::OnNavigateComplete2(strURL );
		}

		int CWebCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
		{
			return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
		}

		void CWebCtrl::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel) 
		{
			// TODO: Add your specialized code here and/or call the base class
			if(m_pfnWebCallBack_ONBN2)
			{
				m_pfnWebCallBack_ONBN2(lpszURL, nFlags, lpszTargetFrameName,
					(char*)baPostedData.GetData(), lpszHeaders, pbCancel);
			}

			CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
		}

		void CWebCtrl::OnDocumentComplete(LPCTSTR lpszURL) 
		{
			// TODO: Add your specialized code here and/or call the base class
			if(m_bHideBorder) 
				SetBorderNone();

			CHtmlView::OnDocumentComplete(lpszURL);
		}

		BOOL CWebCtrl::OnWebClose(BOOL IsChildWindow, BOOL *pCancel)
		{
			UNREFERENCED_PARAMETER( IsChildWindow );

			// user will override to handle this notification
			*pCancel = FALSE;

			if(m_pfnWebCallBack_OnQUIT)
			{
				*pCancel = TRUE;
				m_pfnWebCallBack_OnQUIT();
			}

			return TRUE;
		}

		void CWebCtrl::OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel) 
		{
			// TODO: Add your specialized code here and/or call the base class
			IDispatchPtr spDisp;
			HRESULT hr = E_FAIL;
			IWebBrowser2 *pWebBrowser;
			IHTMLDocument2* pDoc = NULL;
			CString strAddress;

			CoInitialize(NULL);
			hr = ::CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (void**)&pWebBrowser);
			if(SUCCEEDED(hr) && pWebBrowser)
			{
				VARIANT vtFlags, vtTarget, vtPostData, vtHeader;
				::VariantInit(&vtFlags);
				::VariantInit(&vtTarget);
				::VariantInit(&vtPostData);
				::VariantInit(&vtHeader);

				pWebBrowser->put_RegisterAsBrowser(TRUE);
				pWebBrowser->put_Visible(VARIANT_FALSE);
				strAddress.Format(GetActCookieURL());
				hr = pWebBrowser->Navigate(strAddress.AllocSysString(), &vtFlags, &vtTarget, &vtPostData, &vtHeader);

				READYSTATE readystate = READYSTATE_UNINITIALIZED;
				DWORD dwLoadTime = GetTickCount();
				DWORD dwElapsedTime = dwLoadTime;
				const int TIMEOUT_COMPLETE = 5000;	// 5 sec timout
				while (readystate != READYSTATE_COMPLETE && (dwElapsedTime - dwLoadTime) < TIMEOUT_COMPLETE)
				{
					pWebBrowser->get_ReadyState(&readystate);
					dwElapsedTime= GetTickCount();
					Sleep(1);
				}

				if(pWebBrowser->get_Document(&spDisp) == S_OK && spDisp != NULL)
				{
					IHTMLDocument2Ptr spHtmlDocument(spDisp);

					IDispatchPtr pthisDoc = GetHtmlDocument();
					if(pthisDoc)
					{
						IHTMLDocument2Ptr spthisHtmlDoc(pthisDoc);

						BSTR bstr;
						spthisHtmlDoc->get_cookie(&bstr);
						CString str(bstr);
						CString strDomain;
						CString strFlag;

						strDomain = GetCookieDomain();

						strFlag.Format(_T("domain=%s;path=/;"), strDomain.GetString());

						CString strCookie;

						for (int i = 0; i < str.GetLength(); i++)
						{
							strCookie += str.GetAt(i);

							if(str.GetAt(i) == ';')
							{
								strCookie += strFlag;
								strCookie.TrimLeft();
								strCookie.TrimRight();
								bstr = strCookie.AllocSysString();
								spHtmlDocument->put_cookie(bstr);

								::SysFreeString(bstr);
								strCookie = "";
							}
						}
					}
				}

				pWebBrowser->get_Application(ppDisp);
				pWebBrowser->Release();
			}

			CHtmlView::OnNewWindow2(ppDisp, Cancel);
		}

		void CWebCtrl::Navigate2(LPCTSTR lpszURL, DWORD dwFlags, LPCTSTR lpszTargetFrameName)
		{

			char *pConverted = Test_URLConvert(lpszURL);

			CHtmlView::Navigate2(pConverted, dwFlags, lpszTargetFrameName);
		}

	}//namespace UTIL

}//namespace NMBASE

