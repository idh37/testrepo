#pragma once
#include "nmwidgetmanager.h"
#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		class NMSKINGDI_CLASS CNMUnitWidget : public CNMWidgetCompact
		{
		public:
			CNMUnitWidget(void);
			virtual ~CNMUnitWidget(void);


			//BOOL Initialize( int nControlID, int nButtonID, CWnd* pParent, int nPosX, int nPosY );
			virtual BOOL Initialize( int nControlID, CWnd* pParent, NMBASE::GRAPHICGDI::xSprite *pSpr, int nPosX, int nPosY );
			virtual void SetSound( char* szOver, char* szDown );
			virtual void UpdateWidget( DWORD dwCurTick );
			virtual void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )=0;
			virtual BOOL OnMouseMoveWidget( UINT nFlags, CPoint point );
			virtual BOOL OnMouseLeaveWidget( UINT nFlags, CPoint point );
			virtual BOOL OnLButtonDownWidget( UINT nFlags, CPoint point );
			virtual BOOL OnLButtonUpWidget( UINT nFlags, CPoint point );
			virtual BOOL OnLButtonDbClkWidget( UINT nFlags, CPoint point );

			void SetSetInTick( DWORD dwDelay );
			void SetOverLayerRect( CRect rectOverLayer, CRect rectOverLimit );
			void DrawOverLayer( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );
			RECT *GetRect();
			int GetHeight();
			int GetWidth();
			int	 GetControlD()
			{
				return m_nControlID;
			}

			virtual void Repostion(int nPosX, int nPosY);



		protected:
			int			m_nControlID;
			CWnd*		m_pParent;
			CRect		m_rectPosition;
			CRect		m_rectTargetPosition;

			BOOL		m_bUse;

			BOOL		m_bOver;
			DWORD		m_dwOverTick;
			BOOL		m_bDown;
			DWORD		m_dwDownTick;

			std::string m_strOverSnd;
			std::string m_strDownSnd;


			CRect		m_rectOverLayer;
			CRect		m_rectOverLimit;

			DWORD		m_dwSetInTick;

			NMBASE::GRAPHICGDI::xSprite *m_pSprUnit;

		};


	} //namespace SKINGDI

}//namespace NMBASE