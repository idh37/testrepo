// WebCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "./webcontrol/WebCtrl4New.h"
#include "mshtml.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// for mshtml

#pragma warning(push)
#pragma warning(disable: 4099)
#pragma warning(disable: 4146)
#pragma warning(disable: 4192)
#import <mshtml.tlb> rename("TranslateAccelerator", "TranslateAcceleratorA") 
#pragma warning(default: 4192)
#pragma warning(default: 4049)
#pragma warning(default: 4146)
#pragma warning(pop)


namespace NMBASE
{
	namespace UTIL
	{

		/////////////////////////////////////////////////////////////////////////////
		// CWebCtrl
		IMPLEMENT_DYNCREATE(CWebCtrl4New, CHtmlView)

		CWebCtrl4New::CWebCtrl4New(HWND hParent/* = NULL*/)
		{
			//Parent가 NULL이면 안된다. ex) g_pMainDlg->m_hWnd
			ASSERT(m_hParent != NULL);

			//{{AFX_DATA_INIT(CWebCtrl)
			// NOTE: the ClassWizard will add member initialization here
			//}}AFX_DATA_INIT
			m_bNaviate = FALSE;
			m_bHideBorder = TRUE;
			m_hParent = hParent;
		}

		CWebCtrl4New::~CWebCtrl4New()
		{
		}

		void CWebCtrl4New::DoDataExchange(CDataExchange* pDX)
		{
			CHtmlView::DoDataExchange(pDX);
			//{{AFX_DATA_MAP(CWebCtrl)
			// NOTE: the ClassWizard will add DDX and DDV calls here
			//}}AFX_DATA_MAP
		}


		BEGIN_MESSAGE_MAP(CWebCtrl4New, CHtmlView)
			//{{AFX_MSG_MAP(CWebCtrl)
			// NOTE - the ClassWizard will add and remove mapping macros here.
			//}}AFX_MSG_MAP
		END_MESSAGE_MAP()

		/////////////////////////////////////////////////////////////////////////////
		// CWebCtrl diagnostics

#ifdef _DEBUG
		void CWebCtrl4New::AssertValid() const
		{
			CHtmlView::AssertValid();
		}

		void CWebCtrl4New::Dump(CDumpContext& dc) const
		{
			CHtmlView::Dump(dc);
		}
#endif //_DEBUG


		void CWebCtrl4New::SetOption(BOOL bHideBorder)
		{
			m_bHideBorder = bHideBorder;
		}


		void CWebCtrl4New::SetBorderNone()
		{
			MSHTML::IHTMLDocument2Ptr doc = (MSHTML::IHTMLDocument2Ptr)GetHtmlDocument();
			if(doc!=NULL) 
			{
				MSHTML::IHTMLElementPtr body = (MSHTML::IHTMLElementPtr)doc->body;      
				if(body!=NULL)	body->style->border = _T("none");
			}
		}

