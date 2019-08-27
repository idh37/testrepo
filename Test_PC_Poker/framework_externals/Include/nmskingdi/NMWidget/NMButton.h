#pragma once

#include "NMWidgetManager.h"
#include <string>
#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		//////////////////////////////////////////////////////////////////////////////
		// CNMButton

		enum BUTTON_STATE 
		{

			BUTTON_STATE_NORMAL = 0,
			BUTTON_STATE_OVER,
			BUTTON_STATE_DOWN,
			BUTTON_STATE_UP = BUTTON_STATE_OVER,
		};

		//////////////////////////////////////////////////////////////////////////////
		// CNMInvisibleButton
		class NMSKINGDI_CLASS CNMInvisibleButton : public CNMWidgetCompact
		{
		public:
			CNMInvisibleButton();
			virtual ~CNMInvisibleButton();

			virtual BOOL Initialize( int nControlID,
				CWnd* pParent,
				int nPosX, int nPosY,
				int nWidth, int nHeight );
			virtual void SetControlID(int nControlID);
			virtual void SetEnable( BOOL bFlag );
			virtual BOOL IsInRect( CPoint point );
			virtual void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC ) {}
			virtual BOOL OnMouseMoveWidget( UINT nFlags, CPoint point );
			virtual BOOL OnMouseLeaveWidget( UINT nFlags, CPoint point );
			virtual BOOL OnLButtonDownWidget( UINT nFlags, CPoint point );
			virtual BOOL OnLButtonUpWidget( UINT nFlags, CPoint point );

			virtual CRect GetButtonRect() { return m_rectPosition; }
			virtual BOOL GetEnable() { return m_bEnable; }
			virtual int GetControlID() { return m_nControlID; }


		protected:
			int				m_nControlID;
			CWnd*			m_pParent;
			CRect			m_rectPosition;
			CRect			m_rectBasePosition;
			int				m_nWidth, m_nHeight;

			BOOL			m_bEnable;
			BUTTON_STATE	m_nState;


		};

		//////////////////////////////////////////////////////////////////////////////
		// CNMButton
		enum {
			BUTTON_TOOLTIP_HL = -1000,
			BUTTON_TOOLTIP_HC = 0,
			BUTTON_TOOLTIP_HR = 1000,

			BUTTON_TOOLTIP_VT = -1000,
			BUTTON_TOOLTIP_VC = 0,
			BUTTON_TOOLTIP_VB = 1000,
		};

		class NMSKINGDI_CLASS CNMButton : public CNMInvisibleButton
		{
		public:
			CNMButton();
			virtual ~CNMButton();

			virtual BOOL Initialize( int nControlID,
				CWnd* pParent,
				int nPosX, int nPosY,
				int nWidth, int nHeight );
			virtual BOOL Initialize( int nControlID,
				CWnd* pParent,
				int nPosX, int nPosY,
				char* szSprKey,
				int nSprID,
				int nSprCount );

			virtual BOOL SetSprite( const char* szSprKey,	int nSprID,	int nSprCount );
			virtual void SetSound( char* szOver, char* szDown );
			virtual void SetTooltip( char* szTooTip, int nTootipOffsetX=0, int nTootipOffsetY=0 );
			virtual void SetSendParam( WPARAM wParam, LPARAM lParam );
			virtual void SetPosition( int nPosX, int nPosY );
			virtual void SetRelationPosition( int nOffsetX, int nOffsetY );
			virtual WPARAM GetwParam()	{ return m_wParam; }
			virtual LPARAM GetlParam()	{ return m_lParam; }
			virtual int GetPosX()		{ return m_rectPosition.left; }
			virtual int GetPosY()		{ return m_rectPosition.top; }

			virtual void Show(BOOL bShow);
			virtual void SetEnable( BOOL bFlag );
			virtual void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );
			virtual void DrawToolTip( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );
			virtual BOOL IsDrawAbleToolTip();
			virtual BOOL OnMouseMoveWidget( UINT nFlags, CPoint point );
			virtual BOOL OnMouseLeaveWidget( UINT nFlags, CPoint point );
			virtual BOOL OnLButtonDownWidget( UINT nFlags, CPoint point );
			virtual BOOL OnLButtonUpWidget( UINT nFlags, CPoint point );	

		protected:
			NMBASE::GRAPHICGDI::xSprite*	m_pSprite;
			int			m_nSprID;
			int			m_nSprCount;
			int			m_nCurSprID;

			std::string m_strOverSnd;
			std::string m_strDownSnd;

			BOOL		m_bOver;
			DWORD		m_dwOverTick;
			BOOL		m_bDown;
			DWORD		m_dwDownTick;

			std::string m_strTootip;
			int			m_nTooltipOffsetX, m_nTooltipOffsetY;

			WPARAM		m_wParam;
			LPARAM		m_lParam;

			BOOL		m_bShow;
		};
		//////////////////////////////////////////////////////////////////////////////
		// CNMSceneButton
		class CNMSceneButton : public CNMButton
		{
		public:
			CNMSceneButton();
			virtual ~CNMSceneButton();

			virtual BOOL SetSprite( char* szSprKey,	int nSprID,	int nSprCount );
			virtual void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );
			virtual BOOL OnMouseMoveWidget( UINT nFlags, CPoint point );
			virtual BOOL OnLButtonDownWidget( UINT nFlags, CPoint point );
			virtual BOOL OnLButtonUpWidget( UINT nFlags, CPoint point );

		protected:
			DWORD	m_dwCurSprTick;
		};

		//////////////////////////////////////////////////////////////////////////////
		// CNMMultiSceneButton
		class NMSKINGDI_CLASS CNMMultiSceneButton : public CNMSceneButton
		{
		public:
			CNMMultiSceneButton();
			virtual ~CNMMultiSceneButton();

			virtual void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );
		};

		//////////////////////////////////////////////////////////////////////////////
		// CNMThumButton
		class NMSKINGDI_CLASS CNMThumButton : public CNMButton
		{
		public:
			CNMThumButton();
			virtual ~CNMThumButton();

			void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );

			BOOL OnMouseMoveWidget( UINT nFlags, CPoint point );
			BOOL OnMouseLeaveWidget( UINT nFlags, CPoint point );
			BOOL OnLButtonDownWidget( UINT nFlags, CPoint point );
			BOOL OnLButtonUpWidget( UINT nFlags, CPoint point );
			BOOL OnMouseWheelWidget( UINT nFlags, short zDelta, CPoint point );

			BOOL SetThumAttribute( CNMButton* pUpButton, CNMButton* pDownButton,
				CRect rectWheel,
				int nElementCountPerPage, int nColumnCount = 1 );
			void SetThumPosition( CNMButton* pUpButton, CNMButton* pDownButton,
				CRect rectWheel );
			void SetAutoLastPageAttri( BOOL bFlag );
			void SetElementCount( int nElementCount );

			void SetupThumPosition();
			void ChangePosition( int nPosX, int nPosY );

			void SetCurrentPage( int nPage );
			void BeforePage();
			void NextPage();
			int	GetStartElement();

		protected:
			CRect	m_rectThum;
			BOOL	m_bDrag;
			CPoint	m_ptDragPoint;

			CRect	m_rectWheel;
			int		m_nColumnCount;
			int		m_nElementCountPerPage;
			int		m_nMaxPageCount;
			int		m_nCurrentPage;
			float	m_fUnitHeight;

			int		m_WheelPosX;
			int		m_WheelPosY;

			BOOL	m_bAutoLastPage;
		};


		//////////////////////////////////////////////////////////////////////////////
		// CNMCheckButton

		class NMSKINGDI_CLASS CNMCheckButton : public CNMButton
		{
		public:
			CNMCheckButton();
			virtual ~CNMCheckButton();

			void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );

			BOOL OnMouseMoveWidget( UINT nFlags, CPoint point );
			BOOL OnLButtonDownWidget( UINT nFlags, CPoint point );
			BOOL OnLButtonUpWidget( UINT nFlags, CPoint point );


			bool GetButtonState();
			void SetButtonState( bool state );

		protected:
			bool	m_bClick;
		};

	} //namespace SKINGDI

}//namespace NMBASE