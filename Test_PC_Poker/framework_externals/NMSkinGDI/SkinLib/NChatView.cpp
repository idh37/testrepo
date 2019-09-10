// NListControl.cpp: implementation of the CNChatView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NChatView.h"
#include "NText.h"
#include "ChatViewEdit.h"

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

		CNChatView::CNChatView()
		{
			m_pPage = NULL;
			m_bHideText = FALSE;
			m_pChatViewEdit = NULL;
			m_pParentWnd=NULL;
			m_nFontIndex = NMBASE::UTIL::FONT_12_NORMAL;
			m_bOutline = false;
			m_clrOutline = RGB(0,0,0);
		}

		CNChatView::CNChatView(int x, int y, int width, int height, CWnd* pParentWnd, NMBASE::GRAPHICGDI::CPage* pPage , bool bAutoScroll)
		{	
			m_Width = width;
			m_Height = height;
			m_Xp = x;
			m_Yp = y;
			m_pPage = pPage;
			m_bAutoScroll = bAutoScroll;
			m_pParentWnd = pParentWnd;

			CRect rc;
			// ä�� �� ��ġ����Ʈ ��Ʈ�� ����
			m_pChatViewEdit = new CChatViewEdit();
			rc.SetRect( x, y, x+m_Width, y+m_Height);
			BOOL bRet=m_pChatViewEdit->Create( 0x50201044, rc, m_pParentWnd, 124 );
			m_pChatViewEdit->ShowScrollBar(SB_VERT, TRUE);
			m_pChatViewEdit->SetFont(&NMBASE::UTIL::GetMyFont(m_nFontIndex));
			rc.OffsetRect( -1000, 0 );
			m_pChatViewEdit->MoveWindow( rc );
			m_pChatViewEdit->ShowWindow( SW_SHOW );
			UINT nMode = m_pChatViewEdit->GetWordWrapMode();
			m_pChatViewEdit->SetWordWrapMode(WBF_WORDWRAP);

			m_bScroll = FALSE;
			m_NowIndex = 0;

			m_bPressBtn = FALSE;
			m_tPressTime = 0;
			m_ptBottomClicked.x = 0;
			m_ptBottomClicked.y = 0;

			m_bStartPressBtn = FALSE;		//������ ������ �÷���
			m_tStartPressTime = 0;		//������ ������ Ÿ��
			m_bShowThumb	= FALSE;

			m_bOutline = false;
			m_clrOutline = RGB(0,0,0);
		}

		CNChatView::~CNChatView()
		{
			if( m_pChatViewEdit != NULL ){
				delete m_pChatViewEdit;
			}
		}

		void CNChatView::SetFontIndex(NMBASE::UTIL::STD_FONT nFontIndex)
		{
			m_nFontIndex = nFontIndex;
			m_pChatViewEdit->SetFont(&NMBASE::UTIL::GetMyFont(m_nFontIndex));
		}

		void CNChatView::ReSetWindowPos(int x, int y)
		{
			m_Xp = x;
			m_Yp = y;
		}

		void CNChatView::OnEnterRoom()
		{
			// 	m_UpBtn.SetSprite(&ChatBoxSpr_sp,10);	
			// 	m_DnBtn.SetSprite(&ChatBoxSpr_sp,11);	
		}

		void CNChatView::BindScrollBtn(NMBASE::GRAPHICGDI::xSprite* pSpr, CPoint ptPos, int nHeight, int nUp, int nDown, int nThumb, int id/* = 8200*/)
		{
			if( pSpr == NULL ) return;

			m_UpBtn.Init(m_pParentWnd , m_pPage,0,0,pSpr,nUp,id);
			m_ThumbBtn.Init(m_pParentWnd , m_pPage,0,0,pSpr,nThumb,id+1);
			m_DnBtn.Init(m_pParentWnd , m_pPage,0,0,pSpr,nDown,id+2);

			//�Ʒ��� ���� �д�.
			int width = m_Width - m_UpBtn.m_Width-3;
			CRect rc;
			rc.SetRect( m_Xp, m_Yp, m_Xp + width, m_Yp +m_Height);
			rc.OffsetRect( -1000, 0 );
			m_pChatViewEdit->MoveWindow( rc );

			//��ũ�� �� ��ư ��ġ ����
			m_nScrollHeight = nHeight;

			m_UpBtn.SetButtonPos(ptPos.x, ptPos.y);
			m_ThumbBtn.SetButtonPos(ptPos.x, ptPos.y + m_nScrollHeight - m_DnBtn.m_Height - m_ThumbBtn.m_Height);
			m_DnBtn.SetButtonPos(ptPos.x, ptPos.y + m_nScrollHeight - m_DnBtn.m_Height);

			m_pSprScroll = pSpr;
			m_bScroll = TRUE;
			m_bShowThumb = FALSE;

			m_UpBtn.Enable(FALSE);
			m_ThumbBtn.Enable(FALSE);
			m_DnBtn.Enable(FALSE);
		}


		void CNChatView::SetResize(int width, int height)
		{
			m_Width = width;
			m_Height = height;	

			int tmpWidth;

			if(m_bScroll)
				tmpWidth = m_Width - m_UpBtn.m_Width-3;
			else
				tmpWidth = m_Width;

			CRect rc;
			rc.SetRect( m_Xp, m_Yp, m_Xp + width, m_Yp +m_Height);
			rc.OffsetRect( -1000, 0 );
			m_pChatViewEdit->MoveWindow( rc );

			if(m_bScroll) {
				m_UpBtn.SetButtonPos(m_Xp + m_Width, 0);
				m_ThumbBtn.SetButtonPos(m_Xp + m_Width, m_UpBtn.m_Height);
				m_DnBtn.SetButtonPos(m_Xp + m_Width, m_nScrollHeight - m_DnBtn.m_Height);
			}	
		}

		BOOL CNChatView::OnMouseOver(POINT pt)
		{	
			if( !IsShow() )return FALSE ;
			if(OnMouseMove(pt)) return TRUE;
			return FALSE;
		}

		BOOL CNChatView::OnMouseMove(POINT pt)
		{
			if(m_UpBtn.OnMouseMove(pt.x,pt.y)) return TRUE;
			if(m_ThumbBtn.OnMouseMove(pt.x,pt.y)) return TRUE;
			if(m_DnBtn.OnMouseMove(pt.x,pt.y)) return TRUE;

			if( !IsShow() ) return FALSE ;


			//	if(m_bPressBtn == TRUE) {
			if( m_ThumbBtn.m_bClick )
			{	
				int totPos = GetTotalLine() - GetOnePageWriteLine();
				if( totPos < 0 )return 1;

				int setY = pt.y - m_ptBottomClicked.y;

				if(setY < m_UpBtn.m_Yp + m_UpBtn.m_Height ) {
					setY = m_UpBtn.m_Yp + m_UpBtn.m_Height;
				}
				else if(setY > m_DnBtn.m_Yp - m_ThumbBtn.m_Height) {
					setY = m_DnBtn.m_Yp - m_ThumbBtn.m_Height;
				}

				m_ThumbBtn.m_Yp = setY;

				//if( m_ThumbBtn.m_Yp == m_UpBtn.m_Yp + m_UpBtn.m_Height ) m_ThumbBtn.m_Yp++;
				//else if(m_ThumbBtn.m_Yp == m_DnBtn.m_Yp - m_ThumbBtn.m_Height) {
				//	m_ThumbBtn.m_Yp--;
				//}			

				CRect rt;
				rt.SetRect( m_UpBtn.m_Xp , m_UpBtn.m_Yp + m_UpBtn.m_Height+1, m_UpBtn.m_Xp + m_UpBtn.m_Width , m_DnBtn.m_Yp-1);

				float height = (float)(m_ThumbBtn.m_Yp -  rt.top) / (rt.Height() - m_ThumbBtn.m_Height);
				int per =  (int)(height * 100) ;			
				int posLine = ( totPos * per ) / 100;			
				m_NowIndex = posLine;			
			}		
			//	}

			return FALSE;
		}

		void CNChatView::AddText( char *pStr, COLORREF crNewColor)
		{
			//	���̰� 0�ΰ��� �߰� �Ұ���
			if( pStr == NULL || strlen( pStr ) == 0 )			return;
			if( m_pChatViewEdit->m_hWnd == NULL )				return;

			int oldtotline = GetTotalLine();

			int wline = GetOnePageWriteLine();
			int totline = GetTotalLine();
			bool bScroll = (m_NowIndex == (totline - wline))?true:false;

			m_pChatViewEdit->AddText( pStr, crNewColor, m_bAutoScroll && bScroll );

			//	��Ŭ�ιٰ� ���ٸ� ó�� ���Ѵ�
			if(m_bScroll == FALSE)
				return;

			//	���������� ���μ�
			totline = GetTotalLine();

			//	�Ѷ��ε� ���������� ���μ����� ���ٸ�
			if( totline <= wline )
			{
				return;
			}
			else
			{
				if(!m_bShowThumb)
				{
					m_bShowThumb = TRUE;
					bScroll = true;
				}

				m_UpBtn.Enable(TRUE);
				m_ThumbBtn.Enable(TRUE);
				m_DnBtn.Enable(TRUE);

			}

			if(m_bAutoScroll && bScroll) m_NowIndex = totline - wline;
			ThumbReposition();
		}


		void CNChatView::AddText(CString *str, COLORREF crNewColor)
		{
			AddText( ( char * )( LPCTSTR )( *str ), crNewColor);
			return;
		}

		int CNChatView::GetOnePageWriteLine()
		{
			int wline = ( m_Height ) / 15;
			return wline;
		}

		int CNChatView::GetTotalLine()
		{
			if(m_pChatViewEdit == NULL)
				return 0;

			if(m_pChatViewEdit->m_hWnd == NULL)
				return 0;

			return m_pChatViewEdit->GetLineCount()-1;
		}


		void CNChatView::UpdateButtonState( int nCmd )
		{	
			//	ä������ ��ũ�� �� ��ư
			if( nCmd == m_UpBtn.m_CmndID ){

				OnScrollButtonUp();
			}
			//	ä������ ��ũ�� �ٿ� ��ư
			else if( nCmd == m_DnBtn.m_CmndID ){		
				OnScrollButtonDown();
			}	

			return;
		}

		BOOL CNChatView::OnPressProcess()
		{
			if( m_bStartPressBtn == FALSE && m_bPressBtn == FALSE ){
				m_tStartPressTime = timeGetTime();
				m_bStartPressBtn = TRUE;
			}


			DWORD CurTime = timeGetTime() - m_tStartPressTime;

			if (CurTime > 100 ){
				if( m_bPressBtn == FALSE ){
					m_tPressTime = timeGetTime();
					m_bPressBtn = TRUE;
				}
			}

			if( m_tPressTime > 0 ){
				CurTime = timeGetTime() - m_tStartPressTime;

				if( CurTime > 100 ){			
					m_tPressTime = timeGetTime();
					return TRUE;
				}
			}

			return FALSE;
		}
		BOOL CNChatView::OnLButtonDown(int x, int y)
		{
			if( !IsShow() )return FALSE ;
			if( m_UpBtn.OnLButtonDown(x,y) )
			{
				if( OnPressProcess() ) 
				{
					UpdateButtonState( m_UpBtn.m_CmndID );
				}
				else{
					UpdateButtonState( m_UpBtn.m_CmndID );
				}		
				return TRUE;
			}
			else if ( m_bShowThumb && m_ThumbBtn.OnLButtonDown(x,y) )
			{		
				m_ptBottomClicked.x = x - m_ThumbBtn.m_Xp;
				m_ptBottomClicked.y = y - m_ThumbBtn.m_Yp;		
				return TRUE;
			}
			else if( m_DnBtn.OnLButtonDown(x, y))
			{
				if( OnPressProcess() ) 
				{
					UpdateButtonState( m_DnBtn.m_CmndID );
				}
				else{
					UpdateButtonState( m_DnBtn.m_CmndID );
				}		
				return TRUE;
			}

			return FALSE;
		}

		BOOL CNChatView::OnLButtonUp(int x, int y)
		{	
			m_bPressBtn = FALSE;
			m_tPressTime = 0;
			m_bStartPressBtn = FALSE;		//������ ������ �÷���
			m_tStartPressTime = 0;		//������ ������ Ÿ��

			if( m_UpBtn.OnLButtonUp(x,y) )return 1;
			if( m_bShowThumb && m_ThumbBtn.OnLButtonUp(x,y) )return 1;
			if( m_DnBtn.OnLButtonUp(x,y) )return 1;	
			return FALSE;
		}

		void CNChatView::ThumbReposition(void)
		{
			int totPos = GetTotalLine() - GetOnePageWriteLine();

			float fRate = 1.0f - float(totPos - m_NowIndex) / float(totPos);

			int nTop = m_UpBtn.m_Yp + m_UpBtn.m_Height;
			int nBottom = m_DnBtn.m_Yp - m_ThumbBtn.m_Height;

			m_ThumbBtn.m_Yp =  nTop + (int)((float)(nBottom - nTop) * fRate);
		}

		void CNChatView::OnScrollButtonUp()
		{
			int totline = GetTotalLine();
			//	���������� ���μ�
			int wline = GetOnePageWriteLine();
			if( wline <= 0 )		wline = 1;

			if( totline < wline )		return;

			m_NowIndex--;
			if( m_NowIndex < 0 )
			{
				m_NowIndex = 0;	
			}
			else ThumbReposition();
		}

		void CNChatView::OnScrollButtonDown()
		{
			int totline = GetTotalLine();
			//	���������� ���μ�
			int wline = GetOnePageWriteLine();
			if( wline <= 0 )		wline = 1;

			if( totline < wline )		return;

			m_NowIndex++;

			if( m_NowIndex > (totline - wline))
			{
				m_NowIndex = totline - wline;
			}
			else ThumbReposition();
		}

		void CNChatView::SetScrollBar()
		{
		}

		void CNChatView::ResetChat()
		{	
			if(m_pChatViewEdit == NULL)
				return;

			if(m_pChatViewEdit->m_hWnd == NULL)
				return;

			m_pChatViewEdit->SetWindowText("");
			m_pChatViewEdit->ShowScrollBar(SB_VERT, TRUE);

			m_NowIndex = 0;
			ThumbReposition();

			m_bScroll = TRUE;
			m_bShowThumb = FALSE;

			m_UpBtn.Enable(FALSE);
			m_ThumbBtn.Enable(FALSE);
			m_DnBtn.Enable(FALSE);
		}

		void CNChatView::SetOutlineColor(COLORREF clrOutline)
		{
			m_bOutline = true;
			m_clrOutline = clrOutline;
		}

		void CNChatView::DrawChatText(CDC *pDC)
		{
			pDC->SetBkMode( TRANSPARENT );
			pDC->SetTextColor( RGB( 78, 78, 78) );
			pDC->SelectObject( &NMBASE::UTIL::GetMyFont(m_nFontIndex));	

			CRect rc;
			int totline = GetTotalLine();
			if( totline == 0 )		return ;

			rc.SetRect( m_Xp, m_Yp+8, m_Xp + m_Width, m_Yp + 8 + 12);

			CString str;

			//	���������� ���μ�
			int wline = GetOnePageWriteLine();
			if( wline <= 0 )		return ;

			int line=0;

			for( int i = 0; i < wline; i++ )
			{

				line = i + m_NowIndex;
				if( line >= totline )
					return ;

				// ����2000�� �Ϻ� 98 �ý��ۿ��� ���ڰ� �����°� �����ϱ� ���� ó��
				char buf[ 256 ] = { 0, };
				m_pChatViewEdit->GetLine( line, buf, 255 );

				int slen = strlen( buf );
				if(slen > 255){
					buf[ 255] = 0;
				}

				if( slen > 0 )
				{
					// ���� ���� �ٴ� �������� �ڵ带 ����
					// ���� 2000�� ��� ������ ���� ����� �������� ����
					// if(buf[slen-2]==13) buf[slen-2]=0;

					for( int k = 0; k <= slen; k++ )
					{
						if( buf[ k ] == 10 ) buf[ k ] = 0;
						if( buf[ k ] == -1 ) buf[ k ] = 0;
						if( buf[ k ] == 13 ) buf[ k ] = 0;

						// �����ڵ尡 �ƴϸ� ������ ���������
						//if(buf[k]>=0 && buf[k]<=31) buf[k]=0;
					}

					// ù��° ���ڿ� ���� ���δ����� ������ �����Ѵ�
					int nindex = m_pChatViewEdit->LineIndex( line );
					CHARFORMAT cf;
					cf.cbSize = sizeof( CHARFORMAT );
					cf.dwMask = CFM_COLOR;
					m_pChatViewEdit->SetSel( nindex, nindex );			
					m_pChatViewEdit->GetSelectionCharFormat( cf );

					//���߱� ������ �Ǹ� TRUE���� �Ǿ� �ִ� �͵鸸 ����� 
					//�Ʒ� ���ڸ� ������ .NET  ������ ��� �����ϴ�.
					//			if(cf.dwReserved == 1 && m_bHideText == TRUE ){
					//				continue;
					//			}

					str.Format( "%s", buf );

					if(m_bOutline)
					{
						pDC->SetTextColor( m_clrOutline );
						rc.OffsetRect(1,0);
						pDC->DrawText( str, &rc, DT_LEFT );
						rc.OffsetRect(0,2);
						pDC->DrawText( str, &rc, DT_LEFT );
						rc.OffsetRect(-1,-1);
						pDC->DrawText( str, &rc, DT_LEFT );
						rc.OffsetRect(2,0);
						pDC->DrawText( str, &rc, DT_LEFT );

						// ���� �������̸� ���� ������� ����ְ� �ٸ����̶�� �׻� �״�� ����
						if( cf.crTextColor == 0 )	pDC->SetTextColor( RGB( 78, 78, 78) );
						else pDC->SetTextColor( cf.crTextColor );

						rc.OffsetRect(-1,0);
						pDC->DrawText( str, &rc, DT_LEFT );

						rc.OffsetRect(-1,-1);
					}
					else
					{
						// ���� �������̸� ���� ������� ����ְ� �ٸ����̶�� �׻� �״�� ����
						if( cf.crTextColor == 0 )	pDC->SetTextColor( RGB( 78, 78, 78) );
						else pDC->SetTextColor( cf.crTextColor );

						pDC->DrawText( str, &rc, DT_LEFT );
					}
				}

				rc.OffsetRect( 0, 14 );
			}
		}

		void CNChatView::Draw(CDC *pDC)
		{
			if( !IsShow() )return;
#ifdef _DEBUG
			// 	CRect rt;
			// 	rt.SetRect(0,0,m_Width,m_Height);
			// 	rt.OffsetRect(m_Xp,m_Yp);
			// 	m_pPage->DrawFillBoxGB(rt.left,rt.top,rt.right,rt.bottom,RGBmix( 0, 2, 5 ),3);
#endif


			if( pDC == NULL )return;	

			DrawChatText(pDC);

			m_UpBtn.Draw(pDC);
			if(m_bShowThumb) m_ThumbBtn.Draw(pDC);
			m_DnBtn.Draw(pDC);
		}



		// ���콺 �� ó��
		BOOL CNChatView::OnMouseWheel( WPARAM wParam, LPARAM lParam )
		{
			// lParam ��ǥ
			// LOWORD(wParam) Ű�������
			// HIWORD(wParam) ���� �������Ÿ�
			if( !IsShow() ) return FALSE;
			if(!m_bShowThumb) return FALSE;

			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			if (m_pParentWnd)
				m_pParentWnd->ScreenToClient(&pt);

			CRect rt;
			rt.SetRect(0,0,m_Width,m_Height);
			rt.OffsetRect(m_Xp,m_Yp);
			if( !rt.PtInRect(pt) ) return FALSE;

			short status = LOWORD(wParam);
			short lenth  = HIWORD(wParam);

			if( lenth >= 120 )	{
				UpdateButtonState( m_UpBtn.m_CmndID );
			}
			else {
				UpdateButtonState( m_DnBtn.m_CmndID );
			}

			return TRUE;
		}

	} //namespace SKINGDI

}//namespace NMBASE