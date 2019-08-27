#pragma once
#include "nmwidgetmanager.h"
#include "NMUnitWdget.h"
#include "NMButton.h"
#include "skingdidefine.h"

#define SPACE_SCROLL_RECT (0)
#define SPACE_BETWEEN_BUTTONS (1)
#define SPACE_BETWEEN_UNITS (0)

namespace NMBASE
{
	namespace SKINGDI
	{

		class NMSKINGDI_CLASS CNMListWidget :
			public CNMWidgetComposite
		{
		protected:
			CWnd*	m_pParent;

			vector<CNMUnitWidget*> m_vec_Units;
			vector<CNMButton*> m_vec_Btns;

			CNMButton* m_pButtonUp;
			CNMButton* m_pButtonDown;
			CNMThumButton* m_pButtonThum;

			POINT	m_ptPos;	


			CRect	m_rtScroll, m_rtWheel;


			int m_nControlID;

			int	m_nSpaceScrollRect, m_nSpaceBetweenButtons, m_nSpanceBetweenUnits;

			int m_nColumn; 
			int m_nRow;

			NMBASE::GRAPHICGDI::xSprite *m_pSprListUnit;
		protected:	
			virtual void RepositionScroll();
			virtual void RepositionUnit();
			virtual void RepositionButtons();

		public:

			virtual BOOL Initialize(int nControlID, CWnd *pParent, NMBASE::GRAPHICGDI::xSprite *pSprUnit, int nPosX, int nPosY);		
			virtual void AddSortButton(CNMButton *pButton);
			virtual CNMButton *GetSortButton(int nButtonID);
			virtual BOOL RemoveSortButton(int nButtonID);
			virtual void SetScrollButton(CNMButton *pUpButton, CNMButton *pDownButton, CNMThumButton *pThumButton);

			void Reposition(int nPosX, int nPosY);
			void SetUnitSize(int nRow, int nColumn);

			void SetSpaceScrollRect(int nSpaceScrollRect) {m_nSpaceScrollRect = nSpaceScrollRect;}
			void SetSpaceBetweenButtons(int nSpaceBetweenButtons) {m_nSpaceBetweenButtons = nSpaceBetweenButtons; }
			void SetSpaceBetweenUnits(int nSpanceBetweenUnits) {m_nSpanceBetweenUnits= nSpanceBetweenUnits;}

			void SetScrollTop(); //스크롤을 제일 위로 올린다.


			virtual int  GetTotalDataCount()=0;
			virtual BOOL RenewalList();
			virtual void OnResetDataList();
			virtual BOOL CreateWidget( CWnd* pParent );
			virtual void EnterWidget();
			virtual void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );
			virtual BOOL OnMouseMoveWidget( UINT nFlags, CPoint point );
			virtual BOOL OnCommandWidget( int nCommand, WPARAM wParam, LPARAM lParam );
			virtual CNMUnitWidget *CNMListWidget::CreateNewUnitWidget(int nIndex)=0;


			CNMListWidget(void);
			~CNMListWidget(void);
		};


	} //namespace SKINGDI

}//namespace NMBASE