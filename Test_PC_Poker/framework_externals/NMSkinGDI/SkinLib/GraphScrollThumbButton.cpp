// GraphScrollThumbButton.cpp: implementation of the CGraphScrollThumbButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphScrollThumbButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


namespace NMBASE
{
	namespace SKINGDI
	{
		//////////////////////////////////////////////////////////////////////
		// Construction/Destruction
		//////////////////////////////////////////////////////////////////////
		//$$$$$$ 이 그래픽 스크롤 텀바 이미지를 사용 할때 주위점
		//텀바가 최대 줄어들수 잇는 사이즈는 아래 디파인되어 있는 수치 만큼이다.
		//변경하기 위해선 부분부분 수정을 해야 한다...귀찮으면 걍 쓰지 마셈.

		//상하단 이미지는 1픽셀로 정의 한다.
		//무늬 이미지는 4픽셀로 

		//총 몸통 + 머리 + 다리 + 무늬 가 한쌍이 된다 기준은 몸통 


		//#define MIN_THUMB_HEIGHT_SIZE 6	//최대 줄어들수있는 사이즈

		CGraphScrollThumbButton::CGraphScrollThumbButton()
		{
			m_pParent = NULL;
			m_pPage = NULL;
			m_pSpr = NULL;
			m_CmndID = 0;
			m_bShow = FALSE;
			m_bEnable = TRUE;
			m_Xp = m_Yp = 0;
			m_Width = 0;
			m_Height = 0;
			m_bClick = FALSE;
			m_bOver = FALSE;
			m_iButtonNumber = 0;


			//	마우스 오버됐을때 한번만 나오게 한다
			m_bOverSoundInit = m_bOverSound = false;
			m_SoundNum = 0;


			m_ButtonCnt = 0;
			m_SceneObject = FALSE;
		}

		CGraphScrollThumbButton::~CGraphScrollThumbButton()
		{
		}

		void CGraphScrollThumbButton::Init(CWnd *pWnd, NMBASE::GRAPHICGDI::CPage *ppage, int x, int y, NMBASE::GRAPHICGDI::xSprite *pspr, int iButtonNumber, int cmndid, int ButtonCnt,bool scenebtn, bool bSound )
		{
			m_pParent = pWnd;
			m_pPage = ppage;
			m_pSpr = pspr;
			m_CmndID = cmndid;
			m_bEnable = m_bShow = TRUE;
			m_bClick = FALSE;
			m_bOver = FALSE;

			m_iButtonNumber = iButtonNumber;
			m_ButtonCnt = ButtonCnt; // 기본은 4장으로 구성 
			m_bOverSoundInit = bSound;

			m_Xp = x;
			m_Yp = y;
			m_RealXp=x;
			m_RealYp=y;	//실제 처음 셋팅된 값
			m_Width = 0;
			m_Height = 0;

			// 0~ 3 바디 부분(텀버) 4~7 텀버의 윗 부분 8~11 텀버의 아랫부분 
			//12~15 텀버의 가운데 들어갈 모양부분
			if( m_pSpr != NULL )
			{
				if(m_pSpr->spr != NULL)
				{
					m_Width = m_pSpr->spr[ m_iButtonNumber * m_ButtonCnt ].xl;
					m_Height = m_pSpr->spr[ m_iButtonNumber * m_ButtonCnt ].yl;
					m_RealWidth = m_Width;		//이미지 원본 사이즈 
					m_RealHeight = m_Height;
				}

				if( m_pSpr->scene.GetScenePt(0) != NULL && scenebtn == TRUE)
				{
					if( m_pSpr->scene.TotSceneNum >= 2 )
						m_SceneObject = TRUE;
				}
			}

			InitBtn();
		}

		void CGraphScrollThumbButton::SetSprite(NMBASE::GRAPHICGDI::xSprite *pspr, int iButtonNumber)
		{
			m_iButtonNumber = iButtonNumber;
			m_pSpr = pspr;
			if(m_pSpr->spr != NULL)
			{
				m_Width = m_pSpr->spr[ m_iButtonNumber * 3 ].xl;
				m_Height = m_pSpr->spr[ m_iButtonNumber * 3 ].yl;
			}
		}
		void CGraphScrollThumbButton::SetButtonPos(int x, int y)
		{
			m_Xp = x;
			m_Yp = y;
		}
		void CGraphScrollThumbButton::SetButtonPos(int x, int y, NMBASE::GRAPHICGDI::xSprite *pspr)
		{
			m_Xp = x;
			m_Yp = y;
			m_Width = 0;
			m_Height = 0;
			if(pspr != NULL && pspr->spr != NULL)
			{
				m_Width = pspr->spr[0].xl;
				m_Height = pspr->spr[0].yl;
			}
		}
		void CGraphScrollThumbButton::Enable(BOOL bVal)
		{
			if(bVal==FALSE)
			{
				m_bClick = FALSE;
				m_bOver = FALSE;
			}
			m_bEnable = bVal;
		}

