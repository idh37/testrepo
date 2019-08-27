// GameProcess7Poker.cpp: implementation of the CGameProcessSutda class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameProcessSutda.h"
#include "GlobalGame.h"
#include "SelectCardSutda.h"
#include "AutoBetSetUpWnd.h"
#include "CommMsgDef_Game.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int CARDMOVESPD = 30;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameProcessSutda::CGameProcessSutda(CGame *pGame)
: CGameProcess(pGame)
{
	m_pPage = NULL;
	m_bOnceInit = FALSE;
	m_i64JackPotMoney = 0;
	m_i64OldJackPotMoney = 0;
	Clear();
}

CGameProcessSutda::~CGameProcessSutda()
{
	Clear();
}


// 칩을 그리는 함수
void CGameProcessSutda::chip_draw_func(int PNum, INT64 Money)
{
	if(PNum < 0 || PNum >= MAX_PLAYER || Money < 0) 
		return;	

	if(g_RI.ChipKind == (int)ROOM_CHIPKIND_GOLD)
	{
 		m_AllBetChip.SetMoney(GetRealBet());
 		g_PLAYER(PNum)->OnSetChip(Money);
	}
	else
	{
		g_pGameView->m_cChipBoard.CastChip(PNum, Money, 0, g_PLAYER(PNum)->UI.GetMoney());
	}
}


// 최대 카드 개수
int  CGameProcessSutda::GetTotalCardNum()
{
	return 7;
}


// 마지막 베팅 금액
INT64 CGameProcessSutda::GetLastBetMoney(int nPlayerNum, int betCmd)
{
	INT64 Betmoney = 0;
	INT64 CallMoney = 0;
	INT64 DadangMoney = 0;
	INT64 HalfMoney	= 0;	

	Betmoney	= g_RI.CurSeedMoney;												// 삥 금액
	CallMoney	= g_Poker.RU[nPlayerNum].nReCall;									// 콜 금액
	DadangMoney = g_Poker.RU[nPlayerNum].nCalcBat + CallMoney;						// 따당 금액
	HalfMoney	= g_Poker.RU[nPlayerNum].nReCall + g_Poker.RU[nPlayerNum].nCalcBat;	// 하프 금액

	if(betCmd == RAISE_LAST_CMD_FOLD)
	{
		return  0;
	}	
	else if( betCmd == RAISE_LAST_CMD_CHECK)
	{
		return 0;
	}
	else if( betCmd == RAISE_LAST_CMD_CALL)		// 콜 금액
	{
		return CallMoney;
	}
	else if(betCmd == RAISE_LAST_CMD_PING)		// 삥 금액
	{
		return g_RI.CurSeedMoney;
	}
	else if(betCmd == RAISE_LAST_CMD_DADANG)	// 따당 금액
	{
		return DadangMoney;
	}
	else if( betCmd == RAISE_LAST_CMD_HALF || betCmd == RAISE_LAST_CMD_ALLIN ) // 하프 금액
	{
		return HalfMoney;
	}		

	return 0;
}


