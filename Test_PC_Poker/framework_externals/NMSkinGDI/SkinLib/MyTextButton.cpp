// MyTextButton.cpp: implementation of the CMyTextButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyTextButton.h"

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

		CMyTextButton::CMyTextButton()
		{
		}

		CMyTextButton::~CMyTextButton()
		{
		}

		void CMyTextButton::Draw(CDC& dc)
		{
			CMyButton::Draw( dc );

			dc.SetBkMode( TRANSPARENT );

			CRect rc;
			for( int i = 0; i < static_cast<int>(m_TextInfo.size()); i++ )
			{
				dc.SetTextColor( m_TextInfo[i].Color );
				dc.SelectObject( m_TextInfo[i].pFont );

				rc = m_TextInfo[i].rc;
				if( 3 == nState ) rc.OffsetRect( 1, 1 );

				if( m_TextInfo[i].bLayered ) DrawLayeredStr( m_TextInfo[i].Text, rc, m_TextInfo[i].Color, m_TextInfo[i].BackColor, dc, m_TextInfo[i].format );
				else dc.DrawText( m_TextInfo[i].Text, rc, m_TextInfo[i].format );
			}

		}

		void CMyTextButton::DrawLayeredStr( CString& str, CRect& rc, COLORREF MainColor, COLORREF ShellColor, CDC& dc, DWORD format )
		{
			dc.SetTextColor( ShellColor );
			rc.OffsetRect( -1, 0 );		dc.DrawText( str, rc, format );
			rc.OffsetRect( 2, 0 );		dc.DrawText( str, rc, format );
			rc.OffsetRect( -1, -1 );	dc.DrawText( str, rc, format );
			rc.OffsetRect( 0, 2 );		dc.DrawText( str, rc, format );

			dc.SetTextColor( MainColor );
			rc.OffsetRect( 0, -1 );	dc.DrawText( str, rc, format );

		}

		void CMyTextButton::SetText( const char* pStr, CRect& rc, COLORREF color, CFont* pFont, BOOL bLayered, COLORREF BackColor, DWORD format )
		{
			if( pStr ) 
			{
				BUTTON_TEXT_INFO info;

				info.rc = rc;
				info.Text.Format( "%s", pStr );
				info.pFont = pFont;
				info.Color = color;

				info.bLayered = bLayered;
				info.BackColor = BackColor;
				info.format = format;

				m_TextInfo.push_back( info );
			}
		}

	} //namespace SKINGDI

}//namespace NMBASE