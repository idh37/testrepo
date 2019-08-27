#include "stdafx.h"
#include "./NMWidget/NMWidgetManager.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		///////////////////////////////////////////////////////////////////////////////////
		// INMWidget
		// nothing..


		///////////////////////////////////////////////////////////////////////////////////
		// CNMWidgetComposite
		//CNMWidgetComposite::MAPWIDGET CNMWidgetComposite::m_mapGlobalWidget;

		CNMWidgetComposite::CNMWidgetComposite()
		{
			m_mapWidget.clear();
		}
		CNMWidgetComposite::~CNMWidgetComposite()
		{
			DestroyWidget();
		}

		BOOL CNMWidgetComposite::CreateWidget( CWnd* pParent )
		{
			for( MAPWIDGET::iterator curit = m_mapWidget.begin(); curit != m_mapWidget.end(); curit++ )
			{
				if( (curit->second) == NULL )	continue;
				if( (curit->second)->CreateWidget( pParent ) == FALSE )
					return FALSE;
			}

			return TRUE;
		}
		void CNMWidgetComposite::EnterWidget()
		{
			for( MAPWIDGET::iterator curit = m_mapWidget.begin(); curit != m_mapWidget.end(); curit++ )
			{
				if( (curit->second) == NULL )	continue;
				(curit->second)->EnterWidget();
			}
		}
		void CNMWidgetComposite::UpdateWidget( DWORD dwCurTick )
		{
			for( MAPWIDGET::iterator curit = m_mapWidget.begin(); curit != m_mapWidget.end(); curit++ )
			{
				if( (curit->second) == NULL )				continue;
				(curit->second)->UpdateWidget( dwCurTick );
			}
		}
		void CNMWidgetComposite::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
		{
			for( MAPWIDGET::iterator curit = m_mapWidget.begin();
				curit != m_mapWidget.end(); curit++ )
			{
				if( (curit->second) == NULL )				continue;
				(curit->second)->DrawWidget( Page, DC );
			}
		}
		void CNMWidgetComposite::ExitWidget()
		{
			for( MAPWIDGET::iterator curit = m_mapWidget.begin();
				curit != m_mapWidget.end(); curit++ )
			{
				if( (curit->second) == NULL )	continue;
				(curit->second)->ExitWidget();
			}
		}
		void CNMWidgetComposite::DestroyWidget()	// 체크해봐야 한다.
		{
			for( MAPWIDGET::iterator curit = m_mapWidget.begin(); curit != m_mapWidget.end(); curit++ )
			{
				if( (curit->second) == NULL )				continue;

				(curit->second)->DestroyWidget();

				delete (curit->second);
				(curit->second) = NULL;
			}
			m_mapWidget.clear();
		}
		BOOL CNMWidgetComposite::OnMouseMoveWidget( UINT nFlags, CPoint point )
		{
			BOOL bTakeMessage = FALSE;
			for( MAPWIDGET::reverse_iterator curit = m_mapWidget.rbegin(); curit != m_mapWidget.rend(); curit++ )
			{
				if( (curit->second) == NULL )				continue;
				if( (curit->second)->OnMouseMoveWidget( nFlags, point ) == TRUE )
					bTakeMessage = TRUE;
			}

			return bTakeMessage;
		}
		BOOL CNMWidgetComposite::OnMouseLeaveWidget( UINT nFlags, CPoint point )
		{
			BOOL bTakeMessage = FALSE;
			for( MAPWIDGET::reverse_iterator curit = m_mapWidget.rbegin(); curit != m_mapWidget.rend(); curit++ )
			{
				if( (curit->second) == NULL )				continue;
				if( (curit->second)->OnMouseLeaveWidget( nFlags, point ) == TRUE )
					bTakeMessage = TRUE;
			}

			return bTakeMessage;
		}
		BOOL CNMWidgetComposite::OnLButtonDownWidget( UINT nFlags, CPoint point )
		{
			BOOL bTakeMessage = FALSE;
			for( MAPWIDGET::reverse_iterator curit = m_mapWidget.rbegin(); curit != m_mapWidget.rend(); curit++ )
			{
				if( (curit->second) == NULL )				continue;
				if( (curit->second)->OnLButtonDownWidget( nFlags, point ) == TRUE )
					return TRUE;//bTakeMessage = TRUE;
			}

			return bTakeMessage;
		}
		BOOL CNMWidgetComposite::OnLButtonUpWidget( UINT nFlags, CPoint point )
		{
			BOOL bTakeMessage = FALSE;
			for( MAPWIDGET::reverse_iterator curit = m_mapWidget.rbegin(); curit != m_mapWidget.rend(); curit++ )
			{
				if( (curit->second) == NULL )				continue;
				if( (curit->second)->OnLButtonUpWidget( nFlags, point ) == TRUE )
					return TRUE;//bTakeMessage = TRUE;
			}

			return bTakeMessage;
		}
		BOOL CNMWidgetComposite::OnLButtonDbClkWidget( UINT nFlags, CPoint point )
		{
			BOOL bTakeMessage = FALSE;
			for( MAPWIDGET::reverse_iterator curit = m_mapWidget.rbegin(); curit != m_mapWidget.rend(); curit++ )
			{
				if( (curit->second) == NULL )				continue;
				if( (curit->second)->OnLButtonDbClkWidget( nFlags, point ) == TRUE )
					return TRUE;//bTakeMessage = TRUE;
			}

			return bTakeMessage;
		}
		BOOL CNMWidgetComposite::OnRButtonDownWidget( UINT nFlags, CPoint point )
		{
			BOOL bTakeMessage = FALSE;
			for( MAPWIDGET::reverse_iterator curit = m_mapWidget.rbegin(); curit != m_mapWidget.rend(); curit++ )
			{
				if( (curit->second) == NULL )				continue;
				if( (curit->second)->OnRButtonDownWidget( nFlags, point ) == TRUE )
					return TRUE;//bTakeMessage = TRUE;
			}

			return bTakeMessage;
		}
		BOOL CNMWidgetComposite::OnRButtonUpWidget( UINT nFlags, CPoint point )
		{
			BOOL bTakeMessage = FALSE;
			for( MAPWIDGET::reverse_iterator curit = m_mapWidget.rbegin(); curit != m_mapWidget.rend(); curit++ )
			{
				if( (curit->second) == NULL )				continue;
				if( (curit->second)->OnRButtonUpWidget( nFlags, point ) == TRUE )
					return TRUE;//bTakeMessage = TRUE;
			}

			return bTakeMessage;
		}
		BOOL CNMWidgetComposite::OnRButtonDbClkWidget( UINT nFlags, CPoint point )
		{
			BOOL bTakeMessage = FALSE;
			for( MAPWIDGET::reverse_iterator curit = m_mapWidget.rbegin(); curit != m_mapWidget.rend(); curit++ )
			{
				if( (curit->second) == NULL )				continue;
				if( (curit->second)->OnRButtonDbClkWidget( nFlags, point ) == TRUE )
					return TRUE;//bTakeMessage = TRUE;
			}

			return bTakeMessage;
		}
		BOOL CNMWidgetComposite::OnMouseWheelWidget( UINT nFlags, short zDelta, CPoint point )
		{
			BOOL bTakeMessage = FALSE;
			for( MAPWIDGET::reverse_iterator curit = m_mapWidget.rbegin(); curit != m_mapWidget.rend(); curit++ )
			{
				if( (curit->second) == NULL )				continue;
				if( (curit->second)->OnMouseWheelWidget( nFlags, zDelta, point ) == TRUE )
					return TRUE;//bTakeMessage = TRUE;
			}

			return bTakeMessage;
		}
		BOOL CNMWidgetComposite::OnKeyDownWidget( int nKeyCode, int nExtra )
		{
			BOOL bTakeMessage = FALSE;
			for( MAPWIDGET::reverse_iterator curit = m_mapWidget.rbegin(); curit != m_mapWidget.rend(); curit++ )
			{
				if( (curit->second) == NULL )				continue;
				if( (curit->second)->OnKeyDownWidget( nKeyCode, nExtra ) == TRUE )
					return TRUE;//bTakeMessage = TRUE;
			}

			return bTakeMessage;
		}

		BOOL CNMWidgetComposite::OnCommandWidget( int nCommand, WPARAM wParam, LPARAM lParam )
		{
			BOOL bTakeMessage = FALSE;
			for( MAPWIDGET::iterator curit = m_mapWidget.begin(); curit != m_mapWidget.end(); curit++ )
			{
				if( (curit->second) == NULL )				continue;
				if( (curit->second)->OnCommandWidget( nCommand, wParam, lParam ) == TRUE )
					return TRUE;//bTakeMessage = TRUE;
			}

			return bTakeMessage;
		}

		BOOL CNMWidgetComposite::AttachWidget( int nWidgetKey, INMWidget* pIWidget )
		{
			if( pIWidget == NULL )
			{
				AfxMessageBox( "pIWidget == NULL" );
				return FALSE;
			}

			if( m_mapWidget.find( nWidgetKey ) != m_mapWidget.end() )
			{
				AfxMessageBox( "nWidgetKey already exist" );
				return FALSE;
			}

			m_mapWidget.insert( MAPWIDGET::value_type( nWidgetKey, pIWidget ) );

			return TRUE;
		}

		BOOL CNMWidgetComposite::DetachWidget( int nWidgetKey )
		{
			MAPWIDGET::iterator findit = m_mapWidget.find( nWidgetKey );
			if( findit == m_mapWidget.end() )
				return FALSE;

			if( (findit->second) == NULL )
				return FALSE;

			(findit->second)->DestroyWidget();

			delete (findit->second)	;
			(findit->second) = NULL;

			m_mapWidget.erase( findit );

			return TRUE;
		}

		INMWidget* CNMWidgetComposite::GetWidget( int nWidgetKey )
		{
			MAPWIDGET::iterator findit = m_mapWidget.find( nWidgetKey );
			if( findit == m_mapWidget.end() )
				return NULL;

			if( (findit->second) == NULL )
				return NULL;

			return (findit->second);
		}

		INMWidget* CNMWidgetComposite::FindWidget( int nWidgetKey )
		{	
			INMWidget *pWidget = GetWidget(nWidgetKey);
			if (pWidget)
				return pWidget;

			MAPWIDGET::iterator findit =m_mapWidget.begin();
			for(; findit!=m_mapWidget.end(); findit++)
			{
				CNMWidgetComposite *pWidgetComposite = dynamic_cast<CNMWidgetComposite*> (findit->second);
				if (pWidgetComposite)
				{
					pWidget = pWidgetComposite->FindWidget(nWidgetKey);
					if (pWidget)
						return pWidget;
				}
			}

			return NULL;
		}




		// BOOL CNMWidgetComposite::AttachGlobalWidget( int nWidgetKey, INMWidget* pIWidget )
		// {
		// 	if( pIWidget == NULL )
		// 		return FALSE;
		// 
		// 	if( m_mapGlobalWidget.find( nWidgetKey ) != m_mapGlobalWidget.end() )
		// 		return FALSE;
		// 
		// 	m_mapGlobalWidget.insert( MAPWIDGET::value_type( nWidgetKey, pIWidget ) );
		// 
		// 	return TRUE;
		// }
		// 
		// BOOL CNMWidgetComposite::DetachGlobalWidget( int nWidgetKey )
		// {
		// 	MAPWIDGET::iterator findit = m_mapGlobalWidget.find( nWidgetKey );
		// 	if( findit == m_mapGlobalWidget.end() )
		// 		return FALSE;
		// 
		// 	if( (findit->second) == NULL )
		// 		return FALSE;
		// 
		// 	m_mapGlobalWidget.erase( findit );
		// 
		// 	return TRUE;
		// }
		// 
		// INMWidget* CNMWidgetComposite::GetGlobalWidget( int nWidgetKey )
		// {
		// 	MAPWIDGET::iterator findit = m_mapGlobalWidget.find( nWidgetKey );
		// 	if( findit == m_mapGlobalWidget.end() )
		// 		return NULL;
		// 
		// 	if( (findit->second) == NULL )
		// 		return NULL;
		// 
		// 	return (findit->second);
		// }

		///////////////////////////////////////////////////////////////////////////////////
		// CNMWidgetState
		CNMWidgetState::CNMWidgetState()
		{
			m_mapWidget.clear();
			m_nBeforeWidgtKey = 0;
			m_nCurrentWidgetKey = 0;
		}
		CNMWidgetState::~CNMWidgetState()
		{
			DestroyWidget();
		}

		void CNMWidgetState::EnterWidget()
		{
			MAPWIDGET::iterator itCurrent = m_mapWidget.find( m_nCurrentWidgetKey );
			if( itCurrent == m_mapWidget.end() )
				return;

			if( (itCurrent->second) == NULL )
				return;

			(itCurrent->second)->EnterWidget();
		}
		void CNMWidgetState::UpdateWidget( DWORD dwCurTick )
		{
			MAPWIDGET::iterator itCurrent = m_mapWidget.find( m_nCurrentWidgetKey );
			if( itCurrent == m_mapWidget.end() )
				return;

			if( (itCurrent->second) == NULL )
				return;

			(itCurrent->second)->UpdateWidget( dwCurTick );
		}
		void CNMWidgetState::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
		{
			MAPWIDGET::iterator itCurrent = m_mapWidget.find( m_nCurrentWidgetKey );
			if( itCurrent == m_mapWidget.end() )
				return;

			if( (itCurrent->second) == NULL )
				return;

			(itCurrent->second)->DrawWidget( Page, DC );
		}
		void CNMWidgetState::ExitWidget()
		{
			MAPWIDGET::iterator itCurrent = m_mapWidget.find( m_nCurrentWidgetKey );
			if( itCurrent == m_mapWidget.end() )
				return;

			if( (itCurrent->second) == NULL )
				return;

			(itCurrent->second)->ExitWidget();
		}
		BOOL CNMWidgetState::OnMouseMoveWidget( UINT nFlags, CPoint point )
		{
			MAPWIDGET::iterator itCurrent = m_mapWidget.find( m_nCurrentWidgetKey );
			if( itCurrent == m_mapWidget.end() )
				return FALSE;

			if( (itCurrent->second) == NULL )
				return FALSE;

			return (itCurrent->second)->OnMouseMoveWidget( nFlags, point );
		}
		BOOL CNMWidgetState::OnMouseLeaveWidget( UINT nFlags, CPoint point )
		{
			MAPWIDGET::iterator itCurrent = m_mapWidget.find( m_nCurrentWidgetKey );
			if( itCurrent == m_mapWidget.end() )
				return FALSE;

			if( (itCurrent->second) == NULL )
				return FALSE;

			return (itCurrent->second)->OnMouseLeaveWidget( nFlags, point );
		}
		BOOL CNMWidgetState::OnLButtonDownWidget( UINT nFlags, CPoint point )
		{
			MAPWIDGET::iterator itCurrent = m_mapWidget.find( m_nCurrentWidgetKey );
			if( itCurrent == m_mapWidget.end() )
				return FALSE;

			if( (itCurrent->second) == NULL )
				return FALSE;

			return (itCurrent->second)->OnLButtonDownWidget( nFlags, point );
		}
		BOOL CNMWidgetState::OnLButtonUpWidget( UINT nFlags, CPoint point )
		{
			MAPWIDGET::iterator itCurrent = m_mapWidget.find( m_nCurrentWidgetKey );
			if( itCurrent == m_mapWidget.end() )
				return FALSE;

			if( (itCurrent->second) == NULL )
				return FALSE;

			return (itCurrent->second)->OnLButtonUpWidget( nFlags, point );
		}
		BOOL CNMWidgetState::OnLButtonDbClkWidget( UINT nFlags, CPoint point )
		{
			MAPWIDGET::iterator itCurrent = m_mapWidget.find( m_nCurrentWidgetKey );
			if( itCurrent == m_mapWidget.end() )
				return FALSE;

			if( (itCurrent->second) == NULL )
				return FALSE;

			return (itCurrent->second)->OnLButtonDbClkWidget( nFlags, point );
		}
		BOOL CNMWidgetState::OnRButtonDownWidget( UINT nFlags, CPoint point )
		{
			MAPWIDGET::iterator itCurrent = m_mapWidget.find( m_nCurrentWidgetKey );
			if( itCurrent == m_mapWidget.end() )
				return FALSE;

			if( (itCurrent->second) == NULL )
				return FALSE;

			return (itCurrent->second)->OnRButtonDownWidget( nFlags, point );
		}
		BOOL CNMWidgetState::OnRButtonUpWidget( UINT nFlags, CPoint point )
		{
			MAPWIDGET::iterator itCurrent = m_mapWidget.find( m_nCurrentWidgetKey );
			if( itCurrent == m_mapWidget.end() )
				return FALSE;

			if( (itCurrent->second) == NULL )
				return FALSE;

			return (itCurrent->second)->OnRButtonUpWidget( nFlags, point );
		}
		BOOL CNMWidgetState::OnRButtonDbClkWidget( UINT nFlags, CPoint point )
		{
			MAPWIDGET::iterator itCurrent = m_mapWidget.find( m_nCurrentWidgetKey );
			if( itCurrent == m_mapWidget.end() )
				return FALSE;

			if( (itCurrent->second) == NULL )
				return FALSE;

			return (itCurrent->second)->OnRButtonDbClkWidget( nFlags, point );
		}
		BOOL CNMWidgetState::OnMouseWheelWidget( UINT nFlags, short zDelta, CPoint point )
		{
			MAPWIDGET::iterator itCurrent = m_mapWidget.find( m_nCurrentWidgetKey );
			if( itCurrent == m_mapWidget.end() )
				return FALSE;

			if( (itCurrent->second) == NULL )
				return FALSE;

			return (itCurrent->second)->OnMouseWheelWidget( nFlags, zDelta, point );
		}
		BOOL CNMWidgetState::OnKeyDownWidget( int nKeyCode, int nExtra )
		{
			MAPWIDGET::iterator itCurrent = m_mapWidget.find( m_nCurrentWidgetKey );
			if( itCurrent == m_mapWidget.end() )
				return FALSE;

			if( (itCurrent->second) == NULL )
				return FALSE;

			return (itCurrent->second)->OnKeyDownWidget( nKeyCode, nExtra );
		}
		BOOL CNMWidgetState::OnCommandWidget( int nCommand, WPARAM wParam, LPARAM lParam )
		{
			MAPWIDGET::iterator itCurrent = m_mapWidget.find( m_nCurrentWidgetKey );
			if( itCurrent == m_mapWidget.end() )
				return FALSE;

			if( (itCurrent->second) == NULL )
				return FALSE;

			return (itCurrent->second)->OnCommandWidget( nCommand, wParam, lParam );
		}

		BOOL CNMWidgetState::ChangeWidget( int nWidgetKey )
		{
			MAPWIDGET::iterator itCurrent = m_mapWidget.find( m_nCurrentWidgetKey );
			if( itCurrent != m_mapWidget.end() )
			{
				if( (itCurrent->second) != NULL )
				{
					(itCurrent->second)->ExitWidget();
					m_nBeforeWidgtKey = m_nCurrentWidgetKey;
				}
			}

			MAPWIDGET::iterator itNext = m_mapWidget.find( nWidgetKey );
			if( itNext != m_mapWidget.end() )
			{
				if( (itNext->second) != NULL )
				{
					m_nCurrentWidgetKey = nWidgetKey;
					(itNext->second)->EnterWidget();			
				}
				else
					return FALSE;
			}
			else
				return FALSE;

			return TRUE;
		}

		BOOL CNMWidgetState::ChangeBeforeWidget()
		{
			return ChangeWidget( m_nBeforeWidgtKey );
		}

		//////////////////////////////////////////////////////////////////////////////
		// CObserverSet
		CObserverSet::CObserverSet()
		{
			m_VecWidget.clear();
		}
		CObserverSet::~CObserverSet()
		{
			m_VecWidget.clear();
		}
		void CObserverSet::AttachObserver( INMWidget* pObserver )
		{
			if( pObserver == NULL )	return;
			m_VecWidget.push_back( pObserver );
		}

		void CObserverSet::SendCommand( int nCommand, WPARAM wParam, LPARAM lParam )
		{
			if( nCommand <= 0 )	return;

			int nCount = m_VecWidget.size();
			for( int i=0; i<nCount; i++ )
			{
				if( m_VecWidget[i] == NULL )	continue;
				m_VecWidget[i]->OnCommandWidget( nCommand, wParam, lParam );
			}
		}



	} //namespace SKINGDI

}//namespace NMBASE