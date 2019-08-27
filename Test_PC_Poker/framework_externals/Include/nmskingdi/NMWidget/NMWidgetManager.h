#pragma once

#include <map>
#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		enum WIDGET_ID_	
		{

			WIDGET_ID_NULL			= 0,

			WIDGET_ID_START			= WM_USER + 1000,

			WIDGET_ID_EDIT_ENTER,
			WIDGET_ID_EDIT_NORMAL,
			WIDGET_ID_EDIT_WARNING,

			WIDGET_ID_END			= WM_USER + 1999,
		};

		class CNMWidgetComposite;
		class CNMWidgetState;

		///////////////////////////////////////////////////////////////////////////////////
		// INMWidget
		class NMSKINGDI_CLASS INMWidget
		{
		public:
			INMWidget() {}
			virtual ~INMWidget() {}

			virtual BOOL CreateWidget( CWnd* pParent ) = 0;
			virtual void EnterWidget() = 0;
			virtual void UpdateWidget( DWORD dwCurTick ) = 0;
			virtual void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC ) = 0;
			virtual void ExitWidget() = 0;
			virtual void DestroyWidget() = 0;

			virtual BOOL OnMouseMoveWidget( UINT nFlags, CPoint point ) = 0;
			virtual BOOL OnMouseLeaveWidget( UINT nFlags, CPoint point ) = 0;
			virtual BOOL OnLButtonDownWidget( UINT nFlags, CPoint point ) = 0;
			virtual BOOL OnLButtonUpWidget( UINT nFlags, CPoint point ) = 0;
			virtual BOOL OnLButtonDbClkWidget( UINT nFlags, CPoint point ) = 0;
			virtual BOOL OnRButtonDownWidget( UINT nFlags, CPoint point ) = 0;
			virtual BOOL OnRButtonUpWidget( UINT nFlags, CPoint point ) = 0;
			virtual BOOL OnRButtonDbClkWidget( UINT nFlags, CPoint point ) = 0;
			virtual BOOL OnMouseWheelWidget( UINT nFlags, short zDelta, CPoint point ) = 0;
			virtual BOOL OnKeyDownWidget( int nKeyCode, int nExtra ) = 0;

			virtual BOOL OnCommandWidget( int nCommand, WPARAM wParam, LPARAM lParam ) = 0;

			virtual BOOL IsComposite()	{	return FALSE;	}

		protected:
			virtual BOOL NMSendMessage( CWnd* pParent, int nControlID, WPARAM wParam = 0, LPARAM lParam = 0 )
			{
				if( pParent && nControlID != WIDGET_ID_NULL )
				{
					pParent->SendMessage( nControlID, wParam, lParam );
					return TRUE;
				}
				return FALSE;
			}
		};

		///////////////////////////////////////////////////////////////////////////////////
		// CNMWidgetCompact
		class NMSKINGDI_CLASS CNMWidgetCompact : public INMWidget
		{
		public:
			CNMWidgetCompact() {}
			virtual ~CNMWidgetCompact() {}

			virtual BOOL CreateWidget( CWnd* pParent ){	return TRUE; }
			virtual void EnterWidget(){}
			virtual void UpdateWidget( DWORD dwCurTick ){}
			virtual void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC ){}
			virtual void ExitWidget(){}
			virtual void DestroyWidget(){}

			virtual BOOL OnMouseMoveWidget( UINT nFlags, CPoint point ){ return FALSE; }
			virtual BOOL OnMouseLeaveWidget( UINT nFlags, CPoint point ){ return FALSE; }
			virtual BOOL OnLButtonDownWidget( UINT nFlags, CPoint point ){ return FALSE; }
			virtual BOOL OnLButtonUpWidget( UINT nFlags, CPoint point ){ return FALSE; }
			virtual BOOL OnLButtonDbClkWidget( UINT nFlags, CPoint point ){ return FALSE; }
			virtual BOOL OnRButtonDownWidget( UINT nFlags, CPoint point ){ return FALSE; }
			virtual BOOL OnRButtonUpWidget( UINT nFlags, CPoint point ){ return FALSE; }
			virtual BOOL OnRButtonDbClkWidget( UINT nFlags, CPoint point ){ return FALSE; }
			virtual BOOL OnMouseWheelWidget( UINT nFlags, short zDelta, CPoint point ){ return FALSE; }
			virtual BOOL OnKeyDownWidget( int nKeyCode, int nExtra ){ return FALSE; }
			virtual BOOL OnCommandWidget( int nCommand, WPARAM wParam, LPARAM lParam ){ return FALSE; }
		};


		///////////////////////////////////////////////////////////////////////////////////
		// CNMWidgetComposite
		class NMSKINGDI_CLASS CNMWidgetComposite : public INMWidget
		{
			friend class CNMWidgetState;
			friend class CNMDialog;

		public:
			CNMWidgetComposite();
			virtual ~CNMWidgetComposite();

			virtual BOOL CreateWidget( CWnd* pParent );
			virtual void EnterWidget();
			virtual void UpdateWidget( DWORD dwCurTick );
			virtual void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );
			virtual void ExitWidget();
			virtual void DestroyWidget();

			virtual BOOL OnMouseMoveWidget( UINT nFlags, CPoint point );
			virtual BOOL OnMouseLeaveWidget( UINT nFlags, CPoint point );
			virtual BOOL OnLButtonDownWidget( UINT nFlags, CPoint point );
			virtual BOOL OnLButtonUpWidget( UINT nFlags, CPoint point );
			virtual BOOL OnLButtonDbClkWidget( UINT nFlags, CPoint point );
			virtual BOOL OnRButtonDownWidget( UINT nFlags, CPoint point );
			virtual BOOL OnRButtonUpWidget( UINT nFlags, CPoint point );
			virtual BOOL OnRButtonDbClkWidget( UINT nFlags, CPoint point );
			virtual BOOL OnMouseWheelWidget( UINT nFlags, short zDelta, CPoint point );
			virtual BOOL OnKeyDownWidget( int nKeyCode, int nExtra );
			virtual BOOL OnCommandWidget( int nCommand, WPARAM wParam, LPARAM lParam );

			virtual BOOL AttachWidget( int nWidgetKey, INMWidget* pIWidget );
			virtual INMWidget* GetWidget( int nWidgetKey );	//find a widget out of children
			virtual INMWidget* FindWidget( int nWidgetKey );	//find a widget out of decendants 
			virtual BOOL IsComposite()	{	return TRUE;	}

		private:
			typedef std::map< int, INMWidget* > MAPWIDGET;
			MAPWIDGET							m_mapWidget;

			// 일단 쓰지 못하게 하자. 추후 확장용	///////////////////////////////////
		public:
			virtual BOOL DetachWidget( int nWidgetKey );

			//private:
			//	virtual INMWidget* GetGlobalWidget( int nWidgetKey );
			//	virtual BOOL AttachGlobalWidget( int nWidgetKey, INMWidget* pIWidget );
			//	virtual BOOL DetachGlobalWidget( int nWidgetKey );
			//	static MAPWIDGET					m_mapGlobalWidget;
		};

		///////////////////////////////////////////////////////////////////////////////////
		// CNMWidgetState
		class NMSKINGDI_CLASS CNMWidgetState : public CNMWidgetComposite
		{
		public:
			CNMWidgetState();
			virtual ~CNMWidgetState();

			virtual void EnterWidget();
			virtual void UpdateWidget( DWORD dwCurTick );
			virtual void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );
			virtual void ExitWidget();

			virtual BOOL OnMouseMoveWidget( UINT nFlags, CPoint point );
			virtual BOOL OnMouseLeaveWidget( UINT nFlags, CPoint point );
			virtual BOOL OnLButtonDownWidget( UINT nFlags, CPoint point );
			virtual BOOL OnLButtonUpWidget( UINT nFlags, CPoint point );
			virtual BOOL OnLButtonDbClkWidget( UINT nFlags, CPoint point );
			virtual BOOL OnRButtonDownWidget( UINT nFlags, CPoint point );
			virtual BOOL OnRButtonUpWidget( UINT nFlags, CPoint point );
			virtual BOOL OnRButtonDbClkWidget( UINT nFlags, CPoint point );
			virtual BOOL OnMouseWheelWidget( UINT nFlags, short zDelta, CPoint point );
			virtual BOOL OnKeyDownWidget( int nKeyCode, int nExtra );
			virtual BOOL OnCommandWidget( int nCommand, WPARAM wParam, LPARAM lParam );

			virtual BOOL ChangeWidget( int nWidgetKey );
			virtual BOOL ChangeBeforeWidget();

			virtual int GetCurrentState()	{	return m_nCurrentWidgetKey;		}
		private:
			int		m_nCurrentWidgetKey;
			int		m_nBeforeWidgtKey;
		};


		//////////////////////////////////////////////////////////////////////////////
		// CObserverSet
		class NMSKINGDI_CLASS CObserverSet
		{
		public:
			CObserverSet();
			virtual ~CObserverSet();

			void AttachObserver( INMWidget* pObserver );
			void SendCommand( int nCommand, WPARAM wParam, LPARAM lParam );

		protected:
			typedef std::vector< INMWidget* >	VECTORWIDGET;
			VECTORWIDGET	m_VecWidget;
		};


	} //namespace SKINGDI

}//namespace NMBASE