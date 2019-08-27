#include "stdafx.h"
#include "./NMWidget/NMButton.h"
#include "./NMWidget/NMResourceManager.h"

#define _RGB16BIT555(r,g,b) ((b & 31) + ((g & 31) << 5) + ((r & 31) << 10))
#define _RGB16BIT565(r,g,b) ((b & 31) + ((g & 63) << 5) + ((r & 31) << 11))

namespace NMBASE
{
	namespace SKINGDI
	{


		//////////////////////////////////////////////////////////////////////////////
		// CNMInvisibleButton
		CNMInvisibleButton::CNMInvisibleButton()
		{
			m_nControlID = WIDGET_ID_NULL;
			m_pParent = NULL;
			m_rectPosition.SetRect(0,0,0,0);
			m_rectBasePosition.SetRect(0,0,0,0);
			m_nWidth = m_nHeight = 0;

			m_bEnable = FALSE;
			m_nState = BUTTON_STATE_NORMAL;
		}

		CNMInvisibleButton::~CNMInvisibleButton()
		{

		}

		BOOL CNMInvisibleButton::Initialize( int nControlID,
			CWnd* pParent,
			int nPosX, int nPosY,
			int nWidth, int nHeight )
		{
			m_nControlID = nControlID;

			if( pParent == NULL )
				return FALSE;

			m_pParent = pParent;

			m_nWidth = nWidth;
			m_nHeight = nHeight;
			m_rectPosition.SetRect( nPosX, nPosY, nPosX + nWidth, nPosY + nHeight );
			m_rectBasePosition = m_rectPosition;

			SetEnable( TRUE );

			return TRUE;
		}
		void CNMInvisibleButton::SetControlID(int nControlID)
		{
			m_nControlID = nControlID;
		}

		void CNMInvisibleButton::SetEnable( BOOL bFlag )
		{
			m_bEnable = bFlag;
		}

		BOOL CNMInvisibleButton::IsInRect( CPoint point )
		{
			if( m_bEnable == FALSE )
			{
				return FALSE;
			}

			if( m_rectPosition.PtInRect( point ) )
				return TRUE;

			return FALSE;
		}

		BOOL CNMInvisibleButton::OnMouseMoveWidget( UINT nFlags, CPoint point )
		{
			if( m_bEnable == FALSE )
			{
				return FALSE;
			}

			if( IsInRect( point ) )
			{
				m_nState = BUTTON_STATE_OVER;
				return TRUE;
			}

			return FALSE;
		}

		BOOL CNMInvisibleButton::OnMouseLeaveWidget( UINT nFlags, CPoint point )
		{
			if( m_bEnable == FALSE )
			{
				return FALSE;
			}

			m_nState = BUTTON_STATE_NORMAL;

			return FALSE;
		}

		BOOL CNMInvisibleButton::OnLButtonDownWidget( UINT nFlags, CPoint point )
		{
			if( m_bEnable == FALSE )
			{
				return FALSE;
			}

			if( IsInRect( point ) )
			{
				m_nState = BUTTON_STATE_DOWN;
				return TRUE;
			}

			return FALSE;
		}

		BOOL CNMInvisibleButton::OnLButtonUpWidget( UINT nFlags, CPoint point )
		{
			if( m_bEnable == FALSE )
			{
				return FALSE;
			}

			if( IsInRect( point ) )
			{
				if( m_nState == BUTTON_STATE_DOWN )
				{
					NMSendMessage( m_pParent, m_nControlID );
				}
				m_nState = BUTTON_STATE_NORMAL;

				return TRUE;
			}

			return FALSE;
		}

		//////////////////////////////////////////////////////////////////////////////
		// CNMButton
		CNMButton::CNMButton()
		{
			m_pSprite = NULL;
			m_nSprID = 0;
			m_nSprCount = 1;
			m_nCurSprID = 0;

			m_bOver = FALSE;
			m_dwOverTick = 0;
			m_bDown = FALSE;
			m_dwDownTick = 0;
			m_strTootip = "";
			m_nTooltipOffsetX = m_nTooltipOffsetY = 0;

			m_strOverSnd = "";
			m_strDownSnd = "";

			m_wParam = 0;
			m_lParam = 0;

			m_bShow=TRUE;
		}

