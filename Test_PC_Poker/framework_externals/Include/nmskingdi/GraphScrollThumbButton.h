// GraphScrollThumbButton.h: interface for the CGraphScrollThumbButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHSCROLLTHUMBBUTTON_H__CB01CA1D_7120_41A7_ACE6_5DCB06C580AC__INCLUDED_)
#define AFX_GRAPHSCROLLTHUMBBUTTON_H__CB01CA1D_7120_41A7_ACE6_5DCB06C580AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MIN_THUMB_HEIGHT_SIZE 6	//최대 줄어들수있는 사이즈

#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		class NMSKINGDI_CLASS CGraphScrollThumbButton
		{

		protected:
			CWnd		*m_pParent;
			NMBASE::GRAPHICGDI::CPage		*m_pPage;
			NMBASE::GRAPHICGDI::xSprite		*m_pSpr;
			BOOL		m_bShow;
			BOOL		m_bEnable;

			int		m_iButtonNumber;
		public:

			inline  BOOL IsShow() const	  { return m_bShow; }
			inline  BOOL IsEnable() const { return m_bEnable; }

			int		m_RealWidth;		//이미지 원본 사이즈 
			int		m_RealHeight;

			int		m_RealXp, m_RealYp;	//실제 처음 셋팅된 값

			int		m_Xp, m_Yp;	
			int		m_Width, m_Height; //변동되는 가로 세로 사이즈
			BOOL	m_bClick;
			BOOL	m_bOver;
			int		m_CmndID;
			BOOL    m_SceneObject;

			//	========= 마우스 오버됐을때 한번만 나오게 한다 =============
			//	사운드 번호가 지정되었다
			bool	m_bOverSoundInit;
			bool	m_bOverSound;
			//	볼룸
			int		m_SoundVol;

			int		m_SoundNum;

			void	SetMouseOverSound( int snd, int vol = 100 );
			void	MouseOverSoundNoUse( void );
			POINT	GetButtonPoint( void );
			//	============================================================

			//	===================== 윈도 이동 ===========================================
			void	MoveWindow( int x, int y ) { m_Xp = x; m_Yp = y; }
			void	MoveWindow( POINT point ) { MoveWindow( point.x, point.y ); }
			void	MoveWindowEx( int x, int y ) { m_Xp += x; m_Yp += y; }
			//	===========================================================================

			void SetButtonPos(int x, int y, NMBASE::GRAPHICGDI::xSprite *pspr);
			void SetButtonPos(int x, int y);
			int		m_ButtonCnt;

			CGraphScrollThumbButton();
			virtual ~CGraphScrollThumbButton();

			void Init(CWnd *pWnd, NMBASE::GRAPHICGDI::CPage *ppage, int x, int y, NMBASE::GRAPHICGDI::xSprite *pspr, int iButtonNumber, int cmndid, int ButtonCnt = 4,bool scenebtn = false, bool bSound = false );
			void SetSprite(NMBASE::GRAPHICGDI::xSprite *pspr, int iButtonNumber);
			void Enable(BOOL bVal);
			void Show(BOOL bVal);
			virtual void InitBtn(){};
			virtual BOOL OnMouseMove(int x, int y);
			virtual BOOL OnLButtonDown(int x, int y);
			virtual BOOL OnLButtonUp(int x, int y);
			virtual void Draw( CDC *pDC, int opt = 0, int grade = 0 );
			virtual int GetButtonState( void );
			virtual void SetButtonState( int state );
			CWnd * GetParent(){return m_pParent;}
			NMBASE::GRAPHICGDI::xSprite* GetSprite(){ return m_pSpr;}	

			// 줄어드는 길이 값으로 이미지 가로 또는 세로 사이즈를 줄인다
			BOOL SetChangeScrollThumbSize( int w_shorten , int h_shorten );

			//방에서 나갈때 또는 들어오는부분에서 초기화 하는 부분 
			void ReSetScrollThumSize();
		};

	} //namespace SKINGDI

}//namespace NMBASE

#endif // !defined(AFX_GRAPHSCROLLTHUMBBUTTON_H__CB01CA1D_7120_41A7_ACE6_5DCB06C580AC__INCLUDED_)
