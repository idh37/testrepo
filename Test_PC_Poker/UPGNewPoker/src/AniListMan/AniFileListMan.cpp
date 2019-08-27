// AniFileListMan.cpp: implementation of the CAniFileListMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AniFileListMan.h"
#include "mmsystem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	HIGHLIGHT_HALF_INDEX	30
#define	ALLIN_INDEX				65

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

////////////////////////////////////////////////////////////////////////////////////////////
//배팅 커멘트 표현

CBetCmdAni::CBetCmdAni()
{	
	m_dwStartTime = 0;
	m_dwEndTime = 0 ;
	m_bSmallCmd = FALSE;	
	m_nPNum  = -1;
	m_nBetCmd = -1;
}

CBetCmdAni::~CBetCmdAni()
{	
//	g_pGameView->m_cPlayerDrawManager.ClearBettingMoney(m_nPNum);
	m_dwStartTime = 0;	
	m_bSmallCmd = FALSE;
	m_nPNum  = -1;
	m_nBetCmd = -1;
	m_nDelTime = 0;
}

BOOL CBetCmdAni::Init(int pnum,ENUM_ANIFILE enumkind,int betcmd, bool bHighlight)
{
	if ( enumkind < ANF_BET_CMD || enumkind >= ANF_BET_MAX)
		return FALSE;

	m_nObjID = enumkind;

	if (betcmd == ANF_BET_ALLIN)
	{
		m_nBetCmd = ALLIN_INDEX + pnum;
	}
	else if (betcmd == ANF_BET_HALF && bHighlight)
	{
		m_nBetCmd = HIGHLIGHT_HALF_INDEX + pnum;
	}
	else
	{
		m_nBetCmd = (betcmd - ANF_BET_HALF) * GM().GetMaxPlayer() + pnum;
	}

	if (betcmd == ANF_BET_MAX)
	{
		m_nBetCmd = 130 + pnum;
	}

#if defined(BUTTON_CHANGE_FULLTODADANG)
	else if (betcmd==ANF_BET_FULL)
	{
		m_nBetCmd = 140 + pnum;
	}
#endif
	
	DrawFaceKind = DRAW_BOTTOM;
	m_nDelTime = 0;
	m_dwEndTime = g_sprBetCmd.GetMScenePlayTime(m_nBetCmd);
	m_bSmallCmd = FALSE;
	m_dwStartTime = timeGetTime();
	
	//if (m_nBetCmd >= 5 && m_nBetCmd <= 9)
	//	ErrorLogConsole("m_dwStartTime %d", m_dwStartTime);
	
	m_nPNum = pnum;
	
	return TRUE;
}

CPoint CBetCmdAni::GetEffectPos(void)
{
	if (NULL == g_sprBetCmd.multiscene.ppMScene[m_nBetCmd])
		return CPoint(0, 0);
	
	NMBASE::GRAPHICGDI::TRACK_SCENE* pEffect = g_sprBetCmd.multiscene.ppMScene[m_nBetCmd]->ppTrack[0]->GetDataScene();
	
	if (NULL == pEffect)
		return CPoint(0, 0);

	return pEffect->Pos;
}

void CBetCmdAni::Destroy()
{
	m_dwEndTime = 0;
	m_dwStartTime = 0;
	m_nDelTime = 0;
}

int CBetCmdAni::ProcessObject()
{	
	if (m_nDelTime > 0)
	{
		m_nDelTime--;

		if (m_nDelTime <= 0)
		{
			m_nDelTime = 0;
			m_dwStartTime = 0;
			m_dwEndTime = 0;

			return PROCESS_KILLME;
		}
	}

	if (m_dwStartTime == 0)
	{
		m_dwStartTime = 0;
		m_dwEndTime = 0;
		m_nDelTime = 0;

		return PROCESS_KILLME;				
	}

	return PROCESS_OK;
}

void CBetCmdAni::DrawObject(CDC *pDC)
{
	if (m_dwStartTime > 0)
	{
		NMBASE::GRAPHICGDI::CPage* pPage = &g_pGameView->Page;
		DWORD drawt = timeGetTime() - m_dwStartTime;
		
		pPage->DrawMultiScene(g_cUIData.m_ptBettingCmd.x, g_cUIData.m_ptBettingCmd.y, &g_sprBetCmd, m_nBetCmd, drawt);
	}
}

void CBetCmdAni::SetSmallCmd()
{
	if (m_bSmallCmd || m_nBetCmd > (ALLIN_INDEX + GM().GetMaxPlayer() * 2))
		return;

	m_bSmallCmd = TRUE;

	if (m_nBetCmd >= ALLIN_INDEX)
	{
		m_nBetCmd += GM().GetMaxPlayer();
	}
	else if (m_nBetCmd >= HIGHLIGHT_HALF_INDEX && m_nBetCmd < (HIGHLIGHT_HALF_INDEX+GM().GetMaxPlayer()))
	{
		m_nBetCmd = HIGHLIGHT_HALF_INDEX+GM().GetMaxPlayer() + m_nPNum;
	}
	else
	{
		m_nBetCmd += (HIGHLIGHT_HALF_INDEX + GM().GetMaxPlayer());
	}

	m_dwEndTime = g_sprBetCmd.GetMScenePlayTime(m_nBetCmd);
}

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

	m_pPage = pPage;
	POINT pt;

	if ( index == 0 )
	{		
		if ( GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY )
		{
			pt.x = 361;
			pt.y = 150;			
		}
		else
		{
			pt.x = 361;
			pt.y = 200;
		}				
	}
	else if ( index == 1 )
	{
		pt.x = 10;
		pt.y = 20;	
	}
	else if ( index == 2 )
	{
		pt.x = 10;
		pt.y = 10;
	}
	else if ( index == 3)
	{
		index = 0;
		pt.x = 2;
		pt.y = 20;
	}
	
	m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(pt.x,pt.y,g_pGameView,m_pPage,&g_sprGrade,index);
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

