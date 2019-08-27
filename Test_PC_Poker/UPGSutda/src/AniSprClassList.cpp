// AniSprClassList.cpp: implementation of the AniSprClassList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "AniSprClassList.h"
#include "mmsystem.h"
#include "GlobalGame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
//배팅 커멘트 표현


CBettingEffect::CBettingEffect()
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

	
	m_sEffectPos[ ANF_BET_HALF - ANF_BET_CMD - 1 ].nBigPos	 = 0;	// 하프
	m_sEffectPos[ ANF_BET_HALF - ANF_BET_CMD - 1 ].nSmallPos= 7; 

	m_sEffectPos[ ANF_BET_CALL - ANF_BET_CMD - 1 ].nBigPos	 = 1;	// 콜
	m_sEffectPos[ ANF_BET_CALL - ANF_BET_CMD - 1 ].nSmallPos= 8; 
	
	m_sEffectPos[ ANF_BET_CHECK - ANF_BET_CMD - 1 ].nBigPos	 = 2;	// 체크
	m_sEffectPos[ ANF_BET_CHECK - ANF_BET_CMD - 1 ].nSmallPos= 9; 
	
	m_sEffectPos[ ANF_BET_DADANG - ANF_BET_CMD - 1 ].nBigPos	 = 3;	// 따당
	m_sEffectPos[ ANF_BET_DADANG - ANF_BET_CMD - 1 ].nSmallPos= 10; 
	
	m_sEffectPos[ ANF_BET_PING - ANF_BET_CMD - 1 ].nBigPos	 = 4;	// 삥
	m_sEffectPos[ ANF_BET_PING - ANF_BET_CMD - 1 ].nSmallPos= 11; 
	
	m_sEffectPos[ ANF_BET_DIE - ANF_BET_CMD - 1 ].nBigPos	 = 5;	// 다이
	m_sEffectPos[ ANF_BET_DIE - ANF_BET_CMD - 1 ].nSmallPos= 12; 

	m_sEffectPos[ ANF_BET_ALLIN - ANF_BET_CMD - 1 ].nBigPos	 = 13;	// 올인
	m_sEffectPos[ ANF_BET_ALLIN - ANF_BET_CMD - 1 ].nSmallPos= 14; 

	m_sEffectPos[ ANF_BET_RAISE_X1 - ANF_BET_CMD - 1 ].nBigPos	 = 15;	// 맥스 2018.05.16 추후에 _X1을 삭제
	m_sEffectPos[ ANF_BET_RAISE_X1 - ANF_BET_CMD - 1 ].nSmallPos= 16; 
	
	m_sEffectPos[ ANF_BET_RAISE_X2 - ANF_BET_CMD - 1 ].nBigPos	 = 17;	// _X2
	m_sEffectPos[ ANF_BET_RAISE_X2 - ANF_BET_CMD - 1 ].nSmallPos= 20; 

	m_sEffectPos[ ANF_BET_RAISE_X3 - ANF_BET_CMD - 1 ].nBigPos	 = 18;	// _X5
	m_sEffectPos[ ANF_BET_RAISE_X3 - ANF_BET_CMD - 1 ].nSmallPos= 21; 

// 	m_sEffectPos[ ANF_BET_LIMIT_X10 - ANF_BET_CMD - 1 ].nBigPos	 = 18;	// _X10
// 	m_sEffectPos[ ANF_BET_LIMIT_X10 - ANF_BET_CMD - 1 ].nSmallPos= 23; 
// 
// 	m_sEffectPos[ ANF_BET_LIMIT_X20 - ANF_BET_CMD - 1 ].nBigPos	 = 19;	// _X20
// 	m_sEffectPos[ ANF_BET_LIMIT_X20 - ANF_BET_CMD - 1 ].nSmallPos= 24; 

}

CBettingEffect::~CBettingEffect()
{	
	m_nStartTime = 0;	
	m_PlayerNumber  = -1;
	m_nDestroyDelTime = 0;
	//	LastBetEftSpr.Remove();

	m_nBetEffectBigPos = -1;		// 큰 이펙트 위치
	m_nBetEffectSmallPos = -1;		// 작은 이펙트 위치

	if ( m_pEffectScene != NULL)
	{
		delete m_pEffectScene;
	}

	m_pEffectScene = NULL;
}


