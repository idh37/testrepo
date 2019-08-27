// IconList.cpp: implementation of the CIconList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "IconList.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIconObject::CIconObject()
{
	m_pSpr = NULL;
	m_State = ICS_NULL;
	
	m_SprIndex = 0;
	
	m_Pt.x = 0; m_Pt.y = 0;
	m_ToolTip_Gab = m_ToolTipPos = m_Pt;

	m_ToolTipIndex = -1;
	m_Cmd = 0;
	m_DataVal = 0;
}

CIconObject::~CIconObject()
{

}

void CIconObject::Init( xSprite* pSpr, CPoint BasePos, int Cmd, int SprIndex, int TooltipIndex, IC_STATE state )
{
	m_Cmd = Cmd;
	m_pSpr = pSpr;
	m_Pt = BasePos;
	m_State = state;
	m_SprIndex = SprIndex;
	m_ToolTipIndex = TooltipIndex;
}

void CIconObject::Draw(CDC& DC,  NMBASE::GRAPHICGDI::CPage& Page, CPoint& mouse_pt )
{
	if (m_State==ICS_HIDE) return;

	Page.PutSprAuto( m_Pt.x, m_Pt.y, m_pSpr, m_SprIndex + (int)m_State );

	//< CYS 100814
	//보유한도초과금
	if ( m_ToolTipIndex < 0 )
	{
		return;
	}
	//> CYS 100814

	if( PtInRect( mouse_pt ) )
	{
		Page.PutSprAuto( m_ToolTipPos.x, m_ToolTipPos.y, m_pSpr, m_ToolTipIndex, AB, 31 );

		if (m_strText.IsEmpty()==FALSE)
		{
			DC.SetBkMode(TRANSPARENT);
			DC.SetTextColor(m_TextColor);
			DC.DrawText(m_strText, &m_TextRect, DT_CENTER);
		}
	}
}

void CIconObject::SetState( IC_STATE st )
{
	m_State = st;
}

BOOL CIconObject::PtInRect( CPoint pt )
{
	ASSERT( m_pSpr );
	if( NULL == m_pSpr ) return FALSE;
	if (m_State==ICS_HIDE)	return FALSE;
	if( m_SprIndex + m_State >= m_pSpr->sfhead.TotalSprNum ) return FALSE;

	CRect rc( m_Pt.x, m_Pt.y, m_Pt.x + m_pSpr->spr[m_SprIndex + m_State].xl, m_Pt.y + m_pSpr->spr[m_SprIndex + m_State].yl );
	return rc.PtInRect( pt );
}

void CIconObject::SetToolTip( int Index )
{
	m_ToolTipIndex = Index;	
}

void CIconObject::SetToolTip( int index, CPoint ToolTipPos )
{
	m_ToolTipIndex = index;
	m_ToolTipPos = ToolTipPos;

	m_ToolTip_Gab = m_ToolTipPos - m_Pt;
}

void CIconObject::RePostion(int x, int y)
{
	m_Pt.x = x;
	m_Pt.y = y;
	m_ToolTipPos = m_Pt;
	m_ToolTipPos += m_ToolTip_Gab;
}


void CIconObject::SendCommand( HWND hwnd )
{
	if (hwnd)
		::SendMessage( hwnd, WM_COMMAND, m_Cmd, (LPARAM)m_State );
}


//자리 지킴이 
void CIconObject::SetToolTip(int index, CPoint ToolTipPos, CRect &Rect, const char *pText, COLORREF color)//툴팁에 시스템 폰트를 찍을 필요가 있을때
{
	m_ToolTipIndex = index;
	m_ToolTipPos = ToolTipPos;

	if (pText)
		m_strText=pText;

	m_TextRect = Rect;
	m_TextColor = color;

}



CIconList::CIconList()
{
	m_MousePt.x = 0;
	m_MousePt.y = 0;
	m_Clicked_Index = -1;

	m_hParentWnd = NULL;
	m_Icons.clear();
}

CIconList::~CIconList()
{
	for( int i = 0; i <  static_cast< int >( m_Icons.size() ); i++ )
	{
		delete m_Icons[i];
	}

	m_Icons.clear();
}

void CIconList::Init( HWND hParenWnd )
{
	m_hParentWnd = hParenWnd;
	m_hCursor[0] = LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND) );
	m_hCursor[1] = LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND) );
}

int CIconList::AddIcon( xSprite* pSpr, CPoint BasePos, int Cmd, int SprIndex , int TooltipIndex , IC_STATE state )
{
	CIconObject* pObj = new CIconObject;
	pObj->Init( pSpr, BasePos, Cmd, SprIndex, TooltipIndex, state );
	m_Icons.push_back( pObj );
	return ( static_cast< int >( m_Icons.size() ) - 1);
}

CIconObject* CIconList::GetIcon( int index )
{
	if( index < 0 || index >=  static_cast< int >( m_Icons.size() ) ) return NULL;

	return m_Icons[index];
}

void CIconList::SetIconState( int type, IC_STATE state )
{
	if( type < 0 || type >=  static_cast< int >( m_Icons.size() ) ) return;
	m_Icons[type]->SetState( state );
}

void CIconList::Draw( CDC& DC, NMBASE::GRAPHICGDI::CPage& Page, CPoint mouse_pt )
{
	if (g_bMyObserver) return;

	m_MousePt = mouse_pt;
	for( int i = 0; i <  static_cast< int >( m_Icons.size() ); i++ )
	{
		if( !m_Icons[i]->IsValid() )continue;
		m_Icons[i]->Draw( DC, Page, m_MousePt );
	}
}

BOOL CIconList::OnMouseMove( int x, int y )
{
	if (g_bMyObserver) return FALSE;

	m_MousePt.x = x;
	m_MousePt.y = y;

	for( int i = 0; i <  static_cast< int >( m_Icons.size() ); i++ )
	{
		if( !m_Icons[i]->IsValid() )continue;
		if( m_Icons[i]->PtInRect( m_MousePt ) )
		{
			SetCursor( m_hCursor[0] );
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CIconList::OnLButtonDown( int x, int y )
{
	if (g_bMyObserver) return FALSE;


	m_MousePt.x = x; m_MousePt.y = y;

	for( int i = 0; i <  static_cast< int >( m_Icons.size() ); i++ )
	{
		if( !m_Icons[i]->IsValid() )continue;
		if( m_Icons[i]->PtInRect( m_MousePt ) )
		{
			SetCursor( m_hCursor[1] );
			m_Clicked_Index = i;
			return TRUE;
		}
	}

	m_Clicked_Index = -1;

	return FALSE;
}

BOOL CIconList::OnLButtonUp( int x, int y )
{
	if (g_bMyObserver) return FALSE;

	m_MousePt.x = x; m_MousePt.y = y;
	
	if( m_Clicked_Index >= 0 && m_Clicked_Index <  static_cast< int >( m_Icons.size() ) )
	{
		if( !m_Icons[m_Clicked_Index]->IsValid() )	return FALSE;

		if( m_Icons[m_Clicked_Index]->PtInRect( m_MousePt ) )
		{
			m_Icons[m_Clicked_Index]->SendCommand( m_hParentWnd );
			return TRUE;
		}
	}

	return FALSE;
}
