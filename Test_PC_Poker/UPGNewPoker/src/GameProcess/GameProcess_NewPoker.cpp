// GameProcess_Seven.cpp: implementation of the CGameProcess_Seven class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameProcess_NewPoker.h"
#include "../AutoBet/AutoBetSetUpWnd.h"
#include "../SprDlg/SprDlgObjectMan.h"
#include "../AniListMan/AniFileListMan.h"
#include <mmsystem.h>
#include "../GameNewPoker.h"
#include <CommMsgDef_Game.h>

#pragma comment(lib,"winmm.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CGameProcess_NewPoker::CGameProcess_NewPoker(CGame *pGame)
: CGameProcess(pGame)
{
	g_pGameProcess = this;

	// 게임 정보
	m_bGameStart = FALSE;

	m_bEventDrawFlg = FALSE;
	m_bBettingDelayCheck = FALSE ;

	m_i64JackPotMoney = 0;
	m_i64OldJackPotMoney = 0;
	m_nActiveBetBtnDelayTime = 0 ;

	m_pPlayer = new CPlayer_NP[GetMaxNewPlayer()];

#ifdef UPGRADE_20120502_TIMING_TEST_CODE
//#ifdef _DEBUG
	g_cUIData.m_dwStartGameDealingTimeGab = GetPrivateProfileInt("NewPoker", "BetDelayTime", 180, ".\\TimingTest.ini");
	g_cUIData.m_dwDealingTimeGab = GetPrivateProfileInt("NewPoker", "CardStartGabTime", 220, ".\\TimingTest.ini");

//#endif //_DEBUG
#endif //UPGRADE_20120502_TIMING_TEST_CODE
}

CGameProcess_NewPoker::~CGameProcess_NewPoker()
{
	if (m_pPlayer) delete [] m_pPlayer;
	m_pPlayer = NULL;
}

void CGameProcess_NewPoker::OnTimer()
{
	m_nGameCount++;
	// [ 카드 날리기 ]
	if (g_RI.State == 1 )  // (0:대기중 1:게임중)
	{
		if ( m_bWindCardEnd )
		{
			m_nWindCardEndCnt--;
			if (m_nWindCardEndCnt <= 0)
			{
				m_bWindCardEnd = FALSE;
				// 서버로 메세지 보낸다.
				// ### [ 관전기능 ] ### //JoinState 0 == 게임불참 1 == 게임 참여 // PlayState 0 == 게임종료 1 == 게임중
				if (!g_ObserverMan.IsMyObserver() && !GetPlayerNP(0)->bFold && GetPlayerNP(0)->JoinState == 1 && GetPlayerNP(0)->PlayState == 1)
				{
					CCL_WINDCARDEND msg2;
					msg2.Set(g_RI.RoomNum, g_pMyInfo_NP->ServPNum, g_pMyInfo_NP->UI.UNum, GM().GetMyInfo()->UI.ID);//int rnum, int pnum, int unum, char *id)
					NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg2.pData, msg2.GetTotalSize());
				}
			}
		}

		if ( m_nActiveBetBtnDelayTime >= 0 && m_bBettingDelayCheck )
		{
			m_nActiveBetBtnDelayTime--;
			if (m_nActiveBetBtnDelayTime <= 0 )
			{				
				m_nActiveBetBtnDelayTime = 0;
				
				g_pGameView->m_cGameViewBtnMan.SetButtonClick(false);
				g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(false);
				// 폭탄카드 예외처리.
				if (false == m_bGameOver)
				{
					g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable(false); 		
				}
				AutoBetSetUpWnd().Set_auto_betting_flag();

				m_bBettingDelayCheck = FALSE ;
				//	Set_auto_betting_flag();
			}
		}

		//혹시 몰라 한번 더 검사
		if ( m_nActiveBetBtnDelayTime < 0 )
		{
			m_nActiveBetBtnDelayTime = 0;
			g_pGameView->m_cGameViewBtnMan.SetButtonClick(false);
			g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(false);
			// 폭탄카드 예외처리.
			if (false == m_bGameOver)
			{
				g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable(false); 		
			}
			AutoBetSetUpWnd().Set_auto_betting_flag();

			m_bBettingDelayCheck = FALSE ;
			//Set_auto_betting_flag();
		}

		else{		
			
			//자동 베팅
			// 카드 날리는 중에는 그리지 않는다. 자동 배팅 설정일때 턴 제한시간 다되가면 콜로 보낸다.
			OnCheckTurnLimitTime();
			
			AutoBetSetUpWnd().OnTimer();
		}
	}

	// 총 배팅금액 셋팅
	BOOL bSetAllBet = FALSE;
	int i=0;
	const int nMaxPlayer = GetMaxPlayer();
	for (i = 0 ; i < nMaxPlayer; i++){
		GetPlayerNP(i)->OnTimer();
	}
	
	m_cAllBetChip.OnTimer();

	if (g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME)
	{
		m_cBombCard.OnTimer();// 폭탄 
	}
	
	if ( g_pGameView->m_cPlayerDrawManager.IsPlayerGameOver(0)  && g_pGameView->m_cPlayerDrawManager.IsGameOver()){
		m_cResultFocus.OnTimer();
	}
		
	m_cAniSprObjectMan.ProcessAll();

#ifdef _DEBUG
	if (g_bDeBugModeFlg == TRUE){	

		if (g_RI.State == 0 && g_pGameView->m_cGameViewBtnMan.m_btnGameStart.GetButtonState() > -1)
		{
			g_pGameView->SendMessage(WM_COMMAND,IDM_GAMESTART,0);
		}
		
		if (g_Poker.nAutoBetCmd <= 0)DebugAutobet();
	}
#endif	
}

// 게임 오버시 
void CGameProcess_NewPoker::ResetGame()
{
	g_RI.State = 0;

	m_bGameStart = FALSE;
	m_nGameCount = 0;

	m_nCurPlayer = -1;

	g_pGameView->m_cGameViewBtnMan.SetButtonClick(false);
	g_pGameView->m_cPlayerDrawManager.Reset();
	g_pGameView->m_cGameViewBtnMan.ResetGame();

	// 상태
	m_nGameState = 0;
	
	// ### [ 관전기능 ] ###
	int i=0;
	const int nMaxNewPlayer = GetMaxNewPlayer();
	const int nMaxPlayer = GetMaxPlayer();

	for (i=0; i<nMaxNewPlayer; i++) {// 플레이어 리셋
		GetPlayerNP(i)->Reset();
	}

	// [ 카드 이벤트 에러시 ]
	m_bWindCardEnd	= FALSE;
	m_nWindCardEndCnt = 0;
	m_nStartUserNum	= 0;
	m_bBettingDelayCheck = FALSE ;

	// ###  [ 칩 ] ###
	// 배팅 칩 게임판을 지운다
	m_i64JackPotActive = 0;

	ZeroMemory(m_pJackpotWinnerID, sizeof(m_pJackpotWinnerID));
	ZeroMemory(&g_Poker, sizeof(POKERGAME));

	// 결과 구조체
	ZeroMemory(&m_cGameOverResultData,sizeof(GAMEOVERRESULT));

	// ### [ 관전기능 ] ###
	g_pMyInfo_NP->JoinState = 0;
	g_pMyInfo_NP->PlayState = 0;

	m_nHinddenArrow_x = 0;
	m_nHinddenArrow_y = 0;
	m_MouseHiddenState = 0;
	m_bMouseHiddenClick = FALSE;
	m_bkeyboardHiddenClick = FALSE;
	m_HiddenClickPos.x = 0;
	m_HiddenClickPos.y = 0;
	m_dwHiddenSceneDrawTime = 0;
	//g_pGameView->m_cPlayerDrawManager.SetHiddenCardShow(false); //
// 	m_bHiddenClick = FALSE;
// 	m_ptMouse.x = 0;
// 	m_ptMouse.y = 0;

	m_nActiveBetBtnDelayTime = 0;
	
	m_cAllBetChip.Reset();
	g_pGameView->m_cTotalBetMoney.Reset();
	
	m_cBombCard.Clear();//폭탄
	m_cResultFocus.ReSet();
	m_bEventDrawFlg = FALSE;

	//JOKER_GAME
	memset(&g_JokerCardDeck,-1,sizeof(g_JokerCardDeck));
	g_bGetJokerCard = FALSE;

	m_nDebugCnt = 0;

	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();
	g_pGameView->m_cGameViewBtnMan.m_btnGameStart.Enable(FALSE);
	g_pGameView->m_cGameViewBtnMan.m_btnGameStart.Show(FALSE);

	//조커 카드 
	m_dwStartDrawJokerTime = 0;

	//2006.1.4 종료시 안내 메세지 그리는 부분
	m_nEndingInformationTextMsgCnt = 0;//종료 안내 메세지 개수
	for ( i = 0 ; i < 30 ; i++){
		m_listEndingInformationTextMsg[i] = "";//종료 안내 메세지 내용
	}

	m_cSelectCardMan.Reset();
	g_pGameView->m_cGameViewBtnMan.SetBetBtnType(g_Config.eBetBtnType);

	//규제안 : 예약버튼 활성화 가능상태
	ReserveState(RESERVESTATE_BET_NONE);
	CanReserve(true);
	g_nCardTotalCount = 0 ; // 카드 세팅 비교값을 0으로 초기화.

	m_bGameOver = false ;

	m_bOpenBombcard = false ;
}

void CGameProcess_NewPoker::OnInit()
{
	set_GameSpeed();
}

void CGameProcess_NewPoker::set_GameSpeed()
{
	g_GameSpeed.Clear();

	int i = 0, j = 0;

	g_GameSpeed.set_CardDistributeDelay(i, j, 20);
	g_GameSpeed.set_CardSpeed(i, j, 7.1f);
	g_GameSpeed.set_CardFlyDelay(i, j, 2);

	g_GameSpeed.set_BetCmdActiveDelay(i, j, 0);// 게임사업 1팀의 요청으로 변경
	//g_GameSpeed.set_BetCmdActiveDelay(i, j, 60);

	g_GameSpeed.set_BetCmdActiveDelayInAutoBet(i, j, 0);
	g_GameSpeed.set_AutoBetActiveDelay(i, j, 13); 

#ifdef UPGRADE_20120502_TIMING_TEST_CODE
//#ifdef _DEBUG
	int nBetCmdActiveDelay = GetPrivateProfileInt("NewPoker", "BetBtnActiveFrameNum", 4, ".\\TimingTest.ini");
	g_GameSpeed.set_BetCmdActiveDelay(i, j, nBetCmdActiveDelay);

	nBetCmdActiveDelay = GetPrivateProfileInt("NewPoker", "BetBtnActiveInAutoBetFrameNum", 4, ".\\TimingTest.ini");
	g_GameSpeed.set_BetCmdActiveDelayInAutoBet(i, j, nBetCmdActiveDelay);
//#endif //_DEBUG
#endif //UPGRADE_20120502_TIMING_TEST_CODE

	const int CARDMOVESPEED = 1;
	g_GameSpeed.nDebug_ModeKind = CARDMOVESPEED;
	g_GameSpeed.bShowBetMoney = FALSE;
}


BOOL CGameProcess_NewPoker::OnMouseMove(int x, int y)
{
	if (g_pGameView->OnCardMouseMove(x, y) ) return TRUE;
	return FALSE;
}

BOOL CGameProcess_NewPoker::OnLButtonDown(int x, int y)
{
	if (g_pGameView->OnCardLButtonDown(x, y)) { return TRUE; }
	return FALSE;
}

BOOL CGameProcess_NewPoker::OnLButtonUp(int x, int y)
{	
	if (g_pGameView->OnCardLButtonUp(x, y) ) return TRUE;
	return FALSE;
}

BOOL CGameProcess_NewPoker::OnLButtonDblclk(int x, int y)
{
	return FALSE;
}

void CGameProcess_NewPoker::OnGameKey(int nVKey, int nextended, int repeat)

{
#ifdef _DEBUG	
	if ( /*VK_F2*/60 == nVKey )
	{
		//CPromotionPokerTime *pPokerTime = (CPromotionPokerTime *)g_cPromotionMan.MakePromotion(CPromotionManager::EPT_POKERTIME);
		//pPokerTime->ShowAniEffect(0, "스트레이트", "50억");
		//pPokerTime->ShowAniEffect(1, "스트레이트", "50억");
		//pPokerTime->ShowAniEffect(2, "스트레이트", "50억");
		//pPokerTime->ShowAniEffect(3, "스트레이트", "50억");
		//pPokerTime->ShowAniEffect(4, "스트레이트", "50억");

		m_cResultFocus.SetRFocus(0,CCardRule::N7C_QUADS);
		g_pGameView->m_cSidePotMoney.Check();
	}
	
	if ( /*VK_F3*/61 == nVKey )
	{
	}

	if ( /*VK_F4*/62 == nVKey )
	{
	}

	if ( /*VK_F5*/63 == nVKey )
	{
	}
	if ( nVKey == rule::SKEY_F7 )
	{
		CMadeEffect *pObj = NULL;
		SAFE_NEW( pObj, CMadeEffect );
		pObj->Init(4);
		m_cAniSprObjectMan.AddObject(pObj);
	}
#endif


	if ( m_cSelectCardMan.IsShow() )	m_cSelectCardMan.OnGameKey(nVKey, nextended, repeat);
	else
	{
		if (g_ObserverMan.IsMyObserver() == TRUE ) return;
		if ( nVKey == rule::SKEY_TAB ) //< CYS 100705 > // 단축키
		{	
			g_pChatWinMan->SelectNextTab();
			return;
		}

		CPlayer_NP *pPlayer = GetPlayerNP(0);

		if (pPlayer->JoinState == 0) return;

 			// ### [ 관전기능 ] ###
 		if (!g_pGameView->m_cPlayerDrawManager.IsFlyWindCard() && pPlayer->m_nCardTotal == GetTotalCardNum() && !g_pGameView->m_cPlayerDrawManager.IsHiddenCardShow())
 		{
			if ( ( IS_VKEY( G_KEYDEF[ KEY_HIDDEN_OPEN ][ 0 ], nVKey, nextended ) ) || 
		//		( IS_VKEY( G_KEYDEF[ KEY_HIDDEN_OPEN ][ 1 ], nVKey, nextended ) && g_Poker.CurPlayer != pPlayer->ServPNum ) || //< CYS 100705 > // delete 키는베팅시제외
				IS_VKEY( G_KEYDEF[ KEY_HIDDEN_OPEN ][ 1 ], nVKey, nextended ) ||
				IS_VKEY( G_KEYDEF[ KEY_HIDDEN_OPEN ][ 2 ], nVKey, nextended ) )
			{	
 				if (g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME )
 					pPlayer->SetFaceUp(7); // 보이기									
 				else 
 					pPlayer->SetFaceUp(6); // 보이기				
	 			
	 		
 				pPlayer->m_bHiddenOpen = TRUE; // 카드 클래스에서 보이기 한다.
				PBPlayEffectSound(SND_HIDDEN_OPEN);
				g_pGameView->m_cPlayerDrawManager.SetHiddenCardShow(true);

				int nOldPoint = GAME()->m_cSvRule.get_MyRule(0);

 				SetJokerCard();
 				g_pGameView->m_cGameHelper.CalculationProbability();
				g_pGameView->m_cPlayerDrawManager.MyJokboNameUpdate();
				
				pPlayer->DrawEft_CompleteMadeValue( nOldPoint );

			}
			else if ( IS_VKEY( G_KEYDEF[ KEY_HIDDEN_SLOW ][ 0 ], nVKey, nextended ) || 
				IS_VKEY( G_KEYDEF[ KEY_HIDDEN_SLOW ][ 1 ], nVKey, nextended ) )
			{
				//if ( m_nHinddenArrow == 0 )
				if ( GetHiddenArrowPosY() == 0 )
				{
 					pPlayer->m_bHiddenOpen = TRUE; // 카드 클래스에서 보이기 한다.
 					g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(false);
 					if (g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME)
 						pPlayer->SetFaceUp(7); // 보이기
 					else 
 						pPlayer->SetFaceUp(6); // 보이기
 				}

				SetkeyboardHiddenClick( TRUE );
				SetHiddenArrowPosY( GetHiddenArrowPosY() + 4 );

 				if ( GetHiddenArrowPosY() > 20 )
 				{
					PBPlayEffectSound(SND_HIDDEN_OPEN);
 					g_pGameView->m_cPlayerDrawManager.SetHiddenCardShow(true);
	 				
					int nOldPoint = GAME()->m_cSvRule.get_MyRule(0);

 					SetJokerCard();
 					g_pGameView->m_cGameHelper.CalculationProbability();
					// 자신의 카드위에 족보명을 보위기 위함
					g_pGameView->m_cPlayerDrawManager.MyJokboNameUpdate();

					pPlayer->DrawEft_CompleteMadeValue( nOldPoint );
 				}
			}
		}

		//규제안 관련 수정 : 게임중 조건과 현재 플레이어와 상관없이 게이키가 작동하도록 수정 (예약기능)
		if (/*g_RI.State == 1 && m_nCurPlayer == pPlayer->ServPNum &&*/ pPlayer->JoinState == 1 && pPlayer->PlayState == 1 && !g_pGameView->m_cPlayerDrawManager.IsFlyWindCard())
		{
			for ( int i = KEY_BET_DIE; i <= KEY_BET_HAF; i++ )
			{
				for ( int j = 0; j < MAX_GAME_KEY; j++ )
				{
					if ( IS_VKEY( G_KEYDEF[ i ][ j ], nVKey, nextended ) )
					{
						switch( i )
						{
						case KEY_BET_DIE:
							{
								if ( g_Poker.nAutoBetNodie != 1 ) 
								{
									g_pGameView->OnBtnFold();
								}
								break;
							}
						case KEY_BET_CHK:
							{
								if ( g_Poker.nAutoBetCmd <= 0)
								{
									g_pGameView->OnBtnCheck();
								}
								break;
							}
						case KEY_BET_CAL:
							{
								if ( g_Poker.nAutoBetCmd <= 0)
								{
									g_pGameView->OnBtnCall();
								}
								break;
							}
						case KEY_BET_PNG:
							{
								if ( g_Poker.nAutoBetCmd <= 0)
								{
									g_pGameView->OnBtnPing();
								}
								break;
							}
						case KEY_BET_DAD:
							{
								if ( g_Poker.nAutoBetCmd <= 0)
								{
									g_pGameView->OnBtnDda();
								}
								break;
							}
						case KEY_BET_HAF:
							{
								if ( g_Poker.nAutoBetCmd <= 0)
								{
									g_pGameView->OnBtnMax();
								}
								break;
							}
						}
					}
				}
			}
		}
	}
}

void CGameProcess_NewPoker::CheckMinimumUser(void)
{
	int nMaxPlayer = GM().GetMaxPlayer();
	CPlayer_NP *pPlayer = NULL;
	int nPlayer = 0;
	for (int i=0;i<nMaxPlayer;i++)
	{
		pPlayer = (CPlayer_NP *)GetPlayer(i);
		if (strlen(pPlayer->UI.ID) > 0)
			nPlayer++;
	}

	if (nPlayer < 2) g_pGameView->m_cDisplayBoard.SetText(EDBT_MINUSER);
}

bool CGameProcess_NewPoker::Accept_CreateRoom(void *pMsgBase) // 방만들기 허가
{
	CSV_ACCEPT_CREATEROOM *pMsg = (CSV_ACCEPT_CREATEROOM *)pMsgBase;

	if (GM().GetCurrentWhere()!=IDX_GAMEWHERE_LOBY) return false;

	ResetGame();
	g_pGameView->m_cGameHelper.OnEnterRoom();
	
	memcpy(&g_RI, pMsg->RI, sizeof(ROOMINFO));
	g_pGameView->m_cTitle.MakeTitle();
	GM().SetWaitResponse(false);
	SetAccessRoomState(false);
	// 게임 정보 구조체를 초기화 한다	
	int i=0;
	const int nMaxPlayer = GetMaxPlayer();
	for (i=1; i<nMaxPlayer; i++) GetPlayerNP(i)->Clear();
	for (i = 0 ; i < nMaxPlayer ; i ++){		
		GetPlayerNP(i)->InitRoomData();
	}

	g_pGameView->m_cPlayerDrawManager.OnEnterRoom();

	GetPlayerNP(0)->JoinState = 0;
	// ### [ 중복된 IP가 ] ###
	GetPlayerNP(0)->MyIpCheck = -1;
	
	m_nNo1PosPNum = 0;
	m_nMyPosPNum = 0;
	
	// 서버측 플레이어 번호 지정
	for (i=0; i<nMaxPlayer; i++)	{ GetPlayerNP(i)->ServPNum = i; }

	memcpy(&GetPlayerNP(0)->UI,&g_pMyInfo_NP->UI,sizeof(GetPlayerNP(0)->UI));

	//Buy in 맥스방 UI 머니셋팅 방만들기
	if ( g_RI.bIsMaxRoom )
	{
		//설정머니보다 자산이 많을경우 설정 머니로 셋팅한다.
		if ( g_RI.ChipKind==0 && g_RI.llInMoney > GetPlayerNP(0)->UI.GetMoney() )
		{
			GetPlayerNP(0)->UI.SetInMoney( GetPlayerNP(0)->UI.GetMoney() );
			GM().GetMyInfo()->UI.SetInMoney( GetPlayerNP(0)->UI.GetMoney() );
		}
		else if ( g_RI.ChipKind==1 && g_RI.llInMoney > GM().GetMyGold() )
		{
			GetPlayerNP(0)->UI.SetInMoney( GM().GetMyGold()  );
			GM().GetMyInfo()->UI.SetInMoney( GM().GetMyGold()  );
		}
		else
		{
			GetPlayerNP(0)->UI.SetInMoney( g_RI.llInMoney );
			GM().GetMyInfo()->UI.SetInMoney( g_RI.llInMoney );
		}
	}

	// ### [ 관전기능 ] ###
	g_pMyInfo_NP->JoinState = 0;
	g_pMyInfo_NP->MyIpCheck = -1;
	g_pMyInfo_NP->ServPNum = GetPlayerNP(0)->ServPNum;
	
	g_MyPN = g_MastPN = 0;// 내가 방장

	GM().ReLoadSprite(g_Config.nSKinKind);
	m_nCurPlayer = 0;// 방처음 개설자가 선
	
	// 캐릭터창 초기화
	if (g_RI.MaxUserNum > nMaxPlayer) g_RI.MaxUserNum=nMaxPlayer;
	
	
	// 버튼 활성화
	g_pGameView->m_cGameViewBtnMan.EnterRoom_Button_Init( g_RI.sBetRuleKind );
	//g_pGameView->m_cGameViewBtnMan.m_btnExit.Enable(TRUE);

//	PlayMusic();
	
	g_pGameView->m_nRoomKindCnt = 0 ;
	g_pGameView->m_nTipCnt = 0;

	g_cPromotionMan.Accept_CreateRoom();

	m_cAniSprObjectMan.Destroy();

	DWORD em_dwCurTime = timeGetTime();

	// 입장 음성 사운드 
	//특정 조건일때 큰 족보로 틀어준다.
	g_cBanishMan.Clear();

	// 규제안 입장 유저의 손실머니 정보 추가.
	INT64 lossMoney = GM().GetLossMaxMoney() + GM().GetLossMoney();
	g_pGameView->m_cPlayerDrawManager.OnEnterRoomLimitMoney(0, lossMoney);

	//규제안 : 방입장시에 손실금액 경고 문구 상태 체크
	g_pGameView->m_cGameViewBtnMan.SetLossMoneyWarning(true) ;

	//규제안 : 대기상태의 방정보를 설정한다.
	g_pGameView->m_cGameViewBtnMan.m_bIsWaitUser = false;

	return true;
}


