#include "stdafx.h"
#include "./NMWidget/NMDialog.h"
#include "./NMWidget/NMButton.h"
#include "./NMWidget/NMTab.h"
#include "./NMWidget/NMTimeGauge.h"
#include "./NMWidget/NMResourceManager.h"


namespace NMBASE
{
	namespace SKINGDI
	{
		//////////////////////////////////////////////////////////////////////////////
		// CNMDialog
		CNMDialog::CNMDialog()
		{
			m_bDisplay = FALSE;
			m_rectPosition.SetRect(0,0,0,0);
			m_nWidth = 0;
			m_nHeight = 0;
			m_pSprite = NULL;
			m_nBGIndex = -1;

			m_bDragEnable = TRUE;
			m_bDrag = FALSE;
			m_nDragGepX = 0;
			m_nDragGepY = 0;
			m_pParent=NULL;
		}

		CNMDialog::~CNMDialog()
		{
		}

		void CNMDialog::InitializeDialog( CWnd* pParent,  int nWidthGameView, int nHeightGameView )
		{
			m_pParent = pParent;

			m_sizeGameView.SetSize(nWidthGameView, nHeightGameView);

		}

		void CNMDialog::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
		{
			if( m_bDisplay == FALSE )	return;
			if( m_pSprite == NULL )		return;

			//Page.DrawMultiScene( m_rectPosition.left, m_rectPosition.top, m_pSprite, m_nBGIndex, GetTickCount() );
			Page.PutSprAuto( m_rectPosition.left, m_rectPosition.top, m_pSprite, m_nBGIndex );

			DrawDialog( Page, DC );

			CNMWidgetComposite::DrawWidget( Page, DC );
		}

		BOOL CNMDialog::OnMouseMoveWidget( UINT nFlags, CPoint point )
		{
			if( m_bDisplay == FALSE )	return FALSE;

			if( m_bDrag )
			{
				CPoint ptPoint = GetWindowRectInScreen( point.x - m_nDragGepX, point.y - m_nDragGepY );
				m_rectPosition.OffsetRect( ptPoint.x, ptPoint.y );

				RelationChildPosition();
			}

			if( IsInWindowRect( point ) )
			{
				OnMouseMoveDialog( nFlags, point );

				CNMWidgetComposite::OnMouseMoveWidget( nFlags, point );

				return TRUE;
			}

			return CNMWidgetComposite::OnMouseMoveWidget( nFlags, point );
		}

		BOOL CNMDialog::OnMouseLeaveWidget( UINT nFlags, CPoint point )
		{
			m_bDrag = FALSE;

			return CNMWidgetComposite::OnMouseLeaveWidget( nFlags, point );
		}




		BOOL CNMDialog::OnLButtonDbClkWidget( UINT nFlags, CPoint point )
		{
			if( m_bDisplay == FALSE )	return FALSE;

			if( IsInWindowRect( point ) )
			{
				CNMWidgetComposite::OnLButtonDbClkWidget( nFlags, point );
				return TRUE;
			}

			return FALSE;
		}

		BOOL CNMDialog::OnRButtonDownWidget( UINT nFlags, CPoint point )
		{
			if( m_bDisplay == FALSE )	return FALSE;

			if( IsInWindowRect( point ) )
			{
				CNMWidgetComposite::OnLButtonDownWidget( nFlags, point );
				return TRUE;
			}

			return FALSE;
		}
		BOOL CNMDialog::OnLButtonDownWidget( UINT nFlags, CPoint point )
		{
			if( m_bDisplay == FALSE )	return FALSE;

			if( IsInWindowRect( point ) )
			{
				if( OnLButtonDownDialog( nFlags, point ) == FALSE )
					if( CNMWidgetComposite::OnLButtonDownWidget( nFlags, point ) == FALSE )
					{
						if( m_bDragEnable )
						{
							if( !m_bDrag )
							{
								m_nDragGepX = point.x - m_rectPosition.left;
								m_nDragGepY = point.y - m_rectPosition.top;
								m_bDrag = TRUE;
							}
						}
					}

					return TRUE;
			}

			return FALSE;
		}
		BOOL CNMDialog::OnLButtonUpWidget( UINT nFlags, CPoint point )
		{
			if( m_bDisplay == FALSE )	return FALSE;

			if( IsInWindowRect( point ) )
			{
				if( m_bDrag )
				{
					m_bDrag = FALSE;

					CPoint ptPoint = GetWindowRectInScreen( point.x - m_nDragGepX, point.y - m_nDragGepY );
					m_rectPosition.OffsetRect( ptPoint.x, ptPoint.y );

					RelationChildPosition();
				}

				if( CNMWidgetComposite::OnLButtonUpWidget( nFlags, point ) == FALSE )
					OnLButtonUpDialog( nFlags, point );

				return TRUE;
			}

			return FALSE;
		}
		BOOL CNMDialog::OnKeyDownWidget( int nKeyCode, int nExtra )
		{
			if( m_bDisplay == FALSE )	return FALSE;

			if( OnKeyDownDialog( nKeyCode, nExtra ) ) return TRUE;
			//	{
			//		if( nKeyCode == VK_ESCAPE )
			//		{
			//			PopDownDialog();
			//		}
			//	}

			return CNMWidgetComposite::OnKeyDownWidget( nKeyCode, nExtra );
		}
		BOOL CNMDialog::OnCommandWidget( int nCommand, WPARAM wParam, LPARAM lParam )
		{
			if( m_bDisplay == FALSE )	return FALSE;

			CNMWidgetComposite::OnCommandWidget( nCommand, wParam, lParam );

			return OnCommandDialog( nCommand, wParam, lParam );
		}

