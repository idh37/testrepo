#pragma once

#include "NMWidgetManager.h"
#include <afxhtml.h>
#include <atlbase.h>
#include <mshtml.h>
// 
// extern CString	g_SSO_WEB_URL;
// extern int		g_SSO_WEB_nType;
// extern int		g_SSO_WEB_nItemCode;
// extern int		g_SSO_WEB_nGroupCode;
// extern int		g_SSO_WEB_nItemno;
// extern BOOL		g_bJumpItemBuying;
// 
// void SSO_OnBeforeNavigate2( LPCTSTR lpszURL,
// 						   DWORD nFlags,
// 						   LPCTSTR lpszTargetFrameName,
// 						   LPCTSTR lpszPostedData,
// 						   LPCTSTR lpszHeaders,
// 						   BOOL* pbCancel );
// void SSO_OnBeforeNavigate2ForAppPopup( LPCTSTR lpszURL, BOOL* pbCancel );
// void SSO_OnBeforeNavigate2ForStatus( LPCTSTR lpszURL, BOOL* pbCancel );
// CString GetPostedData( CString sMacroString, LPCTSTR lpszPostedData );
// CString GetPostedData1( CString sMacroString, LPCTSTR lpszPostedData );
// 
// int GetChannelGroupForWeb( int nGroupNo );

#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		////////////////////////////////////////////////////////////////////////////////////
		// CSSOWebWidget dialog
		class NMSKINGDI_CLASS CSSOWebWidget : public CNMWidgetCompact
		{
		public:
			CSSOWebWidget();
			virtual ~CSSOWebWidget();

			BOOL Initialize( int nControlID, CWnd* pParant, CRect rectPosition, LPDISPATCH lpExternalDisp);
			virtual void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );
			virtual void SetShow( BOOL bShow );
			virtual BOOL GetShow() {	return m_bShow;	}

			void NavigateSSOWeb(const char *pURL);
			void DestroySSOWeb();	

		private:
			BOOL DrawToByRender( HDC hdc );

		protected:
			CWnd*						m_pParent;
			int							m_nControlID;
			BOOL						m_bShow;
			CRect						m_rectPosition;
			NMBASE::UTIL::CNMSSOWebBrowser*			m_pWebItemZone;
		};

	} //namespace SKINGDI

}//namespace NMBASE