// ### [ Frame++ ] ###
void CGameProcessSutda::SetWindCardEnd(BOOL bFirst)
{	
	if(m_bWindCardEnd == FALSE)
	{		
		if( bFirst ) // 3장 3명 => 61초 1사람당 20초
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

// 경기중 베팅 머니
INT64 CGameProcessSutda::GetRaiseBat()
{
	INT64 nRaiseBat = 0;

	for(int i = 0; i < MAX_PLAYER; ++i)
	{
		nRaiseBat = max(nRaiseBat, g_Poker.RU[i].nNowBat);
	}

	return nRaiseBat;
}

// 실제 베팅 머니
INT64 CGameProcessSutda::GetRealBet()
{
	INT64 nRealBet = 0;

	for(int i = 0; i < MAX_PLAYER; ++i)
	{
		if(g_Poker.RU[i].bInUse) 
		{
			nRealBet+= g_Poker.RU[i].nRealBat;
		}
	}

	return nRealBet;
}

// 합계 베팅 머니
INT64 CGameProcessSutda::GetTotalBet()
{
	INT64 nRealBet = 0;

	for(int i = 0; i < MAX_PLAYER; ++i)
	{
		if(g_Poker.RU[i].bInUse)
		{
			nRealBet+= g_Poker.RU[i].nTotBat;
		}
	}

	return nRealBet;
}

// 최대 머니
INT64 CGameProcessSutda::GetMaxMoney(INT64 nTotal,int betkind )
{
	double fBatRate = 0.5;

	if( betkind == BET_CMD_HALF)
	{
		fBatRate = 0.5;
	}
	
	int spnum		 = GetServPNum_ByPN(0);
	INT64 callnMoney = GetRaiseBat() - g_Poker.RU[spnum].nNowBat;
	INT64 TotalMoney = GetTotalBet() + callnMoney;
	INT64 nCalcBat   = 0;

	nCalcBat = (INT64)(TotalMoney * fBatRate);
	nCalcBat += callnMoney;

	return nCalcBat;

}


// 종료시 안내 메세지 삽입 부분
void CGameProcessSutda::Insert_EndingTextInformation(CString str, COLORREF tcolor)
{
	if( m_GameEndMsgData.TextMsgCnt >= 30) 
		return;

	if( m_GameEndMsgData.TextMsgCnt < 0 )
		return;

	m_GameEndMsgData.TextMsg[m_GameEndMsgData.TextMsgCnt]   = str;
	m_GameEndMsgData.TextColor[m_GameEndMsgData.TextMsgCnt] = tcolor;	
	m_GameEndMsgData.TextMsgCnt++;	
}


// 채팅창에 정보 메시지를 출력한다.
void CGameProcessSutda::Draw_TextInformationMsg()
{
	if( m_GameEndMsgData.TextMsgCnt <= 0)
	{
		return;	
	}

	for( int i = 0 ; i < m_GameEndMsgData.TextMsgCnt ; ++i )
	{
		COLORREF tcolor = GetChatColor(ECC2_NOTIFY);

		if( m_GameEndMsgData.TextColor[i] != NULL )
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
void CGameProcessSutda::Clear()
{
	// [자동베팅] 2004.07.15
	//	m_IsAutoKindBet = 0; // 0:X  1:49 2:59 3:69
	//	m_bAutoBet = 0;		
	// 클라이언트 현재 상태
	m_nGameState = 0;				// 0 없음, 1, 2, 3, 4, 5, 6 승결정중 7 승결정(게임 오버)
	m_nNo1PosPNum = 0;
	m_nMyPosPNum = 0;		

	m_nDouble = 0;

	// 추방 기회 초기화 2회 끝 방장이 바뀌면 2회로 초기화
	g_cBanishMan.Clear();
	ResetFlg();

	
	m_bRematch = FALSE;	//재경기인가?
	
}

// 초기화
void CGameProcessSutda::Init(NMBASE::GRAPHICGDI::CPage *ppage)
{
	// 객체를 지우지 않기 때문에 한번만 호출이 되도록 한다.
	if(m_bOnceInit)
	{
		return;
	}		
	m_bOnceInit = TRUE;


	Clear();
	m_pPage = ppage;
	
	// 초대하기 대화상자 만들기
	//	m_InviteDlg.Create(IDD_INVITE_DIALOG, NULL);


	CGameProcess::Init(ppage);

	m_AllBetChip.Init(ppage, &g_cUIData, this, &g_sprChip);	

	int i = 0;
	for( i = 0 ; i < GetMaxNewPlayer() ; ++i )
	{
		m_cPlayer[i].Clear();
		m_cPlayer[i].InitRoomData();
		m_cPlayer[i].SetPlayerNum(i);
	}	

	//오픈애니메이션추가
	//m_CardOpenGuide.resize( MAX_CARDOPENGUIDE );
	//m_CardOpenGuide.clear();
	//플레이할 멀티씬 인덱스가 직접 들어간다.
	m_NowCardOpenGuideIndex = 0;
	for( i = 2; i < MAX_CARDOPENGUIDE + 2; i++ )
	{
		m_CardOpenGuide.push_back( i );
	}

	random_shuffle( m_CardOpenGuide.begin(), m_CardOpenGuide.end() );
}

	//오픈애니메이션추가
int CGameProcessSutda::GetCardOpneGuideIndex()
{
	
	if( g_Config.nbShowHidden == TRUE ) return -1;
	if( m_NowCardOpenGuideIndex < 0 || m_NowCardOpenGuideIndex >= MAX_CARDOPENGUIDE ) return -1;

	return m_CardOpenGuide[ m_NowCardOpenGuideIndex ];
}
 
//오픈애니메이션추가
void CGameProcessSutda::EndCardOpenGuide()
{ 
	if( m_bDrawCardOpenGuide )
	{
		m_bDrawCardOpenGuide = FALSE;
		m_NowCardOpenGuideIndex++; 

		//아이디당 한번씩만 나오게 한다.
		if( m_NowCardOpenGuideIndex >= MAX_CARDOPENGUIDE )
		{
			g_Config.nbShowHidden = TRUE;
			NMBASE::UTIL::WriteProfile_int( GM().GetMyInfo()->UI.ID, "SHOWHIDDEN",		g_Config.nbShowHidden );
		}
	}
}

void CGameProcessSutda::SetCardOpenGuideTime()
{
	m_bDrawCardOpenGuide = TRUE;
	m_dwCardOpenGuideTime = timeGetTime();
}

void CGameProcessSutda::ResetFlg()
{
	m_nGameCount = 0;
	m_bGameStart = FALSE;
	g_pGameView->m_cPlayerDrawManager.SetOpen(false);
	m_nCurPlayer = -1;	
	g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(false);

	//////////////////
	// 타이머
	//m_dwCurTime = 0;
	//g_pGameView->m_cPlayerDrawManager.ClearTimer();
	//g_pGameView->m_cPlayerDrawManager.SetCurTimerTime(0);
	// 상태
	m_nGameState = 0;		

	// [ 카드 이벤트 에러시 ]
	m_bWindCardEnd	= FALSE;
	m_nWindCardEndCnt = 0;
	m_nStartUserNum	= 0;

	m_bPlayEnding = 0;				// 엔딩 사운드 플레이
	m_nPlayEndingDelayTime = 0;		// 엔딩 사운드 플레이

	g_SelectCard().SetChoiceCard(false);

	//2006.1.4 종료시 안내 메세지 그리는 부분
	m_GameEndMsgData.Clear();
	m_AfterEndingUserInfo.Clear();
	// 결과 구조체
	ZeroMemory(&m_GameOverResultData,sizeof(m_GameOverResultData));	
	m_WinnerPNumData.Clear();	

	m_BetCount = 0;
	m_nHinddenArrow_x = 0;
	m_nHinddenArrow_y = 0;
	//m_bHiddenCardShow = 0;
	g_pGameView->m_cPlayerDrawManager.SetHiddenCardShow(false);
	m_MouseHiddenState = 0;
	m_bMouseHiddenClick = FALSE;
	m_bkeyboardHiddenClick = FALSE;
	m_HiddenClickPos.x = 0;
	m_HiddenClickPos.y = 0;
	m_dwHiddenSceneDrawTime = 0;
	
		//관전자 채팅 경고 
	m_bObserverChatWarning = FALSE;

	//결과출력 Delay
	m_nResultDelayCount = 0;
	m_bResultDelayStart = FALSE;



	//섯다용
	
	m_HighJokboPoint = 0;	
	m_NowWinpnum=0;
	
}

// 게임 데이터를 초기화 한다.
void CGameProcessSutda::ResetGame()
{
	ResetFlg();
	g_RI.State = EGAME_WAITING;		

	GM().GetMyInfo()->JoinState = EGAME_JOINOUT;
	GM().GetMyInfo()->PlayState = EPLAYER_GAMEEND;		

	g_Poker.nBossPNum = -1;

	//animan moment destroy
	g_pGameView->m_cAniSprListManager.Destroy(0);
	g_pGameView->m_cPlayerDrawManager.Reset();
	g_pGameView->m_cGameViewBtnMananager.ResetGame();
	
	// 플레이어 리셋
	for(int i = 0; i < GetMaxNewPlayer(); ++i) 
	{
		g_PLAYER(i)->Reset();
	}

	g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();
	g_pGameView->m_cPlayerDrawManager.SetTurn(-1);
	g_pGameView->m_cTotalBetMoney.Reset();
	
	if(g_RI.ChipKind == (int)ROOM_CHIPKIND_GOLD)
	{
		m_AllBetChip.Reset();
	}

	m_nActiveBetBtnDelayTime = 0;
	g_SelectCard().Reset();	
	m_nBetCallMoneyClearCnt = 0;	//콜한금액들 지워주기

	g_pGameView->m_cGameViewBtnMananager.SetBetBtnType(g_Config.eBetBtnType);

	g_Poker.nImageBoss = -1;	

	//섯다 애니메이션
	g_pGameView->m_AniQue.Reset();

	

	SutdaGameSound().OnResetGame();

	m_nNotifyResultCardOpenAnimationEndDelay = 0;


	g_pGameView->m_cAskRematchPopup.Hide();


	//재대결이 아니면 바닥의 칩을 클리어시킨다.
	if (!IsRematchGame())
		g_pGameView->m_cChipBoard.Reset();

	//규제안 : 예약버튼 활성화 가능상태
	ReserveState(RESERVESTATE_BET_NONE);
	CanReserve(true);
	g_nCardTotalCount = 0 ; // 카드 세팅 비교값을 0으로 초기화.
}


// 모든 프로세스는 이곳에서 한다.
void CGameProcessSutda::OnTimer()
{
	m_nGameCount++;

	// [ 카드 날리기 ]
	if(g_RI.State != EGAME_WAITING)		// (0:대기중 1:게임중)
	{
		// 카드 날아가는것이 끝났다.
		if( m_bWindCardEnd )	
		{
			m_nWindCardEndCnt--;
			if(m_nWindCardEndCnt <= 0)
			{
				m_bWindCardEnd = FALSE;

				// 카드 날아가는것이 끝났다는 메세지 보낸다.(클라 -> 서버)
				if( !g_ObserverMan.IsMyObserver() && !g_PLAYER(0)->bFold && 
					g_PLAYER(0)->JoinState == EGAME_JOININ && 
					g_PLAYER(0)->PlayState == EPLAYER_GAMEPLAYING)
				{
					CCL_WINDCARDEND msg2;					
					msg2.Set(g_RI.RoomNum, GM().GetMyInfo()->ServPNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID);
					NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg2.pData, msg2.GetTotalSize());
				}
			}
		}		

		if( m_nActiveBetBtnDelayTime > 0 )
		{
			m_nActiveBetBtnDelayTime--;
			if(m_nActiveBetBtnDelayTime <= 0 )
			{				
				m_nActiveBetBtnDelayTime = 0;
				g_pGameView->m_cGameViewBtnMananager.SetButtonClick(false);
				g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(false);				
				g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable(false);		 		
				AutoBetSetUpWnd().Set_auto_betting_flag();
				//	Set_auto_betting_flag();
			}
		}

		//혹시 몰라 한번 더 검사
		if( m_nActiveBetBtnDelayTime < 0 )
		{
			m_nActiveBetBtnDelayTime = 0;
			g_pGameView->m_cGameViewBtnMananager.SetButtonClick(false);
			g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(false);			
			g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable(false);			
			AutoBetSetUpWnd().Set_auto_betting_flag();
			//Set_auto_betting_flag();
		}

		if( m_nBetCallMoneyClearCnt > 0 )
		{
			m_nBetCallMoneyClearCnt--;
			if( m_nBetCallMoneyClearCnt == 0 )
			{
				m_nBetCallMoneyClearCnt = 0;

				//배팅 금액 표시 한것들 지운다.
				/*for(int i = 0 ; i < MAX_PLAYER ; ++i )
				{
					g_PLAYER(i)->OnReSetBetDraw();
				}*/
			}
		}

		// 자동 베팅 - 카드 날리는 중에는 그리지 않는다. 자동 배팅 설정일때 턴 제한시간 다 되어가면 콜로 보낸다.
		OnCheckTurnLimitTime();		
		AutoBetSetUpWnd().OnTimer();
	}

	// 총 배팅금액 세팅	
	for(int i = 0 ; i < MAX_PLAYER; ++i)
	{
		g_PLAYER(i)->OnTimer();
	}

	if(g_RI.ChipKind == (int)ROOM_CHIPKIND_GOLD)
	{
		m_AllBetChip.OnTimer();
	}

	// 결과 출력에 대한 Delay
	if(m_bResultDelayStart)
	{
		if(m_nResultDelayCount > 0)
		{
			m_nResultDelayCount--;
			if(m_nResultDelayCount <= 0 )
			{				
				OnResultGameOver();
				m_nResultDelayCount = 0;
				m_bResultDelayStart = FALSE;
			}
		}		
	}

	
	
	
	//섯다용
	//섯다 축음기
	g_pGameView->m_Radiophonograph.OnTimer();	
	
	//섯다 애니메이션
	g_pGameView->m_AniQue.Update();

	g_SelectCard().OnTimer();

	//섯다 재대결 조인여부 결정창
	g_pGameView->m_cAskRematchPopup.OnTimer();


	g_pGameView->m_cChipBoard.OnTimer();

	//일정 시간 뒤에 카드연출이 끝났다고 서버에 알려준다.
	if (m_nNotifyResultCardOpenAnimationEndDelay>0)
	{
		m_nNotifyResultCardOpenAnimationEndDelay--;
		if (m_nNotifyResultCardOpenAnimationEndDelay==0)
		{
			CCL_SD_RESULTCARD msg;
			msg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GetServPNum_ByPN(0), GM().GetMyInfo()->UI.ID);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
	}
}

BOOL CGameProcessSutda::OnMouseMove(int x, int y)
{
	g_SelectCard().OnMouseMove(x,y);

	for( int i = 0; i < MAX_PLAYER ; ++i)
	{			
		g_PLAYER(i)->OnMouseMove(x,y);
	}

	return FALSE;
}

BOOL CGameProcessSutda::OnLButtonDblclk(int x, int y)
{
	return FALSE;
}

BOOL CGameProcessSutda::OnLButtonUp(int x, int y)
{
	g_SelectCard().OnLButtonUp(x,y);
	for( int i= 0 ; i < MAX_PLAYER ; i++ )
	{
		g_PLAYER(i)->OnLButtonUp(x,y);
	}
	
	return FALSE;
}

BOOL CGameProcessSutda::OnLButtonDown(int x, int y)
{

	g_SelectCard().OnLButtonDown(x,y);
	m_cPlayer[0].OnLButtonDown(x,y);

	return FALSE;
}

void CGameProcessSutda::OnGameKey(int nVKey, int nextended, int repeat)	
{	

#ifdef _DEBUG	
	if( /*VK_F2*/60 == nVKey )
	{

		
		// 		CPromotionPokerTime *pPokerTime = (CPromotionPokerTime *)g_cPromotionMan.MakePromotion(CPromotionManager::EPT_POKERTIME);
		// 		pPokerTime->ShowAniEffect(0, "스트레이트", "50억");
		// 		pPokerTime->ShowAniEffect(1, "스트레이트", "50억");
		// 		pPokerTime->ShowAniEffect(2, "스트레이트", "50억");
		// 		pPokerTime->ShowAniEffect(3, "스트레이트", "50억");
		// 		pPokerTime->ShowAniEffect(4, "스트레이트", "50억");

	}
	if ( nVKey == rule::SKEY_F7 )
	{
// 		CMadeEffect *pObj = NULL;
// 		SAFE_NEW( pObj, CMadeEffect );
// 		pObj->Init(4);
// 		g_pGameView->m_cAniSprListManager.ADDObject(pObj);
		//g_pGameView->m_AniQue.Ani_Insert(ANIMATION_RESULT_EFT, CPoint(0,0), 0, 0, 32, FALSE, NULL, NULL, 0 + 10);

// 		if (IsHiddenRound())
// 		{
// 			ShowMainMessageDlg("마지막라운드");
// 		}

	//	ShowMainMessageDlg("일 10시간 이용시간을 모두 이용하여, 게임참여가 불가능합니다. 게임 과몰입 방지를 위해 총 이용시간을 총 10시간으로 제한하고 있는 점 양해 부탁 드립니다.\n(일 구분기준 : 오전6시)");
		//SutdaGameSound().OnActionVoice( AV_DRAW_WIN, 2, 2 );
		SutdaGameSound().OnActionVoice( AV_SUPERBANISH, 1); 
	}

	if ( nVKey == rule::SKEY_F8 )
	{

		SutdaGameSound().OnBetVoice(1, 0, RAISE_LAST_CMD_FOLD, FALSE );
		
	}

#endif

	//채팅창변경
	if ( nVKey == rule::SKEY_TAB ) 
	{			
		CChattingWindowMan::ENUM_CHATTING_TAB_TYPE nViewTab = g_pChatWinMan->GetViewMode();
		if(nViewTab == CChattingWindowMan::ECTT_CHAT)
			nViewTab = CChattingWindowMan::ECTT_EVENT;
		else
			nViewTab = CChattingWindowMan::ECTT_CHAT;

		g_pChatWinMan->SetViewPage(nViewTab);	
		return;
	}

//	g_pGameView->m_cAskRematchPopup.OnGameKey(nVKey, nextended, repeat);

	m_cPlayer[0].OnGameKey(nVKey, nextended, repeat);	
}

void CGameProcessSutda::OnCheckTurnLimitTimeBase()
{
	if(g_RI.State == EGAME_WAITING)
		return;

	if(g_pGameView->m_cPlayerDrawManager.GetStartTimerTime() <= 0)
		return;

	g_pGameView->m_cPlayerDrawManager.SetCurTimerTime(timeGetTime());

	if(g_Poker.nState == RSTATE_STARTGAME )
	{
		// 카드 날리는 중에는 그리지 않는다.
		if( g_PLAYER(0)->PlayState == EPLAYER_GAMEPLAYING && m_nCurPlayer == g_PLAYER(0)->ServPNum && !g_pGameView->m_cPlayerDrawManager.IsFlyWindCard() && !g_PLAYER(0)->bFold ) 
		{	
			int tcnt = g_pGameView->m_cPlayerDrawManager.GetTimerTimeGab() / 1000;

			if(tcnt > PBTurnTimeOut(ETTT_BETTING))
			{			
				if( g_PLAYER(0)->GetNowMoney() > 0 )
				{
					if( g_pGameView->m_cGameViewBtnMananager.m_bFold && false == g_pGameView->m_cGameViewBtnMananager.GetButtonClick())
					{
// 						CString szTemp;
// 						szTemp.Format("타임오버 => ST:%u CT:%u GT:%u \n", g_pGameView->m_cPlayerDrawManager.GetStartTimerTime(), g_pGameView->m_cPlayerDrawManager.GetCurTimerTime(), g_pGameView->m_cPlayerDrawManager.GetTimerTimeGab());						
// 						AddGameChatViewEdit(szTemp.GetBuffer(), GetChatColor(ECC2_RESULT_JOKBO));

						g_PLAYER(0)->bFold = TRUE;
						GM().GetMyInfo()->bFold = TRUE;
						SendDieButton( false );						
					}
				}
				//올인 베팅을 하고 있는 유저 체크,콜
				else
				{					
					if( g_pGameView->m_cGameViewBtnMananager.m_bBCall )
					{
						SendCallButton( false );
					}
					else if( g_pGameView->m_cGameViewBtnMananager.m_bBCheck )
					{
						SendCheckButton( false );
					}										
				}

#ifndef _DEBUG
				// 타임 아웃 카운트 증가
				if (!g_ObserverMan.IsMyObserver())
					m_cPlayer[ 0 ].IncTurnLimitCnt();
#endif

			}
		}
	}	
}


// 카드 애니가 끝났을때 클라에서 보낸값을 다시 서버에서 보내준다.
void CGameProcessSutda::EndWindCard()
{
	g_pGameView->m_cPlayerDrawManager.SetStartTimer();	
	
	if( m_BetCount == -1 )
	{
		m_BetCount = 0;
	}	

	// 전광판 출력
	if(g_Poker.nAutoBetCmd > 0)
	{
		g_pGameView->m_cDisplayBoard.SetText(EDBT_AUTOBETTING);
	}
	else
	{

		if (g_Poker.nRound==0)	//첫번째 라운드
		{
			g_pGameView->m_cDisplayBoard.SetText(EDBT_FIRST_BETTING);

			//초구(첫번째장)가 일, 삼. 팔광 일경우 사운드효과를 준다.
			
			if(!g_ObserverMan.IsMyObserver() && m_cPlayer[0].DoesHaveGoodCard())
				SutdaGameSound().OnActionVoice( AV_FIRSTBET, 0, 3 );
		}
		else	//히든라운드
		{
			g_pGameView->m_cDisplayBoard.SetText(EDBT_FINAL_BETTING);
			
			//사운드작업(히든카드받았을때)
			if(!g_ObserverMan.IsMyObserver())
				SutdaGameSound().OnActionVoice( AV_GETHIDDEN, 0, 4);
		}
	}


	// 베팅시작 (카드 다 날리고 베팅 시작)
	// 타이머 보이기

	g_pGameView->m_cPlayerDrawManager.SetStartTimer();

	m_nCurPlayer = g_Poker.CurPlayer;
	int nPlayNum = GetPNum_ByServPN(m_nCurPlayer);
	g_pGameView->m_cPlayerDrawManager.SetTurn(nPlayNum);

	//int nr = g_Poker.nRound;
	if(	g_Poker.nRound != 0 )	//첫번째 라운드의 ENDWIND에서는 SV_RAISE신호가 바로 뒤따라 오기 때문에 버튼활성화하지 않는다.
	{
		if(!g_ObserverMan.IsMyObserver() && m_cPlayer[0].ServPNum == m_nCurPlayer && m_cPlayer[0].bFold==FALSE)
		{			
			g_pGameView->m_cGameViewBtnMananager.SetShowBetMony(); //< CYS 100705 > 교체
			g_pGameView->m_cGameViewBtnMananager.SetButtonClick(false);
			m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelay(0,0);
		}

		// 규제안 예약 베팅 추가.(방장이 아닌 유저는 버튼 활성화가 안옴)
		if(!g_ObserverMan.IsMyObserver() && g_PLAYER(0)->ServPNum != m_nCurPlayer && m_cPlayer[0].bFold==FALSE)
		{
			m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelay(0,0);
		}
		//규제안 예약베팅 셋팅 초기화
		ReserveState(RESERVESTATE_BET_NONE);
		//규제안 :  예약 가능여부 설정
		if(CheckReceivedAllCards())
		{
			CanReserve(true);
		}
	}	

	g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(false);

	if (!IsHiddenRound())				
		g_pGameView->m_cPlayerDrawManager.MyJokboNameUpdate();// 자신의 카드위에 족보명을 보위기 위함
}

//////////////////////////////////////////////////////////////////////////
/*
메세지 처리 
*/
//////////////////////////////////////////////////////////////////////////

// 방만들기 허가
bool CGameProcessSutda::Accept_CreateRoom(void *pMsgParam) 
{
	// 스프라이트 로딩 후 초기화 ~
	if(GetCurrentWhere() != IDX_GAMEWHERE_LOBY) 
		return false;

	CSV_ACCEPT_CREATEROOM *pMsg = (CSV_ACCEPT_CREATEROOM*)pMsgParam;//UGPCommon 작업

	GM().SetWaitResponse(false);
	SetAccessRoomState(false);

	memcpy(&g_RI, pMsg->RI, sizeof(ROOMINFO));
	g_pGameView->m_cTitle.MakeTitle();

	ZeroMemory(&g_Poker, sizeof(POKERGAME));

	g_PLAYER(0)->SetNewPlayer(&GM().GetMyInfo()->UI);

// 	if(g_pGameView)
// 	{
// 		g_pGameView->m_cGameHelper.OnEnterRoom();
// 	}
	
	// 게임 정보 변경 해주기 
	g_MyGameInfo.nGameMode = g_RI.nGameIndex;
	
	// 원샷 충전
	g_cOneShotCharge.OnEnterRoom();

	// 초기화
	SetRematchGame(false);
	ResetGame();	
	
	GM().SetWaitResponse(FALSE);
	
	// 게임 정보 구조체를 초기화 한다
	for(int i = 1; i < MAX_PLAYER; ++i) 
	{
		g_PLAYER(i)->Clear();
	}

	g_PLAYER(0)->JoinState = EGAME_JOINOUT;

	// ### [ 중복된 IP가 ] ###
	g_PLAYER(0)->MyIpCheck = -1;	

	m_nNo1PosPNum = 0;
	m_nMyPosPNum = 0;	

	// 서버측 플레이어 번호 지정
	for(int i = 0; i < MAX_PLAYER; ++i)	
	{
		g_PLAYER(i)->ServPNum = i; 
	}

	memcpy(&g_PLAYER(0)->UI,&GM().GetMyInfo()->UI,sizeof(GM().GetMyInfo()->UI));

	//Buy in 맥스방 UI 머니셋팅 방만들기
	if( g_RI.bIsMaxRoom )
	{
		//설정머니보다 자산이 많을경우 설정 머니로 셋팅한다.
		if( g_RI.ChipKind==0 && g_RI.llInMoney > g_PLAYER(0)->UI.GetMoney() )
		{
			g_PLAYER(0)->UI.SetInMoney( g_PLAYER(0)->UI.GetMoney() );
			GM().GetMyInfo()->UI.SetInMoney( g_PLAYER(0)->UI.GetMoney() );
		}
		else if( g_RI.ChipKind==1 && g_RI.llInMoney > GM().GetMyGold() )
		{
			g_PLAYER(0)->UI.SetInMoney( GM().GetMyGold()  );
			GM().GetMyInfo()->UI.SetInMoney( GM().GetMyGold()  );
		}
		else
		{
			g_PLAYER(0)->UI.SetInMoney( g_RI.llInMoney );
			GM().GetMyInfo()->UI.SetInMoney( g_RI.llInMoney );
		}
	}

	GM().GetMyInfo()->JoinState = EGAME_JOINOUT;
	GM().GetMyInfo()->MyIpCheck = -1;
	GM().GetMyInfo()->ServPNum  = g_PLAYER(0)->ServPNum;

	g_MyPN = g_MastPN = 0;	// 내가 방장

	g_pGameView->m_cPlayerDrawManager.OnEnterRoom();

	m_nCurPlayer = 0;// 방처음 개설자가 선

	// 캐릭터창 초기화
	if(g_RI.MaxUserNum > MAX_PLAYER) 
		g_RI.MaxUserNum = MAX_PLAYER;
	
	if(g_RI.MaxUserNum < 2) 
		g_RI.MaxUserNum = 2;

	//버튼들 초기화
	g_pGameView->m_cGameViewBtnMananager.EnterRoom_Button_Init( g_RI.sBetRuleKind );

	if(!g_ObserverMan.IsMyObserver())
	{
		AutoBetSetUpWnd().OnEnterRoom();
	}

	g_cBanishMan.Clear();

	g_cPromotionMan.Accept_CreateRoom();

	// 규제안 입장 유저의 손실머니 정보 추가.
	INT64 lossMoney = GM().GetLossMaxMoney() + GM().GetLossMoney();
	g_pGameView->m_cPlayerDrawManager.OnEnterRoomLimitMoney(0, lossMoney);

	//규제안 : 방입장시에 손실금액 경고 문구 상태 체크
	g_pGameView->m_cGameViewBtnMananager.SetLossMoneyWarning(true) ;

	//규제안 : 대기상태의 방정보를 설정한다.
	g_pGameView->m_cGameViewBtnMananager.m_bIsWaitUser = false;

	return true;
}

 
// 방에 입장 허가 받음
bool CGameProcessSutda::Accept_EnterRoom(void  *pMsgParam) 
{
	CSV_ACCEPT_ENTERROOM* pMsg = (CSV_ACCEPT_ENTERROOM*) pMsgParam;

	if( pMsg == NULL )
	{
		CSV_ASK_OUTROOM aormsg;
		aormsg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, REASON_OUTROOM_ABNORMAL);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), aormsg.pData, aormsg.GetTotalSize());
		// 연결종료
		g_MastPN = 0;
		CloseMainMessageDlg();
		return false;
	}

	SetRematchGame(false);
	ResetGame(); 
	

	g_MyPN = 0;
	int MyID = -1;
	int MasterID = -1;

	g_RI.nGameIndex			= pMsg->EI->Ri.nGameIndex;
	g_RI.FormKind			= pMsg->EI->Ri.FormKind; // 1 : CHOICE_GAME	
	g_RI.ChipKind			= pMsg->EI->Ri.ChipKind;
	g_MyGameInfo.nGameMode	= pMsg->EI->Ri.nGameIndex;

	//규제안 : 대기상태의 방정보를 설정한다.
	g_pGameView->m_cGameViewBtnMananager.m_bIsWaitUser = pMsg->EI->Ri.bWaitInRoom=='1';

	//규제안 : 방입장시에 손실금액 경고 문구 상태 체크
	g_pGameView->m_cGameViewBtnMananager.SetLossMoneyWarning(true) ;

	for(int i = 0; i < GetMaxNewPlayer(); ++i) 
	{
		if(strlen(pMsg->EI->Ui[i].ID) > 0) 
		{
			// 방장 아이디 찾기
			if(strcmp(pMsg->EI->Ri.ID, pMsg->EI->Ui[i].ID) == 0)
			{
				MasterID = i; 
			}

			// 나의 아이디
			if(strcmp(GM().GetMyInfo()->UI.ID, pMsg->EI->Ui[i].ID) == 0)
			{
				MyID = i; 
			}
		}
	}

	// 방장 혹은 나의 아이디를 찾을 수 없거나 방장과 내 아이디가 동일하다?
	if(MasterID == -1 || MyID == -1 || MasterID == MyID) 
	{
		CSV_ASK_OUTROOM aormsg;
		aormsg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, REASON_OUTROOM_ABNORMAL);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), aormsg.pData, aormsg.GetTotalSize());
		// 연결종료
		g_MastPN = 0;
		//g_pLobyDlg->m_MsgBoxDlg.OnCancel();
		return false;
	}

	g_PLAYER(0)->SetNewPlayer(&GM().GetMyInfo()->UI);

	//맥스방 UI 머니셋팅 입장시
	if( pMsg->EI->Ri.bIsMaxRoom )
	{
		INT64 gold = GM().GetMyGold();
		int kind = g_RI.ChipKind;
		//설정머니보다 자산이 많을경우 설정 머니로 셋팅한다.
		if( g_RI.ChipKind==0 && pMsg->EI->Ri.llInMoney > g_PLAYER(0)->UI.GetMoney() )
		{
			g_PLAYER(0)->UI.SetInMoney( g_PLAYER(0)->UI.GetMoney() );
			GM().GetMyInfo()->UI.SetInMoney( g_PLAYER(0)->UI.GetMoney() );
		}
		else if( g_RI.ChipKind==1 && pMsg->EI->Ri.llInMoney > GM().GetMyGold() )
		{
			g_PLAYER(0)->UI.SetInMoney( GM().GetMyGold()  );
			GM().GetMyInfo()->UI.SetInMoney( GM().GetMyGold()  );
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
	
	if(g_pGameView)
	{
		g_pGameView->m_cPlayerDrawManager.OnEnterRoom();	// 플레이어정보
	//	g_pGameView->m_cGameHelper.OnEnterRoom();			// 족보뷰어
	}
	
	// 원샷 충전
	g_cOneShotCharge.OnEnterRoom();

	if(pMsg->EI->Ui[MyID].cObserver == 1)  
	{ 
		g_ObserverMan.SetMyObserver(TRUE); 
	}

	GM().GetMyInfo()->SetChangeRoomInfo(pMsg->EI->Cui[MyID]);
	
	g_MastPN = MasterID;	

	// 방 정보 복사
	memcpy(&g_RI, &pMsg->EI->Ri, sizeof(ROOMINFO));	
	if(g_RI.State == 1)
	{
		g_pGameView->m_cTotalBetMoney.Start();
	}
	g_pGameView->m_cTitle.MakeTitle();

	if(g_RI.MaxUserNum > MAX_PLAYER) 
	{
		g_RI.MaxUserNum = MAX_PLAYER;
	}

	if(g_RI.MaxUserNum < 2) 
	{
		g_RI.MaxUserNum = 2;//2,3,4,5
	}

	ZeroMemory(&g_Poker,sizeof(g_Poker));
	memcpy(&g_Poker, &pMsg->EI->Pg,sizeof(g_Poker));

	
	// 관전이 아닌 상태로 입장
	if( !g_ObserverMan.IsMyObserver() ) 
	{
		g_PLAYER(0)->JoinState		= EGAME_JOINOUT;
		GM().GetMyInfo()->JoinState = EGAME_JOINOUT;
		m_nMyPosPNum				= MyID;					// 서버쪽에 인덱스
		m_nNo1PosPNum				= GetPNum_ByServPN(0);	// 서버쪽의 0 번 인덱스		

		// 게임 정보 초기화
		for(int i = 1; i < GetMaxNewPlayer(); ++i) 
		{
			g_PLAYER(i)->Clear();
		}

		// 서버측 플레이어 번호 지정
		for(int i = 0; i < GetMaxNewPlayer(); ++i)
		{
			if( i < MAX_PLAYER)
			{
				g_PLAYER(i)->ServPNum = GetServPNum_ByPN(i);
			}
			else
			{
				g_PLAYER(i)->ServPNum = i;
			}
		}

		GM().GetMyInfo()->ServPNum = g_PLAYER(0)->ServPNum;	

		// 플레이어 순서에 맞게 초기화한다
		for(int i = 0; i < GetMaxNewPlayer(); ++i) 
		{
			if(i < MAX_PLAYER)
			{
				int nPlayerNum = GetPNum_ByServPN(i);

				if(strlen(pMsg->EI->Ui[i].ID) > 0)
				{
					if(nPlayerNum != 0) 
					{
						g_PLAYER(nPlayerNum)->SetNewPlayer(&pMsg->EI->Ui[i]);	
					}

					g_PLAYER(nPlayerNum)->JoinState  = pMsg->EI->Pg.PS[i].JoinState;
					g_PLAYER(nPlayerNum)->PlayState  = pMsg->EI->Pg.PS[i].PlayState;
					g_PLAYER(nPlayerNum)->MyIpCheck  = pMsg->EI->Pg.PS[i].nCheckIp;		// ### [ 중복된 IP가 ] ###
					g_PLAYER(nPlayerNum)->nSndFxKind = pMsg->EI->Pg.PS[i].nSndFxKind;	// ### [사운드 추가 작업] ###
					g_PLAYER(nPlayerNum)->UI.nIcon   = GetMoneyGrade(pMsg->EI->Ui[i].GetTotalMoney());
					g_PLAYER(nPlayerNum)->SetChangeRoomInfo(pMsg->EI->Cui[i]);

				/*	char szTemp1[256] = {0, };
					sprintf(szTemp1, "------ Accept_EnterRoom(플레이어) :: PlayerIndex: %d JoinState:%d PlayState:%d \n", nPlayerNum, pMsg->EI->Pg.PS[i].JoinState, pMsg->EI->Pg.PS[i].PlayState);
					OutputDebugString(szTemp1);*/
				}
			}	
			else if(i >= MAX_PLAYER) 
			{
				//관전자 아디셋팅
				if(strlen(pMsg->EI->Ui[i].ID) > 0)
				{
					g_PLAYER(i)->SetNewPlayer(&pMsg->EI->Ui[i]);
					if(pMsg->EI->Ui[i].AdminGrade == 0) 
					{
						// 관전자 리스트 출력
						int ounum   = g_PLAYER(i)->UI.UNum;
						int sex     = g_PLAYER(i)->UI.Sex;
						int level   = g_PLAYER(i)->UI.nIcon;
						INT64 Money = g_PLAYER(i)->GetNowMoney();
						int seq		= 0;

						CObserverDlg *pObserverDlg = GetObserverDlg();
						if(NULL != pObserverDlg)
						{
							pObserverDlg->User_InsItem(ounum, g_PLAYER(i)->UI.ID, g_PLAYER(i)->UI.NickName, sex, level, Money, seq);
						}
					}
				}				
			}					
		}

		// 버튼 상태 설정
		m_nCurPlayer = g_Poker.CurPlayer;
		g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();		
	}
	//관전자로 입장
	else 
	{
		// 서버 클라이언트 1 : 1		
		m_nMyPosPNum  = 0;					 // 서버쪽에 인덱스
		m_nNo1PosPNum = GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스

		for(int i = 0; i < GetMaxNewPlayer(); ++i) 
		{
			g_PLAYER(i)->Clear(); // 게임 정보 초기화
			g_PLAYER(i)->ServPNum = i;
		}

		GM().GetMyInfo()->ServPNum = MyID;

		// 플레이어 순서에 맞게 초기화한다
		for(int i = 0; i < GetMaxNewPlayer(); ++i)
		{		
			int nPlayerNum = i;

			if(strlen(pMsg->EI->Ui[i].ID) > 0)	
			{
				g_PLAYER(nPlayerNum)->SetNewPlayer(&pMsg->EI->Ui[i]);

				// 관전자 리스트 출력
				if(i < MAX_PLAYER)
				{
					g_PLAYER(nPlayerNum)->JoinState  = pMsg->EI->Pg.PS[i].JoinState;
					g_PLAYER(nPlayerNum)->PlayState  = pMsg->EI->Pg.PS[i].PlayState;
					g_PLAYER(nPlayerNum)->MyIpCheck	 = pMsg->EI->Pg.PS[i].nCheckIp;		// ### [ 중복된 IP가 ] ###
					g_PLAYER(nPlayerNum)->nSndFxKind = pMsg->EI->Pg.PS[i].nSndFxKind;	// ### [사운드 추가 작업] ###
					g_PLAYER(nPlayerNum)->UI.nIcon	 = GetMoneyGrade(pMsg->EI->Ui[i].GetTotalMoney());
					g_PLAYER(nPlayerNum)->SetChangeRoomInfo(pMsg->EI->Cui[i]);

					/*char szTemp1[256] = {0, };
					sprintf(szTemp1, "------ Accept_EnterRoom(관전) :: PlayerIndex: %d JoinState:%d PlayState:%d \n", nPlayerNum, pMsg->EI->Pg.PS[i].JoinState, pMsg->EI->Pg.PS[i].PlayState);
					OutputDebugString(szTemp1); */
				}
				else if(i >= MAX_PLAYER) 
				{
					if(pMsg->EI->Ui[i].AdminGrade == 0) 
					{
						int ounum	= g_PLAYER(i)->UI.UNum;
						int sex		= g_PLAYER(i)->UI.Sex;
						int level	= g_PLAYER(i)->UI.nIcon;
						INT64 Money = g_PLAYER(i)->GetNowMoney();
						int seq		= 0;

						CObserverDlg *pObserverDlg = GetObserverDlg();
						if(NULL != pObserverDlg)
						{
							pObserverDlg->User_InsItem(ounum,g_PLAYER(i)->UI.ID, g_PLAYER(i)->UI.NickName, sex, level,Money, seq);
						}
					}
					g_PLAYER(nPlayerNum)->UI.nIcon		 = GetMoneyGrade(pMsg->EI->Ui[i].GetTotalMoney());
					g_PLAYER(nPlayerNum)->SetChangeRoomInfo(pMsg->EI->Cui[i]);
				}				
			}
		}

		// 버튼 상태 설정
		m_nCurPlayer = g_Poker.CurPlayer;
		g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();		
		
		//관전UI
		g_ObserverMan.OnObserve();
	}

	if(!g_Poker.bDistCard && g_Poker.nAutoBetCmd <=0 && g_RI.State == 1 && g_Poker.nRound >= 1)
		g_pGameView->m_cPlayerDrawManager.SetTurn(GetPNum_ByServPN(m_nCurPlayer));

	// 이미 던져진 칩들을 그려준다

	if(g_RI.ChipKind == (int)ROOM_CHIPKIND_GOLD)
	{
		m_AllBetChip.SetMoney(GetRealBet());	
	}
	else
	{
		g_pGameView->m_cChipBoard.PreDrawCastedChip(GetRealBet());
	}
	
	//버튼들 초기화
	if(g_pGameView)
	{
		g_pGameView->m_cGameViewBtnMananager.EnterRoom_Button_Init( g_RI.sBetRuleKind );
	}
	
	// 오토배팅 셋팅
	if(!g_ObserverMan.IsMyObserver())
	{
		AutoBetSetUpWnd().OnEnterRoom();
	}

	

	////////////////////////////////////////

	// 플레이어 순서에 맞게 카드를 세팅한다.
	for(int i = 0; i < MAX_PLAYER; ++i)
	{
		int nPlayNum = GetPNum_ByServPN(i);

		if(strlen(pMsg->EI->Ui[i].ID) > 0) 
		{
			// 게임 진행 상황 그릴것 카드 정보를 카드덱에다가 셋
			if(m_cPlayer[nPlayNum].JoinState == EGAME_JOININ) 
			{
				int nCardTotal = pMsg->EI->Pg.PS[i].nCardTotal;		// 총카드 수
				int nOpenTotal = pMsg->EI->Pg.PS[i].nOpenTotal;		// 오픈된 카드 수	
				
				int nRoundMaxCard = GetRoundMaxCard();
				if (g_Poker.nState == RSTATE_SD_RESULTCARD)
				{
					nRoundMaxCard = 2;		//카드 결과 연출 때는 카드는 맥스카드 2장이다.
				}

				for(int k = 0; k < nCardTotal; ++k)
				{

					m_cPlayer[nPlayNum].SetAddCard(g_nDUMY_CARD); // 카운트 자동증가

					//위치 재설정
					CPoint ptCardPos  = m_cPlayer[nPlayNum].GetFinalCardPt(nRoundMaxCard, k);
					m_cPlayer[nPlayNum].SetCardPos(k, ptCardPos);	

					// 게임도중 입장한 경우 다른 유저가 이미 받은 카드는 카드의 End처리를 해줘야 화면에 보인다.
					m_cPlayer[nPlayNum].m_cMyCard[k].SetMoveEnd(true);
					
					// 카드를 보이게한다.
					m_cPlayer[nPlayNum].ShowCard(k);					


					///히든카드의 경우 이미 벌려진 상태로 보여준다.				
					if (GetRoundMaxCard()-1 == k)
					{
						CPoint ptCardPos = m_cPlayer[nPlayNum].GetHiddenCardFinalPos();
						m_cPlayer[nPlayNum].SetCardPos(k, ptCardPos);
						
					}
				}

				m_cPlayer[nPlayNum].bFold		 = pMsg->EI->Pg.PS[i].bFold;
				m_cPlayer[nPlayNum].SetCardTotal(nCardTotal);
				m_cPlayer[nPlayNum].SetOpenCardCount(nOpenTotal);

// 				// 카드선택중이고 초이스 모드이면 더미를 하나더 추가해 준다.( 카드 선택중에 관전자로 입장하면 카드가 안보이는 문제 )
// 				if(g_ObserverMan.IsMyObserver() && nCardTotal == 3 && g_RI.FormKind == CHOICE_GAME )
// 				{
// 					m_cPlayer[nPlayNum].SetAddCard(g_nDUMY_CARD);
// 
// 					// 게임도중 입장한 경우 다른 유저가 이미 받은 카드는 카드의 End처리를 해줘야 화면에 보인다.
// 					m_cPlayer[nPlayNum].m_cMyCard[3].SetMoveEnd(true);
// 
// 					// 카드를 보이게한다.
// 					m_cPlayer[nPlayNum].ShowCard(3); 
// 				}


// 				if (g_Poker.nState == RSTAT_SD_RESULTCARD)
// 				{
// 					for(int k = 0; k < nCardTotal; ++k)
// 					{
// 						CPoint ptCardPos  = m_cPlayer[nPlayNum].GetFinalCardPt(nCardTotal, k);
// 						m_cPlayer[nPlayNum].SetCardPos(k, ptCardPos);						
// 					}
// 				}

				// 게임중이면서 오프된 카드가 있고 패를 돌리는 중
				if(g_Poker.nState > RSTATE_DISTRIBUTECARD && nOpenTotal > 0 && !m_cPlayer[nPlayNum].bFold)
				{
					if(nOpenTotal <= 2) 
					{
						for(int k = 0; k < nOpenTotal; ++k)
						{
							m_cPlayer[nPlayNum].m_cMyCard[k].SetCardNum(g_Poker.PS[i].nCard[k]); // 값 변경
							m_cPlayer[nPlayNum].SetCardFrontShow(k); // 보이기

							// 게임도중 입장한 경우 다른 유저가 이미 받은 카드는 카드의 End처리를 해줘야 화면에 보인다.
							m_cPlayer[nPlayNum].m_cMyCard[k].SetMoveEnd(true);
							m_cPlayer[nPlayNum].ShowCard(k);					
						}
					}
// 					else 
// 					{	// 모두 카드 오픈
// 						// 히든 카드
// // 						m_cPlayer[nPlayNum].m_cMyCard[0].SetCardNum(g_Poker.BC[i].nCard[0]);
// //  						m_cPlayer[nPlayNum].m_cMyCard[1].SetCardNum(g_Poker.BC[i].nCard[1]);
// //  						m_cPlayer[nPlayNum].m_cMyCard[2].SetCardNum(g_Poker.BC[i].nCard[2]);
// // 						m_cPlayer[nPlayNum].SetCardFrontShow(0);
// //  						m_cPlayer[nPlayNum].SetCardFrontShow(1);
// //  						m_cPlayer[nPlayNum].SetCardFrontShow(2);
// 
// 						for(int k = 0; k < nOpenTotal; ++k)	
// 						{
// 							m_cPlayer[nPlayNum].m_cMyCard[k].SetCardNum(g_Poker.PS[i].nCard[k]); // 값 변경
// 							m_cPlayer[nPlayNum].SetCardFrontShow(k); // 보이기
// 
// 							m_cPlayer[nPlayNum].m_cMyCard[k].SetMoveEnd(true);
// 							m_cPlayer[nPlayNum].ShowCard(k);					
// 						}
// 					}						
				}
			}
		}
	}

	// 사이드팟
	if(g_pGameView)
	{
		g_pGameView->m_cSidePotMoney.Check();
		g_pGameView->m_cTitle.MakeTitle();
	}	

	g_cPromotionMan.Accept_EnterRoom();

	// 게임화면에 포커스를 준다.
	g_pGameView->SetFocus();


	//게임설정 완료후 섯다 보이스 세팅
	SutdaGameSound().SetMyVoiceType(GM().GetMyInfo()->UI.Sex, g_Config.SndFxKind);

	//사운드작업
	SutdaGameSound().OnActionVoice( AV_JOINROOM, 0, 5);
	

	return true;
}


// 유저가 방에 입장 알림
void CGameProcessSutda::User_EnterRoom(void *pMsgParam)
{
	CSV_USERENTERROOM* pMsg= (CSV_USERENTERROOM*) pMsgParam;//UGPCommon 작업
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
	{
		return;
	}
	
	PBPlayEffectSound(SND_USERIN);
	int pnum = -1; // 플레이어 번호


	// 자신(게임참여중) / 상대방(게임참여 입장)
	if( !g_ObserverMan.IsMyObserver() && pMsg->UI->cObserver != 1 ) 
	{
		// 서버쪽의 0번인덱스 클라이언트 인덱스 부터
		for(int i = m_nNo1PosPNum; i < MAX_PLAYER + m_nNo1PosPNum; ++i) 
		{
			int index = i;

			if(index >= MAX_PLAYER) 
				index = index - MAX_PLAYER;
			
			if(index == 0)
				continue; //나
			
			if(strlen(g_PLAYER(index)->UI.ID) == 0)
			{
				pnum=index;
				break;
			}
		}

		// 비어있는 곳을 찾지 못했으면
		if(pnum == -1)
		{
			return; 
		}

		// 방 정보를 수정하고 새로 참여한 플레이어의 정보를 저장
		g_RI.NowUserNum += 1;
		g_PLAYER(pnum)->SetNewPlayer(pMsg->UI);

		g_pGameView->m_cPlayerDrawManager.OnEnterRoom(pnum);

		g_PLAYER(pnum)->ServPNum		= GetServPNum_ByPN(pnum);		// 서버측 플레이어 번호 저장

		CString str;
		str.Format("▶입장: [%s]님\n", pMsg->UI->NickName);		
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
	}		
	// 자신(관전중) / 상대방(게임참여 입장)
	else if(pMsg->UI->cObserver != 1)
	{
		for(int i = 0; i < MAX_PLAYER; ++i) 
		{
			if(strlen(g_PLAYER(i)->UI.ID) == 0)
			{
				pnum=i; 
				break;
			}
		}

		// 비어있는 곳을 찾지 못했으면
		if(pnum == -1) 
		{
			return; 
		}

		m_nMyPosPNum  = 0; // 서버쪽에 인덱스
		m_nNo1PosPNum = GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스

		// 방 정보를 수정하고 새로 참여한 플레이어의 정보를 저장
		g_RI.NowUserNum += 1;
		g_PLAYER(pnum)->SetNewPlayer(pMsg->UI);	

		g_pGameView->m_cPlayerDrawManager.OnEnterRoom(pnum);

		g_PLAYER(pnum)->ServPNum		= GetServPNum_ByPN(pnum);		// 서버측 플레이어 번호 저장

		CString str;
		str.Format("▶입장: [%s]님\n", pMsg->UI->NickName);		
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

	}		
	// 자신(관전중) / 상대방(관전 입장)
	else
	{
		if(pMsg->UI->AdminGrade > 0) 
		{
			pnum = GetMaxNewPlayer() - 1; //관리자
		}
		else 
		{
			for(int i = MAX_PLAYER; i < GetMaxNewPlayer() - 1; ++i)
			{
				if(strlen(g_PLAYER(i)->UI.ID) == 0)
				{
					pnum=i;
					break;
				}
			}
		}

		// 비어있는 곳을 찾지 못했으면
		if(pnum == -1)
		{
			return;			
		}

		g_PLAYER(pnum)->SetNewPlayer(pMsg->UI);		// 방 정보를 수정하고 새로 참여한 플레이어의 정보를 저장
		g_PLAYER(pnum)->ServPNum		= pnum;		// 서버측 플레이어 번호 저장

		// 관리자가 아니면
		if( pMsg->UI->AdminGrade == 0)
		{ 
			g_RI.NowObserverNum += 1;
			
			// 관전자 리스트 출력
			int ounum = g_PLAYER(pnum)->UI.UNum;
			int sex  = g_PLAYER(pnum)->UI.Sex;
			int level = g_PLAYER(pnum)->UI.nIcon;
			INT64 Money = g_PLAYER(pnum)->UI.GetMoney();
			int seq = 0;

			CObserverDlg *pObserverDlg = GetObserverDlg();
			if(NULL != pObserverDlg)
			{
				pObserverDlg->User_InsItem(ounum,g_PLAYER(pnum)->UI.ID, g_PLAYER(pnum)->UI.NickName, sex, level, Money, seq);
			}

			CString str;		
			str.Format("▶관전 입장: [%s]님\n", pMsg->UI->NickName);
			AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
		}
	}

	// 중복된 IP 체크
	for(int i = 0; i < MAX_PLAYER; ++i) 
	{
		int nPlayerNum = GetPNum_ByServPN(i);

		if(strlen(g_PLAYER(nPlayerNum)->UI.ID) > 0) 
		{
			g_PLAYER(nPlayerNum)->MyIpCheck = pMsg->IPA->IpIndex[i];
		}
	}

	g_PLAYER(pnum)->nSndFxKind = *pMsg->SndFxKind;// ### [사운드 추가 작업] ###
	g_pGameView->m_cTitle.MakeTitle();

	// 전광판에 내용 출력
	if(!g_ObserverMan.IsMyObserver())
	{
		CheckMinimumUser();
	}
}


// 유저가 방에서 나감 알림
void CGameProcessSutda::User_OutRoom(void *pMsgParam) 
{
	CSV_USEROUTROOM* pMsg = (CSV_USEROUTROOM*) pMsgParam;//UGPCommon 작업
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	PBPlayEffectSound(SND_USEROUT);
	int pnum=-1;
	for(int i=0; i<GetMaxNewPlayer(); i++) 
	{
		if(strcmp(g_PLAYER(i)->UI.ID, pMsg->ID)==0)
		{
			pnum=i;
			break;
		}
	}

	if(pnum == -1)
	{
		return;
	}

	// 닉네임수정 
	CString NickName =  g_PLAYER(pnum)->UI.NickName;


	// 게임중인 유저
	if( pnum < MAX_PLAYER ) 
	{
		// [ 카드 날리기 ]
		if(g_PLAYER(pnum)->bFold == FALSE && g_PLAYER(pnum)->JoinState == EGAME_JOININ && g_PLAYER(pnum)->PlayState == EPLAYER_GAMEPLAYING)
		{	//현재 겜중인 넘 나갔다.
			m_nStartUserNum--;
			if(m_nStartUserNum <0)
			{
				m_nStartUserNum = 0;
			}
		}

		if( g_pGameView->m_cPlayerDrawManager.IsExitReservation(pnum) )
		{
			//	NMBASE::SOUND::g_Sound.PlayWav(SND_FX_2);
		}

		g_PLAYER(pnum)->Clear(); // UI 최기화		

		g_pGameView->m_cPlayerDrawManager.OnExitRoom(pnum);

		if(g_RI.NowUserNum > 0) 
		{
			g_RI.NowUserNum -= 1;
		}

		CString str;		
		str.Format("◁퇴장: [%s]님\n", NickName.GetBuffer() );		
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
		
		str.Format("## <ID: %s(%s)>님 퇴장 ##\n",pMsg->ID , NickName.GetBuffer() );		
		AddLobbyHideChatText(&str);
	}
	// 관전 중인 유저
	else 
	{
		if(g_PLAYER(pnum)->UI.AdminGrade == 0)
		{			
			CObserverDlg *pObserverDlg = GetObserverDlg();
			if(NULL != pObserverDlg)
			{
				pObserverDlg->User_DelItem(g_PLAYER(pnum)->UI.UNum, g_PLAYER(pnum)->UI.ID);
			}			

			if(g_RI.NowObserverNum > 0) 
			{
				g_RI.NowObserverNum -= 1;
			}
			

			CString str;		
			str.Format("◁관전 퇴장: [%s]님\n", g_PLAYER(pnum)->UI.NickName);	
			AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

		}
		g_PLAYER(pnum)->Clear(); // UI 최기화
		//	NMBASE::SOUND::g_Sound.PlayWav(SND_FX_2);
	}

	// 중복된 IP 체크
	for(int i = 0; i < MAX_PLAYER; ++i)
	{
		int nPlayerNum = GetPNum_ByServPN(i);
		if(strlen(g_PLAYER(nPlayerNum)->UI.ID) > 0) 
		{
			g_PLAYER(nPlayerNum)->MyIpCheck = pMsg->IPA->IpIndex[i];
		}
	}

	g_pGameView->m_cTitle.MakeTitle();

	// 전광판에 내용 출력
	if(!g_ObserverMan.IsMyObserver())
	{
		CheckMinimumUser();
	}
}


void CGameProcessSutda::GameStart(SD_STARTINFO *pSC, bool bRematch)
{
	//HCardIndex : 내가 받는 히든 카드
	//OpenCard : 오픈 카드 정보

	if(pSC == NULL)
		return;	

	bool bOneCardsStart = (g_RI.FormKind==SD_2CARD && !bRematch);	//카드 1장 시작이냐 2장시작이냐?	

	// 룰을 초기화 한다.	
// 	m_cSevenRuleManager.Init();
// 	m_cSevenRuleManager.ClearAll();
	g_pGameView->m_cTotalBetMoney.Start();

	g_RI.State = EGAME_PLAYING;


	if( g_RI.CurSeedMoney != pSC->nSeedMoney )
	{
		g_RI.CurSeedMoney = pSC->nSeedMoney;		
		g_pGameView->m_cTitle.MakeTitle();
	}

	//규제안 : 최초 맥스 활성 여부 초기화 : 맥스이펙트
	for (int i=0; i<MAX_PLAYER; i++)
	{
		GM().g_bFirstMax[i] = false;
	}

	g_RI.RoomMasterType = pSC->RoomMasterType;

	// 서버, 클라이언트 각각 선언하여 별도로 사용한다.
	ZeroMemory(&g_Poker, sizeof(g_Poker));
	memcpy(g_Poker.PS, pSC->Ps, sizeof(PLAYSTATE)*MAX_PLAYER);	

	//규제안 : 예약버튼 활성화 가능상태
	ReserveState(RESERVESTATE_BET_NONE);
	CanReserve(true);
	g_nCardTotalCount = 0 ; // 카드 세팅 비교값을 0으로 초기화.

	g_Poker.nBossPNum = -1;	
	g_Poker.nImageBoss = -1;

	g_pGameView->m_cPlayerDrawManager.Reset();

	// 게임 시작 플래그 세팅
	m_bGameStart = TRUE;	

	for(int i = 0; i < MAX_PLAYER; ++i)
	{		
		// 플레이어 스크린 초기화
		g_PLAYER(i)->Reset();
	}
	
	/////////////////////////////////////////
	BOOL bFalse = FALSE;
	m_nStartUserNum = 0;
	INT64 i64BetMoney = (INT64)((double)g_RI.CurSeedMoney * (double)pSC->fBetMoneyMulti);

	for(int i = 0; i < MAX_PLAYER; ++i) 
	{		
		if(strlen(pSC->JoinID[i]) >0)
		{
			int nPlayNum = GetPNum_ByServPN(i);
			if(strcmp(m_cPlayer[nPlayNum].UI.ID, pSC->JoinID[i]) != 0) 
			{ 
				bFalse = TRUE; 
				break; 
			}
			
			// 플레이어 상태 세팅
			m_cPlayer[nPlayNum].JoinState = pSC->Ps[i].JoinState; 
			m_cPlayer[nPlayNum].PlayState = pSC->Ps[i].PlayState;
			m_cPlayer[nPlayNum].bFold     = FALSE;
			
			if( nPlayNum == 0 && !g_ObserverMan.IsMyObserver() )
			{
				GM().GetMyInfo()->JoinState = m_cPlayer[0].JoinState;
				GM().GetMyInfo()->PlayState = m_cPlayer[0].PlayState;
				GM().GetMyInfo()->bFold		= m_cPlayer[0].bFold;
				GM().GetMyInfo()->ServPNum  = m_cPlayer[0].ServPNum;				
			}

			//히스토리 관련 머니 셋팅 
			if(!g_ObserverMan.IsMyObserver() && m_cPlayer[0].JoinState == EGAME_JOININ && m_cPlayer[0].PlayState == EPLAYER_GAMEPLAYING && nPlayNum == 0 )
			{		
				m_cPlayer[0].nRoundPrevMoney = m_cPlayer[0].UI.GetMoney();
			}
			//yoo
			if((1 == pSC->Ps[i].nState)&&(false == bRematch)){
				g_pGameView->m_cPlayerDrawManager.SetLimitMoney(nPlayNum, true);
			}

			////보유머니에서 시드머니만큼 빼기
			//INT64 i64TempBetMoney = m_cPlayer[nPlayNum].UI.GetMoney() - i64BetMoney;
			//m_cPlayer[nPlayNum].UI.SetMoney( i64TempBetMoney );	

			//입장머니 반영후 제한방 체크 로직 추가 함수 호출
			INT64 i64TempBetMoney = m_cPlayer[nPlayNum].GetNowMoney() - i64BetMoney;
			m_cPlayer[nPlayNum].SetNowMoney( i64TempBetMoney );
			
			g_Poker.RU[i].nPMoney = m_cPlayer[nPlayNum].GetNowMoney();
			m_cPlayer[nPlayNum].nSndFxKind = pSC->Ps[i].nSndFxKind;

			// 유저 카드 셋팅 
			
			int nTotalCard = pSC->Ps[i].nCardTotal;
			int nTotalOpenCard = pSC->Ps[i].nOpenTotal;


			int nBackCardIndex =0;
			for(int k=0; k<nTotalCard; k++)
			{
				//오픈카드 세팅
				if (k < nTotalOpenCard)
				{
					m_cPlayer[nPlayNum].SetAddCard(pSC->Ps[i].nCard[k]);			//오픈카드	
					m_cPlayer[nPlayNum].SetCardFrontShow(k);
					continue;
				}

				//백카드 세팅
				if (nPlayNum==0 && !g_ObserverMan.IsMyObserver())
				{					
					m_cPlayer[nPlayNum].SetAddCard(pSC->nCard[nBackCardIndex]);					
					nBackCardIndex++;
				}
				else
				{
					m_cPlayer[nPlayNum].SetAddCard(g_nDUMY_CARD);
				}
			}

			//2장섯다.
			m_cPlayer[nPlayNum].SetCardTotal(nTotalCard);	
			m_cPlayer[nPlayNum].SetOpenCardCount(nTotalOpenCard);

			// 베팅칩 머니 세팅
			if(g_RI.ChipKind == (int)ROOM_CHIPKIND_GOLD)
			{
				m_AllBetChip.SetMoney(i64TempBetMoney);	
			}

			g_Poker.RU[i].bInUse	= TRUE;
			if (bRematch==false)
			{	
				//재대결이 아닐때만..
				g_Poker.RU[i].nRealBat	= i64BetMoney;
				chip_draw_func(nPlayNum, g_Poker.RU[i].nRealBat); // 기본 배팅 칩을 던짐				
			}

			m_nStartUserNum++;
		}
	}	


	if(bFalse == TRUE)
	{
		// 강퇴 처리
		CString st[MAX_PLAYER], str;
		for(int i = 0; i < MAX_PLAYER; ++i) 
		{
			int nPlayNum = GetPNum_ByServPN(i);
			st[i].Format("%d: %s, %s\n",i, m_cPlayer[nPlayNum].UI.NickName, pSC->JoinID[i]);
			str += st[i];		
		}
		
		ExitGameRequest();

		AfxMessageBox("잘못된 게임 시작정보로 인하여 퇴장합니다");
		AfxMessageBox(str);
		return;
	}


	g_Poker.nState		= RSTATE_PREPARESTART; // 게임 시작을 준비중
	g_Poker.nDistUser	= pSC->nDistUser;
	m_nCurPlayer		= pSC->nDistUser;
	g_Poker.CurPlayer = pSC->nDistUser; 

	g_DebugStartTick = timeGetTime();


	
	PBPlayEffectSound(SND_START);

	Card_Distribute();	


	if (bRematch==false)
	{
		// 자리 지킴이
		g_SeatKeeper.OnStartGame();	

		g_cPromotionMan.OnStartGame();

	}

}



// 카드 분배(게임시작 및 Raise에서 호출)
void CGameProcessSutda::Card_Distribute()
{
	if( RSTATE_NONE == g_Poker.nState) 
	{
		return;  
	}	

	// 카드 날리는 순서
	int nPlayerNumber = GetPNum_ByServPN( g_Poker.nDistUser ); 
	if(nPlayerNumber < 0 || nPlayerNumber >= MAX_PLAYER)
	{
		return;	
	}

	//규제안 :  예약 가능여부 설정
	CanReserve(false);
	g_pGameView->m_cGameViewBtnMananager.ShowReserveBtn(false);

	CPoint ptEndPosition;	
	CPoint ptStartPosition(0, 0);	// 카드의 시작 딜링 위치(멀티 Scene으로 받았다.)
	int nGameTypeToCardCount = 0;

	// 일반 / 초이스 모드에 따라 카드 배분이 다르다.
	if( (g_RI.FormKind==SD_2CARD || IsRematchGame()) )
	{
		nGameTypeToCardCount = 1; // 2장섯다
	}
	else
	{
		nGameTypeToCardCount = 2; // 3장섯다
	}

	// 카드 이동중 세팅
	g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(true);


	// 카드가 한장씩 나와야 하기때문에 누적 계산을 사용한다.
	int nDeleyCount = 0;
	CMyCard* pLastCard = NULL;// 마지막 카드 
	DWORD dwDelay = 0;
	const int DURAITION_CARDDEAL = 400;		//카드가 날라가는데 걸리는 시간
	const CPoint ptCardDealStart(463,4);			//카드딜링이 시작되는 좌표


	BOOL bFirstDeal = FALSE;
	BOOL bHiddenDeal = FALSE;
	//재경기
	if (IsRematchGame())
	{
		bFirstDeal = TRUE;
		bHiddenDeal = TRUE;
	}
	else
	{
		if (!g_Poker.bDistCard)
		{
			bFirstDeal= TRUE;
			bHiddenDeal= FALSE;
		}
		else
		{
			bFirstDeal= FALSE;
			bHiddenDeal= TRUE;
		}

	}

	// 초이스 모드인지 체크해서 한장 또는 4장의 카드를 분배 하느냐를 구분한다.
	if(bFirstDeal)
	{
		int lastp  = 0;
		int lastci = 0;

		// 처음 4장을 분배한다.
		for( int j = 0; j < nGameTypeToCardCount; ++j)	
		{
			//for(int i = nPlayerNumber; i < (MAX_PLAYER + nPlayerNumber); ++i) 
			for(int i = (MAX_PLAYER + nPlayerNumber)-1; i >= nPlayerNumber; i--) 
			{
				int nIndex = i;
				if(nIndex >= MAX_PLAYER)
				{
					nIndex = nIndex - MAX_PLAYER;
				}

				if(strlen(m_cPlayer[nIndex].UI.ID) > 0 && m_cPlayer[nIndex].JoinState == EGAME_JOININ && m_cPlayer[nIndex].PlayState == EPLAYER_GAMEPLAYING)
				{
					CMyCard* pMyCard = &m_cPlayer[nIndex].m_cMyCard[j];
					if(pMyCard != NULL)
					{
						//pMyCard->GetMove().StartMoveMSprite(&g_sprCardDealing, CPoint(0,0), pMyCard->GetPos(), (nIndex * DEALINGCARDGAP) + nGameTypeToCardCount, dwDelay, 0, g_cUIData.m_fStartGameDealingCardSpeed);
						pMyCard->GetMove().StartMove(&g_sprBigCard, ptCardDealStart, pMyCard->GetPos(), g_nDUMY_CARD, DURAITION_CARDDEAL, dwDelay, 0, g_cUIData.m_fStartGameDealingCardSpeed);
						

						// 한장씩 도착하면 보여준다
						pMyCard->SetEvent(EVENT_PREPARESTART_ONEEND, nIndex, j ); // 플레이 넘버,인덱스 값
						lastci = nIndex;
						lastp  = j;

						dwDelay += g_cUIData.m_dwStartGameDealingTimeGab;
						pLastCard = pMyCard;
					}
				}
			}
		}

		if(pLastCard != NULL)	
		{			 
			pLastCard->SetEvent(EVENT_THREEFLYEND, lastci, lastp); //  마지막 까지 날렸으면 서버로
			SetWindCardEnd(TRUE);
		}
	}

	if (bHiddenDeal)
	{
		int nNum = 0;		
		nNum = m_cPlayer[nPlayerNumber].GetCardTotal() - 1;	// 선의 카드 수를  따른다. 초이스

		// 카드 한장씩 분배한다.
		//for(int i = nPlayerNumber; i < (MAX_PLAYER + nPlayerNumber); ++i) 	  
		for(int i = (MAX_PLAYER + nPlayerNumber)-1; i >= nPlayerNumber; i--) 
		{
			int nIndex = i;

			if(nIndex >= MAX_PLAYER) 
			{
				nIndex = nIndex - MAX_PLAYER;
			}

			if(strlen(m_cPlayer[nIndex].UI.ID) > 0 && m_cPlayer[nIndex].JoinState == EGAME_JOININ && m_cPlayer[nIndex].PlayState == EPLAYER_GAMEPLAYING)	
			{
				CMyCard* pMyCard = &m_cPlayer[nIndex].m_cMyCard[nNum];

				if(pMyCard != NULL)
				{
					//pMyCard->GetMove().StartMoveMSprite(&g_sprCardDealing, CPoint(0,0), pMyCard->GetPos(), (nIndex * DEALINGCARDGAP) + nNum, dwDelay, 0, g_cUIData.m_fDealingCardSpeed);																				
					CPoint ptDest = pMyCard->GetPos();
					pMyCard->SetCardPos(ptCardDealStart.x, ptCardDealStart.y);
					pMyCard->SetCardFront(FALSE);
					
					pMyCard->MoveCard( ptDest, g_nDUMY_CARD, DURAITION_CARDDEAL, dwDelay);
					dwDelay += g_cUIData.m_dwDealingTimeGab;

					// 한장씩 도착하면 보여준다
					pMyCard->SetEvent(EVENT_ONEFLYEND, nIndex, nNum ); // 플레이 넘버,인덱스 값
					pLastCard = pMyCard;					
				}
			}
		}

		if(pLastCard != NULL)
		{
			pLastCard->SetEvent1(EVENT_ONEFLYTOTEND); // 모두 도착하면 서버로
			SetWindCardEnd();			
		}



		//전광판 (마지막패입니다)
		if (!IsRematchGame())
		{
			g_pGameView->m_cDisplayBoard.SetText(EDBT_FINAL_DEAL); 			
		}
	}	
}


void CGameProcessSutda::Raise(POKERGAME *pMsg)
{
	// 베팅 버튼 및 금액 관련된 처리를 담당한다.
	RaiseBase(pMsg);
	g_pGameView->m_cGameViewBtnMananager.Reset();
	
	// 현재 순서가 카드 날리기 인가?
	if( pMsg->bDistCard )
	{ 		
		int nMaxCard = 0;

		// 카드 날리기 할때 베팅머니 표시되는 부분을 초기화 한다.
		// 2012. 04. 25 최종학
		for (int i = 0; i < GetMaxPlayer(); ++i)
		{
			g_pGameView->m_cPlayerDrawManager.ClearBettingMoney(i);
		}

		// 장수 증가 할것
		for(int i = 0; i < MAX_PLAYER; ++i)
		{
			int nPlayNum = GetPNum_ByServPN(i);

			// 게임 중이면
			if( strlen(m_cPlayer[nPlayNum].UI.ID) > 0 && 
				m_cPlayer[nPlayNum].JoinState == EGAME_JOININ && 
				m_cPlayer[nPlayNum].PlayState == EPLAYER_GAMEPLAYING && 
				!m_cPlayer[nPlayNum].bFold ) 
			{
				int nCardTotalCount = pMsg->PS[i].nCardTotal;		// 총카드 수
				int nCardOpenCount	= pMsg->PS[i].nOpenTotal;		// 오픈된 카드수	
				
				//히든카드 (섯다)
				m_cPlayer[nPlayNum].SetAddCard(g_nDUMY_CARD);					
								

				m_cPlayer[nPlayNum].SetCardTotal(nCardTotalCount);
				m_cPlayer[nPlayNum].SetOpenCardCount(nCardOpenCount);

				if(nMaxCard < nCardTotalCount) 
				{
					nMaxCard = nCardTotalCount;
				}
			}
			
		}

		g_pGameView->m_cDisplayBoard.SetText(EDBT_HIDDENCARD);	//전광판 히든카드이다 

		Card_Distribute();
		//g_pGameView->m_cGameHelper.SetAllUserCardInfo();
	
		//리미트배팅 초기화( 새카드 오는 시점 )
		g_pGameView->m_cGameViewBtnMananager.SetLastCmdBackup( RAISE_LAST_CMD_NULL, true );

		//규제안 : 예약버튼 설정을 위해 호출
		g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();
	}	

// 	// 사이드팟 
 	if(g_pGameView)
 	{
 		g_pGameView->m_cSidePotMoney.Check();
 	}
}

// 베팅 버튼 및 금액 관련된 처리를 담당한다.
void CGameProcessSutda::RaiseBase(POKERGAME *pMsg)
{
	if(pMsg == NULL)
	{
		return;
	}
	// 현재 베팅한 사람의 베팅금액을 알아오기 위해 이전 베팅정보 임시 저장
	RAISEUSER RU_Before[MAX_PLAYER];
	memcpy(RU_Before, g_Poker.RU, sizeof(RAISEUSER) * MAX_PLAYER);

	// 값을 저장한다.
	memcpy(&g_Poker, (char*)pMsg, sizeof(g_Poker));	
	g_pGameView->m_cPlayerDrawManager.SetTurn(-1);


	// 사용자머니 변경 
	for(int i = 0; i < MAX_PLAYER; ++i)
	{
		int nPlayerNum = GetPNum_ByServPN(i);

		// 게임 중이면
		if(strlen(g_PLAYER(nPlayerNum)->UI.ID) > 0 && g_PLAYER(nPlayerNum)->JoinState == EGAME_JOININ && g_PLAYER(nPlayerNum)->PlayState == EPLAYER_GAMEPLAYING) 
		{  
			//g_PLAYER(nPlayerNum)->UI.SetMoney( g_Poker.RU[i].nPMoney );// 사용자 머니 셋
			//g_PLAYER(nPlayerNum)->UI.PMoney = g_Poker.RU[i].nPMoney;// 사용자 머니 셋
			//입장머니 제한 방 검색 함수 호출
			g_PLAYER(nPlayerNum)->SetNowMoney( g_Poker.RU[i].nPMoney );// 사용자 머니 셋

			if( g_Poker.PS[i].bFold )
			{
				g_PLAYER(nPlayerNum)->bFold = TRUE;
			}

			//// 추가 
			//if(g_PLAYER(nPlayerNum)->UI.GetMoney() < 0) 
			//{
			//	g_PLAYER(nPlayerNum)->UI.SetMoney( 0 );
			//}
			// 추가 
			if(g_PLAYER(nPlayerNum)->GetNowMoney() < 0) 
			{
				g_PLAYER(nPlayerNum)->SetNowMoney( 0 );
				//g_PLAYER(nPlayerNum)->UI.SetMoney( 0 );
			}

			g_PLAYER(nPlayerNum)->UI.nIcon = GetMoneyGrade(g_PLAYER(nPlayerNum)->UI.GetTotalMoney());
		}
	}

	// 정보 창에 메세지 && 사운드 출력
	if(g_Poker.LastPlayer >= 0 && g_Poker.LastPlayer < MAX_PLAYER)
	{
		int LastPnum = g_Poker.LastPlayer;
		int nSNum    = GetPNum_ByServPN(LastPnum);

		if(g_Poker.RU[LastPnum].nLastCmd > 0 && nSNum >= 0 && nSNum < MAX_PLAYER) 
		{				
			// 전사람 타임바 지우기
			g_PLAYER(nSNum)->ClearEffectSpr(ANF_PLAYER_TIMEBAR);		

			int nLastCmd = g_Poker.RU[LastPnum].nLastCmd;

			// 베팅 이미지를 내 전사람은 작은걸로 바꿔 주고 난 지운다.
			g_pGameView->m_cAniSprListManager.BetCmd_ChangeCmd(nSNum);

			// 현재 베팅 금액을 보여주기 위해 (현재 총 베팅금액 - 총 베팅금액)을 클라에서 계산해서 보여줌
			g_pGameView->m_cPlayerDrawManager.SetBettingMoney(nSNum, nLastCmd, g_Poker.RU[LastPnum].nRealBat - RU_Before[LastPnum].nRealBat);
			//g_pGameView->m_cPlayerDrawManager.SetBettingMoney(nSNum, nLastCmd, g_Poker.RU[LastPnum].nNowBat);

			if (2 == g_Poker.RU[LastPnum].bMaxBet)
			{
				g_pGameView->m_cPlayerDrawManager.SetLimitMoney(nSNum, true);
			}

			if(g_Poker.RU[LastPnum].bMaxBet && GM().g_bFirstMax[LastPnum] == false)	
			{
				// 맥스이펙트
				CBettingEffect *pObj = new CBettingEffect(); 
				pObj->Init(nSNum, ANF_BET_CMD, ANF_BET_ALLIN + 1);	
				g_pGameView->m_cAniSprListManager.ADDObject(pObj);

				if(g_PLAYER(nSNum)->GetNowMoney() == 0 )
				{
					g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
				}

				if(IsHiddenRound())
				{					
					SoundFxKindBig(RAISE_LAST_CMD_MAX_BET, g_PLAYER(nSNum)->UI.Sex, g_PLAYER(nSNum)->GetSndFxKind());
				}
				else
				{
					SoundFxKind(RAISE_LAST_CMD_MAX_BET, g_PLAYER(nSNum)->UI.Sex, g_PLAYER(nSNum)->GetSndFxKind());
				}	
			
				GM().g_bFirstMax[LastPnum] = true;
			}
			else
			{
				switch(nLastCmd)
				{
					// 다이
				case RAISE_LAST_CMD_FOLD: // foldUser 에서 처리
					{
						CBettingEffect *pObj = new CBettingEffect();
						pObj->Init(nSNum, ANF_BET_CMD, ANF_BET_DIE);				
						g_pGameView->m_cAniSprListManager.ADDObject(pObj);
					} break; 

					// 체크
				case RAISE_LAST_CMD_CHECK:
					{					
						if(g_Poker.bDistCard == TRUE) break;

						//SoundFxKind(nLastCmd,g_PLAYER(nSNum)->nSndFxKind);

						CBettingEffect *pObj = new CBettingEffect();
						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_CHECK);				
						g_pGameView->m_cAniSprListManager.ADDObject(pObj);
					} break;

					// 콜
				case RAISE_LAST_CMD_CALL: 
					{	
						if(g_Poker.bDistCard == TRUE) break;	

						CBettingEffect *pObj = new CBettingEffect();
						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_CALL);
						g_pGameView->m_cAniSprListManager.ADDObject(pObj);

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);
						if(g_PLAYER(nSNum)->GetNowMoney() == 0 )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}
					} break;

					// 삥
				case RAISE_LAST_CMD_PING:  
					{	
						if(g_Poker.bDistCard == TRUE) break;		

						CBettingEffect *pObj = new CBettingEffect();
						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_PING);
						g_pGameView->m_cAniSprListManager.ADDObject(pObj);

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);
						if(g_PLAYER(nSNum)->GetNowMoney() == 0 )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}
					} break;

					// 따당
				case RAISE_LAST_CMD_DADANG:
					{					
						if(g_Poker.bDistCard == TRUE) break;				

						CBettingEffect *pObj = new CBettingEffect();
						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_DADANG);				
						g_pGameView->m_cAniSprListManager.ADDObject(pObj);

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);
						if(g_PLAYER(nSNum)->GetNowMoney() == 0 )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}
					} break;	
				case RAISE_LAST_CMD_FULL:
					{					
						if(g_Poker.bDistCard == TRUE) break;				

						CBettingEffect *pObj = new CBettingEffect();
						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_FULL);				
						g_pGameView->m_cAniSprListManager.ADDObject(pObj);

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);
						if(g_PLAYER(nSNum)->GetNowMoney() == 0 )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}
					} break;	

					// 하프
				case RAISE_LAST_CMD_HALF:
					{					
						if(g_Poker.bDistCard == TRUE) break;

						BOOL blasthalf = FALSE;

						// 히든
						if(IsHiddenRound())
						{
							blasthalf = TRUE;
							//하프를 쳤던 넘들만 족보를 강하게 불러준다.
							g_PLAYER(nSNum)->m_bLastHalf=TRUE;
						}

						CBettingEffect *pObj = new CBettingEffect();
						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_HALF,blasthalf);
						g_pGameView->m_cAniSprListManager.ADDObject(pObj);

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);
						if(g_PLAYER(nSNum)->GetNowMoney() == 0 )
						{	
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}
					} break;

					// 올인
				case RAISE_LAST_CMD_ALLIN:
					{					
						if(g_Poker.bDistCard == TRUE) break;

						BOOL blasthalf = FALSE;

						// 히든
						if(IsHiddenRound())
						{
							blasthalf = TRUE;
							//하프를 쳤던 넘들만 족보를 강하게 불러준다.
							g_PLAYER(nSNum)->m_bLastHalf = TRUE;
						}

						CBettingEffect *pObj = new CBettingEffect();
						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_ALLIN,blasthalf);
						g_pGameView->m_cAniSprListManager.ADDObject(pObj);

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);
						if(g_PLAYER(nSNum)->GetNowMoney() == 0 )
						{	
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}
					} break;	
					// 1배
				case RAISE_LAST_CMD_LIMIT_1X:
					{
						if( g_Poker.bDistCard == TRUE ) break;		

						CBettingEffect *pObj = new CBettingEffect();
						pObj->Init( nSNum, ANF_BET_CMD, ANF_BET_RAISE_X1 );
						g_pGameView->m_cAniSprListManager.ADDObject( pObj );

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni( nSNum, 0 );
						if( g_PLAYER(nSNum)->GetNowMoney() == 0 )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}
					}break;
					// 2배
				case RAISE_LAST_CMD_LIMIT_2X:
					{
						if( g_Poker.bDistCard == TRUE ) break;		

						CBettingEffect *pObj = new CBettingEffect();
						pObj->Init( nSNum, ANF_BET_CMD, ANF_BET_RAISE_X1 );
						g_pGameView->m_cAniSprListManager.ADDObject( pObj );

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni( nSNum, 0 );
						if( g_PLAYER(nSNum)->GetNowMoney() == 0 )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}
					}break;
					// 5배
				case RAISE_LAST_CMD_LIMIT_5X:
					{
						if( g_Poker.bDistCard == TRUE ) break;		

						CBettingEffect *pObj = new CBettingEffect();
						pObj->Init( nSNum, ANF_BET_CMD, ANF_BET_RAISE_X2 );
						g_pGameView->m_cAniSprListManager.ADDObject( pObj );

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni( nSNum, 0 );
						if( g_PLAYER(nSNum)->GetNowMoney() == 0 )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}
					}break;
					// 10배
				case RAISE_LAST_CMD_LIMIT_10X:
					{
						if( g_Poker.bDistCard == TRUE ) break;		

						CBettingEffect *pObj = new CBettingEffect();

						if( g_Poker.nRound == 1 || g_Poker.nRound == 2 )
						{
							pObj->Init( nSNum, ANF_BET_CMD, ANF_BET_RAISE_X3 );
						}
						else
						{
							pObj->Init( nSNum, ANF_BET_CMD, ANF_BET_RAISE_X2 );
						}

						g_pGameView->m_cAniSprListManager.ADDObject( pObj );

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni( nSNum, 0 );
						if( g_PLAYER(nSNum)->GetNowMoney() == 0 )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}
					}break;
					// 20배
				case RAISE_LAST_CMD_LIMIT_20X:
					{
						if( g_Poker.bDistCard == TRUE ) break;		

						CBettingEffect *pObj = new CBettingEffect();
						pObj->Init( nSNum, ANF_BET_CMD, ANF_BET_RAISE_X3 );
						g_pGameView->m_cAniSprListManager.ADDObject( pObj );

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni( nSNum, 0 );
						if( g_PLAYER(nSNum)->GetNowMoney() == 0 )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}
					}break;	
				}

				if( nLastCmd >= RAISE_LAST_CMD_CHECK && nLastCmd <= RAISE_LAST_CMD_ALLIN && g_Poker.bDistCard == FALSE )
				{
					// 히든
					if(IsHiddenRound())
					{					
						SoundFxKindBig((RAISE_LAST_CMD)nLastCmd, g_PLAYER(nSNum)->UI.Sex, g_PLAYER(nSNum)->GetSndFxKind());
					}
					else
					{
						SoundFxKind((RAISE_LAST_CMD)nLastCmd, g_PLAYER(nSNum)->UI.Sex, g_PLAYER(nSNum)->GetSndFxKind());
					}				
				}
			}
		}

		// 규제안 관련 작업으로 주석처리 2014.01 -> 추후작업
		//따당 금액 구하기
		if( g_Poker.RU[LastPnum].nLastCmd != RAISE_LAST_CMD_FOLD )
		{
			m_nDouble = ( g_Poker.RU[ g_Poker.LastPlayer ].nCalcBat + g_Poker.RU[ g_Poker.LastPlayer ].nReCall ) * 2;	
		}

		if( g_Poker.bDistCard )
		{
			m_BetCount = -1;
			g_pGameView->m_cAniSprListManager.BetCmd_DelDelay(55);
			m_nBetCallMoneyClearCnt = 55;
			return;
		}
	}	

	// 게임이 시작되었음( 레이즈모드이다 베팅시작 내턴이면 버튼 활성화 )
	if(g_Poker.nState == RSTATE_STARTGAME) 
	{
		m_nCurPlayer = g_Poker.CurPlayer;
		g_pGameView->m_cPlayerDrawManager.SetTurn(GetPNum_ByServPN(m_nCurPlayer));

		if(m_nCurPlayer < 0 || m_nCurPlayer >= MAX_PLAYER)
			return;

		int nPlayerNum = GetPNum_ByServPN(m_nCurPlayer);

		if( g_Poker.CurPlayer == g_Poker.nBossPNum && g_Poker.bBtnBet == TRUE )
		{
			m_BetCount++;
		}

		if(g_Poker.LastStyle == -1 )
		{
			INT64 nRaiseBat = GetRaiseBat();
			chip_draw_func(nPlayerNum, nRaiseBat);
			g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();

			int LastPlayer = g_Poker.LastPlayer;
			int bet = GetPNum_ByServPN(LastPlayer);	

			//// 혹시모를 예외 검사
			//if(bet >= 0 && (g_Poker.LastPlayer >= 0 && g_Poker.LastPlayer < MAX_PLAYER) && (LastPlayer >= 0 && LastPlayer < MAX_PLAYER))
			//{
			//	g_PLAYER(bet)->OnSetBetDrawMoney(GetLastBetMoney(LastPlayer, g_Poker.RU[g_Poker.LastPlayer].nLastCmd), 0, g_Poker.RU[LastPlayer].nLastCmd);				
			//}
			return;
		}

		if(g_Poker.LastStyle >= 2  ) // 베팅시에만
		{
			int LastPlayer = g_Poker.LastPlayer;
			
			if(LastPlayer < 0 || LastPlayer >= MAX_PLAYER) 
				return;
			
			int bet = GetPNum_ByServPN(LastPlayer);			

			if(g_Poker.RU[LastPlayer].nLastCmd == 3)
			{
				INT64 nRaiseBat = GetRaiseBat();
				chip_draw_func(bet, nRaiseBat);				
			}
			else
			{
				INT64 nRaiseBat = GetRaiseBat();
				chip_draw_func(bet, nRaiseBat);
			}

			// 배팅 금액 표시
			if(m_i64JackPotActive <=0 && !g_SelectCard().IsChoiceCard())
			{				
				//따당 금액 구하기
				if( g_Poker.RU[g_Poker.LastPlayer].nLastCmd != 1 )
				{
					//규제안 관련하여 따당머니 주석 처리 2014.01-> 추후작업
					m_i64Double = ( g_Poker.RU[ g_Poker.LastPlayer ].nCalcBat + g_Poker.RU[ g_Poker.LastPlayer ].nReCall ) * 2;
				}
			}

			//g_PLAYER(bet)->OnSetBetDrawMoney(GetLastBetMoney(LastPlayer,g_Poker.RU[g_Poker.LastPlayer].nLastCmd), 0,g_Poker.RU[LastPlayer].nLastCmd);	
		}

		g_pGameView->m_cPlayerDrawManager.SetStartTimer();
		g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(false);

		//배팅 시작(베팅 버튼 활성)의 핵심이다. m_nActiveBetBtnDelayTime 값으로 딜레이를 준다.
		if(!g_ObserverMan.IsMyObserver() && g_PLAYER(0)->ServPNum == m_nCurPlayer)
		{
			//g_PLAYER(0)->OnReSetBetDraw();
			g_pGameView->m_cGameViewBtnMananager.SetShowBetMony();
			g_pGameView->m_cGameViewBtnMananager.SetButtonClick(false);
			m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelay(0,0);
		}

		// 규제안 예약 베팅 추가.(방장이 아닌 유저는 버튼 활성화가 안옴)
		if(!g_ObserverMan.IsMyObserver() && g_PLAYER(0)->ServPNum != m_nCurPlayer)
		{
			m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelay(0,0);
		}

		//규제안 :  예약 가능여부 설정
		if(CheckReceivedAllCards())
		{
			CanReserve(true);
		}

		g_pGameView->m_cAniSprListManager.BetCmd_ClearCmd(nPlayerNum, 20);
		
		return;
	}
}