		void CGraphScrollThumbButton::Show(BOOL bVal)
		{
			CGraphScrollThumbButton::Enable( bVal );

			m_bShow = bVal;
		}

		BOOL CGraphScrollThumbButton::OnMouseMove(int x, int y)
		{
			if(m_bShow == FALSE) return FALSE;
			if(m_bEnable==FALSE) return FALSE;

			if(x >= m_Xp && x <= m_Xp+m_Width && y >= m_Yp && y <= m_Yp+m_Height)
			{	
				m_bOver = TRUE;
				//	마우스 오버됐을때 한번만 나오게 한다
				if( !m_bOverSound && m_bOverSoundInit )
				{	
					m_bOverSound = true;
					//			g_Sound.PlayWav( (SND_ENUM) m_SoundNum, false, m_SoundVol );
				}
				return TRUE;
			}
			else
			{

				m_bOverSound = false;
				m_bOver = FALSE;
				return FALSE;
			}
		}

		BOOL CGraphScrollThumbButton::OnLButtonDown(int x, int y)
		{
			if(m_bShow == FALSE) return FALSE;
			if(m_bEnable==FALSE) return FALSE;

			if(x >= m_Xp && x <= m_Xp+m_Width && y >= m_Yp && y <= m_Yp+m_Height)
			{
				m_bClick = TRUE;
			}
			else { m_bClick = FALSE; return FALSE; }
			return TRUE;
		}

		BOOL CGraphScrollThumbButton::OnLButtonUp(int x, int y)
		{
			if(m_bShow == FALSE) return FALSE;
			if(m_bEnable==FALSE) return FALSE;
			if(m_bClick)
			{
				m_bClick = FALSE;
				if(x >= m_Xp && x <= m_Xp+m_Width && y >= m_Yp && y <= m_Yp+m_Height)
				{
					// 버튼 눌림 메세지를 보냄
					m_pParent->SendMessage(WM_COMMAND, m_CmndID, NULL);
					return TRUE;
				}
			}
			m_bClick = FALSE;
			return FALSE;
		}

		void CGraphScrollThumbButton::Draw(CDC *pDC, int opt, int grade )
		{
			if(m_pPage == NULL) return;
			if(m_pSpr == NULL) return;
			if(m_bShow == FALSE) return;

			// 버튼 그리기
			if( m_SceneObject )
			{
				DWORD time = GetTickCount();
				if(m_bEnable==FALSE)
				{
					if(m_ButtonCnt <4) m_pPage->DrawScene(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 2, time );
					else m_pPage->DrawScene(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 3, time);		//	다운된그림
				}
				else if(m_bOver==TRUE && m_bClick==FALSE)	m_pPage->DrawScene(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 1, time);
				else if(m_bClick==FALSE) m_pPage->DrawScene(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 0, time);
				else if(m_bClick==TRUE) m_pPage->DrawScene(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 2, time);
			}
			else
			{
				//무늬는 항상 가운데들어가야 한다.중간을 구한다
				int centerpos_h = (m_Height / 2) - 2;
				if( m_Height <= 4 )
				{
					centerpos_h = 0;
				}


				if(m_bEnable==FALSE)
				{
					if(m_ButtonCnt <4) m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 2, opt, grade );
					else{
						m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 3, opt, grade);		//	다운된그림

						m_pPage->PutSprAuto(m_Xp, m_Yp-1, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 7, opt, grade);		//	머리
						m_pPage->PutSprAuto(m_Xp, m_Yp+m_Height, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 11, opt, grade);		//	다리				

						m_pPage->PutSprAuto(m_Xp, m_Yp+centerpos_h, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 15, opt, grade);		//	무늬
					}
				}
				else if(m_bOver==TRUE && m_bClick==FALSE){
					m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 1, opt, grade);