// 방에 입장 허가 받음
bool CGameProcess_NewPoker::Accept_EnterRoom(void *pMsgBase) 
{
	CSV_ACCEPT_ENTERROOM *pMsg = (CSV_ACCEPT_ENTERROOM *)pMsgBase;

	if ( pMsg == NULL )
	{
		// ### [ 관전기능 ] ###
		CSV_ASK_OUTROOM aormsg;
		aormsg.Set(g_RI.RoomNum, g_pMyInfo_NP->UI.UNum, GM().GetMyInfo()->UI.ID, REASON_OUTROOM_ABNORMAL);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), aormsg.pData, aormsg.GetTotalSize());
		// 연결종료
		g_MastPN = 0;
		CloseMainMessageDlg();
		return false;
	}

	g_pGameView->m_cPlayerDrawManager.OnEnterRoom();

	ResetGame();
	g_pGameView->m_cGameHelper.OnEnterRoom();

	char *strMast = pMsg->EI->Ri.ID;
	
	g_RI.FormKind = pMsg->EI->Ri.FormKind;
	g_MyPN = 0;
	int MyID=-1, MastID=-1;
	int i, k;
	

	//규제안 : 대기상태의 방정보를 설정한다.
	g_pGameView->m_cGameViewBtnMan.m_bIsWaitUser = (pMsg->EI->Ri.bWaitInRoom=='1');

	//규제안 : 방입장시에 손실금액 경고 문구 상태 체크
	g_pGameView->m_cGameViewBtnMan.SetLossMoneyWarning(true) ;

	//[vsmode]070314
	const int nMaxPlayer = GetMaxPlayer();
	const int nMaxNewPlayer = GetMaxNewPlayer();
	
	// ### [ 관전기능 ] ###
	for (i=0; i<nMaxNewPlayer; i++) {
		if (strlen(pMsg->EI->Ui[i].ID) > 0) {			
			if (strcmp(strMast, pMsg->EI->Ui[i].ID)==0) MastID = i; // 방장 아이디 찾기
			if (strcmp(GM().GetMyInfo()->UI.ID, pMsg->EI->Ui[i].ID)==0) MyID = i; // 나의 아이디
		}
	}

	g_pMyInfo_NP->SetChangeRoomInfo(pMsg->EI->Cui[MyID]);

	GM().SetWaitResponse(false);
	SetAccessRoomState(false);
	
	// 방장 혹은 나의 아이디를 찾을 수 없거나 방장과 내 아이디가 동일하다?
	if (MastID == -1 || MyID == -1 || MastID == MyID) {
		// ### [ 관전기능 ] ###
		CSV_ASK_OUTROOM aormsg;
		aormsg.Set(g_RI.RoomNum, g_pMyInfo_NP->UI.UNum, GM().GetMyInfo()->UI.ID, REASON_OUTROOM_ABNORMAL);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), aormsg.pData, aormsg.GetTotalSize());
		// 연결종료
		g_MastPN = 0;
		CloseMainMessageDlg();
		return false;
	}

	if (pMsg->EI->Ui[MyID].cObserver == 1)  { g_ObserverMan.SetMyObserver(TRUE); }
	g_MastPN = MastID;
	GM().ReLoadSprite(g_Config.nSKinKind);

	// 방 정보 복사
	memcpy(&g_RI, &pMsg->EI->Ri, sizeof(ROOMINFO));
	if (g_RI.State == 1)
	{
		g_pGameView->m_cTotalBetMoney.Start();
	}
	g_pGameView->m_cTitle.MakeTitle();
	
	if (g_RI.MaxUserNum > nMaxPlayer) g_RI.MaxUserNum=nMaxPlayer;
	
	// 접속 메세지 대화창 닫기
	CloseMainMessageDlg();
	
	memcpy(&g_Poker, &pMsg->EI->Pg,sizeof(POKERGAME));
	g_pGameView->m_cSidePotMoney.Check();

	//맥스방 UI 머니셋팅 방만들기
	if ( pMsg->EI->Ri.bIsMaxRoom )
	{
		//설정머니보다 자산이 많을경우 설정 머니로 셋팅한다.
		if ( pMsg->EI->Ri.ChipKind == 0 && pMsg->EI->Ri.llInMoney > GetPlayerNP(0)->UI.GetMoney() )
		{
			GetPlayerNP(0)->UI.SetInMoney( GetPlayerNP(0)->UI.GetMoney() );
			GM().GetMyInfo()->UI.SetInMoney( GetPlayerNP(0)->UI.GetMoney() );
		}
		else if ( pMsg->EI->Ri.ChipKind == 1 && pMsg->EI->Ri.llInMoney > GM().GetMyGold() )
		{
			GetPlayerNP(0)->UI.SetInMoney( GM().GetMyGold()  );
			GM().GetMyInfo()->UI.SetInMoney( GM().GetMyGold()  );
		}
		else
		{
			GetPlayerNP(0)->UI.SetInMoney( pMsg->EI->Ri.llInMoney );
			GM().GetMyInfo()->UI.SetInMoney( pMsg->EI->Ri.llInMoney );
		}
	}
	
	if ( !g_ObserverMan.IsMyObserver() ) // 관전이 아니라면
	{		
		GetPlayerNP(0)->JoinState = 0;
		g_pMyInfo_NP->JoinState = 0;
		m_nMyPosPNum = MyID; // 서버쪽에 인덱스
		m_nNo1PosPNum = GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스
		
		// 게임 정보 초기화
		for (i=1; i<nMaxNewPlayer; i++) GetPlayerNP(i)->Clear();
		for (i = 0 ; i < nMaxPlayer ; i ++)GetPlayerNP(i)->InitRoomData();
		// 서버측 플레이어 번호 지정
		for (i=0; i<nMaxNewPlayer; i++) {
			if (i<nMaxPlayer)
				GetPlayerNP(i)->ServPNum = GetServPNum_ByPN(i);
			else
				GetPlayerNP(i)->ServPNum = i;
		}
		
		g_pMyInfo_NP->ServPNum = GetPlayerNP(0)->ServPNum;
		
		// 플레이어 순서에 맞게 초기화한다
		CObserverDlg *pObserverDlg = GetObserverDlg();
		for (i=0; i<nMaxNewPlayer; i++) {
			if (i>=nMaxPlayer) { //관전자 아디셋팅
				if (strlen(pMsg->EI->Ui[i].ID) > 0) {
					GetPlayerNP(i)->SetNewPlayer(&pMsg->EI->Ui[i]);
					if (pMsg->EI->Ui[i].AdminGrade == 0) {
						// 관전자 리스트 출력
						int ounum = GetPlayerNP(i)->UI.UNum;
						int sex  = GetPlayerNP(i)->UI.Sex;
						int level = GetPlayerNP(i)->UI.nIcon;
						INT64 Money = GetPlayerNP(i)->UI.GetMoney();
						int seq = 0;
						pObserverDlg->User_InsItem(ounum, GetPlayerNP(i)->UI.ID,GetPlayerNP(i)->UI.NickName, sex, level, Money, seq);
					}
				}
				continue;
			}
			int pn = GetPNum_ByServPN(i);
			if (strlen(pMsg->EI->Ui[i].ID) > 0) {
				if (pn!=0) GetPlayerNP(pn)->SetNewPlayer(&pMsg->EI->Ui[i]);
				
				GetPlayerNP(pn)->JoinState = pMsg->EI->Pg.PS[i].JoinState;
				GetPlayerNP(pn)->PlayState = pMsg->EI->Pg.PS[i].PlayState;
				GetPlayerNP(pn)->MyIpCheck = pMsg->EI->Pg.PS[i].nCheckIp;   // ### [ 중복된 IP가 ] ###
				GetPlayerNP(pn)->nSndFxKind = pMsg->EI->Pg.PS[i].nSndFxKind; // ### [사운드 추가 작업] ###
				GetPlayerNP(pn)->UI.nIcon = GetMoneyGrade(pMsg->EI->Ui[i].GetTotalMoney());
				GetPlayerNP(pn)->SetChangeRoomInfo(pMsg->EI->Cui[i]);

				// 게임 진행 상황 그릴것 카드 정보를 카드덱에다가 셋
				if (GetPlayerNP(pn)->JoinState==1) {
					int nTotCnt = pMsg->EI->Pg.PS[i].nCardTotal;
					int nOpCnt =  pMsg->EI->Pg.PS[i].nOpenTotal;


					//규제안 : 현재 받은 카드 개수 설정
					g_nCardTotalCount = nTotCnt;

					GetPlayerNP(pn)->bFold = pMsg->EI->Pg.PS[i].bFold;
					GetPlayerNP(pn)->m_nCardTotal = nTotCnt;
					GetPlayerNP(pn)->m_nOpenTotal = nOpCnt;
					for (k = 0; k < nTotCnt; k++) {
						GetPlayerNP(pn)->SetNewCard(DUMY_CARD); // 카운트 자동증가
						CPoint pt = GetPlayerNP(pn)->m_listCard[k].GetPos();
						GetPlayerNP(pn)->m_listCard[k].GetMove().SetCurPos(pt);
						GetPlayerNP(pn)->m_listCard[k].GetMove().SetMoveEnd();
					}
					
					if (g_Poker.nState > RSTATE_PREPARESTART && nOpCnt > 0 && !GetPlayerNP(pn)->bFold) {						

						SetEnterRoomCardDeck(nOpCnt,pn);
					}
				}
			}
		}

		// 버튼 상태 설정
		m_nCurPlayer = g_Poker.CurPlayer;
		g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();
	}
	else //관전
	{
		// 서버 클라이언트 1 : 1
		g_MyIndex = MyID;
		m_nMyPosPNum = 0;    // 서버쪽에 인덱스
		m_nNo1PosPNum = GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스
		for (i=0; i<nMaxNewPlayer; i++) {
			GetPlayerNP(i)->Clear(); // 게임 정보 초기화
			GetPlayerNP(i)->InitRoomData();
			GetPlayerNP(i)->ServPNum = i;
		}
		g_pMyInfo_NP->ServPNum = MyID;
		
		// 플레이어 순서에 맞게 초기화한다
		CObserverDlg *pObserverDlg = GetObserverDlg();
		CPlayer_NP *pPlayer = NULL;
		for (i=0; i<nMaxNewPlayer; i++) {
		//	int pn = GetPNum_ByServPN(i);
			int pn = i;
			if (strlen(pMsg->EI->Ui[i].ID) > 0)	{
				pPlayer = GetPlayerNP(pn);
				pPlayer->SetNewPlayer(&pMsg->EI->Ui[i]);
				if (i>=nMaxPlayer) // 관전자 리스트 출력
				{
					if (pMsg->EI->Ui[i].AdminGrade == 0) 
					{
						CPlayer_NP *pPlayer2 = GetPlayerNP(i);
						int ounum = pPlayer2->UI.UNum;
						int sex  = pPlayer2->UI.Sex;
						int level = pPlayer2->UI.nIcon;
						INT64 Money = pPlayer2->UI.GetMoney();
						int seq = 0;
						pObserverDlg->User_InsItem(ounum,pPlayer2->UI.ID, pPlayer2->UI.NickName, sex, level,Money, seq);
					}
					pPlayer->UI.nIcon = GetMoneyGrade(pMsg->EI->Ui[i].GetTotalMoney());

					//게임 입장시 초기값을 저장한다.
					pPlayer->SetChangeRoomInfo(pMsg->EI->Cui[i]);
					continue;
				}
				
				pPlayer->JoinState = pMsg->EI->Pg.PS[i].JoinState;
				pPlayer->PlayState = pMsg->EI->Pg.PS[i].PlayState;
				pPlayer->MyIpCheck = pMsg->EI->Pg.PS[i].nCheckIp; // ### [ 중복된 IP가 ] ###
				pPlayer->nSndFxKind = pMsg->EI->Pg.PS[i].nSndFxKind;// ### [사운드 추가 작업] ###
				pPlayer->UI.nIcon = GetMoneyGrade(pMsg->EI->Ui[i].GetTotalMoney());
				pPlayer->SetChangeRoomInfo(pMsg->EI->Cui[i]);

				// 게임 진행 상황 그릴것 카드 정보를 카드덱에다가 셋
				if (pPlayer->JoinState==1)
				{
					int nTotCnt = pMsg->EI->Pg.PS[i].nCardTotal;
					int nOpCnt =  pMsg->EI->Pg.PS[i].nOpenTotal;

					//규제안 : 현재 받은 카드 개수 설정
					g_nCardTotalCount = nTotCnt;
					
					pPlayer->bFold = pMsg->EI->Pg.PS[i].bFold;
					pPlayer->m_nCardTotal = nTotCnt;
					pPlayer->m_nOpenTotal = nOpCnt;
					for (k = 0; k < nTotCnt; k++)
					{
						pPlayer->SetNewCard(DUMY_CARD); // 카운트 자동증가
						CPoint pt = pPlayer->m_listCard[k].GetPos();
						pPlayer->m_listCard[k].GetMove().SetCurPos(pt);
						pPlayer->m_listCard[k].GetMove().SetMoveEnd();
					}
					
					if (g_Poker.nState > RSTATE_PREPARESTART && nOpCnt > 0 && !GetPlayerNP(pn)->bFold)
					{
						SetEnterRoomCardDeck(nOpCnt,pn);
					}
				}
			}
		}
		
		// 버튼 상태 설정
		m_nCurPlayer = g_Poker.CurPlayer;
		g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();
		
		//관전UI
		g_ObserverMan.OnObserve();
	}

	if (!g_Poker.bDistCard && g_Poker.nAutoBetCmd <=0 && g_RI.State == 1 && g_Poker.nRound >= 1)
	{
		// 규제안 추가 카드세팅으로 딜링 일때는 턴 표시 안함.
		if ( CheckReceivedAllCards() )
		{
			g_pGameView->m_cPlayerDrawManager.SetTurn(GetPNum_ByServPN(m_nCurPlayer));
		}
	}
	
	//2004.06.29 시작버튼
	if (g_RI.State == 0 && g_RI.NowUserNum >= 3 && g_RI.NowUserNum<g_RI.MaxUserNum)
	{
		// 메세지
		CString str("방장이 시작버튼을 누르면 게임이 시작됩니다.\n");
		AddGameInfoViewEdit(str, GetChatColor(ECC2_NOTIFY));
	}

	m_cAllBetChip.SetMoney(GetRealBet());

	if (g_RI.State == 0){		
		g_pGameView->m_nRoomKindCnt = 0 ;
	}
	else{
		g_pGameView->m_nRoomKindCnt = 150 ;
	}
	g_pGameView->m_nTipCnt = 0;

//	PlayMusic();

	g_cPromotionMan.Accept_EnterRoom();

	// 입장 음성 사운드 
	//특정 조건일때 큰 족보로 틀어준다.	
	m_cAniSprObjectMan.Destroy();

	DWORD em_dwCurTime = timeGetTime();

	

	return true;
}


// 유저가 방에 입장 알림
void CGameProcess_NewPoker::User_EnterRoom(void *pMsgBase)
{
	CSV_USERENTERROOM *pMsg = (CSV_USERENTERROOM *)pMsgBase;

	if (GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME) return;

	PBPlayEffectSound(SND_USERIN);
	int i=0;
	int pnum = -1; // 플레이어 번호

	const int nMaxPlayer = GetMaxPlayer();
	const int nMaxNewPlayer = GetMaxNewPlayer();

	if ( !g_ObserverMan.IsMyObserver() && pMsg->UI->cObserver!=1 ) // 1) 내가 겜참여중이고 겜참여 넘 입장
	{
		for (i=m_nNo1PosPNum; i<nMaxPlayer+m_nNo1PosPNum; i++) {// 서버쪽의 0번인덱스 클라이언트 인덱스 부터
			int index = i;
			if (index>=nMaxPlayer) index = index - nMaxPlayer;
			if (index==0) continue; //나
			if (strlen(GetPlayerNP(index)->UI.ID)==0){pnum=index; break;}
		}
		if (pnum == -1) return; // 비어있는 곳을 찾지 못했으면
		
		// 방 정보를 수정하고 새로 참여한 플레이어의 정보를 저장
		g_RI.NowUserNum += 1;
		GetPlayerNP(pnum)->SetNewPlayer(pMsg->UI);
		g_pGameView->m_cPlayerDrawManager.OnEnterRoom(pnum);

		// 규제안 입장 유저의 손실머니 정보 추가.
		g_pGameView->m_cPlayerDrawManager.OnEnterRoomLimitMoney(pnum, *pMsg->llMaxLossMoney);
		
		GetPlayerNP(pnum)->ServPNum = GetServPNum_ByPN(pnum);// 서버측 플레이어 번호 저장
		
		CString str;
		str.Format("▶입장: [%s]님\n", pMsg->UI->NickName);
		AddGameInfoViewEdit(str, GetChatColor(ECC2_NOTIFY));
		// 닉네임수정 	2006/10/30
		str.Format("## <ID: %s(%s)>님 입장 ##\n", pMsg->UI->ID , pMsg->UI->NickName);
		AddLobbyHideChatText( &str );
		//여기까지
	}
	//2) 내가 겜중이고 관전인넘 입장
	//else if (!g_ObserverMan.IsMyObserver()) 
	//{
	//}
	// 3)내가 관전중이고  겜참여 하는넘 입장
	else if (pMsg->UI->cObserver!=1)
	{
		for (i=0; i<nMaxPlayer; i++) {
			if (strlen(GetPlayerNP(i)->UI.ID)==0){pnum=i; break;}
		}
		if (pnum == -1) return; // 비어있는 곳을 찾지 못했으면
		
		m_nMyPosPNum = 0; // 서버쪽에 인덱스
		m_nNo1PosPNum = GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스
		
		// 방 정보를 수정하고 새로 참여한 플레이어의 정보를 저장
		g_RI.NowUserNum += 1;
		GetPlayerNP(pnum)->SetNewPlayer(pMsg->UI);
		GetPlayerNP(pnum)->Reset(); // 이벤트를 알수 있다면 클리어 하지 않는다.
		g_pGameView->m_cPlayerDrawManager.OnEnterRoom(pnum);
		// 규제안 입장 유저의 손실머니 정보 추가.
		g_pGameView->m_cPlayerDrawManager.OnEnterRoomLimitMoney(pnum, *pMsg->llMaxLossMoney);

		GetPlayerNP(pnum)->ServPNum = GetServPNum_ByPN(pnum);// 서버측 플레이어 번호 저장
		
		CString str;
		str.Format("▶입장: [%s]님\n", pMsg->UI->NickName);
		AddGameInfoViewEdit(str, GetChatColor(ECC2_NOTIFY));
		// 닉네임수정 	2006/10/30
		str.Format("## <ID: %s(%s)>님 입장 ##\n", pMsg->UI->ID , pMsg->UI->NickName);
		AddLobbyHideChatText( &str );
		
	}
	// 2)내가 겜중이고 관전인넘 입장
	// 4)내가 관전 이고 관전인넘 입장
	else
	{
		if (pMsg->UI->AdminGrade > 0) {
			pnum = nMaxNewPlayer-1;//관리자
		}
		else {
			for (i=nMaxPlayer; i<nMaxNewPlayer-1; i++) {
				if (strlen(GetPlayerNP(i)->UI.ID)==0){pnum=i; break;}
			}
		}
		if (pnum == -1) return;

		GetPlayerNP(pnum)->SetNewPlayer(pMsg->UI);// 방 정보를 수정하고 새로 참여한 플레이어의 정보를 저장
		GetPlayerNP(pnum)->ServPNum = pnum;// 서버측 플레이어 번호 저장
		
		if ( pMsg->UI->AdminGrade==0) {// 관리자가 아니면 
			g_RI.NowObserverNum += 1;
			CString str;
			str.Format("▶관전 입장: [%s]님\n", pMsg->UI->NickName);
			AddGameInfoViewEdit(str, GetChatColor(ECC2_NOTIFY));

				// 닉네임수정 	2006/10/30
			str.Format("## <ID: %s(%s)>님 입장 ##\n", pMsg->UI->ID , pMsg->UI->NickName);
			AddLobbyHideChatText( &str );
			
			// 관전자 리스트 출력
			int ounum = GetPlayerNP(pnum)->UI.UNum;
			int sex  = GetPlayerNP(pnum)->UI.Sex;
			int level = GetPlayerNP(pnum)->UI.nIcon;
			INT64 Money = GetPlayerNP(pnum)->UI.GetMoney();
			int seq = 0;
			CObserverDlg *pObserverDlg = GetObserverDlg();
			pObserverDlg->User_InsItem(ounum, GetPlayerNP(pnum)->UI.ID , GetPlayerNP(pnum)->UI.NickName, sex, level, Money, seq);
		}
	}
	
	// ### [ 중복된 IP가 ] ###
	for (i=0; i<nMaxPlayer; i++) {
		int pn = GetPNum_ByServPN(i);
		if (strlen(GetPlayerNP(pn)->UI.ID) > 0) {
			GetPlayerNP(pn)->MyIpCheck = pMsg->IPA->IpIndex[i];
		}
	}
	GetPlayerNP(pnum)->nSndFxKind = *pMsg->SndFxKind;// ### [사운드 추가 작업] ###

	g_pGameView->m_cTitle.MakeTitle();
	
	
	if (g_RI.NowUserNum >= g_RI.MaxUserNum) {
		g_pGameView->m_cGameViewBtnMan.m_btnGameStart.Enable(FALSE);
		g_pGameView->m_cGameViewBtnMan.m_btnGameStart.Show(FALSE);
	}

	if (!g_ObserverMan.IsMyObserver()) CheckMinimumUser();
}

void CGameProcess_NewPoker::User_OutRoom(void *pMsgBase) // 유저가 방에서 나감 알림
{
	CSV_USEROUTROOM *pMsg = (CSV_USEROUTROOM *)pMsgBase;
	if (GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME) return;
	
	PBPlayEffectSound(SND_USEROUT);
	int pnum=-1;
	int i=0;
	
	const int nMaxNewPlayer = GetMaxNewPlayer();
	const int nMaxPlayer = GetMaxPlayer();

	for (i=0; i<nMaxNewPlayer; i++) {
		if (strcmp(GetPlayerNP(i)->UI.ID, pMsg->ID)==0) {pnum=i; break;}
	}
	if (pnum == -1) return;

	if (m_cAniSprObjectMan.pFirst != NULL)
	{
		CBaseObject *pNow = NULL;
		CBaseObject* pNext = m_cAniSprObjectMan.pFirst;

		while(pNext)
		{
			CBaseObject* pNow = pNext;

			if (pNow->m_nObjID == ANF_BET_CMD)
			{
				CBetCmdAni* pObj = (CBetCmdAni*)pNow;
				//새로 찍어야 할때면 기존거는 지우고 다시 찍는다.
				if (pObj->m_nPNum  == pnum)
				{
					pObj->Destroy();
				}
			}
			pNext = pNow->pNextNode;
		}
	}
	
	// 닉네임수정 
	CString NickName =  GetPlayerNP(pnum)->UI.NickName;
	
	if ( pnum<nMaxPlayer ) //겜중인넘
	{
		// [ 카드 날리기 ]
		if (GetPlayerNP(pnum)->bFold == FALSE && GetPlayerNP(pnum)->JoinState == 1 && GetPlayerNP(pnum)->PlayState == 1)
		{	//현재 겜중인 넘 나갔다.
			m_nStartUserNum--;
			if (m_nStartUserNum <0) m_nStartUserNum = 0;
		}
		
		GetPlayerNP(pnum)->Clear(); // UI 최기화
		g_pGameView->m_cPlayerDrawManager.OnExitRoom(pnum);
		// ##### 카드 정보 클리어 ##### 
		GetPlayerNP(pnum)->Reset(); // 이벤트를 알수 있다면 클리어 하지 않는다.
		
		if (g_RI.NowUserNum > 0) g_RI.NowUserNum -= 1;
		
		CString str;
		str.Format("◁퇴장: [%s]님\n", NickName );
		// 여기까지
		AddGameInfoViewEdit(str, GetChatColor(ECC2_NOTIFY));
		// 닉네임수정 	2006/10/30
		str.Format("## <ID: %s(%s)>님 퇴장 ##\n",pMsg->ID , NickName );
		AddLobbyHideChatText( &str );
		//여기까지
		
	}
	else // 관전인넘
	{
		if (GetPlayerNP(pnum)->UI.AdminGrade == 0) {
			CObserverDlg *pObserverDlg = GetObserverDlg();
			pObserverDlg->User_DelItem(GetPlayerNP(pnum)->UI.UNum, GetPlayerNP(pnum)->UI.ID);
			
			if (g_RI.NowObserverNum > 0) g_RI.NowObserverNum -= 1;
			CString str;
			str.Format("◁관전 퇴장:[%s]님\n", GetPlayerNP(pnum)->UI.NickName);
			AddGameInfoViewEdit(str, GetChatColor(ECC2_NOTIFY));
		}
		GetPlayerNP(pnum)->Clear(); // UI 최기화
	}

	// ### [ 중복된 IP가 ] ###
	for (i=0; i<nMaxPlayer; i++)	{
		int pn = GetPNum_ByServPN(i);
		if (strlen(GetPlayerNP(pn)->UI.ID) > 0) {
			GetPlayerNP(pn)->MyIpCheck = pMsg->IPA->IpIndex[i];
		}
	}

	g_pGameView->m_cTitle.MakeTitle();
	
	if (g_RI.NowUserNum <3 && g_RI.State == 0)
	{
		m_cAniSprObjectMan.Destroy();
	}

	if (!g_ObserverMan.IsMyObserver()) CheckMinimumUser();
}

BOOL CGameProcess_NewPoker::IsPlayingNow()
{
	return m_bGameStart;
}

