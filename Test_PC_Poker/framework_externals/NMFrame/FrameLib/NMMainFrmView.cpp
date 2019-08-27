#include "StdAfx.h"


#include "NMApp.h"
#include "NMMainFrmView.h"

//---------------------------------------------------------------------------------



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


namespace NMBASE
{
	namespace FRAME
	{

#ifndef LOWORD
#define LOWORD(l)           ((WORD)((DWORD_PTR)(l) & 0xffff))
#endif
#ifndef HIWORD
#define HIWORD(l)           ((WORD)((DWORD_PTR)(l) >> 16))
#endif

		// define useful macros from windowsx.h
#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lParam)	((int)(short)LOWORD(lParam))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lParam)	((int)(short)HIWORD(lParam))
#endif


		IMPLEMENT_DYNAMIC(CNMMainFrmView, CFrameWnd)

		BEGIN_MESSAGE_MAP(CNMMainFrmView, CFrameWnd)
			ON_WM_DESTROY()
			ON_WM_CREATE()
			ON_WM_TIMER()
		END_MESSAGE_MAP()


		/*!
		\brief  생성자
		\param  void
		\return void 
		*/
		CNMMainFrmView::CNMMainFrmView(void) 
		{
			m_nWinWidth  = 0;
			m_nWinHeight = 0;
			m_dwDragFlags = 0;
			m_ptClickPos.x = m_ptClickPos.y = 0;
		}


		CNMMainFrmView::CNMMainFrmView(int nWidth, int nHeight)
			: m_nWinWidth(nWidth),
			m_nWinHeight(nHeight)
		{
			m_dwDragFlags = 0;
			m_ptClickPos.x = m_ptClickPos.y = 0;

			m_strViewName = "frame";
			m_eViewType   =  EVIEW_NOMAL;
		}

		/*!
		\brief  소멸자
		\param  void
		\return void 
		*/
		CNMMainFrmView::~CNMMainFrmView(void)
		{	

		}

		/*!
		\brief  Main Frame Window 생성
		\param  lpCreateStruct
		\return int 
		*/
		int CNMMainFrmView::OnCreate(LPCREATESTRUCT lpCreateStruct)
		{
			if(CFrameWnd::OnCreate(lpCreateStruct) == -1)
			{
				return -1;
			}

			

			return 0;
		}

		/*!
		\brief  PreCreateWindow
		\param  cs
		\return BOOL 
		*/
		BOOL CNMMainFrmView::PreCreateWindow(CREATESTRUCT& cs)
		{
			UNREFERENCED_PARAMETER(cs);

			CNMApp* pApp = (CNMApp*)AfxGetApp();

			if(pApp)
			{
				m_nWinWidth  = pApp->GetWindowWidth();
				m_nWinHeight = pApp->GetWindowHeight();
			}

			///< 이동 가능한 캡션 영역 지정
			// LSYlib의 SetCaptionHeight()와 중복되는 기능이라서 주석처리 [12/4/2011 guesty]
	//		SetMoveCaptionRect();	

			return TRUE;
		}