CMadeEffect::CMadeEffect()
:m_pSpr( NULL )
{
	m_pMultiScene = NULL;		
}

CMadeEffect::~CMadeEffect()
{
	Destroy();
	m_pSpr = NULL;
	m_nCardCnt=0;
}

BOOL CMadeEffect::Init(int nCardCnt)
{
	m_nObjID = ANF_PLAYER_COMPLETEMADE_EFT;	//종류	
	DrawFaceKind = DRAW_TOP;
	m_nCardCnt = nCardCnt;
	Destroy();

	m_pSpr = &GLOBAL_SPR[ spr::GSPR_ET_MADE ];

	m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene( 0, 0, g_pGameView,&g_pGameView->Page, m_pSpr ,1 );
	m_pMultiScene->StartMScene(1);
	PBPlayEffectSound(SND_MADE);

	return TRUE;
}

void CMadeEffect::DrawObject(CDC *pDC)
{
	if (pDC == NULL)
	{
		nResult = PROCESS_KILLME;
	}

	if ( m_pMultiScene != NULL)
	{
		m_pMultiScene->DrawMScene(pDC);

		//카드이펙트 (카드갯수만큼그린다)		
		for (int i=0; i<m_nCardCnt; i++) 
		{			
			CPoint ptCard = GAME()->GetPlayerNP(0)->m_listCard[i].GetPos();
			g_pGameView->Page.DrawMultiScene(ptCard.x, ptCard.y, m_pSpr, 0, m_pMultiScene->m_stSceneState.nCurPlayTime, FALSE);		
		}
	}
}

void CMadeEffect::Destroy()
{	
	if ( m_pMultiScene != NULL )
	{
		delete m_pMultiScene;
	}

	m_pMultiScene = NULL;
}

int  CMadeEffect::ProcessObject()
{	
	if ( m_pSpr && m_pMultiScene != NULL )
	{
		if ( m_pMultiScene->m_stSceneState.bScenePlay == TRUE )
		{
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
		else
		{
			Destroy();
			return PROCESS_KILLME;
		}
	}

	return PROCESS_OK;
}






//------------------------------------------------------------------------------------------
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

	switch(itemcode) 
	{
// 	case ITEM_SCODE_REWARD_HIGHGRADE:
// 		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&g_ChanceEffectSpr,0);
// 		break;
// 		// 	case ITEM_SCODE_REWARD_MADE:
// 		// 		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,1);
// 		// 		break;
	case ITEM_SCODE_REWARD_RSF:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&g_ChanceEffectSpr,2);
		break;
	case ITEM_SCODE_REWARD_SF:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&g_ChanceEffectSpr,3);
		break;
	case ITEM_SCODE_REWARD_FCARD:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&g_ChanceEffectSpr,4);
		break;
	case ITEM_SCODE_REWARD_FH:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&g_ChanceEffectSpr,5);
		break;	
// 	case ITEM_SCODE_REWARD_CHANCEAVATA:	//이벤트찬스아바타 11.08.01
// 		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&g_ChanceEffectSpr,6);
// 		break;	
	case ITEM_SCODE_REWARD_VIP :	//VIP 찬스 11.08.28
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&g_ChanceEffectSpr,7);
		break;	
	case ITEM_SCODE_REWARD_VVIP :	//VVIP 찬스 11.08.28
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&g_ChanceEffectSpr,8);
		break;
	case ITEM_SCODE_REWARD_BADBEAT_SF_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_RSF_VIP:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&g_ChanceEffectSpr,9);
		break;
	case ITEM_SCODE_REWARD_BADBEAT_FCARD_VIP:
		{
			if (IDX_GAME_SP != GM().GetCurrentGameType())
				m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&g_ChanceEffectSpr,10);
			else										
				m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&g_ChanceEffectSpr,9);
		}
		break;
	case ITEM_SCODE_REWARD_BADBEAT_GOLF_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_SECOND_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_THIRD_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_5MADE_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_6MADE_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_FLUSH_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_FH_VIP:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&g_ChanceEffectSpr,10);
		break;
	case ITEM_SCODE_REWARD_BADBEAT_FH:
	case ITEM_SCODE_REWARD_BADBEAT_FCARD:
	case ITEM_SCODE_REWARD_BADBEAT_SF:
	case ITEM_SCODE_REWARD_BADBEAT_RSF:
	case ITEM_SCODE_REWARD_BADBEAT_GOLF:
	case ITEM_SCODE_REWARD_BADBEAT_SECOND:
	case ITEM_SCODE_REWARD_BADBEAT_THIRD:
	case ITEM_SCODE_REWARD_BADBEAT_5MADE:
	case ITEM_SCODE_REWARD_BADBEAT_6MADE:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&g_ChanceEffectSpr,11);
		break;
	}

//	get_ChanceText(itemcode);
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
// 				if ( strlen(m_ChanceItemText) > 0 )
// 				{
// 					AddGameInfoViewEdit(m_ChanceItemText, RGB(255, 249, 73));
// 				}				
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