// bPaticipation = TRUE  관전 -> 참여
// bPaticipation = FALSE 참여 -> 관전
void CGameProcess_NewPoker::SetPlayInfo(int nowpnum, int newpnum, BOOL bParticipation )
{
	//백업용 - 포인터 사용할까?
	const int nMaxPlayer = GetMaxPlayer();
	USERINFO *tmpUi = new USERINFO[nMaxPlayer];
	ZeroMemory(tmpUi, sizeof(USERINFO) * nMaxPlayer);

	PLAYSTATE *tmpPs = new PLAYSTATE[nMaxPlayer];
	ZeroMemory(tmpPs, sizeof(PLAYSTATE) * nMaxPlayer);

	BACKCARD  *tmpBC = new BACKCARD[nMaxPlayer];
	ZeroMemory(tmpBC, sizeof(BACKCARD) * nMaxPlayer);

	CHANGE_USERINFO_ROOM *tmpCUi = new CHANGE_USERINFO_ROOM[nMaxPlayer];
	ZeroMemory(tmpCUi, sizeof(CHANGE_USERINFO_ROOM) * nMaxPlayer);

	struct tempcarddeck{
		char nCard[10];
	};

	bool *tmpSelectCard = new bool[nMaxPlayer];
	ZeroMemory(tmpSelectCard, sizeof(bool) * nMaxPlayer);

	tempcarddeck *tCardDeck = new tempcarddeck[nMaxPlayer];
	ZeroMemory(tCardDeck, sizeof(tempcarddeck) * nMaxPlayer);
	
	if ( !bParticipation ) {
		//현재 겜참여 중이기 때문에 0 번 인덱스
		//기본 정보 초기화
		GetPlayerNP(0)->Clear();
	}

	// 기존 플레이 정보 backup
	int pn = 0, k;
	int i=0;
	CPlayer_NP *pPlayer = NULL;
	for (i=0; i<nMaxPlayer; i++) 
	{
		pPlayer = GetPlayerNP(i);
		if (strlen(pPlayer->UI.ID)==0) continue;
		if (bParticipation )// 관전 -> 참여
			pn = i;
		else
			pn = GetServPNum_ByPN(i);

		memcpy(&tmpUi[pn], &pPlayer->UI, sizeof(USERINFO));
		memcpy(&tmpCUi[pn], pPlayer->GetChangeRoomInfo(), sizeof(CHANGE_USERINFO_ROOM) );

		tmpPs[pn].JoinState = pPlayer->JoinState;
		tmpPs[pn].PlayState = pPlayer->PlayState;
			
		tmpPs[pn].bFold = pPlayer->bFold;
		tmpPs[pn].nCardTotal= pPlayer->m_nCardTotal;
		tmpPs[pn].nOpenTotal= pPlayer->m_nOpenTotal;
		
		tmpSelectCard[pn]   = g_pGameView->m_cPlayerDrawManager.IsSelectCard(i);


		//서버로부터 다시 받는다
		// ### [사운드 추가 작업] ###
		tmpPs[pn].nSndFxKind = pPlayer->nSndFxKind;

		int tcnt = GetTotalCardNum();
		for (k = 0; k < tcnt ; k++) {
			tCardDeck[pn].nCard[k] = pPlayer->m_listCard[k].GetCardNum();
		}
	}

	//기본 정보 초기화
	if (bParticipation) {   // 관전 -> 참여
		g_pMyInfo_NP->JoinState = 0;
		m_nMyPosPNum = newpnum; // 서버쪽에 새로운 인덱스
		m_nNo1PosPNum = GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스

		pPlayer = GetPlayerNP(nowpnum);
		pPlayer->Clear();
	}
	else {	//참여 -> 관전
		g_pMyInfo_NP->JoinState = 0;
		m_nMyPosPNum = 0; // 서버쪽에 새로운 인덱스
		m_nNo1PosPNum = GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스
	}

	g_pGameView->m_cPlayerDrawManager.OnExitRoom();
	
	// 게임 정보 모두 초기화
	for (i=0; i<nMaxPlayer; i++) {
		pPlayer = GetPlayerNP(i);
		pPlayer->Clear();
		pPlayer->ServPNum = GetServPNum_ByPN(i); // 초기화 되지 않는다.
		pPlayer->InitRoomData();
	}
	
	if (bParticipation ) {	// 관전 -> 참여
		g_pMyInfo_NP->UI.cObserver = 0;
		g_ObserverMan.SetMyObserver(FALSE);
		g_pMyInfo_NP->Reset();
		g_pMyInfo_NP->nSndFxKind = g_Config.SndFxKind;

		pPlayer = GetPlayerNP(0);
		pPlayer->SetNewPlayer(&g_pMyInfo_NP->UI);
		pPlayer->SetChangeRoomInfo(*g_pMyInfo_NP->GetChangeRoomInfo());
		pPlayer->nSndFxKind = g_Config.SndFxKind;
					
		g_RI.NowUserNum++;
		if (g_RI.NowObserverNum > 0) g_RI.NowObserverNum--;
		
		g_pMyInfo_NP->ServPNum = pPlayer->ServPNum;
	}
	else 
	{   //참여 -> 관전
		g_pMyInfo_NP->UI.cObserver = 1;
		g_ObserverMan.SetMyObserver(TRUE);
		g_pMyInfo_NP->Reset();

		pPlayer = GetPlayerNP(newpnum);
		pPlayer->SetNewPlayer(&g_pMyInfo_NP->UI);
		pPlayer->SetChangeRoomInfo(*g_pMyInfo_NP->GetChangeRoomInfo());
		pPlayer->nSndFxKind = g_pMyInfo_NP->nSndFxKind;
		
		g_pMyInfo_NP->ServPNum = newpnum;
		pPlayer->ServPNum = newpnum;

		g_RI.NowObserverNum++;
		if (g_RI.NowUserNum > 0) g_RI.NowUserNum--;

		//관전UI
		g_ObserverMan.OnObserve();
	}

	// 플레이어 순서에 맞게 초기화한다
	for ( i=0; i<nMaxPlayer; i++) {
		int pn = GetPNum_ByServPN(i);
		pPlayer = GetPlayerNP(pn);
		if (strlen(tmpUi[i].ID) > 0)	{
			if (bParticipation ) {
				if (pn!=0) pPlayer->SetNewPlayer(&tmpUi[i]);
			}
			else pPlayer->SetNewPlayer(&tmpUi[i]);
			
			pPlayer->JoinState = tmpPs[i].JoinState;
			pPlayer->PlayState = tmpPs[i].PlayState;
			pPlayer->nSndFxKind = tmpPs[i].nSndFxKind; // ### [사운드 추가 작업] ###
			g_pGameView->m_cPlayerDrawManager.SetSelectCard(pn, tmpSelectCard[i]);

			// 레벨상점 폐쇄
			pPlayer->UI.nIcon = GetMoneyGrade(tmpUi[i].GetTotalMoney());
			pPlayer->SetChangeRoomInfo(tmpCUi[i]);
			
			// 게임 진행 상황 그릴것 카드 정보를 카드덱에다가 셋
			if (pPlayer->JoinState==1) {
				int nTotCnt = tmpPs[i].nCardTotal;
				int nOpCnt =  tmpPs[i].nOpenTotal;
				
				//규제안 : 현재 받은 카드 개수 설정
				g_nCardTotalCount = nTotCnt;
				
				if (g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME)
				{
					// 폭탄 카드 셋 
					if (nOpCnt >=2 && nOpCnt <= 4)
					{
						m_cBombCard.ForceSet(52);
					}
					else if (nOpCnt >= 5)
					{
						m_cBombCard.ForceSet(g_Poker.nBombCard);
					}
				}
				
				pPlayer->bFold = tmpPs[i].bFold;
				pPlayer->m_nCardTotal = nTotCnt;
				pPlayer->m_nOpenTotal = nOpCnt;
				for (k = 0; k < nTotCnt; k++) {
					pPlayer->SetNewCard(DUMY_CARD); // 카운트 자동증가
					CPoint pt = pPlayer->m_listCard[k].GetPos();
					pPlayer->m_listCard[k].GetMove().SetCurPos(pt);
				}
				
				if (g_Poker.nState > RSTATE_PREPARESTART && nOpCnt > 0 && !pPlayer->bFold)	{
					if (nOpCnt <=  5) {
						for (k = 0; k < nTotCnt; k++) {
							pPlayer->m_listCard[k].SetCardNo(tCardDeck[i].nCard[k]);

							if (g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME){
								if (k == 6){
									pPlayer->m_listCard[k].SetBombCard(TRUE);
									pPlayer->m_listCard[k].SetBombShow(TRUE);

									if (g_RI.FormKind == JOKER_GAME){
										pPlayer->m_listCard[k].SetJokerCard(TRUE);
										
										if (nTotCnt == 8 && m_nGameState == GAME_ROUND_RESULT){
											int p = GetServPNum_ByPN(pn);
											if (m_cGameOverResultData.JokerCardDeck[p] >= 0){
												pPlayer->m_listCard[k].SetCardNo(m_cGameOverResultData.JokerCardDeck[p]);
											}
										}
									}
								}
							}

							if (g_RI.FormKind == JOKER_GAME)
							{
								if (pPlayer->m_listCard[k].GetCardNum() < 52 || pPlayer->m_listCard[k].GetCardNum() == 100){
									pPlayer->SetFaceUp(k);
								}
							}
							else{
								if (pPlayer->m_listCard[k].GetCardNum() < 52){
									pPlayer->SetFaceUp(k);
								}
							}
						}
					}
					else // 모두 카드 오픈
					{
						for (k = 0; k < nTotCnt; k++) {
							pPlayer->m_listCard[k].SetCardNo(tCardDeck[i].nCard[k]);

							if (g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME){
								if (k == 6)
								{
									if (g_RI.FormKind == JOKER_GAME){
										pPlayer->m_listCard[k].SetJokerCard(TRUE);

										if (nTotCnt == 8 && m_nGameState == GAME_ROUND_RESULT){
											int p = GetServPNum_ByPN(pn);
											if (m_cGameOverResultData.JokerCardDeck[p] >= 0){
												pPlayer->m_listCard[k].SetCardNo(m_cGameOverResultData.JokerCardDeck[p]);
											}
										}
									}
									pPlayer->m_listCard[k].SetBombCard(TRUE);
									pPlayer->m_listCard[k].SetBombShow(TRUE);
								}
							}
							pPlayer->SetFaceUp(k);
						}
					}
				}
			}
		}
	}

	for (i=0; i<nMaxPlayer; i++) {
		pPlayer = GetPlayerNP(i);
		if (pPlayer->UI.ID[0] != 0)
		{
			g_pGameView->m_cPlayerDrawManager.OnEnterRoom(i);
		}
	}

	// bPaticipation = TRUE  관전 -> 참여
	// bPaticipation = FALSE 참여 -> 관전
	CObserverDlg *pObserverDlg = GetObserverDlg();
	if (bParticipation) {
		pPlayer = GetPlayerNP(0);
		pObserverDlg->User_DelItem(pPlayer->UI.UNum, pPlayer->UI.ID);
	}
	else {
		pPlayer = GetPlayerNP(newpnum);
		int ounum = pPlayer->UI.UNum;
		int sex  = pPlayer->UI.Sex;
		int level = pPlayer->UI.nIcon;
		INT64 Money = pPlayer->UI.GetMoney();
		int seq = 0;

		pObserverDlg->User_InsItem(ounum,pPlayer->UI.ID, pPlayer->UI.NickName, sex, level, Money, seq);
	}

	CString str;
	if (bParticipation) {
		str.Format("참여하기를 선택 하였습니다.\n");
	}
	else {
		str.Format("관전하기를 선택 하였습니다.\n");
	}
	AddGameInfoViewEdit(str, GetChatColor(ECC2_NOTIFY));

	
	//[7월정책변경]2008.07.01
	// 오토배팅 셋팅
	if (!g_ObserverMan.IsMyObserver()){		
		AutoBetSetUpWnd().OnEnterRoom();
	}

	delete [] tmpUi;
	delete [] tmpPs;
	delete [] tmpBC;
	delete [] tmpCUi;
	delete [] tmpSelectCard;
	delete [] tCardDeck;
}

void CGameProcess_NewPoker::Init(NMBASE::GRAPHICGDI::CPage *ppage)
{
	m_pPage = ppage;

	m_cAllBetChip.Init(&g_pGameView->Page, &g_cUIData, g_pGameProcess, &g_sprChip);
	m_cResultFocus.Init(m_pPage);


	//오픈애니메이션추가
	//플레이할 멀티씬 인덱스가 직접 들어간다.
	m_NowCardOpenGuideIndex = 0;
	for ( int i = 2; i < MAX_CARDOPENGUIDE + 2; i++ )
	{
		m_CardOpenGuide.push_back( i );
	}

	random_shuffle( m_CardOpenGuide.begin(), m_CardOpenGuide.end() );
}

//오픈애니메이션추가
int CGameProcess_NewPoker::GetCardOpneGuideIndex()
{
	if ( g_Config.nbShowHidden == TRUE ) return -1;
	if ( m_NowCardOpenGuideIndex < 0 || m_NowCardOpenGuideIndex >= MAX_CARDOPENGUIDE ) return -1;

	return m_CardOpenGuide[ m_NowCardOpenGuideIndex ];
}

//오픈애니메이션추가
void CGameProcess_NewPoker::EndCardOpenGuide()
{ 
	if ( m_bDrawCardOpenGuide )
	{
		m_bDrawCardOpenGuide = FALSE;
		m_NowCardOpenGuideIndex++; 

		//아이디당 한번씩만 나오게 한다.
		if ( m_NowCardOpenGuideIndex >= MAX_CARDOPENGUIDE )
		{
			g_Config.nbShowHidden = TRUE;
			NMBASE::UTIL::WriteProfile_int( GM().GetMyInfo()->UI.ID, "SHOWHIDDEN",		g_Config.nbShowHidden );
		}
	}
}

void CGameProcess_NewPoker::SetCardOpenGuideTime()
{
	m_bDrawCardOpenGuide = TRUE;
	m_dwCardOpenGuideTime = timeGetTime();
}


void CGameProcess_NewPoker::DoPrepareGame(STARTINFO *pSC)
{
	if (pSC == NULL) return;
	
	g_RI.State = 1;

	g_pGameView->m_nRoomKindCnt = 150;
	g_pGameView->m_nTipCnt = 150;

	// 서버, 클라이언트 각각 선언하여 별도로 사용한다.
	ZeroMemory(&g_Poker, sizeof(POKERGAME));
	const int nMaxPlayer = GetMaxPlayer();
	memcpy(g_Poker.PS, pSC->Ps, sizeof(PLAYSTATE) * nMaxPlayer);

	if ( g_RI.CurSeedMoney != pSC->nSeedMoney ){
		g_RI.CurSeedMoney = pSC->nSeedMoney;
		g_pGameView->m_cTitle.MakeTitle();
	}

	//규제안 : 최초 맥스 활성 여부 초기화 : 맥스이펙트
	for (int i=0; i<MAX_PLAYER; i++)
	{
		GM().g_bFirstMax[i] = false;
	}

	//규제안 : 예약버튼 활성화 가능상태
	ReserveState(RESERVESTATE_BET_NONE);
	CanReserve(true);
	g_nCardTotalCount = 0 ; // 카드 세팅 비교값을 0으로 초기화.

	m_bGameOver = false ;

	// 게임 시작 플래그 세팅
	m_bGameStart = TRUE;
	PBPlayEffectSound(SND_START);
	BOOL bFalse = FALSE;
	int i=0;
	g_pGameView->m_cPlayerDrawManager.Reset();

	for (i=0; i<nMaxPlayer; i++) {		
		// 플레이어 스크린 초기화
		GetPlayerNP(i)->Reset();
	}
	m_nStartUserNum = 0;
	INT64 CurSeedMoney = (INT64)((double)g_RI.CurSeedMoney * (double)pSC->fBetMoneyMulti);
	INT64 tbetmoney = 0;
	for ( i=0; i<nMaxPlayer; i++) 
	{		
		if (strlen(pSC->JoinID[i]) >0)
		{
			int pn = GetPNum_ByServPN(i);
			if (strcmp(GetPlayerNP(pn)->UI.ID, pSC->JoinID[i]) != 0) { bFalse = TRUE; break; }
			
			// 플레이어 상태 세팅
			GetPlayerNP(pn)->JoinState = pSC->Ps[i].JoinState; //
			GetPlayerNP(pn)->PlayState = pSC->Ps[i].PlayState;
			GetPlayerNP(pn)->bFold     = FALSE;

			//GetPlayerNP(pn)->UI.SetMoney(GetPlayerNP(pn)->UI.GetMoney() - CurSeedMoney);//100;
			//g_Poker.RU[i].nPMoney = GetPlayerNP(pn)->UI.GetMoney();
			
			//MAX방 처리.
			INT64 i64TempBetMoney = GetPlayerNP(pn)->GetNowMoney() - CurSeedMoney;
			GetPlayerNP(pn)->SetNowMoney( i64TempBetMoney );

			g_Poker.RU[i].nPMoney = GetPlayerNP(pn)->GetNowMoney();

			GetPlayerNP(pn)->nSndFxKind = pSC->Ps[i].nSndFxKind;
			//yoo
			if (1 == pSC->Ps[i].nState){
				g_pGameView->m_cPlayerDrawManager.SetLimitMoney(pn, true);
			}
			
			// 유저 카드 셋팅 3장
			if ( pn == 0)
			{
				// ### [ 관전기능 ] ###
				if ( !g_ObserverMan.IsMyObserver() )
				{
					g_pMyInfo_NP->JoinState = GetPlayerNP(0)->JoinState;
					g_pMyInfo_NP->PlayState = GetPlayerNP(0)->PlayState;
					g_pMyInfo_NP->bFold = GetPlayerNP(0)->bFold;
					g_pMyInfo_NP->ServPNum = GetPlayerNP(0)->ServPNum;
				}
				
				GetPlayerNP(0)->SetNewCard( pSC->nCard[0]);
				GetPlayerNP(0)->SetNewCard( pSC->nCard[1]);
				GetPlayerNP(0)->SetNewCard( pSC->nCard[2]);
				GetPlayerNP(0)->SetNewCard( pSC->nCard[3]);

				m_cSelectCardMan.SetCard(g_RI.FormKind, pSC->nCard);
			}
			else
			{
				GetPlayerNP(pn)->SetNewCard( DUMY_CARD); // 53
				GetPlayerNP(pn)->SetNewCard( DUMY_CARD);
				GetPlayerNP(pn)->SetNewCard( DUMY_CARD);
				GetPlayerNP(pn)->SetNewCard( DUMY_CARD);
			}

			//무조건 초이스 모드다.
			GetPlayerNP(pn)->m_nCardTotal = 4;
			GetPlayerNP(pn)->m_nOpenTotal = 0; 
			g_Poker.RU[i].bInUse = TRUE;
			g_Poker.RU[i].nRealBat = CurSeedMoney;

			// 기본 배팅 칩을 던짐
			chip_draw_func(pn, g_Poker.RU[i].nRealBat, 1); // 여기서의 10은 단위다!!
			tbetmoney += g_Poker.RU[i].nRealBat;
			m_nStartUserNum++;
		}
	}
	
	m_cAllBetChip.SetMoney(tbetmoney);

	if (bFalse == TRUE) {
		// 강퇴 처리
		CString st, str;
		for (int i=0; i<nMaxPlayer; i++) 
		{
			int pn = GetPNum_ByServPN(i);
			st.Format("%d: %s, %s\n",i, GetPlayerNP(pn)->UI.NickName, pSC->JoinID[i]);
			str += st;
		}
		ExitGameRequest();
		ShowMainMessageDlg("잘못된 게임 시작정보로 인하여 퇴장합니다");
		ShowMainMessageDlg(str);
		return;
	}
	
	g_Poker.nState = RSTATE_PREPARESTART; // 서버쪽 셋팅이나
	g_Poker.nDistUser = pSC->nDistUser;
	m_nCurPlayer = pSC->nDistUser;

	//히스토리 관련 머니 셋팅 
	if (!g_ObserverMan.IsMyObserver() && g_pMyInfo_NP->JoinState == 1 && g_pMyInfo_NP->PlayState == 1)GetPlayerNP(0)->nRoundPrevMoney = GetPlayerNP(0)->UI.GetMoney();
	
	// 카드 날리기 => 카드 다날림 서버로
	Card_Distribute();

	g_DebugStartTick = timeGetTime();
	//자리 지킴이
	g_SeatKeeper.OnStartGame();

	g_cPromotionMan.OnStartGame();
	g_cVipJackpot.OnStartGame();
}

