// GraphScrollThumbButton.h: interface for the CGraphScrollThumbButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHSCROLLTHUMBBUTTON_H__CB01CA1D_7120_41A7_ACE6_5DCB06C580AC__INCLUDED_)
#define AFX_GRAPHSCROLLTHUMBBUTTON_H__CB01CA1D_7120_41A7_ACE6_5DCB06C580AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MIN_THUMB_HEIGHT_SIZE 6	//�ִ� �پ����ִ� ������

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

			int		m_RealWidth;		//�̹��� ���� ������ 
			int		m_RealHeight;

			int		m_RealXp, m_RealYp;	//���� ó�� ���õ� ��

			int		m_Xp, m_Yp;	
			int		m_Width, m_Height; //�����Ǵ� ���� ���� ������
			BOOL	m_bClick;
			BOOL	m_bOver;
			int		m_CmndID;
			BOOL    m_SceneObject;

			//	========= ���콺 ���������� �ѹ��� ������ �Ѵ� =============
			//	���� ��ȣ�� �����Ǿ���
			bool	m_bOverSoundInit;
			bool	m_bOverSound;
			//	����
			int		m_SoundVol;

			int		m_SoundNum;

			void	SetMouseOverSound( int snd, int vol = 100 );
			void	MouseOverSoundNoUse( void );
			POINT	GetButtonPoint( void );
			//	============================================================

			//	===================== ���� �̵� ===========================================
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

			// �پ��� ���� ������ �̹��� ���� �Ǵ� ���� ����� ���δ�
			BOOL SetChangeScrollThumbSize( int w_shorten , int h_shorten );

			//�濡�� ������ �Ǵ� �����ºκп��� �ʱ�ȭ �ϴ� �κ� 
			void ReSetScrollThumSize();
		};

	} //namespace SKINGDI

}//namespace NMBASE

#endif // !defined(AFX_GRAPHSCROLLTHUMBBUTTON_H__CB01CA1D_7120_41A7_ACE6_5DCB06C580AC__INCLUDED_)