// 게임 오버
void CGameProcessSutda::OnGameOver(GAMEOVERRESULT *pGOR)
{	
	//NMBASE::SOUND::g_Sound.Stop(SND_HIDDEN_BGM);
	m_nCurPlayer = -1;
	
	// 게임 결과 정보 
	ZeroMemory(&m_GameOverResultData,sizeof(m_GameOverResultData));	
		//g_pGameView->m_cGameHelper.Reset();

	//규제안 관련하여 예약기능 모두 초기화.
	ReserveState(RESERVESTATE_BET_NONE);
	CanReserve(false);
	g_pGameView->m_cGameViewBtnMananager.ShowReserveBtn(false);

	g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();		// 버튼 초기화
	g_pGameView->m_cPlayerDrawManager.SetOpen(pGOR->bOpen);
	g_pGameView->m_cPlayerDrawManager.SetWinnerPNum(-1);
	g_pGameView->m_cPlayerDrawManager.SetTurn(-1);

	g_SelectCard().Clear();

// 	m_WinnerPNumData.Clear();
// 
// 	m_WinnerPNumData.nWinnerNum = pGOR->nWinnerNum;
// 	m_WinnerPNumData.bOneWinner = m_WinnerPNumData.nWinnerNum > 1 ? FALSE : TRUE;
// 	memcpy( m_WinnerPNumData.WinnerSPNum , pGOR->nNowWinner , sizeof(m_WinnerPNumData.WinnerSPNum));

	//베팅 커멘드 지움
	g_pGameView->m_cAniSprListManager.BetCmd_DelDelay(55);

	memcpy(&g_Poker, &pGOR->Pg, sizeof(g_Poker));
	memcpy(&m_GameOverResultData, pGOR, sizeof(m_GameOverResultData));
	
	for(int i = 0 ;  i < MAX_PLAYER ; ++i)
	{
		g_Poker.RU[i].nLastCmd = 0;				

		// 전사람 타임바 지우기 
		g_PLAYER(i)->ClearEffectSpr(ANF_PLAYER_TIMEBAR);		//타임바	
		g_PLAYER(i)->ClearEffectSpr(ANF_PLAYER_CUTINGSELECT);	//커팅 선택중 이미지.
		g_PLAYER(i)->ClearEffectSpr(ANF_PLAYER_CUTINGSELECTED);	//커팅 선택 완료
		g_PLAYER(i)->ClearEffectSpr(ANF_PLAYER_CUTTINGNUMBER);	//커팅 횟수 ,패스
		g_PLAYER(i)->ClearEffectSpr(ANF_PLAYER_TIMEBAR);
		g_PLAYER(i)->ClearEffectSpr(ANF_CUTINFO_BD);
		g_PLAYER(i)->ClearEffectSpr(ANF_PLAYER_ALLINMARK);

		//g_PLAYER(i)->OnReSetBetDraw();
	}
	// 게임 결과를 전광판에 출력한다.
	g_pGameView->m_cDisplayBoard.SetText(EDBT_RESULT_SUTDA, (LPARAM)&m_GameOverResultData); 

	// 결과 출력 Delay를 주기 위함 
	m_nResultDelayCount = 20;
	m_bResultDelayStart = TRUE;

	g_pGameView->m_cPlayerDrawManager.CheckResultValue();

	//카드 쪼으기를 더이상 못하게한다.
	if (!g_ObserverMan.IsMyObserver())
	{
		int nHiddenCardIndex = g_GAMEPROCESS()->GetRoundMaxCard() - 1;
		g_PLAYER(0)->m_cMyCard[nHiddenCardIndex].SetReadyToJJogi(FALSE); 
	}

	////////////////////////////////////////섯다 게임 결과////////////////////////////////////

	int nWinnerServPNum = m_GameOverResultData.nNowWinner[IDX_SP_WINNER]; ///승자 서버 번호
	int nWinnerPNum = GetPNum_ByServPN(nWinnerServPNum);					//승자 클라 번호

	//bts 수정 정보. 족보값 입력 받는 부분.
	//2012.7.26 류승민
	m_strwinnerJokbo = g_PLAYER(nWinnerPNum)->GetJokboName() ;

	int i;
	for(int i = 0; i < MAX_PLAYER; ++i)
	{
		if(strlen(m_GameOverResultData.Ui[i].ID) > 0) //  겜참여자.
		{
			int pnum = GetPlayerPNum(m_GameOverResultData.Ui[i].ID);

			if(pnum == -1)
			{
				continue;
			}
			g_PLAYER(pnum)->m_n64ResultMoney = m_GameOverResultData.Ui[i].nResultMoney;		
			memcpy( &g_PLAYER(pnum)->UI.AllGameRecord, &m_GameOverResultData.Ui[i].AllGameRecord, sizeof(m_GameOverResultData.Ui[i].AllGameRecord) );
			g_PLAYER(pnum)->SetChangeRoomInfo(m_GameOverResultData.Cui[i]);
			
			//g_PLAYER(pnum)->UI.SetMoney( m_GameOverResultData.Ui[i].PMoney );		
			//g_PLAYER(pnum)->UI.PMoney		 = m_GameOverResultData.Ui[i].PMoney;

			//입장머니 제한 설정
			if( g_RI.bIsMaxRoom )
			{
				g_PLAYER(pnum)->SetNowMoney( m_GameOverResultData.Ui[i].llInMoney );	
			}
			else
			{
				g_PLAYER(pnum)->SetNowMoney( m_GameOverResultData.Ui[i].PMoney );	
			}

			//if(	g_PLAYER(pnum)->UI.GetMoney() <= 0)
			//{
			//	g_PLAYER(pnum)->UI.SetMoney( 0 );
			//}
			if(	g_PLAYER(pnum)->GetNowMoney() <= 0)
			{
				g_PLAYER(pnum)->SetNowMoney( 0 );
			}

			// 레벨상점 폐쇄
			g_PLAYER(pnum)->UI.nIcon = GetMoneyGrade(g_PLAYER(pnum)->UI.GetTotalMoney());	

			if(pnum == 0 && !g_ObserverMan.IsMyObserver())
			{								
				g_PLAYER(pnum)->UI.CopyAllGameRecords( GM().GetMyInfo()->UI.AllGameRecord );
				GM().GetMyInfo()->SetChangeRoomInfo(*g_PLAYER(pnum)->GetChangeRoomInfo());
				GM().GetMyInfo()->UI.SetMoney( g_PLAYER(pnum)->UI.GetMoney() ); 
				GM().GetMyInfo()->UI.nIcon			= g_PLAYER(pnum)->UI.nIcon;						
			}
		}
	}	


	BOOL endvoiceallin = FALSE;	//올인된 유저가 있는지 검사한다.

	int winpnum = -1;
	int wingornum = -1;
	for( i=0; i<MAX_PLAYER; i++)
	{
		//### 오류검사		
		if(strlen(m_GameOverResultData.Ui[i].ID) > 0) //  겜참여자.
		{
	
			int pnum = GetPlayerPNum(m_GameOverResultData.Ui[i].ID);		
			if(pnum == -1) 
				continue;


			/////////////////// 유저별 결과 정보 세팅 //////////////////////////////


			g_PLAYER(pnum)->m_n64ResultMoney = m_GameOverResultData.Ui[i].nResultMoney;		
			memcpy( &g_PLAYER(pnum)->UI.AllGameRecord, &m_GameOverResultData.Ui[i].AllGameRecord, sizeof(m_GameOverResultData.Ui[i].AllGameRecord) ); 

			g_PLAYER(pnum)->SetChangeRoomInfo(m_GameOverResultData.Cui[i]);
			
			//g_PLAYER(pnum)->UI.SetMoney( m_GameOverResultData.Ui[i].PMoney );
			//입장머니 제한 설정
			if( g_RI.bIsMaxRoom )
			{
				g_PLAYER(pnum)->SetNowMoney( m_GameOverResultData.Ui[i].llInMoney );	
			}
			else
			{
				g_PLAYER(pnum)->SetNowMoney( m_GameOverResultData.Ui[i].PMoney );	
			}

			g_PLAYER(pnum)->m_llPenaltyMoney = m_GameOverResultData.Ui[i].nPenaltyMoney;	//땡값
			g_PLAYER(pnum)->m_ePenaltyState = (PENALTY_STATE)m_GameOverResultData.Ui[i].nPenaltyState;	//땡값 종류
			
			if(	g_PLAYER(pnum)->GetNowMoney() <= 0)
			{
				g_PLAYER(pnum)->SetNowMoney( 0 );
			}			
			g_PLAYER(pnum)->UI.nIcon = GetMoneyGrade(g_PLAYER(pnum)->UI.GetTotalMoney());	//등급계산

			if(pnum == 0 && !g_ObserverMan.IsMyObserver())
			{								
				g_PLAYER(pnum)->UI.CopyAllGameRecords( GM().GetMyInfo()->UI.AllGameRecord );
				GM().GetMyInfo()->SetChangeRoomInfo(*g_PLAYER(pnum)->GetChangeRoomInfo());
				GM().GetMyInfo()->UI.SetMoney( g_PLAYER(pnum)->UI.GetMoney() ); 
				GM().GetMyInfo()->UI.nIcon			= g_PLAYER(pnum)->UI.nIcon;	
// 				GM().GetMyInfo()->m_llPenaltyMoney = g_PLAYER(pnum)->m_llPenaltyMoney;	;	//땡값
// 				GM().GetMyInfo()->m_ePenaltyState = g_PLAYER(pnum)->m_ePenaltyState;	;	//땡값 종류
			}

			//////////////////////////////////////////////////////////////////////////

			
			// 땡값을 캐릭터창에 표시
			if(g_PLAYER(pnum)->m_llPenaltyMoney != 0)
			{
				//땡값을 물었는데 49박일경우
				if(m_GameOverResultData.Ui[i].nPenaltyState == PSTATE_49)
				{					
					CPoint ptEffecct = g_cUIData.m_listResultStampPos[pnum];
					g_pGameView->m_AniQue.Ani_Insert(ANIMATION_WINLOSS, ptEffecct, 0, 0, 3, FALSE, SND43, NULL, 5 );

					if( pnum == 0) 
						NMBASE::SOUND::g_Sound.PlayWav( SND22 );	//49박사운드
				}

				g_pGameView->m_cPlayerDrawManager.ShowPenaltyMoney(pnum, true);				
			}		

			//올인된 유저
			if(g_PLAYER(pnum)->GetNowMoney()==0) 
			{
				CPoint ptEffecct = g_cUIData.m_listResultStampPos[pnum];

				//올인유저를 표시한다.
				g_pGameView->m_AniQue.Ani_Insert(ANIMATION_WINLOSS, ptEffecct, 0, 0, 4, FALSE,  pnum == 0 ? SND42: NULL, NULL, 5 );						
				endvoiceallin = TRUE;	//올인된 유저가 있다.

				//내가 올인되었을경우
				if( pnum == 0 ) 
				{
					SutdaGameSound().SetEndActionVoice( END_ALLIN );					
				}
			}


			//승자일경우			
			if(m_GameOverResultData.nNowWinner[IDX_SP_WINNER] == i)
			{
				wingornum = i;
				m_NowWinpnum = winpnum = pnum;	//이긴사람 번호 넣어줌...

				//승자가 본인일 경우 
				if( pnum == 0 )
				{
					//대박승 및 쪽박승의 범위설정
					INT64 maxgetmoney = g_RI.CurSeedMoney * (INT64)500000000;
					INT64 mingetmoney = g_RI.CurSeedMoney * (INT64)4000000;
					
					//대박승이였을경우 (기본배팅액 * 5억원)
					if( m_GameOverResultData.Ui[i].nResultMoney > maxgetmoney ) 
						SutdaGameSound().SetEndActionVoice( END_BIG );
					//쪽박승이였을경우 (기본배팅액 * 4백만원)
					else if( m_GameOverResultData.Ui[i].nResultMoney < mingetmoney ) 
						SutdaGameSound().SetEndActionVoice( END_SMALL );
				}
			}

		}		
	}
	
	//내가 올인시킨 사람이 있을경우
	if( endvoiceallin && m_NowWinpnum == 0 ) SutdaGameSound().SetEndActionVoice( END_ALLIN );
		
	//////////////////////////////////////////////////////////////////////////
	//배팅대결이 끝까지 갔을경우 (포기승이 아닐경우)
 	if( m_GameOverResultData.bOpen )
	{			
		//승자표시이펙트
		CPoint ptEffecct = g_cUIData.m_listCardStartPos[nWinnerPNum];
		g_pGameView->m_AniQue.Ani_Insert(ANIMATION_WINLOSS, ptEffecct, 0, 0, 0, FALSE, NULL, NULL, 10 );


		int winnum = -1;	
		//내가 게임에 참여중이 아니거나 관전중일경우 승자와 같은 효과를 본다.
		if( g_PLAYER(0)->JoinState == EGAME_JOINOUT ||  g_ObserverMan.IsMyObserver() ) winnum = nWinnerPNum;
		else																		   winnum = 0;

		//족보에 따른 카드 오픈 애니메이션을 보여준다.
		int point = GetAniPoint( g_PLAYER(nWinnerPNum)->GetOrginalCard(0), g_PLAYER(nWinnerPNum)->GetOrginalCard(1));
		int losepoint = GetAniPoint(  g_PLAYER(0)->GetOrginalCard(0), g_PLAYER(0)->GetOrginalCard(1));

		//한끗차이로 승부가 났을경우
		CheckSortVoice(nWinnerPNum);

		char* sndkind = NULL;
		//승패에 따른 효과음 			
		//내가 이겼을때 효과음 
		//if( m_NowWinpnum == 0 )
		if( winnum == 0 )
		{
			
			if( point == 0)	sndkind = SND30;				//38광땡 
			else if(point == 1 || point == 2 )	sndkind = SND31;	//광땡 
			else if( point == 3 ) sndkind = SND32;					//장땡
			else if( point >= 4 && point <= 12 ) sndkind = SND33;	//삥땡 ~ 구땡
			else if( point >= 13 && point <= 18 ) sndkind = SND34;	//알리~세륙
			else if( point >= 19 && point <= 27) sndkind = SND35;	//1끗~갑오
			else if( point == 31) sndkind = SND31;//SND23;			//암행어사로 이길경우
			else if( point == 30) sndkind = SND33;//SND24;			//삼칠땡잡이

		}
		//내가 졌을때 효과음
		else
		{	
			if( losepoint == 1 || losepoint == 2) sndkind = SND36;	//광땡
			else if( losepoint == 3) sndkind = SND37;	//장땡
			else if( losepoint >= 4  && losepoint <= 12 ) sndkind = SND38;	//삥땡~구땡
			else if( losepoint >= 13 && losepoint <= 18 ) sndkind = SND39;	//알리~세륙
			else if( losepoint >= 19 && losepoint <= 27) sndkind = SND40;	//1끗~갑오
		}
		
		//족보결과에 따른 애니메이션 		
		int delay = 10;
		g_pGameView->m_AniQue.Ani_Insert(ANIMATION_RESULT_EFT, CPoint(362, 107), point, (nWinnerPNum==0), point, FALSE, sndkind, NULL, delay );

		//38에서 장땡까지 금가루 뿌려준다. 		
		if(point == 0) 
			g_pGameView->m_AniQue.Ani_Insert(ANIMATION_RESULT_EFT, CPoint(0,0) , 0, 0, 33, FALSE, NULL, NULL, delay + 10);
		else if(point == 1 || point == 2 || point == 3) 
			g_pGameView->m_AniQue.Ani_Insert(ANIMATION_RESULT_EFT, CPoint(0,0), 0, 0, 32, FALSE, NULL, NULL, delay + 10);
		//암행어사나 땡잡이일경우 애니메이션을 보여준다.
		else if( point == 30 ) 			
			g_pGameView->m_AniQue.Ani_Insert( ANIMATION_ETCEFFECT, g_cUIData.m_listParkmoonsuPos[nWinnerPNum], 0, 0, 1, FALSE, SND24, NULL, 10 );			
		else if( point == 31 ) 
			g_pGameView->m_AniQue.Ani_Insert( ANIMATION_ETCEFFECT, g_cUIData.m_listParkmoonsuPos[nWinnerPNum], 0, 0, 0, FALSE, SND23, NULL, 10 );
			
	}
	//포기에 의한 결과일 경우
	else
	{
		if (!g_ObserverMan.IsMyObserver() )
		{		
			SutdaGameSound().SetEndActionVoice( END_DRAW );
				
			//포기승일경우 족보확인 애니메이션이 안나오므로 이곳에서 나와준다.
			BOOL bMyWin  =  (BOOL) nWinnerPNum == 0;
			BOOL bMyFold = (BOOL) g_PLAYER(0)->bFold;    //내가 다이했나?
			SutdaGameSound().OnResultVoice(bMyWin, bMyFold);
		}
	}
	
	//승자표시를 찍는다.
	g_pGameView->m_AniQue.Ani_Insert( ANIMATION_WINLOSS, g_cUIData.m_listResultStampPos[nWinnerPNum], 0, 1, 2, FALSE, NULL, NULL, 10 );
	
	///패자 카드 찢기
	if ( m_GameOverResultData.bOpen )
	{
		for( i = 0; i < MAX_PLAYER; i++)
		{	
			if ( strlen(g_PLAYER(i)->UI.ID)==0 || g_PLAYER(i)->JoinState==EGAME_JOINOUT || g_PLAYER(i)->bFold )
				continue;
			if( i == nWinnerPNum ) 
				continue;

			//패한 사람은 카드가 찢어진다.(마음도 찢어지게 아프다.)
			g_pGameView->m_AniQue.Ani_Insert( ANIMATION_WINLOSS, g_cUIData.m_listCardStartPos[i], i, i, 1, FALSE, SND44, NULL, 30 );
		}
	}

	//축음기 볼륨이 줄었으면 볼륨을 켜준다.
	g_pGameView->m_Radiophonograph.SetFade( TRUE );

	//////////////////////////////////////////////////////////////////////////
	// 기권오픈 확인

	// 버튼 spr 인덱스가 0이면 본인 카드 오픈 On, 1 이면 Off
	bool bUIFoldOpen = false;
	if ( g_pGameView->m_cGameViewBtnMananager.m_btnCardOpen.GetButtonNumber() == 0 ) bUIFoldOpen = true;

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
			for ( int k = 0; k < 4; k++ )
			{
				if( g_Poker.BC[ i ].nCard[ k ] > 0 ) nCount++; // 닫힌 카드 정보
				if( g_Poker.PS[ i ].nCard[ k ] > 0 ) nCount++; // 열린 카드 정보
			}

			// 본인카드오픈 상태를 확인한다.( 2장게임 3장 게임에 따라 처리 방식이 달라진다. )
			if( CARD_TOTALCOUNT == 3 )
			{
				// 3장게임은 기본 1장 오픈이기 때문에 2장이 들어오면 본인카드오픈으로 간주한다.
				if( nCount > 1 ) 
				{
					// 게임방식에 따라 최종 보여지는 카드 갯수가 달라서 별도 예외처리
					if( m_GameOverResultData.bOpen )
					{
						// 정상 쇼다운 상태라면 기권 유저 정보만 초기화 해둔다( 승패 연출 문제 )
						if( m_cPlayer[ pnum ].bFold )
						{
							// 2018.09.27 이슈 발생으로 기권 오픈 기능 제거
							//m_cPlayer[ pnum ].bFoldOpen = TRUE;	
							m_cPlayer[ pnum ].ResetOrginalCard();

							// 3장게임 최종 2장 오픈( 직전 3장 게임중 선택된 2장 정보만 온다. )
							m_cPlayer[ pnum ].SetOrginalCard( 0, g_Poker.PS[ i ].nCard[ 0 ] );
							m_cPlayer[ pnum ].SetOrginalCard( 1, g_Poker.BC[ i ].nCard[ 0 ] );
							m_cPlayer[ pnum ].SetOrginalCard( 2, g_Poker.BC[ i ].nCard[ 1 ] );
						}
					}
					else
					{
						// 2018.09.27 이슈 발생으로 기권 오픈 기능 제거
						//m_cPlayer[ pnum ].bFoldOpen = TRUE;	
						m_cPlayer[ pnum ].ResetOrginalCard();

						// 3장게임 게임 중 기권 상태( 2 ~ 3장 까지 보여진다. 처음 1장은 오픈 상태 )
						m_cPlayer[ pnum ].SetOrginalCard( 0, g_Poker.PS[ i ].nCard[ 0 ] );
						m_cPlayer[ pnum ].SetOrginalCard( 1, g_Poker.BC[ i ].nCard[ 0 ] );
						m_cPlayer[ pnum ].SetOrginalCard( 2, g_Poker.BC[ i ].nCard[ 1 ] );
					}

					if( m_cPlayer[ pnum ].bFoldOpen ) m_cPlayer[ pnum ].CardResultOpen_FoldOpen(); // 본인카드 오픈시 로직처리( 자신의 족보 표시 및 카드음영 처리 )
				}
			}
			else
			{
				// 2장게임은 기권시 오픈하지 않기 때문에 카드정보가 1장이라도 있다면 입력해준다.
				if( nCount > 0 ) 
				{
					if( m_GameOverResultData.bOpen )
					{
						// 정상 쇼다운 상태라면 기권 유저 정보만 초기화 해둔다( 승패 연출 문제 )
						if( m_cPlayer[ pnum ].bFold )
						{
							// 2018.09.27 이슈 발생으로 기권 오픈 기능 제거
							//m_cPlayer[ pnum ].bFoldOpen = TRUE; 
							m_cPlayer[ pnum ].ResetOrginalCard();	

							// 3장게임 최종 2장 오픈( 직전 3장 게임중 선택된 2장 정보만 온다. )
							m_cPlayer[ pnum ].SetOrginalCard( 0, g_Poker.BC[ i ].nCard[ 0 ] );
							m_cPlayer[ pnum ].SetOrginalCard( 1, g_Poker.BC[ i ].nCard[ 1 ] );
						}
					}
					else
					{	
						// 2018.09.27 이슈 발생으로 기권 오픈 기능 제거
						//m_cPlayer[ pnum ].bFoldOpen = TRUE;	
						m_cPlayer[ pnum ].ResetOrginalCard();

						m_cPlayer[ pnum ].SetOrginalCard( 0, g_Poker.BC[ i ].nCard[ 0 ] );
						m_cPlayer[ pnum ].SetOrginalCard( 1, g_Poker.BC[ i ].nCard[ 1 ] );
					}

					if( m_cPlayer[ pnum ].bFoldOpen ) m_cPlayer[ pnum ].CardResultOpen_FoldOpen(); // 본인카드 오픈시 로직처리( 자신의 족보 표시 및 카드음영 처리 )
				}
			}
		}
	}
}


