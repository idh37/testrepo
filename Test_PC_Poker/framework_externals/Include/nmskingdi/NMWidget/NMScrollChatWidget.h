#pragma once
#include "nmchatwidget.h"
#include "NMButton.h"
#include "skingdidefine.h"


#define SPACE_CHAT_VIEW_SCROLL_RECT (1)		//채팅박스와 스크롤 사이 간격
#define OFFSET_SCROLL_BUTTONS (0)			//스크롤 버튼의 위아래 간격을 얼만큼 더 벌릴것인가.

namespace NMBASE
{
	namespace SKINGDI
	{

		class NMSKINGDI_CLASS CNMScrollChatWidget :
			public CNMChatWidget
		{



			CNMButton* m_pButtonUp;
			CNMButton* m_pButtonDown;
			CNMThumButton* m_pButtonThum;





			int m_nSpaceScrollRect;
			int m_nOffsetScrollBtns;

			CRect m_rtScroll;

		protected:
			void RepositionScroll();
		public:
			CNMScrollChatWidget(void);
			virtual ~CNMScrollChatWidget(void);


			virtual BOOL CreateWidget( CWnd* pParent );
			virtual BOOL Initialize( int nControlID, CWnd* pParent, CRect rectEdit,
				int nLinePerPage, int nLinePerLimit );
			virtual BOOL OnCommandWidget( int nCommand, WPARAM wParam, LPARAM lParam );


			void SetSpaceScrollRect(int nSpaceScrollRect) {m_nSpaceScrollRect = nSpaceScrollRect;}
			void SetOffsetScrollBtns(int nOffsetScrollBtns) {m_nOffsetScrollBtns = nOffsetScrollBtns;}

			void SetScrollButton(CNMButton *pUpButton, CNMButton *pDownButton, CNMThumButton *pThumButton);

			virtual void SetPosition(int nPosX, int nPosY);


			virtual BOOL OnMouseMoveWidget( UINT nFlags, CPoint point );	
			virtual BOOL OnLButtonDownWidget( UINT nFlags, CPoint point );
			virtual BOOL OnLButtonUpWidget( UINT nFlags, CPoint point );
			virtual BOOL OnLButtonDbClkWidget( UINT nFlags, CPoint point );

		};

	} //namespace SKINGDI

}//namespace NMBASE