void CGameProcess_NewPoker::OnGameOver(GAMEOVERRESULT *pGOR)
{
	NMBASE::SOUND::g_Sound.Stop(SND_HIDDEN_BGM);
	m_nCurPlayer = -1;
	// 게임 결과 정보 
	ZeroMemory(&m_cGameOverResultData,sizeof(GAMEOVERRESULT));

	//규제안 관련하여 예약기능 모두 초기화.
	ReserveState(RESERVESTATE_BET_NONE);
	CanReserve(false);

	m_bGameOver = true ;

	g_pGameView->m_cGameHelper.Reset();
	m_cSelectCardMan.Clear();
	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();// 버튼 초기화
	g_pGameView->m_cPlayerDrawManager.SetOpen(pGOR->bOpen);
	g_pGameView->m_cPlayerDrawManager.SetWinnerPNum(-1);
	g_pGameView->m_cPlayerDrawManager.SetTurn(-1);
	memcpy(&g_Poker,&pGOR->Pg,sizeof(POKERGAME));
	g_pGameView->m_cSidePotMoney.Check();
	memcpy(&m_cGameOverResultData,pGOR,sizeof(GAMEOVERRESULT));

	CGameViewNewPoker *pGameView = (CGameViewNewPoker *)GM().GetCurrentGameView();
	pGameView->m_cDisplayBoard.SetText(EDBT_RESULT, (LPARAM)&m_cGameOverResultData);

	//베팅 커멘드 삭제
	if (m_cAniSprObjectMan.pFirst != NULL)
	{
		CBaseObject *pNow = NULL;
		CBaseObject* pNext = m_cAniSprObjectMan.pFirst;

		while(pNext)
		{
			CBaseObject* pNow = pNext;
			
			if (pNow->m_nObjID == ANF_BET_CMD)
			{
				CBetCmdAni* pObj = (CBetCmdAni*)pNow;
				pObj->SetDelTime(55);
			}
			pNext = pNow->pNextNode;
		}
	}

	//#ifdef _DEBUG
	/*
	// 각 유저가 받은 카드 인덱스(0 ~ 52) 확인
	TRACE( "\n=======================================================\n" );
	TRACE( "Card Info Open[ %d ]\n", pGOR->bOpen );

	for( int i = 0; i < MAX_PLAYER; i++)
	{
		if ( strlen( pGOR->Ui[ i ].ID ) > 0 ) //  겜참여자.
		{
			int pnum = GetPlayerPNum( pGOR->Ui[ i ].ID );

			TRACE( "%d. Player[ %s ] Fold[ %d ] JoinState[ %d ] PlayState[ %d ]: ",
				pnum, pGOR->Ui[ i ].NickName, pGOR->Pg.PS[i].bFold, pGOR->Pg.PS[i].JoinState, pGOR->Pg.PS[i].PlayState );

			for ( int k = 0; k < g_Poker.PS[pnum].nCardTotal; k++ ) TRACE( "%d, ", GetPlayerNP( pnum )->m_listCard[ k ].GetCardNum() );

			TRACE( "\n g_Poker.BC : " );

			for ( int k = 0; k < 3; k++ ) TRACE( "%d, ", g_Poker.BC[ i ].nCard[ k ] );

			TRACE( "\n g_Poker.PS : " );

			for ( int k = 0; k < 4; k++ ) TRACE( "%d, ", g_Poker.PS[ i ].nCard[ k ] );

			TRACE( "\n" );
		}
	}

	TRACE( "\n" );
	*/

	//////////////////////////////////////////////////////////////////////////
	// 기권오픈 확인

	// 버튼 spr 인덱스가 0이면 본인 카드 오픈 On, 1 이면 Off
	bool bUIFoldOpen = false;
	if ( g_pGameView->m_cGameViewBtnMan.m_btnCardOpen.GetButtonNumber() == 0 ) bUIFoldOpen = true;
	
	for( int i = 0; i < MAX_PLAYER; i++ )
	{
		if ( strlen( pGOR->Ui[ i ].ID ) > 0 ) //  겜참여자.
		{
			int pnum = GetPlayerPNum( pGOR->Ui[ i ].ID );
			if( pnum == -1 ) continue;
			
			// 클라이언트 0번 자리는 보통 본인기준으로 본인 카드 정보가 들어 오기 때문에 UI정보를 확인해준다.
			if ( pnum == 0 && !g_ObserverMan.IsMyObserver() )
			{
				if( !bUIFoldOpen ) continue;
			}
			
			// 기권이지만 카드 정보가 모두 0이 아니라면 기권오픈 상태로 카드 정보가들어 왔는걸로 간주
			int nCount = 0;
			
			for ( int k = 0; k < 3; k++ )
			{
				if( g_Poker.BC[ i ].nCard[ k ] > 0 ) nCount++;
			}
			
			for ( int k = 0; k < 4; k++ )
			{
				if( g_Poker.PS[ i ].nCard[ k ] > 0 ) nCount++;
			}
			
			if( nCount > 0 ) 
			{
				// 2018.09.27 이슈 발생으로 기권 오픈 기능 제거
				//GetPlayerNP(pnum)->bFoldOpen = true;
				
				//TRACE( "FoldOpen[ %s ] Fold[ %d ] \n", pGOR->Ui[ i ].NickName, pGOR->Pg.PS[ i ].bFold );
			}
		}
	}
	
	//TRACE( "=======================================================\n\n" );
	//#endif

	int i = 0;
	const int nMaxPlayer = GetMaxPlayer();
	//조커 카드를 넣는다 다른넘들
	if (g_Poker.nBombCard == 100 && g_RI.FormKind == JOKER_GAME && g_bGetJokerCard == TRUE)
	{
		for ( i=0; i<nMaxPlayer; i++)
		{
			if (strlen(pGOR->Ui[i].ID)>0)
			{				
				if (m_cGameOverResultData.JokerCardDeck[i] > -1)
				{
				//나를 제외한 나머지 넘들한테 넣는다
					int pnum = GetPNum_ByServPN(i); // 카드 날리는 순서
					int nJoker = m_cGameOverResultData.JokerCardDeck[i];
					
					if (pnum != 0)
					{
						if (GetPlayerNP(pnum)->bFoldOpen)
							GetPlayerNP(pnum)->m_listCard[6].SetCardNo(nJoker); // 카드 번호 세팅

						GetPlayerNP(pnum)->m_listCard[6].SetJokerCardNo(nJoker);
						GetPlayerNP(pnum)->m_listCard[6].SetJokerCard(TRUE);
					}
					else if (pnum == 0)
					{
						GetPlayerNP(0)->m_listCard[6].SetCardNo(nJoker); // 카드 번호 세팅
						GetPlayerNP(0)->m_listCard[6].SetJokerCardNo(nJoker);
						GetPlayerNP(0)->m_listCard[6].SetJokerCard(TRUE);
					}
				}
			}
		}
	}
	
	int bJackpot = 0;
	
	//GDM().SetINT64(GDK_JACKPOT_MONEY_TEMP, pGOR->JackPotMoney);
	
	for (i = 0 ;  i < nMaxPlayer ; i++)
	{
		g_Poker.RU[i].nLastCmd = RAISE_LAST_CMD_NULL;
	}
	
	for ( i=0; i < nMaxPlayer; i++)
	{
		if (strlen(pGOR->Ui[i].ID)>0) //  겜참여자.
		{
			int pnum = GetPlayerPNum(pGOR->Ui[i].ID);

			GetJokboName(pnum);
			memcpy(&GetPlayerNP(pnum)->UI.PI, &m_cGameOverResultData.Ui[i].PI, sizeof(POKERINFO));

			if (pnum == -1) continue;

			// 히든 카드 오픈
			SetResultCardDeck(g_pGameView->m_cPlayerDrawManager.IsOpen(),pGOR->Pg.PS[i].bFold,bJackpot,pnum);
		}
	}

	// 카드를 덮은 상태로 한곳에 모음
	int wpnum = GetPNum_ByServPN( g_Poker.nDistUser ); // 카드 날리는 순서

	if (wpnum < 0 || wpnum >= nMaxPlayer)
		return;

	int wdelay = 20;
	int cnt = 0;
	
	if ( g_pGameView->m_cPlayerDrawManager.IsOpen() || bJackpot == 1 )
	{
		for (i = wpnum; i<nMaxPlayer + wpnum; i++) 
		{
			int index = i;

			if (index >= nMaxPlayer)
				index = index - nMaxPlayer;

			if (strlen(GetPlayerNP(index)->UI.ID)>0 && GetPlayerNP(index)->JoinState == 1 && GetPlayerNP(index)->PlayState == 1 && !GetPlayerNP(index)->bFold)
			{
				if (index != GetPlayerNP(0)->PNum)
				{
					// 카드를 다 덮은 상태로 모은다.
					GetPlayerNP(index)->SetGameResultCardOpen(wdelay);
				}
			}
		}
	}

	// 포카드 이상일때 카드 오픈 한다 
	/*  삭제 포카드 이상일때도 오픈하지 않음
	if (!g_pGameView->m_cPlayerDrawManager.IsOpen())
	{
		int wp = GetPNum_ByServPN(pGOR->nNowWinner[0]);
		int val = 0;
		int value = GetWinnerValue(wp,val);
		if (value >= CCardRule::N7C_QUADS)
		{
			ResultFourCardInPut(wp);
			GetPlayerNP(wp)->SetGameResultCardOpen(wdelay);
			g_pGameView->m_cPlayerDrawManager.SetDrawMyValue(wp, true);
		}
	}*/

	wdelay += 10;
	
	CString str="";
	CString strM="";
	
	if ( !g_pGameView->m_cPlayerDrawManager.IsOpen() ) // 승자 1명 (나머지 유저 폴드)
	{
		m_nGameState = GAME_ROUND_RESULT; // 게임오버
		g_pGameView->m_cPlayerDrawManager.SetWinnerPNum(pGOR->nNowWinner[0]);
		m_nCurPlayer = pGOR->nNowWinner[0]; // 하이 승자를 선으로 한다.
		g_Poker.CurPlayer = m_nCurPlayer;
	
		strncpy_s(m_pJackpotWinnerID, sizeof(m_pJackpotWinnerID), pGOR->ID, 15);

		int pnum = GetPlayerPNum(pGOR->ID);
		if (pnum != -1)
		{
			g_pGameView->m_cGameOverResult.PutAngelResult(pGOR,&GetPlayerNP(pnum)->UI,pnum,0,wdelay);
			OnSetMoveChip();
		}
	}
	else // (하이:1 로우:2 스윙:3  승자 1명 ) 승자가 2명: 4 (하이로  이겼는가 로우로 이겼는가 승자가 2명인가)
	{
		m_nGameState = GAME_ROUND_RESULT; // 게임오버
		
		g_pGameView->m_cPlayerDrawManager.SetWinnerPNum(pGOR->nNowWinner[0]);
		m_nCurPlayer = pGOR->nNowWinner[0]; // 하이 승자를 선으로 한다.
		g_Poker.CurPlayer = m_nCurPlayer;
		
		strncpy_s(m_pJackpotWinnerID, sizeof(m_pJackpotWinnerID), pGOR->ID, 15);
		
		int pnum = GetPlayerPNum(pGOR->ID);

		if (pnum != -1)
		{
			g_pGameView->m_cGameOverResult.PutAngelResult(pGOR,&GetPlayerNP(pnum)->UI,pnum,1,wdelay);

			OnSetMoveChip();
		}
	}

	// 마지막에 카드 값을 다시 넣는다.
	if (pGOR->bOpen)
	{
		SetRuleCard();
		m_cSvRule.get_cal_Winner_Client();
	}

	int nWinPNum = GetPNum_ByServPN(g_pGameView->m_cPlayerDrawManager.GetWinnerPNum());
	CPlayer_NP *pPlayer = NULL;

	//하프 잭팟 프로모션 때문에 승자 정보를 저장.
	g_cPromotionMan.SetWinnerNum(nWinPNum) ;

	for (int i=0; i<nMaxPlayer; i++)
	{
		if (strlen(m_cGameOverResultData.Ui[i].ID)>0) //  겜참여자.
		{
			int pnum = GetPlayerPNum(m_cGameOverResultData.Ui[i].ID);
			if (pnum == -1) continue;
			// 결과 증감 머니

			pPlayer = GetPlayerNP(pnum);
			pPlayer->m_i64ResultMoney = m_cGameOverResultData.Ui[i].nResultMoney;
		}
	}


	// 자신의 카드위에 족보명을 보위기 위함
	g_pGameView->m_cPlayerDrawManager.MyJokboNameUpdate();
	g_pGameView->m_cPlayerDrawManager.CheckResultValue();
}

// ### [ Frame++ ] ### *3
void CGameProcess_NewPoker::Card_Distribute(int flycardnum)
{
	int pnum = GetPNum_ByServPN( g_Poker.nDistUser ); // 카드 날리는 순서
	const int nMaxPlayer = GetMaxPlayer();
	if (pnum < 0 || pnum >= nMaxPlayer) return;

	//규제안 :  예약 가능여부 설정
	CanReserve(false);
	g_pGameView->m_cGameViewBtnMan.ShowReserveBtn(false);

	g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(true);

	CMyCard* pLastCard = NULL;// 마지막 카드
	DWORD dwDelay = 0;
	// 처음 3장
	if (g_Poker.nState == RSTATE_PREPARESTART )
	{	
		int nCardNum = 4; // 카드 장수
	
		int lastp = 0;
		int lastci = 0;

		for ( int j = 0; j < nCardNum; j++)	{
			for (int i = pnum; i<nMaxPlayer + pnum; i++) {
				int index = i;
				if (index >= nMaxPlayer) index = index - nMaxPlayer;
				if (strlen(GetPlayerNP(index)->UI.ID)>0 && GetPlayerNP(index)->JoinState == 1 && GetPlayerNP(index)->PlayState == 1)	{
					CMyCard* pCard = &GetPlayerNP(index)->m_listCard[j];
					if (pCard != NULL){
						pCard->GetMove().StartMoveMSprite(&g_sprCardDealing, CPoint(0,0), pCard->GetPos(), index*8 + nCardNum, dwDelay, 0, g_cUIData.m_fStartGameDealingCardSpeed);

						// 한장씩 도착하면 보여준다
						pCard->SetEvent(EVENT_PREPARESTART_ONEEND, index, j ); // 플레이 넘버,인덱스 값
						lastci = index;
						lastp = j;

						dwDelay += g_cUIData.m_dwStartGameDealingTimeGab;
						pLastCard = pCard;
					}
				}
			}
		}

		if (pLastCard != NULL)	
		{			 
			pLastCard->SetEvent(EVENT_THREEFLYEND,lastci,lastp); // 3장 마지막 까지 날렸으면 서버로
			SetWindCardEnd(TRUE);
		}
	}
	else if (g_Poker.bDistCard)
	{
		int j = 0;
		
		if ( flycardnum == 1 ){
			j = GetPlayerNP(pnum)->m_nCardTotal-1; // 선의 카드 수를  따른다.
			BOOL m_bOpen = FALSE;
			for (int i=pnum;  i < nMaxPlayer + pnum; i++)  {
				int index = i;
				if (index >= nMaxPlayer) index = index - nMaxPlayer;
				if (strlen(GetPlayerNP(index)->UI.ID)>0 && GetPlayerNP(index)->JoinState == 1 && GetPlayerNP(index)->PlayState == 1 && !GetPlayerNP(index)->bFold) {
					CMyCard* pCard = &GetPlayerNP(index)->m_listCard[j];
					if (pCard != NULL)  {				
						if (GetTotalCardNum() - 1 == j) m_bOpen = FALSE;
						else m_bOpen = TRUE;
						
						pCard->GetMove().StartMoveMSprite(&g_sprCardDealing,  CPoint(0,0), pCard->GetPos(), index*8 + j, dwDelay, 0, g_cUIData.m_fDealingCardSpeed);
						dwDelay += g_cUIData.m_dwDealingTimeGab;
						// 한장씩 도착하면 보여준다
						pCard->SetEvent(EVENT_ONEFLYEND, index, j ); // 플레이 넘버,인덱스 값
						pLastCard = pCard;
					}
				}
				
			}
			
			if (pLastCard != NULL)
			{
				pLastCard->SetEvent1(EVENT_ONEFLYTOTEND); // 모두 도착하면 서버로
				SetWindCardEnd();
			}
		}
		//4구 5구 한꺼번에 7월 정책 변경
		else{
			int nCardNum = 5; 
			
			j = -1; // 선의 카드 수를  따른다.
			
			int lastp = 0;
			int lastci = 0;

			dwDelay = g_sprBombCardDealing.GetMScenePlayTime(0) + g_sprBombSetAni.GetTotalSpr() * g_cUIData.m_dwBombEffectGab;
			
			for ( int j = 3; j < nCardNum; j++)	{
				for (int i = pnum; i<nMaxPlayer + pnum; i++) {
					int index = i;
					if (index >= nMaxPlayer) index = index - nMaxPlayer;
					if (strlen(GetPlayerNP(index)->UI.ID)>0 && GetPlayerNP(index)->JoinState == 1 && GetPlayerNP(index)->PlayState == 1)
					{
						CMyCard* pCard = &GetPlayerNP(index)->m_listCard[j];
						if (pCard != NULL) {
							pCard->GetMove().StartMoveMSprite(&g_sprCardDealing, CPoint(0,0), pCard->GetPos(), index*8 + j, dwDelay, 0, g_cUIData.m_fDealingCardSpeed);
							
							// 한장씩 도착하면 보여준다
							pCard->SetEvent(EVENT_ONEFLYEND, index, j ); // 플레이 넘버,인덱스 값
							lastci = index;
							lastp = j;
							
							dwDelay += g_cUIData.m_dwDealingTimeGab;
							pLastCard = pCard;
						}
					}
				}
			}
			
			if (pLastCard != NULL)	
			{			 
				pLastCard->SetEvent1(EVENT_ONEFLYTOTEND,lastci,lastp); // 3장 마지막 까지 날렸으면 서버로
				SetWindCardEnd(TRUE);
			}
		}
	}
}

// ### [ Frame++ ] ###
void CGameProcess_NewPoker::SetWindCardEnd(BOOL bFirst)
{
	if (m_bWindCardEnd == FALSE)
	{		
		if ( bFirst ) // 3장 3명 => 61초 1사람당 20초
		{
			m_nWindCardEndCnt = 3*200+(50*m_nStartUserNum);
		}
		else // 1장 7초+
		{
			m_nWindCardEndCnt = 3*200+(30*m_nStartUserNum);
		}
		m_bWindCardEnd = TRUE;
	}
}