void CGameProcessSutda::OnResultGameOver()
{
	GM().SetWaitResponse(FALSE);

	g_RI.State		= EGAME_WAITING;
	m_bGameStart	= FALSE;			// 게임 시작 플래그 세팅	

	g_pGameView->m_cPlayerDrawManager.SetGameOver(true, timeGetTime());

	int nWinPlayerNum = GetPNum_ByServPN(m_GameOverResultData.nNowWinner[0]);
	if( nWinPlayerNum < 0 || nWinPlayerNum >= GM().GetMaxPlayer())
	{
		return;
	}

	//  PokerBase쪽 PlayerManger에 승자 번호를 넘겨준다.(서버에서 보내준 값을 넘겨줘야 한다.)
	g_pGameView->m_cPlayerDrawManager.SetWinnerPNum(m_GameOverResultData.nNowWinner[0]);


	// 사이드팟 승자 세팅
	if(g_RI.ChipKind==(int)ROOM_CHIPKIND_GOLD)
	{
		MAP_RANKING mapRanking;
		for(int i=0; i< GetMaxPlayer(); i++)
		{
			if(strlen(m_GameOverResultData.Ui[i].ID)>0) //  겜참여자.
			{
				int pnum = GetPlayerPNum(m_GameOverResultData.Ui[i].ID);
				if(pnum == -1) continue;
				if(m_GameOverResultData.Ui[i].nRank < 0) continue;

				mapRanking[m_GameOverResultData.Ui[i].nRank][pnum] = m_GameOverResultData.Ui[i].nResultMoney;
			}
		}
		g_pGameView->m_cSidePotMoney.SetGameOver(mapRanking);
		m_AllBetChip.SetWinner(&g_pGameView->m_cSidePotMoney);
	}


	//잭팟 공지나 이벤트 공지 (카드 펼친후에 나와야 하는 공지들)
	Draw_TextInformationMsg();

	// 채팅창에 결과를 출력한다.
	ResultInfoDisplay(nWinPlayerNum);
	
	if(!m_GameOverResultData.bOpen)
	{
		if((CPromotionManager::ENUM_PROMOTION_TYPE)GM().GetPromotionData()->nCnts[0] == CPromotionManager::EPT_POKER_2018_CHUSEOK)
		{
			MM().Call(UPG_CMK_SHOW_CHUSEOK_GOLDCOIN, WPARAM(0), LPARAM(0));
		}
		else
		{	
			MM().Call(UPG_CMK_SHOW_LUCKYWHEEL_POPUP, (WPARAM)0, (LPARAM)TRUE);
		}
	}
}


