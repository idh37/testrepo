// GrpChatEdit.cpp: implementation of the CGrpChatEdit2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GrpChatEdit2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



namespace NMBASE
{
	namespace SKINGDI
	{
		//////////////////////////////////////////////////////////////////////
		// Construction/Destruction
		//////////////////////////////////////////////////////////////////////

		CGrpChatEdit2::CGrpChatEdit2()
		{

		}

		CGrpChatEdit2::~CGrpChatEdit2()
		{

		}


		BEGIN_MESSAGE_MAP(CGrpChatEdit2, CChatViewEdit)
			//{{AFX_MSG_MAP(CSkinListCtrl)
			ON_WM_NCCALCSIZE()
			ON_WM_MOUSEWHEEL()
			ON_WM_PAINT()
			ON_WM_SIZE()
			ON_WM_FONTCHANGE()
			//}}AFX_MSG_MAP
		END_MESSAGE_MAP()


		void CGrpChatEdit2::PreSubclassWindow() 
		{
			//use our custom CHeaderCtrl
			CChatViewEdit::PreSubclassWindow();
		}



		////////////////////////////////////////////////////////
		//
		void CGrpChatEdit2::Init(BOOL bViewScroll)
		{
			//another way to hide scrollbars
			InitializeFlatSB(this->m_hWnd);
			FlatSB_EnableScrollBar(this->m_hWnd, SB_BOTH, ESB_DISABLE_BOTH);

			CWnd* pParent = GetParent();

			CRect windowRect;
			GetWindowRect(&windowRect);
			//	ScreenToClient( &windowRect );

			//Create scrollbars at runtime
			m_SkinVerticleScrollbar.LoadBtnImage2();		// 버튼 이미지 읽기
			m_SkinVerticleScrollbar.CalcImageSize();	// 이미지 크기 계산하기 

			CRect rc(windowRect.right,windowRect.top,windowRect.right+m_SkinVerticleScrollbar.m_Thumb.x,windowRect.bottom);
			m_SkinVerticleScrollbar.Create(NULL, WS_CHILD|SS_LEFT|SS_NOTIFY|WS_VISIBLE|WS_GROUP,
				rc, pParent);
			m_SkinVerticleScrollbar.pEdit = this;

			UpdateScrollSize();		// 글씨 세로 사이즈 구하기 
			// 이미지 값 설정 

			if(bViewScroll == FALSE)
				m_SkinVerticleScrollbar.ShowWindow(FALSE);
		}

		void CGrpChatEdit2::OnPaint() 
		{
			CChatViewEdit::OnPaint();
			m_SkinVerticleScrollbar.UpdateThumbPosition();

		}


		void CGrpChatEdit2::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
		{
			/*****************************************************************
			** credits for this method of removing horz and vert
			** scrollbars goes to Filbert Fox. He posted this solution
			** on the www.codeguru.com discussion boards
			******************************************************************/
			UpdateWindow();
			CChatViewEdit::OnNcCalcSize(bCalcValidRects, lpncsp);
		}


		void CGrpChatEdit2::OnSize(UINT nType, int cx, int cy) 
		{
			CChatViewEdit::OnSize(nType, cx, cy);

			// TODO: Add your message handler code here
			CRect windowRect;
			GetWindowRect(&windowRect);

			if(::IsWindow(m_SkinVerticleScrollbar.m_hWnd)) m_SkinVerticleScrollbar.MoveWindow(windowRect.right,windowRect.top,m_SkinVerticleScrollbar.m_Thumb.x,windowRect.bottom-windowRect.top,FALSE);


		}

		BOOL CGrpChatEdit2::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
		{
			m_SkinVerticleScrollbar.UpdateThumbPosition();

			return CChatViewEdit::OnMouseWheel(nFlags, zDelta, pt);
		}



		void CGrpChatEdit2::OnFontChange() 
		{
			CChatViewEdit::OnFontChange();

			UpdateScrollSize();

		}


		void CGrpChatEdit2::UpdateScrollSize(void)
		{
			CHARFORMAT cf;
			GetDefaultCharFormat(cf);

			HDC hdc =::GetDC(m_hWnd);
			m_SkinVerticleScrollbar.m_ThumbScrollSize = MulDiv(cf.yHeight/20,::GetDeviceCaps(hdc,LOGPIXELSY),72);
			::ReleaseDC(m_hWnd,hdc);
		}

	} //namespace SKINGDI

}//namespace NMBASE