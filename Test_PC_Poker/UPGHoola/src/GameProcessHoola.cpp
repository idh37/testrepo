// GameProcessHoola.cpp: implementation of the CGameProcessHoola class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameProcessHoola.h"
#include "GlobalGame.h"
#include "CommMsgDef_Game.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameProcessHoola::CGameProcessHoola(CGame *pGame)
: CGameProcess(pGame)
{
	m_pPage = NULL;
	m_bOnceInit = FALSE;
	m_i64JackPotMoney = 0;
	m_i64OldJackPotMoney = 0;
	m_nStartBtnDelayType = 0;
	m_eMusicType = eMUSIC_NULL;
	
	for ( int i = 0 ; i < MAX_PLAYER ; i++ )
	{
		m_nArrSndData[i].nSndFxKind = -1;
		m_nArrSndData[i].bSex = FALSE;
	}
	Clear();
}

CGameProcessHoola::~CGameProcessHoola()
{
	Clear();
}

// 사용자 카드 정보 전체 초기화
void CGameProcessHoola::ResetAllPlayerCard( int nResetData )
{
// 	for ( int i = 0 ; i < MAX_PLAYER ; i++ )
// 	{
// 		g_PLAYER(i)->ResetPlayerCard( nResetData );
// 	}
}

// 종료시 안내 메세지 삽입 부분
void CGameProcessHoola::Insert_EndingTextInformation(CString str, COLORREF tcolor)
{
	if ( m_GameEndMsgData.TextMsgCnt >= 30) 
		return;

	if ( m_GameEndMsgData.TextMsgCnt < 0 )
		return;

	m_GameEndMsgData.TextMsg[m_GameEndMsgData.TextMsgCnt] = str;
	m_GameEndMsgData.TextColor[m_GameEndMsgData.TextMsgCnt] = tcolor;	
	m_GameEndMsgData.TextMsgCnt++;	
}

// 채팅창에 정보 메시지를 출력한다.
void CGameProcessHoola::Draw_TextInformationMsg()
{
	if ( m_GameEndMsgData.TextMsgCnt <= 0)
	{
		return;	
	}
	
	for ( int i = 0 ; i < m_GameEndMsgData.TextMsgCnt ; ++i )
	{
		COLORREF tcolor = GetChatColor(ECC2_NOTIFY);

		if ( m_GameEndMsgData.TextColor[i] != NULL )
		{
			tcolor = m_GameEndMsgData.TextColor[i];
		}
		
		AddGameInfoViewEdit(m_GameEndMsgData.TextMsg[i].GetString(), tcolor);
	}
}

/////////////////////////////////////////////////////////////////////
/*
이하 함수 들은 추상 객체
*/
/////////////////////////////////////////////////////////////////////
void CGameProcessHoola::Clear()
{
	// [자동베팅] 2004.07.15
	//	m_IsAutoKindBet = 0; // 0:X  1:49 2:59 3:69
	//	m_bAutoBet = 0;		
	// 클라이언트 현재 상태
	m_nGameState = 0;				// 0 없음, 1, 2, 3, 4, 5, 6 승결정중 7 승결정(게임 오버)
	m_nNo1PosPNum = 0;
	m_nMyPosPNum = 0;		
	
	// 추방 기회 초기화 2회 끝 방장이 바뀌면 2회로 초기화
	g_cBanishMan.Clear();
	ResetFlg();
}

// 초기화
void CGameProcessHoola::Init(NMBASE::GRAPHICGDI::CPage *ppage)
{
	// 객체를 지우지 않기 때문에 한번만 호출이 되도록 한다.
	if (m_bOnceInit)
	{
		return;
	}
	
	m_bOnceInit = TRUE;
	
	Clear();
	m_pPage = ppage;
	
	// 초대하기 대화상자 만들기
	//	m_InviteDlg.Create(IDD_INVITE_DIALOG, NULL);


	CGameProcess::Init(ppage);


	int i = 0;
	for ( i = 0 ; i < GetMaxNewPlayer() ; ++i )
	{
		m_cPlayer[i].Clear();
		m_cPlayer[i].InitRoomData();
		m_cPlayer[i].SetPlayerNum(i);
	}


	m_cPlayScr.Init(g_pGameView, &g_pGameView->Page, 0, g_pGameView->GetGameButtonMan());
}


void CGameProcessHoola::ResetFlg()
{
	m_nGameCount = 0;
	g_pGameView->m_cPlayerDrawManager.SetOpen(false);
	m_nCurPlayer = -1;	
	g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(false);

	// 상태
	m_nGameState = 0;	
	m_bGameStart = FALSE;

	m_nStartUserNum	= 0;

	m_bPlayEnding = 0;				// 엔딩 사운드 플레이
	m_nPlayEndingDelayTime = 0;		// 엔딩 사운드 플레이

	//2006.1.4 종료시 안내 메세지 그리는 부분
	m_GameEndMsgData.Clear();
	m_AfterEndingUserInfo.Clear();
	// 결과 구조체
	ZeroMemory(&m_GameOverResultData, sizeof(m_GameOverResultData));	
	m_WinnerPNumData.Clear();	

	
	g_pGameView->m_cPlayerDrawManager.SetHiddenCardShow(false);
	
	//관전자 채팅 경고 
	m_bObserverChatWarning = FALSE;
	//결과출력 Delay
	m_nResultDelayCount = 0;
	m_bResultDelayStart = FALSE;
}

// 게임 데이터를 초기화 한다.
void CGameProcessHoola::ResetGame()
{
	g_RI.State = EGAME_WAITING;

	// 플레이어 리셋
	for (int i=0; i<GetMaxNewPlayer(); i++) 
	{
		// 플레이어 리셋
		g_PLAYER(i)->Reset();	
	}

	m_cPlayScr.Reset();
	g_pGameView->GetGameButtonMan()->SetShowBtn(CGameButtonHA::BTN_RESULTOK, FALSE);

	//게임결과창 관리자 리셋
	g_pGameView->m_cGameResultViewMan.Reset();

	// ### [ 관전기능 ] ###
	GM().GetMyInfo()->JoinState = EGAME_JOINOUT;
	GM().GetMyInfo()->PlayState = EPLAYER_GAMEEND;

	m_bResultDelay = FALSE;
	m_dwResultDelaySt = 0;
	m_dwResultDelayEnd = 0;

}


// 모든 프로세스는 이곳에서 한다.
void CGameProcessHoola::OnTimer()
{
	m_nGameCount++;
	
	// 총 배팅금액 세팅	
	for (int i = 0 ; i < MAX_PLAYER; ++i)
	{
		g_PLAYER(i)->OnTimer();
	}
	
	g_pGameView->m_cGameResultViewMan.OnTimer();
	m_cPlayScr.OnTimer();
	
	if ( m_bResultDelay )
	{
		//게임 종료시 게임 결과 이펙트 딜레시 세팅
		if ( (timeGetTime() - m_dwResultDelaySt) > m_dwResultDelayEnd )
		{
			m_bResultDelay = FALSE;
			m_dwResultDelaySt = 0;
			m_dwResultDelayEnd = 0;
			
			OnGameOver(&m_GameOverResultData);
			
			if ( m_nStartBtnDelayType == 1 && g_RI.NowUserNum > 1 )
			{
				// 게임 시작 버튼 활성화
				m_cPlayScr.EnableGameStartBtn();
				m_cPlayScr.DisableGamePracticsBtn();
			}
			else if ( m_nStartBtnDelayType == 2 && g_RI.NowUserNum < 2 )
			{
				// 연습 하기 버튼 활성화
				m_cPlayScr.EnableGamePracticsBtn();
				m_cPlayScr.DisableGameStartBtn();
			}
			else if ( m_nStartBtnDelayType == 0 )
			{
				m_cPlayScr.DisableGameStartBtn();
				m_cPlayScr.DisableGamePracticsBtn();
			}
		}
	}
}

BOOL CGameProcessHoola::OnMouseMove(int x, int y)
{
	
	for ( int i = 0; i < MAX_PLAYER ; ++i)
	{			
		g_PLAYER(i)->OnMouseMove(x,y);
	}

	//훌라
	if ( m_cPlayScr.OnMouseMove( x, y ) ) return TRUE;
	
	return FALSE;
}

BOOL CGameProcessHoola::OnLButtonDblclk(int x, int y)
{
	m_cPlayScr.OnLButtonDblClick( x, y );

	return FALSE;
}

BOOL CGameProcessHoola::OnLButtonUp(int x, int y)
{
	
	for ( int i= 0 ; i < MAX_PLAYER ; i++ )
	{
		g_PLAYER(i)->OnLButtonUp(x,y);
	}
	m_cPlayScr.OnLButtonUp(x,y);
	
	return FALSE;
}

BOOL CGameProcessHoola::OnLButtonDown(int x, int y)
{

	m_cPlayer[0].OnLButtonDown(x,y);
	m_cPlayScr.OnLButtonDown(x,y);

	return FALSE;
}

void CGameProcessHoola::OnGameKey(int nVKey, int nextended, int repeat)	
{	

#ifdef _DEBUG	

	if ( nVKey == rule::SKEY_F5)
	{
		//NMBASE::SOUND::g_Sound.PlayOggStream_BGM(MUSIC_NORMAL, TRUE, (int) 95);
		//NMBASE::SOUND::g_Sound.Stop(MUSIC_NORMAL);
//		NMBASE::SOUND::g_Sound.StopAllMusic();
		m_cPlayScr.MakeTestCardDeck();
	}

	if ( nVKey == rule::SKEY_F6)
	{
		//NMBASE::SOUND::g_Sound.Stop(MUSIC_NORMAL);
//		NMBASE::SOUND::g_Sound.StopAllMusic();
//		NMBASE::SOUND::g_Sound.StopAllBGM();
//		MusicTest();
		m_cPlayScr.SetThankSelectButton();

//		NMBASE::SOUND::g_Sound.PlayOggStream_BGM(MUSIC_NORMAL, TRUE, (int) 100);
	}
	
	if ( nVKey == rule::SKEY_F8 )
	{
//		PlayVoice(false,0, DUMP);
	}
	if ( nVKey == rule::SKEY_F7 )
	{
		PlayVoice(false,0, DUMP);
// 		CMadeEffect *pObj = NULL;
// 		SAFE_NEW( pObj, CMadeEffect );
// 		pObj->Init(4);
// 		g_pGameView->m_cAniSprListManager.ADDObject(pObj);
//		ShowMainMessageDlg("게임 서버와의 접속이 끊어졌습니다");

// 		static int i = 0;
// 		m_cPlayScr.SetJackpotState( GM().GetMyInfo()->UI.ID, i );
// 		if ( i > 2)
// 		{
// 			i = 0;
// 		}
// 		else
// 		{
// 			i++;
//  		}


//	SetStopJackpotMsgTitle(NULL);
//PlayAnimation( int nPnum, eNUM_EFFECT eType )
//		static int i = 0;

//		g_pGameView->m_cEffectDrawMan.PlayAnimation( (eNUM_EFFECT)i++, 1 );
// 		g_pGameView->m_cEffectDrawMan.PlayAnimation( eNUM_EFFECT_ALLIN, i++ );
// 		if (i >= MAX_PLAYER)
// 		{
// 			i = 0;
// 		}
		NMBASE::SOUND::g_Sound.PlayOggStream( "Hoola\\Sound\\Voice\\F2\\Sound112.ogg", FALSE, false );	
	}
#endif

	//채팅창변경
	if ( nVKey == rule::SKEY_TAB ) 
	{	
		CChattingWindowMan::ENUM_CHATTING_TAB_TYPE nViewTab = g_pChatWinMan->GetViewMode();
		if (nViewTab == CChattingWindowMan::ECTT_CHAT)
			nViewTab = CChattingWindowMan::ECTT_EVENT;
		else
			nViewTab = CChattingWindowMan::ECTT_CHAT;

		g_pChatWinMan->SetViewPage(nViewTab);	
		return;
	}
	//m_cPlayScr.MakeFakeSet();

	m_cPlayer[0].OnGameKey(nVKey, nextended, repeat);

	m_cPlayScr.OnGameKey(nVKey, nextended, repeat);
}

void CGameProcessHoola::OnCheckTurnLimitTimeBase()
{
	if (g_RI.State == EGAME_WAITING)
		return;

	if (g_pGameView->m_cPlayerDrawManager.GetStartTimerTime() <= 0)
		return;

	g_pGameView->m_cPlayerDrawManager.SetCurTimerTime(timeGetTime());


}


//////////////////////////////////////////////////////////////////////////
/*
메세지 처리 
*/
//////////////////////////////////////////////////////////////////////////