		void CNMDialog::SetSprite( char* szSprKey, int nBGIndex, int nPosX, int nPosY )
		{
			m_pSprite = NMRESSM.GetSprite(szSprKey);
			if (m_pSprite==NULL)
				return;

			m_nBGIndex = nBGIndex;

			m_nWidth = m_pSprite->spr[m_nBGIndex].xl;
			m_nHeight = m_pSprite->spr[m_nBGIndex].yl;
			int nPosx = 0;	int nPosy = 0;
			if( nPosX == 0 )
				nPosx = ( m_sizeGameView.cx - m_nWidth ) / 2;
			else
				nPosx = nPosX;

			if( nPosY == 0 )
				nPosy = ( m_sizeGameView.cy - m_nHeight ) / 2;
			else
				nPosy = nPosY;

			m_rectPosition.SetRect( nPosx, nPosy, nPosx+m_nWidth, nPosy+m_nHeight );

			RelationChildPosition();
		}

		BOOL CNMDialog::PopUpDialog()
		{
			m_bDisplay = TRUE;
			m_bDrag = FALSE;
			m_nDragGepX = m_nDragGepY = 0;

			CNMWidgetComposite::EnterWidget();
			return TRUE;
		}

		void CNMDialog::PopDownDialog()
		{
			m_bDisplay = FALSE;

			CNMWidgetComposite::ExitWidget();
		}

		BOOL CNMDialog::IsInWindowRect( CPoint ptPoint )
		{
			if( m_bDisplay == FALSE )	return FALSE;

			if( m_rectPosition.PtInRect( ptPoint ) )
			{
				return TRUE;
			}

			return FALSE;
		}

		CPoint CNMDialog::GetWindowRectInScreen( int nleft, int ntop )
		{
			CPoint ptRetValue( nleft, ntop );

			if( ptRetValue.x < 0 )							ptRetValue.x = 0;
			if( ptRetValue.y < 0 )							ptRetValue.y = 0;
			if( ptRetValue.x + m_nWidth > m_sizeGameView.cx )	ptRetValue.x = m_sizeGameView.cx - m_nWidth;
			if( ptRetValue.y + m_nHeight > m_sizeGameView.cy )	ptRetValue.y = m_sizeGameView.cy - m_nHeight;

			ptRetValue.x = ptRetValue.x - m_rectPosition.left;
			ptRetValue.y = ptRetValue.y - m_rectPosition.top;

			return ptRetValue;
		}

		void CNMDialog::RelationChildPosition()
		{
			CNMButton* pButton = NULL;
			CNMTimeGauge* pTimeGauge = NULL;
			CNMTab		*pTab = NULL;
			for( MAPWIDGET::reverse_iterator curit = m_mapWidget.rbegin(); curit != m_mapWidget.rend(); curit++ )
			{
				if( (curit->second) == NULL )	continue;

				pTab = dynamic_cast<CNMTab*>( (curit->second) );
				if( pTab )
					pTab->SetRelationPosition( m_rectPosition.left, m_rectPosition.top );

				pButton = dynamic_cast<CNMButton*>( (curit->second) );
				if( pButton )
					pButton->SetRelationPosition( m_rectPosition.left, m_rectPosition.top );

				pTimeGauge = dynamic_cast<CNMTimeGauge*>( (curit->second) );
				if( pTimeGauge )
					pTimeGauge->SetRelationPosition( m_rectPosition.left, m_rectPosition.top );
			}
		}

		void CNMDialog::SetPosition(int nPosX, int nPosY)
		{

			CPoint ptPoint( nPosX - m_rectPosition.left, nPosY - m_rectPosition.top );
			m_rectPosition.OffsetRect( ptPoint.x, ptPoint.y );

			RelationChildPosition();

		}

	} //namespace SKINGDI

}//namespace NMBASE