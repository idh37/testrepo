#include "StdAfx.h"
#include "./NMWidget/NMListWidget.h"
#include "./NMWidget/NMButton.h"
#include "./NMWidget/NMResourceManager.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		CNMListWidget::CNMListWidget(void)
		{
			m_pParent=NULL;

			m_pButtonUp=NULL;
			m_pButtonDown=NULL;
			m_pButtonThum=NULL;
			m_nControlID=0;

			m_nSpaceScrollRect=SPACE_SCROLL_RECT;
			m_nSpaceBetweenButtons=SPACE_BETWEEN_BUTTONS;
			m_nSpanceBetweenUnits=SPACE_BETWEEN_UNITS;

			m_nColumn=0;
			m_nRow = 0;


			m_pSprListUnit=NULL;


		}

		CNMListWidget::~CNMListWidget(void)
		{

		}


		void CNMListWidget::RepositionScroll()
		{
			//사이즈 변동시 스크롤 영역을 재구성할 필요가 있을 때
			if (m_vec_Btns.size()==0)
				return;


			/*

			CRect rtRightEndBtn = (m_vec_Btns[m_vec_Btns.size()-1])->GetButtonRect();
			CRect rtUpBtn = m_pButtonUp->GetButtonRect();
			CRect rtDownBtn = m_pButtonDown->GetButtonRect();

			int scrrect_left = 	rtRightEndBtn.right- rtUpBtn.Width() - m_nSpaceScrollRect;
			int scrrect_top = 	rtRightEndBtn.bottom + m_nSpaceScrollRect;

			int height_units = 0;
			if (m_vec_Units.size()>0)
			{		
			height_units = (   (*m_vec_Units.begin())->GetHeight() * (int) m_vec_Units.size()  )
			+  (   m_nSpanceBetweenUnits * ((int) m_vec_Units.size()-1) );
			}

			int Height = m_ptPos.y+rtRightEndBtn.Height() + height_units;

			int screct_bottom = Height - rtDownBtn.Height() - m_nSpaceScrollRect;

			int screct_right = scrrect_left+rtUpBtn.Width();

			*/

			CRect rtRightEndBtn = (m_vec_Btns[m_vec_Btns.size()-1])->GetButtonRect();
			CRect rtUpBtn = m_pButtonUp->GetButtonRect();
			CRect rtDownBtn = m_pButtonDown->GetButtonRect();

			int scrrect_left = 	0;	
			int scrrect_top = 	m_ptPos.y + rtRightEndBtn.Height() + m_nSpaceScrollRect -1;

			int height_units = 0;		//유닛높이
			if (m_vec_Units.size()>0)
			{		
				scrrect_left =m_ptPos.x
					+ ( (*m_vec_Units.begin())->GetWidth() * m_nColumn )
					+( m_nSpanceBetweenUnits * (m_nColumn-1) );

				height_units = (   (*m_vec_Units.begin())->GetHeight() * m_nRow  )
					+  (   m_nSpanceBetweenUnits * (m_nRow-1) );
			}

			int Height = m_ptPos.y + rtRightEndBtn.Height() + height_units; 

			int screct_bottom = Height - rtDownBtn.Height() - m_nSpaceScrollRect;

			int screct_right = scrrect_left+rtUpBtn.Width();


			m_rtScroll.SetRect( scrrect_left, 
				scrrect_top, 
				screct_right,
				screct_bottom);

			m_rtWheel.SetRect(	m_ptPos.x , 
				scrrect_top, 
				screct_right,
				screct_bottom);

			m_pButtonUp->SetPosition(m_rtScroll.left, m_rtScroll.top);
			m_pButtonDown->SetPosition(m_rtScroll.left, m_rtScroll.bottom);	
			m_pButtonThum->SetThumPosition(m_pButtonUp, m_pButtonDown, &m_rtWheel);
			m_pButtonThum->SetElementCount(GetTotalDataCount());

		}

		BOOL CNMListWidget::CreateWidget( CWnd* pParent)
		{
			m_pParent = pParent;	

			RepositionScroll();
			RepositionUnit();	

			return CNMWidgetComposite::CreateWidget( pParent );;
		}

		BOOL CNMListWidget::Initialize(int nControlID, CWnd *pParent, NMBASE::GRAPHICGDI::xSprite *pSprUnit, int nPosX, int nPosY)
		{
			m_nControlID = nControlID;
			m_pSprListUnit = pSprUnit;
			m_ptPos.x = nPosX;
			m_ptPos.y = nPosY;
			m_pParent = pParent;


			return TRUE;
		}

		void CNMListWidget::AddSortButton(CNMButton *pButton)
		{
			int xpos = 0;
			for(int i=0; i< (int) m_vec_Btns.size(); i++)
			{
				CRect rt= m_vec_Btns[i]->GetButtonRect();
				xpos += rt.Width();
				xpos += m_nSpaceBetweenButtons;

			}

			pButton->SetPosition(m_ptPos.x + xpos, m_ptPos.y);

			m_vec_Btns.push_back(pButton);

			AttachWidget( pButton->GetControlID(), pButton );
		}

		BOOL CNMListWidget::RemoveSortButton(int nButtonID)
		{
			BOOL bFound = FALSE;
			vector<CNMButton*>::iterator it = m_vec_Btns.begin();
			for(; it!= m_vec_Btns.end(); it++)
			{
				if ((*it)->GetControlID()==nButtonID)
				{
					m_vec_Btns.erase(it);
					bFound=TRUE;
					break;
				}

			}
			if (bFound==FALSE)
				return FALSE;


			RepositionButtons();
			DetachWidget( nButtonID );

			return TRUE;
		}


		CNMButton *CNMListWidget::GetSortButton(int nButtonID)
		{

			for(int i=0; i< (int) m_vec_Btns.size(); i++)
			{
				if (m_vec_Btns[i]->GetControlID()==nButtonID)
				{
					return m_vec_Btns[i];
				}	
			}
			return NULL;

		}


		void CNMListWidget::SetScrollButton(CNMButton *pUpButton, CNMButton *pDownButton, CNMThumButton *pThumButton)
		{

			m_pButtonUp = pUpButton;
			AttachWidget( m_pButtonUp->GetControlID(), m_pButtonUp );
			m_pButtonDown = pDownButton;
			AttachWidget( m_pButtonDown->GetControlID(), m_pButtonDown );
			m_pButtonThum = pThumButton;
			AttachWidget( m_pButtonThum->GetControlID(), m_pButtonThum );


			m_pButtonThum->SetThumAttribute( m_pButtonUp, m_pButtonDown, &m_rtWheel, m_vec_Units.size(), m_nColumn );

		}

		void CNMListWidget::Reposition(int nPosX, int nPosY)
		{
			m_ptPos.x = nPosX;
			m_ptPos.y = nPosY;

			RepositionButtons();
			RepositionUnit();
			RepositionScroll();

		}

		void CNMListWidget::RepositionUnit()
		{
			if (m_vec_Btns.size()==0) 
				return;

			CRect rtBtn =(*m_vec_Btns.begin())->GetButtonRect();

			int x=m_ptPos.x;
			int y=m_ptPos.y + rtBtn.Height()+m_nSpanceBetweenUnits;
			for(int i=0; i< (int)m_vec_Units.size(); i++)
			{

				if (i%m_nColumn==0 &&i!=0)
				{
					x= m_ptPos.x;

					y += m_vec_Units[i]->GetHeight();
					y += m_nSpanceBetweenUnits;		
				}

				m_vec_Units[i]->Repostion(x, y);

				x += m_vec_Units[i]->GetWidth();
				x += m_nSpanceBetweenUnits;		

			}

		}



		void CNMListWidget::RepositionButtons()
		{
			int xpos = 0;
			for(int i=0; i< (int) m_vec_Btns.size(); i++)
			{
				CRect rt= m_vec_Btns[i]->GetButtonRect();		
				m_vec_Btns[i]->SetPosition(m_ptPos.x + xpos, m_ptPos.y);
				xpos += rt.Width();
				xpos += m_nSpaceBetweenButtons;
			}



		}
		void CNMListWidget::SetUnitSize(int nRow, int nColumn)
		{

			if (m_nRow == nRow && m_nColumn == nColumn)
				return;

			m_nRow = nRow;
			m_nColumn = nColumn;


			int nTotalUnit = m_nRow * m_nColumn;
			if ((int)m_vec_Units.size()!=nTotalUnit)
			{
				if ((int)m_vec_Units.size() < nTotalUnit)
				{
					int count = nTotalUnit - m_vec_Units.size();
					//count만큼 Unit을 만든다.
					int idx=(int)m_vec_Units.size();
					for(int i=0; i<count; i++, idx++)
					{
						CNMUnitWidget *pUnitWidget = CreateNewUnitWidget(idx);			
						m_vec_Units.push_back(pUnitWidget);			
					}			
				}
				else	
				{
					int count =  (int)m_vec_Units.size() - nTotalUnit;
					//count만큼 Unit을 삭제
					for(int i=m_vec_Units.size(); i>count; i--)
					{
						DetachWidget(m_vec_Units[i]->GetControlD());

						m_vec_Units.erase(m_vec_Units.begin()+i);

					}
				}
			}


			//	RepositionButtons();
			RepositionUnit();
			RepositionScroll();

			m_pButtonThum->SetThumAttribute( m_pButtonUp, m_pButtonDown, &m_rtWheel, m_vec_Units.size(), m_nColumn );
		}


		void CNMListWidget::EnterWidget()
		{

			CNMWidgetComposite::EnterWidget();
		}
		void CNMListWidget::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
		{

			CNMWidgetComposite::DrawWidget( Page, DC );

		}
		BOOL CNMListWidget::OnMouseMoveWidget( UINT nFlags, CPoint point )
		{

			return CNMWidgetComposite::OnMouseMoveWidget( nFlags, point );
		}
		BOOL CNMListWidget::OnCommandWidget( int nCommand, WPARAM wParam, LPARAM lParam )
		{


			if (m_pButtonUp && nCommand == m_pButtonUp->GetControlID())
			{
				if( m_pButtonThum )
					m_pButtonThum->BeforePage();

			}
			else if (m_pButtonDown && nCommand == m_pButtonDown->GetControlID())
			{
				if(m_pButtonThum)
					m_pButtonThum->NextPage();
			}
			else if (m_pButtonThum && nCommand == m_pButtonThum->GetControlID())
			{
				RenewalList();	// 리스트의 포지션이 변했다 .. 리스트오브젝트 재세팅		
			}
			else if (nCommand == m_nControlID) //데이터 갯수가 변했다
			{
				OnResetDataList();	
			}


			return CNMWidgetComposite::OnCommandWidget( nCommand, wParam, lParam );
		}


		BOOL CNMListWidget::RenewalList()
		{
			if(NULL==m_pButtonThum) return FALSE;	

			return TRUE;
		}

		void  CNMListWidget::OnResetDataList()
		{
			int nTotalDataCnt = GetTotalDataCount();

			if( m_pButtonThum )
			{
				m_pButtonThum->SetElementCount(nTotalDataCnt);
				m_pButtonThum->Show((nTotalDataCnt > (m_nColumn*m_nRow)));		// 리스트 데이터가 적을 때는 스크롤바 숨기기
			}	

		}

		void CNMListWidget::SetScrollTop()
		{
			m_pButtonThum->SetCurrentPage(0);
		}

	} //namespace SKINGDI

}//namespace NMBASE