		CNMButton::~CNMButton()
		{
		}

		BOOL CNMButton::Initialize( int nControlID,
			CWnd* pParent,
			int nPosX, int nPosY,
			int nWidth, int nHeight )
		{
			m_bShow=TRUE;
			return CNMInvisibleButton::Initialize( nControlID, pParent, nPosX, nPosY, nWidth, nHeight );
		}

		BOOL CNMButton::Initialize( int nControlID,
			CWnd* pParent,
			int nPosX, int nPosY,
			char* szSprKey,
			int nSprID,
			int nSprCount )
		{
			if( Initialize( nControlID, pParent, nPosX, nPosY, 0, 0 ) == FALSE )
				return FALSE;

			if( SetSprite( szSprKey, nSprID, nSprCount ) == FALSE )
			{
				m_bEnable = FALSE;
				return FALSE;
			}
			m_rectBasePosition = m_rectPosition;

			m_bEnable = TRUE;
			m_bShow=TRUE;

			return TRUE;
		}

		BOOL CNMButton::SetSprite( const char* szSprKey, int nSprID, int nSprCount )
		{
			m_pSprite = NMRESSM.GetSprite( szSprKey );
			if( m_pSprite == NULL )
				return FALSE;

			m_nSprID = nSprID;

			if( nSprCount < 1 || nSprCount > 4 )
				return FALSE;

			m_nSprCount = nSprCount;

			m_nCurSprID = nSprID;

			if( m_pSprite->spr == NULL )
				return FALSE;

			int nPosX = m_rectPosition.left;
			int nPosY = m_rectPosition.top;
			int nBaseSprID = m_nSprID;
			if( nBaseSprID >= m_pSprite->GetTotalSpr() )	nBaseSprID = 0;
			m_nWidth = m_pSprite->spr[nBaseSprID].xl;
			m_nHeight = m_pSprite->spr[nBaseSprID].yl;
			m_rectPosition.SetRect( nPosX, nPosY, nPosX + m_nWidth, nPosY + m_nHeight );

			return TRUE;
		}

		void CNMButton::SetSound( char* szOver, char* szDown )
		{
			m_strOverSnd = szOver;
			m_strDownSnd = szDown;
		}

		void CNMButton::SetTooltip(  char* szTooTip, int nTooltipOffsetX, int nTooltipOffsetY )
		{
			m_strTootip = szTooTip;
			m_nTooltipOffsetX = nTooltipOffsetX;
			m_nTooltipOffsetY = nTooltipOffsetY;
		}

		void CNMButton::SetSendParam( WPARAM wParam, LPARAM lParam )
		{
			m_wParam = wParam;
			m_lParam = lParam;
		}

		void CNMButton::SetPosition( int nPosX, int nPosY )
		{	
			m_rectBasePosition.SetRect( nPosX, nPosY, nPosX + m_nWidth, nPosY + m_nHeight );
			m_rectPosition = m_rectBasePosition;
		}

		void CNMButton::SetRelationPosition( int nOffsetX, int nOffsetY )
		{
			m_rectPosition.SetRect( m_rectBasePosition.left,
				m_rectBasePosition.top,
				m_rectBasePosition.left + m_nWidth,
				m_rectBasePosition.top + m_nHeight );
			m_rectPosition.OffsetRect( nOffsetX, nOffsetY );
		}

		void CNMButton::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
		{
			if (m_bShow==FALSE)
				return;

			if( m_pSprite != NULL )
			{
				Page.PutSprAuto( m_rectPosition.left, m_rectPosition.top, m_pSprite, m_nCurSprID , AB);
			}

			//Page.DrawEmptyBox(m_rectBasePosition.left, m_rectBasePosition.top, m_rectBasePosition.right, m_rectBasePosition.bottom, 24);

			DrawToolTip( Page, DC );
		}

