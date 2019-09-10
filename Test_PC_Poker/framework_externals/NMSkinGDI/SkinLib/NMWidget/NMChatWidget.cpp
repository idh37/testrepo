#include "stdafx.h"
#include "./NMWidget/NMChatWidget.h"


namespace NMBASE
{
	namespace SKINGDI
	{
		/////////////////////////////////////////////////////////////////////////////
		// CNMCommonRichEdit
		CNMCommonRichEdit::CNMCommonRichEdit()
		{
			m_pParent = NULL;
		}

		CNMCommonRichEdit::~CNMCommonRichEdit()
		{
		}


		BEGIN_MESSAGE_MAP(CNMCommonRichEdit, CRichEditCtrl)
			//{{AFX_MSG_MAP(CNMCommonRichEdit)
			ON_WM_SETFOCUS()
			//}}AFX_MSG_MAP
		END_MESSAGE_MAP()

		/////////////////////////////////////////////////////////////////////////////
		// CNMCommonRichEdit message handlers

		BOOL CNMCommonRichEdit::Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID )
		{
			if (!AfxInitRichEdit())
				return FALSE;

			CWnd* pWnd = this;
			return pWnd->Create( _T("RICHEDIT"), NULL, dwStyle, rect, pParentWnd, nID );
		}

		BOOL CNMCommonRichEdit::Initialize( INMWidget* pParent )
		{
			m_pParent = pParent;
			return TRUE;
		}


		// void CNMCommonRichEdit::AddText( CString* pstrText, COLORREF crText, int nAttri )
		// {
		// 	AddText( (char*)pstrText->operator LPCTSTR(), crText, nAttri );
		// }