// 방만들기 허가
bool CGameProcessHoola::Accept_CreateRoom(void *pMsgParam) 
{
	// 스프라이트 로딩 후 초기화 ~
	if (GetCurrentWhere() != IDX_GAMEWHERE_LOBY) 
		return false;
	
	CSV_ACCEPT_CREATEROOM *pMsg = (CSV_ACCEPT_CREATEROOM*)pMsgParam;//UGPCommon 작업

	GM().SetWaitResponse(false);
	SetAccessRoomState(false);

	memcpy(&g_RI, pMsg->RI, sizeof(ROOMINFO));
	g_pGameView->m_cTitle.MakeTitle();

	//////////////////////////////////////////////////////////////////////////
	//		초기화
	//////////////////////////////////////////////////////////////////////////

	//BTS대응 결과창 출력 문제
	m_cPlayScr.m_bShowGameResult = FALSE;
	ResetGame();
	GM().SetWaitResponse(FALSE);
	
	// 게임 정보 구조체를 초기화 한다
	for (int i = 1; i < MAX_PLAYER; ++i) 
	{
		g_PLAYER(i)->Clear();
	}

	g_PLAYER(0)->JoinState = EGAME_JOINOUT;

	//초기에 방을 만들면 자신만 존재한다
	//자신의 정보를 0번 인덱스 플레이어 정보에 초기화 시킨다
	g_PLAYER(0)->SetNewPlayer(&GM().GetMyInfo()->UI);
	g_PLAYER(0)->nSndFxKind = g_Config.SndFxKind;
	int nsnd = g_Config.SndFxKind;


	// 게임 정보 변경 해주기 
	g_MyGameInfo.nGameMode = g_RI.nGameIndex;

	// 원샷 충전
	g_cOneShotCharge.OnEnterRoom();


	// ### [ 중복된 IP가 ] ###
	g_PLAYER(0)->MyIpCheck = -1;	

	m_nNo1PosPNum = 0;
	m_nMyPosPNum = 0;	

	// 서버측 플레이어 번호 지정
	for (int i = 0; i < MAX_PLAYER; ++i)	
	{
		g_PLAYER(i)->ServPNum = i; 
	}

	memcpy(&g_PLAYER(0)->UI, &GM().GetMyInfo()->UI, sizeof(GM().GetMyInfo()->UI));

	//Buy in 맥스방 UI 머니셋팅 방만들기
	if ( g_RI.bIsMaxRoom )
	{
		//설정머니보다 자산이 많을경우 설정 머니로 셋팅한다.
		if ( g_RI.llInMoney > g_PLAYER(0)->UI.GetMoney() )
		{
			g_PLAYER(0)->UI.SetInMoney( g_PLAYER(0)->UI.GetMoney() );
			GM().GetMyInfo()->UI.SetInMoney( g_PLAYER(0)->UI.GetMoney() );
		}
		else
		{
			g_PLAYER(0)->UI.SetInMoney( g_RI.llInMoney );
			GM().GetMyInfo()->UI.SetInMoney( g_RI.llInMoney );
		}
	}

	GM().GetMyInfo()->JoinState = EGAME_JOINOUT;
	GM().GetMyInfo()->MyIpCheck = -1;
	GM().GetMyInfo()->ServPNum = g_PLAYER(0)->ServPNum;

	g_MyPN = g_MastPN = 0; // 내가 방장

	g_pGameView->m_cPlayerDrawManager.OnEnterRoom();

	m_nCurPlayer = 0; // 방처음 개설자가 선

	// 캐릭터창 초기화
	if (g_RI.MaxUserNum > MAX_PLAYER) 
		g_RI.MaxUserNum = MAX_PLAYER;
	
	//버튼들 초기화
	g_pGameView->m_cGameViewBtnMananager.EnterRoom_Button_Init( g_RI.sBetRuleKind );
	
	g_cBanishMan.Clear();
	
	g_cPromotionMan.Accept_CreateRoom();
	
	//훌라관련
	g_pGameView->PlayIntroAnimation();
	m_cPlayScr.m_bGoldState = 0;
	m_cPlayScr.m_nGoldStage = 0;
	m_cPlayScr.m_nGoldStage0 = 0;
	m_cPlayScr.m_bDrawGold = FALSE;
	m_cPlayScr.SetAllBtnClose();
	
	//잭팟 정보 초기화	
	m_cPlayScr.m_nJackpotConditionStep = 0;

	// 규제안 입장 유저의 손실머니 정보 추가.
	INT64 lossMoney = GM().GetLossMaxMoney() + GM().GetLossMoney();
	g_pGameView->m_cPlayerDrawManager.OnEnterRoomLimitMoney(0, lossMoney);

	//규제안 : 게임내 경고 문구 노출
	g_pGameView->m_cGameViewBtnMananager.SetLossMoneyWarning(true);

	return true;
}

 
// 방에 입장 허가 받음(생성된 방에 입장했을시 이 패킷이 옴)
// 나 자신이 입장 허가 받을때만 패킷이 오고 다른유저는 해당사항 없음(다른유저가 내가있는 방에 입장해도 이 패킷은 오지 않는다)
// 방 생성(Create Room) 시에도 이 패킷은 오지 않음
bool CGameProcessHoola::Accept_EnterRoom(void  *pMsgParam) 
{

	CSV_ACCEPT_ENTERROOM* pMsg = (CSV_ACCEPT_ENTERROOM*) pMsgParam;

	if ( pMsg == NULL )
	{
		CSV_ASK_OUTROOM aormsg;
		aormsg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, REASON_OUTROOM_ABNORMAL);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), aormsg.pData, aormsg.GetTotalSize());
		// 연결종료
		g_MastPN = 0;
		CloseMainMessageDlg();
		return false;
	}

	//BTS대응 결과창 출력 문제
	m_cPlayScr.m_bShowGameResult = FALSE;

	ResetGame();

	g_MyPN = 0;
	int MyID = -1;
	int MasterID = -1;

	g_RI.nGameIndex			= pMsg->EI->Ri.nGameIndex;
	g_RI.FormKind			= pMsg->EI->Ri.FormKind; 

	g_RI.cRuleType			= pMsg->EI->Ri.cRuleType; // 스톱
	g_MyGameInfo.nGameMode	= pMsg->EI->Ri.nGameIndex;

	//규제안 : 게임내 경고 문구 노출
	g_pGameView->m_cGameViewBtnMananager.SetLossMoneyWarning(true);

	for (int i = 0; i < GetMaxNewPlayer(); ++i) 
	{
		if (strlen(pMsg->EI->Ui[i].ID) > 0) 
		{
			// 방장 아이디 찾기
			if (strcmp(pMsg->EI->Ri.ID, pMsg->EI->Ui[i].ID) == 0)
			{
				MasterID = i; 
			}

			// 나의 아이디
			if (strcmp(GM().GetMyInfo()->UI.ID, pMsg->EI->Ui[i].ID) == 0)
			{
				MyID = i; 
			}
		}
	}

	// 방장 혹은 나의 아이디를 찾을 수 없거나 방장과 내 아이디가 동일하다?
	if (MasterID == -1 || MyID == -1 || MasterID == MyID) 
	{
		CSV_ASK_OUTROOM aormsg;
		aormsg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, REASON_OUTROOM_ABNORMAL);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), aormsg.pData, aormsg.GetTotalSize());
		// 연결종료
		g_MastPN = 0;
		//g_pLobyDlg->m_MsgBoxDlg.OnCancel();
		return false;
	}
	//내 전적 보이기
	GM().GetMyInfo()->SetChangeRoomInfo(pMsg->EI->Cui[MyID]);

	g_PLAYER(0)->SetNewPlayer(&GM().GetMyInfo()->UI);

	//맥스방 UI 머니셋팅 입장시
	if ( pMsg->EI->Ri.bIsMaxRoom )
	{
		//설정머니보다 자산이 많을경우 설정 머니로 셋팅한다.
		if ( pMsg->EI->Ri.llInMoney > g_PLAYER(0)->UI.GetMoney() )
		{
			g_PLAYER(0)->UI.SetInMoney( g_PLAYER(0)->UI.GetMoney() );
			GM().GetMyInfo()->UI.SetInMoney( g_PLAYER(0)->UI.GetMoney() );
		}
		else
		{
			g_PLAYER(0)->UI.SetInMoney( pMsg->EI->Ri.llInMoney );
			GM().GetMyInfo()->UI.SetInMoney( pMsg->EI->Ri.llInMoney );
		}
	}

	GM().SetWaitResponse(false);
	SetAccessRoomState(false);

	// 접속 메세지 대화창 닫기
	CloseMainMessageDlg();
	
	if (g_pGameView)
	{
		g_pGameView->m_cPlayerDrawManager.OnEnterRoom();	// 플레이어정보
	} 
	
	// 원샷 충전
	g_cOneShotCharge.OnEnterRoom();

	if (pMsg->EI->Ui[MyID].cObserver == 1)  
	{ 
		//자신을 관전자로 세팅
		g_ObserverMan.SetMyObserver(TRUE); 
	}

	GM().GetMyInfo()->SetChangeRoomInfo(pMsg->EI->Cui[MyID]);
	
	g_MastPN = MasterID;	

	// 방 정보 복사
	memcpy(&g_RI, &pMsg->EI->Ri, sizeof(ROOMINFO));	

	g_pGameView->m_cTitle.MakeTitle();
	if (g_RI.MaxUserNum > MAX_PLAYER) 
	{
		g_RI.MaxUserNum = MAX_PLAYER;
	}
	
	// 관전이 아닌 상태로 입장
	if ( !g_ObserverMan.IsMyObserver() ) 
	{
		g_PLAYER(0)->JoinState		= EGAME_JOINOUT;
		GM().GetMyInfo()->JoinState = EGAME_JOINOUT;

		m_nMyPosPNum				= MyID;					// 서버쪽에 인덱스
		m_nNo1PosPNum				= GetPNum_ByServPN(0);	// 서버쪽의 0 번 인덱스		

		// 게임 정보 초기화
		for (int i = 1; i < GetMaxNewPlayer(); ++i) 
		{
			// 위에서 자기 정보를 입력했음
			g_PLAYER(i)->Clear();
		}

		// 서버측 플레이어 번호 지정
		for (int i = 0; i < GetMaxNewPlayer(); ++i)
		{
			if ( i < MAX_PLAYER)
			{
				g_PLAYER(i)->ServPNum = GetServPNum_ByPN(i);
			}
			else
			{
				g_PLAYER(i)->ServPNum = i;
			}
		}

		GM().GetMyInfo()->ServPNum = g_PLAYER(0)->ServPNum;	

		//훌라코드
		g_PLAYER(0)->SetCardCount( 0 );
		m_cPlayScr.m_bPracticsGame = pMsg->EI->Hg.bPractics;
		m_cPlayScr.m_bGoldState = pMsg->EI->Hg.bGoldState;
		m_cPlayScr.m_nGoldStage = pMsg->EI->Hg.nGoldStage;

		if ( pMsg->EI->Ri.State && !pMsg->EI->Hg.bPractics )
		{
			//만약 연습게임이 아니고 게임이 진행중인 상태였다면
			//화면에 카드 정보가 노출되어야 한다
			m_cPlayScr.SetCardData(pMsg->EI->Hg.arrCenterCard,  pMsg->EI->Hg.arrSameNumCard);

			m_cPlayScr.m_nCurStockCnt = pMsg->EI->Hg.sBackCardCnt;
			m_cPlayScr.m_nDisCardCnt = pMsg->EI->Hg.sDisCardCnt;
			m_cPlayScr.m_DisCard[m_cPlayScr.m_nDisCardCnt-1] = ( char )pMsg->EI->Hg.sDisCardNum;

			if ( pMsg->EI->Hg.sCurMission )
			{
				m_cPlayScr.m_bMission = TRUE;
				m_cPlayScr.m_nMissionNum = pMsg->EI->Hg.sCurMission;
			}
		}

		// 플레이어 순서에 맞게 초기화한다
		for (int i = 0; i < GetMaxNewPlayer(); ++i) 
		{
			if (i < MAX_PLAYER)
			{
				//게임 방 인원에 맞게 입장한 사용자
				//이 경우가 아닌 경우가 있을까
				int nPlayerNum = GetPNum_ByServPN(i);

				//다른사용자 카드덱 정보 표시			
				if (strlen(pMsg->EI->Ui[i].ID) > 0)
				{
					if (nPlayerNum != 0) 
					{
						g_PLAYER(nPlayerNum)->SetNewPlayer(&pMsg->EI->Ui[i]);	
					}
					g_PLAYER(nPlayerNum)->SetCardCount( pMsg->EI->Hg.Ps[i].cCurCardCnt );
					g_PLAYER(nPlayerNum)->JoinState  = pMsg->EI->Hg.Ps[i].JoinState;
					g_PLAYER(nPlayerNum)->PlayState  = pMsg->EI->Hg.Ps[i].PlayState;
					g_PLAYER(nPlayerNum)->MyIpCheck  = pMsg->EI->Hg.Ps[i].nCheckIp;		// ### [ 중복된 IP가 ] ###
					g_PLAYER(nPlayerNum)->nSndFxKind = pMsg->EI->Hg.Ps[i].nSndFxKind;	// ### [사운드 추가 작업] ###
					g_PLAYER(nPlayerNum)->UI.nIcon   = GetMoneyGrade(pMsg->EI->Ui[i].GetTotalMoney());
					g_PLAYER(nPlayerNum)->SetChangeRoomInfo(pMsg->EI->Cui[i]);
					pMsg->EI->Ui[i].CopyAllGameRecords( g_PLAYER(nPlayerNum)->UI.AllGameRecord );
				}
			}
			else if (i >= MAX_PLAYER) 
			{
				//관전자 세팅
				if (strlen(pMsg->EI->Ui[i].ID) > 0)
				{
					g_PLAYER(i)->SetNewPlayer(&pMsg->EI->Ui[i]);
					if (pMsg->EI->Ui[i].AdminGrade == 0) 
					{
						// 관전자 리스트 출력
						int ounum   = g_PLAYER(i)->UI.UNum;
						int sex     = g_PLAYER(i)->UI.Sex;
						int level   = g_PLAYER(i)->UI.nIcon;
						INT64 Money = g_PLAYER(i)->GetNowMoney();
						int seq		= 0;

						CObserverDlg *pObserverDlg = GetObserverDlg();
						if (NULL != pObserverDlg)
						{
							pObserverDlg->User_InsItem(ounum, g_PLAYER(i)->UI.ID, g_PLAYER(i)->UI.NickName, sex, level, Money, seq);
						}
					}
				}
			}
		}

		for ( int i = 0; i < MAX_PLAYER; i++ )
		{
			if ( g_PLAYER(i)->UI.UNum == 0 ) 
				continue;

			int nServPNum = GetServPNum_ByPN( i );

			if ( !m_cPlayScr.m_bPracticsGame )
			{
				if ( pMsg->EI->Hg.Ps[ nServPNum ].bRegister ) 
				{
					g_PLAYER(i)->m_bRegist = TRUE;
				}
				else			
					g_PLAYER(i)->m_bRegist = FALSE;
			}
			else
			{
				g_PLAYER(i)->m_bRegist = FALSE;
			}
		}

		CString str;
		//게임방 내부 입장 메시지
		str.Format("## <%s>님 입장 ##\n", g_PLAYER(0)->UI.NickName);
		AddGameInfoViewEdit(str.GetString(), RGB(214,214,214));//Color InOutColor

		//로비 입장 메시지
		str.Format("## <ID: %s(%s)>님 입장 ##\n",  g_PLAYER(0)->UI.ID ,  g_PLAYER(0)->UI.NickName);
		AddLobbyHideChatText( &str );

		g_PLAYER(0)->nRoundPrevMoney = g_PLAYER(0)->UI.GetMoney();

		m_cPlayScr.SetAllBtnClose();

		PlayMusic();
	}
	//관전자로 입장
	else 
	{
		// 서버 클라이언트 1 : 1		
		m_nMyPosPNum  = 0;					 // 서버쪽에 인덱스
		m_nNo1PosPNum = GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스

		for (int i = 0; i < GetMaxNewPlayer(); ++i) 
		{
			g_PLAYER(i)->Clear(); // 게임 정보 초기화
			g_PLAYER(i)->ServPNum = i;
		}

		GM().GetMyInfo()->ServPNum = MyID;

		//훌라코드
		m_cPlayScr.m_bPracticsGame = pMsg->EI->Hg.bPractics;

		if ( pMsg->EI->Ri.State && !pMsg->EI->Hg.bPractics )
		{
//			연습게임이 진행중이지 않고 게임이 진행중인 상태라면

			m_cPlayScr.SetCardData(pMsg->EI->Hg.arrCenterCard,  pMsg->EI->Hg.arrSameNumCard);

			m_cPlayScr.m_nCurStockCnt = pMsg->EI->Hg.sBackCardCnt;
			m_cPlayScr.m_nDisCardCnt = pMsg->EI->Hg.sDisCardCnt;
			m_cPlayScr.m_DisCard[m_cPlayScr.m_nDisCardCnt-1] = ( char )pMsg->EI->Hg.sDisCardNum;

			if ( pMsg->EI->Hg.sCurMission )
			{
				m_cPlayScr.m_bMission = TRUE;
				m_cPlayScr.m_nMissionNum = pMsg->EI->Hg.sCurMission;
			}
		}

		for (int i = 0; i < GetMaxNewPlayer(); ++i)
		{
			//관전자는 화면에 표시 되지 않으므로 0번 인덱스는 방장이 갖는다. 그래서 서버 PN을 사용해서 카드덱을 세팅한다.
			int nPlayerNum = i;

			if (strlen(pMsg->EI->Ui[i].ID) > 0)	
			{
				g_PLAYER(nPlayerNum)->SetNewPlayer(&pMsg->EI->Ui[i]);

				if ( i >= MAX_PLAYER ) // 관전자 리스트 출력
				{
					if (pMsg->EI->Ui[i].AdminGrade == 0) 
					{
						int ounum = g_PLAYER(i)->UI.UNum;
						int sex  = g_PLAYER(i)->UI.Sex;
						int level = g_PLAYER(i)->UI.nIcon;
						INT64 Money = g_PLAYER(i)->GetNowMoney();
						int seq = 0;

						CObserverDlg *pObserverDlg = GetObserverDlg();
						if (NULL != pObserverDlg)
						{
							pObserverDlg->User_InsItem(ounum,g_PLAYER(i)->UI.ID, g_PLAYER(i)->UI.NickName, sex, level,Money, seq);
						}
					}
					continue;
				}

				if ( m_cPlayScr.m_bPracticsGame )
				{
					g_PLAYER(nPlayerNum)->JoinState = 0;
					g_PLAYER(nPlayerNum)->PlayState = 0;
					g_PLAYER(nPlayerNum)->SetCardCount( 0 );
				}
				else
				{
					g_PLAYER(nPlayerNum)->JoinState = pMsg->EI->Hg.Ps[i].JoinState;
					g_PLAYER(nPlayerNum)->PlayState = pMsg->EI->Hg.Ps[i].PlayState;
				}
				g_PLAYER(nPlayerNum)->MyIpCheck		= pMsg->EI->Hg.Ps[i].nCheckIp; // ### [ 중복된 IP가 ] ###
				g_PLAYER(nPlayerNum)->nSndFxKind    = pMsg->EI->Hg.Ps[i].nSndFxKind;
				g_PLAYER(nPlayerNum)->UI.nIcon		= GetMoneyGrade(pMsg->EI->Ui[i].GetTotalMoney());
				g_PLAYER(nPlayerNum)->SetChangeRoomInfo(pMsg->EI->Cui[i]);

				// 게임 진행 상황 그릴것 카드 정보를 카드덱에다가 셋
				if ( g_PLAYER(nPlayerNum)->JoinState == 1 )
				{
					 g_PLAYER(nPlayerNum)->SetCardCount( pMsg->EI->Hg.Ps[i].cCurCardCnt );

					if ( pMsg->EI->Hg.Ps[i].bRegister ) 
					{
						g_PLAYER(nPlayerNum)->m_bRegist = TRUE;
					}
					else
					{
						g_PLAYER(nPlayerNum)->m_bRegist = FALSE;
					}
				}
			}
		}
		//관전UI
		g_ObserverMan.OnObserve();

		CString str;
		str.Format("## <%s>님 입장[관전중]##\n", GM().GetMyInfo()->UI.NickName );
		AddGameInfoViewEdit( str.GetString(), RGB(100,255,100) );

		// 버튼 상태 설정
		g_PLAYER(0)->nRoundPrevMoney = g_PLAYER(0)->UI.GetMoney();

		m_cPlayScr.SetAllBtnClose();

		PlayMusic();
	}
	
	//버튼들 초기화
	if (g_pGameView)
	{
		g_pGameView->m_cGameViewBtnMananager.EnterRoom_Button_Init( g_RI.sBetRuleKind );
	}

	m_cPlayScr.m_bGoldState = pMsg->EI->Hg.bGoldState;
	m_cPlayScr.m_nGoldStage = pMsg->EI->Hg.nGoldStage;
	if ( m_cPlayScr.m_bGoldState )
	{
		m_cPlayScr.m_nGoldStage0 = m_cPlayScr.m_nGoldStage;
		if ( pMsg->EI->Ri.State )//게임 진행 정보 처리는 어떻게???
		{
			m_cPlayScr.m_bDrawGold = TRUE;
		}
	}
	else
	{
		m_cPlayScr.m_nGoldStage0 = 1;
	}

	NMBASE::SOUND::g_Sound.PlayWav( SND57 );

	if ( !pMsg->EI->Hg.bPractics )
	{
		for ( int i = 0; i < MAX_PLAYER; i++ )
		{
			if ( strcmp( g_PLAYER(i)->UI.ID, "_PRACTICSUSER_" ) == 0 )
			{
				g_PLAYER(i)->Clear();	
				g_RI.NowUserNum--;
			}
		}
	}

	if (g_pGameView)
	{
		g_pGameView->m_cTitle.MakeTitle();
	}

	g_cPromotionMan.Accept_EnterRoom();

	// 게임화면에 포커스를 준다.
	g_pGameView->SetFocus();
	m_cPlayScr.m_nJackpotConditionStep = 0;

	return true;
}

