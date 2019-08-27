#include "StdAfx.h"
#include "BetCmdEffect.h"

namespace effect
{

CBetCmdEffect::CBetCmdEffect()
{	
	m_nStartTime = 0;
	m_nEndTime = 0 ;
	Xp = 0 ;
	Yp = 0 ;
	m_nDestroyDelTime = 0;
	m_PlayerNumber  = -1;
	m_pEffectScene = NULL;
	m_nBetEffectBigPos = 0;
	m_nBetEffectSmallPos = 0;


	m_sEffectPos[ ANF_BET_HALF - 1 ].nBigPos	 = 0;	// 하프
	m_sEffectPos[ ANF_BET_HALF - 1 ].nSmallPos= 7; 

	m_sEffectPos[ ANF_BET_CALL - 1 ].nBigPos	 = 1;	// 콜
	m_sEffectPos[ ANF_BET_CALL - 1 ].nSmallPos= 8; 

	m_sEffectPos[ ANF_BET_CHECK - 1 ].nBigPos	 = 2;	// 체크
	m_sEffectPos[ ANF_BET_CHECK - 1 ].nSmallPos= 9; 

	m_sEffectPos[ ANF_BET_DADANG - 1 ].nBigPos	 = 3;	// 따당
	m_sEffectPos[ ANF_BET_DADANG - 1 ].nSmallPos= 10; 

	m_sEffectPos[ ANF_BET_PING - 1 ].nBigPos	 = 4;	// 삥
	m_sEffectPos[ ANF_BET_PING - 1 ].nSmallPos= 11; 

	m_sEffectPos[ ANF_BET_DIE - 1 ].nBigPos	 = 5;	// 다이
	m_sEffectPos[ ANF_BET_DIE - 1 ].nSmallPos= 12; 

	m_sEffectPos[ ANF_BET_ALLIN - 1 ].nBigPos	 = 13;	// 올인
	m_sEffectPos[ ANF_BET_ALLIN - 1 ].nSmallPos= 14; 

	m_sEffectPos[ ANF_BET_LIMIT_SMALL_BET - 1 ].nBigPos	 = 21;	// BET (리미트베팅)
	m_sEffectPos[ ANF_BET_LIMIT_SMALL_BET - 1 ].nSmallPos= 24; 

	m_sEffectPos[ ANF_BET_LIMIT_BIG_BET - 1 ].nBigPos	 = 20;	// BET (리미트베팅)
	m_sEffectPos[ ANF_BET_LIMIT_BIG_BET - 1 ].nSmallPos= 23; 

	m_sEffectPos[ ANF_BET_LIMIT_RAISE - 1 ].nBigPos	 = 22;	// RAISE (리미트베팅)
	m_sEffectPos[ ANF_BET_LIMIT_RAISE - 1 ].nSmallPos= 25; 

// 	m_sEffectPos[ ANF_BET_RAISE_X3 - 1 ].nBigPos	 = 18;	// _X5
// 	m_sEffectPos[ ANF_BET_RAISE_X3 - 1 ].nSmallPos= 21; 


}

CBetCmdEffect::~CBetCmdEffect()
{	
//	g_pGameView->m_cPlayerDrawManager.ClearBettingMoney(m_PlayerNumber);
	m_nStartTime = 0;	
	m_PlayerNumber  = -1;
	m_nDestroyDelTime = 0;

	m_nBetEffectBigPos = -1;		// 큰 이펙트 위치
	m_nBetEffectSmallPos = -1;		// 작은 이펙트 위치

	if( m_pEffectScene != NULL)
	{
		delete m_pEffectScene;
	}

	m_pEffectScene = NULL;
}


BOOL CBetCmdEffect::Init( int nPlayerNum, OBJECT_TYPE eAniType, int nEffectType, BOOL bLastBetMax )
{			
	if ( eAniType < ANF_BET_CMD || eAniType >= _ANF_SEPERATOR1_)
	{
		return FALSE;
	}

	// 이펙트의 위치를 찾아 준다.
	// -1을 해주는 이유는 씬이 0부터 시작 하기 때문이다.
	// g_GAME_BD()->GetMaxPlayer()를 곱해주는 이유는 버튼간 Gap이 플레이어 수만큼이기 때문

	//int nBetEffectStartPos = nBetEffectStartPos = nEffectType - ANF_BET_CMD - 1;
	//규제안 : 맥스 이펙트 추가
	int nBetEffectStartPos = 0;
	if (nEffectType == ANF_BET_MAX)
	{
		nBetEffectStartPos = 130;
		m_nBetEffectBigPos   = 130 + nPlayerNum;
		m_nBetEffectSmallPos = 135 + nPlayerNum;
	} 
#if defined(BUTTON_CHANGE_FULLTODADANG)
	else if (nEffectType == ANF_BET_FULL)
	{
		nBetEffectStartPos = 140;
		m_nBetEffectBigPos   = 140 + nPlayerNum;
		m_nBetEffectSmallPos = 145 + nPlayerNum;
	}

#endif
	else
	{
		nBetEffectStartPos = nBetEffectStartPos = nEffectType - ANF_BET_CMD - 1;
	}


	//규제안 : 맥스 이펙트 추가
	if(nBetEffectStartPos >= 0 && nBetEffectStartPos < 10 && nBetEffectStartPos != 130 )
	{
		m_nBetEffectBigPos   = (m_sEffectPos[nBetEffectStartPos].nBigPos   * g_GAME_BD()->GetMaxPlayer()) + nPlayerNum;
		m_nBetEffectSmallPos = (m_sEffectPos[nBetEffectStartPos].nSmallPos * g_GAME_BD()->GetMaxPlayer()) + nPlayerNum;
	}
	else if( nBetEffectStartPos != 130 )
	{
		// 값이 잘 못 들어갔습니다.
		//__asm int 3
	}

	m_nObjID			= eAniType;		
	DrawFaceKind		= DRAW_BOTTOM;
	m_nDestroyDelTime	= 0;
	m_nEndTime			= GLOBAL_SPR[ spr::GSPR_ET_BETCMD ].GetMScenePlayTime( m_nBetEffectBigPos );
	m_nStartTime		= timeGetTime();	
	m_PlayerNumber		= nPlayerNum;
	bDifferntBetStyle	= FALSE;


	m_pEffectScene = new NMBASE::SKINGDI::CNMultiScene(0, 0, g_pGameView, &g_pGameView->Page, &GLOBAL_SPR[ spr::GSPR_ET_BETCMD ], m_nBetEffectBigPos );
	m_pEffectScene->StartMScene(1);

#ifdef TRACE_OUTPUT			
	char aaa[256] = {0, };
	sprintf(aaa, "%d 유저", m_PlayerNumber);
	TTrace::Debug()->Send ("CBetCmdEffect::Init()", aaa);   // single byte string	
#endif

	return TRUE;
}


CPoint CBetCmdEffect::GetEffectPos(void)
{
	if(NULL==GLOBAL_SPR[ spr::GSPR_ET_BETCMD ].multiscene.ppMScene[m_nBetEffectBigPos]) return CPoint(0, 0);
	NMBASE::GRAPHICGDI::TRACK_SCENE* pEffect = GLOBAL_SPR[ spr::GSPR_ET_BETCMD ].multiscene.ppMScene[m_nBetEffectBigPos]->ppTrack[0]->GetDataScene();
	if(NULL==pEffect) return CPoint(0, 0);
	return pEffect->Pos;
}

void CBetCmdEffect::Destroy()
{
	m_nEndTime = 0;
	m_nStartTime = 0;
	m_nDestroyDelTime = 0;

	SAFE_DELETE( m_pEffectScene );
	
#ifdef TRACE_OUTPUT		
	char aaa[256] = {0, };
	sprintf(aaa, "%d 유저", m_PlayerNumber);
	TTrace::Debug()->Send ("CBetCmdEffect::Destroy()", aaa);   // single byte string	
#endif
}

void CBetCmdEffect::SetSmallCmd()
{
	if ( bDifferntBetStyle ) 
	{
		return;
	}

	bDifferntBetStyle = TRUE;

	if ( m_pEffectScene != NULL ) 
	{		
		m_pEffectScene->SetDrawIndex(m_nBetEffectSmallPos);
	}
}

int CBetCmdEffect::ProcessObject()
{	
	if(m_nDestroyDelTime > 0)
	{
		m_nDestroyDelTime--;
		if(m_nDestroyDelTime <= 0)
		{
			m_nDestroyDelTime = 0;
			m_nStartTime = 0;
			m_nEndTime = 0;
			return PROCESS_KILLME;
		}
	}	

	if( m_pEffectScene != NULL )
	{
		if( m_pEffectScene->m_stSceneState.bScenePlay == TRUE )
		{
			m_pEffectScene->OnTimer();
		}
		else
		{
			Destroy();
			return PROCESS_KILLME;
		}
	}	

	if(m_nStartTime == 0) 
	{		
		m_nStartTime = 0;
		m_nEndTime = 0;
		m_nDestroyDelTime = 0;
		return PROCESS_KILLME;				
	}
	return PROCESS_OK;
}

void CBetCmdEffect::DrawObject(CDC *pDC)
{	
	if( m_pEffectScene != NULL )
	{
		m_pEffectScene->DrawMScene(NULL);
	}
}

}