		void CNMButton::DrawToolTip( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
		{
			if (m_bShow==FALSE)	return;
			if( m_strTootip.length() == 0 )		return;
			if( IsDrawAbleToolTip() == FALSE )	return;

			CFont *pOldFont = DC.SelectObject( &NMBASE::UTIL::GetMyFont( NMBASE::UTIL::FONT_11D_NORMAL ) );
			SIZE sz = DC.GetTextExtent( m_strTootip.c_str() );

			int nPosX = 0;
			switch( m_nTooltipOffsetX )
			{
			case BUTTON_TOOLTIP_HL:
				{
					nPosX = m_rectPosition.left + 4;
				} break;
			case BUTTON_TOOLTIP_HC:
				{
					nPosX = m_rectPosition.left + (m_rectPosition.Width() - sz.cx)/2;
				} break;
			case BUTTON_TOOLTIP_HR:
				{
					nPosX = m_rectPosition.right - sz.cx - 4;
				} break;
			default:
				{
					nPosX = m_rectPosition.left + m_nTooltipOffsetX;
				} break;
			}
			int nPosY = 0;
			switch( m_nTooltipOffsetY )
			{
			case BUTTON_TOOLTIP_VT:
				{
					nPosY = m_rectPosition.top - sz.cy - 4;
				} break;
			case BUTTON_TOOLTIP_VC:
				{
					nPosY = m_rectPosition.top + (m_rectPosition.Height() - sz.cy)/2;
				} break;
			case BUTTON_TOOLTIP_VB:
				{
					nPosY = m_rectPosition.bottom + 4;
				} break;
			default:
				{
					nPosY = m_rectPosition.top + m_nTooltipOffsetY;
				} break;
			}

			Page.DrawFillBoxGB( nPosX - 1, nPosY - 1, nPosX + sz.cx + 1, nPosY + sz.cy + 1, _RGB16BIT555(0,0,0), 20 );
			Page.DrawEmptyBox( nPosX - 2, nPosY - 2, nPosX + sz.cx + 2, nPosY + sz.cy + 2, _RGB16BIT555(29,29,29) );//RGB555mix(17,22,13)

			COLORREF oldcolor = DC.SetTextColor( RGB(0,0,0) );			DC.TextOut( nPosX+1, nPosY+2, m_strTootip.c_str() );
			DC.SetTextColor( RGB(255,255,255) );	DC.TextOut( nPosX, nPosY+1, m_strTootip.c_str() );


			DC.SelectObject(pOldFont);
			DC.SetTextColor( oldcolor);
		}

		BOOL CNMButton::IsDrawAbleToolTip()
		{
			if (m_bShow==FALSE)	return FALSE;

			if( m_bOver )
			{
				if( GetTickCount() > m_dwOverTick + 500 )
				{
					return TRUE;
				}
			}

			return FALSE;
		}

		BOOL CNMButton::OnMouseMoveWidget( UINT nFlags, CPoint point )
		{
			if (m_bShow==FALSE)	
			{
				return FALSE;
			}

			if( m_bEnable == FALSE )
			{
				return FALSE;
			}

			if( IsInRect( point ) )
			{
				switch( m_nSprCount )
				{
				case 1:
					{
						m_nCurSprID = m_nSprID;
					} break;
				case 2:
				case 3:
				case 4:
					{
						m_nCurSprID = m_nSprID+1;
					} break;
				}

				if( m_bOver == FALSE )
				{
					if( m_strOverSnd.length() )
					{
						NMRESAM.PlayWav( m_strOverSnd );
					}
					m_bOver = TRUE;
					m_dwOverTick = GetTickCount();

				}

				return TRUE;
			}
			else
			{
				m_nCurSprID = m_nSprID;
				m_bOver = FALSE;
				m_dwOverTick = 0;
			}

			return FALSE;
		}

		BOOL CNMButton::OnMouseLeaveWidget( UINT nFlags, CPoint point )
		{
			if (m_bShow==FALSE)	
			{
				return FALSE;
			}

			if( m_bEnable == FALSE )
			{
				return FALSE;
			}

			m_nCurSprID = m_nSprID;
			m_bOver = FALSE;
			m_dwOverTick = 0;

			return FALSE;
		}

		BOOL CNMButton::OnLButtonDownWidget( UINT nFlags, CPoint point )
		{
			if (m_bShow==FALSE)	
			{
				return FALSE;
			}
			if( m_bEnable == FALSE )
			{
				return FALSE;
			}

			if( IsInRect( point ) )
			{
				switch( m_nSprCount )
				{
				case 1:
					{
						m_nCurSprID = m_nSprID;
					} break;
				case 2:
					{
						m_nCurSprID = m_nSprID+1;
					} break;
				case 3:
				case 4:
					{
						m_nCurSprID = m_nSprID+2;
					} break;
				}

				if( m_strDownSnd.length() )
				{
					NMRESAM.PlayWav( m_strDownSnd );
				}

				m_bDown = TRUE;
				m_dwDownTick = GetTickCount();

				m_bOver = FALSE;
				m_dwOverTick = 0;

				return TRUE;
			}

			return FALSE;
		}

		BOOL CNMButton::OnLButtonUpWidget( UINT nFlags, CPoint point )
		{
			if (m_bShow==FALSE)	
			{
				return FALSE;
			}

			if( m_bEnable == FALSE )
			{
				return FALSE;
			}

			if( IsInRect( point ) )
			{
				switch( m_nSprCount )
				{
				case 1:
					{
						m_nCurSprID = m_nSprID;
					} break;
				case 2:
				case 3:
				case 4:
					{
						m_nCurSprID = m_nSprID+1;
					} break;
				}

				if( m_bDown )
				{
					NMSendMessage( m_pParent, m_nControlID, m_wParam, m_lParam );

					TRACE( "CNMButton::OnLButtonUpWidget( ID : %d, w:%d, l:%d )\n", m_nControlID, m_wParam, m_lParam );

					m_bDown = FALSE;
					m_dwDownTick = 0;

					return TRUE;
				}


			}

			return FALSE;
		}

		void CNMButton::SetEnable( BOOL bFlag )
		{
			//	if( m_bEnable == bFlag ) return;

			m_bEnable = bFlag;

			if( bFlag )
			{
				m_nCurSprID = m_nSprID;
				m_bOver = FALSE;
			}
			else
			{
				switch( m_nSprCount )
				{
				case 1:
				case 2:
				case 3:
					{
						m_nCurSprID = -1;
					} break;
				case 4:
					{
						m_nCurSprID = m_nSprID+3;
					} break;
				}
			}
		}
		void CNMButton::Show(BOOL bShow)
		{
			m_bShow=bShow;	

		}

		//////////////////////////////////////////////////////////////////////////////
		// CNMSceneButton
		CNMSceneButton::CNMSceneButton()
		{

		}
		CNMSceneButton::~CNMSceneButton()
		{

		}
		BOOL CNMSceneButton::SetSprite( char* szSprKey,	int nSprID,	int nSprCount )
		{
			m_pSprite = NMRESSM.GetSprite( szSprKey );
			if( m_pSprite == NULL )
				return FALSE;

			m_nSprID = nSprID;

			if( nSprCount < 1 || nSprCount > 4 )
				return FALSE;

			m_nSprCount = nSprCount;

			m_nCurSprID = nSprID;

			if( m_pSprite->spr == NULL )
				return FALSE;

			int nPosX = m_rectPosition.left;
			int nPosY = m_rectPosition.top;
			m_nWidth = m_pSprite->spr[0].xl;
			m_nHeight = m_pSprite->spr[0].yl;
			m_rectPosition.SetRect( nPosX, nPosY, nPosX + m_nWidth, nPosY + m_nHeight );

			return TRUE;
		}
		void CNMSceneButton::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
		{
			if( m_pSprite != NULL )
			{
				Page.DrawScene( m_rectPosition.left, m_rectPosition.top,
					m_pSprite,
					m_nCurSprID,
					GetTickCount() - m_dwCurSprTick );
			}

			DrawToolTip( Page, DC );
		}
		BOOL CNMSceneButton::OnMouseMoveWidget( UINT nFlags, CPoint point )
		{
			BOOL bBefore = m_bOver;
			BOOL bRet = CNMButton::OnMouseMoveWidget( nFlags, point );
			if( bBefore != m_bOver )
			{
				m_dwCurSprTick = GetTickCount();
			}
			return bRet;
		}
		BOOL CNMSceneButton::OnLButtonDownWidget( UINT nFlags, CPoint point )
		{
			m_dwCurSprTick = GetTickCount();
			return CNMButton::OnLButtonDownWidget( nFlags, point );
		}
		BOOL CNMSceneButton::OnLButtonUpWidget( UINT nFlags, CPoint point )
		{
			m_dwCurSprTick = GetTickCount();
			return CNMButton::OnLButtonUpWidget( nFlags, point );
		}

		//////////////////////////////////////////////////////////////////////////////
		// CNMMultiSceneButton
		CNMMultiSceneButton::CNMMultiSceneButton()
		{

		}
		CNMMultiSceneButton::~CNMMultiSceneButton()
		{

		}
		void CNMMultiSceneButton::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
		{
			if( m_pSprite != NULL )
			{
				Page.DrawMultiScene( m_rectPosition.left, m_rectPosition.top,
					m_pSprite,
					m_nCurSprID,
					GetTickCount() - m_dwCurSprTick );
			}

			DrawToolTip( Page, DC );
		}

		//////////////////////////////////////////////////////////////////////////////
		// CNMThumButton

		CNMThumButton::CNMThumButton()
		{
			m_rectThum.SetRect(0,0,0,0);
			m_bDrag = FALSE;
			m_ptDragPoint.x = m_ptDragPoint.y = 0;
			m_rectWheel.SetRect(0,0,0,0);
			m_nElementCountPerPage = 1;
			m_nColumnCount = 1;
			m_nMaxPageCount = 1;
			m_nCurrentPage = 0;
			m_fUnitHeight = 1.0f;
			m_bAutoLastPage = FALSE;
		}

		CNMThumButton::~CNMThumButton()
		{

		}

		BOOL CNMThumButton::SetThumAttribute( CNMButton* pUpButton, CNMButton* pDownButton,
			CRect rectWheel,
			int nElementCountPerPage,
			int nColumnCount )

		{
			if( pUpButton == NULL )		return FALSE;
			if( pDownButton == NULL )	return FALSE;

			m_rectThum.SetRect( pUpButton->GetButtonRect().left,
				pUpButton->GetButtonRect().top + (pUpButton->GetButtonRect().Height()),
				pUpButton->GetButtonRect().right,
				pDownButton->GetButtonRect().bottom - (pDownButton->GetButtonRect().Height()) );

			m_rectWheel = rectWheel;
			m_nElementCountPerPage = nElementCountPerPage;
			m_nColumnCount = nColumnCount;

			SetupThumPosition();

			return TRUE;
		}

		void CNMThumButton::SetThumPosition( CNMButton* pUpButton, CNMButton* pDownButton, CRect rectWheel )
		{
			if( pUpButton == NULL )		return;
			if( pDownButton == NULL )	return;

			m_rectThum.SetRect( pUpButton->GetButtonRect().left,
				pUpButton->GetButtonRect().top + (pUpButton->GetButtonRect().Height()),
				pUpButton->GetButtonRect().right,
				pDownButton->GetButtonRect().bottom - (pDownButton->GetButtonRect().Height()) );

			m_rectWheel = rectWheel;

			m_rectPosition.SetRect( m_rectThum.left, m_rectThum.top, m_rectThum.left + m_nWidth, m_rectThum.top + m_nHeight );
			SetCurrentPage( m_nMaxPageCount-1 );
		}

		void CNMThumButton::SetAutoLastPageAttri( BOOL bFlag )
		{
			m_bAutoLastPage = bFlag;
		}

		void CNMThumButton::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
		{
			CNMButton::DrawWidget( Page, DC );

			// #ifdef _DEBUG
			//  	CString strDebug;
			//  	strDebug.Format( "CurPage/MAXPage : %03d/%03d(%03d)",  m_nCurrentPage, m_nMaxPageCount-1, m_nMaxPageCount );
			//  
			//  	DC.SelectObject( &GetMyFont( FONT_11_NORMAL ) );
			//  	DC.SetTextColor( RGB(0,0,0) );
			//  	SIZE sz = DC.GetTextExtent( strDebug );
			//  
			//  	int nPosX = m_rectPosition.left - 190;
			//  	int nPosY = m_rectPosition.top + 30;
			//  
			//  	Page.DrawFillBox( nPosX, nPosY, nPosX + sz.cx, nPosY + sz.cy,	RGB555mix(31,31,29) );
			//  	Page.DrawEmptyBox( nPosX - 1, nPosY - 1, nPosX + sz.cx + 1, nPosY + sz.cy + 1, RGB555mix(0,0,0) );
			//  	DC.TextOut( nPosX + 1, nPosY + 1, strDebug );
			//  
			//  	Page.DrawEmptyBox( m_rectWheel.left, m_rectWheel.top, m_rectWheel.right, m_rectWheel.bottom, RGB555mix(31,0,0) );
			// #endif _DEBUG
		}

		BOOL CNMThumButton::OnMouseMoveWidget( UINT nFlags, CPoint point )
		{
			if( m_bDrag )
			{
				int nGapX = point.x - m_ptDragPoint.x;
				int nGapY = point.y - m_ptDragPoint.y;

				ChangePosition( nGapX, nGapY );

				//return TRUE;	// << return check
			}
			else
			{
				CNMButton::OnMouseMoveWidget( nFlags, point );
			}

			return FALSE;
		}

		BOOL CNMThumButton::OnMouseLeaveWidget( UINT nFlags, CPoint point )
		{
			if( m_bDrag )
			{
				m_bDrag = FALSE;
			}

			CNMButton::OnMouseLeaveWidget( nFlags, point );

			return FALSE;
		}

		BOOL CNMThumButton::OnLButtonDownWidget( UINT nFlags, CPoint point )
		{
			if( CNMButton::OnLButtonDownWidget( nFlags, point ) )
			{
				m_bDrag = TRUE;
				m_ptDragPoint.x = point.x - m_rectPosition.left;
				m_ptDragPoint.y = point.y - m_rectPosition.top;

				if( m_pParent )
					m_pParent->SetCapture();	// << capture check
			}
			else
			{
				m_ptDragPoint = 0;
			}

			return FALSE;
		}
		BOOL CNMThumButton::OnLButtonUpWidget( UINT nFlags, CPoint point )
		{
			ReleaseCapture();	// << capture check

			if( CNMButton::OnLButtonUpWidget( nFlags, point ) )
			{
				SetCurrentPage( m_nCurrentPage );
			}
			else
			{
				if( m_bDrag )
				{
					SetCurrentPage( m_nCurrentPage );
					m_bDrag = FALSE;
					//return TRUE;	// << return check
				}
				else
				{
					if( m_rectThum.PtInRect( point ) )
					{
						if( m_rectPosition.top > point.y )
						{
							BeforePage();
						}
						else
						{
							NextPage();
						}
					}
				}

				m_nCurSprID = m_nSprID;
				m_bOver = FALSE;
				m_dwOverTick = 0;
			}

			m_bDrag = FALSE;

			return FALSE;
		}

		BOOL CNMThumButton::OnMouseWheelWidget( UINT nFlags, short zDelta, CPoint point )
		{
			if( m_rectWheel.PtInRect( point ) )
			{
				int nRowsScrolled = zDelta/120;

				if( nRowsScrolled > 0 )
				{
					SetCurrentPage( m_nCurrentPage - 3 );
				}
				else
				{
					SetCurrentPage( m_nCurrentPage + 3 );
				}
				return TRUE;
			}
			return FALSE;
		}

		void CNMThumButton::SetupThumPosition()
		{
			m_rectPosition.SetRect( m_rectThum.left, m_rectThum.top, m_rectThum.left + m_nWidth, m_rectThum.top + m_nHeight );
			SetCurrentPage( 0 );
		}

		void CNMThumButton::SetElementCount( int nElementCount )
		{
			if( m_nColumnCount <= 0 )
			{
				AfxMessageBox( "CNMThumButton::SetElementCount - [ m_nColumnCount <= 0 ]" );
				return;
			}

			BOOL bLast = FALSE;
			if( m_nCurrentPage == m_nMaxPageCount-1 )
				bLast = TRUE;

			// 	int nTmpCount = nElementCount;
			// 	int nOffset = nTmpCount%m_nColumnCount;
			// 	if( nOffset )	nTmpCount = nTmpCount + m_nColumnCount;
			// 	nTmpCount = (nTmpCount-(m_nElementCountPerPage-1))/m_nColumnCount;	
			// 	if( nTmpCount <= 0 )	nTmpCount = 1;


			int nTotalUnits  = nElementCount;
			int nOffset = nTotalUnits%m_nColumnCount;
			if (nOffset>0) nTotalUnits += (m_nColumnCount-nOffset);
			int nTmpCount = (nTotalUnits / m_nColumnCount) - (m_nElementCountPerPage/m_nColumnCount)+1; 
			if( nTmpCount <= 0 )	nTmpCount = 1;




			if( m_nMaxPageCount != nTmpCount )
			{
				//////////////////////////////////////////////////////////////////////////////////
				// set m_nMaxPageCount
				m_nMaxPageCount = nTmpCount;

				//////////////////////////////////////////////////////////////////////////////////
				// calculate m_fUnitHeight
				int nMaxHeight = m_rectThum.bottom - m_rectThum.top - m_nHeight;
				if( m_nMaxPageCount <= 1 )
				{
					m_fUnitHeight = 1.0f;
				}
				else
				{
					m_fUnitHeight = (float)nMaxHeight/(float)(m_nMaxPageCount-1);
					if( m_fUnitHeight <= 0 )	m_fUnitHeight = 1.0f;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////
			// check safe position
			if( m_nCurrentPage >= m_nMaxPageCount )
			{
				if( m_bAutoLastPage && bLast )
					SetCurrentPage( m_nMaxPageCount-1 );
				else
					SetCurrentPage( 0 );
				m_rectPosition.SetRect( m_rectThum.left, m_rectThum.top, m_rectThum.left + m_nWidth, m_rectThum.top + m_nHeight );
			}
			else if( m_bAutoLastPage && bLast )	// auto last page setting
			{
				SetCurrentPage( m_nMaxPageCount-1 );
			}
			else
				NMSendMessage( m_pParent, m_nControlID );
		}

		void CNMThumButton::ChangePosition( int nPosX, int nPosY )
		{
			//////////////////////////////////////////////////////////////////////////////////
			// Set Safe Position
			int nSafePosX = nPosX;
			int nSafePosY = nPosY;

			nSafePosX = m_rectThum.left;
			if( nSafePosY < m_rectThum.top )						nSafePosY = m_rectThum.top;
			else if( m_rectThum.bottom - m_nHeight < nSafePosY )	nSafePosY = m_rectThum.bottom - m_nHeight;

			m_rectPosition.SetRect( nSafePosX, nSafePosY,
				nSafePosX + m_nWidth, nSafePosY + m_nHeight );

			float fRelativePosY = (float) (m_rectPosition.top - m_rectThum.top);
			int nCurrentPage = 0;
			if( fRelativePosY != 0.0f )	nCurrentPage = (int) (fRelativePosY/m_fUnitHeight) + 1;

			SetCurrentPage( nCurrentPage );
		}

		void CNMThumButton::SetCurrentPage( int nPage )
		{
			if( m_nCurrentPage == nPage )		return;

			//////////////////////////////////////////////////////////////////////////////////
			// Set Safe nPage
			if( nPage < 0 )						m_nCurrentPage = 0;
			else if( nPage >= m_nMaxPageCount )	m_nCurrentPage = m_nMaxPageCount-1;
			else								m_nCurrentPage = nPage;

			//////////////////////////////////////////////////////////////////////////////////
			// calculate Position;
			if( m_nCurrentPage == 0 )
			{
				m_rectPosition.SetRect( m_rectThum.left, m_rectThum.top, m_rectThum.left + m_nWidth, m_rectThum.top + m_nHeight );
			}
			else if( m_nCurrentPage == m_nMaxPageCount-1 )
			{
				m_rectPosition.SetRect( m_rectThum.left, m_rectThum.bottom - m_nHeight,
					m_rectThum.left + m_nWidth, m_rectThum.bottom );
			}
			else
			{
				int nRelativePosY = (int) (m_fUnitHeight*m_nCurrentPage);
				m_rectPosition.SetRect( m_rectThum.left, m_rectThum.top + nRelativePosY,
					m_rectThum.left + m_nWidth, m_rectThum.top + nRelativePosY + m_nHeight );
			}

			NMSendMessage( m_pParent, m_nControlID );
		}

		void CNMThumButton::BeforePage()
		{
			SetCurrentPage( m_nCurrentPage - m_nElementCountPerPage );
		}

		void CNMThumButton::NextPage()
		{	
			SetCurrentPage( m_nCurrentPage + m_nElementCountPerPage );
		}

		int	CNMThumButton::GetStartElement()
		{
			return m_nCurrentPage * m_nColumnCount;
		}


		//////////////////////////////////////////////////////////////////////////////
		// CNMCheckButton


		CNMCheckButton::CNMCheckButton()
		{
			m_bClick = FALSE;
		}

		CNMCheckButton::~CNMCheckButton()
		{

		}

		void CNMCheckButton::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
		{
			if( m_bShow == FALSE ) return;


			if( m_bClick == FALSE ) Page.PutSprAuto( m_rectPosition.left, m_rectPosition.top, m_pSprite, m_nCurSprID );
			else if( m_bClick == TRUE ) Page.PutSprAuto( m_rectPosition.left, m_rectPosition.top, m_pSprite, m_nCurSprID + 1 );

		}


		BOOL CNMCheckButton::OnMouseMoveWidget( UINT nFlags, CPoint point )
		{
			if(m_bShow==FALSE) return FALSE;

			return FALSE;
		}
		BOOL CNMCheckButton::OnLButtonDownWidget( UINT nFlags, CPoint point )
		{
			if(m_bShow==FALSE) return FALSE;

			return FALSE;
		}
		BOOL CNMCheckButton::OnLButtonUpWidget( UINT nFlags, CPoint point )
		{
			if( m_bShow == FALSE ) return FALSE;
			if( m_bEnable == FALSE ) return FALSE;

			if( IsInRect( point ) )
			{
				// 버튼 눌림 메세지를 보냄
				if( m_bClick == TRUE) m_bClick = FALSE;
				else  m_bClick = TRUE;

				NMSendMessage( m_pParent, m_nControlID, m_bClick, m_lParam );

				return TRUE;
			}

			return FALSE;
		}


		bool CNMCheckButton::GetButtonState()
		{
			//	if( !m_bClick )	return 0;
			return m_bClick;
		}

		void CNMCheckButton::SetButtonState( bool state )
		{
			m_bClick = state;
		}

	} //namespace SKINGDI

}//namespace NMBASE