void CGameProcessSutda::ResultInfoDisplay(int nWinPlayerNum)
{
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

	int nWinnerServPNum =m_GameOverResultData.nNowWinner[IDX_SP_WINNER];
	if (nWinnerServPNum<0 || nWinnerServPNum>=MAX_PLAYER)
		return;

	int nMaxPlayer	= GetMaxPlayer();
	int wpnum		= GetPNum_ByServPN(nWinnerServPNum);
	if( !g_pGameView-> m_cPlayerDrawManager.IsOpen() ) // 승자 1명 (나머지 유저 폴드)
	{

		CString strResultMsg;
		CString stResultMoney = NMBASE::UTIL::g_MoneyMark(m_GameOverResultData.Ui[nWinnerServPNum].nResultMoney);
		stResultMoney.Append(strChip_Name[g_RI.ChipKind]);
		strResultMsg.Format("[%s]님 승리\n◆획득금액: %s\n◆족보: %s\n",
			m_GameOverResultData.Ui[nWinnerServPNum].NickName, 
			stResultMoney, 
			"기권승"
			);

		AddGameInfoViewEdit(strResultMsg, GetChatColor(ECC2_RESULT));

		
		// 잃은 금액을 표시한다.
		for(int i = 0 ; i < nMaxPlayer; ++i)
		{
			if(strlen(m_GameOverResultData.Ui[i].ID) > 0 && g_pGameView->m_cPlayerDrawManager.GetWinnerPNum() != i)
			{
				CString stLostMoney;
				INT64 lostmoney = m_GameOverResultData.Ui[i].nResultMoney;
				stLostMoney = (CString)NMBASE::UTIL::g_MoneyMark(lostmoney);
				stLostMoney.Append(strChip_Name[g_RI.ChipKind]);
				str.Format(" [%s]: ", m_GameOverResultData.Ui[i].NickName);
				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_LOSER));
				str.Format("%s\n", stLostMoney.GetBuffer());
				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_LOSEMONEY));
			}
		}		
	}
	else 
	{	
		//bts 족보 노출 관련 수정.
		//2012.7.26 류승민
		//오픈승
		CString strResultMsg;
		CString stResultMoney = NMBASE::UTIL::g_MoneyMark(m_GameOverResultData.Ui[nWinnerServPNum].nResultMoney);
		stResultMoney.Append(strChip_Name[g_RI.ChipKind]);
		strResultMsg.Format("[%s]님 승리\n◆획득금액: %s\n◆족보: %s\n",
			m_GameOverResultData.Ui[nWinnerServPNum].NickName, 
			stResultMoney, 
			m_strwinnerJokbo
			);

		// 이전 소스
		/*CString strResultMsg;
		strResultMsg.Format("[%s]님 승리\n◆획득머니: %s\n◆족보: %s\n",
		m_GameOverResultData.Ui[nWinnerServPNum].NickName, 
		NMBASE::UTIL::g_MoneyMark(m_GameOverResultData.Ui[nWinnerServPNum].nResultMoney), 
		g_PLAYER(wpnum)->GetJokboName()
		);*/
		
		AddGameInfoViewEdit(strResultMsg, GetChatColor(ECC2_RESULT));
		
		// 잃은 금액을 표시한다.
// 		int *rank = new int[nMaxPlayer];
// 		memset(rank, -1, sizeof(int) * nMaxPlayer);
// 		int rcnt = 4;
// 		for(int i = 0 ; i < nMaxPlayer; ++i)
// 		{
// 			if(strlen(m_GameOverResultData.Ui[i].ID) > 0 && g_pGameView->m_cPlayerDrawManager.GetWinnerPNum() != i)
// 			{				
// 				int nRank = m_GameOverResultData.Ui[i].nRank;
// 
// 				if( nRank > 0 && nRank <= nMaxPlayer )
// 				{
// 					rank[nRank-1] = i;				
// 				}
// 				else if( nRank == -1)
// 				{
// 					rank[rcnt] = i;
// 					rcnt --;
// 					if(rcnt < 0)
// 					{
// 						rcnt =0;
// 					}
// 				}
// 			}		
// 		}
// 
// 		
// 		for(int i = 0 ; i < nMaxPlayer ; ++i)
// 		{
// 			if(rank[i] > -1 && rank[i] < nMaxPlayer)
// 			{			
// 				if(strlen(m_GameOverResultData.Ui[rank[i]].ID) > 0 && g_pGameView->m_cPlayerDrawManager.GetWinnerPNum() != rank[i])
// 				{
// 					CString stLostMoney;
// 					INT64 lostmoney = m_GameOverResultData.Ui[rank[i]].nResultMoney;
// 					stLostMoney = (CString)NMBASE::UTIL::g_MoneyMark(lostmoney);
// 					str.Format(" [%s]: ",m_GameOverResultData.Ui[rank[i]].NickName);
// 					AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_LOSER));
// 					str.Format("%s\n",stLostMoney.GetBuffer());
// 					AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_LOSEMONEY));
// 				}
// 			}
// 		}
// 
// 		delete [] rank;

		for(int i = 0 ; i < MAX_PLAYER ; ++i)
		{
			if (i==m_GameOverResultData.nNowWinner[IDX_SP_WINNER])		//승자 제외
				continue;
				
			if(strlen(m_GameOverResultData.Ui[i].ID) > 0 )
			{
				CString stLostMoney;
				INT64 lostmoney = m_GameOverResultData.Ui[i].nResultMoney;
				stLostMoney = (CString)NMBASE::UTIL::g_MoneyMark(lostmoney);
				stLostMoney.Append(strChip_Name[g_RI.ChipKind]);
				str.Format(" [%s]: ",m_GameOverResultData.Ui[i].NickName);
				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_LOSER));
				str.Format("%s\n",stLostMoney.GetBuffer());
				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_LOSEMONEY));
			}
		}
	}
}


// 삥 버튼
void CGameProcessSutda::SendPingButton( bool bUserAction )
{
	if(g_ObserverMan.IsMyObserver() == TRUE) 
		return;

	if( !g_pGameView->m_cGameViewBtnMananager.m_bBPing || g_PLAYER(0)->PlayState == EPLAYER_GAMEEND)
		return;


	// 타임아웃 카운트 초기화
	if (bUserAction)
	{
		m_cPlayer[ 0 ].ResetTurnLimitCnt();
	}

	// 따당이 예약되어 있어도 상태방이 체크를 선택해서 따당이 실패하면 예약 취소를 위해서 넣어줌.
	SetReserveButton(RESERVESTATE_BET_NONE);

	g_PLAYER(0)->ClearEffectSpr(ANF_PLAYER_TIMEBAR);
	//g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();

	POKERCLNT pc; //MakeBasic(pc);
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 3; // 배팅
	pc.PlayTick = g_pGameView->GetCurrentPlayTick(); 
	
	pc.nBtn   = BET_CMD_PING ; // 삥

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  g_PLAYER(0)->UI.UNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.ID, &pc);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), raise.pData, raise.GetTotalSize());

	g_pGameView->m_cGameViewBtnMananager.SetButtonClick(true);
}

// 따당 -> 풀버튼 기능으로 변경(규제안관련 작업 2014.01)
void CGameProcessSutda::SendDdaButton( bool bUserAction )
{
	if(g_ObserverMan.IsMyObserver() == TRUE) 
		return;
	if( !g_pGameView->m_cGameViewBtnMananager.m_bBDDa || g_PLAYER(0)->PlayState == EPLAYER_GAMEEND) 
		return;


	// 2014.2.7 crpark
	// 규제안 베팅예약 공통화
	if(false == CheckMyTurn(GetPlayer(0)->ServPNum, m_nCurPlayer))
	{
#if defined(BUTTON_CHANGE_FULLTODADANG)
		if(true==GM().IsChangeFullDDang()){
			SetReserveButton(RESERVESTATE_BET_BET_CMD_FULL);
		}
		else{
			SetReserveButton(RESERVESTATE_BET_BET_CMD_DADANG);
		}
#else
		SetReserveButton(RESERVESTATE_BET_BET_CMD_DADANG);
#endif
		return;
	}
	SetReserveButton(RESERVESTATE_BET_NONE);

	// 타임아웃 카운트 초기화
	if (bUserAction)
	{
		m_cPlayer[ 0 ].ResetTurnLimitCnt();
	}


	g_PLAYER(0)->ClearEffectSpr(ANF_PLAYER_TIMEBAR);

	//g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();

	POKERCLNT pc; //MakeBasic(pc);
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 3; // 배팅
	//pc.nBtn   = BET_CMD_DADANG; // 따당
	pc.PlayTick = g_pGameView->GetCurrentPlayTick(); 

#if defined(BUTTON_CHANGE_FULLTODADANG)
	if(true==g_RI.bIsFriendRoom && g_PLAYER(0)->UI.GetMoney() >= GM().GetLossMaxMoneyForGame()){
		pc.nBtn   = BET_CMD_MAX_BET;
	}
	else if(true==GM().IsChangeFullDDang()){
		pc.nBtn   = BET_CMD_FULL;
	}
	else{
		pc.nBtn   = BET_CMD_DADANG;//BET_CMD_FULL;//BET_CMD_DADANG ; // 따당에서 풀로교체(규제안관련 작업 2014.01)->추후작업
	}
#else
	pc.nBtn   = BET_CMD_DADANG;//BET_CMD_FULL;//BET_CMD_DADANG ; // 따당에서 풀로교체(규제안관련 작업 2014.01)->추후작업
#endif

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  g_PLAYER(0)->UI.UNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.ID, &pc);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), raise.pData, raise.GetTotalSize());
	g_pGameView->m_cGameViewBtnMananager.SetButtonClick(true);
}

// 하프
void CGameProcessSutda::SendHalfButton( bool bUserAction )
{
	if( g_ObserverMan.IsMyObserver() == TRUE ) return;
	if( !g_pGameView->m_cGameViewBtnMananager.m_bBMax || g_PLAYER(0)->PlayState == EPLAYER_GAMEEND ) return;


	// 2014.2.7 crpark
	// 규제안 베팅예약 공통화
	if(false == CheckMyTurn(GetPlayer(0)->ServPNum, m_nCurPlayer))
	{
		SetReserveButton(RESERVESTATE_BET_BET_CMD_HALF);
		return;
	}
	SetReserveButton(RESERVESTATE_BET_NONE);

	// 타임아웃 카운트 초기화
	if (bUserAction)
	{
		m_cPlayer[ 0 ].ResetTurnLimitCnt();
	}


	int spnum       = GetServPNum_ByPN(0);
	INT64 CallMoney = GetRaiseBat() - g_Poker.RU[spnum].nNowBat;
	INT64 nTotal    = GetRealBet();

	g_PLAYER(0)->ClearEffectSpr(ANF_PLAYER_TIMEBAR);
	//g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();

	POKERCLNT pc; //MakeBasic(pc);
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 3; // 배팅


	if( g_PLAYER(0)->GetNowMoney() > CallMoney && g_PLAYER(0)->GetNowMoney() < CGameProcess::GetMaxMoney( nTotal ) )
	{
		pc.nBtn   = BET_CMD_ALLIN; // 맥스
	} 
	else
	{
		pc.nBtn   = BET_CMD_HALF; // 맥스
	}

	pc.PlayTick = g_pGameView->GetCurrentPlayTick(); 


	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  g_PLAYER(0)->UI.UNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.ID, &pc);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), raise.pData, raise.GetTotalSize());
	g_pGameView->m_cGameViewBtnMananager.SetButtonClick(true);
}

// 다이
void CGameProcessSutda::SendDieButton( bool bUserAction )
{
	if(g_ObserverMan.IsMyObserver() == TRUE)
		return;
	if(!g_pGameView->m_cGameViewBtnMananager.m_bFold || g_PLAYER(0)->PlayState == EPLAYER_GAMEEND) 
		return;

	if( g_Poker.nAutoBetCmd > 0 && g_RI.nAutoBetKind > 0 )
	{
		return;
	}

	// 2014.2.7 crpark
	// 규제안 베팅예약 공통화
	if(false == CheckMyTurn(GetPlayer(0)->ServPNum, m_nCurPlayer))
	{
		SetReserveButton(RESERVESTATE_BET_BET_CMD_FOLD);
		return;
	}
	SetReserveButton(RESERVESTATE_BET_NONE);
	CanReserve(false);

	// 타임아웃 카운트 초기화
	if (bUserAction)
	{
		m_cPlayer[ 0 ].ResetTurnLimitCnt();
	}

// 	CString szTemp;
// 	szTemp.Format("CGameProcessSutda::SendDieButton() : 유저ID:%s \n", g_PLAYER(0)->UI.ID);
// 	AddGameChatViewEdit(szTemp.GetBuffer(), GetChatColor(ECC2_RESULT_JOKBO));

	g_PLAYER(0)->ClearEffectSpr(ANF_PLAYER_TIMEBAR);

	//g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();

	POKERCLNT pc; 
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 0; // 게임포기
	pc.nBtn   = 0; // 삥
	pc.PlayTick = g_pGameView->GetCurrentPlayTick(); 

	CCL_ASK_FOLD cDie;
	cDie.Set(g_RI.RoomNum,  g_PLAYER(0)->UI.UNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.ID, &pc);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), cDie.pData, cDie.GetTotalSize());
	g_pGameView->m_cGameViewBtnMananager.SetButtonClick(true);
}