BOOL CBettingEffect::Init(int nPlayerNum, ENUM_ANIFILEID eAniType, int nEffectType, BOOL bLastBetMax)
{			
	//if ( eAniType < ANF_BET_CMD || eAniType > ANF_BET_ALLIN )
	if ( eAniType < ANF_BET_CMD || eAniType > ANF_BET_RAISE_X3 )
	{
		return FALSE;
	}

	// 이펙트의 위치를 찾아 준다.
	// -1을 해주는 이유는 씬이 0부터 시작 하기 때문이다.
	// g_GAMEPROCESS()->GetMaxPlayer()를 곱해주는 이유는 버튼간 Gap이 플레이어 수만큼이기 때문

	int nBetEffectStartPos = nBetEffectStartPos = nEffectType - ANF_BET_CMD - 1;
#if defined(BUTTON_CHANGE_FULLTODADANG)
	if (nEffectType==ANF_BET_FULL)
	{
		m_nBetEffectBigPos   = 140 + nPlayerNum;
		m_nBetEffectSmallPos = 145 + nPlayerNum;
	}
	else if (nBetEffectStartPos >= 0 && nBetEffectStartPos < 10)
	{
		m_nBetEffectBigPos   = (m_sEffectPos[nBetEffectStartPos].nBigPos   * g_GAMEPROCESS()->GetMaxPlayer()) + nPlayerNum;
		m_nBetEffectSmallPos = (m_sEffectPos[nBetEffectStartPos].nSmallPos * g_GAMEPROCESS()->GetMaxPlayer()) + nPlayerNum;
	}
	else
	{
		// 값이 잘 못 들어갔습니다.
		__asm int 3
	}
#else
	if (nBetEffectStartPos >= 0 && nBetEffectStartPos < 10)
	{
		m_nBetEffectBigPos   = (m_sEffectPos[nBetEffectStartPos].nBigPos   * g_GAMEPROCESS()->GetMaxPlayer()) + nPlayerNum;
		m_nBetEffectSmallPos = (m_sEffectPos[nBetEffectStartPos].nSmallPos * g_GAMEPROCESS()->GetMaxPlayer()) + nPlayerNum;
	}
	else
	{
		// 값이 잘 못 들어갔습니다.
		__asm int 3
	}
#endif

	m_nObjID			= eAniType;		
	DrawFaceKind		= DRAW_BOTTOM;
	m_nDestroyDelTime	= 0;
	m_nEndTime			= g_sprBettingEffect.GetMScenePlayTime(m_nBetEffectBigPos);
	m_nStartTime		= timeGetTime();	
	m_PlayerNumber		= nPlayerNum;
	bDifferntBetStyle	= FALSE;


	m_pEffectScene = new NMBASE::SKINGDI::CNMultiScene(0, 0, g_pGameView, &g_pGameView->Page, &g_sprBettingEffect, m_nBetEffectBigPos);
	m_pEffectScene->StartMScene(1);

#ifdef TRACE_OUTPUT			
	char aaa[256] = {0, };
	sprintf(aaa, "%d 유저", m_PlayerNumber);
	TTrace::Debug()->Send ("CBettingEffect::Init()", aaa);   // single byte string	
#endif

	return TRUE;
}

void CBettingEffect::Destroy()
{
	m_nEndTime = 0;
	m_nStartTime = 0;
	m_nDestroyDelTime = 0;

	if ( m_pEffectScene != NULL ) 
	{
		delete m_pEffectScene;
	}

	m_pEffectScene = NULL;

#ifdef TRACE_OUTPUT		
	char aaa[256] = {0, };
	sprintf(aaa, "%d 유저", m_PlayerNumber);
	TTrace::Debug()->Send ("CBettingEffect::Destroy()", aaa);   // single byte string	
#endif
}

void CBettingEffect::SetSmallCmd()
{
	if (bDifferntBetStyle) 
		return;

	bDifferntBetStyle = TRUE;

	if ( m_pEffectScene != NULL ) 
	{		
		if ( bDifferntBetStyle == FALSE )
		{
			m_pEffectScene->SetDrawIndex(m_nBetEffectBigPos);

#ifdef TRACE_OUTPUT		
			char aaa[256] = {0, };
			sprintf(aaa, "%d 유저", m_PlayerNumber);
			TTrace::Debug()->Send ("CBettingEffect::SetSmallCmd()-big", aaa);   // single byte string	
#endif
		}
		else
		{			
			m_pEffectScene->SetDrawIndex(m_nBetEffectSmallPos);

#ifdef TRACE_OUTPUT
			char aaa[256] = {0, };
			sprintf(aaa, "%d 유저", m_PlayerNumber);
			TTrace::Debug()->Send ("CBettingEffect::SetSmallCmd()-small", aaa);   // single byte string	
#endif
		}


	}
}

int CBettingEffect::ProcessObject()
{	
	if (m_nDestroyDelTime > 0)
	{
		m_nDestroyDelTime--;
		if (m_nDestroyDelTime <= 0)
		{
			m_nDestroyDelTime = 0;
			m_nStartTime = 0;
			m_nEndTime = 0;
			return PROCESS_KILLME;
		}
	}	

	if ( m_pEffectScene != NULL )
	{
		if ( m_pEffectScene->m_stSceneState.bScenePlay == TRUE )
		{
			m_pEffectScene->OnTimer();
		}
		else
		{
			Destroy();
			return PROCESS_KILLME;
		}
	}	

	if (m_nStartTime == 0) 
	{		
		m_nStartTime = 0;
		m_nEndTime = 0;
		m_nDestroyDelTime = 0;
		return PROCESS_KILLME;				
	}
	return PROCESS_OK;
}