		/*!
		\brief  WindowProc
		\param  message
		\param  wParam
		\param  lParam
		\return LRESULT 
		*/
		LRESULT CNMMainFrmView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
		{
#define DRAGGING    0x01			// dragging flag
			static bool	 bProcFlag = false;

			switch (message)
			{
			case WM_TIMER:  // WM_PAINT에서 WM_TIMER로 변경 됨.
				{
					///< LSY LSYLibDialogBase의 Timer에서 호출을 해줍니다.
					if(wParam == 10000)
					{
						OnDialogToPaint();
					}
				}
				break;
			case WM_LBUTTONDOWN:
				{
					POINT pt;
					pt.x = GET_X_LPARAM(lParam);
					pt.y = GET_Y_LPARAM(lParam);
					m_ptClickPos = pt;		
					// 타이틀바 클릭 + Move를 위해 필요			
					//if( DxDevice().IsWindowMode() && m_rcMoveCaption.PtInRect( pt ) )
					if( m_rcMoveCaption.PtInRect( pt ) )
					{
						m_ptClickPos = pt;
						m_dwDragFlags |= DRAGGING;

						SetCapture();
					}
				}
				break;

			case WM_LBUTTONDBLCLK:
				{
					POINT pt;
					pt.x = GET_X_LPARAM(lParam);
					pt.y = GET_Y_LPARAM(lParam);

					//// 타이틀바 클릭 + Move를 위해 필요
					//if(m_rcMoveCaption.PtInRect( pt ))
					//{
					//	if(DxDevice().IsWindowMode())
					//	{
					//		ProcessFullBut();
					//	}
					//	else 
					//	{
					//		ProcessWindowBut();
					//	}
					//}
				}
				break;

			case WM_LBUTTONUP:
				{
					if(m_dwDragFlags & DRAGGING)
					{
						RECT rect;
						GetWindowRect( &rect );

						m_dwDragFlags &= ~DRAGGING;
						ReleaseCapture();
					}
				}
				break;

			case WM_ERASEBKGND:
				return TRUE;

			case WM_MOUSEMOVE:
				{
					// Windows Move(Don't Used Neon) 
					if(m_dwDragFlags & DRAGGING)
					{
						POINT pt;
						pt.x = GET_X_LPARAM(lParam);
						pt.y = GET_Y_LPARAM(lParam);

						RECT rect;
						GetWindowRect( &rect );

						rect.left += pt.x - m_ptClickPos.x;
						rect.top  += pt.y - m_ptClickPos.y;

						SetWindowPos(NULL, rect.left, rect.top, m_nWinWidth, m_nWinHeight, SWP_NOZORDER | SWP_NOSIZE );
					}
				}
				break;

			default:
				break;

			}


			return CFrameWnd::WindowProc(message, wParam, lParam);
		}

		/*!
		\brief  Window 파괴
		\param  void
		\return void 
		*/
		void CNMMainFrmView::OnDestroy()
		{
			///< 모든 메시지의 Call Command를 삭제한다.
			NMBASE::UTIL::MM().ClearAllCommand();

			///< 반드시 메인 윈도우가 파괴되기 전에 호출해야한다. 
			NMBASE::UTIL::NMUnInitSSOWebBrowser();	// [SSO 작업]

			CFrameWnd::OnDestroy();
		}


		/*!
		\brief  PreTranslateMessage
		\param  pMsg
		\return BOOL 
		*/
		BOOL CNMMainFrmView::PreTranslateMessage(MSG* pMsg)
		{
			if( pMsg->message == WM_SYSKEYDOWN )
			{
				short shScanCode = VkKeyScan( (TCHAR)pMsg->wParam);

				//ALT + F4 KeyDown
				if( shScanCode == 83 && pMsg->wParam == 115 )
				{
					// 예외 처리 할것이 있으면 한다.
					
					WPARAM wparam = 0;
					LPARAM lparam = 0;

					m_cOnClickAltF4KeyDown.Call((WPARAM&)wparam,(LPARAM&)lparam);
					return TRUE;
				}
			}


			return CFrameWnd::PreTranslateMessage(pMsg);
		}


		/*!
		\brief  OnTimer
		\param  nIDEvent
		\return void 
		*/
		void CNMMainFrmView::OnTimer( UINT nIDEvent )
		{

		}

		/*!
		\brief  OnClose
		\param  void
		\return void 
		*/
		void CNMMainFrmView::OnClose()
		{
			CFrameWnd::OnClose();
		}


		///< 이동 가능한 캡션 영역 지정
		void CNMMainFrmView::SetMoveCaptionRect()
		{
			// 메인 타이틀바 영역을 세팅한다.(마우스로 클릭하여 이동시켜 주기 위함.)
			m_rcMoveCaption.SetRect( 0, 0, 1024, 40 );
		}

	}//namespace FRAME

}//namespace NMBASE


