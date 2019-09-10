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
		//$$$$$$ �� �׷��� ��ũ�� �ҹ� �̹����� ��� �Ҷ� ������
		//�ҹٰ� �ִ� �پ��� �մ� ������� �Ʒ� �����εǾ� �ִ� ��ġ ��ŭ�̴�.
		//�����ϱ� ���ؼ� �κкκ� ������ �ؾ� �Ѵ�...�������� �� ���� ����.

		//���ϴ� �̹����� 1�ȼ��� ���� �Ѵ�.
		//���� �̹����� 4�ȼ��� 

		//�� ���� + �Ӹ� + �ٸ� + ���� �� �ѽ��� �ȴ� ������ ���� 


		//#define MIN_THUMB_HEIGHT_SIZE 6	//�ִ� �پ����ִ� ������

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


			//	���콺 ���������� �ѹ��� ������ �Ѵ�
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
			m_ButtonCnt = ButtonCnt; // �⺻�� 4������ ���� 
			m_bOverSoundInit = bSound;

			m_Xp = x;
			m_Yp = y;
			m_RealXp=x;
			m_RealYp=y;	//���� ó�� ���õ� ��
			m_Width = 0;
			m_Height = 0;

			// 0~ 3 �ٵ� �κ�(�ҹ�) 4~7 �ҹ��� �� �κ� 8~11 �ҹ��� �Ʒ��κ� 
			//12~15 �ҹ��� ��� �� ���κ�
			if( m_pSpr != NULL )
			{
				if(m_pSpr->spr != NULL)
				{
					m_Width = m_pSpr->spr[ m_iButtonNumber * m_ButtonCnt ].xl;
					m_Height = m_pSpr->spr[ m_iButtonNumber * m_ButtonCnt ].yl;
					m_RealWidth = m_Width;		//�̹��� ���� ������ 
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
				//	���콺 ���������� �ѹ��� ������ �Ѵ�
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
					// ��ư ���� �޼����� ����
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

			// ��ư �׸���
			if( m_SceneObject )
			{
				DWORD time = GetTickCount();
				if(m_bEnable==FALSE)
				{
					if(m_ButtonCnt <4) m_pPage->DrawScene(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 2, time );
					else m_pPage->DrawScene(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 3, time);		//	�ٿ�ȱ׸�
				}
				else if(m_bOver==TRUE && m_bClick==FALSE)	m_pPage->DrawScene(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 1, time);
				else if(m_bClick==FALSE) m_pPage->DrawScene(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 0, time);
				else if(m_bClick==TRUE) m_pPage->DrawScene(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 2, time);
			}
			else
			{
				//���̴� �׻� ������� �Ѵ�.�߰��� ���Ѵ�
				int centerpos_h = (m_Height / 2) - 2;
				if( m_Height <= 4 )
				{
					centerpos_h = 0;
				}


				if(m_bEnable==FALSE)
				{
					if(m_ButtonCnt <4) m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 2, opt, grade );
					else{
						m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 3, opt, grade);		//	�ٿ�ȱ׸�

						m_pPage->PutSprAuto(m_Xp, m_Yp-1, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 7, opt, grade);		//	�Ӹ�
						m_pPage->PutSprAuto(m_Xp, m_Yp+m_Height, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 11, opt, grade);		//	�ٸ�				

						m_pPage->PutSprAuto(m_Xp, m_Yp+centerpos_h, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 15, opt, grade);		//	����
					}
				}
				else if(m_bOver==TRUE && m_bClick==FALSE){
					m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 1, opt, grade);

					m_pPage->PutSprAuto(m_Xp, m_Yp-1, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 5, opt, grade);		//	�Ӹ�
					m_pPage->PutSprAuto(m_Xp, m_Yp+m_Height, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 9, opt, grade);		//	�ٸ�			

					m_pPage->PutSprAuto(m_Xp, m_Yp+centerpos_h, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 13, opt, grade);		//	����
				}
				else if(m_bClick==FALSE){
					m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 0, opt, grade);

					m_pPage->PutSprAuto(m_Xp, m_Yp-1, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 4, opt, grade);		//	�Ӹ�
					m_pPage->PutSprAuto(m_Xp, m_Yp+m_Height, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 8, opt, grade);		//	�ٸ�			

					m_pPage->PutSprAuto(m_Xp, m_Yp+centerpos_h, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 12, opt, grade);		//	����
				}
				else if(m_bClick==TRUE){
					m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 2, opt, grade);

					m_pPage->PutSprAuto(m_Xp, m_Yp-1, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 6, opt, grade);		//	�Ӹ�
					m_pPage->PutSprAuto(m_Xp, m_Yp+m_Height, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 10, opt, grade);		//	�ٸ�			

					m_pPage->PutSprAuto(m_Xp, m_Yp+centerpos_h, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 14, opt, grade);		//	����
				}
			}
		}

		int CGraphScrollThumbButton::GetButtonState( void )
		{
			if(m_bShow == FALSE) return -1;

			// ��ư �׸���
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

		// �پ��� ���� ������ �̹��� ���� �Ǵ� ���� ����� ���δ�
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
				//�Ʒ��� (�ٸ�)�� �־ -1 �̴�
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