void CBettingEffect::DrawObject(CDC *pDC)
{	
	if ( m_pEffectScene != NULL )
	{
		m_pEffectScene->DrawMScene(NULL);
	}
}

//=========================================================================//


//////////////////////////////////////////////////////////////////////////

//ANF_SDLG_GRADE
CGradeSpr::CGradeSpr()
{
	m_pMultiScene = NULL;	
	m_pPage = NULL;
}

CGradeSpr::~CGradeSpr()
{
	Destroy();
}

BOOL CGradeSpr::Init( int index , NMBASE::GRAPHICGDI::CPage *pPage)
{
	m_nObjID = ANF_SDLG_GRADE;	//종류	
	DrawFaceKind = DRAW_TOP;
	Destroy();

	if ( pPage == NULL )
	{
		nResult = PROCESS_KILLME;
		return FALSE;
	}
	m_pPage = pPage ;
	POINT pt;
	if ( index == 0 )
	{
		pt.x = 361;
		pt.y = 200;
	}
	else if ( index == 1 )
	{
		pt.x = 5;
		pt.y = 10;	
	}
	else if ( index == 2 )
	{
		pt.x = 5;
		pt.y = 69;
	}

	m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(pt.x,pt.y,g_pGameView,m_pPage,&GradeSpr,index);
	m_pMultiScene->StartMScene(1);

	return TRUE;
}

void CGradeSpr::DrawObject(CDC *pDC)
{
	if (pDC == NULL)
	{
		nResult = PROCESS_KILLME;
	}

	if ( m_pMultiScene != NULL)
	{
		m_pMultiScene->DrawMScene(pDC);
	}
}

void CGradeSpr::Destroy()
{	
	if ( m_pMultiScene != NULL )
	{
		delete m_pMultiScene;
	}
	m_pMultiScene = NULL;
}

int  CGradeSpr::ProcessObject()
{	
	if ( m_pMultiScene != NULL )
	{
		if ( m_pMultiScene->m_stSceneState.bScenePlay == TRUE ){
			m_pMultiScene->OnTimer();		
		}
		else
		{
			Destroy();
			return PROCESS_KILLME;
		}
	}
	return PROCESS_OK;
}





//ANF_PLAYER_CHANCEITEM_MSG 찬스 아이템 효과
//채팅창 찬스 메세지 지급 받을때 같이 보여준다.

CChance_Msg_EffectSpr::CChance_Msg_EffectSpr()
{		
	m_pMultiScene = NULL;	
	ZeroMemory(m_ChanceItemText,sizeof(m_ChanceItemText));
}

CChance_Msg_EffectSpr::~CChance_Msg_EffectSpr()
{	
	Destroy();
}

void CChance_Msg_EffectSpr::Init( int itemcode )
{
	m_nObjID = ANF_PLAYER_CHANCEITEM;	
	nDelTime = 5;
	DrawFaceKind = DRAW_TOP;

	if ( m_pMultiScene != NULL)
	{
		delete m_pMultiScene;
		m_pMultiScene = NULL;
	}	

	/*
	ITEM_SCODE_REWARD_MADE=58,			// 메이드
	ITEM_SCODE_REWARD_RSF=54,			//로티플	//찬스 아이템
	ITEM_SCODE_REWARD_SF=53,			//스티플
	ITEM_SCODE_REWARD_FCARD=52,			//포카드
	ITEM_SCODE_REWARD_FH=51,			//풀하우스
	ITEM_SCODE_REWARD_LUCKAVATA=109,	//[행운아바타] 2006.07.24
	ITEM_SCODE_REWARD_LOWGRADE=123,		//승부사 입문 [입문아이템]
	ITEM_SCODE_REWARD_HIGHGRADE=124,	//승부사 등극
	ITEM_SCODE_REWARD_ALLINKING=125,	//올인왕찬스	
	ITEM_SCODE_REWARD_SPECIALLUCKITEM = 132, //[스행운]2007.10.17
	ITEM_SCODE_REWARD_ALLINCHANCE=134,	//[올인찬스아바타] 2008.07
	*/

	switch(itemcode) 
	{
	case ITEM_SCODE_REWARD_38:	//38광땡	ITEM_SCODE_REWARD38		59	
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,0);
		break;
	case ITEM_SCODE_REWARD_PARKMUNSU:	//암행어사	ITEM_SCODE_REWARDPARKMUNSU 60
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,1);
		break;
	case ITEM_SCODE_REWARD_TANGKILLER:	//땡잡이	ITEM_SCODE_TANGKILLER 61
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,2);
		break;
	case ITEM_SCODE_REWARD_VIP :	////VIP 찬스	ITEM_SCODE_REWARD_VIP = 373		11.08.28
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,4);
		break;
	case ITEM_SCODE_REWARD_VVIP :	////VVIP 찬스	ITEM_SCODE_REWARD_VVIP = 374	11.08.28
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,5);
		break;	
	}

	//get_ChanceText(itemcode);
}

