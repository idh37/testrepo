// NListControl.h: interface for the CNChatView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NLISTCONTROL_H__9FE77199_586E_4F5A_A40C_AA6B34FA3E6C__INCLUDED_)
#define AFX_NLISTCONTROL_H__9FE77199_586E_4F5A_A40C_AA6B34FA3E6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "skingdidefine.h"
#include "GraphButton.h"

namespace NMBASE
{
	namespace SKINGDI
	{
		class NMBASE::GRAPHICGDI::xSprite;
		class CChatViewEdit;

		class NMSKINGDI_CLASS CNChatView  
		{
		public:
			NMBASE::UTIL::STD_FONT				m_nFontIndex;
		public:
			CNChatView();
			CNChatView(int x, int y, int width, int height, CWnd* pParentWnd, NMBASE::GRAPHICGDI::CPage* pPage , bool bAutoScroll = true);	
			virtual ~CNChatView();

			CWnd*					m_pParentWnd;
			NMBASE::GRAPHICGDI::CPage*					m_pPage;
			int						m_Xp;
			int						m_Yp;
			int						m_Width;
			int						m_Height;
			NMBASE::GRAPHICGDI::xSprite*				m_pSprScroll;
			CChatViewEdit*			m_pChatViewEdit;

			bool					m_bOutline;
			COLORREF				m_clrOutline;

			CGraphButton			m_UpBtn;
			CGraphButton			m_ThumbBtn;
			CGraphButton			m_DnBtn;

			BOOL					m_bStartPressBtn;		//������ ������ �÷���
			DWORD					m_tStartPressTime;		//������ ������ Ÿ��

			BOOL					m_bPressBtn;	
			DWORD					m_tPressTime;
			CPoint					m_ptBottomClicked;

			int						m_nScrollHeight;

			BOOL					m_bScroll;
			BOOL					m_bShowThumb;

			int						m_NowIndex;
			bool					m_bAutoScroll;

			BOOL					m_bShow;

			BOOL					m_bHideText;		//��ȭ ���� ���߱� ��� 

			void BindScrollBtn(NMBASE::GRAPHICGDI::xSprite* pSpr, CPoint ptPos, int nHeight, int nUp, int nDown, int nThumb, int id = 8200);
			void SetResize(int width, int height);
			int GetTotalLine();					// ��ü ���ΰ����� ����
			int GetOnePageWriteLine();			// ȭ�鿡 �����ִ� ���� ������ ����

			void AddText(char *pStr, COLORREF crNewColor = RGB( 254, 255, 170 ));
			void AddText(CString *str, COLORREF crNewColor = RGB( 254, 255, 170 ));
			void SetFontIndex(NMBASE::UTIL::STD_FONT nFontIndex);

			void OnScrollButtonUp();			// ��ũ�� ����ư ��������
			void OnScrollButtonDown();			// ��ũ�� �ٿ��ư ��������
			void OnScrollDrag(int pos);			// ��ũ�ѹ� �巡�׽�
			void SetScrollBar();
			void SetOutlineColor(COLORREF clrOutline);

			void Draw(CDC *pDC = NULL);
			void DrawChatText(CDC* pDC);
			void ResetChat();	
			BOOL OnMouseMove(POINT pt);
			BOOL OnMouseOver(POINT pt);
			BOOL OnLButtonDown(int x, int y);
			BOOL OnLButtonUp(int x, int y);
			void UpdateButtonState( int nCmd );

			BOOL  OnPressProcess(); //1 Ŭ���� �����Ͽ� 100 �� �Ѿ����� 

			void ReSetWindowPos(int x, int y);

			BOOL IsShow(){
				return m_bShow;
			}
			void SetShow(BOOL bshow){
				m_bShow = bshow == TRUE ? 1 : 0;		
			}

			BOOL	OnMouseWheel( WPARAM wParam, LPARAM lParam );
			void	ThumbReposition(void);

			void	OnEnterRoom();
		};

	} //namespace SKINGDI

}//namespace NMBASE

#endif // !defined(AFX_NLISTCONTROL_H__9FE77199_586E_4F5A_A40C_AA6B34FA3E6C__INCLUDED_)