// 체크
void CGameProcessSutda::SendCheckButton( bool bUserAction )
{
	if(g_ObserverMan.IsMyObserver() == TRUE) 
		return;
	if( !g_pGameView->m_cGameViewBtnMananager.m_bBCheck || g_PLAYER(0)->PlayState == EPLAYER_GAMEEND) 
		return;

	// 타임아웃 카운트 초기화
	if (bUserAction)
	{
		m_cPlayer[ 0 ].ResetTurnLimitCnt();
	}

	g_PLAYER(0)->ClearEffectSpr(ANF_PLAYER_TIMEBAR);
	//g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();

	POKERCLNT pc; //MakeBasic(pc);
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 1; // 배팅
	pc.nBtn   = 0; // 삥
	pc.PlayTick = g_pGameView->GetCurrentPlayTick(); 

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  g_PLAYER(0)->UI.UNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.ID, &pc);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), raise.pData, raise.GetTotalSize());
	g_pGameView->m_cGameViewBtnMananager.SetButtonClick(true);
}

// 콜
void CGameProcessSutda::SendCallButton( bool bUserAction )
{
	if(g_ObserverMan.IsMyObserver() == TRUE) 
		return;
	if( !g_pGameView->m_cGameViewBtnMananager.m_bBCall || g_PLAYER(0)->PlayState == EPLAYER_GAMEEND)
		return;

	// 2014.2.7 crpark
	// 규제안 베팅예약 공통화
	if(false == CheckMyTurn(GetPlayer(0)->ServPNum, m_nCurPlayer))
	{
		SetReserveButton(RESERVESTATE_BET_BET_CMD_CALL);
		return;
	}
	SetReserveButton(RESERVESTATE_BET_NONE);

	// 타임아웃 카운트 초기화
	if (bUserAction)
	{
		m_cPlayer[ 0 ].ResetTurnLimitCnt();
	}

	g_PLAYER(0)->ClearEffectSpr(ANF_PLAYER_TIMEBAR);
	//if(	!m_bOnGame || !m_bTurn || m_nCardWindDelay>0 ){ SetBtn(); return; }
	//m_bTurn = FALSE;
	//m_nTime = 15;
	//m_bTime = FALSE;
	
	//g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();

	POKERCLNT pc; 
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 2; // 배팅
	pc.nBtn   = 0; // 삥
	pc.PlayTick = g_pGameView->GetCurrentPlayTick(); 

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  g_PLAYER(0)->UI.UNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.ID, &pc);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), raise.pData, raise.GetTotalSize());
	g_pGameView->m_cGameViewBtnMananager.SetButtonClick(true);
}

//G_TFT
void CGameProcessSutda::Change_Money(CSV_CHANGE_MONEY *pMsg)
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
	if(strcmp(pUserID, GM().GetMyInfo()->UI.ID) == 0) 
	{
		/*GM().GetMyInfo()->UI.SetMoney(PMoney);*/
		GM().GetMyInfo()->UI.SetRealPMoney(PMoney);
		if( !g_ObserverMan.IsMyObserver() ) 
		{
			/*g_PLAYER(0)->UI.SetMoney(PMoney);*/
			g_PLAYER(0)->UI.SetRealPMoney(PMoney);
			//g_PLAYER(0)->PrevMoney = g_PLAYER(0)->UI.GetPMoney();//PMONEY , PMONEY2 통합시에 아래 살리기 //@#$%
		}
	}
	// 상대방 머니 처리
	else	
	{
		int pnum = g_GAMEPROCESS()->GetPlayerPNum(pUserID);
		if(pnum != -1) 
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

	if( strUMCode == "ABB02019" || strUMCode == "ABB02020" || strUMCode == "ABB02021" ) //원샷(재기)충전 메타코드 
	{
		//원샷(재기)충전
		g_cOneShotCharge.OnChargeMoney(*pMsg);
	}


	if(!sysmsg.IsEmpty())
	{
		if(GetCurrentWhere()==IDX_GAMEWHERE_LOBY)
		{
			//UPG작업 추가
			AddLobbyChatViewEdit(sysmsg, RGB(0,44,93));
		}

		if(GetCurrentWhere()==IDX_GAMEWHERE_GAME)
		{			
			AddGameInfoViewEdit(sysmsg.GetString(), GetChatColor(ECC2_SERVMSG));
		}
	}
	if(GetCurrentWhere()==IDX_GAMEWHERE_LOBY){
		MM().Call(UPG_CMK_RESET_GAMEMONEY, 0, 0);
	}
}


// bPaticipation = TRUE  관전 -> 참여
// bPaticipation = FALSE 참여 -> 관전
void CGameProcessSutda::SetPlayInfo(int nowpnum, int newpnum, BOOL bParticipation )
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

	if( !bParticipation ) 
	{
		//현재 겜참여 중이기 때문에 0 번 인덱스
		//기본 정보 초기화
		m_cPlayer[0].Clear();
	}	

	// 기존 플레이 정보 backup
	int nIndex = 0;
	for(int i = 0; i < MAX_PLAYER; ++i) 
	{
		if(strlen(m_cPlayer[i].UI.ID) == 0) 
		{
			continue;
		}

		if(bParticipation )// 관전 -> 참여
			nIndex = i;
		else
			nIndex = GetServPNum_ByPN(i);

		memcpy(&tmpUi[nIndex], &m_cPlayer[i].UI, sizeof(USERINFO));
		memcpy(&tmpCUi[nIndex], m_cPlayer[i].GetChangeRoomInfo(), sizeof(CHANGE_USERINFO_ROOM) );
		tmpPs[nIndex].JoinState = m_cPlayer[i].JoinState;
		tmpPs[nIndex].PlayState = m_cPlayer[i].PlayState;
		tmpPs[nIndex].bFold		= m_cPlayer[i].bFold;
		tmpPs[nIndex].nCardTotal= m_cPlayer[i].GetCardTotal();

		/*char szTemp1[256] = {0, };
		sprintf(szTemp1, "------ SetPlayInfo :: PlayerIndex: %d JoinState:%d PlayState:%d \n", nIndex, m_cPlayer[i].JoinState, m_cPlayer[i].PlayState);
		OutputDebugString(szTemp1);*/

		//서버로부터 다시 받는다
		// ### [ 중복된 IP가 ] ###
		//tmpPs[nIndex].nCheckIp = m_cPlayer[i].MyIpCheck;
		// ### [사운드 추가 작업] ###
		tmpPs[nIndex].nSndFxKind = m_cPlayer[i].nSndFxKind;
		tmpPs[nIndex].nOpenTotal= m_cPlayer[i].GetOpenCardCount();

		tmpSelectCard[nIndex] = g_pGameView->m_cPlayerDrawManager.IsSelectCard(i);

		int tcnt = GetTotalCardNum();
		for(int k = 0; k < tcnt ; ++k) 
		{
			tCardDeck[nIndex].nCard[k] = m_cPlayer[i].m_cMyCard[k].GetCardNum();
		}
	}

	//기본 정보 초기화
	if(bParticipation)
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
	for(int i=0; i<MAX_PLAYER; ++i) 
	{
		m_cPlayer[i].Clear();
		m_cPlayer[i].ServPNum = GetServPNum_ByPN(i); // 초기화 되지 않는다.
		m_cPlayer[i].InitRoomData();
	}


	// 관전 -> 참여
	if(bParticipation)
	{	
		GM().GetMyInfo()->UI.cObserver = 0;
		g_ObserverMan.SetMyObserver(FALSE);
		GM().GetMyInfo()->Reset();
		GM().GetMyInfo()->nSndFxKind = g_Config.SndFxKind;
		m_cPlayer[0].SetNewPlayer(&GM().GetMyInfo()->UI);
		m_cPlayer[0].SetChangeRoomInfo(*GM().GetMyInfo()->GetChangeRoomInfo());
		m_cPlayer[0].nSndFxKind = g_Config.SndFxKind;

		g_RI.NowUserNum++;
		if(g_RI.NowObserverNum > 0) 
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
		if(g_RI.NowUserNum > 0) 
			g_RI.NowUserNum--;
		
		//관전UI		
		g_ObserverMan.OnObserve();
	}

	// 플레이어 순서에 맞게  카드를 초기화한다
	for(int i = 0; i < MAX_PLAYER; ++i) 
	{
		int nServerToClientIndex = GetPNum_ByServPN(i);
		if(strlen(tmpUi[i].ID) > 0)	
		{
			if(bParticipation ) 
			{
				if(nServerToClientIndex != 0) 
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

			g_pGameView->m_cPlayerDrawManager.SetSelectCard(nServerToClientIndex, tmpSelectCard[i]);

			/*char szTemp1[256] = {0, };
			sprintf(szTemp1, "------ SetPlayInfo2 :: PlayerIndex: %d JoinState:%d PlayState:%d \n", nServerToClientIndex, tmpPs[i].JoinState, tmpPs[i].PlayState);
			OutputDebugString(szTemp1);*/ 

			if( g_Poker.nState < RSTATE_RESULT )
			{
				// 게임 진행 상황 그릴것 카드 정보를 카드덱에다가 셋
				if(m_cPlayer[nServerToClientIndex].JoinState == EGAME_JOININ) 
				{
					int nTotCnt = tmpPs[i].nCardTotal;
					int nOpCnt  = tmpPs[i].nOpenTotal;

					m_cPlayer[nServerToClientIndex].bFold = tmpPs[i].bFold;
					m_cPlayer[nServerToClientIndex].SetCardTotal(nTotCnt);
					m_cPlayer[nServerToClientIndex].SetOpenCardCount(nOpCnt);

					for(int k = 0; k < nTotCnt; ++k) 
					{
						m_cPlayer[nServerToClientIndex].SetAddCard(g_nDUMY_CARD); // 카운트 자동증가
						//CPoint pt;
						//pt.x = m_cPlayer[nServerToClientIndex].m_cMyCard[k].GetPos().x;
						//pt.y = m_cPlayer[nServerToClientIndex].m_cMyCard[k].GetPos().y;
						//m_cPlayer[nServerToClientIndex].m_cMyCard[k].GetMove().SetCurPos(pt);
						m_cPlayer[nServerToClientIndex].m_cMyCard[k].SetMoveEnd(true);
						m_cPlayer[nServerToClientIndex].ShowCard(k);						
					}				

					if(g_Poker.nState > RSTATE_DISTRIBUTECARD && nOpCnt > 0 && !m_cPlayer[nServerToClientIndex].bFold)
					{
						for(int k = 0; k < nTotCnt; ++k) 
						{
							m_cPlayer[nServerToClientIndex].m_cMyCard[k].SetCardNum(tCardDeck[i].nCard[k]);
							m_cPlayer[nServerToClientIndex].SetCardFrontShow(k);
						}	
					}
				}
			}			
		}
	}

	for(int i = 0; i < MAX_PLAYER; ++i) 
	{
		if(m_cPlayer[i].UI.ID[0] != 0)	
		{
			g_pGameView->m_cPlayerDrawManager.OnEnterRoom(i);
		}
	}


	// bPaticipation = TRUE  관전 -> 참여
	// bPaticipation = FALSE 참여 -> 관전
	if(bParticipation) 
	{
		CObserverDlg *pObserverDlg = GetObserverDlg();
		if(pObserverDlg)
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
		if(pObserverDlg)
		{
			pObserverDlg->User_InsItem(ounum,m_cPlayer[newpnum].UI.ID, m_cPlayer[newpnum].UI.NickName, sex, level, Money, seq);
		}
	}

	CString str;
	if(bParticipation) 
	{
		str.Format("관전 → 참여\n");
	}
	else 
	{
		str.Format("참여 → 관전\n");
	}
	
	AddGameInfoViewEdit(str.GetString(), RGB(153,153,255));			

	// 오토배팅 셋팅
	if(!g_ObserverMan.IsMyObserver())
	{		
		AutoBetSetUpWnd().OnEnterRoom();
	}
}

BOOL CGameProcessSutda::IsPlayingNow()
{
	return m_bGameStart;
}

//UPG작업 추가
void CGameProcessSutda::OnEnterRoom(BOOL bCreation)
{
	//내가 방에 들어간다 (bCreation은 방을 생성해서 입장하는지 여부)
	
	// 본인 카드 오픈 버튼 상태 Off로 초기화(0이면 On, 1이면 Off)
	g_pGameView->m_cGameViewBtnMananager.m_btnCardOpen.SetSprite(&(g_pGameView->m_cGameViewBtnMananager.m_sprUICardOpenBtn), 1);

	CString str;	
	#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
		if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
			str.Format("톡톡 섯다");
		else
			str.Format("윈조이 섯다");
	#else //UPGRADE_20120308_TOCTOC_CHANNELING
		str.Format("윈조이 섯다");
	#endif //UPGRADE_20120308_TOCTOC_CHANNELING

	g_pGameView->SetWindowText(str);

	str.Format("☆도움말☆\n1. 게임창->채팅창 포커스 이동\n단축키 - Enter, ESC\n2. 채팅창->게임창 포커스 이동\n단축키 - ESC\n");
	g_pChatWinMan->ResetChatting_STR();	
	AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

	g_pGameView->Reset();
	g_pGameView->m_cBackGround.OnEnterRoom();
	g_pGameView->m_cGameViewBtnMananager.InitObserverBtn();

	g_pChatWinMan->SetViewPage();

	str = " 비정상적인 게임행위(짜고치기) 및 사행성 조장 행위(거래,광고)등이 발생할 시 게임 내 신고 버튼을 이용하여, 신고해 주시기 바랍니다.\n";	
	AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

	//자리지킴이
	g_SeatKeeper.OnEnterGame();
	g_ObserverMan.OnEnterRoom();

	//보유한도
	g_cOwnLimit.OnEnterRoom(&g_cUIData, g_GAMEPROCESS());

	//관전자 채팅 경고
	m_bObserverChatWarning = FALSE;

	//사이드팟
 	if(g_pGameView)
 	{
 		g_pGameView->m_cSidePotMoney.Reset();
 	}

	// 전광판에 내용 출력
	if(!g_ObserverMan.IsMyObserver())
	{
		CheckMinimumUser();
	}	

	CGameProcess::OnEnterRoom(bCreation);

	g_pGameView->m_cGameViewBtnMananager.SetBetBtnType( g_Config.eBetBtnType, true );
	g_pGameView->m_cGameViewBtnMananager.EnterRoom_Button_Init( g_RI.sBetRuleKind );
	g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();

	// 포커스
	g_pGameView->SetFocus();

	if(g_ObserverMan.IsMyObserver())
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
	if(GetJackPotMoney() <= 0)
	{
		JackPotMoneyUpdate(&g_JackPotMoneyInfo);
	}


	//축음기를 켜준다
	g_pGameView->m_Radiophonograph.ReStart();

	g_cPromotionMan.OnEnterRoom();
}

// 방에서 나간다.
void CGameProcessSutda::OnExitRoom()
{
	CGameProcess::OnExitRoom();

	g_ObserverMan.SetMyObserver(FALSE);

	for(int i = 1; i < GetMaxNewPlayer(); ++i) 
	{
		g_PLAYER(i)->Clear();
	}
	
	// 방 목록 및 사용자 목록 리스트를 요구	
	g_pGameView->m_cAniSprListManager.Destroy();
	g_pGameView->m_cStayAniSprListManager.Destroy();
	g_pGameView->m_cTotalBetMoney.Reset();

	// 자리 지킴이
	g_SeatKeeper.OnOutRoom();

	g_pChatWinMan->ResetChatting_STR();

	// 섯다 인게임 배경음 끄기
	NMBASE::SOUND::g_Sound.Stop(SND_HIDDEN_BGM);
	
	m_i64JackPotMoney = 0;
	m_i64OldJackPotMoney = 0;
	
	// 섯다 애니메이션
	g_pGameView->m_AniQue.Reset();
	g_pGameView->m_Radiophonograph.SetOnOff(FALSE);
	g_pGameView->m_Radiophonograph.On_OffBoard(false);

	//보유한도
	g_cOwnLimit.OnExitRoom();	
}