void CChance_Msg_EffectSpr::get_ChanceText(int itemCode)
{

// 	ZeroMemory(m_ChanceItemText,sizeof(m_ChanceItemText));
// 	CString str;
// 
// 	//UGP작업 아래삭제후 교체
// 	// 	str.Format( "[%s]님 %s \n구입자격 획득.\n", 
// 	// 		GM().GetMyInfo()->UI.NickName, g_pMainDlg->m_RewardItemMan.GetRewardItemName( itemCode ));
// 
// 	str.Format( "◈[%s]님 %s \n구입자격 획득.\n", GM().GetMyInfo()->UI.NickName, RewardItemMan().GetRewardItemName( itemCode ).GetBuffer());
// 
// 	strncpy(m_ChanceItemText,(char*)(LPCTSTR)str,str.GetLength());	
}

void CChance_Msg_EffectSpr::Destroy()
{	
	if ( m_pMultiScene != NULL )
	{
		delete m_pMultiScene;		
	}
	m_pMultiScene= NULL;
}

int CChance_Msg_EffectSpr::ProcessObject()
{	
	if (nDelTime>0)
	{
		nDelTime--;
		if ( nDelTime == 0 )
		{
			nDelTime = 0;
			if ( m_pMultiScene != NULL )
			{
				m_pMultiScene->StartMScene(1);
				if ( strlen(m_ChanceItemText) > 0 )
				{
					AddGameInfoViewEdit(m_ChanceItemText, RGB(255, 249, 73));
				}				
			}
		}
		return PROCESS_OK;
	}

	if ( m_pMultiScene != NULL )
	{
		if ( m_pMultiScene->m_stSceneState.bScenePlay == TRUE )
		{
			m_pMultiScene->OnTimer();
		}
		else
		{
			Destroy();
			return PROCESS_KILLME;
		}		
	}
	return PROCESS_OK;
}

void CChance_Msg_EffectSpr::DrawObject(CDC *pDC)
{
	if ( m_pMultiScene != NULL )
	{

		//		if ( g_pChatWinMan->IsShow() && g_pChatWinMan->GetViewMode() == eCHATVIEW::eCHAT){
		//			m_pMultiScene->DrawScene(NULL);
		//		}

		m_pMultiScene->DrawMScene(NULL);		
	}
}




CMadeEffect::CMadeEffect()
:m_pSpr( NULL )
{
	m_pMultiScene = NULL;	
	m_nCardCnt=0;
}

CMadeEffect::~CMadeEffect()
{
	Destroy();
	m_pSpr = NULL;
}

BOOL CMadeEffect::Init(int nCardCnt)
{
	m_nObjID = ANF_PLAYER_COMPLETEMADE_EFT;	//종류	
	DrawFaceKind = DRAW_TOP;
	m_nCardCnt = nCardCnt;
	Destroy();

	m_pSpr = &g_sprGLOBAL[ spr::GSPR_ET_MADE ];

	m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene( 0, 0, g_pGameView,&g_pGameView->Page, m_pSpr ,2 );
	m_pMultiScene->StartMScene(1);
	PBPlayEffectSound(SND_MADE);

	return TRUE;
}

void CMadeEffect::DrawObject(CDC *pDC)
{
	if (pDC == NULL){nResult = PROCESS_KILLME;}

	if ( m_pMultiScene != NULL){
		m_pMultiScene->DrawMScene(pDC);


		//카드이펙트 (카드갯수만큼그린다)		
		for (int i=0; i<m_nCardCnt; i++) 
		{			
			CPoint ptCard = g_PLAYER(0)->m_cMyCard[i].GetPos();
			g_pGameView->Page.DrawMultiScene(ptCard.x, ptCard.y, m_pSpr, 0, m_pMultiScene->m_stSceneState.nCurPlayTime, FALSE);		
		}
			
		
	}
}

void CMadeEffect::Destroy()
{	
	if ( m_pMultiScene != NULL ){
		delete m_pMultiScene;
	}
	m_pMultiScene = NULL;
}

int  CMadeEffect::ProcessObject()
{	
	if ( m_pSpr && m_pMultiScene != NULL )
	{	
		
		
		if ( m_pMultiScene->m_stSceneState.bScenePlay == TRUE ){

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