#include "StdAfx.h"
#include "MadeEffect.h"

namespace effect
{

CMadeEffect::CMadeEffect()
:m_pSpr( NULL )
{
	m_pMultiScene = NULL;		
}

CMadeEffect::~CMadeEffect()
{
	Destroy();
	m_pSpr = NULL;
}

BOOL CMadeEffect::Init()
{
	m_nObjID = ANF_PLAYER_COMPLETEMADE_EFT;	//종류	
	DrawFaceKind = DRAW_TOP;
	Destroy();

	m_pSpr = &GLOBAL_SPR[ spr::GSPR_ET_MADE ];

	m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene( 0, 0, g_pGameView,&g_pGameView->Page, m_pSpr ,0 );
	m_pMultiScene->StartMScene(1);
	PBPlayEffectSound(SND_MADE);

	return TRUE;
}

void CMadeEffect::DrawObject(CDC *pDC)
{
	if(pDC == NULL){nResult = PROCESS_KILLME;}

	if( m_pMultiScene != NULL){
		m_pMultiScene->DrawMScene(pDC);
	}
}

void CMadeEffect::Destroy()
{	
	if( m_pMultiScene != NULL ){
		delete m_pMultiScene;
	}
	m_pMultiScene = NULL;
}

int  CMadeEffect::ProcessObject()
{	
	if ( m_pSpr && m_pMultiScene != NULL )
	{
		if( m_pMultiScene->m_stSceneState.bScenePlay == TRUE ){

			DWORD curtime = m_pMultiScene->m_stSceneState.nCurPlayTime;

			m_pMultiScene->OnTimer();

			DWORD NowPlayTime = m_pMultiScene->m_stSceneState.nCurPlayTime;

			NMBASE::GRAPHICGDI::QUERYMSCENE_SOUND sndquery;
			sndquery.Reset();
			while ( m_pSpr->QueryMSceneSound( &sndquery, m_pMultiScene->m_nSceneNum, curtime , NowPlayTime, TRUE))
			{
				// 등록되지 않은 사운드 파일이면 등록
				if (!NMBASE::SOUND::g_Sound.IsRegistered(sndquery.SndFile)) 
				{
					NMBASE::SOUND::g_Sound.RegistWav(sndquery.SndFile, NMBASE::SOUND::SNDPRIORITY_MID, 1, TRUE);
				}

				// 사운드 연주
				NMBASE::SOUND::g_Sound.PlayWav(sndquery.SndFile, FALSE, sndquery.Volume);
			}
		}
		else{
			Destroy();
			return PROCESS_KILLME;
		}
	}
	return PROCESS_OK;
}

}


