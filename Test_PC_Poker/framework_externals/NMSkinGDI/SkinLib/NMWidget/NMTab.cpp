#include "StdAfx.h"
#include "./NMWidget/NMTab.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		CNMTab::CNMTab(void)
		{

			m_pWidgetState=NULL;
			m_nStateWidgetID=0;
			m_nControlID = 0;
		}

		CNMTab::~CNMTab(void)
		{

			m_vec_TabInfo.clear();

		}


		BOOL CNMTab::Initialize(int nControlID, int nStateWidgetID)
		{
			m_nControlID= nControlID;
			m_nStateWidgetID = nStateWidgetID;

			m_pWidgetState = new CNMWidgetState();
			AttachWidget(m_nStateWidgetID, m_pWidgetState);

			return TRUE;
		}


		BOOL CNMTab::CreateWidget( CWnd* pParent )
		{

			// 	ASSERT (m_nStateWidgetID>0);	//에러 : CreateWidget 되기전에 Initialize 호출해야함	
			// 	
			// 	for (int i =0; i < (int)m_vec_TabInfo.size(); i++)
			// 	{
			// 		AttachWidget(m_vec_TabInfo[i].pTabButton->GetControlID(), m_vec_TabInfo[i].pTabButton);
			// 		m_pWidgetState->AttachWidget(m_vec_TabInfo[i].nViewWidgetID, m_vec_TabInfo[i].pViewWidget);				
			// 	}

			ASSERT(m_pWidgetState); //에러 CNMTab::Initialize 가 호출 되지 않았음.



			return CNMWidgetComposite::CreateWidget( pParent );
		}

		BOOL CNMTab::OnCommandWidget( int nCommand, WPARAM wParam, LPARAM lParam )
		{

			for (int i =0; i < (int)m_vec_TabInfo.size(); i++)
			{
				if (m_vec_TabInfo[i].pTabButton->GetControlID()==nCommand)
				{
					//change tab
					ChangeTab(m_vec_TabInfo[i].nViewWidgetID);				
					break;
				}		
			}


			return CNMWidgetComposite::OnCommandWidget(nCommand, wParam, lParam );
		}


		BOOL CNMTab::AddTab(int nViewWidgetID, INMWidget* pViewWidget, CNMButton *pTabButton)
		{
			TABINFO ti;
			ti.nViewWidgetID = nViewWidgetID;
			ti.pViewWidget	= pViewWidget;
			ti.pTabButton	= pTabButton;
			m_vec_TabInfo.push_back(ti);

			AttachWidget(pTabButton->GetControlID(), pTabButton);
			m_pWidgetState->AttachWidget(nViewWidgetID, pViewWidget);		

			return TRUE;
		}

		int CNMTab::GetCurTab()
		{
			if (m_pWidgetState==NULL)
				return 0;

			return m_pWidgetState->GetCurrentState();
		}

		BOOL CNMTab::ChangeTab(int nViewWidgetID)
		{
			if (m_pWidgetState->ChangeWidget(nViewWidgetID))
			{
				for (int i =0; i < (int)m_vec_TabInfo.size(); i++)
				{
					if (m_vec_TabInfo[i].nViewWidgetID==nViewWidgetID)
					{
						m_vec_TabInfo[i].pTabButton->SetEnable(FALSE);
					}	
					else
					{
						m_vec_TabInfo[i].pTabButton->SetEnable(TRUE);
					}
				}
				return TRUE;
			}	

			return FALSE;

		}


		INMWidget* CNMTab::GetWidget( int nWidgetKey )
		{


			INMWidget *pWidget = m_pWidgetState->GetWidget(nWidgetKey);
			if (pWidget)
				return pWidget;

			return CNMWidgetComposite::GetWidget(nWidgetKey);

		}

		void CNMTab::SetRelationPosition( int nOffsetX, int nOffsetY )
		{
			for (int i =0; i < (int)m_vec_TabInfo.size(); i++)
			{
				m_vec_TabInfo[i].pTabButton->SetRelationPosition(nOffsetX, nOffsetY);		
			}

		}


	} //namespace SKINGDI

}//namespace NMBASE