void CGameProcess_NewPoker::Raise(POKERGAME *pMsg)
{
	if (pMsg == NULL)
		return;

	//규제안 : 맥스버튼 초기화
	g_pGameView->m_cGameViewBtnMan.ResetMaxBtn();

	// 현재 베팅한 사람의 베팅금액을 알아오기 위해 이전 베팅정보 임시 저장
	RAISEUSER RU_Before[MAX_PLAYER];
	memcpy(RU_Before, g_Poker.RU, sizeof(RAISEUSER) * MAX_PLAYER);

	const int nMaxPlayer = GetMaxPlayer();
	memcpy(&g_Poker, (char*)pMsg, sizeof(POKERGAME));
	g_pGameView->m_cSidePotMoney.Check();
	g_pGameView->m_cPlayerDrawManager.SetTurn(-1);
	g_pGameView->m_cGameViewBtnMan.Reset();

	// ### [ 사용자머니 변경 ] ### 
	for (int i = 0; i < nMaxPlayer; i++)
	{
		int pn = GetPNum_ByServPN(i);

		if (strlen(GetPlayerNP(pn)->UI.ID)>0 && GetPlayerNP(pn)->JoinState == 1 && GetPlayerNP(pn)->PlayState == 1) // 겜중이면
		{
			//GetPlayerNP(pn)->UI.SetMoney(g_Poker.RU[i].nPMoney); // 사용자 머니 셋
			//MAX방 함수 호출
			GetPlayerNP(pn)->SetNowMoney( g_Poker.RU[i].nPMoney ); // 사용자 머니 셋
			
			if ( g_Poker.PS[i].bFold )
				GetPlayerNP(pn)->bFold = TRUE;
			
			// 추가 
			//if (GetPlayerNP(pn)->UI.GetMoney() < 0) GetPlayerNP(pn)->UI.SetMoney(0);
			if (GetPlayerNP(pn)->GetNowMoney() < 0) 
			{
				GetPlayerNP(pn)->SetNowMoney( 0 );
			}

			// ### [ 관전기능 ] ###
			GetPlayerNP(pn)->UI.nIcon = GetMoneyGrade(GetPlayerNP(pn)->UI.GetTotalMoney() );
		}
	}
	
	CPoint cpEffectPos = CPoint(0, 0);

	// ##### [ 정보 창에 메세지 && 사운드 출력] #####
	if (g_Poker.LastPlayer >= 0 && g_Poker.LastPlayer < nMaxPlayer)
	{
		// 이전에 배팅한 플레이어 번호
		int LastPnum = g_Poker.LastPlayer;
		int nSNum = GetPNum_ByServPN(LastPnum);
		
		// 이전 배팅이 없을때
		if ( g_Poker.RU[LastPnum].nLastCmd>RAISE_LAST_CMD_NULL && nSNum >= 0 && nSNum < nMaxPlayer) 
		{
			int nLastCmd = g_Poker.RU[LastPnum].nLastCmd;
			
			// 현재 베팅 금액을 보여주기 위해 (현재 총 베팅금액 - 총 베팅금액)을 클라에서 계산해서 보여줌
			//g_pGameView->m_cPlayerDrawManager.SetBettingMoney(nSNum, nLastCmd, g_Poker.RU[LastPnum].nNowBat);
			g_pGameView->m_cPlayerDrawManager.SetBettingMoney(nSNum, nLastCmd, g_Poker.RU[LastPnum].nRealBat - RU_Before[LastPnum].nRealBat);

			if (m_cAniSprObjectMan.pFirst != NULL)
			{
				CBaseObject *pNow = NULL;
				CBaseObject* pNext = m_cAniSprObjectMan.pFirst;
				
				while(pNext)
				{
					pNow = pNext;
					
					if (pNow->m_nObjID == ANF_BET_CMD)
					{
						CBetCmdAni* pObj = (CBetCmdAni*)pNow;
						//새로 찍어야 할때면 기존거는 지우고 다시 찍는다.
						if (pObj->m_nPNum  == nSNum && g_Poker.bDistCard == FALSE && nLastCmd != RAISE_LAST_CMD_FOLD )
						{
							pObj->Destroy();
						}

						// 작은 커멘드로 보여줌
						if (pObj->m_nPNum != nSNum)
						{
							pObj->SetSmallCmd();
						}
					}

					pNext = pNow->pNextNode;
				}
			}

			if (2 == g_Poker.RU[LastPnum].bMaxBet)
			{
				g_pGameView->m_cPlayerDrawManager.SetLimitMoney(nSNum, true);
			}

			//규제안 : 맥스 커맨드 추가 : 맥스이펙트
			if (g_Poker.RU[LastPnum].bMaxBet && GM().g_bFirstMax[LastPnum] == false)
			{

				// 맥스이펙트
				if (g_Poker.nRound == 5)
				{
					GetPlayerNP(nSNum)->m_bLastHalf=TRUE;
					SoundFxKindBig(nLastCmd,GetPlayerNP(nSNum)->UI.Sex, true);
				}
				else
				{
					SoundFxKind(nLastCmd,GetPlayerNP(nSNum)->UI.Sex,true);
				}
				
				g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);

				if (GetPlayerNP(nSNum)->GetNowMoney() == 0 )
				{
					g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
				}
				
				CBetCmdAni *pObj = new CBetCmdAni();
				pObj->Init(nSNum,ANF_BET_CMD, ANF_BET_MAX, false);
				cpEffectPos=pObj->GetEffectPos();
				g_cVipJackpot.OnBetGame(cpEffectPos, false);
				m_cAniSprObjectMan.AddObject(pObj);

				GM().g_bFirstMax[LastPnum] = true;
			}
			// 일반 배팅 커맨드
			else
			{
				switch(nLastCmd)
				{
				case RAISE_LAST_CMD_FOLD: // foldUser 에서 처리
					{
					}
					break; // 다이
				case RAISE_LAST_CMD_CHECK:
					{					
						if (g_Poker.bDistCard == TRUE)
							break;
						
						//규제안 작업 : 카드세팅 중이면 이펙트를 노출하지 않는다. 2014.01
						if ( CheckReceivedAllCards() )
						{
							SoundFxKind(nLastCmd,GetPlayerNP(nSNum)->UI.Sex);
							
							CBetCmdAni *pObj = new CBetCmdAni();
							pObj->Init(nSNum, ANF_BET_CMD, ANF_BET_CHECK, false);
							cpEffectPos = pObj->GetEffectPos();
							g_cVipJackpot.OnBetGame(cpEffectPos, false);
							m_cAniSprObjectMan.AddObject(pObj);
							
							if (g_Poker.nRound == 5)
							{
								GetPlayerNP(nSNum)->m_bLastHalf = TRUE;
							}
						}
					}
					break; // 체크
				case RAISE_LAST_CMD_CALL: 
					{	
						if (g_Poker.bDistCard == TRUE)
							break;
						
						//규제안 작업 : 카드세팅 중이면 이펙트를 노출하지 않는다. 2014.01
						if ( CheckReceivedAllCards() )
						{
							if (g_Poker.nRound == 5)
							{
								GetPlayerNP(nSNum)->m_bLastHalf=TRUE;
								SoundFxKindBig(nLastCmd,GetPlayerNP(nSNum)->UI.Sex);
							}
							else
							{
								SoundFxKind(nLastCmd,GetPlayerNP(nSNum)->UI.Sex);
							}
							
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);
							
							if (GetPlayerNP(nSNum)->GetNowMoney() == 0 )
							{
								g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
							}
							
							CBetCmdAni *pObj = new CBetCmdAni();
							pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_CALL, false);
							cpEffectPos = pObj->GetEffectPos();
							g_cVipJackpot.OnBetGame(cpEffectPos, false);
							m_cAniSprObjectMan.AddObject(pObj);
						}
					}
					break; // 콜
				case RAISE_LAST_CMD_PING: 
					{	
						if (g_Poker.bDistCard == TRUE)
							break;
						
						if (g_Poker.nRound == 5)
						{
							GetPlayerNP(nSNum)->m_bLastHalf=TRUE;
							SoundFxKindBig(nLastCmd,GetPlayerNP(nSNum)->UI.Sex);
						}
						else
						{
							SoundFxKind(nLastCmd,GetPlayerNP(nSNum)->UI.Sex);
						}
						
						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);
						if (GetPlayerNP(nSNum)->GetNowMoney() == 0 )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}

						CBetCmdAni *pObj = new CBetCmdAni();
						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_PING,false);
						cpEffectPos=pObj->GetEffectPos();
						g_cVipJackpot.OnBetGame(cpEffectPos, false);
						m_cAniSprObjectMan.AddObject(pObj);
					}  
					break; // 삥
				case RAISE_LAST_CMD_DADANG:
					{					
						if (g_Poker.bDistCard == TRUE)
							break;
						
						if (g_Poker.nRound == 5){
							GetPlayerNP(nSNum)->m_bLastHalf=TRUE;
							SoundFxKindBig(nLastCmd,GetPlayerNP(nSNum)->UI.Sex);
						}
						else
						{
							SoundFxKind(nLastCmd,GetPlayerNP(nSNum)->UI.Sex);
						}
						
						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);
						if (GetPlayerNP(nSNum)->GetNowMoney() == 0 )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}

						CBetCmdAni *pObj = new CBetCmdAni();
						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_DADANG,false);
						cpEffectPos=pObj->GetEffectPos();
						g_cVipJackpot.OnBetGame(cpEffectPos, false);
						m_cAniSprObjectMan.AddObject(pObj);
					}  
					break; // 따당
				case RAISE_LAST_CMD_FULL:
					{					
						if (g_Poker.bDistCard == TRUE)
							break;

						if (g_Poker.nRound == 5)
						{
							GetPlayerNP(nSNum)->m_bLastHalf = TRUE;
							SoundFxKindBig(nLastCmd,GetPlayerNP(nSNum)->UI.Sex);
						}
						else
						{
							SoundFxKind(nLastCmd,GetPlayerNP(nSNum)->UI.Sex);
						}

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);

						if (GetPlayerNP(nSNum)->GetNowMoney() == 0 )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}

						CBetCmdAni *pObj = new CBetCmdAni();
						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_FULL,false);
						cpEffectPos = pObj->GetEffectPos();
						g_cVipJackpot.OnBetGame(cpEffectPos, true);
						m_cAniSprObjectMan.AddObject(pObj);
					}
					break; // 풀
				case RAISE_LAST_CMD_QUARTER:
				case RAISE_LAST_CMD_HALF:
					{					
						if (g_Poker.bDistCard == TRUE)
							break;

						// 하프 잭팟 프로모션 하프 배팅 카운트 증가.
						if (g_RI.State == 1 && g_Poker.nAutoBetCmd <= 0)
							g_cPromotionMan.SetHalfJackpotCountPlus();
						
						//if ( g_RI.State == 1 && g_Poker.nAutoBetCmd <= 0 && g_RI.NowUserNum >= 4)
						//{
						//	int nCount = 0 ;
						//	for (int i = 0 ;i < g_RI.NowUserNum; i++)
						//	{
						//		if (1 == g_Poker.PS[i].JoinState) // 게임에 참가중이면.
						//		{
						//			nCount++ ;
						//		}
						//	}
						//	
						//	if (nCount == g_RI.NowUserNum)
						//		g_cPromotionMan.SetHalfJackpotCountPlus() ;
						//}
						
						if (g_Poker.nRound == 5)
						{
							GetPlayerNP(nSNum)->m_bLastHalf=TRUE;
							SoundFxKindBig(nLastCmd,GetPlayerNP(nSNum)->UI.Sex);
						}
						else
						{
							SoundFxKind(nLastCmd,GetPlayerNP(nSNum)->UI.Sex);
						}

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);

						if (GetPlayerNP(nSNum)->GetNowMoney() == 0 )
						{	
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}

						if (g_Poker.nRound == 5)
						{
							//하프를 쳤던 넘들만 족보를 강하게 불러준다.
							GetPlayerNP(nSNum)->m_bLastHalf=TRUE;
						}
						
						CBetCmdAni *pObj = new CBetCmdAni();
						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_HALF,g_Poker.nRound>4?true:false);
						cpEffectPos=pObj->GetEffectPos();
						g_cVipJackpot.OnBetGame(cpEffectPos, false);
						m_cAniSprObjectMan.AddObject(pObj);
					} 
					break; // 맥스	
				case RAISE_LAST_CMD_ALLIN:
					{					
						if (g_Poker.bDistCard == TRUE)
							break;

						if (g_Poker.nRound == 5)
						{
							GetPlayerNP(nSNum)->m_bLastHalf=TRUE;
							SoundFxKindBig(nLastCmd,GetPlayerNP(nSNum)->UI.Sex);
						}
						else
						{
							SoundFxKind(nLastCmd,GetPlayerNP(nSNum)->UI.Sex);
						}

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);

						if (GetPlayerNP(nSNum)->GetNowMoney() == 0 )
						{	
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}

						if (g_Poker.nRound == 5)
						{
							//하프를 쳤던 넘들만 족보를 강하게 불러준다.
							GetPlayerNP(nSNum)->m_bLastHalf=TRUE;
						}

						CBetCmdAni *pObj = new CBetCmdAni();
						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_ALLIN,g_Poker.nRound>4?true:false);
						cpEffectPos=pObj->GetEffectPos();
						g_cVipJackpot.OnBetGame(cpEffectPos, false);
						m_cAniSprObjectMan.AddObject(pObj);
					}
					break; // 맥스
				}
			}
		}
	}
	
	// 현재 순서가 카드 날리기 인가?
	if ( g_Poker.bDistCard ) 
	{
		g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(true);

		//규제안 : 예약버튼 설정을 위해 호출
		g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable(); 

		// 베팅머니 표시되는 부분을 초기화 한다.
		// 2012. 04. 25 최종학
		for (int i = 0; i < GetMaxPlayer(); ++i)
		{
			g_pGameView->m_cPlayerDrawManager.ClearBettingMoney(i);
		}

		m_nCurPlayer = -1;
		//칩을 이동 시킨다.
		OnSetMoveChip();

		int FlyCardNum = 1;
		int nMaxCard = 0;

		// 장수 증가 할것
		for (int i = 0; i < nMaxPlayer; i++)
		{
			int pn = GetPNum_ByServPN(i);

			if (strlen(GetPlayerNP(pn)->UI.ID)>0 && GetPlayerNP(pn)->JoinState == 1 && GetPlayerNP(pn)->PlayState == 1 && !GetPlayerNP(pn)->bFold ) // 겜중이면
			{
				int totCnt = g_Poker.PS[i].nCardTotal;
				int openCnt = g_Poker.PS[i].nOpenTotal;
				
				//규제안 : 카드세팅 현재 받은 카드 개수 설정
				g_nCardTotalCount = totCnt;

				// 자기 자신 처리
				if (pn == 0)
				{
					if (totCnt == 4 && (g_RI.nAutoBetKind == 490 || g_RI.nAutoBetKind == 400) ) 
					{
						FlyCardNum = 1;

						// ### [ 관전기능 ] ###
						if ( g_ObserverMan.IsMyObserver() )
						{
							// 2가지 경우 = 3장 나누어준 상태에서 방입장 카드숫자 -1 하지 않는당
							if (m_bGameStart)
							{	// 처음서 부터 관전중 
								GetPlayerNP(pn)->m_listCard[3].Clear();
								GetPlayerNP(pn)->m_nTotalCardNum -= 1;
							}

							GetPlayerNP(pn)->m_listCard[2].SetCardNo(g_Poker.PS[i].nCard[0]); // 값 변경
							GetPlayerNP(pn)->SetNewCard( g_Poker.PS[i].nCard[1]);  // 값 등록

							for (int k = 2; k < totCnt-2; k++)
							{
								GetPlayerNP(pn)->SetFaceUp(k); // 앞면(보이기)
							}
						}
						else
						{
							GetPlayerNP(0)->m_listCard[2].SetCardNo(g_Poker.PS[i].nCard[0]); // 값 변경

							for (int k = 0; k < totCnt-2; k++)
							{
								GetPlayerNP(0)->SetFaceUp(k); // 앞면(보이기) 나는 모두
							}

							// 3번째 장 위쪽으로 그린다
							CPoint pt = GetPlayerNP(0)->m_listCard[2].GetPos();
							pt.y -= 7;
							GetPlayerNP(0)->m_listCard[2].SetPos(pt);
							GetPlayerNP(0)->m_listCard[2].GetMove().SetCurPos(pt);
							GetPlayerNP(0)->SetNewCard( g_Poker.PS[i].nCard[1], FALSE, TRUE); // 4번째 카드 등록
						}

						GetPlayerNP(0)->SetChioceCardMove(FALSE, 2);
					}
					else if (totCnt == 5) 
					{// 1 한번에 4장
						if ( g_RI.nAutoBetKind == 490 || g_RI.nAutoBetKind == 400 )
						{
							if (openCnt > 0)
							{
								GetPlayerNP(0)->SetNewCard( g_Poker.PS[i].nCard[openCnt-1]); // 4번째 카드 등록
							}
						}
						else
						{
							FlyCardNum = 2;

							// ### [ 관전기능 ] ###
							if ( g_ObserverMan.IsMyObserver() )
							{
								// 2가지 경우 = 3장 나누어준 상태에서 방입장 카드숫자 -1 하지 않는당
								if (m_bGameStart)
								{	// 처음서 부터 관전중 
									GetPlayerNP(pn)->m_listCard[3].Clear();
									GetPlayerNP(pn)->m_nTotalCardNum -= 1;
								}

								GetPlayerNP(pn)->m_listCard[2].SetCardNo(g_Poker.PS[i].nCard[0]); // 값 변경						
								GetPlayerNP(pn)->SetNewCard( g_Poker.PS[i].nCard[1]);  // 값 등록
								GetPlayerNP(pn)->SetNewCard( g_Poker.PS[i].nCard[2]);  // 값 등록
								
								for (int k = 2; k < totCnt-2; k++)
								{
									GetPlayerNP(pn)->SetFaceUp(k); // 앞면(보이기)
								}
							}
							else
							{
								GetPlayerNP(0)->m_listCard[2].SetCardNo(g_Poker.PS[i].nCard[0]); // 값 변경
								
								for (int k = 0; k < totCnt-2; k++)
								{
									GetPlayerNP(0)->SetFaceUp(k); // 앞면(보이기) 나는 모두
								}

								// 3번째 장 위쪽으로 그린다
								CPoint pt = GetPlayerNP(0)->m_listCard[2].GetPos();
								pt.y -= 7;
								GetPlayerNP(0)->m_listCard[2].SetPos(pt);

								GetPlayerNP(0)->m_listCard[2].GetMove().SetCurPos(pt);
								GetPlayerNP(0)->SetNewCard( g_Poker.PS[i].nCard[1]); // 4번째 카드 등록

								pt = GetPlayerNP(0)->m_listCard[3].GetPos();
								GetPlayerNP(0)->m_listCard[3].GetMove().SetCurPos(pt);
								GetPlayerNP(0)->SetNewCard( g_Poker.PS[i].nCard[2]); // 4번째 카드 등록
							}

							GetPlayerNP(0)->SetChioceCardMove(FALSE,2);
						}
					}
					else if (totCnt < 7) // 히든 전
					{
						if (openCnt > 0)
							GetPlayerNP(0)->SetNewCard( g_Poker.PS[i].nCard[openCnt-1]); // 4번째 카드 등록
					}
					else if (totCnt == 7 && g_RI.FormKind < BOMB_GAME)// hidden
					{
						// ### [ 관전기능 ] ###
						GetPlayerNP(0)->SetNewCard( DUMY_CARD);
						// 자동히든 카드 플래그 온			
						
						SetCardOpenGuideTime();//오픈애니메이션추가
					}
					else if ((g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME) && totCnt == 8)
					{
						GetPlayerNP(0)->SetNewCard( DUMY_CARD); // 4번째 카드 등록

						SetCardOpenGuideTime();//오픈애니메이션추가
					}
				}
				else
				{
					if (totCnt == 4 && (g_RI.nAutoBetKind == 490 || g_RI.nAutoBetKind == 400) ) 
					{
						FlyCardNum = 1;
						GetPlayerNP(pn)->m_listCard[2].SetCardNo(g_Poker.PS[i].nCard[0]); // 값 변경
						GetPlayerNP(pn)->SetNewCard( g_Poker.PS[i].nCard[1]);  // 값 등록		

						for (int k = 2; k < totCnt-2; k++)
						{
							GetPlayerNP(pn)->SetFaceUp(k);
						}

						GetPlayerNP(pn)->SetChioceCardMove(FALSE);
					}
					else if (totCnt == 5)
					{
						if ( g_RI.nAutoBetKind == 490 || g_RI.nAutoBetKind == 400 )
						{
							if (openCnt > 0)
								GetPlayerNP(pn)->SetNewCard( g_Poker.PS[i].nCard[openCnt-1]); // 4번째 카드 등록
						}
						else
						{
							FlyCardNum = 2;
							GetPlayerNP(pn)->m_listCard[2].SetCardNo(g_Poker.PS[i].nCard[0]); // 값 변경
							GetPlayerNP(pn)->SetNewCard( g_Poker.PS[i].nCard[1]);  // 값 등록						
							GetPlayerNP(pn)->SetNewCard( g_Poker.PS[i].nCard[2]);  // 값 등록

							for (int k = 2; k < totCnt-2; k++)
							{
								GetPlayerNP(pn)->SetFaceUp(k);
							}

							GetPlayerNP(pn)->SetChioceCardMove(FALSE);
						}
					}
					else if (totCnt < 7)
					{
						if (openCnt > 0)
							GetPlayerNP(pn)->SetNewCard( g_Poker.PS[i].nCard[openCnt-1]); // 4번째 카드 등록
					}
					else if (totCnt == 7 && g_RI.FormKind < BOMB_GAME) // hidden
					{
						GetPlayerNP(pn)->SetNewCard( DUMY_CARD); // 4번째 카드 등록

						SetCardOpenGuideTime();//오픈애니메이션추가
					}
					else if ((g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME) && totCnt == 8)
					{
						GetPlayerNP(pn)->SetNewCard( DUMY_CARD); // 4번째 카드 등록

						SetCardOpenGuideTime();//오픈애니메이션추가
					}
				}

				GetPlayerNP(pn)->m_nCardTotal = totCnt;
				GetPlayerNP(pn)->m_nOpenTotal = openCnt;

				if (nMaxCard < totCnt)
					nMaxCard = totCnt;
			}

			switch(nMaxCard)
			{
			case 4:
			case 5:
			case 6:
				g_pGameView->m_cDisplayBoard.SetText(ENUM_DISPLAY_BOARD_TEXT((int)EDBT_4CARD + nMaxCard - 4));
				break;
			case 7:
				//조커랑 포탄카드는 다른곳에서 등록
				break;
			case 8:
				g_pGameView->m_cDisplayBoard.SetText(EDBT_HIDDENCARD);
				break;
			}
		}
		
		// 카드날리기~
		Card_Distribute(FlyCardNum);

		g_pGameView->m_cGameHelper.SetAllUserCardInfo();
		
		return;
	}
	
	// 게임이 끝났을때
	if (g_Poker.nState == RSTATE_RESULT) 
	{
		// '게임 끝났을때' 베팅 커멘드를 지움
		// 기존에는 매턴마다 배팅 커맨드를 지워서 배팅 커맨드가 안나타나는 이슈 발생했음
		if (m_cAniSprObjectMan.pFirst != NULL)
		{
			CBaseObject *pNow = NULL;
			CBaseObject* pNext = m_cAniSprObjectMan.pFirst;

			while (pNext)
			{
				CBaseObject* pNow = pNext;

				if (pNow->m_nObjID == ANF_BET_CMD)
				{
					CBetCmdAni* pObj = (CBetCmdAni*)pNow;
					pObj->Destroy();
				}

				pNext = pNow->pNextNode;
			}
		}
	}

	// 레이즈모드이다 베팅시작  내턴이면 버튼 활성화
	if (g_Poker.nState == RSTATE_STARTGAME) 
	{
		m_nCurPlayer = g_Poker.CurPlayer;
		
		if (m_nCurPlayer < 0 || m_nCurPlayer >= nMaxPlayer)
			return;
		
		int pn = GetPNum_ByServPN(m_nCurPlayer);
		
		if (g_Poker.LastStyle == -1)
		{
			INT64 nRaiseBat = GetRaiseBat();
			chip_draw_func(pn, nRaiseBat);
			g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();

			return;
		}
		
		if (g_Poker.LastStyle >= 2) // 베팅시에만
		{
			int LastPlayer = g_Poker.LastPlayer;

			if (LastPlayer < 0 || LastPlayer >= nMaxPlayer)
				return;

			int bet = GetPNum_ByServPN(LastPlayer);
		
			if (g_Poker.RU[LastPlayer].nLastCmd == RAISE_LAST_CMD_CALL)
			{
				INT64 nRaiseBat = GetRaiseBat();
				chip_draw_func(bet, nRaiseBat);
			}
			else
			{
				INT64 nRaiseBat = GetRaiseBat();
				chip_draw_func(bet, nRaiseBat);
			}

			//규제안 관련 작업으로 주석처리 2014.01->추후작업
			// 배팅 금액 표시
			if (m_i64JackPotActive <=0 && !m_cSelectCardMan.IsChoiceCard())
			{
				//< CYS 100705 > 교체
				//따당 금액 구하기
				if ( g_Poker.RU[g_Poker.LastPlayer].nLastCmd != RAISE_LAST_CMD_FOLD )
					m_i64Double = ( g_Poker.RU[ g_Poker.LastPlayer ].nCalcBat + g_Poker.RU[ g_Poker.LastPlayer ].nReCall ) * 2;
			}
		}
		
		g_pGameView->m_cPlayerDrawManager.SetStartTimer();
		
		// ### [ 관전기능 ] ###
		if (!g_ObserverMan.IsMyObserver() && GetPlayerNP(0)->ServPNum == m_nCurPlayer )
		{
			g_pGameView->m_cGameViewBtnMan.SetButtonClick(false);
			//g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable(false);
			
			// 자동베팅시에는 베팅버튼 활성화 딜레이를 따로 설정한다.
			if (g_Poker.nAutoBetCmd > 0)
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelayInAutoBet(0, 0);
			else
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelay(0, 0);
			
			m_bBettingDelayCheck = TRUE;
			
			AutoBetSetUpWnd().Set_auto_betting_flag();
			g_pGameView->m_cGameViewBtnMan.SetShowBetMony();
		}
		
		if (true == m_bOpenBombcard)
		{
			// 규제안 예약 베팅 추가.(방장이 아닌 유저는 버튼 활성화가 안옴)
			if (!g_ObserverMan.IsMyObserver() && GetPlayerNP(0)->ServPNum != m_nCurPlayer)
			{
				// 자동베팅시에는 베팅버튼 활성화 딜레이를 따로 설정한다.
				if (g_Poker.nAutoBetCmd > 0)
					m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelayInAutoBet(0, 0);
				else
					m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelay(0, 0);
				
				m_bBettingDelayCheck = TRUE;
			}
			
			//규제안 :  예약 가능여부 설정
			CanReserve(true);
			m_bOpenBombcard = false;
		}
		
		// 규제안 추가 카드세팅으로 딜링 일때는 턴 표시 안함.
		if ( CheckReceivedAllCards() )
		{
			g_pGameView->m_cPlayerDrawManager.SetTurn(this->GetPNum_ByServPN(m_nCurPlayer));
		}
		
		// 자신의 카드위에 족보명을 보위기 위함
		g_pGameView->m_cPlayerDrawManager.MyJokboNameUpdate();
		
		return;
	}
}

void CGameProcess_NewPoker::chip_draw_func(int PNum, INT64 Money, int sDelay, int Kind)
{
	if (PNum < 0 || PNum > 4)
		return;

	if (Money < 0)
		return;
	
	GetPlayerNP(PNum)->OnSetChip(Money);

	m_cAllBetChip.SetMoney(GetRealBet());
}

void CGameProcess_NewPoker::EndWindCard()
{
	g_pGameView->m_cPlayerDrawManager.SetStartTimer();
	//// 카드 선택시 시간보이기
	const int nMaxPlayer = GetMaxPlayer();
	for (int i=0; i<nMaxPlayer;i++) {
		if (g_Poker.nState==RSTATE_PREPARESTART && strlen(GetPlayerNP(i)->UI.ID)>0 && GetPlayerNP(i)->JoinState != 0) {
			g_pGameView->m_cPlayerDrawManager.SetSelectCard(i, true);
		}
	}
	// ### [ 관전기능 ] ###
	if ( !g_ObserverMan.IsMyObserver() && g_Poker.nState==RSTATE_PREPARESTART && strlen(GetPlayerNP(0)->UI.ID)>0 && GetPlayerNP(0)->JoinState != 0)	// 시작준비 모드이다(카드선택모드)
	{		
		m_cSelectCardMan.SetChoiceCard(true);
		g_pGameView->m_cDisplayBoard.SetText(EDBT_SELECTCARD);
		m_cSelectCardMan.ShowSelectCard();

		return;
	}

	OnSetMoveChip();

	if (g_Poker.nState==RSTATE_STARTGAME){ // 레이즈모드이다
		// 베팅시작 (카드 다 날리고 베팅 시작)		
		// 현재 턴 셋팅
		if (g_Poker.nAutoBetCmd > 0)
		{
			g_pGameView->m_cDisplayBoard.SetText(EDBT_AUTOBETTING);
		}
		else
		{
			switch(g_Poker.nRound)
			{
			case 1:
			case 2:
			case 3:
			case 4:
				{
					g_pGameView->m_cDisplayBoard.SetText(ENUM_DISPLAY_BOARD_TEXT((int)EDBT_4BETTING + g_Poker.nRound - 1));

					// 자신의 카드위에 족보명을 보위기 위함 (히든카드시에는 클릭하면 보이도록 하기위해 이곳에서 호출한다.)
					g_pGameView->m_cPlayerDrawManager.MyJokboNameUpdate();
				}
				break;
			case 5:
				g_pGameView->m_cDisplayBoard.SetText(EDBT_HIDDENBETTING);
				break;
			}
		}

		m_nCurPlayer = g_Poker.CurPlayer;
		//  타이머 보이기
		g_pGameView->m_cPlayerDrawManager.SetStartTimer();
	
		int pn = GetPNum_ByServPN(m_nCurPlayer);
		
		// ### [ 관전기능 ] ###
		if (!g_ObserverMan.IsMyObserver() && GetPlayerNP(0)->ServPNum == m_nCurPlayer )
		{			
			g_pGameView->m_cGameViewBtnMan.SetButtonClick(false);
			//g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable(false);
			AutoBetSetUpWnd().Set_auto_betting_flag();
			g_pGameView->m_cGameViewBtnMan.SetShowBetMony();

			// 자동베팅시에는 베팅버튼 활성화 딜레이를 따로 설정한다.
			if (g_Poker.nAutoBetCmd > 0)
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelayInAutoBet(0, 0);
			else
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelay(0,0);

			m_bBettingDelayCheck = TRUE ;
		}

		// 규제안 예약 베팅 추가.(방장이 아닌 유저는 버튼 활성화가 안옴)
		if (!g_ObserverMan.IsMyObserver() && GetPlayerNP(0)->ServPNum != m_nCurPlayer)
		{
			// 자동베팅시에는 베팅버튼 활성화 딜레이를 따로 설정한다.
			if (g_Poker.nAutoBetCmd > 0)
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelayInAutoBet(0, 0);
			else
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelay(0,0);

			m_bBettingDelayCheck = TRUE ;
		}

		g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(false);
		// 규제안 추가 카드세팅으로 딜링 일때는 턴 표시 안함.
		if ( CheckReceivedAllCards() )
		{
			g_pGameView->m_cPlayerDrawManager.SetTurn(GetPNum_ByServPN(m_nCurPlayer));
		}

		//규제안 예약베팅 셋팅 초기화
		ReserveState(RESERVESTATE_BET_NONE);
		CanReserve(true);
		
		return;
	}
}

void CGameProcess_NewPoker::OnCardEvent(CARDEVENT *pEvent)
{
	if (pEvent == NULL) return;
	const int nMaxPlayer = GetMaxPlayer();
	switch( pEvent->EventCode )
	{
	case EVENT_PREPARESTART_ONEEND:
		{
			int pnum = pEvent->Option;
			int Index = pEvent->Option2;

			if (pnum < 0 || pnum >= nMaxPlayer) break;

			if (GetPlayerNP(0)->PNum == pnum){
				GetPlayerNP(pnum)->SetFaceUp(Index);
			}
		}
		break;
	case EVENT_CHOICESELECT_AFTER_ONEEND:
		{
			int pnum = pEvent->Option;
			int Index = pEvent->Option2;

			if (pnum < 0 || pnum >= nMaxPlayer) break;

			if (GetPlayerNP(0)->PNum == pnum){
				GetPlayerNP(pnum)->SetFaceUp(Index);
			}
		}
		break;
	case EVENT_THREEFLYEND: // 처음 3장
		{

			int pnum = pEvent->Option;
			int Index = pEvent->Option2;

			if (pnum < 0 || pnum >= nMaxPlayer) break;

			if (GetPlayerNP(0)->PNum == pnum){
				GetPlayerNP(pnum)->SetFaceUp(Index);
			}

			// ### [ 관전기능 ] ###
			if (!g_ObserverMan.IsMyObserver() && m_bWindCardEnd  && GetPlayerNP(0)->JoinState == 1 && GetPlayerNP(0)->PlayState == 1)			
			{
				CCL_WINDCARDEND msg2;
				msg2.Set(g_RI.RoomNum, GetPlayerNP(0)->ServPNum, GetPlayerNP(0)->UI.UNum, GetPlayerNP(0)->UI.ID);//int rnum, int pnum, int unum, char *id)
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg2.pData, msg2.GetTotalSize());
			}
			
			m_bWindCardEnd = FALSE;
		} 
		break;
	case EVENT_ONEFLYEND: // 한장씩 날리고 도착하면 보여준다.
		{
			int pnum = pEvent->Option;
			int Index = pEvent->Option2;

			if (pnum < 0 || pnum >= nMaxPlayer) break;

			if (Index != 6 && Index != 7){
				GetPlayerNP(pnum)->SetFaceUp(Index);
			}
		}
		break;
	case EVENT_ONEFLYTOTEND: // 한장씩 모두 날렸다면 서버로
		{
			// ### [ 관전기능 ] ###
			if (!g_ObserverMan.IsMyObserver() && m_bWindCardEnd && GetPlayerNP(0)->JoinState == 1 && GetPlayerNP(0)->PlayState == 1)
			{
				CCL_WINDCARDEND msg2;
				msg2.Set(g_RI.RoomNum, GetPlayerNP(0)->ServPNum, GetPlayerNP(0)->UI.UNum, GetPlayerNP(0)->UI.ID);//int rnum, int pnum, int unum, char *id)
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg2.pData, msg2.GetTotalSize());
				g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(false);
			}

			for (int i = 0 ; i < nMaxPlayer ; i ++){
				g_Poker.RU[i].nLastCmd = RAISE_LAST_CMD_NULL;
			}

			if (!g_ObserverMan.IsMyObserver() && GetPlayerNP(0)->JoinState == 1 && GetPlayerNP(0)->PlayState == 1)
			{
				int nOldPoint = GAME()->m_cSvRule.get_MyRule(0);

				if ( GetPlayerNP(0)->m_nTotalCardNum <= 7 ){			
					g_pGameView->m_cGameHelper.CalculationProbability();
				}
				else if ( GetPlayerNP(0)->m_nTotalCardNum == 8 ){
					g_pGameView->m_cGameHelper.ClearProbability();
				}

				GetPlayerNP(0)->DrawEft_CompleteMadeValue( nOldPoint );
			}

			m_bWindCardEnd = FALSE;

			if (!g_ObserverMan.IsMyObserver() && GetPlayerNP(0)->JoinState == 1){
				if ( GetPlayerNP(0)->m_nTotalCardNum < GetTotalCardNum() ){
					g_pGameView->m_cGameHelper.CheckOtherJokboCheck(0);
				}
			}
			
		} 
		break;
	case EVENT_BOMBFLYEND:
		{
			int pnum = pEvent->Option;
			int Index = pEvent->Option2;

			if (pnum < 0 || pnum >= nMaxPlayer) break;
			GetPlayerNP(pnum)->SetFaceUp(Index);
		}
		break;
	case EVENT_BOMBFLY_TO_END:
		{
			if (!g_ObserverMan.IsMyObserver() &&  GetPlayerNP(0)->JoinState == 1 && GetPlayerNP(0)->PlayState == 1)
			{
				CCL_OPEN_BOMBCARD_RESULT rmsg;
				rmsg.Set(g_RI.RoomNum,GetPlayerNP(0)->ServPNum,GetPlayerNP(0)->UI.ID);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), rmsg.pData, rmsg.GetTotalSize());
			}

			g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(false);

			g_pGameView->m_cDisplayBoard.SetText(EDBT_7BETTING);

			//조커 카드 이면 효과 출력
			if (g_bGetJokerCard){
				m_dwStartDrawJokerTime = timeGetTime();
				PBPlayEffectSound(SND_JOKERCHANGE);
			}

			int nOldPoint = GAME()->m_cSvRule.get_MyRule(0);

			// 폭탄 카드 로 인해 활률 다시 셋
			if (!g_ObserverMan.IsMyObserver() && !GetPlayerNP(0)->bFold)
			{
				// 확률 셋
				g_pGameView->m_cGameHelper.SetAllUserCardInfo();
				g_pGameView->m_cGameHelper.CalculationProbability();
			}

			if (!g_ObserverMan.IsMyObserver() && GetPlayerNP(0)->JoinState == 1){			
				g_pGameView->m_cGameHelper.CheckOtherJokboCheck(0);
			}

			// 자신의 카드위에 족보명을 보위기 위함
			g_pGameView->m_cPlayerDrawManager.MyJokboNameUpdate();

			GetPlayerNP(0)->DrawEft_CompleteMadeValue( nOldPoint );

			m_bOpenBombcard = true ;
		}
		break;
	}
}