		/////////////////////////////////////////////////////////////////////////////
		// CWebCtrl message handlers
		void CWebCtrl4New::OnBeforeNavigate2( LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel )
		{
			CString MacroString = lpszURL;

			if(MacroString.Find("_충전소가기_",0)>=0) 
			{
				*pbCancel = TRUE;
				::SendMessage(m_hParent,WM_ALLINMESSAGE,ACD_GO_CHARGE,(DWORD)GetParent()->m_hWnd);
			} 
			else if(MacroString.Find("_리더스_",0)>=0) 
			{
				*pbCancel = TRUE;
				::SendMessage(m_hParent,WM_ALLINMESSAGE,ACD_GO_LEADERS,(DWORD)GetParent()->m_hWnd);
			} 
			else if(MacroString.Find("_보디가드자세히보기_",0)>=0) 
			{
				*pbCancel = TRUE;
				::SendMessage(m_hParent,WM_ALLINMESSAGE,ACD_ALLIN_INSURANCE,(DWORD)GetParent()->m_hWnd);
			}
			else if(MacroString.Find("_천하무적구매하기_",0)>=0) 
			{
				*pbCancel = TRUE;
				::SendMessage(m_hParent,WM_ALLINMESSAGE,ACD_INVINCIBLE_BUY,(DWORD)GetParent()->m_hWnd);
			}
			else if(MacroString.Find("_천하무적자세히보기_",0)>=0) 
			{
				*pbCancel = TRUE;
				::SendMessage(m_hParent,WM_ALLINMESSAGE,ACD_INVINCIBLE_DETAILS,(DWORD)GetParent()->m_hWnd);
			}
			else if(MacroString.Find("_보디가드구매하기_",0)>=0) 
			{
				*pbCancel = TRUE;
				::SendMessage(m_hParent,WM_ALLINMESSAGE,ACD_ALLIN_INSURANCE_BUY,(DWORD)GetParent()->m_hWnd);
			}
			else if(MacroString.Find("_보디가드사용하기_",0)>=0) 
			{
				*pbCancel = TRUE;
				::SendMessage(m_hParent,WM_ALLINMESSAGE,ACD_ALLIN_INSURANCE_USE,(DWORD)GetParent()->m_hWnd);
			} 
			else if(MacroString.Find("_게임종료_",0)>=0) 
			{
				*pbCancel = TRUE;
				::SendMessage(m_hParent,WM_ALLINMESSAGE,ACD_CLOSEWINDOW,(DWORD)GetParent()->m_hWnd);
			} 
			else if(MacroString.Find("_게임메인_",0)>=0)
			{
				*pbCancel = TRUE;
				::SendMessage(m_hParent,WM_ALLINMESSAGE,ACD_GO_MAINPAGE,(DWORD)GetParent()->m_hWnd);
			}
			// [DB기록 Process] 2004.11.29
			else if(MacroString.Find("_게임계속하기_",0)>=0) 
			{
				*pbCancel = TRUE;
				::SendMessage(m_hParent, WM_ALLINMESSAGE, ACD_GAME_CONTINUE, (DWORD)GetParent()->m_hWnd);
			} 
			// [DB기록 Process] 2004.11.29  리더스 회원존
			else if(MacroString.Find("_리더스회원존_",0)>=0) 
			{
				*pbCancel = TRUE;
				::SendMessage(m_hParent, WM_ALLINMESSAGE, ACD_GO_LEADERS_ZONE, (DWORD)GetParent()->m_hWnd);
			}
			// [DB기록 Process] 2004.11.29  아바타 상점
			else if(MacroString.Find("_아바타상점_",0)>=0) 
			{
				*pbCancel = TRUE;
				::SendMessage(m_hParent, WM_ALLINMESSAGE, ACD_GO_AVATA_ZONE, (DWORD)GetParent()->m_hWnd);
			}
			else if( MacroString.Find("_피시방찾기_",0)>=0) 
			{
				*pbCancel = TRUE;
				::SendMessage(m_hParent, WM_ALLINMESSAGE, ACD_GO_SEARCH_PCROOM, (DWORD)GetParent()->m_hWnd);
			}
			// [아이템 유료화 영역]
			else if(MacroString.Find("_팝업창닫기_",0)>=0) 
			{
				*pbCancel = TRUE;
				::SendMessage(m_hParent, WM_ALLINMESSAGE, ACD_CLOSEPOPUP, (DWORD)GetParent()->m_hWnd);
			}
			else if(!m_bNaviate) 
			{
				m_bNaviate = TRUE;
			} 
			else
			{
				//		*pbCancel = TRUE;
			}

			CHtmlView::OnBeforeNavigate2(lpszURL,nFlags,lpszTargetFrameName,baPostedData,lpszHeaders,pbCancel );
		}


		void CWebCtrl4New::OnNavigateComplete2( LPCTSTR strURL )
		{
			if(m_bHideBorder) SetBorderNone();
			CHtmlView::OnNavigateComplete2(strURL );
		}

	}//namespace UTIL

}//namespace NMBASE


