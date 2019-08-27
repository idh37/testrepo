#include "stdafx.h"
#include "./NMWidget/SSOWebWidget.h"

namespace NMBASE
{
	namespace SKINGDI
	{
		////////////////////////////////////////////////////////////////////////////////////
		// CSSOWebWidget dialog

		CSSOWebWidget::CSSOWebWidget()
		{
			m_pParent = NULL;
			m_nControlID = WIDGET_ID_NULL;
			m_bShow = FALSE;
			m_rectPosition.SetRect(0,0,0,0);
			m_pWebItemZone = NULL;
		}

		CSSOWebWidget::~CSSOWebWidget()
		{
			DestroySSOWeb();
		}

		BOOL CSSOWebWidget::Initialize( int nControlID, CWnd* pParant, CRect rectPosition, LPDISPATCH lpExternalDisp=NULL)
		{
			if( pParant == NULL )	return FALSE;
			m_pParent = pParant;
			m_nControlID = nControlID;
			m_rectPosition = rectPosition;

			if( m_pWebItemZone )	return FALSE;

			m_pWebItemZone = new NMBASE::UTIL::CNMSSOWebBrowser();

			BOOL bRet = FALSE;
			bRet = m_pWebItemZone->Create( WS_CHILD | WS_VISIBLE, m_rectPosition, pParant->GetSafeHwnd(), nControlID, lpExternalDisp);

			if( bRet == FALSE )
			{
				delete m_pWebItemZone;
				m_pWebItemZone = NULL;
				return FALSE;
			}

			m_pWebItemZone->Navigate( "about:blank" );
			m_pWebItemZone->SetHideBorder( TRUE );
			//	m_pWebItemZone->SetCallback_OnBeforeNavigate2( SSO_OnBeforeNavigate2 );
			return TRUE;
		}

		void CSSOWebWidget::NavigateSSOWeb(const char *pURL)
		{
			if( m_pWebItemZone == NULL || pURL==NULL)	return;
			m_pWebItemZone->Navigate( pURL );

			/*
			CString strBaseURL;
			CString strGameParam;
			CString strChannelGroup;
			CString strChanceURL;
			CString strURL;

			strBaseURL.Format( "http://game1.netmarble.net/sub/client/itemmall.asp" );		// 추가 이것으로 수정
			//strBaseURL.Format( "http://game1.netmarble.net/sub/client/item.asp" );
			strGameParam.Format( "?game=%s", g_GameParam );
			strChannelGroup.Format( "&channel=%d", GetChannelGroupForWeb( g_NowGroupNo ) );
			strChanceURL = g_pMainDlg->m_RewardItemMan.GetItemGroupSet( "&groupcode=" );

			strURL = strBaseURL + strGameParam + strChannelGroup + strChanceURL;

			m_pWebItemZone->Navigate( strURL );
			m_pWebItemZone->SetUserNavigatedCount( 1 );
			*/
		}

		void CSSOWebWidget::DestroySSOWeb()
		{
			if( m_pWebItemZone )
			{
				delete m_pWebItemZone;
				m_pWebItemZone = NULL;
			}
		}

		void CSSOWebWidget::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
		{
			/////////////////////////////////////////////////////////////////////
			// 현재로서는 해당 기능이 필요없다. 일단 빼 놓자.
			// 해당 기능을 다시 사용하려면 CLobbyDlg::OnInitDialog()에서
			// m_LobbyView를 생성할 때 WS_CLIPCHILDREN 옵션을 빼고 사용해야 의도한 대로 동작한다.
			return;
			/////////////////////////////////////////////////////////////////////

			if( m_bShow == FALSE )	return;

			CDC SSOWebDC;
			SSOWebDC.CreateCompatibleDC(&DC);
			if( SSOWebDC.m_hDC == NULL ) return;

			HBITMAP SSOWebBitmap;
			SSOWebBitmap = CreateCompatibleBitmap( DC.m_hDC, m_rectPosition.Width(), m_rectPosition.Height() );
			SSOWebDC.SelectObject( SSOWebBitmap );

			DrawToByRender( SSOWebDC.m_hDC );

			DC.BitBlt( m_rectPosition.left, m_rectPosition.top,
				m_rectPosition.right, m_rectPosition.bottom,
				&SSOWebDC, 0, 0, SRCCOPY );

			DeleteObject( SSOWebBitmap );
			SSOWebDC.DeleteDC();
		}

		void CSSOWebWidget::SetShow( BOOL bShow )
		{
			if( m_pWebItemZone == NULL )	return;

			m_pWebItemZone->ShowWindow( ( bShow ? SW_SHOW : SW_HIDE ) );
			m_bShow = bShow;
		}

		BOOL CSSOWebWidget::DrawToByRender(HDC hdc)
		{
			IWebBrowser2* pWeb;
			if( m_pWebItemZone->QueryInterface( IID_IWebBrowser2, (void **)&pWeb) )
			{
				HRESULT hr;
				IDispatch* pDispatch;
				hr = pWeb->get_Document(&pDispatch);
				if(hr != S_OK)
				{
					pWeb->Release();
					return FALSE;
				}

				IHTMLDocument2* pDoc2 = NULL;
				hr=pDispatch->QueryInterface(__uuidof(IHTMLDocument2), (void**)&pDoc2);
				if(hr != S_OK)
				{
					pWeb->Release();
					return FALSE;
				}

				IHTMLElement* pElement; 
				pDoc2->get_body(&pElement);
				if (!pElement)
				{
					pDoc2->Release();
					pWeb->Release();
					return FALSE;
				}

				IHTMLElementRender* pRender; 
				pElement->QueryInterface(IID_IHTMLElementRender, (void **) &pRender);

				if (!pRender)
				{
					pElement->Release();
					pDoc2->Release();
					pWeb->Release();
					return FALSE;
				}

				pRender->DrawToDC(hdc);

				pRender->Release();
				pElement->Release();
				pDoc2->Release();
				pWeb->Release();
			}

			return TRUE;
		}



	} //namespace SKINGDI

}//namespace NMBASE