CString CGameProcess_NewPoker::GetMyJokboName()
{
	// ### [ 관전기능 ] ###
	if (g_ObserverMan.IsMyObserver() || strlen(GetPlayerNP(0)->UI.ID)==0 || GetPlayerNP(0)->JoinState == 0 || GetPlayerNP(0)->PlayState == 0 ) return "";
	int nSNum = g_pMyInfo_NP->ServPNum;
	
	const int nMaxPlayer = GetMaxPlayer();
	if (nSNum < 0 || nSNum >= nMaxPlayer) return "";
	if ( g_Poker.PS[nSNum].bFold ) return "";

	enum SEVEN_CARD eName;
	int nVal=0,i=0;
	int aCard[10]={0,};
	int nCard = g_Poker.PS[nSNum].nCardTotal;
	if (nCard<3) return "";
	
	if (g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME)
	{
		for (i=0;i<2;i++){
			aCard[i] = GetPlayerNP(0)->GetCardNo(i);
		}
		for (i=2;i<min(nCard,8);i++){		
			aCard[i] = GetPlayerNP(0)->GetCardNo(i);
		}
	}
	else {
		for (i=0;i<2;i++){
			aCard[i] = GetPlayerNP(0)->GetCardNo(i);
		}
		
		for (i=2;i<min(nCard,7);i++){
			if (i==6) aCard[i] = GetPlayerNP(0)->GetCardNo(6);
			else     aCard[i] = g_Poker.PS[nSNum].nCard[i-2];
		}
	}

	m_cSvRule.set_total_Card(0,aCard,nCard);
	// 족보
	m_cSvRule.get_cal_rule_user(0);
	eName = (SEVEN_CARD)m_cSvRule.get_MyRule(0);
	CString strName="";
	strName = m_cSvRule.m_cRule[0].get_RuleValue_to_Name();

	return strName;
}

CString CGameProcess_NewPoker::GetJokboName(int nPNum)
{	
	const int nMaxPlayer = GetMaxPlayer();

	if (nPNum < 0 || nPNum >= nMaxPlayer) return "";
	//bts 처리. 승자가 결과를 보기전 방을 나오면 족보가 보이지 않던 버그 수정.
	//이 예외 처리가 왜 있었는지는 모르겠지만, 이거 때문이라 주석처리함
	//2012.07.26 류승민
	//if (strlen(GetPlayerNP(nPNum)->UI.ID)==0 || GetPlayerNP(nPNum)->JoinState == 0 || GetPlayerNP(nPNum)->PlayState == 0 ) return "";
	
	int nSNum = GetPlayerNP(nPNum)->ServPNum;
	if ( g_Poker.PS[nSNum].bFold && (GetPlayerNP(nPNum)->bFoldOpen == false) ) return "";
	
	enum SEVEN_CARD eName;
	int nVal=0,i=0;
	int aCard[10]={0,};
	int nCard = g_Poker.PS[nSNum].nCardTotal;

	if (g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME)
	{
		for (i=0;i<2;i++){
			aCard[i] = g_Poker.BC[nSNum].nCard[i];
		}
		for (i=2;i<min(nCard,8);i++){
			if (i==7) aCard[i] = g_Poker.BC[nSNum].nCard[2];
			else if ( i == 6)aCard[i] = GetPlayerNP(nPNum)->GetCardNo(6);//g_Poker.nBombCard;//
			else     aCard[i] = g_Poker.PS[nSNum].nCard[i-2];
		}
	}
	else 
	{
		for (i=0;i<2;i++){
			aCard[i] = g_Poker.BC[nSNum].nCard[i];
		}
		for (i=2;i<min(nCard,7);i++){
			if (i==6) aCard[i] = g_Poker.BC[nSNum].nCard[2];
			else     aCard[i] = g_Poker.PS[nSNum].nCard[i-2];
		}
	}

	//카드 셋팅
	m_cSvRule.set_total_Card(nPNum,aCard,nCard);
	// 족보값 구하기
	m_cSvRule.get_cal_rule_user(nPNum);

	// 족보 이름 구함 
	eName = (SEVEN_CARD)m_cSvRule.get_MyRule(nPNum);
	CString strName("");
	strName = m_cSvRule.m_cRule[nPNum].get_RuleValue_to_Name();

	return strName;
}

// ### [사운드 추가 작업] ###
void CGameProcess_NewPoker::SoundFxKind(int betkind, int sex, bool bMax)
{
	//규제안 : 맥스 커맨드 추가 : 맥스이펙트
	if ( bMax )
	{
		PBPlaySound(sex?true:false, SND_MAX);
	} 
	else
	{
		switch(betkind)
		{
		case RAISE_LAST_CMD_FOLD: //다이
			PBPlaySound(sex?true:false, SND_DIE);
			break;
		case RAISE_LAST_CMD_CHECK:// 체크
			//규제안 관련 : 카드세팅중에 이펙트 제거
			if (CheckReceivedAllCards()){
				PBPlaySound(sex?true:false, SND_CHECK);
			}
			break;
		case RAISE_LAST_CMD_CALL: // 콜
			//규제안 관련 : 카드세팅중에 이펙트 제거
			if (CheckReceivedAllCards()){
				PBPlaySound(sex?true:false, SND_CALL);
			}
			break;
		case RAISE_LAST_CMD_PING: // 삥
			PBPlaySound(sex?true:false, SND_PING);
			break;
		case RAISE_LAST_CMD_DADANG: // 따당
			PBPlaySound(sex?true:false, SND_DDANG);
			break;
#if defined(BUTTON_CHANGE_FULLTODADANG)
		case RAISE_LAST_CMD_QUARTER:
		case RAISE_LAST_CMD_HALF:
			PBPlaySound(sex?true:false, SND_HALF);
			break;
		case RAISE_LAST_CMD_FULL:
			PBPlaySound(sex?true:false, SND_FULL);
			break;
#else
		case RAISE_LAST_CMD_QUARTER:
		case RAISE_LAST_CMD_HALF:
		case RAISE_LAST_CMD_FULL:
			PBPlaySound(sex?true:false, SND_HALF);
			break;
#endif
		case RAISE_LAST_CMD_ALLIN:
			PBPlaySound(sex?true:false, SND_ALLIN);
			break;
		}
	}
}

void CGameProcess_NewPoker::SoundFxKindBig(int betkind, int sex, bool bMax )
{
	//규제안 : 맥스 커맨드 추가 : 맥스이펙트
	if ( bMax )
	{
		PBPlaySound(sex?true:false, SND_MAX_H);
	} 
	else
	{
		switch(betkind)
		{
		case RAISE_LAST_CMD_FOLD: //다이
			PBPlaySound(sex?true:false, SND_DIE_H);
			break;
		case RAISE_LAST_CMD_CHECK:// 체크
			//규제안 관련 : 카드세팅중에 이펙트 제거
			if (CheckReceivedAllCards()){
				PBPlaySound(sex?true:false, SND_CHECK_H);
			}
			break;
		case RAISE_LAST_CMD_CALL: // 콜
			//규제안 관련 : 카드세팅중에 이펙트 제거
			if (CheckReceivedAllCards()){
				PBPlaySound(sex?true:false, SND_CALL_H);
			}
			break;
		case RAISE_LAST_CMD_PING: // 삥
			PBPlaySound(sex?true:false, SND_PING_H);
			break;
		case RAISE_LAST_CMD_DADANG: // 따당
			PBPlaySound(sex?true:false, SND_DDANG_H);
			break;
#if defined(BUTTON_CHANGE_FULLTODADANG)
		case RAISE_LAST_CMD_QUARTER:
		case RAISE_LAST_CMD_HALF:
			PBPlaySound(sex?true:false, SND_HALF_H);
			break;
		case RAISE_LAST_CMD_FULL:
			PBPlaySound(sex?true:false, SND_FULL_H);
			break;
#else
		case RAISE_LAST_CMD_QUARTER:
		case RAISE_LAST_CMD_HALF:
		case RAISE_LAST_CMD_FULL:
			PBPlaySound(sex?true:false, SND_HALF_H);
			break;
#endif
		case RAISE_LAST_CMD_ALLIN:
			PBPlaySound(sex?true:false, SND_ALLIN_H);
			break;
		}
	}
}

int CGameProcess_NewPoker::GetWinnerValue(int nPNum,int &nVal)
{
	const int nMaxPlayer = GetMaxPlayer();

	if (nPNum < 0 || nPNum >= nMaxPlayer) return -1;
	if (strlen(GetPlayerNP(nPNum)->UI.ID)==0 || GetPlayerNP(nPNum)->JoinState == 0 || GetPlayerNP(nPNum)->PlayState == 0 ) return -1;
	
	int nSNum = GetPlayerNP(nPNum)->ServPNum;
	if ( g_Poker.PS[nSNum].bFold ) return -1;

	enum SEVEN_CARD eName;
	int i=0;
	int aCard[10]={0,};
	int nCard = g_Poker.PS[nSNum].nCardTotal;
//	if (nCard<6) return ""; //  마지막

	if (g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME)
	{
		for (i=0;i<2;i++){
			aCard[i] = g_Poker.BC[nSNum].nCard[i];
		}
		for (i=2;i<min(nCard,8);i++){
			if (i==7) aCard[i] = g_Poker.BC[nSNum].nCard[2];
			else if ( i == 6){
				aCard[i] = GetPlayerNP(nPNum)->GetCardNo(6);//g_Poker.nBombCard;//				
			}
			else     aCard[i] = g_Poker.PS[nSNum].nCard[i-2];
		}
	}
	else 
	{
		for (i=0;i<2;i++){
			aCard[i] = g_Poker.BC[nSNum].nCard[i];
		}
		for (i=2;i<min(nCard,7);i++){
			if (i==6) aCard[i] = g_Poker.BC[nSNum].nCard[2];
			else     aCard[i] = g_Poker.PS[nSNum].nCard[i-2];
		}
	}

	//카드 셋팅
	m_cSvRule.set_total_Card(nPNum,aCard,nCard);
	// 족보값 구하기
	m_cSvRule.get_cal_rule_user(nPNum);
	
	eName = (SEVEN_CARD)m_cSvRule.get_MyRule(nPNum);
	return  eName;
}

int CGameProcess_NewPoker::GetBigGame()				// 판이 큰지 아닌지 검사.
{	
	return 1;
}

int  CGameProcess_NewPoker::GetTotalCardNum()
{
	
	if (g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME)
		return 8;
	else return 7;
	
	return 0;
}

void CGameProcess_NewPoker::SetResultCardDeck(BOOL open, BOOL fold, BOOL jactpot, int pnum)
{
	int spnum  = GetServPNum_ByPN(pnum);
	
	if (g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME)
	{
		if ( open && !fold )
		{
			if (pnum == 0)
			{
				//	if ( g_ObserverMan.IsMyObserver() ) {// ### [ 관전기능 ] ###
				GetPlayerNP(pnum)->m_listCard[0].SetCardNo(g_Poker.BC[spnum].nCard[0]); // 값 변경
				GetPlayerNP(pnum)->m_listCard[1].SetCardNo(g_Poker.BC[spnum].nCard[1]); // 값 변경
				GetPlayerNP(pnum)->m_listCard[7].SetCardNo(g_Poker.BC[spnum].nCard[2]); // 값 변경
				//		}
				GetPlayerNP(0)->SetFaceUp(0);
				GetPlayerNP(0)->SetFaceUp(1);
				GetPlayerNP(0)->SetFaceUp(7);
			}
			else
			{
				GetPlayerNP(pnum)->m_listCard[0].SetCardNo(g_Poker.BC[spnum].nCard[0]); // 값 변경
				GetPlayerNP(pnum)->m_listCard[1].SetCardNo(g_Poker.BC[spnum].nCard[1]); // 값 변경
				GetPlayerNP(pnum)->m_listCard[7].SetCardNo(g_Poker.BC[spnum].nCard[2]); // 값 변경
			}
		} 
		else if ( !open && jactpot==1 && !fold) 
		{
			GetPlayerNP(pnum)->m_listCard[0].SetCardNo(g_Poker.BC[spnum].nCard[0]); // 값 변경
			GetPlayerNP(pnum)->m_listCard[1].SetCardNo(g_Poker.BC[spnum].nCard[1]); // 값 변경
			GetPlayerNP(pnum)->m_listCard[7].SetCardNo(g_Poker.BC[spnum].nCard[2]); // 값 변경
		}
		else if (!open && !fold)
		{			
			if (!GetPlayerNP(0)->bFold)GetPlayerNP(0)->SetFaceUp(7);
		}
	}
	else // normal, choice
	{
		if ( open && !fold )
		{
			if (pnum == 0)
			{
				GetPlayerNP(pnum)->m_listCard[0].SetCardNo(g_Poker.BC[spnum].nCard[0]); // 값 변경
				GetPlayerNP(pnum)->m_listCard[1].SetCardNo(g_Poker.BC[spnum].nCard[1]); // 값 변경
				GetPlayerNP(pnum)->m_listCard[6].SetCardNo(g_Poker.BC[spnum].nCard[2]); // 값 변경

				GetPlayerNP(0)->SetFaceUp(0);
				GetPlayerNP(0)->SetFaceUp(1);
				GetPlayerNP(0)->SetFaceUp(6);
			}
			else
			{
				GetPlayerNP(pnum)->m_listCard[0].SetCardNo(g_Poker.BC[spnum].nCard[0]); // 값 변경
				GetPlayerNP(pnum)->m_listCard[1].SetCardNo(g_Poker.BC[spnum].nCard[1]); // 값 변경
				GetPlayerNP(pnum)->m_listCard[6].SetCardNo(g_Poker.BC[spnum].nCard[2]); // 값 변경			
			}
		}
		else if ( !open && jactpot==1 && !fold) 
		{
			GetPlayerNP(pnum)->m_listCard[0].SetCardNo(g_Poker.BC[spnum].nCard[0]); // 값 변경
			GetPlayerNP(pnum)->m_listCard[1].SetCardNo(g_Poker.BC[spnum].nCard[1]); // 값 변경
			GetPlayerNP(pnum)->m_listCard[6].SetCardNo(g_Poker.BC[spnum].nCard[2]); // 값 변경		
		}
		else if (!open && !fold)
		{			
			if (!GetPlayerNP(0)->bFold)GetPlayerNP(0)->SetFaceUp(6);
		}
	}

	// 본인카드오픈 상태에 따라서 최종 결과를 더 출력한다.
	if( GetPlayerNP(pnum)->bFoldOpen )
	{

		if (g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME)
		{
			GetPlayerNP(pnum)->m_listCard[0].SetCardNo(g_Poker.BC[spnum].nCard[0]); // 값 변경
			GetPlayerNP(pnum)->m_listCard[1].SetCardNo(g_Poker.BC[spnum].nCard[1]); // 값 변경
			GetPlayerNP(pnum)->m_listCard[7].SetCardNo(g_Poker.BC[spnum].nCard[2]); // 값 변경

			GetPlayerNP(pnum)->SetFaceUp(0); // 카드 앞면 표시
			GetPlayerNP(pnum)->SetFaceUp(1); // 카드 앞면 표시
			GetPlayerNP(pnum)->SetFaceUp(7); // 카드 앞면 표시

			GetPlayerNP(pnum)->m_listCard[2].SetCardNo(g_Poker.PS[spnum].nCard[0]); // 값 변경
			GetPlayerNP(pnum)->m_listCard[3].SetCardNo(g_Poker.PS[spnum].nCard[1]); // 값 변경
			GetPlayerNP(pnum)->m_listCard[4].SetCardNo(g_Poker.PS[spnum].nCard[2]); // 값 변경
			GetPlayerNP(pnum)->m_listCard[5].SetCardNo(g_Poker.PS[spnum].nCard[3]); // 값 변경


			if (g_RI.FormKind == JOKER_GAME)
			{
				/*
				int nJoker = m_cGameOverResultData.JokerCardDeck[pnum];
				//int nJoker = g_JokerCardDeck.nJokerCardDeck[spnum];
				GetPlayerNP(pnum)->m_listCard[6].SetCardNo(nJoker); // 카드 번호 세팅
				GetPlayerNP(pnum)->m_listCard[6].SetJokerCardNo(nJoker);
				GetPlayerNP(pnum)->m_listCard[6].SetJokerCard(TRUE);
				*/
			}
			else if (g_RI.FormKind == BOMB_GAME)
			{
				/*
				GetPlayerNP(pnum)->m_listCard[6].SetCardNo(g_Poker.nBombCard);
				GetPlayerNP(pnum)->m_listCard[6].SetIsBombCard(TRUE);
				GetPlayerNP(pnum)->m_listCard[6].SetBombCard(TRUE);
				GetPlayerNP(pnum)->m_listCard[6].SetBombShow(TRUE);
				*/
			}

			GetPlayerNP(pnum)->SetFaceUp(2); // 카드 앞면 표시
			GetPlayerNP(pnum)->SetFaceUp(3); // 카드 앞면 표시
			GetPlayerNP(pnum)->SetFaceUp(4); // 카드 앞면 표시
			GetPlayerNP(pnum)->SetFaceUp(5); // 카드 앞면 표시
			//GetPlayerNP(pnum)->SetFaceUp(6); // 카드 앞면 표시

			//GetPlayerNP(pnum)->SetCardResult(); // 족보 표시
			//GetPlayerNP(pnum)->Draw_DarkCard(); // 족보에 해당하지 않는 카드 어둡게
		}
		else
		{
			GetPlayerNP(pnum)->m_listCard[0].SetCardNo(g_Poker.BC[spnum].nCard[0]); // 값 변경
			GetPlayerNP(pnum)->m_listCard[1].SetCardNo(g_Poker.BC[spnum].nCard[1]); // 값 변경
			GetPlayerNP(pnum)->m_listCard[6].SetCardNo(g_Poker.BC[spnum].nCard[2]); // 값 변경

			GetPlayerNP(pnum)->SetFaceUp(0); // 카드 앞면 표시
			GetPlayerNP(pnum)->SetFaceUp(1); // 카드 앞면 표시
			GetPlayerNP(pnum)->SetFaceUp(6); // 카드 앞면 표시

			GetPlayerNP(pnum)->m_listCard[2].SetCardNo(g_Poker.PS[spnum].nCard[0]); // 값 변경
			GetPlayerNP(pnum)->m_listCard[3].SetCardNo(g_Poker.PS[spnum].nCard[1]); // 값 변경
			GetPlayerNP(pnum)->m_listCard[4].SetCardNo(g_Poker.PS[spnum].nCard[2]); // 값 변경
			GetPlayerNP(pnum)->m_listCard[5].SetCardNo(g_Poker.PS[spnum].nCard[3]); // 값 변경

			GetPlayerNP(pnum)->SetFaceUp(2); // 카드 앞면 표시
			GetPlayerNP(pnum)->SetFaceUp(3); // 카드 앞면 표시
			GetPlayerNP(pnum)->SetFaceUp(4); // 카드 앞면 표시
			GetPlayerNP(pnum)->SetFaceUp(5); // 카드 앞면 표시
		}
	}
}

void CGameProcess_NewPoker::SetEnterRoomCardDeck(int nOpCnt , int pn)
{
	int spnum  = GetServPNum_ByPN(pn);
	int k = 0;
	
	if (g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME)
	{		
		// 폭탄 카드 셋 
		if (nOpCnt >=2 && nOpCnt <= 4)
		{
			m_cBombCard.ForceSet(52);
		}
		else if (nOpCnt == 5)
		{
			m_cBombCard.ForceSet(g_Poker.nBombCard);
		}

		if (nOpCnt <=  5) {
			for (k = 0; k < nOpCnt; k++)	{
				GetPlayerNP(pn)->m_listCard[k+2].SetCardNo(g_Poker.PS[spnum].nCard[k]); // 값 변경
				GetPlayerNP(pn)->SetFaceUp(k+2); // 보이기
				// 폭탄 카드 삽입
				if (k == 4 && (k+2) == 6){
					GetPlayerNP(pn)->m_listCard[k+2].SetCardNo(g_Poker.nBombCard);
					GetPlayerNP(pn)->m_listCard[k+2].SetIsBombCard(TRUE);
					GetPlayerNP(pn)->m_listCard[k+2].SetBombCard(TRUE);
					GetPlayerNP(pn)->m_listCard[k+2].SetBombShow(TRUE);
					if (g_RI.FormKind == JOKER_GAME && g_Poker.nBombCard == 100)
					{
						g_bGetJokerCard = TRUE;
						GetPlayerNP(pn)->m_listCard[k+2].SetJokerCard(TRUE);
					}
				}
				
				CPoint pt = GetPlayerNP(pn)->m_listCard[k+2].GetPos();
				GetPlayerNP(pn)->m_listCard[k+2].GetMove().SetCurPos(pt);
			}
		}
		else {// 모두 카드 오픈
			// 히든 카드
			GetPlayerNP(pn)->m_listCard[0].SetCardNo(g_Poker.BC[spnum].nCard[0]);
			GetPlayerNP(pn)->m_listCard[1].SetCardNo(g_Poker.BC[spnum].nCard[1]);
			GetPlayerNP(pn)->m_listCard[7].SetCardNo(g_Poker.BC[spnum].nCard[2]);
			GetPlayerNP(pn)->SetFaceUp(0);
			GetPlayerNP(pn)->SetFaceUp(1);
			GetPlayerNP(pn)->SetFaceUp(7);
			
			for ( k = 0; k < nOpCnt; k++)	{
				GetPlayerNP(pn)->m_listCard[k+2].SetCardNo(g_Poker.PS[spnum].nCard[k]); // 값 변경
				GetPlayerNP(pn)->SetFaceUp(k+2); // 보이기

				// 폭탄 카드 삽입
				if (k == 4 && (k+2) == 6){					
					GetPlayerNP(pn)->m_listCard[6].SetCardNo(g_Poker.nBombCard);
					GetPlayerNP(pn)->m_listCard[6].SetIsBombCard(TRUE);
					GetPlayerNP(pn)->m_listCard[6].SetBombCard(TRUE);
					GetPlayerNP(pn)->m_listCard[6].SetBombShow(TRUE);
				}
			}
			// 위치셋
			for (k = 0; k < 8; k++)	{
				CPoint pt = GetPlayerNP(pn)->m_listCard[k].GetPos();
				GetPlayerNP(pn)->m_listCard[k].GetMove().SetCurPos(pt);
			}
		}
	}
	else{
		if (nOpCnt <=  4) {
			for (k = 0; k < nOpCnt; k++)	{
				GetPlayerNP(pn)->m_listCard[k+2].SetCardNo(g_Poker.PS[spnum].nCard[k]); // 값 변경
				GetPlayerNP(pn)->SetFaceUp(k+2); // 보이기
				
				CPoint pt = GetPlayerNP(pn)->m_listCard[k+2].GetPos();
				GetPlayerNP(pn)->m_listCard[k+2].GetMove().SetCurPos(pt);
			}
		}
		else {// 모두 카드 오픈
			// 히든 카드
			GetPlayerNP(pn)->m_listCard[0].SetCardNo(g_Poker.BC[spnum].nCard[0]);
			GetPlayerNP(pn)->m_listCard[1].SetCardNo(g_Poker.BC[spnum].nCard[1]);
			GetPlayerNP(pn)->m_listCard[6].SetCardNo(g_Poker.BC[spnum].nCard[2]);
			GetPlayerNP(pn)->SetFaceUp(0);
			GetPlayerNP(pn)->SetFaceUp(1);
			GetPlayerNP(pn)->SetFaceUp(6);
			
			for ( k = 0; k < nOpCnt; k++)	{
				GetPlayerNP(pn)->m_listCard[k+2].SetCardNo(g_Poker.PS[spnum].nCard[k]); // 값 변경
				GetPlayerNP(pn)->SetFaceUp(k+2); // 보이기
			}
			// 위치셋
			for (k = 0; k < 7; k++)	{
				CPoint pt = GetPlayerNP(pn)->m_listCard[k].GetPos();
				GetPlayerNP(pn)->m_listCard[k].GetMove().SetCurPos(pt);
			}
		}
	}
}

