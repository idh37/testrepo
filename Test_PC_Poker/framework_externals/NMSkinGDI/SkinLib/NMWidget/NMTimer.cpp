#include "stdafx.h"
#include "./NMWidget/NMTimer.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		CNMTimer::CNMTimer()
		{
			m_nControlID = WIDGET_ID_NULL;
			m_pParent = NULL;
			m_bActive = FALSE;
			m_dwStartTick = 0;
			m_dwBeforeCycle = 0;
			m_dwAccTime = 0;
			m_dwCycle = 1000;
		}

		CNMTimer::~CNMTimer()
		{
		}

		void CNMTimer::Initialize( int nControlID, CWnd* pParent, DWORD dwCycle )
		{
			m_nControlID = nControlID;
			m_pParent = pParent;
			m_dwCycle = dwCycle;

			m_bActive = TRUE;
		}
		void CNMTimer::EnterWidget()
		{
			m_dwStartTick = GetTickCount();
			m_dwBeforeCycle = 0;
			m_dwAccTime = 0;

			m_bActive = TRUE;

			if( m_pParent == NULL )					return;
			if( m_nControlID == WIDGET_ID_NULL )	return;

			NMSendMessage( m_pParent, m_nControlID, 0, 0 );
		}
		void CNMTimer::UpdateWidget( DWORD dwCurTick )
		{
			if( m_bActive == FALSE )	return;
			if( m_pParent == NULL )		return;
			if( m_nControlID == WIDGET_ID_NULL )	return;

			DWORD dwNowTick = dwCurTick - m_dwStartTick;
			DWORD dwNowCycle = dwNowTick / m_dwCycle;

			if( dwNowCycle != m_dwBeforeCycle )
			{
				NMSendMessage( m_pParent, m_nControlID, m_dwBeforeCycle, dwNowCycle );
				m_dwBeforeCycle = dwNowCycle;
			}
		}
		void CNMTimer::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
		{

		}
		void CNMTimer::ExitWidget()
		{
			m_dwStartTick = 0;
			m_dwBeforeCycle = 0;
			m_dwAccTime = 0;

			m_bActive = FALSE;
		}


	} //namespace SKINGDI

}//namespace NMBASE