// 유저가 방에 입장 알림
// 방 생성(Create room) 시에는 오지 않음
void CGameProcessHoola::User_EnterRoom(void *pMsgParam)
{
	CSV_USERENTERROOM* pMsg= (CSV_USERENTERROOM*) pMsgParam;//UGPCommon 작업

	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
	{
		return;
	}
	
	PBPlayEffectSound(SND_USERIN);
	int pnum = -1; // 플레이어 번호
	
	// 자신(게임참여중) / 상대방(게임참여 입장)
	if ( !g_ObserverMan.IsMyObserver() && pMsg->UI->cObserver != 1 ) 
	{
		// 서버쪽의 0번인덱스 클라이언트 인덱스 부터
		for (int i = m_nNo1PosPNum; i < MAX_PLAYER + m_nNo1PosPNum; ++i) 
		{
			int index = i;

			if (index >= MAX_PLAYER) 
				index = index - MAX_PLAYER;
			
			if (index == 0)
				continue; //나
			
			if (strlen(g_PLAYER(index)->UI.ID) == 0)
			{
				pnum=index;
				break;
			}
		}

		// 비어있는 곳을 찾지 못했으면
		if (pnum == -1)
		{
			return; 
		}

		// 방 정보를 수정하고 새로 참여한 플레이어의 정보를 저장
		g_RI.NowUserNum += 1;
		g_PLAYER(pnum)->SetNewPlayer(pMsg->UI);

		g_pGameView->m_cPlayerDrawManager.OnEnterRoom(pnum);

		// 규제안 입장 유저의 손실머니 정보 추가.
		g_pGameView->m_cPlayerDrawManager.OnEnterRoomLimitMoney(pnum, *pMsg->llMaxLossMoney);	

		g_PLAYER(pnum)->ServPNum		= GetServPNum_ByPN(pnum);		// 서버측 플레이어 번호 저장

		CString str;
		str.Format("▶입장: [%s]님\n", pMsg->UI->NickName);		
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
	}
	// 자신(관전중) / 상대방(게임참여 입장)
	else if (pMsg->UI->cObserver != 1)
	{
		for (int i = 0; i < MAX_PLAYER; ++i) 
		{
			if (strlen(g_PLAYER(i)->UI.ID) == 0)
			{
				pnum=i; 
				break;
			}
		}

		// 비어있는 곳을 찾지 못했으면
		if (pnum == -1) 
		{
			return; 
		}

		m_nMyPosPNum  = 0; // 서버쪽에 인덱스
		m_nNo1PosPNum = GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스

		// 방 정보를 수정하고 새로 참여한 플레이어의 정보를 저장
		g_RI.NowUserNum += 1;
		g_PLAYER(pnum)->SetNewPlayer(pMsg->UI);	

		g_pGameView->m_cPlayerDrawManager.OnEnterRoom(pnum);

		// 규제안 입장 유저의 손실머니 정보 추가.
		g_pGameView->m_cPlayerDrawManager.OnEnterRoomLimitMoney(pnum, *pMsg->llMaxLossMoney);	

		g_PLAYER(pnum)->ServPNum		= GetServPNum_ByPN(pnum);		// 서버측 플레이어 번호 저장

		CString str;
		str.Format("▶입장: [%s]님\n", pMsg->UI->NickName);		
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
	}
	// 자신(관전중) / 상대방(관전 입장)
	else
	{
		if (pMsg->UI->AdminGrade > 0) 
		{
			pnum = GetMaxNewPlayer() - 1; //관리자
		}
		else 
		{
			for (int i = MAX_PLAYER; i < GetMaxNewPlayer() - 1; ++i)
			{
				if (strlen(g_PLAYER(i)->UI.ID) == 0)
				{
					pnum=i;
					break;
				}
			}
		}

		// 비어있는 곳을 찾지 못했으면
		if (pnum == -1)
		{
			return;
		}

		g_PLAYER(pnum)->SetNewPlayer(pMsg->UI);		// 방 정보를 수정하고 새로 참여한 플레이어의 정보를 저장
		g_PLAYER(pnum)->ServPNum		= pnum;		// 서버측 플레이어 번호 저장

		// 관리자가 아니면
		if ( pMsg->UI->AdminGrade == 0)
		{ 
			g_RI.NowObserverNum += 1;
			
			// 관전자 리스트 출력
			int ounum = g_PLAYER(pnum)->UI.UNum;
			int sex  = g_PLAYER(pnum)->UI.Sex;
			int level = g_PLAYER(pnum)->UI.nIcon;
			INT64 Money = g_PLAYER(pnum)->UI.GetMoney();
			int seq = 0;

			CObserverDlg *pObserverDlg = GetObserverDlg();

			if (NULL != pObserverDlg)
			{
				pObserverDlg->User_InsItem(ounum,g_PLAYER(pnum)->UI.ID, g_PLAYER(pnum)->UI.NickName, sex, level, Money, seq);
			}

			CString str;		
			str.Format("▶관전 입장: [%s]님\n", pMsg->UI->NickName);
			AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
		}
	}

	// 중복된 IP 체크
	for (int i = 0; i < MAX_PLAYER; ++i) 
	{
		int nPlayerNum = GetPNum_ByServPN(i);

		if (strlen(g_PLAYER(nPlayerNum)->UI.ID) > 0) 
		{
			g_PLAYER(nPlayerNum)->MyIpCheck = pMsg->IPA->IpIndex[i];
		}
	}

	if ( strcmp( pMsg->UI->ID, "_PRACTICSUSER_") != 0 )
		g_PLAYER(pnum)->nSndFxKind = *pMsg->SndFxKind;// ### [사운드 추가 작업] ###
	else
		g_PLAYER(pnum)->nSndFxKind = -1;
	
	g_pGameView->m_cTitle.MakeTitle();

	// 전광판에 내용 출력
	if (!g_ObserverMan.IsMyObserver())
	{
		CheckMinimumUser();
	}
}