void CGameProcess_NewPoker::SetADDBombCard()
{
	m_cBombCard.SetBombCard();
}

void CGameProcess_NewPoker::SetOPenBombCard(char nCard)
{	
	int i=0;
	const int nMaxPlayer = GetMaxPlayer();
	for (i = 0 ; i < nMaxPlayer ; i ++){
		g_Poker.RU[i].nLastCmd = RAISE_LAST_CMD_NULL;
	}

	// 베팅머니 표시되는 부분을 초기화 한다.
	// 2012. 04. 25 최종학
	for (int i = 0; i < GetMaxPlayer(); ++i)
	{
		g_pGameView->m_cPlayerDrawManager.ClearBettingMoney(i);
	}
	
	m_cBombCard.SetOPenBombCard(nCard);

	for (i=0; i<nMaxPlayer; i++) {		
		if (strlen(GetPlayerNP(i)->UI.ID)>0 && GetPlayerNP(i)->JoinState == 1 && GetPlayerNP(i)->PlayState == 1 && !GetPlayerNP(i)->bFold ) // 겜중이면
		{
			int spnum = GetServPNum_ByPN(i);

			if (g_RI.FormKind == JOKER_GAME)
			{
				if (g_bGetJokerCard == TRUE)
				{				
					if (!g_ObserverMan.IsMyObserver()){

						if (g_JokerCardDeck.nJokerCardDeck[spnum] == -1)
						{
							GetPlayerNP(i)->m_listCard[6].SetCardNo(100);
							GetPlayerNP(i)->SetNewCard( 100, TRUE);  // 값 등록			
						}
						else
						{
							GetPlayerNP(i)->m_listCard[6].SetCardNo(g_JokerCardDeck.nJokerCardDeck[spnum]);
							GetPlayerNP(i)->SetNewCard( g_JokerCardDeck.nJokerCardDeck[spnum], TRUE);  // 값 등록						
						}
					}
					else
					{
						GetPlayerNP(i)->m_listCard[6].SetCardNo(100);
						GetPlayerNP(i)->SetNewCard( 100, TRUE);  // 값 등록			
					}
					GetPlayerNP(i)->m_listCard[6].SetJokerCard(TRUE);
				}
				else
				{
					GetPlayerNP(i)->m_listCard[6].SetCardNo(nCard);
					GetPlayerNP(i)->SetNewCard( nCard, TRUE);  // 값 등록			
				}
			}
			else
			{
				GetPlayerNP(i)->m_listCard[6].SetCardNo(nCard);
				GetPlayerNP(i)->SetNewCard( nCard, TRUE);  // 값 등록			
			}
			
			GetPlayerNP(i)->m_listCard[6].SetBombCard(TRUE);
			GetPlayerNP(i)->m_listCard[6].SetBombShow(FALSE);

			
			g_Poker.PS[spnum].nCardTotal++;
			g_Poker.PS[spnum].nOpenTotal++;
			GetPlayerNP(i)->m_nCardTotal = g_Poker.PS[spnum].nCardTotal;
			GetPlayerNP(i)->m_nOpenTotal = g_Poker.PS[spnum].nOpenTotal;
		}
	}
	
	g_Poker.nBombCard = nCard;

	//규제안 예약베팅 셋팅 초기화
	ReserveState(RESERVESTATE_BET_NONE);
	////규제안 :  예약 가능여부 설정
	CanReserve(false);
	g_pGameView->m_cGameViewBtnMan.ShowReserveBtn(false);
}

void CGameProcess_NewPoker::OnOPenBombCard(int nCard)
{
	int i=0;
	const int nMaxPlayer = GetMaxPlayer();
	for (i = 0 ; i < nMaxPlayer ; i ++){
		g_Poker.RU[i].nLastCmd = RAISE_LAST_CMD_NULL;
	}

	//베팅 커멘드를 지움
	if (m_cAniSprObjectMan.pFirst != NULL)
	{
		CBaseObject *pNow = NULL;
		CBaseObject* pNext = m_cAniSprObjectMan.pFirst;
		while(pNext)
		{
			CBaseObject* pNow = pNext;
			
			if (pNow->m_nObjID == ANF_BET_CMD)
			{
				CBetCmdAni* pObj = (CBetCmdAni*)pNow;
				pObj->Destroy();
			}
			pNext = pNow->pNextNode;
		}
	}

	CPoint stpt,dest_pos;
	stpt = g_cUIData.m_ptBonusOpen;

	CMyCard* pLastCard = NULL;// 마지막 카드

	int pnum = GetPNum_ByServPN( g_Poker.nDistUser ); // 카드 날리는 순서
	if (pnum < 0 || pnum >= nMaxPlayer) return;
	
	DWORD dwDelay = 0;
	for (i=0; i<nMaxPlayer; i++) {		
		int index = i;
		if (index >= nMaxPlayer) index = index - nMaxPlayer;

		if (strlen(GetPlayerNP(index)->UI.ID)>0 && GetPlayerNP(index)->JoinState == 1 && GetPlayerNP(index)->PlayState == 1) // 겜중이면
		{
			CMyCard* pCard = &GetPlayerNP(index)->m_listCard[6];
			if (pCard != NULL)  {
			
				GetPlayerNP(index)->m_listCard[6].SetBombShow(TRUE);
				dest_pos = pCard->GetPos();
				pCard->GetMove().StartMove(stpt, dest_pos, dwDelay, 300);

				// 한장씩 도착하면 보여준다
				pCard->SetEvent(EVENT_BOMBFLYEND, index, 6 ); // 플레이 넘버,인덱스 값
				pLastCard = pCard;
				dwDelay += 100; // 폭탄카드 딜레이.
			}
		}
	}
	
	if (pLastCard != NULL)
	{
		pLastCard->SetEvent1(EVENT_BOMBFLY_TO_END); // 모두 도착하면 서버로		
	}
}

void CGameProcess_NewPoker::OnSetMoveChip()
{
	const int nMaxPlayer = GetMaxPlayer();
	for (int i = 0 ; i < nMaxPlayer ; i ++){
		GetPlayerNP(i)->OnReSetBetDraw();
	}
}

INT64 CGameProcess_NewPoker::GetLastBetMoney(int sPNum,int betCmd)
{
	INT64 Betmoney = 0;
	INT64 CallMoney = 0;
	INT64 DadangMoney = 0;
	INT64 MaxMoney = 0;
	INT64 AllinMoney = 0;

	//하프 금액
	double fBatRate = 0.25;
//	if (g_RI.GameKind==0)      fBatRate = 0.25; // 쿼터방
//	else if (g_RI.GameKind==1) fBatRate = 0.5;  // 하프방
//	else                      fBatRate = 0.25; // 쿼터방		

	int pnum = GetServPNum_ByPN(sPNum);
	// 삥금액//[vsmode]070314
	Betmoney = g_RI.CurSeedMoney;

	//콜 금액
	CallMoney = g_Poker.RU[sPNum].nReCall;

	//따당 금액
	DadangMoney = g_Poker.RU[sPNum].nReCall + CallMoney;
	
	// 하프 
	MaxMoney = g_Poker.RU[sPNum].nReCall + g_Poker.RU[sPNum].nCalcBat;

	//올인 금액	
	AllinMoney = g_Poker.RU[sPNum].nCalcBat;
	
	if (betCmd == 0)
	{
		return  0;
	}
	else if ( betCmd == 1)
	{
		return 0;
	}
	else if ( betCmd == 2) // 콜 금액
	{
		return  CallMoney;
	}
	else if (betCmd == 3) // 삥 금액
	{
		return 0;
	}
	else if (betCmd == 4) // 따당 금액
	{
		return DadangMoney;
	}
	else if (betCmd == 5) // 하프 금액
	{
		return MaxMoney	;
	}
	else if (betCmd == 6) // 올인
	{
		return AllinMoney;
	}
	return 0;
}

void CGameProcess_NewPoker::DebugAutobet()
{
	if ( g_pGameView->m_cGameViewBtnMan.m_bBMax ) {
		g_pGameView->OnBtnMax();
	}
	else if ( g_pGameView->m_cGameViewBtnMan.m_bBCall ) {
		g_pGameView->OnBtnCall();
	} 
	else if ( g_pGameView->m_cGameViewBtnMan.m_bBCheck ) {
		g_pGameView->OnBtnCheck();
	}
}

void CGameProcess_NewPoker::Insert_EndingTextInformation(CString str)
{
	if ( m_nEndingInformationTextMsgCnt >= 30) return;
	if ( m_nEndingInformationTextMsgCnt < 0 )return;
	
	m_listEndingInformationTextMsg[m_nEndingInformationTextMsgCnt] = str;
	m_nEndingInformationTextMsgCnt ++;
}

void CGameProcess_NewPoker::Draw_TextInformationMsg()
{
	if ( m_nEndingInformationTextMsgCnt <= 0)return;
	
	COLORREF tcolor = GetChatColor(ECC2_NOTIFY);

	for ( int i = 0 ; i < m_nEndingInformationTextMsgCnt ; i ++ )
	{
		AddGameInfoViewEdit(m_listEndingInformationTextMsg[i], tcolor);
	}
}

void CGameProcess_NewPoker::OnCheckTurnLimitTime()
{
	if (g_RI.State != 1)return;
	if (g_pGameView->m_cPlayerDrawManager.GetStartTimerTime() <= 0)return;

	g_pGameView->m_cPlayerDrawManager.SetCurTimerTime(timeGetTime());

	if (g_Poker.nState == RSTATE_PREPARESTART)
	{
		if ( g_pGameView->m_cPlayerDrawManager.IsSelectCard(0) == FALSE)
			return;

		int tcnt = g_pGameView->m_cPlayerDrawManager.GetTimerTimeGab() / 1000;
		

#ifdef _DEBUG
		if (tcnt > PBTurnTimeOut(ETTT_BETTING) && m_cSelectCardMan.IsChoiceCard() ){
			if ( g_bDeBugModeFlg == TRUE )
			{
				if (g_pGameView->m_cPlayerDrawManager.IsSelectCard(0) == TRUE ){
					m_cSelectCardMan.OnSelectCard(0);
					m_cSelectCardMan.OnSelectCard(1);
					g_pGameView->m_cPlayerDrawManager.SetSelectCard(0, false);
					return;
				}
			}
			else{
				//2006.3.20
				//카드 선택시 시간이 지나 게임에서 자동으로 선택한다. 3번까지 자동 선택하면 게임 종료 후 강퇴 시킨다.
				if ( GetPlayerNP(0)->m_nCardChoiceLimtCnt < MAX_TURN_TIMEOUT_LIMIT_COUNT && g_pGameView->m_cPlayerDrawManager.IsSelectCard(0) == TRUE ){
					m_cSelectCardMan.EndSelectCard(TRUE);
					g_pGameView->m_cPlayerDrawManager.SetSelectCard(0, false);
#ifndef _DEBUG
					GetPlayerNP(0)->m_nCardChoiceLimtCnt ++;
#endif
					g_pGameView->m_cDisplayBoard.SetText(EDBT_LIMITTIME);
					return;
				}
			}
#else
		if (tcnt >= PBTurnTimeOut(ETTT_BETTING) && !g_ObserverMan.IsMyObserver() && m_cSelectCardMan.IsChoiceCard() ){
			if ( GetPlayerNP(0)->m_nCardChoiceLimtCnt < MAX_TURN_TIMEOUT_LIMIT_COUNT && g_pGameView->m_cPlayerDrawManager.IsSelectCard(0) == TRUE ){
				m_cSelectCardMan.EndSelectCard(TRUE);
				g_pGameView->m_cPlayerDrawManager.SetSelectCard(0, false);
#ifndef _DEBUG
				GetPlayerNP(0)->m_nCardChoiceLimtCnt ++;
#endif
				g_pGameView->m_cDisplayBoard.SetText(EDBT_LIMITTIME);
				return;
			}
#endif			
		}
	}
	
	
	if (g_Poker.nState == RSTATE_STARTGAME || g_Poker.nState == RSTATE_BOMB)
	{
		if ( GetPlayerNP(0)->PlayState==1 && m_nCurPlayer == GetPlayerNP(0)->ServPNum && !g_pGameView->m_cPlayerDrawManager.IsFlyWindCard()
			&& !GetPlayerNP(0)->bFold ) // 카드 날리는 중에는 그리지 않는다.
		{		
			int tcnt = g_pGameView->m_cPlayerDrawManager.GetTimerTimeGab() / 1000;
			
			if (tcnt > PBTurnTimeOut(ETTT_CARDCHOICE))
			{
				if ( g_pGameView->m_cGameViewBtnMan.m_bFold && false == g_pGameView->m_cGameViewBtnMan.GetButtonClick())
				{
					g_pGameView->OnBtnFold();
					GetPlayerNP(0)->bFold = TRUE;
					g_pMyInfo_NP->bFold = TRUE;
				}
				
				//[7월정책변경]2008.07.01
				if ( g_pGameView->m_cGameViewBtnMan.m_bFold == TRUE && g_RI.nAutoBetKind > 0 && g_Poker.nAutoBetNodie > 0 )
				{					
					g_pGameView->OnBtnCall();
				}
				else
				{
					if ( g_pGameView->m_cGameViewBtnMan.m_bFold && false == g_pGameView->m_cGameViewBtnMan.GetButtonClick())
					{
						g_pGameView->OnBtnFold();
						GetPlayerNP(0)->bFold = TRUE;
						g_pMyInfo_NP->bFold = TRUE;
					}
				}
			}
		}
	}
}

void CGameProcess_NewPoker::OnExitRoom(void)
{
	CGameProcess::OnExitRoom();

	g_ObserverMan.SetMyObserver(FALSE);

	g_pGameView->m_cSprDlgMan.Destroy();
	g_SeatKeeper.OnOutRoom();
	g_cVipJackpot.OnOutRoom();
	g_pGameView->m_cTotalBetMoney.Reset();

	// ### [ 관전기능 ] ###
	GM().GetMyInfo()->UI.cObserver = 0;
	//send를 줄이기위해 2006.3.15
	g_pMyInfo_NP->JoinState = 0;
	g_pMyInfo_NP->PlayState = 0;
	g_pMyInfo_NP->nSndFxKind = g_Config.SndFxKind;
	
	GetPlayerNP(0)->SetNewPlayer(&GM().GetMyInfo()->UI);
	GetPlayerNP(0)->nSndFxKind = g_Config.SndFxKind;
	
// 	if (RewardItemMan().m_RewardItemData.bValid == TRUE)
// 	{
// 		for ( int i = 0 ; i < RewardItemMan().m_RewardItemData.TotalCnt ; i ++ ){
// 			RewardItemMan().AllowBuyItem( RewardItemMan().m_RewardItemData.itemcode[i],
// 			RewardItemMan().m_RewardItemData.gamecode[i],
// 			RewardItemMan().m_RewardItemData.rctime[i],
// 			RewardItemMan().m_RewardItemData.expiretime[i]);
// 		}
// 		
// 		ZeroMemory(&RewardItemMan().m_RewardItemData , sizeof(REWARDITEM_DATA));
// 	}

	m_cAniSprObjectMan.Destroy();
	m_cResultFocus.ReSet(); // 2010.06.15 추가
	NMBASE::SOUND::g_Sound.StopAllWav();
	NMBASE::SOUND::g_Sound.Stop(SND_HIDDEN_BGM);

	m_i64JackPotMoney = 0;
	m_i64OldJackPotMoney = 0;

	// 보유 한도 초과 팝업
	g_cOwnLimit.OnExitRoom();
}

void CGameProcess_NewPoker::OnEnterRoom(BOOL bCreation)
{
	// 본인 카드 오픈 버튼 상태 Off로 초기화(1이면 Off, 0이면 On)
	g_pGameView->m_cGameViewBtnMan.m_btnCardOpen.SetSprite(&(g_pGameView->m_cGameViewBtnMan.m_sprUICardOpenBtn), 1);

	CString str;
	g_pGameView->m_cGameViewBtnMan.m_btnExitBottom.Enable(TRUE);
	g_pGameView->m_cSidePotMoney.Reset();

	// 포커스
	g_pGameView->SetFocus();
	g_pGameView->Reset();
	str.Format("☆도움말☆\n1. 게임창->채팅창 포커스 이동\n단축키 - Enter, ESC\n2. 채팅창->게임창 포커스 이동\n단축키 - ESC\n");
	g_pChatWinMan->ResetChatting_STR();
	AddGameInfoViewEdit(str,GetChatColor(ECC2_NOTIFY));

	g_pGameView->m_cBackGround.OnEnterRoom();
	g_pGameView->m_cGameViewBtnMan.InitObserverBtn();
	
	//채팅
	g_pChatWinMan->SetViewPage(CChattingWindowMan::ECTT_CHAT);

	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();

	//자리 지킴이
	g_SeatKeeper.OnEnterGame();
	g_cOneShotCharge.OnEnterRoom();
	//보유한도
	g_cOwnLimit.OnEnterRoom(&g_cUIData, GAME());
	g_ObserverMan.OnEnterRoom();
	g_cVipJackpot.OnEnterRoom();

	g_pGameView->m_cGameViewBtnMan.SetBetBtnType( g_Config.eBetBtnType, true );
	g_pGameView->m_cGameViewBtnMan.EnterRoom_Button_Init( g_RI.sBetRuleKind );
	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();

	if (!g_ObserverMan.IsMyObserver()) CheckMinimumUser();

	m_cAllBetChip.Init(&g_pGameView->Page, &g_cUIData, g_pGameProcess, &g_sprChip);

	if (!g_ObserverMan.IsMyObserver()){
		AutoBetSetUpWnd().OnEnterRoom();
	}

	//[오류수정안]2010.03.08
	str = "비정상적인 게임행위(짜고치기) 및 사행성 조장 행위(거래,광고)등이 발생할 시 게임 내 신고 버튼을 이용하여, 신고해 주시기 바랍니다.\n";

	AddGameInfoViewEdit(str,GetChatColor(ECC2_NOTIFY));

	if (g_ObserverMan.IsMyObserver())
	{
		str.Format("▶관전 입장: [%s]님\n", g_pMyInfo_NP->UI.NickName);
	}
	else
	{
		str.Format("▶입장: [%s]님\n", g_pMyInfo_NP->UI.NickName);
	}

	AddGameInfoViewEdit(str, GetChatColor(ECC2_NOTIFY));

	str.Format("## <ID: %s(%s)>님 입장 ##\n", g_pMyInfo_NP->UI.ID , g_pMyInfo_NP->UI.NickName);
	AddLobbyHideChatText( &str );


	// 잭팟금액을 세팅해준다. 
	// ( 잭팟 패킷이 게임입장 패킷보다 늦게 올수 있어서 값이 없으면 대기실에서 받은 잭팟 금액으로 세팅을 해준다. )
	if (GetJackPotMoney() <= 0)
	{
		JackPotMoneyUpdate(&g_JackPotMoneyInfo);
	}
	g_cPromotionMan.OnEnterRoom();
}

void CGameProcess_NewPoker::SetRuleCard()
{
	int deck[8] = {0,};

	m_cSvRule.ClearAll();

	const int nMaxPlayer = GetMaxPlayer();
	for (int p = 0 ; p < nMaxPlayer; p++)
	{
		int pp = GetServPNum_ByPN(p);
		
		// 겜중이면
		if ((strlen(GetPlayerNP(p)->UI.ID) > 0) && (GetPlayerNP(p)->JoinState == 1) && (GetPlayerNP(p)->PlayState == 1) && !GetPlayerNP(p)->bFold ) 
		{
			int totCnt = g_Poker.PS[pp].nCardTotal;
			ZeroMemory(deck,sizeof(int)*8);
			for (int j = 0 ; j < totCnt ; j ++)
			{
				deck[j] = GetPlayerNP(p)->m_listCard[j].GetCardNum();
			}
			m_cSvRule.set_total_Card(p,deck,totCnt);
		}
	}

	m_cSvRule.get_cal_rule();
}

void CGameProcess_NewPoker::ResultFourCardInPut(int pnum)
{
	int spnum  = GetPlayerNP(pnum)->ServPNum;
	
	if (g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME)
	{		
		GetPlayerNP(pnum)->m_listCard[0].SetCardNo(g_Poker.BC[spnum].nCard[0]); // 값 변경
		GetPlayerNP(pnum)->m_listCard[1].SetCardNo(g_Poker.BC[spnum].nCard[1]); // 값 변경
		GetPlayerNP(pnum)->m_listCard[7].SetCardNo(g_Poker.BC[spnum].nCard[2]); // 값 변경

		GetPlayerNP(pnum)->SetFaceUp(0);
		GetPlayerNP(pnum)->SetFaceUp(1);
		GetPlayerNP(pnum)->SetFaceUp(7);
	}
	else// normal, choice
	{			
		GetPlayerNP(pnum)->m_listCard[0].SetCardNo(g_Poker.BC[spnum].nCard[0]); // 값 변경
		GetPlayerNP(pnum)->m_listCard[1].SetCardNo(g_Poker.BC[spnum].nCard[1]); // 값 변경
		GetPlayerNP(pnum)->m_listCard[6].SetCardNo(g_Poker.BC[spnum].nCard[2]); // 값 변경
		
		GetPlayerNP(pnum)->SetFaceUp(0);
		GetPlayerNP(pnum)->SetFaceUp(1);
		GetPlayerNP(pnum)->SetFaceUp(6);
	}
}

void CGameProcess_NewPoker::Change_Money(CSV_CHANGE_MONEY *pMsg)
{	
	const char *pUserID = pMsg->ID;
	const char *pUMCode = pMsg->UMCode;
	INT64 ChangeMoney = *pMsg->ChangeMoney;
	INT64 PMoney = *pMsg->PMoney;

	//pUserID : 머니 변화가 일어난 유저아이디 (내가 될수도 있고 내가 있는 방의 다른 유저가 될 수도있다.)
	//pUMCode : 머니 변화가 일어난 메타코드
	//ChnageMoney : 변화된 머니량
	//PMoney : 변화 후 머니 

	if (strcmp(pUserID, GM().GetMyInfo()->UI.ID) == 0) //내머니 라면?
	{
		/*g_pMyInfo_NP->UI.SetMoney(PMoney);*/
		g_pMyInfo_NP->UI.SetRealPMoney(PMoney);
		g_pMyInfo_NP->SetFlag_Money_Chip( MONEY_CHANGED );
		if ( !g_ObserverMan.IsMyObserver() ) 
		{
			/*GetPlayerNP(0)->UI.SetMoney(PMoney);*/
			GetPlayerNP(0)->UI.SetRealPMoney(PMoney);
			GetPlayerNP(0)->SetFlag_Money_Chip( MONEY_CHANGED );
			//PLAYER(0)->PrevMoney = PLAYER(0)->UI.GetMoney();//PMONEY , PMONEY2 통합시에 아래 살리기 //@#$%
		}
	}
	else	//상대방이라면?
	{
		int pnum = GetPlayerPNum((char*)pUserID);
		if (pnum != -1) {								
			/*GetPlayerNP(pnum)->UI.SetMoney(PMoney);*/
			GetPlayerNP(pnum)->UI.SetRealPMoney(PMoney);
			GetPlayerNP(pnum)->SetFlag_Money_Chip( MONEY_CHANGED );
		}
	}
		
	CString strUMCode = pUMCode;
	if ( strUMCode == "ABB02019" || strUMCode == "ABB02020" || strUMCode == "ABB02021" ) //원샷(재기)충전 메타코드 
	{
		//원샷(재기)충전
		g_cOneShotCharge.OnChargeMoney(*pMsg);
	}
	if (GetCurrentWhere()==IDX_GAMEWHERE_LOBY){
		MM().Call(UPG_CMK_RESET_GAMEMONEY, 0, 0);
	}
}

void CGameProcess_NewPoker::ReSetGameOverFlg()
{
	int i=0;
	GM().SetWaitResponse(false);
	g_RI.State = 0;
	m_bGameStart = FALSE;// 게임 시작 플래그 세팅
	g_pGameView->m_cPlayerDrawManager.SetGameOver(true, timeGetTime());
	const int nMaxPlayer = GetMaxPlayer();

	int wpnum = GetPNum_ByServPN(g_pGameView->m_cPlayerDrawManager.GetWinnerPNum());

	MAP_RANKING mapRanking;
	for (i=0; i<nMaxPlayer; i++)
	{
		if (strlen(m_cGameOverResultData.Ui[i].ID)>0) //  겜참여자.
		{
			int pnum = GetPlayerPNum(m_cGameOverResultData.Ui[i].ID);
			if (pnum == -1) continue;
			if (m_cGameOverResultData.Ui[i].nRank < 0) continue;

			mapRanking[m_cGameOverResultData.Ui[i].nRank][pnum] = m_cGameOverResultData.Ui[i].nResultMoney;
		}
	}
	g_pGameView->m_cSidePotMoney.SetGameOver(mapRanking);

	CPlayer_NP *pPlayer = NULL;
	for (i=0; i<nMaxPlayer; i++)
	{
		if (strlen(m_cGameOverResultData.Ui[i].ID)>0) //  겜참여자.
		{
			int pnum = GetPlayerPNum(m_cGameOverResultData.Ui[i].ID);
			if (pnum == -1) continue;
			// 결과 증감 머니

			pPlayer = GetPlayerNP(pnum);
			memcpy( &pPlayer->UI.AllGameRecord, &m_cGameOverResultData.Ui[i].AllGameRecord, sizeof(m_cGameOverResultData.Ui[i].AllGameRecord) );
			pPlayer->SetChangeRoomInfo(m_cGameOverResultData.Cui[i]);

			pPlayer->UI.PI.nFCARD = m_cGameOverResultData.Ui[i].PI.nFCARD;
			pPlayer->UI.PI.nSF = m_cGameOverResultData.Ui[i].PI.nSF;
			pPlayer->UI.PI.nRSF = m_cGameOverResultData.Ui[i].PI.nRSF;

			if (pnum == 0 && !g_ObserverMan.IsMyObserver())
			{
				g_pMyInfo_NP->UI.DS_IDColor = GetPlayerNP(0)->UI.DS_IDColor ;
				g_pMyInfo_NP->UI.DS_GetMaxMoney = GetPlayerNP(0)->UI.DS_GetMaxMoney ;
				g_pMyInfo_NP->UI.DS_GetBetMaxMoney = GetPlayerNP(0)->UI.DS_GetBetMaxMoney ;
				g_pMyInfo_NP->SetChangeRoomInfo(*GetPlayerNP(0)->GetChangeRoomInfo());
				//뉴도신지수 2006.3.2
				g_pMyInfo_NP->UI.DS_SupplyBenefit = GetPlayerNP(0)->UI.DS_SupplyBenefit;
				g_pMyInfo_NP->UI.DS_Exp = GetPlayerNP(pnum)->UI.DS_Exp;
			}
			
			if (pPlayer->m_i64MyJackpatMoney > 0)
			{				
				if ( pPlayer->IsFlag_Money_Chip(MONEY_CHANGED) == FALSE && g_RI.ChipKind != 1 )
				{
					//pPlayer->UI.SetMoney(m_cGameOverResultData.Ui[i].PMoney + pPlayer->m_i64MyJackpatMoney);
					//Max방 제한 설정
					if ( g_RI.bIsMaxRoom )
					{
						pPlayer->SetNowMoney( m_cGameOverResultData.Ui[i].llInMoney + pPlayer->m_i64MyJackpatMoney );
					}
					else
					{
						pPlayer->SetNowMoney( m_cGameOverResultData.Ui[i].PMoney );
					}
				}
				
				pPlayer->m_i64MyJackpatMoney = 0;
			}
			else
			{
				if ( pPlayer->IsFlag_Money_Chip(MONEY_CHANGED) == FALSE && g_RI.ChipKind != 1 )
				{
					//Max방 제한 설정
					if ( g_RI.bIsMaxRoom )
					{
						pPlayer->SetNowMoney( m_cGameOverResultData.Ui[i].llInMoney );
					}
					else
					{
						pPlayer->SetNowMoney( m_cGameOverResultData.Ui[i].PMoney );
					}
				}
			}
			
			if (	pPlayer->GetNowMoney() < 100 && g_RI.ChipKind == 0) 	pPlayer->SetNowMoney(0);
			// 레벨상점 폐쇄
			pPlayer->UI.nIcon = GetMoneyGrade(pPlayer->UI.GetTotalMoney());

			memcpy(&pPlayer->UI.PI,&m_cGameOverResultData.Ui[i].PI,sizeof(POKERINFO));
		}
	}
	
	// ### [ 관전기능 ] ###
	if ( g_ObserverMan.IsMyObserver() != TRUE ) //관전이 아니면
	{
		GetPlayerNP(0)->UI.CopyAllGameRecords( g_pMyInfo_NP->UI.AllGameRecord );
		g_pMyInfo_NP->UI.AllinCnt =  GetPlayerNP(0)->UI.AllinCnt;
		g_pMyInfo_NP->UI.SetMoney(GetPlayerNP(0)->UI.GetMoney()); 
		g_pMyInfo_NP->UI.nIcon   =  GetPlayerNP(0)->UI.nIcon;
		memcpy(&g_pMyInfo_NP->UI.PI,&GetPlayerNP(0)->UI.PI,sizeof(POKERINFO));
	}
	
	CString str="";
	CString strM="";
	/*
┌───────────────┐
   ★ [bital]님 승★               
└───────────────┘ 
* 총배팅액: 
* 획득금액: 
* 족보: 
─────────────────
  aaaa   -10만원 (족보)
  bbbb   -50만원 (족보)
  게임비 5%가 적립되었습니다.
┌───────────────┐
           게임을 시작 합니다.
└───────────────┘ 
*/

	if ( !g_pGameView-> m_cPlayerDrawManager.IsOpen() ) // 승자 1명 (나머지 유저 폴드)
	{
		CString str1;
		//str1.Format(" ----------------------------\n%s ----------------------------\n",g_StrMan.Get(_T("WIN_MESINFO")));
		str1.Format(" %s",g_StrMan.Get(_T("WIN_MESINFO")));
		str.Format(str1,m_cGameOverResultData.Ui[g_pGameView->m_cPlayerDrawManager.GetWinnerPNum()].NickName);

		AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT));

		INT64 nTotBat = GetRealBet();

		strM = (CString)NMBASE::UTIL::g_MoneyMark(m_cGameOverResultData.nWinMoney); // 하이 머니 

		strM.Append(strChip_Name[g_RI.ChipKind]);

		// 골드 경기장일 경우
		if (g_RI.ChipKind==1)
		{			
			// 딜러비 할인 이벤트 중일 경우 표시
			if (m_cGameOverResultData.CommitionDiscount)
			{
				strM.Append("(딜러비 할인 이벤트중)");
			}
		}

		//str.Format(g_StrMan.Get(_T("BET_WIN")), strM); // 하이 승자 아디 ## 하이승:%s(+%s)		
		str.Format("◈ 획득금액:%s \n", strM); // 하이 승자 아디 ## 하이승:%s(+%s)		
		AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_WINNER));

		str = "";
		str.Format("◈ 족보: %s \n","기권승");
		AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_JOKBO));

