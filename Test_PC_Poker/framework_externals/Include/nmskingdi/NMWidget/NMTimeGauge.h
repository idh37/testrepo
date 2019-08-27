#pragma once

#include "NMWidgetManager.h"
#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{
		//////////////////////////////////////////////////////////////////////////////
		// CNMTimeGauge
		class NMSKINGDI_CLASS CNMTimeGauge : public CNMWidgetCompact
		{
		public:
			CNMTimeGauge();
			virtual ~CNMTimeGauge();

			virtual BOOL Initialize( int nControlID, CWnd* pParent, int nPosX, int nPosY, char* szSprKey );
			virtual void EnterWidget();
			virtual void UpdateWidget( DWORD dwCurTick );
			virtual void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );
			virtual void ExitWidget();

			virtual void SetRelationPosition( int nOffsetX, int nOffsetY );
			virtual void SetTimer( DWORD dwDelayTick );
			virtual void ClearTimer();

			virtual void SetVisible( BOOL bVisible )	{ m_bVisible = bVisible; }

		protected:
			BOOL		m_bVisible;
			int			m_nControlID;
			CWnd*		m_pParent;
			NMBASE::GRAPHICGDI::xSprite*	m_pSprite;
			CRect		m_rectPosition;
			CRect		m_rectBasePosition;
			int			m_nWidth, m_nHeight;

			BOOL		m_bSendTimeEnd;
			DWORD		m_dwStartTick;
			DWORD		m_dwDelayTick;
		};


	} //namespace SKINGDI

}//namespace NMBASE