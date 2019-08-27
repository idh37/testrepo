// NMultiScene.cpp: implementation of the CNMultiScene class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NMultiScene.h"

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
		CNMultiScene::CNMultiScene()
		{
			m_nSceneNum = 0;		
			ZeroMemory(&m_stSceneState, sizeof(SCENE_STATUS));	
			ZeroMemory(&scrquery, sizeof(scrquery));	

			m_pPage = NULL;
			m_pSpr = NULL;
			m_pParent = NULL;

			scrquery.Reset();
		}

		CNMultiScene::CNMultiScene(int x, int y,void *pParent, NMBASE::GRAPHICGDI::CPage* pPage, NMBASE::GRAPHICGDI::xSprite* pSpr, int nSceneNum)
		{
			ZeroMemory(&m_stSceneState, sizeof(SCENE_STATUS));

			bShow = TRUE;
			m_pPage = pPage;
			m_stSceneState.nSceneTime = 0;
			m_stSceneState.nLoop = 0;	
			m_nSceneNum = nSceneNum;	
			SetPostion(x,y);	
			SetSpriteFile(pSpr);
		}

		CNMultiScene::~CNMultiScene()
		{
		}

		void CNMultiScene::SetPostion(int x, int y )
		{
			m_Xp = x ;
			m_Yp = y ;
		}

		void CNMultiScene::StartMScene(int nLoop)
		{
			if(m_pSpr == NULL)
				return;

			m_stSceneState.nLoop = nLoop;
			m_stSceneState.nSceneTime = m_pSpr->GetMScenePlayTime(m_nSceneNum);
			m_stSceneState.nStartSceneTime = timeGetTime();
			m_stSceneState.bScenePlay = TRUE;
			m_stSceneState.bLoop = FALSE;
			m_stSceneState.nCurPlayTime = timeGetTime() - m_stSceneState.nStartSceneTime;
		}

		void CNMultiScene::SetDrawIndex( int index )
		{
			if ( m_pSpr == NULL )
			{
				return;
			}

			int total = m_pSpr->multiscene.TotMSceneNum; //m_pSpr->GetTotalSpr();
			if( index < 0 || index > total )return;

			m_nSceneNum = index;
			m_stSceneState.nLoop = 1;
			m_stSceneState.nSceneTime = m_pSpr->GetMScenePlayTime(m_nSceneNum);
			m_stSceneState.nStartSceneTime = timeGetTime()-270;
			m_stSceneState.bScenePlay = TRUE;
			m_stSceneState.bLoop = FALSE;
			m_stSceneState.nCurPlayTime = timeGetTime() - m_stSceneState.nStartSceneTime;
		}

		void CNMultiScene::StartMScene(int x, int y, int nLoop)
		{
			if(m_pSpr == NULL)
				return;

			SetPostion(x,y);

			StartMScene(nLoop);
		}

		void CNMultiScene::StartMSceneLoop(int x, int y)
		{
			if(m_pSpr == NULL)
				return;

			SetPostion(x, y);

			StartMScene(0);
			m_stSceneState.bLoop = TRUE;
		}

		void CNMultiScene::StartMSceneLoop()
		{
			if(m_pSpr == NULL)
				return;	

			StartMScene(0);
			m_stSceneState.bLoop = TRUE;
		}

		void CNMultiScene::SetSpriteFile(NMBASE::GRAPHICGDI::xSprite* pSpr)
		{
			if(pSpr == NULL )return;
			m_pSpr = pSpr;	 
		}

		void CNMultiScene::StopMScene(DWORD StopTime)
		{
			m_stSceneState.bScenePlay = FALSE;
			m_stSceneState.nCurPlayTime = StopTime;
		}


		NMBASE::GRAPHICGDI::QUERYMSCENE_SCRIPT* CNMultiScene::OnTimer()
		{
			if( m_pSpr == NULL )return NULL;
			if( m_stSceneState.bScenePlay == FALSE )return NULL;
			if( m_stSceneState.nStartSceneTime <=0 )return NULL;

			DWORD curtime = timeGetTime();

			m_stSceneState.nCurPlayTime = curtime - m_stSceneState.nStartSceneTime;

			scrquery.Reset();

			if(m_pSpr != NULL) {
				while(m_pSpr->QueryMSceneScript(&scrquery, m_nSceneNum, m_stSceneState.nCurPlayTime, m_stSceneState.nSceneTime , TRUE))
				{
					return &scrquery;
				}
			}

			if(m_stSceneState.bLoop == FALSE) {
				if(m_stSceneState.nCurPlayTime >= m_stSceneState.nSceneTime * m_stSceneState.nLoop) {
					StopMScene();
				}
			}

			return NULL;
		}

		BOOL CNMultiScene::DrawMScene(CDC *pDC)
		{
			if(m_stSceneState.bScenePlay == FALSE)
				return TRUE;	

			m_pPage->DrawMultiScene(m_Xp, m_Yp, m_pSpr, m_nSceneNum, m_stSceneState.nCurPlayTime, TRUE);

			return TRUE;
		}

	} //namespace SKINGDI

}//namespace NMBASE