// 		str.Format(" ──────────────\n");
// 		AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT));

		for (i = 0 ; i < nMaxPlayer ; i ++)
		{
			if (strlen(m_cGameOverResultData.Ui[i].ID) > 0 && g_pGameView->m_cPlayerDrawManager.GetWinnerPNum() != i)
			{
				CString s;
				INT64 lostmoney = m_cGameOverResultData.Ui[i].nResultMoney;
				s = (CString)NMBASE::UTIL::g_MoneyMark(lostmoney);
				str.Format(" %s : ",m_cGameOverResultData.Ui[i].NickName);
				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_LOSER));
				s.Append(strChip_Name[g_RI.ChipKind]);
				str.Format("%s\n",s);
				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_LOSEMONEY));
			}
		}
	}
	else // (하이:1 로우:2 스윙:3  승자 1명 ) 승자가 2명: 4 (하이로  이겼는가 로우로 이겼는가 승자가 2명인가)
	{	
		CString str1;
		//str1.Format(" ----------------------------\n%s ----------------------------\n",g_StrMan.Get(_T("WIN_MESINFO")));
		str1.Format(" %s",g_StrMan.Get(_T("WIN_MESINFO")));
		str.Format(str1,m_cGameOverResultData.Ui[g_pGameView->m_cPlayerDrawManager.GetWinnerPNum()].NickName);
		
		AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT));

		INT64 nTotBat = GetRealBet();
		
		strM = (CString)NMBASE::UTIL::g_MoneyMark(m_cGameOverResultData.nWinMoney); // 하이 머니 

		strM.Append(strChip_Name[g_RI.ChipKind]);

		// 골드 경기장일 경우
		if (g_RI.ChipKind==1)
		{			
			// 딜러비 할인 이벤트 중일 경우 표시
			if (m_cGameOverResultData.CommitionDiscount)
			{
				strM.Append("(딜러비 할인 이벤트중)");
			}
		}

		//str.Format(g_StrMan.Get(_T("BET_WIN")), strM); // 하이 승자 아디 ## 하이승:%s(+%s)		
		str.Format("◈ 획득금액:%s \n", strM);
		AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_WINNER));

		str = "";
		str.Format("◈ 족보: %s \n",GetJokboName(wpnum));
		AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_JOKBO));

// 		str.Format(" ──────────────\n");
// 		AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT));

		int *rank = new int[nMaxPlayer];
		memset(rank,-1,sizeof(int) * nMaxPlayer);
		int rcnt = 4;
		for ( i = 0 ; i < nMaxPlayer ; i ++)
		{
			
			if (strlen(m_cGameOverResultData.Ui[i].ID) > 0 && g_pGameView->m_cPlayerDrawManager.GetWinnerPNum() != i){
				int p = GetPNum_ByServPN(i);
				int r = m_cGameOverResultData.Ui[i].nRank;
				
				if ( r > 0 && r <= nMaxPlayer )
					rank[r-1] = i;
				else if ( r == -1)
				{
					rank[rcnt] = i;
					rcnt --;
					if (rcnt < 0)rcnt =0;
				}
			}
		}

		for (i = 0 ; i < nMaxPlayer ; i ++)
		{
			if (rank[i] > -1 && rank[i] < nMaxPlayer){			
				if (strlen(m_cGameOverResultData.Ui[rank[i]].ID) > 0 && g_pGameView->m_cPlayerDrawManager.GetWinnerPNum() != rank[i]){
					int p = GetPNum_ByServPN(rank[i]);
					CString s;
					INT64 lostmoney = m_cGameOverResultData.Ui[rank[i]].nResultMoney;
					s = (CString)NMBASE::UTIL::g_MoneyMark(lostmoney);
					str.Format(" %s : ",m_cGameOverResultData.Ui[rank[i]].NickName);
					AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_LOSER));
					s.Append(strChip_Name[g_RI.ChipKind]);
					str.Format("%s\n",s);
					AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_LOSEMONEY));
				}
			}
		}

		delete [] rank;
	}

	DWORD dwStartTime = timeGetTime();
	g_pGameView->m_cPlayerDrawManager.SetPlayerGameOver(0, true);
	for ( i=0; i<nMaxPlayer; i++){

		if (GetPlayerNP(i)->JoinState == 1)
			g_pGameView->m_cPlayerDrawManager.SetPlayerGameOver(i, true);// 게임 종료 상태
	}

	////////////////////////////////////////////////////////////////////////////////////
	// 이벤트 일때 카드 활성화
	int nValue = 0;
    int WValue = GetWinnerValue(wpnum, nValue);
	
	// 카드 셀렉팅 (족보를 이룬 카드)
	GetResultCard(g_pGameView->m_cPlayerDrawManager.IsOpen(), wpnum);
	////////////////////////////////////////////////////////////////////////////////////	
	
	if (g_pGameView->m_cPlayerDrawManager.GetWinnerPNum() >-1)
	{		
		m_cAllBetChip.SetWinner(&g_pGameView->m_cSidePotMoney);
		
		for (int i = 0 ; i < nMaxPlayer ; i++)
		{
			if (GetPlayerNP(i)->JoinState == 1)
			{
				if (wpnum != i)
				{
					// 패배한 유저의 모든 카드를 회색으로
					//GetPlayerNP(i)->SetResultFace(0,TRUE);
					
					// 본인 카드 오픈 여부
					BOOL bFoldOpen = GetPlayerNP(i)->bFoldOpen;
					
					// 패배한 유저가 본인 카드를 오픈할 때
					// 족보에 해당하지 않는 카드 회색 표시
					if (bFoldOpen)
						GetResultCard(FALSE, i);
				}
			}
		}

		g_pGameView->m_cPlayerDrawManager.SetHiddenCardShow(true);
		//찬스아이템 노출관련 수정
		//if (g_pGameView->m_cPlayerDrawManager.IsOpen()) m_cResultFocus.SetRFocus(wpnum,WValue);
		m_cResultFocus.SetRFocus(wpnum,WValue);

		if (wpnum == 0)
		{
			if (CCardRule::N7C_TITLE <= WValue)
			{
				PBPlayEffectSound(SND_WIN_HIGH);
			}
			else
			{
				PBPlayEffectSound(SND_WIN_LOW);
			}
		}
		else
		{
			CPlayer *pPlayer = GAME()->GetPlayer(0);
			if ((GetPlayerNP(0)->GetNowMoney() < 100 && g_RI.ChipKind == 0)
				|| (GetPlayerNP(0)->GetNowMoney() < 1 && g_RI.ChipKind == 1))
			{
				PBPlayEffectSound(SND_LOSE_ALLIN);
			}
			else
			{
				PBPlayEffectSound(SND_LOSE_NORMAL);
			}
		}

		// 미션 + 족보 
// 		if (g_RI.bNightEvActive == 1)
// 		{
// 			if ( g_RI.CurRoomGoldenGaugeStep != m_cGameOverResultData.nCurRoomGoldenGaugeStep ){
// 				g_RI.CurRoomGoldenGaugeStep = m_cGameOverResultData.nCurRoomGoldenGaugeStep;
// 			}
// 		}
// 		//이번판은 골든벨 스테이지다
// 		else if ( g_RI.bNightEvActive == 2 )
// 		{
// 			g_RI.CurRoomGoldenGaugeStep = 0;
// 		}
		
		m_bEventDrawFlg = TRUE;
	}

	if ( !g_ObserverMan.IsMyObserver() && GetPlayerNP(0)->JoinState == 1 )
	{
		if ( g_RI.FormKind == JOKER_GAME && g_bGetJokerCard == TRUE ){
			int spnum = GetServPNum_ByPN(0);
			if (strlen(m_cGameOverResultData.Ui[spnum].ID)>0 && strcmp(m_cGameOverResultData.Ui[spnum].ID , GetPlayerNP(0)->UI.ID) == 0 ) //  겜참여자.
			{
				int value = m_cSvRule.get_MyRule(0);
				if ( value >= CCardRule::N7C_SFULSH ){
					if ( CheckComplateHaveJokerCard(0) ){
						//- 잭팟 당첨시
						CString wstr;
						wstr.Format("※조커 포함 족보 달성 시 잭팟이 지급되지 않습니다.\n");
						AddGameInfoViewEdit(wstr, GetChatColor(ECC2_NOTIFY));
					}
				}
			}
		}
	}
	
// 	if (RewardItemMan().m_RewardItemData.bValid == TRUE)
// 	{
// 		for ( int i = 0 ; i < RewardItemMan().m_RewardItemData.TotalCnt ; i ++ )
// 		{
// 					RewardItemMan().AllowBuyItem( RewardItemMan().m_RewardItemData.itemcode[i],
// 					RewardItemMan().m_RewardItemData.gamecode[i],
// 					RewardItemMan().m_RewardItemData.rctime[i],
// 					RewardItemMan().m_RewardItemData.expiretime[i]);
// 		}
// 		RewardItemMan().m_RewardItemData.bValid = FALSE;
// 	}
	
	if ( !g_ObserverMan.IsMyObserver() && GetPlayerNP(0)->m_nCardChoiceLimtCnt >= MAX_TURN_TIMEOUT_LIMIT_COUNT )
	{
		GetPlayerNP(0)->m_nCardChoiceLimtCnt = 0;
		ExitGameRequest(true);
		CString strOutput;
		strOutput.Format("게임 시작 후 %d회 이상 수동으로 카드 선택을 하지 않아 게임방에서 자동 퇴장 하였습니다.\n", MAX_TURN_TIMEOUT_LIMIT_COUNT);
		ShowMainMessageDlg(strOutput);
	}
}

void CGameProcess_NewPoker::SetJokerCard()
{
	if ( g_RI.FormKind != JOKER_GAME )return;

	if (g_RI.FormKind == JOKER_GAME && g_bGetJokerCard == TRUE)
	{					
		int spnum = GetServPNum_ByPN(0);
		
		if (GetPlayerNP(0)->m_listCard[6].GetCardNo() != g_JokerCardDeck.nJokerCardDeck[spnum]){
			m_dwStartDrawJokerTime = timeGetTime();
			PBPlayEffectSound(SND_JOKERCHANGE);
		}
		
		GetPlayerNP(0)->m_listCard[6].SetCardNo(g_JokerCardDeck.nJokerCardDeck[spnum]);
		GetPlayerNP(0)->m_listCard[6].SetJokerCard(TRUE);
	}
}

BOOL CGameProcess_NewPoker::CheckComplateHaveJokerCard(int wpnum)
{
	int wvalue = m_cSvRule.get_MyRule(wpnum);

	if (wvalue >= CCardRule::N7C_QUADS){				
		
		int arryindex = -1;
		
		arryindex = m_cSvRule.m_cRule[wpnum].m_SevenValue ; 	
		
		if ( arryindex <= -1 && arryindex >=14 )return FALSE;

		int total = 0;//1부터 시작 장수	
		
		int aCalCard[10];
		
		memset( aCalCard , -1 , sizeof(aCalCard) );
		
		for (int x = 0 ; x < 10 ; x ++){				
			if ( m_cSvRule.m_cRule[wpnum].m_AllRuleCard[arryindex][x] > -1 )
			{			
				if (m_cSvRule.m_cRule[wpnum].m_AllRuleCard[arryindex][x] != GetPlayerNP(wpnum)->GetCardNo(6) ){
					
					aCalCard[total] = m_cSvRule.m_cRule[wpnum].m_AllRuleCard[arryindex][x];
					total ++;
				}
			}
		}
		
		if ( total == 0 ) return TRUE;
		
		CSevenRuleMan rule;
		rule.set_total_Card(0, aCalCard, total );
		rule.get_cal_rule_user(0);
		
		int value = rule.IsSpecial_Rule(0);
		
		if ( value >= 1 )
		{	
			return FALSE;
		}

		/*
		if (m_cSvRule.IsRuleComplate(wpnum,GetPlayerNP(wpnum)->GetCardNo(6))){
			return TRUE;//조커카드를 포함한 상위 족보
		}
		*/
	}
	//조커카드를 포함하지 않은 상휘 족보
	return TRUE;
}

// 5 장의 카드를 리턴 한다.
void CGameProcess_NewPoker::GetResultCard(BOOL m_bOpen, int winner)
{
	int wvalue = m_cSvRule.get_MyRule(winner);
	BOOL bFoldOpen = GetPlayerNP(winner)->bFoldOpen;
	
	if ( (m_bOpen == FALSE && wvalue >= CCardRule::N7C_QUADS) || m_bOpen == TRUE
		|| bFoldOpen == TRUE )
	{
		for (int i = 0 ; i < GetPlayerNP(winner)->m_nTotalCardNum ; i++)
		{
			if (!m_cSvRule.IsRuleComplate(winner, GetPlayerNP(winner)->GetCardNo(i)))
			{
				GetPlayerNP(winner)->m_listCard[i].SetDark(TRUE);
			}
		}
	}
}

CString CGameProcess_NewPoker::GetShortname(int eName)
{
	CString strName = "";
	
	switch(eName)
	{
	case CCardRule::N7C_SROYAL:
	case CCardRule::N7C_DROYAL:
	case CCardRule::N7C_HROYAL:
	case CCardRule::N7C_CROYAL:
		{
			strName = "로티플";
		}break;
	case CCardRule::N7C_BSFLUSH:
		{
			strName = "빽스티플";
		}break;
	case CCardRule::N7C_SFULSH:
		{
			strName = "스티플";
		}break;
		
	case CCardRule::N7C_QUADS:
		{
			strName = "포카드";
		}break;
	case CCardRule::N7C_TITLE:
		{
			strName = "풀하우스";
		}break;
	case CCardRule::N7C_FLUSH:
		{
			strName = "플러쉬";
		}break;
	case CCardRule::N7C_MOUNT:		
	case CCardRule::N7C_BACKS:		
	case CCardRule::N7C_STRAIGHT:
		{
			strName = "스트레이트";
		}break;
		
	case CCardRule::N7C_TRIPLE:
		{
			strName = "트리플";
		}break;
	case CCardRule::N7C_TWOPAIR:
		{
			strName = "투페어";
		}break;
	case CCardRule::N7C_ONEPAIR:
		{
			strName = "원페어";
		}break;
	case CCardRule::N7C_NOPAIR:
		{
			strName = "노페어";
		}break;
	}
	
	return strName;
}

CPlayer *CGameProcess_NewPoker::GetPlayer(int i)
{
	return &m_pPlayer[i];
}

CPlayer_NP *CGameProcess_NewPoker::GetPlayerNP(int i)
{
	return (CPlayer_NP *)GetPlayer(i);
}


void  CGameProcess_NewPoker::SetJackPotMoney(INT64 i64Money)
{
	m_i64JackPotMoney = i64Money;
	g_cVipJackpot.SetShowJackpotMoney(m_i64JackPotMoney);
}


void  CGameProcess_NewPoker::SetOldJackPotMoney(INT64 i64Money)
{
	m_i64OldJackPotMoney = i64Money;
	g_cVipJackpot.SetShowOldJackpotMoney(m_i64JackPotMoney);
}

void CGameProcess_NewPoker::JackPotMoneyUpdate(jackpot::JP_MONEYINFO* pInfo)
{
	// 게임 타입
	IDX_GAME eGameType = (IDX_GAME)g_RI.nGameIndex;

	// 자신이 입장한 방의 최소 금액
	INT64 i64RoomMoney = g_RI.llEnterLimitMoney;

	if (i64RoomMoney <= 0)	///자유방 제외
		return;

	// 테스트 코드
	//	ROOMINFO sRoomInfo;
	//	memcpy(&sRoomInfo, &g_RI, sizeof(ROOMINFO));

	//뉴포커, 로우, 7포커
	ASSERT (eGameType > IDX_GAME_NULL);

	const int INDEX_ARRAY_JACKPOT_GAME_NEWPOKER = 2; //뉴포커 인덱스 2

	INT64 i64JackPotMoney = 0;
	
	for (int i = 0; i < jackpot::DEF_MONEYRANGE_NUM; ++i)
	{
		if (LIMITMONEY_INT_INT64(pInfo->stJPBase[ INDEX_ARRAY_JACKPOT_GAME_NEWPOKER ].nLimitMoney[i]) == i64RoomMoney)
		{
			i64JackPotMoney = pInfo->stJPBase[ INDEX_ARRAY_JACKPOT_GAME_NEWPOKER ].llJackPotMoney[ i ];//  llJackpotMoney[eGameType][i];  
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


void CGameProcess_NewPoker::ClearJackpotMoney(int nGameCode, INT64 i64LimitMoney)
{
	// 게임 타입
	IDX_GAME eGameType = IDX_GAME_NULL;

	switch(nGameCode)
	{
	case 42: eGameType = IDX_GAME_SP;
		break;

	case 54: eGameType = IDX_GAME_BD;
		break;

	case 61: eGameType = IDX_GAME_NP;
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
				g_JackPotMoneyInfo.stJPBase[ eGameType ].llJackPotMoney[ i ] = 0; // llJackpotMoney[eGameType][i] = 0;  
				break;
			}
		}
	}
}


//규제안 예약기능 추가로 인해 GameProcess에 추가된 가상 함수
void CGameProcess_NewPoker::SendCallButton()
{
	g_pGameView->OnBtnCall();
}

void CGameProcess_NewPoker::SendCheckButton()
{
	g_pGameView->OnBtnCheck();
}

void CGameProcess_NewPoker::SendDieButton()
{
	g_pGameView->OnBtnFold();
}

void CGameProcess_NewPoker::SendHalfButton()
{
	g_pGameView->OnBtnMax();
}

void CGameProcess_NewPoker::SendDdaButton()
{
	g_pGameView->OnBtnDda();
}

void CGameProcess_NewPoker::SendPingButton()
{

}