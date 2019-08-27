#pragma once

#include "NMWidgetManager.h"
#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		//////////////////////////////////////////////////////////////////////////////
		// CNMDialog
		class NMSKINGDI_CLASS CNMDialog : public CNMWidgetComposite
		{
		public:
			CNMDialog();
			virtual ~CNMDialog();

			void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );
			BOOL OnMouseMoveWidget( UINT nFlags, CPoint point );
			BOOL OnMouseLeaveWidget( UINT nFlags, CPoint point );
			BOOL OnLButtonDownWidget( UINT nFlags, CPoint point );
			BOOL OnLButtonDbClkWidget( UINT nFlags, CPoint point );
			BOOL OnLButtonUpWidget( UINT nFlags, CPoint point );
			BOOL OnRButtonDownWidget( UINT nFlags, CPoint point );
			BOOL OnKeyDownWidget( int nKeyCode, int nExtra );
			BOOL OnCommandWidget( int nCommand, WPARAM wParam, LPARAM lParam );

		public:
			virtual BOOL PopUpDialog();
			virtual void PopDownDialog();
			virtual BOOL IsShow()	{ return m_bDisplay; }

		protected:
			virtual void InitializeDialog( CWnd* pParent,int nWidthGameView, int nHeightGameView );
			virtual void DrawDialog( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC ) = 0;
			virtual BOOL OnMouseMoveDialog( UINT nFlags, CPoint point ){ return FALSE; }
			virtual BOOL OnLButtonDownDialog( UINT nFlags, CPoint point ){ return FALSE; }
			virtual BOOL OnRButtonDownDialog( UINT nFlags, CPoint point ){ return FALSE; }


			virtual BOOL OnLButtonUpDialog( UINT nFlags, CPoint point ){ return FALSE; }
			virtual BOOL OnKeyDownDialog( int nKeyCode, int nExtra ) = 0;
			virtual BOOL OnCommandDialog( int nCommand, WPARAM wParam, LPARAM lParam ) = 0;

			virtual void SetSprite( char* szSprKey, int nBGIndex = 0, int nPosX = 0, int nPosY = 0 );
			virtual void SetDragEnable( BOOL bFlag )	{ m_bDragEnable = bFlag; }	
			virtual BOOL IsInWindowRect( CPoint ptPoint );

			virtual void RelationChildPosition();
			virtual void SetPosition(int nPosX, int nPosY);




		protected:
			CWnd*			m_pParent;
			CSize			m_sizeGameView;
			BOOL			m_bDisplay;
			CRect			m_rectPosition;
			int				m_nWidth;
			int				m_nHeight;
			NMBASE::GRAPHICGDI::xSprite*		m_pSprite;
			int				m_nBGIndex;

			// Drag Process ////////////////////////////////////
			BOOL			m_bDragEnable;
			BOOL			m_bDrag;
			int				m_nDragGepX;
			int				m_nDragGepY;
			CPoint			GetWindowRectInScreen( int nleft, int ntop );
			////////////////////////////////////////////////////
		};

	} //namespace SKINGDI

}//namespace NMBASE