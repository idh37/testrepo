#include "StdAfx.h"
#include "./NMWidget/NMUnitWdget.h"
#include "./NMWidget/NMResourceManager.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		//////////////////////////////////////////////////////////////////////////////
		// CUnitWdget
		CNMUnitWidget::CNMUnitWidget()
		{
			m_nControlID = WIDGET_ID_NULL;
			m_pParent = NULL;
			m_rectPosition.SetRect(0,0,0,0);
			m_rectTargetPosition = m_rectPosition;

			m_bUse=FALSE;

			m_bOver = FALSE;
			m_dwOverTick = 0;
			m_bDown = FALSE;
			m_dwDownTick = 0;

			m_strOverSnd = "";
			m_strDownSnd = "";


			m_rectOverLayer.SetRect(0,0,0,0);
			m_rectOverLimit.SetRect(0,0,0,0);

			m_dwSetInTick = 0;

			m_pSprUnit=NULL;
		}
		CNMUnitWidget::~CNMUnitWidget()
		{

		}

		int CNMUnitWidget::GetHeight()
		{

			return m_rectPosition.Height();
		}
		int CNMUnitWidget::GetWidth()
		{

			return m_rectPosition.Width();
		}

		BOOL CNMUnitWidget::Initialize( int nControlID, CWnd* pParent, NMBASE::GRAPHICGDI::xSprite *pSpr, int nPosX, int nPosY )
		{
			if (pSpr==NULL) 
				return FALSE;

			if (pSpr->GetTotalSpr()==0)
				return FALSE;

			m_nControlID = nControlID;
			m_pParent = pParent;
			m_pSprUnit = pSpr;


			m_rectPosition.SetRect( nPosX, nPosY, nPosX + pSpr->spr[0].xl, nPosY + pSpr->spr[0].yl );
			m_rectTargetPosition = m_rectPosition;

			return TRUE;
		}

		void CNMUnitWidget::Repostion(int nPosX, int nPosY)
		{
			int offset_x = nPosX - m_rectPosition.left;
			int offset_y = nPosY - m_rectPosition.top;

			m_rectPosition.OffsetRect(offset_x, offset_y);
			m_rectTargetPosition.OffsetRect(offset_x, offset_y);

		}

		void CNMUnitWidget::SetSound( char* szOver, char* szDown )
		{
			m_strOverSnd = szOver;
			m_strDownSnd = szDown;
		}

		void CNMUnitWidget::SetOverLayerRect( CRect rectOverLayer, CRect rectOverLimit )
		{
			m_rectOverLayer = rectOverLayer;
			m_rectOverLimit = rectOverLimit;
		}

		void CNMUnitWidget::UpdateWidget( DWORD dwCurTick )
		{

		}

		void CNMUnitWidget::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
		{

		}

		void CNMUnitWidget::DrawOverLayer( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
		{

		}


		BOOL CNMUnitWidget::OnMouseMoveWidget( UINT nFlags, CPoint point )
		{
			if( m_rectPosition.PtInRect( point ) )
			{
				if( m_bOver == FALSE )
				{
					if( m_strOverSnd.length() )
					{
						NMRESAM.PlayWav( m_strOverSnd );
					}
					m_bOver = TRUE;
					m_dwOverTick = GetTickCount();
				}
			}
			else
			{
				m_bOver = FALSE;
				m_dwOverTick = 0;
			}
			return FALSE;
		}

		BOOL CNMUnitWidget::OnMouseLeaveWidget( UINT nFlags, CPoint point )
		{
			m_bOver = FALSE;
			m_dwOverTick = 0;

			return FALSE;
		}

		BOOL CNMUnitWidget::OnLButtonDownWidget( UINT nFlags, CPoint point )
		{

			return FALSE;
		}
		BOOL CNMUnitWidget::OnLButtonUpWidget( UINT nFlags, CPoint point )
		{


			return FALSE;
		}

		BOOL CNMUnitWidget::OnLButtonDbClkWidget( UINT nFlags, CPoint point )
		{

			return FALSE;
		}



		void CNMUnitWidget::SetSetInTick( DWORD dwdelay )
		{
			m_dwSetInTick = GetTickCount() + dwdelay;
		}

		RECT *CNMUnitWidget::GetRect()
		{
			return m_rectPosition;
		}

	} //namespace SKINGDI

}//namespace NMBASE