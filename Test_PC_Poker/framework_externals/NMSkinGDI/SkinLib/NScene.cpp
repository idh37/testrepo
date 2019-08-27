// NScene.cpp: implementation of the CNScene class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NScene.h"

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

		CNScene::CNScene()
		{
			m_pPage = NULL;
			m_Xp = 0;;
			m_Yp = 0;;

			m_pSpr = NULL;

			m_nSceneNum = 0;		
			ZeroMemory(&m_stSceneState, sizeof(SCENE_STATUS));	
			m_pPage = NULL;
		}

		CNScene::CNScene(int x, int y, NMBASE::GRAPHICGDI::CPage* pPage, NMBASE::GRAPHICGDI::xSprite* pSpr, int nSceneNum)
		{
			ZeroMemory(&m_stSceneState, sizeof(SCENE_STATUS));

			m_nSceneNum = nSceneNum;
			m_pPage = pPage;
			m_stSceneState.nSceneTime = 0;
			m_stSceneState.nLoop = 0;
			SetSpriteFile(pSpr);	
			SetPostion(x,y);

		}

		CNScene::~CNScene()
		{
		}

		void CNScene::SetPostion(int x, int y )
		{
			m_Xp = x ;
			m_Yp = y ;
		}

		void CNScene::StartScene(int nLoop, DWORD deleytime)
		{
			if(m_pSpr == NULL)
				return;	

			m_stSceneState.nLoop = nLoop;
			m_stSceneState.nSceneTime = m_pSpr->GetScenePlayTime(m_nSceneNum);
			m_stSceneState.nStartSceneTime = timeGetTime();
			m_stSceneState.bScenePlay = TRUE;
			m_stSceneState.bLoop = FALSE;
			if( deleytime > 0 ){
				m_stSceneState.nDeleyTime = deleytime;
			}

		}

		void CNScene::ChangeSceneNum( int index )
		{
			if( index > -1 ){
				m_nSceneNum = index;
			}
		}

		void CNScene::StartSceneLoop()
		{
			if(m_pSpr == NULL)
				return;	
			m_stSceneState.bLoop = TRUE;
		}

		void CNScene::SetSpriteFile(NMBASE::GRAPHICGDI::xSprite* pSpr)
		{
			if(pSpr == NULL )return;
			m_pSpr = pSpr;	 
		}

		void CNScene::StopScene()
		{
			m_stSceneState.bScenePlay = FALSE;
			m_stSceneState.nCurPlayTime = 0;
		}


		void CNScene::OnTimer()
		{
			if( m_pSpr == NULL )return;
			if( m_stSceneState.bScenePlay == FALSE )return;
			if( m_stSceneState.nStartSceneTime <=0 )return;

			DWORD curtime = timeGetTime();

			if( m_stSceneState.nDeleyTime > 0 ){

				DWORD deleytime = curtime - m_stSceneState.nStartSceneTime;

				if( deleytime >= m_stSceneState.nDeleyTime){
					m_stSceneState.nDeleyTime = 0;
					m_stSceneState.nStartSceneTime = curtime;			
				}

				return;
			}

			m_stSceneState.nCurPlayTime = curtime - m_stSceneState.nStartSceneTime;

			if(m_stSceneState.bLoop == FALSE) {
				if(m_stSceneState.nCurPlayTime >= m_stSceneState.nSceneTime * m_stSceneState.nLoop) {
					StopScene();
				}
			}
		}

		BOOL CNScene::DrawScene(CDC *pDC)
		{
			if(m_stSceneState.bScenePlay == FALSE)
				return TRUE;	

			m_pPage->DrawScene(m_Xp, m_Yp, m_pSpr, m_nSceneNum, m_stSceneState.nCurPlayTime, TRUE);

			return TRUE;
		}

	} //namespace SKINGDI

}//namespace NMBASE