// 유저가 방에서 나감 알림
void CGameProcessHoola::User_OutRoom(void *pMsgParam) 
{
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	CSV_USEROUTROOM* pMsg = (CSV_USEROUTROOM*) pMsgParam;//UGPCommon 작업

	PBPlayEffectSound(SND_USEROUT);

	//나간 사용자  Pnum을 받아온다
	int pnum=-1;

	for (int i=0; i<GetMaxNewPlayer(); i++) 
	{
		if (strcmp(g_PLAYER(i)->UI.ID, pMsg->ID)==0)
		{
			pnum=i;
			break;
		}
	}

	if (pnum == -1)
	{
		return;
	}

	// 닉네임수정 
	CString NickName =  g_PLAYER(pnum)->UI.NickName;


	// 만약 사용자가 게임중인 유저 였다면
	if ( pnum < MAX_PLAYER ) 
	{
		// [ 카드 날리기 ]
		if (g_PLAYER(pnum)->bFold == FALSE && g_PLAYER(pnum)->JoinState == EGAME_JOININ && g_PLAYER(pnum)->PlayState == EPLAYER_GAMEPLAYING)
		{	//현재 겜중인 넘 나갔다.
			m_nStartUserNum--;
			if (m_nStartUserNum <0)
			{
				m_nStartUserNum = 0;
			}
		}

		if ( g_pGameView->m_cPlayerDrawManager.IsExitReservation(pnum) )
		{
			//	NMBASE::SOUND::g_Sound.PlayWav(SND_FX_2);
		}

		g_PLAYER(pnum)->Clear(); // UI 초기화

		g_pGameView->m_cPlayerDrawManager.OnExitRoom(pnum);

		if (g_RI.NowUserNum > 0) 
		{
			g_RI.NowUserNum -= 1;
		}

		CString str;		
		str.Format("◁퇴장: [%s]님\n", NickName.GetString() );		
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

		
		str.Format("## <ID: %s(%s)>님 퇴장 ##\n",pMsg->ID , NickName.GetBuffer() );		
		AddLobbyHideChatText(&str);
	}
	// 관전 중인 유저
	else 
	{
		if (g_PLAYER(pnum)->UI.AdminGrade == 0)
		{			
			CObserverDlg *pObserverDlg = GetObserverDlg();
			if (NULL != pObserverDlg)
			{
				pObserverDlg->User_DelItem(g_PLAYER(pnum)->UI.UNum, g_PLAYER(pnum)->UI.ID);
			}

			if (g_RI.NowObserverNum > 0) 
			{
				g_RI.NowObserverNum -= 1;
			}
			

			CString str;		
			str.Format("◁관전 퇴장:[%s]님\n", g_PLAYER(pnum)->UI.NickName);		
			AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

		}
		g_PLAYER(pnum)->Clear(); // UI 초기화
//		NMBASE::SOUND::g_Sound.PlayWav(SND_FX_2);
	}

	// 중복된 IP 체크
	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		int nPlayerNum = GetPNum_ByServPN(i);
		if (strlen(g_PLAYER(nPlayerNum)->UI.ID) > 0) 
		{
			g_PLAYER(nPlayerNum)->MyIpCheck = pMsg->IPA->IpIndex[i];
		}
	} 

	if ( TRUE == m_cPlayScr.m_bPracticsGame )
	{
		if ( g_RI.NowUserNum <= 2 )
		{
			m_cPlayScr.DisableGameStartBtn();
		}
	}

	g_pGameView->m_cTitle.MakeTitle();

	// 전광판에 내용 출력
	if (!g_ObserverMan.IsMyObserver())
	{
		CheckMinimumUser();
	}
}

void CGameProcessHoola::SetGoldStage(bool bGoldState, int nGoldStage)
{
	// 골드판 관련 메소드 옮기는 작업
	m_cPlayScr.m_bGoldState = bGoldState;
	m_cPlayScr.m_nGoldStage = nGoldStage;
	if ( m_cPlayScr.m_bGoldState )
	{
		if ( m_cPlayScr.m_nGoldStage == 2 )
		{
			m_cPlayScr.m_nGoldStage0 = 2;
		}
		else if ( m_cPlayScr.m_nGoldStage0 == 2 && m_cPlayScr.m_nGoldStage == 4 )
		{
			m_cPlayScr.m_nGoldStage0 = 3;
		}
		else if ( m_cPlayScr.m_nGoldStage0 == 4 && m_cPlayScr.m_nGoldStage == 4 )
		{
			m_cPlayScr.m_nGoldStage0 = 4;
		}
	}// if ( m_cPlayScr.m_bGoldState )
	else
	{
		if ( m_cPlayScr.m_nGoldStage0 == 2 )
		{
			m_cPlayScr.m_nGoldStage0 = 5;
			m_cPlayScr.m_bEndGoldStage = TRUE;
			m_cPlayScr.m_bPlayMusicAfter = TRUE;
		}
		else if ( m_cPlayScr.m_nGoldStage0 == 4 )
		{
			m_cPlayScr.m_nGoldStage0 = 6;
			m_cPlayScr.m_bEndGoldStage = TRUE;
			m_cPlayScr.m_bPlayMusicAfter = TRUE;
		}
		else
		{
			m_cPlayScr.m_nGoldStage0 = 1;
		}
	}// if ( !m_cPlayScr.m_bGoldState )
}

void CGameProcessHoola::StopResultDelay()
{
	m_bResultDelay = FALSE;
	m_dwResultDelaySt = 0;
	m_dwResultDelayEnd = 0;
}
void CGameProcessHoola::SetResultDelay(DWORD dwTime)
{
	m_bResultDelay = TRUE;
	m_dwResultDelaySt = timeGetTime();
	m_dwResultDelayEnd = dwTime;
}

// 게임 오버
void CGameProcessHoola::OnGameOver(GAMEOVERRESULT *pGOR)
{
	if ( FALSE == m_cPlayScr.IsMyPracticeGameObserver() )
		g_pGameView->m_cGameResultViewMan.OnGameOver(pGOR);	

	m_cPlayScr.SetButtonGameOver();	//게임오버 상황에서의 버튼으로 세팅
	m_cPlayScr.ResetLimitTime();		//지난시간 표시 변경

	SetGoldStage(pGOR->Hg.bGoldHoola,pGOR->Hg.sGoldStage);

	if ( pGOR->Hg.sWinnerCase < 0 )
	{
		//정상적인 값이 들어오지 않음
		return;		
	}
	if ( pGOR->Hg.sWinnerCase == 0 )
	{
		//이경우도 정상적이지 않은듯. 
		//어떻게 처리할거?
		m_cPlayScr.Reset();
		g_RI.State = EGAME_WAITING;
	}

	m_cPlayScr.m_nTurnIndex = -1;
	m_cPlayScr.ResetLimitTime();
	g_RI.State = EGAME_WAITING;

	m_cPlayScr.m_dwHelpTime = timeGetTime();
	m_cPlayScr.m_nHelpIndex = 8;
	
	m_cPlayScr.ResetLimitTime();

	for (int i=0; i<MAX_PLAYER; i++)
	{
		//게임 머니 전적등의 변동사항과 상태 정보를 저장
		if ( strlen( pGOR->Ui[ i ].ID ) > 0 )
		{			
			int pnum = GetPlayerPNum( pGOR->Ui[ i ].ID );
			if ( pnum == -1 )
			{
				continue;
			}
			//m_GameOverResultData에 데이터 저장해서 처리하게 해놓음
			g_PLAYER(pnum)->m_n64ResultMoney = m_GameOverResultData.Ui[i].nResultMoney;		
			memcpy( &g_PLAYER(pnum)->UI.AllGameRecord, &m_GameOverResultData.Ui[i].AllGameRecord, sizeof(m_GameOverResultData.Ui[i].AllGameRecord) );
			g_PLAYER(pnum)->SetChangeRoomInfo(m_GameOverResultData.Cui[i]);
			g_PLAYER(pnum)->PlayState = EPLAYER_GAMEEND;


			if (pnum == 0 && !g_ObserverMan.IsMyObserver())
			{								
				g_PLAYER(pnum)->UI.CopyAllGameRecords( GM().GetMyInfo()->UI.AllGameRecord );
				GM().GetMyInfo()->SetChangeRoomInfo(*g_PLAYER(pnum)->GetChangeRoomInfo());
				GM().GetMyInfo()->UI.SetMoney( g_PLAYER(pnum)->UI.GetMoney() ); 
				GM().GetMyInfo()->UI.nIcon			= g_PLAYER(pnum)->UI.nIcon;						
			}
		}

	}

	BOOL bPracticeGameObserver = FALSE;
	BOOL bPracticeGame = FALSE;
	for ( int i = 0 ; i < MAX_PLAYER ; i++ )
	{
		if ( strcmp(pGOR->Ui[i].ID, "_PRACTICSUSER_" ) == 0 )
		{
			bPracticeGame = TRUE;
		}
	}

	if (g_ObserverMan.IsMyObserver() && bPracticeGame)
		bPracticeGameObserver = TRUE;
	else
		bPracticeGameObserver = FALSE;

	if ( FALSE == bPracticeGameObserver )
	{	
		for ( int i = 0 ; i < MAX_PLAYER ; i++ )
		{
			if ( strlen( pGOR->Ui[i].ID ) > 0 )
			{		
				GAMEOVERINFO *pGOI = NULL;
				pGOI = &pGOR->Ui[ i ];
				int nPN = GetPNum_ByServPN(i);

				// 스톱박 표시
				if ( pGOI->bStopBak )
				{
					g_pGameView->m_cEffectDrawMan.PlayAnimation( eNUM_EFFECT_STOP_BAK, pGOR->Ui[i].ID );
					PlayVoice( m_nArrSndData[nPN].bSex,  m_nArrSndData[nPN].nSndFxKind, STOPBAK );
				}
				if ( pGOI->bThankYouBak )
				{
					nPN = GetPNum_ByServPN(i);
					g_pGameView->m_cEffectDrawMan.PlayAnimation( eNUM_EFFECT_THANK_BAK, pGOR->Ui[i].ID );
					PlayVoice( m_nArrSndData[nPN].bSex,  m_nArrSndData[nPN].nSndFxKind, THANKBAK );
				}
			}
		}
	}
	
	for ( int i = 0; i < MAX_PLAYER; i++ )
	{
		if (strlen(pGOR->Ui[i].ID)>0)
		{		
			GAMEOVERINFO *pGOI = NULL;
			pGOI = &pGOR->Ui[ i ];

			CString str, str1;
			if ( pGOI->nResultMoney > 0 )
			{
				str = "------------------------------\n";
				AddGameInfoViewEdit(str.GetString(), RGB(255,255,255));

				str.Format( "[%s]님 승리\n", pGOI->NickName );
				AddGameInfoViewEdit(str.GetString(), RGB(255,255,255));

				str.Format( "획득금액 %s\n", (LPCTSTR)NMBASE::UTIL::g_NumberToHangul( pGOI->nResultMoney ));
				AddGameInfoViewEdit(str.GetString(), RGB(255,253,161));

				str = "------------------------------\n";
				AddGameInfoViewEdit(str.GetString(), RGB(255,253,161));
			}
		}
	}

	for ( int i = 0; i < MAX_PLAYER; i++ )
	{
		if (strlen(pGOR->Ui[i].ID)>0)
		{		
			GAMEOVERINFO *pGOI = NULL;
			pGOI = &pGOR->Ui[ i ];

			CString str, str1;
			if ( pGOI->nResultMoney <= 0 )
			{
				int nBak = 0;
				str.Format( "[%s]님 %s\n", pGOI->NickName, (LPCTSTR)NMBASE::UTIL::g_NumberToHangul( pGOI->nResultMoney ) );
				AddGameInfoViewEdit(str.GetString(), RGB(203,203,203));			
			}
		}
	}

	// 게임이 종료되고 경과에 해당하는 카드 정보를 세팅한다.
	m_cPlayScr.SetResultCard();
	m_cPlayScr.SetGameOver( TRUE );
}

