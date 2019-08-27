// GraphButtonList.h: interface for the CGraphButtonList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHBUTTONLIST_H__2A35DA0A_B581_4371_B827_D0947C9BFE77__INCLUDED_)
#define AFX_GRAPHBUTTONLIST_H__2A35DA0A_B581_4371_B827_D0947C9BFE77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "GraphButton.h"
#include "GraphButtonEx.h"
#include <map>
#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		typedef map< int , CGraphButton* > BtnList;
		typedef map< int , CGraphButtonEx* > BtnListEx;

		/// 그래픽 버튼 리스트 클래스.
		class NMSKINGDI_CLASS CGraphButtonList  
		{
		public:

			CGraphButtonList();
			virtual ~CGraphButtonList();

			BtnList m_Btn;

			static BOOL    m_bUseCursor;
			static HCURSOR m_Normal;
			static HCURSOR m_Select;

			void Init();
			void SetButtonCursor( UINT hNormalCursorID , UINT hSelectCursorID );

			void Add(CGraphButton * pBtn);
			void Add(CGraphButton * pBtn,CWnd *pWnd, NMBASE::GRAPHICGDI::CPage *ppage, int x, int y, NMBASE::GRAPHICGDI::xSprite *pspr, int iButtonNumber, int cmndid, int ButtonCnt = 4, bool bSound = false);

			BOOL OnLButtonDown(int mx , int  my );
			BOOL OnLButtonUp(int mx , int  my );
			BOOL OnMouseMove(int mx , int  my );

			void Invalidate();
			void Draw( CDC* pDC );

			void ShowWindow( int cmdid , BOOL bShow );	
			void Enable( int cmdid , BOOL bVal);

			void AllEnable(BOOL bshow);

			CGraphButton* GetButton( int cmdid );
		};


		class NMSKINGDI_CLASS CGraphButtonListEx  
		{
		public:

			CGraphButtonListEx();
			virtual ~CGraphButtonListEx();

			BtnListEx m_Btn;

			void Init();	

			void Add(CGraphButtonEx * pBtn);
			void Add(CGraphButtonEx * pBtn,CWnd *pWnd, NMBASE::GRAPHICGDI::CPage *ppage, int x, int y, NMBASE::GRAPHICGDI::xSprite *pspr, int iButtonNumber, int cmndid, int ButtonCnt = 4, bool bSound = false);

			BOOL OnLButtonDown(int mx , int  my );
			BOOL OnLButtonUp(int mx , int  my );
			BOOL OnMouseMove(int mx , int  my );

			void Invalidate();
			void Draw( CDC* pDC );

			void ShowWindow( int cmdid , BOOL bShow );	
			void Enable( int cmdid , BOOL bVal);

			void AllEnable(BOOL bshow);

			CGraphButtonEx* GetButton( int cmdid );
		};

	} //namespace SKINGDI

}//namespace NMBASE

#endif // !defined(AFX_GRAPHBUTTONLIST_H__2A35DA0A_B581_4371_B827_D0947C9BFE77__INCLUDED_)