//UPG작업 추가 (기존 GameDlg::PreTranslateMessage() 에 있던 부분을 가져옴)
BOOL CGameProcessSutda::PreTranslateMessage(MSG* pMsg) 
{
	///////////////////////////////////////////////////////////////
	if(pMsg->message == WM_SYSKEYDOWN)
	{
		if((pMsg->lParam & 0x20000000) > 0 && (pMsg->wParam == '1' || pMsg->wParam == '2' || pMsg->wParam == '3' ||	 pMsg->wParam == '4' || pMsg->wParam == '5')) 
		{			
			int num = (pMsg->wParam - 49)+5;

			if(num >= 0 && num < 10)
			{
				CString str = g_ChatMacro.Macrostr[num];
				g_pChatWinMan->SystemKeyReturn(str);
			}
		}		
	}

	if(pMsg->message == WM_MOUSEWHEEL) // 채팅창에서 마우스 휠
	{
		if( g_pChatWinMan != NULL && g_pChatWinMan->GetChatView() != NULL )
		{
			g_pChatWinMan->GetChatView()->OnMouseWheel(pMsg->wParam, pMsg->lParam);			
		}		
	} 
	///////////////////////////////////////////////////////////////
	else if(pMsg->message == WM_KEYDOWN) 
	{			
		#ifdef _DEBUG
		TestCode(pMsg);
		#endif
		

		// 센드 에디트가 아닌 다른곳에 포커스가 있다면 게임키로 인식
		//if(g_pGameView->GetFocus() != (CWnd*)g_pChatWinMan->GetSendEdit()->pSendEdit)
		if(!g_pChatWinMan->IsEditFocus())
		{
			//////////
			// 숫자키패드 게임키 추가 2004.07.21
			int scankey = 0, extended = 0, previous = 0;
			BOOL bGameKey = IsSacnCode(pMsg->lParam, scankey, extended, previous);

			//사구 재경기 안내창이 최우선 노출
			if( g_pGameView->m_cAskRematchPopup.OnGameKey(scankey, extended, previous) )
				return TRUE;

			if((pMsg->wParam == VK_RETURN && extended == 0 )|| pMsg->wParam == VK_ESCAPE) 
			{			
				// 채팅창에 포커스를 옮긴다
				// 0 : eCHATVIEW::eCHAT
				if( g_pChatWinMan->GetViewMode() == 0 )
				{
					g_pChatWinMan->GetSendEdit()->pSendEdit->SetFocus();
					return TRUE;
				}
			}		

			if(GetCurrentWhere() == IDX_GAMEWHERE_GAME)
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
			if(GetAsyncKeyState(VK_LCONTROL) & 0x8000) bPushCtrl = true;
			if(GetAsyncKeyState(VK_LSHIFT) & 0x8000) bPushShift = true;

			if(bPushCtrl)
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
			if(strlen(str) == 0 && (pMsg->wParam == VK_UP || pMsg->wParam == VK_SPACE||
				pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_LEFT || pMsg->wParam == VK_DOWN ||
				pMsg->wParam == VK_INSERT || pMsg->wParam == VK_HOME || pMsg->wParam == 33 ||
				pMsg->wParam ==  VK_DELETE ||pMsg->wParam == VK_END ||pMsg->wParam ==  34 ||
				(pMsg->wParam == VK_RETURN && extended == 1 ) || 
// 				pMsg->wParam == VK_NUMPAD0 || pMsg->wParam == VK_NUMPAD1 || pMsg->wParam == VK_NUMPAD2 ||
// 				pMsg->wParam == VK_NUMPAD3 || pMsg->wParam == VK_NUMPAD4 || pMsg->wParam == VK_NUMPAD5 || 
// 				pMsg->wParam == VK_NUMPAD6 || pMsg->wParam == VK_NUMPAD7 || pMsg->wParam == VK_NUMPAD8 ||
				pMsg->wParam == VK_DECIMAL ))
			{
				g_pGameView->SetFocus();
				if(GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				{					
					OnGameKey( scankey, extended, previous ); //수정
				}

				/*if(pMsg->wParam == VK_SPACE && g_RI.State == EGAME_WAITING)
				{
					g_pGameView->SendMessage(WM_COMMAND,IDM_GAMESTART,0);
					return TRUE;
				}*/

				return TRUE;
			}

			// 대화 내용이 있어도
			// 채팅 창에서 예외 키
			if(pMsg->wParam == VK_INSERT || pMsg->wParam == VK_HOME || pMsg->wParam == VK_PRIOR ||
				pMsg->wParam ==  VK_DELETE ||pMsg->wParam == VK_END ||pMsg->wParam ==  VK_NEXT ||
				pMsg->wParam == VK_TAB || pMsg->wParam == VK_F2 || pMsg->wParam == VK_F3    ||
				pMsg->wParam == VK_F4 || pMsg->wParam == VK_F5 || pMsg->wParam == VK_F6   ||
				pMsg->wParam == VK_F7 || pMsg->wParam == VK_F8 || pMsg->wParam == VK_DOWN  ||
				(pMsg->wParam == VK_RETURN && extended == 1 ) || 
// 				pMsg->wParam == VK_NUMPAD0 || pMsg->wParam == VK_NUMPAD1 || pMsg->wParam == VK_NUMPAD2 ||
// 				pMsg->wParam == VK_NUMPAD3 || pMsg->wParam == VK_NUMPAD4 || pMsg->wParam == VK_NUMPAD5 || 
// 				pMsg->wParam == VK_NUMPAD6 || pMsg->wParam == VK_NUMPAD7 || pMsg->wParam == VK_NUMPAD8 ||
				pMsg->wParam == VK_DECIMAL )
			{
				if(GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				{
					OnGameKey( scankey, extended, previous ); //수정
				}
				return TRUE;
			}

			if(GM().GetMyInfo()->PlayState == EPLAYER_GAMEPLAYING && g_Poker.nRound == 4 && pMsg->wParam == VK_UP  )
			{
				if(GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				{
					OnGameKey( scankey, extended, previous ); //수정
				}
				return TRUE;
			}

			if(pMsg->wParam == VK_ESCAPE)
			{
				// 게임뷰에 포커스를 옮긴다
				//프로필 창을 닫음
				//		g_pGameView->m_cAniSprListManager.Del_AniListOBj(ANF_SDLG_PROFILE);
				g_pGameView->SetFocus();
				return TRUE;
			}
		}			
	}
	else if( pMsg->message == WM_KEYUP )
	{		
		// 숫자키패드 게임키 추가 2004.07.21
		int scankey = 0, extended = 0, previous = 0;
		BOOL bGameKey = IsSacnCode(pMsg->lParam, scankey, extended, previous);

		if(GetCurrentWhere() == IDX_GAMEWHERE_GAME)
		{
			if( scankey != 0 )
			{
				g_SelectCard().OnGameKey( scankey, extended, previous );		
			}
		}
	}

	return FALSE;
}

//UPG작업 함수추가
BOOL CGameProcessSutda::IsSacnCode(int nScanCode, int &r_scankey, int &r_extended, int &r_previous)
{
	// 스캔코드를분석함
	// 스캔키코드
	r_scankey = (nScanCode >> 16) & 0x000000FF; 
	
	// 확장키인지여부(1이면확장키임- 숫자키패드방향키는비확장키보드임)
	r_extended = (nScanCode >> 24) & 0x00000001; 
	
	// 키의이전상태(1이면이전에눌려진상태임)
	r_previous = (nScanCode >> 30) & 0x00000001; 

	// 게임에서사용하는키인가검사
	if( ( r_scankey == 79 && r_extended == 0 ) || // 키패드1		가로삥		| 세로따당
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

void CGameProcessSutda::OnInit()
{
	set_GameSpeed();
}

void CGameProcessSutda::set_GameSpeed()
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


BOOL CGameProcessSutda::OnCommand(WPARAM wParam, LPARAM  lParam)
{
	g_pGameView->m_cChatWindow.OnCommand(wParam);

 	if( g_pGameView->m_cSidePotMoney.OnCommand(wParam,lParam) )
 		return TRUE;

	if( g_ObserverMan.OnCommand(wParam, lParam) )
	{
		if (wParam==IDM_EMERGE)
		{
			if(strcmp(g_RI.ID, GM().GetMyInfo()->UI.ID) == 0)  //내가 방장일때 관전
			{
				//사운드작업 ( " 아~ 방장은 관전안돼지~ ")
				SutdaGameSound().OnActionVoice(AV_WATCHSUBSCRIPT, 0, 0,  4);					
			}
		}
		return TRUE;
	}

	if( g_pGameView->m_cGameViewBtnMananager.OnCommand(wParam,lParam))
	{
		if (wParam==IDM_OPTION)
		{
			//볼륨세팅
			GM().GetCurrentGame()->SetBGMVolume(g_Config.BGMVolume);

			//게임설정 완료후 섯다 보이스 세팅
			SutdaGameSound().SetMyVoiceType(GM().GetMyInfo()->UI.Sex, g_Config.SndFxKind);

			//설정된 나의 보이스 타입을 서버로 보낸다.
			CPlayer *pPlayer = GM().GetMyInfo();
			if (pPlayer)
			{
				CSV_ASK_CHANGESNDFX msg;
				msg.Set(pPlayer->UI.UNum, pPlayer->ServPNum, g_RI.RoomNum, pPlayer->UI.ID, g_Config.SndFxKind);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			}
		}

		return TRUE;
	}

	switch(wParam)
	{		

	case IDM_BADUSER:
		{
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_CHATWINDOW_CHATTING_REPORT);
			GM().BadUserReportGameChat();
		}
		break;
		/*=====================//베팅 커멘드=====================*/
	case IDM_DIE:
		{
			//AddGameChatViewEdit("CGameProcessSutda::OnCommand()", GetChatColor(ECC2_RESULT_JOKBO));
			this->SendDieButton(true);
		}
		break;

	case IDM_CHECK:
		{
			this->SendCheckButton(true);		
		}
		break;

	case IDM_CALL:
		{
			this->SendCallButton(true);
		}
		break;

	case IDM_PING:
		{
			this->SendPingButton(true);
		}
		break;

	case IDM_DADANG:
		{
			this->SendDdaButton(true);
		}
		break;	

	case IDM_FULL:
		{
			this->SendDdaButton(true);
		}
		break;	

	case IDM_HALF:
		{
			this->SendHalfButton(true);
			g_pGameView->m_cGameViewBtnMananager.Reset();
		}
		break;

		//게임방 안의 팁 창
	case IDM_GAMEHELP:
		//{
		//	NMBASE::SOUND::g_Sound.PlayWav(SND_BUTTON_CLICK);
		//	
		//	// 있는지 없는지 검사 없으면 출력 있으면 패스
		//	CSprDlgTip *ppnode = (CSprDlgTip*)m_cSprDlgMan.GetObject(SDLG_TIP);		
		//	if(ppnode != NULL)
		//		return FALSE;

		//	// 사용자 정보를 보여줌
		//	CSprDlgTip *pNode = new CSprDlgTip;//CSprDlgObjectMansizeof(());
		//	if(pNode == NULL)
		//		return FALSE;

		//	if( pNode->Init(g_pGameView,&Page,326,165,SDLG_TIP) )
		//	{
		//		m_cSprDlgMan.AddObject(pNode);
		//	}
		//	else
		//	{
		//		delete pNode;
		//		pNode = NULL;
		//	}
		//}
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
		// 본인 카드 오픈 버튼
	case IDM_CARD_OPEN:
		{
			// 버튼 spr 인덱스가 0이면 본인 카드 오픈 On, 1 이면 Off
			int nBtnNum = g_pGameView->m_cGameViewBtnMananager.m_btnCardOpen.GetButtonNumber(); // 현재 버튼 상태(On or Off)
			bool bUIFoldOpen = false;

			// 버튼이 Off 면 On 시켜줌 -> 버튼 상태 버튼은 서버 결과에 따라 하기로 함
			if (nBtnNum == 1)
			{
				bUIFoldOpen = true;
			}
			
			// 버튼 spr 인덱스를 Toggle 시킴
			//g_pGameView->m_cGameViewBtnMan.m_btnCardOpen.SetSprite(&(g_pGameView->m_cGameViewBtnMan.m_sprUICardOpenBtn), nBtnNum);
			
			// 서버에 알림
			CCL_ASK_CARDOPEN msg;
			msg.Set(bUIFoldOpen);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
	default:
		{
			if( AutoBetSetUpWnd().OnCommand(wParam) )
			{
				return TRUE;
			}

			//재기충전	
			if(g_cOneShotCharge.OnCommand(wParam, lParam))
			{
				return TRUE;
			}

			return FALSE;
		}
		break;
	}

	return TRUE;
}


// ### [사운드 추가 작업] ###
void CGameProcessSutda::SoundFxKind(RAISE_LAST_CMD betkind, BOOL bSex, int nSndFxKind)
{

	SutdaGameSound().OnBetVoice(bSex, nSndFxKind, betkind, FALSE );
// 	switch(betkind)
// 	{
// 	case RAISE_LAST_CMD_FOLD: //다이
// 		PBPlaySound(sex?true:false, SND_DIE);
// 		break;
// 	case RAISE_LAST_CMD_CHECK:// 체크
// 		PBPlaySound(sex?true:false, SND_CHECK);
// 		break;
// 	case RAISE_LAST_CMD_CALL: // 콜
// 		PBPlaySound(sex?true:false, SND_CALL);
// 		break;
// 	case RAISE_LAST_CMD_PING: // 삥
// 		PBPlaySound(sex?true:false, SND_PING);
// 		break;
// 	case RAISE_LAST_CMD_DADANG: // 따당
// 		PBPlaySound(sex?true:false, SND_DDANG);
// 		break;
// 	case RAISE_LAST_CMD_HALF: // 하프
// 		PBPlaySound(sex?true:false, SND_HALF);
// 		break;
// 	case RAISE_LAST_CMD_ALLIN: // 올인
// 		PBPlaySound(sex?true:false, SND_ALLIN);
// 		break;	
// 	}
}

void CGameProcessSutda::SoundFxKindBig(RAISE_LAST_CMD betkind, BOOL bSex, int nSndFxKind)
{
	SutdaGameSound().OnBetVoice(bSex, nSndFxKind, betkind, TRUE );
// 	switch(betkind)
// 	{
// 	case RAISE_LAST_CMD_FOLD: //다이
// 		PBPlaySound(sex?true:false, SND_DIE_H);
// 		break;
// 	case RAISE_LAST_CMD_CHECK:// 체크
// 		PBPlaySound(sex?true:false, SND_CHECK_H);
// 		break;
// 	case RAISE_LAST_CMD_CALL: // 콜
// 		PBPlaySound(sex?true:false, SND_CALL_H);
// 		break;
// 	case RAISE_LAST_CMD_PING: // 삥
// 		PBPlaySound(sex?true:false, SND_PING_H);
// 		break;
// 	case RAISE_LAST_CMD_DADANG: // 따당
// 		PBPlaySound(sex?true:false, SND_DDANG_H);
// 		break;
// 	case RAISE_LAST_CMD_HALF: // 하프
// 		PBPlaySound(sex?true:false, SND_HALF_H);
// 		break;
// 	case RAISE_LAST_CMD_ALLIN: // 올인
// 		PBPlaySound(sex?true:false, SND_ALLIN_H);
// 		break;
// 	}
}


void CGameProcessSutda::OnCheckTurnLimitTime()
{
	

	if(g_RI.State == EGAME_WAITING)
		return;

	if(g_pGameView->m_cPlayerDrawManager.GetStartTimerTime() <= 0)
		return;

	OnCheckTurnLimitTimeBase();

// 	if(g_Poker.nState == RSTATE_PREPARESTART || g_Poker.nState == RSTATE_EXTRAFIRSTBET )
// 	{
// 		if( g_pGameView->m_cPlayerDrawManager.IsSelectCard(0) == FALSE)
// 		{
// 			return;
// 		}
// 
// 		int tcnt = g_pGameView->m_cPlayerDrawManager.GetTimerTimeGab() / 1000;		
// 
// 		if(tcnt >= PBTurnTimeOut(ETTT_CARDCHOICE) && !g_ObserverMan.IsMyObserver() && g_SelectCard().IsChoiceCard() )
// 		{	
// 			//2006.3.20
// 			//카드 선택시 시간이 지나 게임에서 자동으로 선택한다. 3번까지 자동 선택하면 게임 종료 후 강퇴 시킨다.
// 			if( m_cPlayer[0].m_nCardChoiceLimtCnt < MAX_TURN_TIMEOUT_LIMIT_COUNT && g_pGameView->m_cPlayerDrawManager.IsSelectCard(0) == TRUE )
// 			{				
// 				//g_SelectCard().EndSelectCard(TRUE);				
// 				g_pGameView->m_cPlayerDrawManager.SetSelectCard(0, false);
// 				m_cPlayer[0].m_nCardChoiceLimtCnt ++;
// 				g_pGameView->m_cDisplayBoard.SetText(EDBT_LIMITTIME);
// 				return;
// 			}
// 		}
// 	}
}



void CGameProcessSutda::OnCardEvent(CARDEVENT *pEvent)
{
	if(pEvent == NULL)
	{
		return;
	}

	switch( pEvent->EventCode )
	{
	case EVENT_PREPARESTART_ONEEND:
		{
			int nPlayNum = pEvent->Option;
			int Index    = pEvent->Option2;

			if(nPlayNum < 0 || nPlayNum >= MAX_PLAYER) 
			{
				break;
			}

			if(m_cPlayer[0].PNum == nPlayNum)
			{
				if( !g_ObserverMan.IsMyObserver())
				{					
					m_cPlayer[nPlayNum].SetCardFrontShow(Index);	
				}
				m_cPlayer[nPlayNum].ShowCard(Index);
			}
			else
			{
				m_cPlayer[nPlayNum].ShowCard(Index);
			}	
		}
		break;

	case EVENT_THREEFLYEND: // 처음 4장
		{
			int nPlayNum = pEvent->Option;
			int Index    = pEvent->Option2;

			if(nPlayNum < 0 || nPlayNum >= MAX_PLAYER)
			{
				break;
			}

			if(m_cPlayer[0].PNum == nPlayNum && !g_ObserverMan.IsMyObserver())
			{				
				m_cPlayer[nPlayNum].SetCardFrontShow(Index);				
			}
			m_cPlayer[nPlayNum].ShowCard(Index);

			if(!g_ObserverMan.IsMyObserver() && m_bWindCardEnd  && m_cPlayer[0].JoinState == EGAME_JOININ && m_cPlayer[0].PlayState == EPLAYER_GAMEPLAYING)			
			{
				CCL_WINDCARDEND msg2;
				msg2.Set(g_RI.RoomNum, m_cPlayer[0].ServPNum, m_cPlayer[0].UI.UNum, m_cPlayer[0].UI.ID);//int rnum, int nPlayNum, int unum, char *id)
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg2.pData, msg2.GetTotalSize());				
				m_bWindCardEnd = FALSE;				
			}
		}
		break;

	case EVENT_ONEFLYEND:		//섯다 히든카드 도착
		{
			int nPlayNum = pEvent->Option;
			int Index    = pEvent->Option2;

			if(nPlayNum < 0 || nPlayNum >= MAX_PLAYER) 
				break;		

			//쪼으기를 했을 때 최종 완료 위치 세팅

			//CPoint ptCardPosTest = g_PLAYER(nPlayNum)->GetCardPos(Index-1);
			CPoint ptCardPos = g_PLAYER(nPlayNum)->GetHiddenCardFinalPos();

			if (nPlayNum==0 && !g_ObserverMan.IsMyObserver() )
			{						
				
				//ptCardPosTest.x += (g_cUIData.m_nMyCardGab);

				if (g_Config.bAutoHidden /*|| rand()%2 */)//자동쪼으기
				{					
					g_PLAYER(0)->SetCardPos(Index, ptCardPos);
					//자동쪼으기
					g_PLAYER(0)->m_cMyCard[Index].SetReadyToJJogi( FALSE, ptCardPos );		//쪼으기를 했을 때 최종위치 설정

					/////////애니메이션 구현 ////////////////

					//성별구분
					int scenekind = 0;
					if( g_PLAYER(0)->UI.Sex ) scenekind = 3;
					else scenekind = 0;

					//좋은패 나쁜패 구분
					int goodandbad = GetJokboGoodAndBad();
					scenekind = scenekind + goodandbad;				

					CPoint ptHiddenCard = g_PLAYER(0)->GetCardPos(Index);
					int		nCardNo = g_PLAYER(0)->GetCardNum(Index);				

					ptHiddenCard.x -= 28;
					ptHiddenCard.y -= 23;

					m_cPlayer[nPlayNum].ShowCard(Index, false);
					g_pGameView->m_AniQue.Ani_Insert( ANIMATION_HIDDEN, ptHiddenCard, nCardNo, goodandbad, scenekind );

				}
				else
				{
					//유저가 수동으로 쪼으기
					g_PLAYER(nPlayNum)->m_cMyCard[Index].SetReadyToJJogi( TRUE, ptCardPos );		//쪼으기를 했을 때 최종위치 설정

				}
			}
			else 
			{
// 				if (	GetRoundMaxCard()==2 ||							//2장카드 모드일경우는 좀더 카드를 벌려준다.
// 						(nPlayNum==0 && g_ObserverMan.IsMyObserver())	//관전중 0번자리는 좀더 카드를 벌려준다.
// 					)
// 				{
// 					ptCardPos.x += (g_cUIData.m_nMyCardGab); //2장카드 모드일경우 or 관전중 0번자리는  좀 더 카드를 벌려준다.
// 				}
// 				else
// 				{
// 					ptCardPos.x += (g_cUIData.m_nCardGab);	
// 				}
// 				

				g_PLAYER(nPlayNum)->m_cMyCard[Index].SetReadyToJJogi( FALSE, ptCardPos );		//쪼으기를 했을 때 최종위치 설정
				m_cPlayer[nPlayNum].ShowCard(Index);
			}

		}
		break;

	case EVENT_ONEFLYTOTEND: // 한장씩 모두 날렸다면 서버로
		{
			if(!g_ObserverMan.IsMyObserver() && m_bWindCardEnd && m_cPlayer[0].JoinState == EGAME_JOININ && m_cPlayer[0].PlayState == EPLAYER_GAMEPLAYING)
			{
				CCL_WINDCARDEND msg2;
				msg2.Set(g_RI.RoomNum, m_cPlayer[0].ServPNum, m_cPlayer[0].UI.UNum, m_cPlayer[0].UI.ID);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg2.pData, msg2.GetTotalSize());			

				g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(false);
			}

			for(int i = 0 ; i < MAX_PLAYER ; ++i)
			{
				g_Poker.RU[i].nLastCmd = 0;
			}

// 			if(!g_ObserverMan.IsMyObserver() && g_PLAYER(0)->JoinState == 1 && g_PLAYER(0)->PlayState == 1)
// 			{
// 				if( g_PLAYER(0)->m_nTotalCardCount <= 6 )
// 				{
// 					g_pGameView->m_cGameHelper.CalculationProbability();
// 				}
// 				else if( g_PLAYER(0)->m_nTotalCardCount == 7 )
// 				{
// 					g_pGameView->m_cGameHelper.ClearProbability();
// 				}
// 			}

			m_bWindCardEnd = FALSE;

// 			if(!g_ObserverMan.IsMyObserver() && g_PLAYER(0)->JoinState == 1)
// 			{
// 				if( g_PLAYER(0)->m_nTotalCardCount < GetTotalCardNum() )
// 				{
// 					g_pGameView->m_cGameHelper.CheckOtherJokboCheck(0);
// 				}
// 			}
		} 
		break;

	case EVENT_CARD_GATHEREND:		//섯다 결과때 카드를 다 모았을때
		{

			int nPlayNum		= pEvent->Option;
			int bLastPlayer     = pEvent->Option2;

			g_PLAYER(nPlayNum)->CardResultOpen(bLastPlayer);


		} break;

	case EVENT_CARD_OPENEND_EFT:
		{
			int nPlayNum = pEvent->Option;

			g_PLAYER(nPlayNum)->GenealogyEffect();			

		} break;
	case EVENT_CARD_END_RESULT_CARD:
		{
			int nPlayNum		= pEvent->Option;
			BOOL bEffectExist = pEvent->Option2;///bEffetExist 는 족보확인이펙트(카드커지는거)가 있는지 유무

			//카드 결과 오픈 연출이 완료되었다.
			g_PLAYER(nPlayNum)->SetResultCardOpenAni(FALSE);
			if (nPlayNum<0 || nPlayNum>=MAX_PLAYER)
				break;
			
			//다른유저들의 카드 오픈 연출도 완료되었나 확인해본다.
			BOOL bAllPlayerOpenCard = TRUE;		//모든유저가  오픈하였나?
			for(int i=0; i<MAX_PLAYER; i++)
			{
				if(strlen(g_PLAYER(i)->UI.ID)==0)
					continue;

				if(g_PLAYER(i)->JoinState == EGAME_JOINOUT || g_PLAYER(i)->bFold == TRUE)
					continue;
				
				if (g_PLAYER(i)->IsResultCardOpenAni())
				{
					//한명이라도 남아있다면
					bAllPlayerOpenCard = FALSE;
					break;
				} 

			}

			if (bAllPlayerOpenCard)
			{
				//족보확인이펙트가 없으면 바로 서버에 카드오픈 연출이 끝낫다고 알려준다.
				if (bEffectExist==FALSE)
				{
					NotifyResultCardOpenAnimationEnd(1);
				}
				else //족보확인이펙트가 있으면  일정 시간 후에 서버에 카드오픈 연출이 끝낫다고 알려준다.
				{
					NotifyResultCardOpenAnimationEnd(80);				
				}			

			}
		} break;

	}
}


CPlayer* CGameProcessSutda::GetPlayer(int i)
{
	ASSERT(i<GetMaxNewPlayer() && i>=0);
	return &m_cPlayer[i];
}

CPlayerSutda* CGameProcessSutda::GetPlayerNP(int i)
{
	return (CPlayerSutda *)GetPlayer(i);
}


void CGameProcessSutda::CheckMinimumUser(void)
{
	int nPlayer = 0;

	for(int i = 0; i < MAX_PLAYER; ++i) 
	{
		if( strlen(m_cPlayer[i].UI.ID) > 0 && m_cPlayer[i].JoinState != EGAME_JOINOUT)
		{
			++nPlayer;
		}
	}

	if(nPlayer < 2) 
	{
		g_pGameView->m_cDisplayBoard.SetText(EDBT_MINUSER);
	}
}

void CGameProcessSutda::JackPotMoneyUpdate(jackpot::JP_MONEYINFO* pInfo)
{
	// 게임 타입
	IDX_GAME eGameType = (IDX_GAME)g_RI.nGameIndex;

	// 자신이 입장한 방의 최소 금액
	INT64 i64RoomMoney = g_RI.llEnterLimitMoney;

	if(i64RoomMoney <= 0)	///자유방 제외
		return;

	// 테스트 코드
//	ROOMINFO sRoomInfo;
//	memcpy(&sRoomInfo, &g_RI, sizeof(ROOMINFO));

	//뉴포커, 로우, 7포커
	ASSERT (eGameType > IDX_GAME_NULL);

	const int INDEX_ARRAY_JACKPOT_GAME_7POKER = 0; //7포커 인덱스 0

	INT64 i64JackPotMoney = 0; 

	for(int i = 0; i < jackpot::DEF_MONEYRANGE_NUM; ++i)
	{
		if(LIMITMONEY_INT_INT64(pInfo->stJPBase[ INDEX_ARRAY_JACKPOT_GAME_7POKER ].nLimitMoney[i]) == i64RoomMoney)
		{
			i64JackPotMoney = pInfo->stJPBase[INDEX_ARRAY_JACKPOT_GAME_7POKER ].llJackPotMoney[ i ];//  llJackpotMoney[eGameType][i];  
			break;
		}
	}

	if(i64JackPotMoney > 0)
	{
		// 이전 잭팟 금액
		SetOldJackPotMoney(GetJackPotMoney());

		// 새로운 잭팟 금액
		SetJackPotMoney(i64JackPotMoney);
	}
}


void CGameProcessSutda::ClearJackpotMoney(int nGameCode, INT64 i64LimitMoney)
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

	if(i64LimitMoney <= 0)
		return;


	//1. 게임타입 검사
	//2. 입장한 방 머니 검사   
	//3. 두가지 값이 동일하면 현재 방의 잭팟 머니를 초기화 한다.
	int nGameIndexTemp		= g_RI.nGameIndex;
	INT64 i64LimitMoneyTemp = g_RI.MoneyRange.GetDispMinMoney();

	if(eGameType == nGameIndexTemp && i64LimitMoney == i64LimitMoneyTemp)
	{
		SetOldJackPotMoney(0);
		SetJackPotMoney(0);

		// 전역변수의 해당 머니 구간의 값도 0으로 초기화 한다.
		// 현재 방에 대당하는 구간의 잭팟금액을 초기화 한다.
		for(int i = 0; i < jackpot::DEF_MONEYRANGE_NUM; ++i)
		{
			if(g_RoomMoneyRanges[i] >= i64LimitMoneyTemp)
			{
				g_JackPotMoneyInfo.stJPBase[ eGameType ].llJackPotMoney[ i ] = 0;
				//g_JackPotMoneyInfo.llJackpotMoney[eGameType][i] = 0;  
				break;
			}
		}
	}	
}


void CGameProcessSutda::TestCode(MSG* pMsg)
{
	
	
}


void CGameProcessSutda::OnAskChoiceCard(char JokboCard[])	//3장모드 카드선택
{
	g_pGameView->m_cPlayerDrawManager.SetTurn(-1);

	//관전일경우 return;
	if(g_ObserverMan.IsMyObserver())
		return;

	//포기한 사용자면 리턴...
	if(g_PLAYER(0)->bFold || g_PLAYER(0)->JoinState == EGAME_JOINOUT) 
		return;
	
	
	//카드 쪼으기가 되지 않았을 경우 쪼으기 완료해주기
	//int nHiddenCardIndex = CARD_TOTALCOUNT-1;
	g_PLAYER(0)->CompleteJJogi(TRUE);


	//처음 받았던 카드로 다시 셋팅한다.	
	g_PLAYER(0)->ResetOrginalCard();
	
	//카드 선택창을 띄워준다.
	g_SelectCard().SetCard ( g_PLAYER(0)->GetCardNum(0), g_PLAYER(0)->GetCardNum(1), g_PLAYER(0)->GetCardNum(2));
	g_SelectCard().ShowSelectCard();//카드선택창 표시


	//높은 족보를 구해서 저장하고, SelectCard에 높은족보조합인덱스를 세팅한다.
	GetGenealogyPoint();


	//전광판 (패를 선택해주세요)
	g_pGameView->m_cDisplayBoard.SetText(EDBT_SELECTCARD_SUTDA);


}

void CGameProcessSutda::GetGenealogyPoint()
{

	//관전일경우 return;
	if(g_ObserverMan.IsMyObserver())
		return;

	int CardPoint[3] = {0,};

	//카드를 조합해서 점수를 저장
	if(g_PLAYER(0)->GetCardTotal() == 2)
	{
		
		//[관전추가]
		CardPoint[0] = GetPairPoint(g_PLAYER(0)->GetCardNum(0), g_PLAYER(0)->GetCardNum(1)); //족보찍어줄 위치
	}	
	else if(g_PLAYER(0)->GetCardTotal() == 3)
	{
		//[관전추가]
		CardPoint[0] = GetPairPoint(g_PLAYER(0)->GetCardNum(0), g_PLAYER(0)->GetCardNum(1));	//족보찍어줄 위치
		CardPoint[1] = GetPairPoint(g_PLAYER(0)->GetCardNum(1), g_PLAYER(0)->GetCardNum(2));
		CardPoint[2] = GetPairPoint(g_PLAYER(0)->GetCardNum(0), g_PLAYER(0)->GetCardNum(2));



		//3장섯다일경우 특수 족보를 끗점수로 변환 (2장은 변환없음 )
		//땡잡이, 사구, 암행어사점수는 끗 점수로 변환시켜줌
		for(int i = 0; i<3; i++)
		{
			//땡잡이 & 사구는 망통처리
			if( CardPoint[i] == 30 || CardPoint[i] == 11 || CardPoint[i] == 10 )
			{
				CardPoint[i] = 0;
			}
			//암행어사는 한끗 처리
			if( CardPoint[i] == 31 )
			{
				CardPoint[i] = 1;
			}
		}

	}	

	

	//내가 가진 족보중에 높은 족보를 찾아낸다.
	int temp;
	int nHighSelCardIndex = 0;
	m_HighJokboPoint = 0;

	m_HighJokboPoint = temp = CardPoint[nHighSelCardIndex];	//최고포인트점수에 임시로 번호를 정해서 넣는다.

	for(int i=1; i<3; i++)
	{
		if(temp < CardPoint[i])
		{
			//높은 포인트를 구해준다. 
			m_HighJokboPoint = temp = CardPoint[i];
			//카드선택에서 높은 위치를 구한다.
			nHighSelCardIndex = i;
		}
	}


	//카드선택창에 높은 족보 조합의 인덱스값 세팅
	g_SelectCard().SetHighSelCardIndex(nHighSelCardIndex);	

}



//족보의 순위를 매긴다.
int CGameProcessSutda::GetPairPoint(int card1, int card2)
{
	if(card1<1 || card1>20) return 0;
	if(card2<1 || card2>20) return 0;

	int kind1 = (card1+1)/2;
	int kind2 = (card2+1)/2;


	// 삼팔광땡
	if((card1==5 && card2==15)||(card1==15 && card2==5)) return 29;
	// 일삼광땡 
	if((card1==1 && card2==5)||(card1==5 && card2==1)) return 28;
	// 일팔광땡 
	if((card1==1 && card2==15)||(card1==15 && card2==1)) return 28;


	// 땡(1-10)
	if(kind1==kind2) return 17+kind1;

	// 얄리
	if((kind1==2 && kind2==1)||(kind1==1 && kind2==2)) return 17;
	// 독사
	if((kind1==1 && kind2==4)||(kind1==4 && kind2==1)) return 16;
	// 구삥
	if((kind1==1 && kind2==9)||(kind1==9 && kind2==1)) return 15;
	// 장삥
	if((kind1==10 && kind2==1)||(kind1==1 && kind2==10)) return 14;
	// 장사
	if((kind1==10 && kind2==4)||(kind1==4 && kind2==10)) return 13;
	// 세륙
	if((kind1==4 && kind2==6)||(kind1==6 && kind2==4)) return 12;

	// 사구(멍텅구리)
	if((card1==7 && card2==17)||(card1==17 && card2==7)) return 11;

	// 사구
	if((kind1==4 && kind2==9)||(kind1==9 && kind2==4)) return 10;

	//암행어사(광땡만잡음)
	if((card1==7 && card2==13)||(card1==13 && card2==7)) return 31; //


	// 삼칠망통(땡잡이)
	if((card1==5 && card2==13)||(card1==13 && card2==5)) return 30;	//사구 하나만 인정...

	// 끗수 구하기
	int point = (kind1 + kind2) % 10;
	// 끗
	return point;
}


//애니메이션에서 사용할 족보별 숫자
int CGameProcessSutda::GetAniPoint( int card1, int card2 )
{
	int kind1 = 0;
	int kind2 = 0;

	if(card1<1 || card1>20) return 0;
	if(card2<1 || card2>20) return 0;

	kind1 = (card1+1)/2;
	kind2 = (card2+1)/2;


	// 삼팔광땡
	if((card1==5 && card2==15)||(card1==15 && card2==5)) return 0;
	// 일삼광땡 
	if((card1==1 && card2==5)||(card1==5 && card2==1)) return 1;
	// 일팔광땡 
	if((card1==1 && card2==15)||(card1==15 && card2==1)) return 2;


	// 땡(1-10)
	if(kind1==kind2) return 13 - kind1;		//17+kind1

	
	// 얄리
	if((kind1==2 && kind2==1)||(kind1==1 && kind2==2)) return 13;
	// 독사
	if((kind1==1 && kind2==4)||(kind1==4 && kind2==1)) return 14;
	// 구삥
	if((kind1==1 && kind2==9)||(kind1==9 && kind2==1)) return 15;
	// 장삥
	if((kind1==10 && kind2==1)||(kind1==1 && kind2==10)) return 16;
	// 장사
	if((kind1==10 && kind2==4)||(kind1==4 && kind2==10)) return 17;
	// 세륙
	if((kind1==4 && kind2==6)||(kind1==6 && kind2==4)) return 18;

	// 사구(멍텅구리)
	if((card1==7 && card2==17)||(card1==17 && card2==7)) return 29;	//3끗

	// 사구
	if((kind1==4 && kind2==9)||(kind1==9 && kind2==4)) return 29;	//3끗

	//암행어사(광땡만잡음)
	if((card1==7 && card2==13)||(card1==13 && card2==7)) return 31; //1끗 

	// 삼칠망통(땡잡이)
	if((card1==5 && card2==13)||(card1==13 && card2==5)) return 30;	//0

	// 끗수 구하기
	int point = 28 - (kind1 + kind2) % 10;
	// 끗
	return point;
}


//## 족보표의 위치를 구하기 구한다.
int CGameProcessSutda::GetJokboIndex(int card1, int card2)
{
	//##  0 ~ 11 까지
	int kind1 = 0;
	int kind2 = 0;

	if(card1<1 || card1>20) return 0;
	if(card2<1 || card2>20) return 0;

	kind1 = (card1+1)/2;
	kind2 = (card2+1)/2;


	// 삼팔광땡
	if((card1==5 && card2==15)||(card1==15 && card2==5)) return 0;
	// 일삼광땡 
	if((card1==1 && card2==5)||(card1==5 && card2==1)) return 1;
	// 일팔광땡 
	if((card1==1 && card2==15)||(card1==15 && card2==1)) return 1;


	// 땡(1-10)
	if(kind1==kind2) return 1;


	// 얄리
	if((kind1==2 && kind2==1)||(kind1==1 && kind2==2)) return 2;
	// 독사
	if((kind1==1 && kind2==4)||(kind1==4 && kind2==1)) return 3;
	// 구삥
	if((kind1==1 && kind2==9)||(kind1==9 && kind2==1)) return 4;
	// 장삥
	if((kind1==10 && kind2==1)||(kind1==1 && kind2==10)) return 5;
	// 장사
	if((kind1==10 && kind2==4)||(kind1==4 && kind2==10)) return 6;
	// 세륙
	if((kind1==4 && kind2==6)||(kind1==6 && kind2==4)) return 7;


	//갑오
	if( (kind1 + kind2) % 10 == 9) return 8;

	//땡잡이
	if((card1==5 && card2==13) || (card1==13 && card2==5))	return 10;//사구 하나만 인정...	


	//사구1
	if((card1==7 && card2==18) || (card1==18 && card2==7))	return 10;
	//사구2
	if((card1==8 && card2==17) || (card1==17 && card2==8))	return 10;
	//사구3
	if((card1==8 && card2==18) || (card1==18 && card2==8))	return 10;
	//사구4(십자리)
	if((card1==7 && card2==17) || (card1==17 && card2==7))	return 10;

	//암행어사(광땡만잡음)
	if((card1==7 && card2==13)||(card1==13 && card2==7)) return 11;	 //31


	return 9;	//끗 망통
}


char* CGameProcessSutda::GetPairString(int point)
{

	static char szNullString[]=" ";

	static char *szRankingString[33]={"망통","한끗","두끗","세끗","네끗","다섯끗","여섯끗","일곱끗","여덟끗","갑오","사구","사구-십자리","세륙","장사","장삥","구삥","독사","알리","삥땡","이땡","삼땡","사땡","오땡","육땡","칠땡","팔땡","구땡","장땡","광땡","삼팔광땡","땡잡이","암행어사",""};

	if(point<0 || point>32) return szNullString;
	return szRankingString[point];
}




int CGameProcessSutda::GetJokboGoodAndBad()
{
	int goodandbad = 0;

	int point1 = 0;
	int point2 = 0;
	int point3 = 0;

	int nCard1 = g_PLAYER(0)->GetCardNum(0);
	int nCard2 = g_PLAYER(0)->GetCardNum(1);

	//2장이나 재시합일때
	if( g_RI.FormKind == SD_2CARD || IsRematchGame() )
	{
		
		point1 = GetAniPoint( nCard1, nCard2 );

		if( point1 < 13) goodandbad = 1; //땡이상이다(좋음)
		else if( point1 > 18 && point1 < 29) goodandbad = 2;	//갑오에서 망통 (나쁨)
		else goodandbad = 0;	//기타전부(보통)
	}
	else
	{
		
		int nCard3 = g_PLAYER(0)->GetCardNum(2);

		point1 = GetAniPoint( nCard1, nCard2 );
		point2 = GetAniPoint( nCard1, nCard3 );
		point3 = GetAniPoint( nCard2, nCard3 );

		//1,2장으로 만든 족보보다 1,3or2,3으로 만든 족보가 높은경우
		if( point3 < point1 || point2 < point1 )
		{
			if( point2 < 13 || point3 < 13 ) goodandbad = 1; //땡이상이다
			else if( (point2 > 18 && point2 < 29) && (point3 > 18 && point3 < 29) ) goodandbad = 2;	//갑오에서 망통 
			else goodandbad = 0;	//기타전부
		}
		else
		{
			if( (point1 > 18 && point1 < 29) && (point2 > 18 && point2 < 29) && (point3 > 18 && point3 < 29) )
				goodandbad = 2;		//갑오에서 망통
			else goodandbad = 0;	//기타전부
		}
	}

	return goodandbad;
}

void CGameProcessSutda::OnCompleteJJogiAnimation()
{
	///자동쪼으기 애니메이션이 끝났을때 처리

	int nHiddenCardIndex = g_PLAYER(0)->GetHiddenCardIndex();
	g_PLAYER(0)->CompleteJJogi(TRUE);
	g_PLAYER(0)->ShowCard(nHiddenCardIndex);
}


int CGameProcessSutda::GetFirstPlayer()			//보스 사용자 
{
	return GetPNum_ByServPN(g_Poker.nBossPNum);

}
int CGameProcessSutda::GetLastPlayer()			//보스 이전 사용자
{	

	int nFirstPlayer = GetFirstPlayer();
	return ( nFirstPlayer + 1 ) % GetMaxPlayer();

}


//관전자게임종료안보임
//게임종료후 유저의 카드정보를 받는다.(오픈전)
void CGameProcessSutda::OnResultCard(SD_RESULTCARD *pResultCard)
{

	g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();		// 버튼 초기화	
	g_pGameView->m_cPlayerDrawManager.SetWinnerPNum(-1);
	g_pGameView->m_cPlayerDrawManager.SetTurn(-1);

	//베팅 커멘드 지움
	g_pGameView->m_cAniSprListManager.BetCmd_DelDelay(55);

	//살아있는 유저를 구한다.
	int totlive = 0;	
	for(int i = 0; i<MAX_PLAYER; i++) 
	{
		int pnum = GetPNum_ByServPN(i);


		if(strlen(g_PLAYER(pnum)->UI.ID)==0)
			continue;

		if(g_PLAYER(pnum)->JoinState == EGAME_JOINOUT || g_PLAYER(pnum)->bFold == TRUE)
			continue;
		

		totlive++;

		g_PLAYER(pnum)->SetJokboPoint(-1);
		g_PLAYER(pnum)->SetOrginalCard( pResultCard->Bc[i].nCard, CARD_TOTALCOUNT);	//유저카드정보

		
		int anipoint = GetAniPoint( g_PLAYER(pnum)->GetOrginalCard(0), g_PLAYER(pnum)->GetOrginalCard(1));
		g_PLAYER(pnum)->SetJokboAniPoint(anipoint);
	}

	//m_Jjogi.bHiddenCardShow = FALSE;

	//bNowCheckPair = TRUE;	//패비교중이라고 표시 
	g_pGameView->m_Radiophonograph.SetFade( FALSE ); //섯다 축음기

	//### 카드 셋팅 부분 ###//
	//서버로 부터 전송받은 카드를 세팅한다
// 	int i;
// 	for(i=0; i<MAX_PLAYER; i++)
// 	{
// 		//모든 유저의 족보를 초기화 해준다.
// 		g_PLAYER(i)->SetJokboPoint(-1);
// 
// 		if(strlen(ci.ID[i])>0)
// 		{
// 			int pnum = GetPlayerPNum( ci.ID[i] );
// 			if( pnum == -1 ) continue;
// 
// 			int nServPNum  = GetServPNum_ByPN(i);
// 
// 			g_PLAYER(0)->SetOrginalCard(m_GameOverResultData.Ui[nServPNum].)
// 
// 			m_OrgCard[pnum][0] = ci.Card[i][0];
// 			m_OrgCard[pnum][1] = ci.Card[i][1];
// 			m_OrgCard[pnum][2] = ci.Card[i][2];
// 		}
// 	}

	//리뉴얼테스트
	//g_pGameView->m_strDebug = _T("");

	//### 족보확인 효과 부분 ###//
	BOOL bLastPlayer = FALSE;
	int opencnt = 0;
	int delay = 0;
	int jokbopoint = 0;
	int nEffectDelay = 700;	//족보효과 딜레이
	int GenealogyEftCnt = 0;	//패확인 이팩트가 몇개 나왔나 검사한다.

	//마지막 유저부터 족보를 확인한다.
	//for( int i = GetLastPlayer()+GetMaxPlayer()-1; i >= GetLastPlayer(); i--)
	for( int i = GetLastPlayer(); i < GetLastPlayer()+GetMaxPlayer() ; i++)
	{
		int pnum = i % GetMaxPlayer();

		if(strlen(g_PLAYER(pnum)->UI.ID)==0)
			continue;

		g_PLAYER(pnum)->SetResultCardOpenAni(FALSE);	//카드 연출 애니메이션 플래그 초기화

		if(g_PLAYER(pnum)->JoinState == EGAME_JOINOUT || g_PLAYER(pnum)->bFold == TRUE)
			continue;

		//족보효과가 나왔을경우 딜레이를 준다. 
		int totdelay = delay + (GenealogyEftCnt * nEffectDelay);

		opencnt++; //총 몇명이 오픈됐나 검사한다.

		if( opencnt ==  totlive ) 
			bLastPlayer= TRUE;


		
		//카드를 모아준다.
		g_PLAYER(pnum)->CardResultGather( bLastPlayer, totdelay );  
		g_PLAYER(pnum)->SetResultCardOpenAni(TRUE);					//여기서 카드 연출 애니메이션 플래그를 켜준다.

		delay += 750;

		//## 족보에 관련되서 효과를 보여줄경우 딜레이를 추가해준다. ##//
		//시작 유저이면 넘어간다.
		//if( i == GetLastPosPNum() ) continue;
		int point = GetAniPoint( g_PLAYER(pnum)->GetOrginalCard(0), g_PLAYER(pnum)->GetOrginalCard(1));
		if( point < 20 ) 
			GenealogyEftCnt++;


	}

	//카드 쪼으기를 더이상 못하게한다.
	if (!g_ObserverMan.IsMyObserver())
	{
		int nHiddenCardIndex = g_GAMEPROCESS()->GetRoundMaxCard() - 1;
		g_PLAYER(0)->m_cMyCard[nHiddenCardIndex].SetReadyToJJogi(FALSE); 
	}

	//규제안 관련하여 예약기능 모두 초기화.
	ReserveState(RESERVESTATE_BET_NONE);
	CanReserve(false);
	g_pGameView->m_cGameViewBtnMananager.ShowReserveBtn(false);

}

//현재 이전에 유저가 높은 족보를 가졌는지 검사한다. 
//다른유저의 족보는 오픈되는 순서되로 알수있다. 
//Play[pnum].m_JokboPoint 값이 -1 이면 아직 오픈하지 않은 족보이다.
BOOL CGameProcessSutda::ResultJokboExamine( int nPnum )
{
	// 리턴값 이 TRUE : 애니메이션 효과가 있다. 
	
	int i;
	int winpnum = -1;
	int nowpoint = -1;
	int totlive = 0;
	int pnt[MAX_PLAYER]={0,};

	BOOL bSagu   = FALSE;		// 기존의 사구다시 방식
	BOOL bSagu2  = FALSE;		// 추가된 사구다시 방식(낮은 족보-알리까지만 잡는다)
	BOOL bWin73  = FALSE;		// 73망통이 가장 센패가 될 경우(구땡 ~ 삥땡이 있을때...)
	BOOL b38Tang = FALSE;		// 38광땡에서 장땡까지 있는가?
	BOOL bKwangTang = FALSE;	//광땡(암행어사추가)
	BOOL b38KwangTang = FALSE;	//순수 38광땡체크(암행어사때 추가)

	//사운드에서 사용
	int sagupnum = 0;	//49 유저 

	BOOL bPlayerSex = g_PLAYER(nPnum)->UI.Sex;
	int	 nSndFxKind = g_PLAYER(nPnum)->GetSndFxKind();

	// 땡잡이 관련 플래그 설정
	// 먼저 장땡에서 삥땡까지로 있나 검사 (땡잡이)
	// 광땡이 있나 검사 (암행어사)
	for(int i=0; i<MAX_PLAYER; i++)
	{
		
		if(strlen(g_PLAYER(i)->UI.ID) == 0 || g_PLAYER(i)->JoinState == EGAME_JOINOUT || g_PLAYER(i)->bFold )
			continue;

		int point = g_PLAYER(i)->GetJokboPoint();
		if( point <= -1) 
			continue;

		if(point>=27 && point<=29) b38Tang = TRUE;	// 38광땡에서 장땡까지 있을경우 땡잡이 활동불가
		if(point==28) bKwangTang = TRUE;			//광땡이 있는가 검사	(암행어사추가)
		if(point==29) b38KwangTang = TRUE;			//38광땡이 있다.		(암행어사추가)

	}
	// 38광땡에서 장땡까지는 없고, 딴땡이 있는가?
	if(b38Tang==FALSE)
	{
		for(i=0; i<MAX_PLAYER; i++)
		{
			if(strlen(g_PLAYER(i)->UI.ID) == 0 || g_PLAYER(i)->JoinState == EGAME_JOINOUT || g_PLAYER(i)->bFold )
				continue;

			int point = g_PLAYER(i)->GetJokboPoint();
			if( point <= -1) 
				continue;

			if(point>17 && point<27)	// 삥땡에서 구땡까지로 바뀜...
			{
				bWin73 = TRUE; // 땡잡이가 있다면 활동가능하다....	
			}			
		}
	}

	for(i=0; i<MAX_PLAYER; i++)
	{
		pnt[i] = -1;

		//if(strlen(Play[i].UI.ID) == 0 || Play[i].JoinState == 0 ||Play[i].LastState == GCMND_FOLD) continue;
		if(strlen(g_PLAYER(i)->UI.ID) == 0 || g_PLAYER(i)->JoinState == EGAME_JOINOUT || g_PLAYER(i)->bFold )
			continue;

		int point = g_PLAYER(i)->GetJokboPoint();
		if( point <= -1) 
			continue;

		totlive++;

		//땡잡이는  땡이 있다면 가장센패가 되지만 없을 경우 망통처리한다
		if(point==30 && bWin73==FALSE){ // 73망통 땡잡이
			point = 0; // 망통처리
		}

		//게이머중에 광땡으로 이긴기는 사람이 있을경우엔 가장센패가되고 없을 경우엔 1끗처리
		if(point==31 && (bKwangTang==FALSE || b38KwangTang==TRUE)){ // 7, 13 암행어사...
			point = 1; // 한끗처리
		}

		if(point > nowpoint) 
		{
			winpnum = i; 
			nowpoint = point; 
		}
		pnt[i]=point;
	}

	//제일높은패가 내패일경우
	if( nPnum == winpnum )
	{
		//족보 확인 이펙트(카드 커지는거)를 준다.
		//Play[r_pnum].MyCard[1].SetEvent(EVENT_OPENEFT, winpnum);
		

		int point = g_PLAYER(nPnum)->GetJokboPoint();

		//내가 이겼을경우 38에서 빙땡, 땡잡이, 암행어사로 이겼을경우
		if( (point<= 29 && point >=18) ||point == 30 || point == 31 ) 
			SutdaGameSound().WinJokboVoice(point, bPlayerSex, nSndFxKind);
		//그이외 일반 족보
		else 
			SutdaGameSound().NormalJokboVoice(point, bPlayerSex, nSndFxKind);


		
		int anipoint = g_PLAYER(nPnum)->GetJokboAniPoint();
		if (anipoint <= 19) // 갑오이상만 이펙트가 있다. 족보 확인 이펙트(카드 커지는거)가 있다고 알려준다.
		{

			g_PLAYER(nPnum)->m_cMyCard[1].SetEvent1(EVENT_CARD_OPENEND_EFT, winpnum, 1);
			return TRUE;
		}
		else
		{

			return FALSE;
		}		
	}
	else
	{
		int point = g_PLAYER(nPnum)->GetJokboPoint();

		//광땡에서 일땡까지의 족보를 가지고 질때
		if( point <= 28 && point >=18 ) 
			SutdaGameSound().LoseJokboVoice(point, bPlayerSex, nSndFxKind);
		//그이외 일반 족보 
		else
			SutdaGameSound().NormalJokboVoice(point, bPlayerSex, nSndFxKind);
	}

	return FALSE;		//return FALSE는 족보 확인 이펙트(카드 커지는거)가 없다는 의미
}




//한끗차이 승부인지 검사한다.
void CGameProcessSutda::CheckSortVoice(int nWinnerPNum)
{
	//족보에 따른 카드 오픈 애니메이션을 보여준다.
	int mypoint = GetAniPoint( g_PLAYER(0)->GetOrginalCard(0), g_PLAYER(0)->GetOrginalCard(1) );	//나의 족보 점수를 구한다.
	int otherpoint = 0;

	//내가 승자일경우 
	if( nWinnerPNum == 0 )
	{
		int point1 = 0;

		for( int i = 0; i < MAX_PLAYER; i++)
		{
			if( i == g_Poker.CurPlayer ) continue;		//$$$$$$$$$$$ 먼지 모르겠음

			point1 = mypoint;

			//49, 땡잡이, 암행어사일경우 끗점수로 변환한다.
			if( point1 == 29 ) point1 = 25;
			else if( point1 == 30 ) point1 = 28;
			else if( point1 == 31 ) point1 = 27;

			//저장된 포인트가 현재 포인트보다 클경우(현재족보가 더 높을경우) 저장된 포인트를 바꿔준다.
			if( otherpoint > point1 )  otherpoint = point1;
		}

		//내가 한끗차이로 졌을경우...
		if( mypoint == otherpoint - 1) 
			SutdaGameSound().SetEndActionVoice( END_SORT ); 
	}
	//내가 패자일경우 
	else
	{
		otherpoint = GetAniPoint(  g_PLAYER(nWinnerPNum)->GetOrginalCard(0), g_PLAYER(nWinnerPNum)->GetOrginalCard(1) );

		//49, 땡잡이, 암행어사일경우 끗점수로 변환한다.
		if( otherpoint == 29 ) otherpoint = 25;
		else if( otherpoint == 30 ) otherpoint = 28;
		else if( otherpoint == 31 ) otherpoint = 27;

		//내가 한끗차이로 졌을경우...
		if( mypoint - 1 == otherpoint ) 
			SutdaGameSound().SetEndActionVoice( END_SORT ); 
	}
}

void CGameProcessSutda::NotifyResultCardOpenAnimationEnd(int nDelay)		//일정 딜레이 후  카드 오픈 연출이 끝났다고 알려준다.
{
	m_nNotifyResultCardOpenAnimationEndDelay = nDelay;
}

	


void CGameProcessSutda::OnSamePair()
{
	//동률패일때 동률패 유저의 카드를 깜빡이게 하는 효과를 준다.

	int i=0;
	int maxpoint = -1;
	int point[MAX_PLAYER]={0,};
	for(i=0; i<MAX_PLAYER; i++)
	{		
		if(strlen(g_PLAYER(i)->UI.ID)>0 && g_PLAYER(i)->JoinState==EGAME_JOININ && g_PLAYER(i)->bFold==FALSE)
		{			
			point[i] = GetPairPoint(g_PLAYER(i)->GetOrginalCard(0), g_PLAYER(i)->GetOrginalCard(1));
		
			if(point[i] > maxpoint) 
			{
				maxpoint = point[i];
			}
		}
	}

	int samenum = 0;
	BOOL sameplayer[MAX_PLAYER]={0,};
	for(i=0; i<MAX_PLAYER; i++)
	{
		if(strlen(g_PLAYER(i)->UI.ID)>0 && g_PLAYER(i)->JoinState==EGAME_JOININ && g_PLAYER(i)->bFold==FALSE)
		{
			if(point[i] == maxpoint)
			{ 
				//최고 점수자가 체크
				samenum++; 
				sameplayer[i]=TRUE;
			}	
		}
	}

	if(samenum >= 2)
	{	
		for(i=0; i<MAX_PLAYER; i++)	//최고 점수가 두명이상일 경우..
		{
			if(sameplayer[i])
			{
				g_PLAYER(i)->SetCardBlinking(180);
			}
		}
	}

}

void CGameProcessSutda::OnRematch_by_49(bool bWinnerJoin, bool bReEntry)			//49에 의한 재경기
{

	//사구로 인한 재게임...
	if(bWinnerJoin)			
	{
		if (bReEntry)
		{			
			//재경기 참가 
			g_pGameView->m_AniQue.Ani_Insert( ANIMATION_POP2, g_cUIData.m_ptRematchNotice, 0, 0, 1, 0 );
		}
		else
		{
			g_pGameView->m_AniQue.Ani_Insert( ANIMATION_REGAMENOTICE, g_cUIData.m_ptRematchNotice, 0, 0, 1, 0, SND20, NULL );
		}
		

		//재대결 보이스
		//게임에 참여했던 사람들
 		if( !g_ObserverMan.IsMyObserver())
 		{
			SutdaGameSound().OnActionVoice(AV_49_REMATCH, 0, 2 );
		}

	}
	else
	{
		//재경기 안내

		if( g_ObserverMan.IsMyObserver() || g_PLAYER(0)->JoinState != EGAME_JOININ  || g_PLAYER(0)->bFold== FALSE )
			g_pGameView->m_AniQue.Ani_Insert( ANIMATION_REGAMENOTICE, g_cUIData.m_ptRematchNotice, 0, 0, 1, 0, SND20, NULL );
	}

	//전광판
	g_pGameView->m_cDisplayBoard.SetText(EDBT_NOTIFY_REMATCH_BY_49); 	
}


void CGameProcessSutda::OnRematch_by_Draw(bool bWinnerJoin)		//동률패에 의한 재경기
{

	// 동률패 재경기 안내 공지 애니메이션
	g_pGameView->m_AniQue.Ani_Insert( ANIMATION_REGAMENOTICE, g_cUIData.m_ptRematchNotice, 0, 0, 0, 0, SND21, NULL );

	//동일패 깜빡이기;
	OnSamePair();
	

	//사운드 

	//재시합에 참여하는 유저 
	if(bWinnerJoin)
	{
		if( !g_ObserverMan.IsMyObserver())
			SutdaGameSound().OnActionVoice(AV_JOKBO_REMATCH, 0);
	}
	else
	{
		//게임에 참여했던 사람들 		
		if( !g_ObserverMan.IsMyObserver() && strlen(g_PLAYER(0)->UI.ID) > 0 && g_PLAYER(0)->JoinState == EGAME_JOININ )
		{
			SutdaGameSound().OnActionVoice(AV_NORMAL_LOSS, 0);
		}
		//게임에 참여하지 않았던 사람들
		else
		{
			SutdaGameSound().OnActionVoice(AV_FAIL_HIDDEN, 0);
		}

	}


	//전광판
	g_pGameView->m_cDisplayBoard.SetText(EDBT_NOTIFY_REMATCH_BY_DRAW); 	

}

int CGameProcessSutda::GetRoundMaxCard()
{
	//현재 판에서 받아야할 최대 카드장수 (재대결판:2장, 2장섯다:2장, 3장섯다:3장);
	if (IsRematchGame())
	{
		return 2;
	}
	else
	{
		return CARD_TOTALCOUNT;
	}
}



void CGameProcessSutda::OnJoinRematchGame(const char* szID)		//유저가 재경기에 참가했다고 알려줌
{
	int pnum = GetPlayerPNum( szID );

	if( pnum == -1 ) return;

	if( g_PLAYER(pnum)->JoinState == EGAME_JOININ && g_PLAYER(pnum)->bFold )
	{
		g_PLAYER(pnum)->bFold = FALSE;

		//유저가 참여했다고 알려준다.
		g_pGameView->m_AniQue.Ani_Insert(ANIMATION_ETCEFFECT, g_cUIData.m_listResultStampPos[pnum], 0, 0, 2, FALSE, SND44, NULL, 5 );
	}

}


BOOL CGameProcessSutda::IsHiddenRound()		///마지막 히든 라운드인가?
{
	if (IsRematchGame())
	{
		return TRUE;
	}
	
	return (g_Poker.nRound == 2);
	
}

//규제안 예약기능 추가로 인해 GameProcess에 추가된 가상 함수
void CGameProcessSutda::SendCallButton()
{
	SendCallButton(true);
}

void CGameProcessSutda::SendCheckButton()
{
	SendCheckButton(true);
}

void CGameProcessSutda::SendDieButton()
{
	SendDieButton(true);
}

void CGameProcessSutda::SendHalfButton()
{
	SendHalfButton(true);
}

void CGameProcessSutda::SendDdaButton()
{
	SendDdaButton(true);
}

void CGameProcessSutda::SendPingButton()
{
	SendPingButton(true);
}