//G_TFT
void CGameProcessHoola::Change_Money(CSV_CHANGE_MONEY *pMsg)
{	
	const char *pUserID = pMsg->ID;
	const char *pUMCode = pMsg->UMCode;
	INT64 ChangeMoney = *pMsg->ChangeMoney;
	INT64 PMoney = *pMsg->PMoney;

	//pUserID		: 머니 변화가 일어난 유저아이디 (내가 될수도 있고 내가 있는 방의 다른 유저가 될 수도있다.)
	//pUMCode		: 머니 변화가 일어난 메타코드
	//ChnageMoney	: 변화된 머니량
	//PMoney		: 변화 후 머니 

	// 자신의 머니 처리
	if (strcmp(pUserID, GM().GetMyInfo()->UI.ID) == 0) 
	{
		/*GM().GetMyInfo()->UI.SetMoney(PMoney);*/
		GM().GetMyInfo()->UI.SetRealPMoney(PMoney);
		if ( !g_ObserverMan.IsMyObserver() ) 
		{
			//g_PLAYER(0)->UI.SetMoney(PMoney);
			g_PLAYER(0)->UI.SetRealPMoney(PMoney);
			//g_PLAYER(0)->PrevMoney = g_PLAYER(0)->UI.GetPMoney();//PMONEY , PMONEY2 통합시에 아래 살리기 //@#$%
		}
	}
	// 상대방 머니 처리
	else	
	{
		int pnum = g_GAMEPROCESS()->GetPlayerPNum(pUserID);
		if (pnum != -1) 
		{								
			/*g_PLAYER(pnum)->UI.SetMoney(PMoney);*/
			g_PLAYER(pnum)->UI.SetRealPMoney(PMoney);	
		}
	}


	CString strUMCode = pUMCode;
	/////////////////////////////////////////////////////////////
	//각 케이스별로 애니메이션 효과 , 팝업 등을 보여준다.
	// 뿌려줄 메세지도 설정한다.
	CString sysmsg;	

	if ( strUMCode == "ABB02019" || strUMCode == "ABB02020" || strUMCode == "ABB02021" ) //원샷(재기)충전 메타코드 
	{
		//원샷(재기)충전
		g_cOneShotCharge.OnChargeMoney(*pMsg);
	}


	if (!sysmsg.IsEmpty())
	{
		if (GetCurrentWhere()==IDX_GAMEWHERE_LOBY)
		{
			//UPG작업 추가
			AddLobbyChatViewEdit(sysmsg, RGB(0,44,93));
		}

		if (GetCurrentWhere()==IDX_GAMEWHERE_GAME)
		{			
			AddGameInfoViewEdit(sysmsg.GetString(), GetChatColor(ECC2_SERVMSG));
		}
	}
	if (GetCurrentWhere()==IDX_GAMEWHERE_LOBY){
		MM().Call(UPG_CMK_RESET_GAMEMONEY, 0, 0);
	}
}


// bPaticipation = TRUE  관전 -> 참여
// bPaticipation = FALSE 참여 -> 관전
void CGameProcessHoola::SetPlayInfo(int nowpnum, int newpnum, BOOL bParticipation )
{
	//백업용 - 포인터 사용할까?
	USERINFO  tmpUi[MAX_PLAYER];
	PLAYSTATE tmpPs[MAX_PLAYER] = {0,};
	BACKCARD  tmpBC[MAX_PLAYER] = {0,};
	CHANGE_USERINFO_ROOM tmpCUi[MAX_PLAYER];

	ZeroMemory(tmpUi, sizeof(USERINFO) * MAX_PLAYER);
	ZeroMemory(tmpCUi, sizeof(CHANGE_USERINFO_ROOM) * MAX_PLAYER);

	typedef struct _tagtempcarddeck
	{
		char nCard[10];
	}STempCardDeck;

	bool			tmpSelectCard[MAX_PLAYER] = {0,};
	STempCardDeck	tCardDeck[MAX_PLAYER]	  = {0,};	

	if ( !bParticipation ) 
	{
		//현재 겜참여 중이기 때문에 0 번 인덱스
		//기본 정보 초기화
		m_cPlayer[0].Clear();
	}

	// 기존 플레이 정보 backup
	int nIndex = 0;
	for (int i = 0; i < MAX_PLAYER; ++i) 
	{
		if (strlen(m_cPlayer[i].UI.ID) == 0) 
		{
			continue;
		}

		if (bParticipation )// 관전 -> 참여
			nIndex = i;
		else
			nIndex = GetServPNum_ByPN(i);

		memcpy(&tmpUi[nIndex], &m_cPlayer[i].UI, sizeof(USERINFO));
		memcpy(&tmpCUi[nIndex], m_cPlayer[i].GetChangeRoomInfo(), sizeof(CHANGE_USERINFO_ROOM) );
		tmpPs[nIndex].JoinState = m_cPlayer[i].JoinState;
		tmpPs[nIndex].PlayState = m_cPlayer[i].PlayState;
		tmpPs[nIndex].bFold		= m_cPlayer[i].bFold;
		tmpPs[nIndex].nCardTotal= m_cPlayer[i].GetCardCount();

		//서버로부터 다시 받는다
		// ### [ 중복된 IP가 ] ###
		//tmpPs[nIndex].nCheckIp = m_cPlayer[i].MyIpCheck;
		// ### [사운드 추가 작업] ###
		tmpPs[nIndex].nSndFxKind = m_cPlayer[i].nSndFxKind;
		tmpSelectCard[nIndex] = g_pGameView->m_cPlayerDrawManager.IsSelectCard(i);
	}

	//기본 정보 초기화
	if (bParticipation)
	{   // 관전 -> 참여
		GM().GetMyInfo()->JoinState = EGAME_JOINOUT;
		m_nMyPosPNum = newpnum; // 서버쪽에 새로운 인덱스
		m_nNo1PosPNum = GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스

		m_cPlayer[nowpnum].Clear();
	}
	else 
	{	//참여 -> 관전
		GM().GetMyInfo()->JoinState = EGAME_JOINOUT;
		m_nMyPosPNum = 0; // 서버쪽에 새로운 인덱스
		m_nNo1PosPNum = GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스
	}

	g_pGameView->m_cPlayerDrawManager.OnExitRoom();

	// 게임 정보 모두 초기화
	for (int i=0; i<MAX_PLAYER; ++i) 
	{
		m_cPlayer[i].Clear();
		m_cPlayer[i].ServPNum = GetServPNum_ByPN(i); // 초기화 되지 않는다.
		m_cPlayer[i].InitRoomData();
	}

	// 관전 -> 참여
	if (bParticipation)
	{	
		GM().GetMyInfo()->UI.cObserver = 0;
		g_ObserverMan.SetMyObserver(FALSE);
		GM().GetMyInfo()->Reset();
		GM().GetMyInfo()->nSndFxKind = g_Config.SndFxKind;
		m_cPlayer[0].SetNewPlayer(&GM().GetMyInfo()->UI);
		m_cPlayer[0].SetChangeRoomInfo(*GM().GetMyInfo()->GetChangeRoomInfo());
		m_cPlayer[0].nSndFxKind = g_Config.SndFxKind;

		g_RI.NowUserNum++;
		if (g_RI.NowObserverNum > 0) 
		{
			g_RI.NowObserverNum--;
		}

		GM().GetMyInfo()->ServPNum  = m_cPlayer[0].ServPNum;

		//관전자 이미지 그리기 삭제
		g_pGameView->m_cStayAniSprListManager.Del_AniListOBj(ANF_PLAYER_OBSERVER);		
	}
	// 참여 -> 관전
	else 
	{   
		GM().GetMyInfo()->UI.cObserver = 1;
		g_ObserverMan.SetMyObserver(TRUE);
		GM().GetMyInfo()->Reset();
		m_cPlayer[newpnum].SetNewPlayer(&GM().GetMyInfo()->UI);
		m_cPlayer[newpnum].SetChangeRoomInfo(*GM().GetMyInfo()->GetChangeRoomInfo());
		m_cPlayer[newpnum].nSndFxKind = GM().GetMyInfo()->nSndFxKind;

		GM().GetMyInfo()->ServPNum = newpnum;
		m_cPlayer[newpnum].ServPNum = newpnum;

		g_RI.NowObserverNum++;
		if (g_RI.NowUserNum > 0) 
			g_RI.NowUserNum--;
		
		//관전UI		
		g_ObserverMan.OnObserve();
	}

	// 플레이어 순서에 맞게  카드를 초기화한다
	for (int i = 0; i < MAX_PLAYER; ++i) 
	{
		int nServerToClientIndex = GetPNum_ByServPN(i);
		if (strlen(tmpUi[i].ID) > 0)	
		{
			if (bParticipation ) 
			{
				if (nServerToClientIndex != 0) 
				{
					m_cPlayer[nServerToClientIndex].SetNewPlayer(&tmpUi[i]);
				}
			}
			else
			{
				m_cPlayer[nServerToClientIndex].SetNewPlayer(&tmpUi[i]);
			}

			m_cPlayer[nServerToClientIndex].JoinState		= tmpPs[i].JoinState;
			m_cPlayer[nServerToClientIndex].PlayState		= tmpPs[i].PlayState;			
			m_cPlayer[nServerToClientIndex].nSndFxKind		= tmpPs[i].nSndFxKind; // ### [사운드 추가 작업] ###			
			m_cPlayer[nServerToClientIndex].UI.nIcon		= GetMoneyGrade(tmpUi[i].GetTotalMoney());
			m_cPlayer[nServerToClientIndex].SetChangeRoomInfo(tmpCUi[i]);

			//훌라코드
			m_cPlayer[nServerToClientIndex].SetCardCount( tmpPs[i].nCardTotal );

			g_pGameView->m_cPlayerDrawManager.SetSelectCard(nServerToClientIndex, tmpSelectCard[i]);
		}
	}

	for (int i = 0; i < MAX_PLAYER; ++i) 
	{
		if (m_cPlayer[i].UI.ID[0] != 0)	
		{
			g_pGameView->m_cPlayerDrawManager.OnEnterRoom(i);
		}
	}

	// bPaticipation = TRUE  관전 -> 참여
	// bPaticipation = FALSE 참여 -> 관전
	if (bParticipation) 
	{
		CObserverDlg *pObserverDlg = GetObserverDlg();
		if (pObserverDlg)
		{
			pObserverDlg->User_DelItem(m_cPlayer[0].UI.UNum, m_cPlayer[0].UI.ID);
		}
	}
	else
	{
		int ounum	= m_cPlayer[newpnum].UI.UNum;
		int sex		= m_cPlayer[newpnum].UI.Sex;
		int level	= m_cPlayer[newpnum].UI.nIcon;
		INT64 Money = m_cPlayer[newpnum].UI.GetMoney();
		int seq		= 0;

		CObserverDlg *pObserverDlg = GetObserverDlg();
		if (pObserverDlg)
		{
			pObserverDlg->User_InsItem(ounum,m_cPlayer[newpnum].UI.ID, m_cPlayer[newpnum].UI.NickName, sex, level, Money, seq);
		}
	}

	CString str;
	if (bParticipation) 
	{
		str.Format("관전 → 참여\n");
	}
	else 
	{
		str.Format("참여 → 관전\n");
	}
	AddGameInfoViewEdit(str.GetString(), RGB(153,153,255));	
}

BOOL CGameProcessHoola::IsPlayingNow()
{
	return m_bGameStart;
}

//UPG작업 추가
void CGameProcessHoola::OnEnterRoom(BOOL bCreation)
{
	//내가 방에 들어간다 (bCreation은 방을 생성해서 입장하는지 여부)
	CString str;	
	
	#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
		if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
			str.Format("톡톡 훌라");
		else
			str.Format("윈조이 훌라");
	#else //UPGRADE_20120308_TOCTOC_CHANNELING
		str.Format("윈조이 훌라");
	#endif //UPGRADE_20120308_TOCTOC_CHANNELING

	g_pGameView->SetWindowText(str);

	str.Format("☆도움말☆\n1. 게임창->채팅창 포커스 이동\n단축키 - Enter, ESC\n2. 채팅창->게임창 포커스 이동\n단축키 - ESC\n");
	g_pChatWinMan->ResetChatting_STR();	
	AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

	g_pGameView->Reset();
	g_pGameView->m_cBackGround.OnEnterRoom();
	g_pGameView->m_cGameViewBtnMananager.InitObserverBtn();

	g_pChatWinMan->SetViewPage(CChattingWindowMan::ECTT_CHAT);

	str = " 비정상적인 게임행위(짜고치기) 및 사행성 조장 행위(거래,광고)등이 발생할 시 게임 내 신고 버튼을 이용하여, 신고해 주시기 바랍니다.\n";	
	AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

	//자리지킴이
	g_SeatKeeper.OnEnterGame();
	g_ObserverMan.OnEnterRoom();

	//보유한도
	g_cOwnLimit.OnEnterRoom(&g_cUIData, g_GAMEPROCESS());

	//관전자 채팅 경고
	m_bObserverChatWarning = FALSE;

	// 전광판에 내용 출력
	if (!g_ObserverMan.IsMyObserver())
	{
		CheckMinimumUser();
	}

	CGameProcess::OnEnterRoom(bCreation);

	g_pGameView->m_cGameViewBtnMananager.SetBetBtnType( g_Config.eBetBtnType, true );
	g_pGameView->m_cGameViewBtnMananager.EnterRoom_Button_Init( g_RI.sBetRuleKind );
	g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();

	// 포커스
	g_pGameView->SetFocus();

	if (g_ObserverMan.IsMyObserver())
	{
		str.Format("▶관전 입장: [%s]님\n", GM().GetMyInfo()->UI.NickName);
	}
	else
	{
		str.Format("▶입장: [%s]님\n", GM().GetMyInfo()->UI.NickName);
	}

	AddGameInfoViewEdit(str, GetChatColor(ECC2_NOTIFY));

	str.Format("## <ID: %s(%s)>님 입장 ##\n", GM().GetMyInfo()->UI.ID , GM().GetMyInfo()->UI.NickName);
	AddLobbyHideChatText( &str );

	// 잭팟금액을 세팅해준다. 
	// ( 잭팟 패킷이 게임입장 패킷보다 늦게 올수 있어서 값이 없으면 대기실에서 받은 잭팟 금액으로 세팅을 해준다. )
	if (GetJackPotMoney() <= 0)
	{
		JackPotMoneyUpdate(&g_JackPotMoneyInfo);
	}
	g_cPromotionMan.OnEnterRoom();
}