		void CNMCommonRichEdit::AddText( const char* szText, COLORREF crText, int nAttri )
		{
			int iStartPos = LineIndex( max( GetLineCount()-1, 0 )  );
			int iTotalTextLength = GetWindowTextLength();
			SetSel(-1, -1);

			CHARFORMAT cf;
			cf.cbSize		= sizeof(CHARFORMAT);
			cf.dwMask		= CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;

			// ���� boldü�� ����Ϸ��� �Ķ���Ͱ����� bBold���� �޾ƿ��� �̺κ��� �ּ��� Ǭ��.
			//	if (bBold)
			//		cf.dwEffects =(unsigned long) ~( CFE_AUTOCOLOR | CFE_UNDERLINE) | CFE_BOLD;
			//	else
			cf.dwEffects =(unsigned long) ~( CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
			cf.crTextColor	= crText;

			SetSelectionCharFormat(cf);
			ReplaceSel(szText);
			HideSelection( TRUE, FALSE );	

			const int MAX_RICHCHARSIZE = 8192;
			// Over Flow ���� �� �߰��� �� ��ŭ ���ٿ��� �����Ѵ�
			if( iTotalTextLength > MAX_RICHCHARSIZE )
			{
				int iEndPos = iStartPos + strlen(szText);
				SetSel( 0, iEndPos - iStartPos );
				ReplaceSel("");
			}
		}

		void CNMCommonRichEdit::ClearAllData()
		{
			SetWindowText( "" );
		}

		void CNMCommonRichEdit::OnSetFocus(CWnd* pOldWnd) 
		{
			CRichEditCtrl::OnSetFocus(pOldWnd);
			// TODO: Add your message handler code here
			::SetFocus( pOldWnd->m_hWnd );
		}

		//////////////////////////////////////////////////////////////////////////////
		// CNMChatWidget
		CNMChatWidget::CNMChatWidget()
		{
			m_nControlID = WIDGET_ID_NULL;
			m_pParent = NULL;
			m_rectPosition.SetRect(0,0,0,0);

			m_nLinePerPage = 10;
			m_nLinePerLimit = 10;
			m_nCurrentLine = 0;

			m_pFont = NULL;
		}

		CNMChatWidget::~CNMChatWidget()
		{
		}

		BOOL CNMChatWidget::Initialize( int nControlID, CWnd* pParent, CRect rectEdit, int nLinePerPage, int nLinePerLimit )
		{
			m_nControlID = nControlID;
			m_pParent = pParent;
			m_rectPosition = rectEdit;

			m_nLinePerPage = nLinePerPage;
			m_nLinePerLimit = nLinePerLimit;

			if( m_CommonRichEdit.m_hWnd == NULL )
			{
				// window edit ����
				if( m_CommonRichEdit.Create( ES_READONLY | ES_AUTOVSCROLL | ES_WANTRETURN | ES_MULTILINE,
					m_rectPosition,
					m_pParent,
					nControlID ) == FALSE )
					return FALSE;
				m_CommonRichEdit.Initialize( this );

				// window richedit�� �ʺ��̴� ������ �Ű� ���´�.
				CRect rectTemp;
				m_CommonRichEdit.GetClientRect( rectTemp );
				rectTemp.OffsetRect( -1000, 0 );
				m_CommonRichEdit.MoveWindow( rectTemp );
				m_CommonRichEdit.ShowWindow( SW_SHOW );
			}

			return TRUE;
		}

		void CNMChatWidget::SetPosition( int nPosX, int nPosY )
		{
			int nWidth = m_rectPosition.Width();
			int nHeight = m_rectPosition.Height();
			m_rectPosition.SetRect( nPosX, nPosY, nPosX + nWidth, nPosY + nHeight );
		}

		void CNMChatWidget::SetFont( CFont* pFont )
		{
			m_pFont = pFont;
			m_CommonRichEdit.SetFont( pFont );
		}

		void CNMChatWidget::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
		{
			if( m_CommonRichEdit.m_hWnd == NULL )	return;

#ifdef _DEBUG
			//	Page.DrawEmptyBox( m_rectPosition.left, m_rectPosition.top, m_rectPosition.right, m_rectPosition.bottom, RGB555mix(0,31,0) );
#endif

			int nTotline = m_CommonRichEdit.GetLineCount() - 1;
			if( nTotline == 0 )		return;

			DC.SelectObject( m_pFont );

			CRect rectTmp = m_rectPosition;

			if( m_nCurrentLine < 0 )	m_nCurrentLine = 0;
			int nLine;
			for( int i=0; i<m_nLinePerPage; i++ )
			{
				nLine = i + m_nCurrentLine;
				if( nLine > nTotline )	return;

				// ����2000�� �Ϻ� 98 �ý��ۿ��� ���ڰ� �����°� �����ϱ� ���� ó��
				char szBuf[256] = {0,};
				m_CommonRichEdit.GetLine( nLine, szBuf, 255 );
				szBuf[255] = 0;

				int nLineLength = strlen( szBuf );
				if( nLineLength > 0 )
				{
					// ���� ���� �ٴ� �������� �ڵ带 ����
					// ���� 2000�� ��� ������ ���� ����� �������� ����
					// if( szBuf[nLineLength-2]==13 ) szBuf[nLineLength-2] = 0;
					for( int c=0; c<=nLineLength; c++ )
					{
						if( szBuf[c] == 10 ) szBuf[c] = 0;
						if( szBuf[c] == -1 ) szBuf[c] = 0;
						if( szBuf[c] == 13 ) szBuf[c] = 0;
						// �����ڵ尡 �ƴϸ� ������ ���������
						// if( szBuf[c]>=0 && szBuf[c]<=31 ) szBuf[c]=0;
					}

					// ù��° ���ڿ� ���� ���δ����� ������ �����Ѵ�
					int nIndex = m_CommonRichEdit.LineIndex( nLine );
					CHARFORMAT cf;
					cf.cbSize = sizeof( CHARFORMAT );
					cf.dwMask = CFM_COLOR;
					m_CommonRichEdit.SetSel( nIndex, nIndex );
					m_CommonRichEdit.GetSelectionCharFormat( cf );
					DC.SetTextColor( cf.crTextColor );

					DC.DrawText( szBuf, &rectTmp, DT_LEFT );
					SIZE sz = DC.GetTextExtent( szBuf );

					rectTmp.OffsetRect( 0, sz.cy+2 );
				}
			}

			return CNMWidgetComposite::DrawWidget(Page, DC);
		}

		void CNMChatWidget::InsertData( CString* pstrText, COLORREF crText, int nAttri )
		{
			InsertData( (char*)(pstrText->operator LPCTSTR()), crText, nAttri );
		}

		void CNMChatWidget::InsertData(const char* szText, COLORREF crText, int nAttri )
		{
			if( szText == NULL || strlen( szText ) == 0 )	return;
			if( m_CommonRichEdit.m_hWnd == NULL )			return;

			m_CommonRichEdit.AddText( szText, crText, nAttri );

			NMSendMessage( m_pParent, m_nControlID, m_CommonRichEdit.GetLineCount()-1, m_nCurrentLine );
		}

		void CNMChatWidget::ClearAllData()
		{
			if( m_CommonRichEdit.m_hWnd == NULL )		return;

			m_CommonRichEdit.ClearAllData();

			NMSendMessage( m_pParent, m_nControlID, m_CommonRichEdit.GetLineCount()-1, m_nCurrentLine );
		}

		void CNMChatWidget::SetCurrentLine( int nLine )
		{
			m_nCurrentLine = nLine;
		}


	} //namespace SKINGDI

}//namespace NMBASE