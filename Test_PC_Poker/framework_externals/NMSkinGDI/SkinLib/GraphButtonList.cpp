// GraphButtonList.cpp: implementation of the CGraphButtonList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphButtonList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



namespace NMBASE
{
	namespace SKINGDI
	{

		HCURSOR CGraphButtonList::m_Normal;// = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_SELECT));
		HCURSOR CGraphButtonList::m_Select;// = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_SELECTCLICK));
		BOOL CGraphButtonList::m_bUseCursor = FALSE;
		//////////////////////////////////////////////////////////////////////
		// Construction/Destruction
		//////////////////////////////////////////////////////////////////////

		CGraphButtonList::CGraphButtonList()
		{
			m_Btn.clear();
		}

		CGraphButtonList::~CGraphButtonList()
		{
			m_Btn.clear();
		}
		void CGraphButtonList::Init()
		{
			m_Btn.clear();
		}

		void CGraphButtonList::SetButtonCursor( UINT hNormalCursorID , UINT hSelectCursorID )
		{
			m_Normal  = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(hNormalCursorID));
			m_Select  = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(hSelectCursorID));
			m_bUseCursor = TRUE;
		}

		void CGraphButtonList::Add(CGraphButton * pBtn)
		{
			if( pBtn->GetSprite()->scene.GetScenePt(0) != NULL && pBtn->m_Xp == 0 )
			{		
				if( pBtn->GetSprite()->scene.TotSceneNum < 2 ){
					NMBASE::GRAPHICGDI::CellData * pt = pBtn->GetSprite()->scene.GetScenePt(0)->GetPtrFrame(0)->GetPtrCell(0);
					pBtn->MoveWindow( pt->Rx, pt->Ry );
				}
			}
			CGraphButton * temp = m_Btn[pBtn->m_CmndID];
			if( temp != NULL)
			{
				//이미 있는아이디가 들어온경우 즉 윈도우 종료는 안됐는데 다시 초기화될경우 들어올수 있다.
				//new 한데이터인지 아닌지 모르므로 함부로 지우면 안될꺼 같으다
				delete pBtn;
			}
			m_Btn[pBtn->m_CmndID] = pBtn;
		}
		void CGraphButtonList::Add(CGraphButton * pBtn,CWnd *pWnd, NMBASE::GRAPHICGDI::CPage *ppage, int x, int y, NMBASE::GRAPHICGDI::xSprite *pspr, int iButtonNumber, int cmndid, int ButtonCnt, bool bSound)
		{
			if( pBtn == NULL)
			{
				AfxMessageBox("버튼에 널포인트");
				return;
			}
			pBtn->Init( pWnd, ppage,  x,  y, pspr,  iButtonNumber,  cmndid,  ButtonCnt,  bSound );
			Add(pBtn);
		}


		BOOL CGraphButtonList::OnLButtonDown(int mx , int  my )
		{
			for(BtnList::iterator i= m_Btn.begin(); i != m_Btn.end(); ++i )
			{
				if( (*(i->second)).OnLButtonDown(  mx , my  ) == TRUE )
				{
					if( m_bUseCursor ) SetCursor( m_Select );
					return TRUE;
				}
			}

			return FALSE;
		}
		BOOL CGraphButtonList::OnLButtonUp(int mx , int  my )
		{
			for(BtnList::iterator i= m_Btn.begin(); i != m_Btn.end(); ++i )
			{
				if( (*(i->second)).OnLButtonUp(  mx , my  ) == TRUE ) 
				{
					if( m_bUseCursor ) SetCursor( m_Normal );
					return TRUE;
				}
			}
			return FALSE;
		}
		BOOL CGraphButtonList::OnMouseMove(int mx , int  my )
		{
			BOOL re = FALSE;
			for(BtnList::iterator i= m_Btn.begin(); i != m_Btn.end(); ++i )
			{
				if( TRUE ==(*(i->second)).OnMouseMove( mx , my ) )
				{
					if( m_bUseCursor ) 
					{
						if( (*(i->second)).m_bClick == TRUE )
						{
							SetCursor( m_Select );			
						}
						else
						{
							SetCursor( m_Normal );			
						}
					}
					re = TRUE;	
				}
			}
			return re;
		}
		void CGraphButtonList::Invalidate()
		{
			for(BtnList::iterator i= m_Btn.begin(); i != m_Btn.end(); ++i )
			{
				CRect rc((*(i->second)).m_Xp,
					(*(i->second)).m_Yp,
					(*(i->second)).m_Xp +(*(i->second)).m_Width,
					(*(i->second)).m_Yp +(*(i->second)).m_Height);
				(*(i->second)).GetParent()->InvalidateRect(&rc,FALSE);
			}
		}
		void CGraphButtonList::Draw( CDC* pDC )
		{
			for(BtnList::iterator i= m_Btn.begin(); i != m_Btn.end(); ++i )
			{
				(*(i->second)).Draw( pDC );
			}
			for(BtnList::iterator i2= m_Btn.begin(); i2 != m_Btn.end(); ++i2 )
			{
				(*(i2->second)).DrawToolTip( pDC );
			}

		}
		void CGraphButtonList::ShowWindow( int cmdid , BOOL bShow )
		{
			m_Btn[cmdid]->Show( bShow );
		}
		void CGraphButtonList::Enable( int cmdid , BOOL bVal)
		{
			m_Btn[cmdid]->Enable( bVal );
		}
		CGraphButton* CGraphButtonList::GetButton( int cmdid )
		{
			return m_Btn[cmdid];
		}

		void CGraphButtonList::AllEnable(BOOL bshow)
		{
			for(BtnList::iterator i2= m_Btn.begin(); i2 != m_Btn.end(); ++i2 )
			{
				(*(i2->second)).Enable(bshow);
				(*(i2->second)).Show(bshow);
			}
		}


		//////////////////////////////////////////////////////////////////////
		// Construction/Destruction
		//////////////////////////////////////////////////////////////////////

		CGraphButtonListEx::CGraphButtonListEx()
		{
			m_Btn.clear();
		}

		CGraphButtonListEx::~CGraphButtonListEx()
		{
			m_Btn.clear();
		}
		void CGraphButtonListEx::Init()
		{
			m_Btn.clear();
		}


		void CGraphButtonListEx::Add(CGraphButtonEx * pBtn)
		{
			if( pBtn->GetSprite()->scene.GetScenePt(0) != NULL && pBtn->m_Xp == 0 )
			{		
				if( pBtn->GetSprite()->scene.TotSceneNum < 2 )
				{
					NMBASE::GRAPHICGDI::CellData * pt = pBtn->GetSprite()->scene.GetScenePt(0)->GetPtrFrame(0)->GetPtrCell(0);
					pBtn->MoveWindow( pt->Rx, pt->Ry );
				}
			}
			CGraphButtonEx * temp = m_Btn[pBtn->m_CmndID];
			if( temp != NULL)
			{
				//이미 있는아이디가 들어온경우 즉 윈도우 종료는 안됐는데 다시 초기화될경우 들어올수 있다.
				//new 한데이터인지 아닌지 모르므로 함부로 지우면 안될꺼 같으다
				delete pBtn;
			}
			m_Btn[pBtn->m_CmndID] = pBtn;
		}
		void CGraphButtonListEx::Add(CGraphButtonEx * pBtn,CWnd *pWnd, NMBASE::GRAPHICGDI::CPage *ppage, int x, int y, NMBASE::GRAPHICGDI::xSprite *pspr, int iButtonNumber, int cmndid, int ButtonCnt, bool bSound)
		{
			if( pBtn == NULL)
			{
				AfxMessageBox("버튼에 널포인트");
				return;
			}
			pBtn->Init( pWnd, ppage,  x,  y, pspr,  iButtonNumber,  cmndid,  ButtonCnt,  bSound );
			Add(pBtn);
		}


		BOOL CGraphButtonListEx::OnLButtonDown(int mx , int  my )
		{
			for(BtnListEx::iterator i= m_Btn.begin(); i != m_Btn.end(); ++i )
			{
				if( (*(i->second)).OnLButtonDown(  mx , my  ) == TRUE )
				{			
					return TRUE;
				}
			}

			return FALSE;
		}
		BOOL CGraphButtonListEx::OnLButtonUp(int mx , int  my )
		{
			for(BtnListEx::iterator i= m_Btn.begin(); i != m_Btn.end(); ++i )
			{
				if( (*(i->second)).OnLButtonUp(  mx , my  ) == TRUE ) 
				{			
					return TRUE;
				}
			}
			return FALSE;
		}
		BOOL CGraphButtonListEx::OnMouseMove(int mx , int  my )
		{
			BOOL re = FALSE;
			for(BtnListEx::iterator i= m_Btn.begin(); i != m_Btn.end(); ++i )
			{
				if( TRUE ==(*(i->second)).OnMouseMove( mx , my ) )
				{			
					re = TRUE;	
				}
			}
			return re;
		}
		void CGraphButtonListEx::Invalidate()
		{
			for(BtnListEx::iterator i= m_Btn.begin(); i != m_Btn.end(); ++i )
			{
				CRect rc((*(i->second)).m_Xp,
					(*(i->second)).m_Yp,
					(*(i->second)).m_Xp +(*(i->second)).m_Width,
					(*(i->second)).m_Yp +(*(i->second)).m_Height);
				(*(i->second)).GetParent()->InvalidateRect(&rc,FALSE);
			}
		}
		void CGraphButtonListEx::Draw( CDC* pDC )
		{
			for(BtnListEx::iterator i= m_Btn.begin(); i != m_Btn.end(); ++i )
			{
				(*(i->second)).Draw( pDC );
			}
			for(BtnListEx::iterator i2= m_Btn.begin(); i2 != m_Btn.end(); ++i2 )
			{
				(*(i2->second)).DrawToolTip( pDC );
			}

		}
		void CGraphButtonListEx::ShowWindow( int cmdid , BOOL bShow )
		{
			m_Btn[cmdid]->Show( bShow );
		}
		void CGraphButtonListEx::Enable( int cmdid , BOOL bVal)
		{
			m_Btn[cmdid]->Enable( bVal );
		}
		CGraphButtonEx* CGraphButtonListEx::GetButton( int cmdid )
		{
			return m_Btn[cmdid];
		}

		void CGraphButtonListEx::AllEnable(BOOL bshow)
		{
			for(BtnListEx::iterator i2= m_Btn.begin(); i2 != m_Btn.end(); ++i2 )
			{
				(*(i2->second)).Enable(bshow);
				(*(i2->second)).Show(bshow);
			}
		}


	} //namespace SKINGDI

}//namespace NMBASE