// 방에서 나간다.
void CGameProcessHoola::OnExitRoom()
{
	CGameProcess::OnExitRoom();

	g_ObserverMan.SetMyObserver(FALSE);

	for (int i = 1; i < GetMaxNewPlayer(); ++i) 
	{
		g_PLAYER(i)->Clear();
	}
	// 방 목록 및 사용자 목록 리스트를 요구	
	g_pGameView->m_cAniSprListManager.Destroy();
	g_pGameView->m_cStayAniSprListManager.Destroy();
	

	//자리 지킴이
	g_SeatKeeper.OnOutRoom();

	g_pChatWinMan->ResetChatting_STR();
	
	NMBASE::SOUND::g_Sound.Stop(SND_HIDDEN_BGM);

	m_i64JackPotMoney = 0;
	m_i64OldJackPotMoney = 0;

	//BTS대응 결과창 출력 문제
	m_cPlayScr.m_bShowGameResult = FALSE;
	ResetGame();

	//훌라 BGM제거
	//NMBASE::SOUND::g_Sound.StopAllMusic();
	StopMusic();

	// 보유한도 초과 팝업
	g_cOwnLimit.OnExitRoom();
}




//UPG작업 추가 (기존 GameDlg::PreTranslateMessage() 에 있던 부분을 가져옴)
BOOL CGameProcessHoola::PreTranslateMessage(MSG* pMsg) 
{
	///////////////////////////////////////////////////////////////
	if (pMsg->message == WM_SYSKEYDOWN)
	{
		if ((pMsg->lParam & 0x20000000) > 0 && (pMsg->wParam == '1' || pMsg->wParam == '2' || pMsg->wParam == '3' ||	 pMsg->wParam == '4' || pMsg->wParam == '5')) 
		{			
			int num = (pMsg->wParam - 49)+5;

			if (num >= 0 && num < 10)
			{
				CString str = g_ChatMacro.Macrostr[num];
				g_pChatWinMan->SystemKeyReturn(str);
			}
		}
	}

	if (pMsg->message == WM_MOUSEWHEEL) // 채팅창에서 마우스 휠
	{
		if ( g_pChatWinMan != NULL && g_pChatWinMan->GetChatView() != NULL )
		{
			g_pChatWinMan->GetChatView()->OnMouseWheel(pMsg->wParam, pMsg->lParam);			
		}
	} 
	else if (pMsg->message == WM_KEYDOWN) 
	{
		// 센드 에디트가 아닌 다른곳에 포커스가 있다면 게임키로 인식
		//if (g_pGameView->GetFocus() != (CWnd*)g_pChatWinMan->GetSendEdit()->pSendEdit)
		if (!g_pChatWinMan->IsEditFocus())
		{
			//////////
			// 숫자키패드 게임키 추가 2004.07.21
			int scankey = 0, extended = 0, previous = 0;
			BOOL bGameKey = IsSacnCode(pMsg->lParam, scankey, extended, previous);

			if ( (pMsg->wParam == 13 && extended == 1 )|| (pMsg->wParam == VK_RETURN && extended == 0 )|| pMsg->wParam == VK_ESCAPE) 
			{			
				// 채팅창에 포커스를 옮긴다
				// 0 : eCHATVIEW::eCHAT
				if ( g_pChatWinMan->GetViewMode() == 0 )
				{
					g_pChatWinMan->GetSendEdit()->pSendEdit->SetFocus();
					return TRUE;
				}
			}

			if (GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			{
				//추가
				OnGameKey( scankey, extended, previous );				
			}
			return TRUE;
		}
		else 
		{

			// 숫자키패드 게임키 추가 2004.07.21
			int scankey = 0, extended = 0, previous = 0;			
			BOOL bGameKey = IsSacnCode(pMsg->lParam, scankey, extended, previous);	//수정

			// 복사 붙여넣기 막는 작업.
			bool bPushCtrl = false;
			bool bPushShift = false;
			if (GetAsyncKeyState(VK_LCONTROL) & 0x8000) bPushCtrl = true;
			if (GetAsyncKeyState(VK_LSHIFT) & 0x8000) bPushShift = true;

			if (bPushCtrl)
			{
				switch( scankey )
				{
				case VK_S_V:		
					return TRUE;
				case VK_S_C:		
					return TRUE;
				case VK_S_X:		
					return TRUE;
				}
			}

			//대화 내용이 없을때
			// 채팅 창에서 예외 키
			char str[256]={0, };//대화 내용 읽어 오기 			
			g_pChatWinMan->GetSendEdit()->pSendEdit->GetWindowText(str, 128);
			if (strlen(str) == 0 && (pMsg->wParam == VK_UP || pMsg->wParam == VK_SPACE||
				pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_LEFT || pMsg->wParam == VK_DOWN ||
				pMsg->wParam == VK_INSERT || pMsg->wParam == VK_HOME || pMsg->wParam == VK_PRIOR ||
				pMsg->wParam ==  VK_DELETE ||pMsg->wParam == VK_END ||pMsg->wParam ==  VK_NEXT ||
				(pMsg->wParam == VK_RETURN && extended == 1 ) || pMsg->wParam == VK_DECIMAL ))
			{
				g_pGameView->SetFocus();
				if (GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				{				
					

					OnGameKey( scankey, extended, previous ); //수정
				}
				return TRUE;
			}

			// 대화 내용이 있어도
			// 채팅 창에서 예외 키
			if (pMsg->wParam == VK_INSERT || pMsg->wParam == VK_HOME || pMsg->wParam == VK_PRIOR ||
				pMsg->wParam ==  VK_DELETE ||pMsg->wParam == VK_END ||pMsg->wParam ==  VK_NEXT ||
				pMsg->wParam == VK_TAB || pMsg->wParam == VK_F2 || pMsg->wParam == VK_F3    ||
				pMsg->wParam == VK_F4 || pMsg->wParam == VK_F5 || pMsg->wParam == VK_F6   ||
				pMsg->wParam == VK_F7 || pMsg->wParam == VK_F8 || pMsg->wParam == VK_DOWN  ||
				(pMsg->wParam == VK_RETURN && extended == 1 ) || pMsg->wParam == VK_DECIMAL )
			{
				if (GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				{
					OnGameKey( scankey, extended, previous ); //수정
				}
				return TRUE;
			}

			if (pMsg->wParam == VK_ESCAPE)
			{
				// 게임뷰에 포커스를 옮긴다
				//프로필 창을 닫음
				//		g_pGameView->m_cAniSprListManager.Del_AniListOBj(ANF_SDLG_PROFILE);
				g_pGameView->SetFocus();
				return TRUE;
			}
		}
	}


	return FALSE;
}

//UPG작업 함수추가
BOOL CGameProcessHoola::IsSacnCode(int nScanCode, int &r_scankey, int &r_extended, int &r_previous)
{
	// 스캔코드를분석함
	// 스캔키코드
	r_scankey = (nScanCode >> 16) & 0x000000FF; 
	
	// 확장키인지여부(1이면확장키임- 숫자키패드방향키는비확장키보드임)
	r_extended = (nScanCode >> 24) & 0x00000001; 
	
	// 키의이전상태(1이면이전에눌려진상태임)
	r_previous = (nScanCode >> 30) & 0x00000001; 

	// 게임에서사용하는키인가검사
	if ( ( r_scankey == 79 && r_extended == 0 ) || // 키패드1		가로삥		| 세로따당
		( r_scankey == 80 && r_extended == 0 ) || // 키패드2		가로따당	| 세로쿼터
		( r_scankey == 81 && r_extended == 0 ) || // 키패드3		가로쿼터	|
		( r_scankey == 75 && r_extended == 0 ) || // 키패드4		가로다이	| 세로삥
		( r_scankey == 76 && r_extended == 0 ) || // 키패드5		가로체크	| 세로체크
		( r_scankey == 77 && r_extended == 0 ) || // 키패드6		가로콜		|
		( r_scankey == 71 && r_extended == 0 ) || // 키패드7					| 세로콜
		( r_scankey == 72 && r_extended == 0 ) || // 키패드8					| 세로다이
		( r_scankey == 73 && r_extended == 0 ) || // 키패드9					| 
		( r_scankey == 82 && r_extended == 0 ) || // 키패드0					| 세로하프
		( r_scankey == 83 && r_extended == 0 ) || // 키패드.					| 세로하프
		( r_scankey == 53 && r_extended == 1 ) || // 키패드/					| 세로하프
		( r_scankey == 55 && r_extended == 0 ) || // 키패드*					| 세로하프
		( r_scankey == 74 && r_extended == 0 ) || // 키패드-					| 세로하프
		( r_scankey == 78 && r_extended == 0 ) || // 키패드+		가로하프	|
		( r_scankey == 28 && r_extended == 1 )    // 키패드enter	가로풀		|
		)  // 콜
	{
		return true;
	}
	return FALSE;

}

void CGameProcessHoola::OnInit()
{
	set_GameSpeed();
}

void CGameProcessHoola::set_GameSpeed()
{
	g_GameSpeed.Clear();

	int i = 0, j = 0;

	g_GameSpeed.set_CardDistributeDelay(i, j, 20);
	g_GameSpeed.set_CardSpeed(i, j, 7.1f);
	g_GameSpeed.set_CardFlyDelay(i, j, 2);
	g_GameSpeed.set_BetCmdActiveDelay(i, j, 4);
	g_GameSpeed.set_AutoBetActiveDelay(i, j, 13);	 

	const int CARDMOVESPEED = 1;
	g_GameSpeed.nDebug_ModeKind = CARDMOVESPEED;
	g_GameSpeed.bShowBetMoney = FALSE;
}


BOOL CGameProcessHoola::OnCommand(WPARAM wParam, LPARAM  lParam)
{
	g_pGameView->m_cChatWindow.OnCommand(wParam);

	if ( g_ObserverMan.OnCommand(wParam, lParam) )
		return TRUE;

	if ( g_pGameView->m_cGameViewBtnMananager.OnCommand(wParam,lParam))
		return TRUE;

	switch(wParam)
	{
	case IDM_BADUSER:
		{
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_CHATWINDOW_CHATTING_REPORT);
			GM().BadUserReportGameChat();
		}
		break;
	


	case IDM_GRADE:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_GAME_USE_GRADE);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			CGradeSpr *pObj = new CGradeSpr;
			pObj->Init(0,&g_pGameView->Page);
			g_pGameView->m_cAniSprListManager.ADDObject(pObj);
		}
		break;

		//보유한도초과금
	case IDM_BTN_EXCESSMONEY:
		{
		} 
		break;

	default:
		{
			//재기충전	
			if (g_cOneShotCharge.OnCommand(wParam, lParam))
			{
				return TRUE;
			}

			return FALSE;
		}
		break;
	}

	return TRUE;
}

void CGameProcessHoola::OnCheckTurnLimitTime()
{
	if (g_RI.State == EGAME_WAITING)
		return;

	if (g_pGameView->m_cPlayerDrawManager.GetStartTimerTime() <= 0)
		return;

	OnCheckTurnLimitTimeBase();
}

CPlayer* CGameProcessHoola::GetPlayer(int i)
{
	ASSERT(i<GetMaxNewPlayer() && i>=0);
	return &m_cPlayer[i];
}

CPlayerHoola* CGameProcessHoola::GetPlayerNP(int i)
{
	return (CPlayerHoola *)GetPlayer(i);
}


void CGameProcessHoola::CheckMinimumUser(void)
{
	int nPlayer = 0;
	for (int i = 0; i < MAX_PLAYER; ++i) 
	{
		if ( strlen(m_cPlayer[i].UI.ID) > 0 && m_cPlayer[i].JoinState != EGAME_JOINOUT)
		{
			++nPlayer;
		}
	}
	if (nPlayer < 2) 
	{
		g_pGameView->m_cDisplayBoard.SetText(EDBT_MINUSER);
	}
}

