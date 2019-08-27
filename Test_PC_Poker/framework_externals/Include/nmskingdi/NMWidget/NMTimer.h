#pragma once

#include "NMWidgetManager.h"
#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		class NMSKINGDI_CLASS CNMTimer : public CNMWidgetCompact
		{
		public:
			CNMTimer();
			virtual ~CNMTimer();

			virtual void Initialize( int nControlID, CWnd* pParent, DWORD dwCycle );
			virtual void EnterWidget();
			virtual void UpdateWidget( DWORD dwCurTick );
			virtual void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );
			virtual void ExitWidget();

		protected:
			int		m_nControlID;
			CWnd*	m_pParent;
			BOOL	m_bActive;
			DWORD	m_dwStartTick;
			DWORD	m_dwBeforeCycle;
			DWORD	m_dwAccTime;
			DWORD	m_dwCycle;
		};

	} //namespace SKINGDI

}//namespace NMBASE