					m_pPage->PutSprAuto(m_Xp, m_Yp-1, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 5, opt, grade);		//	머리
					m_pPage->PutSprAuto(m_Xp, m_Yp+m_Height, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 9, opt, grade);		//	다리			

					m_pPage->PutSprAuto(m_Xp, m_Yp+centerpos_h, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 13, opt, grade);		//	무늬
				}
				else if(m_bClick==FALSE){
					m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 0, opt, grade);

					m_pPage->PutSprAuto(m_Xp, m_Yp-1, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 4, opt, grade);		//	머리
					m_pPage->PutSprAuto(m_Xp, m_Yp+m_Height, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 8, opt, grade);		//	다리			

					m_pPage->PutSprAuto(m_Xp, m_Yp+centerpos_h, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 12, opt, grade);		//	무늬
				}
				else if(m_bClick==TRUE){
					m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 2, opt, grade);

					m_pPage->PutSprAuto(m_Xp, m_Yp-1, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 6, opt, grade);		//	머리
					m_pPage->PutSprAuto(m_Xp, m_Yp+m_Height, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 10, opt, grade);		//	다리			

					m_pPage->PutSprAuto(m_Xp, m_Yp+centerpos_h, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 14, opt, grade);		//	무늬
				}
			}
		}

		int CGraphScrollThumbButton::GetButtonState( void )
		{
			if(m_bShow == FALSE) return -1;

			// 버튼 그리기
			if( m_bOver && !m_bClick )		return 1;
			else if( !m_bClick )			return 0;
			else if( m_bClick )				return 2;

			return -1;
		}
		void CGraphScrollThumbButton::SetButtonState( int state )
		{
			if( state == 1)
			{
				m_bOver =TRUE ;
				m_bClick = FALSE;
			}
			else if( state == 0)
			{
				m_bOver  = FALSE;
				m_bClick = FALSE;
			}
			else if( state == 2)
			{
				m_bOver  = FALSE;
				m_bClick = TRUE;
			}
		}
		void CGraphScrollThumbButton::SetMouseOverSound( int snd, int vol )
		{
			m_SoundNum = snd;
			m_SoundVol = vol;
			m_bOverSoundInit = true;
			return;
		}

		void CGraphScrollThumbButton::MouseOverSoundNoUse( void )
		{
			m_bOverSoundInit = false;
			return;
		}

		POINT CGraphScrollThumbButton::GetButtonPoint( void )
		{
			return CPoint( m_Xp, m_Yp );
		}

		// 줄어드는 길이 값으로 이미지 가로 또는 세로 사이즈를 줄인다
		BOOL CGraphScrollThumbButton::SetChangeScrollThumbSize( int w_shorten , int h_shorten )
		{
			if(m_pSpr->spr == NULL)return FALSE;

			if( w_shorten != 0 ){

				m_Width += w_shorten;
				m_pSpr->spr[0].xl = m_Width;
				m_pSpr->spr[1].xl = m_Width;
				m_pSpr->spr[2].xl = m_Width;
				m_pSpr->spr[3].xl = m_Width;

				m_Xp += w_shorten;
			}

			if( h_shorten != 0)
			{
				int t = m_Height + h_shorten;
				//아래로 (다리)가 있어서 -1 이다
				if( t <= MIN_THUMB_HEIGHT_SIZE - 1) 
				{
					return FALSE ;
				}
				m_Height += h_shorten;
				m_pSpr->spr[0].yl = m_Height;
				m_pSpr->spr[1].yl = m_Height;
				m_pSpr->spr[2].yl = m_Height;
				m_pSpr->spr[3].yl = m_Height;

				m_Yp -= h_shorten;
			}

			return TRUE;
		}

		void CGraphScrollThumbButton::ReSetScrollThumSize()
		{
			if(m_pSpr->spr == NULL)return;	

			m_Width = m_RealWidth;
			m_pSpr->spr[0].xl = m_Width;
			m_pSpr->spr[1].xl = m_Width;
			m_pSpr->spr[2].xl = m_Width;
			m_pSpr->spr[3].xl = m_Width;

			m_Xp = m_RealXp;


			m_Height = m_RealHeight;
			m_pSpr->spr[0].yl = m_Height;
			m_pSpr->spr[1].yl = m_Height;
			m_pSpr->spr[2].yl = m_Height;
			m_pSpr->spr[3].yl = m_Height;

			m_Yp = m_RealYp;

		}

	} //namespace SKINGDI

}//namespace NMBASE