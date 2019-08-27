#include "stdafx.h"
#include "./NMWidget/NMTimeGauge.h"
#include "./NMWidget/NMResourceManager.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		//////////////////////////////////////////////////////////////////////////////
		// CNMTimeGauge
		CNMTimeGauge::CNMTimeGauge()
		{
			m_bVisible = TRUE;
			m_nControlID = WIDGET_ID_NULL;
			m_pParent = NULL;
			m_pSprite = NULL;
			m_rectPosition.SetRect(0,0,0,0);
			m_rectBasePosition.SetRect(0,0,0,0);
			m_nWidth = m_nHeight = 0;

			m_bSendTimeEnd = TRUE;
			m_dwStartTick = 0;
			m_dwDelayTick = 0;
		}

		CNMTimeGauge::~CNMTimeGauge()
		{
		}

		BOOL CNMTimeGauge::Initialize( int nControlID,
			CWnd* pParent,
			int nPosX, int nPosY,
			char* szSprKey )
		{
			m_nControlID = nControlID;

			if( pParent == NULL )
				return FALSE;

			m_pParent = pParent;

			m_pSprite = NMRESSM.GetSprite( szSprKey );
			if( m_pSprite == NULL )
				return FALSE;

			if( m_pSprite->spr == NULL )
				return FALSE;

			m_nWidth = m_pSprite->spr[0].xl;
			m_nHeight = m_pSprite->spr[0].yl;
			m_rectPosition.SetRect( nPosX, nPosY, nPosX + m_nWidth, nPosY + m_nHeight );
			m_rectBasePosition = m_rectPosition;

			return TRUE;
		}
		void CNMTimeGauge::EnterWidget()
		{
		}

		void CNMTimeGauge::UpdateWidget( DWORD dwCurTick )
		{
			if( m_bSendTimeEnd )			return;
			if( m_dwStartTick > dwCurTick)	return;

			DWORD dwNowTick = dwCurTick - m_dwStartTick;
			if( dwNowTick > m_dwDelayTick )
			{
				NMSendMessage( m_pParent, m_nControlID );
				ClearTimer();
			}
		}
		void CNMTimeGauge::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
		{
			if( m_bVisible == FALSE )	return;
			if( m_pSprite == NULL )		return;

			if( m_bSendTimeEnd )
			{
				Page.PutSprAuto( m_rectPosition.left, m_rectPosition.top, m_pSprite, 0 );
				DC.SetTextColor( RGB(0,0,0) );
				DC.TextOut( m_rectPosition.left, m_rectPosition.top, "Gauge FULL!!!!" );
			}
			else
			{
				DWORD dwNowTick = GetTickCount() - m_dwStartTick;

				Page.EnableTempClip( TRUE );

				Page.SetTempClip( m_rectPosition.left, m_rectPosition.top,
					m_rectPosition.left + (m_nWidth*dwNowTick/m_dwDelayTick),
					m_rectPosition.bottom );

				Page.PutSprAuto( m_rectPosition.left, m_rectPosition.top, m_pSprite, 0 );

				Page.EnableTempClip( FALSE );
			}
		}

		void CNMTimeGauge::ExitWidget()
		{
			ClearTimer();
		}

		void CNMTimeGauge::SetRelationPosition( int nOffsetX, int nOffsetY )
		{
			m_rectPosition.SetRect( m_rectBasePosition.left,
				m_rectBasePosition.top,
				m_rectBasePosition.left + m_nWidth,
				m_rectBasePosition.top + m_nHeight );
			m_rectPosition.OffsetRect( nOffsetX, nOffsetY );
		}

		void CNMTimeGauge::SetTimer( DWORD dwDelayTick )
		{
			m_dwStartTick = GetTickCount();
			m_dwDelayTick = dwDelayTick;
			m_bSendTimeEnd = FALSE;
		}

		void CNMTimeGauge::ClearTimer()
		{
			m_dwStartTick = 0;
			m_dwDelayTick = 0;
			m_bSendTimeEnd = TRUE;
		}

	} //namespace SKINGDI

}//namespace NMBASE