void CGameProcessHoola::JackPotMoneyUpdate(jackpot::JP_MONEYINFO* pInfo)
{
	// 게임 타입
	IDX_GAME eGameType = (IDX_GAME)g_RI.nGameIndex;

	// 자신이 입장한 방의 최소 금액
	INT64 i64RoomMoney = g_RI.llEnterLimitMoney;

	if (i64RoomMoney <= 0)	///자유방 제외
		return;
	
	//뉴포커, 로우, 7포커
	ASSERT (eGameType > IDX_GAME_NULL);

	const int INDEX_ARRAY_JACKPOT_GAME_HOOLA = 4; //7포커 인덱스 0

	INT64 i64JackPotMoney = 0; 

// 	//훌라는 첫번째 인덱스에있는 잭팟머니만 가져오며 전체구간 동률 적용
// 	i64JackPotMoney = pInfo->stJPBase[INDEX_ARRAY_JACKPOT_GAME_HOOLA].llJackPotMoney[0]; 
// 
	for (int i = 0; i < jackpot::DEF_MONEYRANGE_NUM; ++i)
	{
		if (LIMITMONEY_INT_INT64(pInfo->stJPBase[ INDEX_ARRAY_JACKPOT_GAME_HOOLA ].nLimitMoney[i]) == i64RoomMoney)
		{
			i64JackPotMoney = pInfo->stJPBase[INDEX_ARRAY_JACKPOT_GAME_HOOLA ].llJackPotMoney[ i ];//  llJackpotMoney[eGameType][i];  
			break;
		}
	}

	if (i64JackPotMoney > 0)
	{
		// 이전 잭팟 금액
		SetOldJackPotMoney(GetJackPotMoney());
		// 새로운 잭팟 금액
		SetJackPotMoney(i64JackPotMoney);
	}
}
void CGameProcessHoola::ClearJackpotMoney(int nGameCode, INT64 i64LimitMoney)
{
	// 게임 타입
	IDX_GAME eGameType = IDX_GAME_NULL;

	switch(nGameCode)
	{
	case GAME_CODE_SP: eGameType = IDX_GAME_SP;
		break;
	case GAME_CODE_BD: eGameType = IDX_GAME_BD;
		break;
	case GAME_CODE_NP: eGameType = IDX_GAME_NP;
		break;
	case GAME_CODE_HA: eGameType = IDX_GAME_HA;
		break;
	default:
		{
			// 이곳에 들어가면 초기화 할필요가 없으므로 리턴한다.
			return;
		}
	}

	if (i64LimitMoney <= 0)
		return;


	//1. 게임타입 검사
	//2. 입장한 방 머니 검사   
	//3. 두가지 값이 동일하면 현재 방의 잭팟 머니를 초기화 한다.
	int nGameIndexTemp		= g_RI.nGameIndex;
	INT64 i64LimitMoneyTemp = g_RI.MoneyRange.GetDispMinMoney();

	if (eGameType == nGameIndexTemp && i64LimitMoney == i64LimitMoneyTemp)
	{
		SetOldJackPotMoney(0);
		SetJackPotMoney(0);

		// 전역변수의 해당 머니 구간의 값도 0으로 초기화 한다.
		// 현재 방에 대당하는 구간의 잭팟금액을 초기화 한다.
		for (int i = 0; i < jackpot::DEF_MONEYRANGE_NUM; ++i)
		{
			if (g_RoomMoneyRanges[i] >= i64LimitMoneyTemp)
			{
				g_JackPotMoneyInfo.stJPBase[ eGameType ].llJackPotMoney[ i ] = 0;
				break;
			}
		}
	}
}


void CGameProcessHoola::DoPrepareGame(STARTINFO *pSI)
{
	if (pSI == NULL) return;
	
	m_bForceReserveExitGame = FALSE;

	// 연습게임이 아닐 경우
	if ( !pSI->bPracticsGame )
	{
		for ( int i = 0; i < MAX_PLAYER; i++ )
		{
			// 연습게임이 아닐 경우 연습 게임 봇 클리어
			if ( strcmp( g_PLAYER(i)->UI.ID, "_PRACTICSUSER_" ) == 0 )
			{
				g_PLAYER(i)->Clear();
				g_RI.NowUserNum--;
			}
		}
	}
	// 연습 게임일 경우
	else
	{
		// 인게임 채팅창에 메시지를 띄움
		CString sysmsg;
		sysmsg.Format(_T("연습게임은 게임머니를 주고받지 않습니다.\n"));
		AddGameInfoViewEdit( sysmsg, RGB( 224, 10, 10 ) );
	}

	g_RI.State = EGAME_PLAYING;

	m_cPlayScr.Reset();
	memset( m_cPlayScr.m_MyCard, 0, sizeof( char ) * 13 * 4 );

	// 게임 시작 플래그 세팅
	m_bGameStart = TRUE;	

	for ( int i = 0; i < 7; i ++ )
	{
		if ( g_RI.FormKind == hoola::eFORM_KIND_JOKER && ( pSI->PlayerCard[i] == 52 || pSI->PlayerCard[i] == 53 ) )
		{
			m_cPlayScr.m_JokerCard[ pSI->PlayerCard[i] - 52 ] = 1;
		}
		else
		{
			m_cPlayScr.m_MyCard[pSI->PlayerCard[i] % 13][pSI->PlayerCard[i] / 13] = true;
		}
	}

	int cnt = 0;
	for ( int i = 0; i < 13; i ++ )
	{
		for ( int j = 0; j < 4; j ++ )
		{
			if (m_cPlayScr.m_MyCard[i][j])
				m_cPlayScr.m_PlayerCard[0][cnt++] = j * 13 + i;
		}
	}

	if ( m_cPlayScr.m_JokerCard[0] == 1 )
	{
		m_cPlayScr.m_PlayerCard[0][cnt++] = 52;
	}

	if ( m_cPlayScr.m_JokerCard[1] == 1 )
	{
		m_cPlayScr.m_PlayerCard[0][cnt++] = 53;
	}



	int nPlayerNum = 0;
	BOOL bFalse = FALSE;

	for ( int i = 0 ; i < MAX_PLAYER ; i++ ) 
	{
		if ( strlen(pSI->JoinID[i]) > 0 )
		{
			int pn = GetPNum_ByServPN(i);
			if (strcmp(g_PLAYER(pn)->UI.ID, pSI->JoinID[i]) != 0) 
			{
				bFalse = TRUE; 

				pn = -1;
				for ( int j = 0; j < MAX_PLAYER; j++ )
				{
					if ( strcmp( g_PLAYER(j)->UI.ID, pSI->JoinID[i] ) == 0 )
					{
						pn = j;
						break;
					}
				}
			}

			if ( pn == -1 ) 
				break;

			g_PLAYER(pn)->SetCardCount( 7 );
			m_cPlayScr.m_nOldCardCnt[pn] = 7;
			m_cPlayScr.m_nOldCardCntTime[pn] = timeGetTime();

			// 플레이어 상태 세팅 
			g_PLAYER(pn)->JoinState = pSI->Ps[i].JoinState;
			g_PLAYER(pn)->PlayState = pSI->Ps[i].PlayState;

			if ( pn == 0)
			{
				if ( !g_ObserverMan.IsMyObserver() )
				{
					GM().GetMyInfo()->JoinState = g_PLAYER(0)->JoinState;
					GM().GetMyInfo()->PlayState = g_PLAYER(0)->PlayState;
					GM().GetMyInfo()->ServPNum = g_PLAYER(0)->ServPNum;
				}
			}
			if ( g_PLAYER(pn)->PlayState )
				nPlayerNum++;
		}
	}

	if ( nPlayerNum < 4 )
	{
		//임시로 주석처리
//		m_cPlayScr.m_nJackpotConditionStep = 0;
	}

	if (bFalse == TRUE) {
		m_bForceReserveExitGame = TRUE;
	}

	if ( g_RI.FormKind == hoola::eFORM_KIND_JOKER)
	{
		m_cPlayScr.m_nCurStockCnt = 54 - ( nPlayerNum * 7 );
	}
	else
	{
		m_cPlayScr.m_nCurStockCnt = 52 - ( nPlayerNum * 7 );
	}

	g_pGameView->GetGameButtonMan()->SetEnableBtn(CGameButtonHA::BTN_SORTCARD_STRAIGHT, TRUE);
	g_pGameView->GetGameButtonMan()->SetShowBtn(CGameButtonHA::BTN_SORTCARD_STRAIGHT, TRUE);
	g_pGameView->GetGameButtonMan()->SetShowBtn(CGameButtonHA::BTN_SORTCARD_TOGETHER, FALSE);

	m_cPlayScr.m_bPracticsGame = pSI->bPracticsGame?TRUE:FALSE;

	//자리 지킴이
	g_SeatKeeper.OnStartGame();
}

void CGameProcessHoola::GameOverMoneyProcess( GAMEOVERRESULT *pGOR )
{
// 	for (int i=0; i<MAX_PLAYER; i++)
// 	{
// 		if (strlen(pGOR[i].ID)>0)
// 		{
// 			int pnum = GetPlayerPNum(pGOR[i].ID);
// 			if (pnum == -1) continue;
// 
// 			if ( !m_cPlayScr.m_bPracticsGame )
// 			{
// 				// 프로필용 정보 저장
// 				g_PLAYER(pnum)->UI.WinNum = pGOR[i].NowWinNum;
// 				g_PLAYER(pnum)->UI.LooseNum = pGOR[i].NowLooseNum;
// 				g_PLAYER(pnum)->UI.HoolaWin = pGOR[i].HoolaNum;
// 				g_PLAYER(pnum)->UI.Daebbang = pGOR[i].Daebbang;
// 				g_PLAYER(pnum)->UI.Sobbang = pGOR[i].Sobbang;
// 				if ( pGOR[i].Ranking == 1 )
// 				{
// 					g_PLAYER(pnum)->UI.PMoney += pGOR[i].GetPMoney;
// 				}
// 				else
// 				{
// 					g_PLAYER(pnum)->UI.PMoney = pGOR[i].NowPMoney;
// 				}
// 
// 				// [최고금액 추가] 2006.03.06
// 				if ( g_PLAYER(pnum)->UI.i64GetMaxMoney < pGOR[i].i64GetMaxMoney )
// 					g_PLAYER(pnum)->UI.i64GetMaxMoney = pGOR[i].i64GetMaxMoney;
// 				if ( g_PLAYER(pnum)->UI.i64GetBetMaxMoney < pGOR[i].i64GetBetMaxMoney )
// 					g_PLAYER(pnum)->UI.i64GetBetMaxMoney = pGOR[i].i64GetBetMaxMoney;
// 
// 				g_PLAYER(pnum)->UI.nIcon = GetHoolaLevel( g_PLAYER(pnum)->UI.GetTotalMoney() );
// 			}
// 		}
// 	}
// 	// ### [ 관전기능 ] ###
// 	if ( g_MyObserver != TRUE && !m_cPlayScr.m_bPracticsGame ) //관전이 아니면
// 	{
// 		g_MyInfo.UI.WinNum   = g_PLAYER(0)->UI.WinNum; 
// 		g_MyInfo.UI.LooseNum = g_PLAYER(0)->UI.LooseNum; 
// 		g_MyInfo.UI.HoolaWin =  g_PLAYER(0)->UI.HoolaWin;
// 		g_MyInfo.UI.Daebbang = g_PLAYER(0)->UI.Daebbang;
// 		g_MyInfo.UI.Sobbang = g_PLAYER(0)->UI.Sobbang;
// 		g_MyInfo.UI.PMoney  =  g_PLAYER(0)->UI.PMoney; 
// 		g_MyInfo.PlayState  =  g_PLAYER(0)->PlayState; 
// 
// 		// [최고금액 추가] 2006.03.06
// 		if ( g_MyInfo.UI.i64GetMaxMoney < g_PLAYER(0)->UI.i64GetMaxMoney )
// 			g_MyInfo.UI.i64GetMaxMoney = g_PLAYER(0)->UI.i64GetMaxMoney;
// 		if ( g_MyInfo.UI.i64GetBetMaxMoney < g_PLAYER(0)->UI.i64GetBetMaxMoney )
// 			g_MyInfo.UI.i64GetBetMaxMoney = g_PLAYER(0)->UI.i64GetBetMaxMoney;
// 	}
}

