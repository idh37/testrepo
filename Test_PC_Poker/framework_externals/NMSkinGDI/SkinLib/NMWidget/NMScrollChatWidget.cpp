#include "StdAfx.h"
#include "./NMWidget/NMScrollChatWidget.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		CNMScrollChatWidget::CNMScrollChatWidget(void)
		{

			m_nSpaceScrollRect = SPACE_CHAT_VIEW_SCROLL_RECT;
			m_nOffsetScrollBtns = OFFSET_SCROLL_BUTTONS;

			m_pButtonUp=NULL;
			m_pButtonDown=NULL;
			m_pButtonThum=NULL;
		}

		CNMScrollChatWidget::~CNMScrollChatWidget(void)
		{
		}

		void CNMScrollChatWidget::SetScrollButton(CNMButton *pUpButton, CNMButton *pDownButton, CNMThumButton *pThumButton)
		{
			m_pButtonUp = pUpButton;
			AttachWidget(m_pButtonUp->GetControlID(), m_pButtonUp);

			m_pButtonDown = pDownButton;
			AttachWidget(m_pButtonDown->GetControlID(), m_pButtonDown);

			m_pButtonThum= pThumButton;
			AttachWidget(m_pButtonThum->GetControlID(), m_pButtonThum);	

			m_pButtonThum->SetThumAttribute( m_pButtonUp, m_pButtonDown, &m_rtScroll, m_nLinePerPage, 1 );
			// 	m_pButtonThum->SetElementCount(0);

		}

		BOOL CNMScrollChatWidget::CreateWidget( CWnd* pParent )
		{

			RepositionScroll();

			return CNMChatWidget::CreateWidget(pParent);
		}


		void CNMScrollChatWidget::RepositionScroll()
		{
			//사이즈 변동시 스크롤 영역을 재구성할 필요가 있을 때


			CRect rtUpBtn = m_pButtonUp->GetButtonRect();
			CRect rtDownBtn = m_pButtonDown->GetButtonRect();

			int scrrect_left = 	m_rectPosition.right + m_nSpaceScrollRect;
			int scrrect_top = 	m_rectPosition.top - m_nOffsetScrollBtns;
			int scrrect_right = scrrect_left + rtUpBtn.Width() ;
			int scrrect_bottom = m_rectPosition.bottom - rtDownBtn.Height() + m_nOffsetScrollBtns; //Height - rtDownBtn.Height() - m_nSpaceScrollRect;


			m_rtScroll.SetRect( scrrect_left, 
				scrrect_top, 
				scrrect_right,
				scrrect_bottom);



			m_pButtonUp->SetPosition(m_rtScroll.left, m_rtScroll.top);
			m_pButtonDown->SetPosition(m_rtScroll.left, m_rtScroll.bottom);	
			m_pButtonThum->SetThumPosition(m_pButtonUp, m_pButtonDown, &m_rectPosition);
			//m_pButtonThum->SetThumAttribute( m_pButtonUp, m_pButtonDown, &m_rtScroll, m_nLinePerPage, 1 );
			//m_pButtonThum->SetElementCount(m_CommonRichEdit.GetLineCount()-1);

		}

		void CNMScrollChatWidget::SetPosition(int nPosX, int nPosY)
		{
			CNMChatWidget::SetPosition(nPosX, nPosY);


			RepositionScroll();
		}


		BOOL CNMScrollChatWidget::Initialize( int nControlID, CWnd* pParent, CRect rectEdit,
			int nLinePerPage, int nLinePerLimit )
		{



			return CNMChatWidget::Initialize(nControlID, pParent, rectEdit, nLinePerPage, nLinePerLimit);

		}


		BOOL CNMScrollChatWidget::OnCommandWidget( int nCommand, WPARAM wParam, LPARAM lParam )
		{

			if (nCommand == m_pButtonUp->GetControlID())
			{
				if( m_pButtonThum )
					m_pButtonThum->BeforePage();

			}
			else if (nCommand == m_pButtonDown->GetControlID())
			{
				if(m_pButtonThum)
					m_pButtonThum->NextPage();
			}
			else if (nCommand == m_pButtonThum->GetControlID())
			{
				//RenewalList();	// 리스트의 포지션이 변했다 .. 리스트오브젝트 재세팅		
				SetCurrentLine( m_pButtonThum->GetStartElement() );
			}
			else if (nCommand == m_nControlID) //데이터 갯수가 변했다
			{
				m_pButtonThum->SetElementCount(m_CommonRichEdit.GetLineCount()-1);
			}


			return CNMChatWidget::OnCommandWidget( nCommand, wParam, lParam );
		}




		BOOL CNMScrollChatWidget::OnMouseMoveWidget( UINT nFlags, CPoint point )
		{
			if (m_rectPosition.PtInRect(point))
				return TRUE;

			return CNMChatWidget::OnMouseMoveWidget(nFlags, point);;
		}
		BOOL CNMScrollChatWidget::OnLButtonDownWidget( UINT nFlags, CPoint point )
		{
			if (m_rectPosition.PtInRect(point))
				return TRUE;

			return CNMChatWidget::OnLButtonDownWidget(nFlags, point);;

		}
		BOOL CNMScrollChatWidget::OnLButtonUpWidget( UINT nFlags, CPoint point )
		{
			if (m_rectPosition.PtInRect(point))
				return TRUE;

			return CNMChatWidget::OnLButtonUpWidget(nFlags, point);;

		}
		BOOL CNMScrollChatWidget::OnLButtonDbClkWidget( UINT nFlags, CPoint point )
		{
			if (m_rectPosition.PtInRect(point))
				return TRUE;

			return CNMWidgetComposite::OnLButtonDbClkWidget(nFlags, point);;
		}



	} //namespace SKINGDI

}//namespace NMBASE