void CGameProcessHoola::SetupByConfig()
{
// 	// 사운드 설정 적용
// 	NMBASE::SOUND::g_Sound.SetSndFx
// 	Sound.SetVoice(CFG.bVoice);
// 	Sound.SetSndFx(CFG.bSndFX);
// 	Sound.SetLobyMusic(CFG.bLobyMusic);
// 	//	Sound.SetGameMusic(CFG.bGameMusic);
// 	Sound.SetGameMusic(CFG.bLobyMusic);
// 
// 
// 	if (CFG.bFailToFullScreen) {
// 		// 풀스크린이 성공하는지 여부를 체크하기 위함
// 		CFG.nScreenMode = 0;
// 		CFG.bFailToFullScreen = FALSE;
// 		//		SaveConfig(Play[0].UI.ID);
// 		// ### [ 관전기능 ] ###
// 		SaveConfig(g_MyInfo.UI.ID);
// 		CMsg Msg; Msg.DoModal( "지난번 실행시 풀스크린 모드로 화면 전환에 실패하였기때문에 창모드로 실행합니다.", "알림" );
// 	} else {
// 		// 화면 모드 설정 적용
// 		if (CFG.nScreenMode==1) {
// 			// 풀스크린이 성공하는지 여부를 체크하기 위함
// 			CFG.bFailToFullScreen = TRUE;
// 			//			SaveConfig(Play[0].UI.ID);
// 			// ### [ 관전기능 ] ###
// 			SaveConfig(g_MyInfo.UI.ID);
// 
// 			g_pLobyDlg->bDontClickSound = TRUE;
// 			g_pLobyDlg->SendMessage(WM_COMMAND, IDC_BUTTON_CHANGEDISPLAY);
// 
// 			// 풀스크린이 성공하였기때문에 원래값으로 초기화함
// 			CFG.bFailToFullScreen = FALSE;
// 			//			SaveConfig(Play[0].UI.ID);
// 			// ### [ 관전기능 ] ###
// 			SaveConfig(g_MyInfo.UI.ID);
// 		}
// 	}
// 
// 	// 부가 기능 설정 적용
// 	if (CFG.bNoInvite) {
// 		CButton* pCheck = (CButton*)g_pLobyDlg->GetDlgItem(IDC_CHECK_REFUSEINVITE);
// 		pCheck->SetCheck(TRUE);
// 	}
// 	// 	if (CFG.bNoWhisper) {
// 	// 		CButton* pCheck = (CButton*)g_pLobyDlg->GetDlgItem(IDC_CHECK_REFUSEWHISPER);
// 	// 		pCheck->SetCheck(TRUE);
// 	// 		g_pLobyDlg->PostMessage(WM_COMMAND, IDC_CHECK_REFUSEWHISPER);
// 	// 	}
// 
// 	// 상태에 따라 귓말 거부/허용 메시지를 보냄
// 	CSV_NOWHISPER nwmsg;
// 	nwmsg.Set(CFG.bNoWhisper);
// 	SockMan.SendData(g_MainSrvSID, nwmsg.pData, nwmsg.GetTotalSize());
// 
// 	// ### [ 관전기능 ] ###
// 	if (g_MyInfo.UI.Sex == 1) // 남자
// 	{
// 		if (CFG.nSoundType >= 3 ) CFG.nSoundType = 1; // 1 ~ 2
// 	}
// 	else 
// 	{
// 		if (CFG.nSoundType < 3 ) CFG.nSoundType = 3; // 3 ~ 4
// 	}
// 
// 	// ### [ 관전기능 ] ###
// 	g_MyInfo.Sound = CFG.nSoundType;
// 	Play[0].Sound  = CFG.nSoundType;
// 
// 	g_pLobyDlg->LoadConfig();
}
void CGameProcessHoola::MusicTest()
{
	if ( GetCurrentWhere() == IDX_GAMEWHERE_GAME )
	{
		NMBASE::SOUND::g_Sound.Stop(MUSIC_NORMAL);
		NMBASE::SOUND::g_Sound.Stop(MUSIC_GOLD);
		NMBASE::SOUND::g_Sound.Stop(MUSIC_OGG04);

		static bool bMusicSwitch = false;

		if ( bMusicSwitch )
		{
			NMBASE::SOUND::g_Sound.PlayOggStream_BGM(MUSIC_GOLD, TRUE, (int) g_Config.BGMVolume, false);
		}
		else
		{				
			NMBASE::SOUND::g_Sound.PlayOggStream_BGM(MUSIC_NORMAL, TRUE, (int) g_Config.BGMVolume, false);

		}
		bMusicSwitch = !bMusicSwitch;
	}
}
void  CGameProcessHoola::PlayMusic()
{
	if ( GetCurrentWhere() == IDX_GAMEWHERE_GAME )
	{
//		NMBASE::SOUND::g_Sound.StopAllMusic();
		StopMusic();

		if ( m_cPlayScr.m_nGoldStage >= 2 )
		{
			NMBASE::SOUND::g_Sound.PlayOggStream_BGM(MUSIC_GOLD, TRUE, (int) g_Config.BGMVolume, false);   //PlayOggStreamForMusic(MUSIC_GOLD);
			m_eMusicType = eMUSIC_GOLD;
		}
		else
		{				
			NMBASE::SOUND::g_Sound.PlayOggStream_BGM(MUSIC_NORMAL, TRUE, (int) g_Config.BGMVolume, false);
			m_eMusicType = eMUSIC_NORMAL;
		}
	}
}

void  CGameProcessHoola::StopMusic()
{
	if ( m_eMusicType == eMUSIC_NORMAL )
	{
		NMBASE::SOUND::g_Sound.Stop(MUSIC_NORMAL);
	}
	else if ( m_eMusicType == eMUSIC_GOLD )
	{
		NMBASE::SOUND::g_Sound.Stop(MUSIC_GOLD);
	}

	m_eMusicType=eMUSIC_NULL;
}


void CGameProcessHoola::SetPlayerSndKindArray()
{
	for ( int i = 0 ; i < MAX_PLAYER ; i++ )
	{
		//int nPlayerNum = GetPNum_ByServPN(i);
		m_nArrSndData[i].nSndFxKind = g_PLAYER(i)->nSndFxKind;
		m_nArrSndData[i].bSex = g_PLAYER(i)->UI.Sex;
	}
}
void CGameProcessHoola::SetStartMsgTitle()
{
	// 전광판 시작 메시지 출력
	// 매판 시작 시 랜덤 노출
	int nNormalMsgIndex = rand() % 3;
	switch( nNormalMsgIndex )
	{
	case 0:
		g_pGameView->m_cDisplayBoard.SetText(EDBT_NORMAL0_HOOLA);
		break;
	case 1:
		g_pGameView->m_cDisplayBoard.SetText(EDBT_NORMAL1_HOOLA);
		break;
	case 2:
		g_pGameView->m_cDisplayBoard.SetText(EDBT_NORMAL2_HOOLA);
		break;
	}
}
void CGameProcessHoola::SetMissionCompleteMsgTitle(char* pID)
{
	// 미션 성공상황 메시지 출력
	g_pGameView->m_cDisplayBoard.SetText(EDBT_MISSION_COMPLETE_HOOLA, (LPARAM) pID);
}
void CGameProcessHoola::SetStopJackpotMsgTitle(jackpot::JP_WIN* pAlram)
{
	// 스톱잭팟 성공상황 메시지 출력
	// 제한 머니(1조방 이하
	if (pAlram == NULL)
	{
		return;
	}
	
	if (pAlram->JData.llLimitMoney >= 300000000000)
	{
		g_pGameView->m_cDisplayBoard.SetText(EDBT_STOPJACKPOT_UPPER_3JO_HOOLA, (LPARAM) pAlram);
	}
	else
	{
		g_pGameView->m_cDisplayBoard.SetText(EDBT_STOPJACKPOT_BELOW_1JO_HOOLA, (LPARAM) pAlram);
	}
}


void CGameProcessHoola::SetJacpotAnimationState(jackpot::JP_WIN* pAlram)
{
	m_cPlayScr.ResetLimitTime();

	for (int i=0; i < MAX_PLAYER; i++)
	{
		if (strlen(g_PLAYER(i)->UI.ID) == 0)
		{
			continue;
		}
		if (strcmp(pAlram->JData.szID, g_PLAYER(i)->UI.ID)==0)
		{
			m_cPlayScr.m_bJackpot = TRUE;
			m_cPlayScr.m_dwJackpot = timeGetTime();
			CopyMemory( m_cPlayScr.m_sJackpotID, pAlram->JData.szID, 16 );
			m_cPlayScr.m_nJackPotStep = 1;
			m_cPlayScr.SetJackpotData(pAlram->JData.llBonusPMoney);
			break;						
		}
	}
}

void CGameProcessHoola::SendCL_ASK_CONTINUE( int rnum, int pnum, int unum, char* id )
{
	CCL_ASK_CONTINUE msg;
	msg.Set( rnum, pnum, unum, id);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
}
void CGameProcessHoola::SendCL_STOPGAME( int rnum, int pnum, int unum, char* id, int ntype )
{
	CCL_STOPGAME msg;
	msg.Set( rnum, pnum, unum, id, ntype );
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
}
void CGameProcessHoola::ReviewRCDATA(REGISTERCARD* pRC)
{
	//예외 처리 조커7 묶음조합의 처리
	if ( pRC->nToCount[0] >= 3)
	{
		BOOL bTogetherSeven = FALSE;
		BOOL bTogetherJoker = FALSE;
		for ( int i = 0 ; i < pRC->nToCount[0] ; i++ )
		{
			int nCardValue = pRC->TogetherCard[0][i] % 13;
			if ( nCardValue == 6 ) //7카드일때
			{
				bTogetherSeven = TRUE;
			}
		}
		for ( int i = 0 ; i < pRC->nToCount[0] ; i++ )
		{
			if ( pRC->TogetherCard[0][i] >= 200 ) //조커 카드일때
			{
				bTogetherJoker = TRUE;
			}
		}
		if ( bTogetherSeven && bTogetherJoker )
		{
			for (int i = 0 ; i < pRC->nSevenCount ; i++ )
			{
				 pRC->arrSevenCardValue[i] = -1;
			}
			pRC->nSevenCount = 0;			
		}
	}

	std::deque<BYTE> dqSevenCard;
	for ( int i = 0 ; i < pRC->nStCount[0] ; i++ )
	{
		if ( (pRC->StraightCard[0][i])%13 == 6 )
		{
			dqSevenCard.push_back(pRC->StraightCard[0][i]);
		}
	}
	for ( int i = 0 ; i < pRC->nToCount[0] ; i++ )
	{
		if ( (pRC->TogetherCard[0][i])%13 == 6 )
		{
			dqSevenCard.push_back(pRC->TogetherCard[0][i]);
		}
	}

	int nSevenCardCount = pRC->nSevenCount;
	if ( nSevenCardCount > 0 )
	{
		for ( int i = 0 ; i < dqSevenCard.size() ; i++ )		
		{
			for ( int j = 0 ; j < pRC->nSevenCount ; j++ )
			{
				if ( dqSevenCard[i] == pRC->arrSevenCardValue[j] )
				{
					pRC->arrSevenCardValue[j] = -1;
					nSevenCardCount--;
				}
			}
		}
	}

	int arrSevenCard[4] = {0,};
	int cnt = 0;
	for ( int i = 0 ; i < 4 ; i++ )
	{
		if ( pRC->arrSevenCardValue[i] > 0)
		{
			arrSevenCard[cnt] = pRC->arrSevenCardValue[i];
			cnt++;
		}
	}
	for ( int i = 0 ; i < 4 ; i++ )
	{
		pRC->arrSevenCardValue[i] = (char)arrSevenCard[i];
	}
	pRC->nSevenCount = nSevenCardCount;
}
void CGameProcessHoola::SendCL_REGIST( int rnum, int pnum, int unum, char* id, REGISTERCARD* pRC )
{
	ReviewRCDATA(pRC);
	CCL_REGIST msg;
	msg.Set( rnum, pnum, unum, id, pRC );
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
}
void CGameProcessHoola::SendCL_ASK_ONECARD( int rnum, int pnum, int unum, char* id )
{
	CCL_ASK_ONECARD msg;
	msg.Set( rnum, pnum, unum, id );
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
}
void CGameProcessHoola::SendCL_PRESS_THANK( int rnum, int pnum, int unum, char* id, char *pMyCard )
{
	CCL_PRESS_THANK msg;
	msg.Set( rnum, pnum, unum, id, pMyCard );
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
}
void CGameProcessHoola::SendCL_DISCARD( int rnum, int pnum, int unum, char* id, int cardnum )
{
	CCL_DISCARD msg;
	msg.Set( rnum, pnum, unum, id, cardnum );
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
}
void CGameProcessHoola::SendCL_ASK_THANK( int rnum, int pnum, int unum, char* id, CLTHANKCARD *pct )
{
	CCL_ASK_THANK msg;
	msg.Set( rnum, pnum, unum, id, pct );
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
}
void CGameProcessHoola::SendCL_ENDCARDMOVE( int rnum, int pnum, int unum, char* id, char cEndFlag )
{
	CCL_ENDCARDMOVE msg;
	msg.Set( rnum, pnum, unum, id, cEndFlag );
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
}
void CGameProcessHoola::SendCL_WINDCARDEND( int rnum, int pnum, int unum, char* id )
{
	CCL_WINDCARDEND msg;
	msg.Set( rnum, pnum, unum, id );
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
}
void CGameProcessHoola::SendCL_REQGAMESTART( int nRN, char *pID )
{
	CCL_REQGAMESTART msg;
	msg.Set( nRN, pID );
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
}
void CGameProcessHoola::SendCL_REQGAMEPRACTICS( int nRN, char *pID )
{
	CCL_REQGAMEPRACTICS msg;
	msg.Set( nRN, pID );
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
}

void CGameProcessHoola::SendCL_PRESS_GIVEUP()
{
	CCL_PRESS_GIVEUP msg;
	msg.Set();
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
}

//규제안 예액기능 추가로 인해 GameProcess에 추가된 가상 함수
void CGameProcessHoola::SendCallButton()
{

}

void CGameProcessHoola::SendCheckButton()
{

}

void CGameProcessHoola::SendDieButton()
{

}

void CGameProcessHoola::SendHalfButton()
{

}

void CGameProcessHoola::SendDdaButton()
{

}

void CGameProcessHoola::SendPingButton()
{

}
