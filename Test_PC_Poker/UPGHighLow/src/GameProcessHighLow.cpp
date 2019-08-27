// GameProcessHighLow.cpp: implementation of the CGameProcessHighLow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameProcessHighLow.h"
#include "GlobalGame.h"
#include "SelectCardHighLow.h"
#include "AutoBetSetUpWnd.h"
#include "CommMsgDef_Game.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameProcessHighLow::CGameProcessHighLow(CGame *pGame)
: CGameProcess(pGame)
{
	m_pPage = NULL;
	m_bOnceInit = FALSE;
	m_i64JackPotMoney = 0;
	m_i64OldJackPotMoney = 0;
	Clear();

	//사업팀 요청에 의해서 230으로 입력. 
	//2012.7.10 류승민
	g_cUIData.m_dwStartGameDealingTimeGab = 230 ; 
#ifdef UPGRADE_20120502_TIMING_TEST_CODE
//#ifdef _DEBUG
	g_cUIData.m_dwStartGameDealingTimeGab = GetPrivateProfileInt("HighLow", "BetDelayTime", 180, ".\\TimingTest.ini");
	g_cUIData.m_dwDealingTimeGab = GetPrivateProfileInt("HighLow", "CardStartGabTime", 220, ".\\TimingTest.ini");
//#endif //_DEBUG
#endif //UPGRADE_20120502_TIMING_TEST_CODE
}

CGameProcessHighLow::~CGameProcessHighLow()
{
	Clear();
}


// 칩을 그리는 함수
void CGameProcessHighLow::chip_draw_func(int PNum, INT64 Money)
{
	if(PNum < 0 || PNum > 4 || Money < 0) 
		return;	

	m_AllBetChip.SetMoney(GetRealBet());
	g_PLAYER(PNum)->OnSetChip(Money);
}


// 최대 카드 개수
int  CGameProcessHighLow::GetTotalCardNum()
{
	return 7;
}


// 마지막 베팅 금액
INT64 CGameProcessHighLow::GetLastBetMoney(int nPlayerNum, int betCmd)
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
void CGameProcessHighLow::SetWindCardEnd(BOOL bFirst)
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
INT64 CGameProcessHighLow::GetRaiseBat()
{
	INT64 nRaiseBat = 0;

	for(int i = 0; i < MAX_PLAYER; ++i)
	{
		nRaiseBat = max(nRaiseBat, g_Poker.RU[i].nNowBat);
	}

	return nRaiseBat;
}

// 실제 베팅 머니
INT64 CGameProcessHighLow::GetRealBet()
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
INT64 CGameProcessHighLow::GetTotalBet()
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
INT64 CGameProcessHighLow::GetMaxMoney(INT64 nTotal,int betkind )
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
void CGameProcessHighLow::Insert_EndingTextInformation(CString str, COLORREF tcolor)
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
void CGameProcessHighLow::Draw_TextInformationMsg()
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
void CGameProcessHighLow::Clear()
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
}

// 초기화
void CGameProcessHighLow::Init(NMBASE::GRAPHICGDI::CPage *ppage)
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
	for(i = 0 ; i < GetMaxNewPlayer() ; ++i )
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
int CGameProcessHighLow::GetCardOpneGuideIndex()
{	
	if( g_Config.nbShowHidden == TRUE ) return -1;
	if( m_NowCardOpenGuideIndex < 0 || m_NowCardOpenGuideIndex >= MAX_CARDOPENGUIDE ) return -1;

	return m_CardOpenGuide[ m_NowCardOpenGuideIndex ];
}

//오픈애니메이션추가
void CGameProcessHighLow::EndCardOpenGuide()
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

void CGameProcessHighLow::SetCardOpenGuideTime()
{
	m_bDrawCardOpenGuide = TRUE;
	m_dwCardOpenGuideTime = timeGetTime();
}


void CGameProcessHighLow::ResetFlg()
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


	//하이로우 전용
	m_bSelectBtnBackImage = 0;
	m_SelectHighLowSwing = 0;	
	m_bSentHLS = FALSE;
}

// 게임 데이터를 초기화 한다.
void CGameProcessHighLow::ResetGame()
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
	
	m_AllBetChip.Reset();

	m_nActiveBetBtnDelayTime = 0;
	g_SelectCard().Reset();	
	m_nBetCallMoneyClearCnt = 0;	//콜한금액들 지워주기

	g_pGameView->m_cGameViewBtnMananager.SetBetBtnType(g_Config.eBetBtnType);

	g_Poker.nImageBoss = -1;	

	//규제안 : 예약버튼 활성화 가능상태
	ReserveState(RESERVESTATE_BET_NONE);
	CanReserve(true);
	g_nCardTotalCount = 0 ; // 카드 세팅 비교값을 0으로 초기화.
}


// 모든 프로세스는 이곳에서 한다.
void CGameProcessHighLow::OnTimer()
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

	m_AllBetChip.OnTimer();


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
}

BOOL CGameProcessHighLow::OnMouseMove(int x, int y)
{
	g_SelectCard().OnMouseMove(x,y);

	for( int i = 0; i < MAX_PLAYER ; ++i)
	{			
		g_PLAYER(i)->OnMouseMove(x,y);
	}

	return FALSE;
}

BOOL CGameProcessHighLow::OnLButtonDblclk(int x, int y)
{
	return FALSE;
}

BOOL CGameProcessHighLow::OnLButtonUp(int x, int y)
{
	g_SelectCard().OnLButtonUp(x,y);
	for( int i= 0 ; i < MAX_PLAYER ; i++ )
	{
		g_PLAYER(i)->OnLButtonUp(x,y);
	}
	
	return FALSE;
}

BOOL CGameProcessHighLow::OnLButtonDown(int x, int y)
{

	g_SelectCard().OnLButtonDown(x,y);
	m_cPlayer[0].OnLButtonDown(x,y);

	return FALSE;
}

void CGameProcessHighLow::OnGameKey(int nVKey, int nextended, int repeat)	
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
		CMadeEffect *pObj = NULL;
		SAFE_NEW( pObj, CMadeEffect );
		pObj->Init(4);
		g_pGameView->m_cAniSprListManager.ADDObject(pObj);
	}
#endif

	//채팅창변경
	if ( nVKey == rule::SKEY_TAB ) 
	{	
		g_pChatWinMan->SelectNextTab();
		return;
	}

	m_cPlayer[0].OnGameKey(nVKey, nextended, repeat);	
}

void CGameProcessHighLow::OnCheckTurnLimitTimeBase()
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
						SendDieButton();						
					}
				}
				//올인 베팅을 하고 있는 유저 체크,콜
				else
				{					
					if( g_pGameView->m_cGameViewBtnMananager.m_bBCall )
					{
						SendCallButton();
					}
					else if( g_pGameView->m_cGameViewBtnMananager.m_bBCheck )
					{
						SendCheckButton();
					}										
				}
			}
		}
	}	
}


// 카드 애니가 끝났을때 클라에서 보낸값을 다시 서버에서 보내준다.
void CGameProcessHighLow::EndWindCard()
{
	g_pGameView->m_cPlayerDrawManager.SetStartTimer();	
	
	if( m_BetCount == -1 )
	{
		m_BetCount = 0;
	}	

	if(	g_Poker.nState == RSTATE_PREPARESTART )
	{
		for(int i = 0; i < MAX_PLAYER; ++i) 
		{
			if( strlen(m_cPlayer[i].UI.ID)>0 && m_cPlayer[i].JoinState != EGAME_JOINOUT)
			{				
				g_pGameView->m_cPlayerDrawManager.SetSelectCard(i, true);
			}
		}
	}

	// 시작준비 모드이다(카드선택모드)
	if(	(g_Poker.nState == RSTATE_PREPARESTART || g_Poker.nState == RSTATE_EXTRAFIRSTBET ) && strlen(m_cPlayer[0].UI.ID) > 0 && m_cPlayer[0].JoinState != EGAME_JOINOUT )	
	{
		for(int i = 0; i < MAX_PLAYER; ++i)
		{
			if( strlen(m_cPlayer[i].UI.ID)>0 && m_cPlayer[i].JoinState != EGAME_JOINOUT)
			{
				g_pGameView->m_cPlayerDrawManager.SetSelectCard(i, true);
			}
		}		

		if( m_cPlayer[0].bFold == 0 && !g_ObserverMan.IsMyObserver() )
		{
			g_SelectCard().ShowSelectCard();
			g_SelectCard().SetChoiceCard(true);

			// 전광판 출력
			if(g_RI.FormKind == NORMAL_GAME)
			{
				g_pGameView->m_cDisplayBoard.SetText(EDBT_OPENCARD_CHOICE);
			}
			else
			{
				g_pGameView->m_cDisplayBoard.SetText(EDBT_SELECTCARD);
			}
		}		

		return;
	}

	// 레이즈모드이다
	if(g_Poker.nState == RSTATE_STARTGAME)
	{ 
		// 전광판 출력
		if(g_Poker.nAutoBetCmd > 0)
		{
			g_pGameView->m_cDisplayBoard.SetText(EDBT_AUTOBETTING);
		}
		else
		{
			int nRound = g_Poker.nRound;
			switch(nRound)
			{
			case 1:	// 4구
			case 2:	// 5구
			case 3:	// 6구
				g_pGameView->m_cDisplayBoard.SetText(ENUM_DISPLAY_BOARD_TEXT((int)EDBT_4BETTING + nRound - 1));		// -1을 해주는 이유? 디스플레이 명을 맞추기 위함
				break;
			case 4:	// 히든
				g_pGameView->m_cDisplayBoard.SetText(EDBT_HIDDENBETTING);
				break;
			}
		}

		// 베팅시작 (카드 다 날리고 베팅 시작)
		// 타이머 보이기

		g_pGameView->m_cPlayerDrawManager.SetStartTimer();

		m_nCurPlayer = g_Poker.CurPlayer;
		int nPlayNum = GetPNum_ByServPN(m_nCurPlayer);

		// 규제안 추가 카드세팅으로 딜링 일때는 턴 표시 안함.
		if ( CheckReceivedAllCards() )
		{
			g_pGameView->m_cPlayerDrawManager.SetTurn(nPlayNum);
		}		

		if(!g_ObserverMan.IsMyObserver() && m_cPlayer[0].ServPNum == m_nCurPlayer)
		{			
			g_pGameView->m_cGameViewBtnMananager.SetShowBetMony(); //< CYS 100705 > 교체
			g_pGameView->m_cGameViewBtnMananager.SetButtonClick(false);

			// 자동베팅시에는 베팅버튼 활성화 딜레이를 따로 설정한다.
			if (g_Poker.nAutoBetCmd > 0)
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelayInAutoBet(0, 0);
			else
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelay(0,0);
		}	

		// 규제안 예약 베팅 추가.(방장이 아닌 유저는 버튼 활성화가 안옴)
		if(!g_ObserverMan.IsMyObserver() && m_cPlayer[0].ServPNum != m_nCurPlayer)
		{
			// 자동베팅시에는 베팅버튼 활성화 딜레이를 따로 설정한다.
			if (g_Poker.nAutoBetCmd > 0)
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelayInAutoBet(0, 0);
			else
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelay(0,0);
		}

		g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(false);
		
		// 자신의 카드위에 족보명을 보위기 위함
		g_pGameView->m_cPlayerDrawManager.MyJokboNameUpdate();

		//규제안 예약베팅 셋팅 초기화
		ReserveState(RESERVESTATE_BET_NONE);
		//규제안 :  예약 가능여부 설정
		if(CheckReceivedAllCards())
		{
			CanReserve(true);
		}		
	}
}

//////////////////////////////////////////////////////////////////////////
/*
메세지 처리 
*/
//////////////////////////////////////////////////////////////////////////

// 방만들기 허가
bool CGameProcessHighLow::Accept_CreateRoom(void *pMsgParam) 
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

	if(g_pGameView)
	{
		g_pGameView->m_cGameHelper.OnEnterRoom();
	}
	
	// 게임 정보 변경 해주기 
	g_MyGameInfo.nGameMode = g_RI.nGameIndex;
	
	// 원샷 충전
	g_cOneShotCharge.OnEnterRoom();

	// 초기화
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
		if( g_RI.llInMoney > g_PLAYER(0)->UI.GetMoney() )
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
bool CGameProcessHighLow::Accept_EnterRoom(void  *pMsgParam) 
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

	ResetGame();

	g_MyPN = 0;
	int MyID = -1;
	int MasterID = -1;

	g_RI.nGameIndex			= pMsg->EI->Ri.nGameIndex;
	g_RI.FormKind			= pMsg->EI->Ri.FormKind; // 1 : CHOICE_GAME	
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
		//설정머니보다 자산이 많을경우 설정 머니로 셋팅한다.
		if( pMsg->EI->Ri.llInMoney > g_PLAYER(0)->UI.GetMoney() )
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
	
	if(g_pGameView)
	{
		g_pGameView->m_cPlayerDrawManager.OnEnterRoom();	// 플레이어정보
		g_pGameView->m_cGameHelper.OnEnterRoom();			// 족보뷰어
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
	{
		// 규제안 추가 카드세팅으로 딜링 일때는 턴 표시 안함.
		if ( CheckReceivedAllCards() )
		{
			g_pGameView->m_cPlayerDrawManager.SetTurn(GetPNum_ByServPN(m_nCurPlayer));
		}	
	}		

	m_AllBetChip.SetMoney(GetRealBet());	
	
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

				m_cPlayer[nPlayNum].bFold		 = pMsg->EI->Pg.PS[i].bFold;
				m_cPlayer[nPlayNum].SetCardTotal(nCardTotal);
				m_cPlayer[nPlayNum].SetOpenCardCount(nOpenTotal);

				for(int k = 0; k < nCardTotal; ++k) 
				{
					m_cPlayer[nPlayNum].SetAddCard(g_nDUMY_CARD); // 카운트 자동증가

					// 게임도중 입장한 경우 다른 유저가 이미 받은 카드는 카드의 End처리를 해줘야 화면에 보인다.
					m_cPlayer[nPlayNum].m_cMyCard[k].SetMoveEnd(true);
					
					// 카드를 보이게한다.
					m_cPlayer[nPlayNum].ShowCard(k);					
				}

				// 카드선택중이고 초이스 모드이면 더미를 하나더 추가해 준다.( 카드 선택중에 관전자로 입장하면 카드가 안보이는 문제 )
				if(g_ObserverMan.IsMyObserver() && nCardTotal == 3 && g_RI.FormKind == CHOICE_GAME )
				{
					m_cPlayer[nPlayNum].SetAddCard(g_nDUMY_CARD);

					// 게임도중 입장한 경우 다른 유저가 이미 받은 카드는 카드의 End처리를 해줘야 화면에 보인다.
					m_cPlayer[nPlayNum].m_cMyCard[3].SetMoveEnd(true);

					// 카드를 보이게한다.
					m_cPlayer[nPlayNum].ShowCard(3); 
				}


				// 게임중이면서 오프된 카드가 있고 패를 돌리는 중
				if(g_Poker.nState > RSTATE_DISTRIBUTECARD && nOpenTotal > 0 && !m_cPlayer[nPlayNum].bFold)
				{
					if(nOpenTotal <= 4) 
					{
						for(int k = 0; k < nOpenTotal; ++k)
						{
							m_cPlayer[nPlayNum].m_cMyCard[k+2].SetCardNum(g_Poker.PS[i].nCard[k]); // 값 변경
							m_cPlayer[nPlayNum].SetCardFrontShow(k+2); // 보이기

							// 게임도중 입장한 경우 다른 유저가 이미 받은 카드는 카드의 End처리를 해줘야 화면에 보인다.
							m_cPlayer[nPlayNum].m_cMyCard[k+2].SetMoveEnd(true);
							m_cPlayer[nPlayNum].ShowCard(k);					
						}
					}
					else 
					{	// 모두 카드 오픈
						// 히든 카드
						m_cPlayer[nPlayNum].m_cMyCard[0].SetCardNum(g_Poker.BC[i].nCard[0]);
						m_cPlayer[nPlayNum].m_cMyCard[1].SetCardNum(g_Poker.BC[i].nCard[1]);
						m_cPlayer[nPlayNum].m_cMyCard[6].SetCardNum(g_Poker.BC[i].nCard[2]);
						m_cPlayer[nPlayNum].SetCardFrontShow(0);
						m_cPlayer[nPlayNum].SetCardFrontShow(1);
						m_cPlayer[nPlayNum].SetCardFrontShow(6);

						for(int k = 0; k < nOpenTotal; ++k)	
						{
							m_cPlayer[nPlayNum].m_cMyCard[k+2].SetCardNum(g_Poker.PS[i].nCard[k]); // 값 변경
							m_cPlayer[nPlayNum].SetCardFrontShow(k+2); // 보이기

							m_cPlayer[nPlayNum].m_cMyCard[k+2].SetMoveEnd(true);
							m_cPlayer[nPlayNum].ShowCard(k);					
						}
					}						
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


	
	return true;
}


// 유저가 방에 입장 알림
void CGameProcessHighLow::User_EnterRoom(void *pMsgParam)
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

		// 규제안 입장 유저의 손실머니 정보 추가.
		g_pGameView->m_cPlayerDrawManager.OnEnterRoomLimitMoney(pnum, *pMsg->llMaxLossMoney);	

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
void CGameProcessHighLow::User_OutRoom(void *pMsgParam) 
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


void CGameProcessHighLow::GameStart(STARTINFO *pSC)
{
	if(pSC == NULL)
		return;	

	//규제안 : 최초 맥스 활성 여부 초기화 : 맥스이펙트
	for (int i=0; i<MAX_PLAYER; i++)
	{
		GM().g_bFirstMax[i] = false;
	}

	// 룰을 초기화 한다.	
	m_cSevenRuleManager.Init();
	m_cSevenRuleManager.ClearAll();
	g_pGameView->m_cTotalBetMoney.Start();

	g_RI.State = EGAME_PLAYING;

	// 레이스 베팅 비율 게임 시작 할때 만 갱신 한다.
	g_fRaceBetRatio = (float) pSC->fRaceBetRatio;

	if( g_RI.CurSeedMoney != pSC->nSeedMoney )
	{
		g_RI.CurSeedMoney = pSC->nSeedMoney;		
		g_pGameView->m_cTitle.MakeTitle();
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
	INT64 i64TempBetmoney = 0;

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

			/*char szTemp1[256] = {0, };
			sprintf(szTemp1, "------ GameStart :: PlayerIndex: %d JoinState:%d PlayState:%d \n", nPlayNum, pSC->Ps[i].JoinState, pSC->Ps[i].PlayState);
			OutputDebugString(szTemp1);*/

			//히스토리 관련 머니 셋팅 
			if(!g_ObserverMan.IsMyObserver() && m_cPlayer[0].JoinState == EGAME_JOININ && m_cPlayer[0].PlayState == EPLAYER_GAMEPLAYING && nPlayNum == 0 )
			{		
				m_cPlayer[0].nRoundPrevMoney = m_cPlayer[0].UI.GetMoney();
			}
			//yoo
			if(1 == pSC->Ps[i].nState){
				g_pGameView->m_cPlayerDrawManager.SetLimitMoney(nPlayNum, true);
			}

			//MAX방 반영 전 루틴
			//INT64 i64TempBetMoney = m_cPlayer[nPlayNum].UI.GetMoney() - i64BetMoney;
			//m_cPlayer[nPlayNum].UI.SetMoney( i64TempBetMoney );//100;
			////m_cPlayer[nPlayNum].UI.PMoney -=  i64BetMoney;//100;

			//MAX방 반영후 제한방 체크 로직 추가 함수 호출
			INT64 i64TempBetMoney = m_cPlayer[nPlayNum].GetNowMoney() - i64BetMoney;
			m_cPlayer[nPlayNum].SetNowMoney( i64TempBetMoney );
			
			g_Poker.RU[i].nPMoney = m_cPlayer[nPlayNum].GetNowMoney();
			m_cPlayer[nPlayNum].nSndFxKind = pSC->Ps[i].nSndFxKind;

			// 유저 카드 셋팅 3장
			if( nPlayNum == 0)
			{
				if(!g_ObserverMan.IsMyObserver())
				{
					GM().GetMyInfo()->JoinState = m_cPlayer[0].JoinState;
					GM().GetMyInfo()->PlayState = m_cPlayer[0].PlayState;
					GM().GetMyInfo()->bFold		= m_cPlayer[0].bFold;
					GM().GetMyInfo()->ServPNum  = m_cPlayer[0].ServPNum;
				}

				m_cPlayer[0].SetAddCard(pSC->nCard[0]);
				m_cPlayer[0].SetAddCard(pSC->nCard[1]);
				m_cPlayer[0].SetAddCard(pSC->nCard[2]);

				if( g_RI.FormKind == CHOICE_GAME )
				{
					m_cPlayer[0].SetAddCard(pSC->nCard[3]);
				}				

				g_SelectCard().SetCard(g_RI.FormKind, pSC->nCard);
			}
			else
			{
				m_cPlayer[nPlayNum].SetAddCard(g_nDUMY_CARD); // 52
				m_cPlayer[nPlayNum].SetAddCard(g_nDUMY_CARD);
				m_cPlayer[nPlayNum].SetAddCard(g_nDUMY_CARD);

				if( g_RI.FormKind == CHOICE_GAME )
				{
					m_cPlayer[nPlayNum].SetAddCard(g_nDUMY_CARD);
				}
			}

			if( g_RI.FormKind == CHOICE_GAME )
			{
				m_cPlayer[nPlayNum].SetCardTotal(4);
			}
			else
			{
				m_cPlayer[nPlayNum].SetCardTotal(3);	
			}

			m_cPlayer[nPlayNum].SetOpenCardCount(0); 

			g_Poker.RU[i].bInUse	= TRUE;
			g_Poker.RU[i].nRealBat	= i64BetMoney;

			// 기본 배팅 칩을 던짐
			chip_draw_func(nPlayNum, g_Poker.RU[i].nRealBat); // 여기서의 10은 단위다!!
			i64TempBetmoney += g_Poker.RU[i].nRealBat;

			m_nStartUserNum++;
		}
	}	

	// 베팅칩 머니 세팅
	m_AllBetChip.SetMoney(i64TempBetmoney);	

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

	g_DebugStartTick = timeGetTime();

	// 자리 지킴이
	g_SeatKeeper.OnStartGame();	

	g_cPromotionMan.OnStartGame();
	g_cVipJackpot.OnStartGame();

	PBPlayEffectSound(SND_START);

	Card_Distribute();	
}


// 카드 분배(게임시작 및 Raise에서 호출)
void CGameProcessHighLow::Card_Distribute()
{
	if(!IsHighLow() || RSTATE_NONE == g_Poker.nState) 
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
	if( g_RI.FormKind == CHOICE_GAME)
	{
		nGameTypeToCardCount = 4; // 초이스 카드
	}
	else
	{
		nGameTypeToCardCount = 3; // 노말 카드
	}

	// 카드 이동중 세팅
	g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(true);


	// 카드가 한장씩 나와야 하기때문에 누적 계산을 사용한다.
	int nDeleyCount = 0;
	CMyCard* pLastCard = NULL;// 마지막 카드 
	DWORD dwDelay = 0;

	// 초이스 모드인지 체크해서 한장 또는 4장의 카드를 분배 하느냐를 구분한다.
	if(!g_Poker.bDistCard)
	{
		int lastp  = 0;
		int lastci = 0;

		// 처음 4장을 분배한다.
		for( int j = 0; j < nGameTypeToCardCount; ++j)	
		{
			for(int i = nPlayerNumber; i < (MAX_PLAYER + nPlayerNumber); ++i) 
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
						pMyCard->GetMove().StartMoveMSprite(&g_sprCardDealing, CPoint(0,0), pMyCard->GetPos(), (nIndex * DEALINGCARDGAP) + nGameTypeToCardCount, dwDelay, 0, g_cUIData.m_fStartGameDealingCardSpeed);

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
			pLastCard->SetEvent(EVENT_THREEFLYEND, lastci, lastp); // 3장 마지막 까지 날렸으면 서버로
			SetWindCardEnd(TRUE);
		}
	}
	else
	{
		int nNum = 0;		
		nNum = m_cPlayer[nPlayerNumber].GetCardTotal() - 1;	// 선의 카드 수를  따른다. 초이스

		// 카드 한장씩 분배한다.
		for(int i = nPlayerNumber; i < (MAX_PLAYER + nPlayerNumber); ++i) 
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
					pMyCard->GetMove().StartMoveMSprite(&g_sprCardDealing, CPoint(0,0), pMyCard->GetPos(), (nIndex * DEALINGCARDGAP) + nNum, dwDelay, 0, g_cUIData.m_fDealingCardSpeed);
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
	}	
}


void CGameProcessHighLow::Raise(POKERGAME *pMsg)
{
	// 베팅 버튼 및 금액 관련된 처리를 담당한다.
	RaiseBase(pMsg);
	g_pGameView->m_cGameViewBtnMananager.Reset();
	
	// 현재 순서가 카드 날리기 인가?
	if( pMsg->bDistCard )
	{ 		
		int nMaxCard = 0;

		//규제안 관련하여 예약기능 추가 2014.01
		ReserveState(RESERVESTATE_BET_NONE);
		CanReserve(true);

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


				//규제안 : 카드세팅 현재 받은 카드 개수 설정
				g_nCardTotalCount = nCardTotalCount;


				// 자기 자신 처리
				if(nPlayNum == 0)	
				{
					// 1 한번에 4장
					if(nCardTotalCount == 4) 
					{
						// 관전자 일때.
						if(g_ObserverMan.IsMyObserver()) 
						{
							// 2가지 경우 = 3장 나누어준 상태에서 방입장 카드숫자 -1 하지 않는당
							//if(m_bGameStart && g_RI.FormKind == CHOICE_GAME )	// 4장의 카드를 받아 한장을 버리는 초이스 게임 룰
							//{	
							//	m_cPlayer[nPlayNum].m_cMyCard[3].Clear();
							//	int nCount = m_cPlayer[nPlayNum].GetCardTotal();
							//	m_cPlayer[nPlayNum].SetCardTotal(nCount-1);
							//} 

							if( g_RI.FormKind == CHOICE_GAME )
							{
								m_cPlayer[nPlayNum].m_cMyCard[2].SetCardNum(pMsg->PS[i].nCard[0]);	// 값 변경
								m_cPlayer[nPlayNum].m_cMyCard[3].SetCardNum(pMsg->PS[i].nCard[1]);	// 값 변경

								for(int k = 2; k < nCardTotalCount; ++k)
								{
									m_cPlayer[nPlayNum].SetCardFrontShow(k);  
								}
							}
							else
							{
								m_cPlayer[nPlayNum].m_cMyCard[2].SetCardNum(pMsg->PS[i].nCard[0]);	// 값 변경

								for(int k = 2; k < nCardTotalCount; ++k)
								{
									m_cPlayer[nPlayNum].SetCardFrontShow(k);  
								}

								m_cPlayer[nPlayNum].SetAddCard(pMsg->PS[i].nCard[1]);				// 값 등록
							}
						}
						else
						{
							m_cPlayer[0].m_cMyCard[2].SetCardNum(pMsg->PS[i].nCard[0]);			// 값 변경

							// 앞면(보이기) 자신은 모두 보인다.
							for(int k = 0; k < (nCardTotalCount - 1); ++k) 
							{
								m_cPlayer[0].SetCardFrontShow(k);
							}

							/// 3번째 장 위쪽으로 그린다
							CPoint pt;
							pt.x = m_cPlayer[0].m_cMyCard[2].GetPos().x;

							if( g_Config.bCardArray == FALSE )
							{
								CPoint ptPositon = m_cPlayer[0].m_cMyCard[2].GetPos();

								ptPositon.y -= POKER7_CARD_GAB_Y;

								m_cPlayer[0].m_cMyCard[2].SetCardPos(ptPositon.x, ptPositon.y);
							}

							pt.y = m_cPlayer[0].m_cMyCard[2].GetPos().y;
							m_cPlayer[0].m_cMyCard[2].GetMove().SetCurPos(pt);
							m_cPlayer[0].SetAddCard( pMsg->PS[i].nCard[1]);				// 4번째 카드 등록
						}

						m_cPlayer[0].SetChioceCardMove(2);
					}
					else if(nCardTotalCount < CARD_TOTALCOUNT) // 히든카드 전
					{
						if(nCardOpenCount > 0)
						{
							m_cPlayer[0].SetAddCard( pMsg->PS[i].nCard[nCardOpenCount-1]); // 4번째 카드 등록
						}
					}
					else if(nCardTotalCount == CARD_TOTALCOUNT) // 히든카드
					{
						m_cPlayer[0].SetAddCard( g_nDUMY_CARD);
					}
				}
				else
				{
					if(nCardTotalCount == 4)
					{
						if(g_ObserverMan.IsMyObserver()) 
						{
							if(g_RI.FormKind == CHOICE_GAME )
							{
								m_cPlayer[nPlayNum].m_cMyCard[2].SetCardNum(pMsg->PS[i].nCard[0]);	// 값 변경
								m_cPlayer[nPlayNum].m_cMyCard[3].SetCardNum(pMsg->PS[i].nCard[1]);	// 값 변경
							}
							else
							{
								m_cPlayer[nPlayNum].m_cMyCard[2].SetCardNum(pMsg->PS[i].nCard[0]);	// 값 변경
								m_cPlayer[nPlayNum].SetAddCard( pMsg->PS[i].nCard[1]);				// 값 등록
							}							
						}
						else
						{
							m_cPlayer[nPlayNum].m_cMyCard[2].SetCardNum(pMsg->PS[i].nCard[0]);	// 값 변경
							m_cPlayer[nPlayNum].SetAddCard( pMsg->PS[i].nCard[1]);				// 값 등록
						}
						
						for(int k = 2; k < (nCardTotalCount - 1); ++k)
						{
							m_cPlayer[nPlayNum].SetCardFrontShow(k);
						}

						m_cPlayer[nPlayNum].SetChioceCardMove();
					}
					else if(nCardTotalCount < CARD_TOTALCOUNT)	// 히든카드 전
					{
						if(nCardOpenCount > 0)
						{
							m_cPlayer[nPlayNum].SetAddCard( pMsg->PS[i].nCard[nCardOpenCount - 1]); // 4번째 카드 등록
						}
					}
					else if(nCardTotalCount == CARD_TOTALCOUNT) // 히든카드
					{
						m_cPlayer[nPlayNum].SetAddCard( g_nDUMY_CARD); // 4번째 카드 등록

						SetCardOpenGuideTime();			//오픈애니메이션추가
					}
				}

				m_cPlayer[nPlayNum].SetCardTotal(nCardTotalCount);
				m_cPlayer[nPlayNum].SetOpenCardCount(nCardOpenCount);

				if(nMaxCard < nCardTotalCount) 
				{
					nMaxCard = nCardTotalCount;
				}
			}

			switch(nMaxCard)
			{
			case 4:
			case 5:
			case 6:
				g_pGameView->m_cDisplayBoard.SetText(ENUM_DISPLAY_BOARD_TEXT((int)EDBT_4CARD + nMaxCard - 4));
				break;
			case 7:
				g_pGameView->m_cDisplayBoard.SetText(EDBT_HIDDENCARD);
				break;
			}
		}

		Card_Distribute();
		g_pGameView->m_cGameHelper.SetAllUserCardInfo();

		//리미트배팅 초기화( 새카드 오는 시점 )
		g_pGameView->m_cGameViewBtnMananager.SetLastCmdBackup( RAISE_LAST_CMD_NULL, true );
	}	

	// 사이드팟 
	if(g_pGameView)
	{
		g_pGameView->m_cSidePotMoney.Check();
	}
}

// 베팅 버튼 및 금액 관련된 처리를 담당한다.
void CGameProcessHighLow::RaiseBase(POKERGAME *pMsg)
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
			////g_PLAYER(nPlayerNum)->UI.PMoney = g_Poker.RU[i].nPMoney;// 사용자 머니 셋

			//MAX방 함수 호출
			g_PLAYER(nPlayerNum)->SetNowMoney( g_Poker.RU[i].nPMoney );// 사용자 머니 셋

			if( g_Poker.PS[i].bFold )
			{
				g_PLAYER(nPlayerNum)->bFold = TRUE;
			}

			// 추가 
			if(g_PLAYER(nPlayerNum)->GetNowMoney() < 0) 
			{
				g_PLAYER(nPlayerNum)->SetNowMoney( 0 );
			}

			g_PLAYER(nPlayerNum)->UI.nIcon = GetMoneyGrade(g_PLAYER(nPlayerNum)->UI.GetTotalMoney());
		}
	}

	CPoint cpEffectPos=CPoint(0, 0);
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

			if(2==g_Poker.RU[LastPnum].bMaxBet){
				g_pGameView->m_cPlayerDrawManager.SetLimitMoney(nSNum, true);
			}
			//규제안 : 맥스 커맨드 추가 : 맥스이펙트
			if (g_Poker.RU[LastPnum].bMaxBet && GM().g_bFirstMax[LastPnum] == false)
			{
				// 맥스이펙트
				CBettingEffect *pObj = new CBettingEffect(); 
				pObj->Init(nSNum,ANF_BET_CMD, ANF_BET_MAX);			
				cpEffectPos=pObj->GetEffectPos();
				g_cVipJackpot.OnBetGame(cpEffectPos, false);			
				g_pGameView->m_cAniSprListManager.ADDObject(pObj);

				g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);
				if(g_PLAYER(nSNum)->GetNowMoney() == 0 )
				{
					g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
				}
				if(g_Poker.nRound == 4)
				{
					g_PLAYER(nSNum)->m_bLastHalf = TRUE;					
					SoundFxKindBig(nLastCmd,g_PLAYER(nSNum)->UI.Sex, true);
				}
				else
				{
					SoundFxKind(nLastCmd,g_PLAYER(nSNum)->UI.Sex, true);
				}	
				GM().g_bFirstMax[LastPnum] = true;
				//일반 배팅 커맨드
			}else{

			switch(nLastCmd)
			{
				// 다이
				case RAISE_LAST_CMD_FOLD: // foldUser 에서 처리
					{
						CBettingEffect *pObj = new CBettingEffect();
						pObj->Init(nSNum,ANF_BET_CMD, ANF_BET_DIE);		
						cpEffectPos=pObj->GetEffectPos();
						g_cVipJackpot.OnBetGame(cpEffectPos, false);				
						g_pGameView->m_cAniSprListManager.ADDObject(pObj);
					} break; 

				// 체크
				case RAISE_LAST_CMD_CHECK:
					{					
						if(g_Poker.bDistCard == TRUE)
							break;

						//규제안 작업 : 카드세팅 중이면 이펙트를 노출하지 않는다. 2014.01
						if ( CheckReceivedAllCards() )
						{
							SoundFxKind(nLastCmd,g_PLAYER(nSNum)->UI.Sex);

							CBettingEffect *pObj = new CBettingEffect();
							pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_CHECK);	
							cpEffectPos=pObj->GetEffectPos();
							g_cVipJackpot.OnBetGame(cpEffectPos, false);					
							g_pGameView->m_cAniSprListManager.ADDObject(pObj);
						}
					} break; 

				// 콜
				case RAISE_LAST_CMD_CALL: 
					{	
						if(g_Poker.bDistCard == TRUE)
							break;			


						//규제안 작업 : 카드세팅 중이면 이펙트를 노출하지 않는다. 2014.01

						char a = g_RI.cCardSetting;
						if ( CheckReceivedAllCards() )
						{
							CBettingEffect *pObj = new CBettingEffect();
							pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_CALL);			
							cpEffectPos=pObj->GetEffectPos();
							g_cVipJackpot.OnBetGame(cpEffectPos, false);			
							g_pGameView->m_cAniSprListManager.ADDObject(pObj);

							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);
							if(g_PLAYER(nSNum)->GetNowMoney() == 0 )
							{
								g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
							}
						}
					} break; 

				// 삥
				case RAISE_LAST_CMD_PING:  
					{	
						if(g_Poker.bDistCard == TRUE)
							break;				

						CBettingEffect *pObj = new CBettingEffect();
						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_PING);
						cpEffectPos=pObj->GetEffectPos();
						g_cVipJackpot.OnBetGame(cpEffectPos, false);		
						g_pGameView->m_cAniSprListManager.ADDObject(pObj);

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);
						if(g_PLAYER(nSNum)->GetNowMoney() == 0 )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}
					}
					break; 

				// 따당
				case RAISE_LAST_CMD_DADANG:
					{					
						if(g_Poker.bDistCard == TRUE)
							break;				

						CBettingEffect *pObj = new CBettingEffect();
						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_DADANG);	
						cpEffectPos=pObj->GetEffectPos();
						g_cVipJackpot.OnBetGame(cpEffectPos, false);					
						g_pGameView->m_cAniSprListManager.ADDObject(pObj);

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);
						if(g_PLAYER(nSNum)->GetNowMoney() == 0 )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}
					} break; 			

					// 풀
				case RAISE_LAST_CMD_FULL:
					{					
						if(g_Poker.bDistCard == TRUE)
							break;				

						CBettingEffect *pObj = new CBettingEffect();
						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_FULL);			
						cpEffectPos=pObj->GetEffectPos();
						g_cVipJackpot.OnBetGame(cpEffectPos, true);			
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
						if(g_Poker.bDistCard == TRUE)
							break;

						BOOL blasthalf = FALSE;

						// 히든
						if(g_Poker.nRound == 4)
						{
							blasthalf = TRUE;
							//하프를 쳤던 넘들만 족보를 강하게 불러준다.
							g_PLAYER(nSNum)->m_bLastHalf=TRUE;
						}

						CBettingEffect *pObj = new CBettingEffect();
						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_HALF,blasthalf);
						cpEffectPos=pObj->GetEffectPos();
						g_cVipJackpot.OnBetGame(cpEffectPos, false);		
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
						if(g_Poker.bDistCard == TRUE)
							break;

						BOOL blasthalf = FALSE;

						// 히든
						if(g_Poker.nRound == 4)
						{
							blasthalf = TRUE;
							//하프를 쳤던 넘들만 족보를 강하게 불러준다.
							g_PLAYER(nSNum)->m_bLastHalf = TRUE;
						}

						CBettingEffect *pObj = new CBettingEffect();
						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_ALLIN,blasthalf);
						cpEffectPos=pObj->GetEffectPos();
						g_cVipJackpot.OnBetGame(cpEffectPos, false);		
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
						cpEffectPos=pObj->GetEffectPos();
						g_cVipJackpot.OnBetGame(cpEffectPos, false);		
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
						cpEffectPos=pObj->GetEffectPos();
						g_cVipJackpot.OnBetGame(cpEffectPos, false);		
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
						cpEffectPos=pObj->GetEffectPos();
						g_cVipJackpot.OnBetGame(cpEffectPos, false);		
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
						cpEffectPos=pObj->GetEffectPos();
						g_cVipJackpot.OnBetGame(cpEffectPos, false);		

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
						cpEffectPos=pObj->GetEffectPos();
						g_cVipJackpot.OnBetGame(cpEffectPos, false);		
						g_pGameView->m_cAniSprListManager.ADDObject( pObj );

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni( nSNum, 0 );
						if( g_PLAYER(nSNum)->GetNowMoney() == 0 )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}
					}break;	
				}

				//배팅 컨맨드에 따른 사운드 재생 
				if( nLastCmd >= RAISE_LAST_CMD_CHECK && nLastCmd <= RAISE_LAST_CMD_ALLIN && g_Poker.bDistCard == FALSE )
				{
					// 히든
					if(g_Poker.nRound == 4)
					{
						g_PLAYER(nSNum)->m_bLastHalf = TRUE;					
						SoundFxKindBig(nLastCmd,g_PLAYER(nSNum)->UI.Sex, false);
					}
					else
					{
						SoundFxKind(nLastCmd,g_PLAYER(nSNum)->UI.Sex, false);
					}				
				}
			}
		}
		
		// 규제안 관련 작업으로 주석처리 2014.01 -> 추후반영
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
		// 규제안 추가 카드세팅으로 딜링 일때는 턴 표시 안함.
		if ( CheckReceivedAllCards() )
		{
			g_pGameView->m_cPlayerDrawManager.SetTurn(GetPNum_ByServPN(m_nCurPlayer));
		}		

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
					//규제안 관련하여 따당머니 주석 처리 2014.01 -> 추후반영
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

			// 자동베팅시에는 베팅버튼 활성화 딜레이를 따로 설정한다.
			if (g_Poker.nAutoBetCmd > 0)
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelayInAutoBet(0, 0);
			else
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelay(0,0);
		}

		g_pGameView->m_cAniSprListManager.BetCmd_ClearCmd(nPlayerNum, 20);
		
		return;
	}

	// 4구 실전 
	if(g_Poker.nState == RSTATE_EXTRAFIRSTBET) 
	{
		m_nCurPlayer = g_Poker.CurPlayer;
		// 규제안 추가 카드세팅으로 딜링 일때는 턴 표시 안함.
		if ( CheckReceivedAllCards() )
		{
			g_pGameView->m_cPlayerDrawManager.SetTurn(GetPNum_ByServPN(m_nCurPlayer));
		}	
		
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
			//g_PLAYER(bet)->OnSetBetDrawMoney(GetLastBetMoney(LastPlayer,g_Poker.RU[g_Poker.LastPlayer].nLastCmd), 0,g_Poker.RU[LastPlayer].nLastCmd);				
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

			//g_PLAYER(bet)->OnSetBetDrawMoney(GetLastBetMoney(LastPlayer,g_Poker.RU[g_Poker.LastPlayer].nLastCmd), 0,g_Poker.RU[LastPlayer].nLastCmd);	
		}

		g_pGameView->m_cPlayerDrawManager.SetStartTimer();
		g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(false);
		

		//배팅 시작(베팅 버튼 활성)의 핵심이다. m_nActiveBetBtnDelayTime 값으로 딜레이를 준다.
		if(!g_ObserverMan.IsMyObserver() && g_PLAYER(0)->ServPNum == m_nCurPlayer)
		{
			//g_PLAYER(0)->OnReSetBetDraw();
			g_pGameView->m_cGameViewBtnMananager.SetShowBetMony(); //< CYS 100705 > 교체
			g_pGameView->m_cGameViewBtnMananager.SetButtonClick(false);
			
			// 자동베팅시에는 베팅버튼 활성화 딜레이를 따로 설정한다.
			if (g_Poker.nAutoBetCmd > 0)
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelayInAutoBet(0, 0);
			else
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelay(0,0);
		}

		g_pGameView->m_cAniSprListManager.BetCmd_ClearCmd(nPlayerNum, 20);		
	}	
}


// 게임 오버
void CGameProcessHighLow::OnGameOver(GAMEOVERRESULT *pGOR)
{	
	NMBASE::SOUND::g_Sound.Stop(SND_HIDDEN_BGM);
	m_nCurPlayer = -1;
	
	// 게임 결과 정보 
	ZeroMemory(&m_GameOverResultData,sizeof(m_GameOverResultData));	
	g_pGameView->m_cGameHelper.Reset();

	//규제안 관련하여 예약기능 모두 초기화.
	ReserveState(RESERVESTATE_BET_NONE);
	CanReserve(false);

	g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();		// 버튼 초기화
	g_pGameView->m_cPlayerDrawManager.SetOpen(pGOR->bOpen);
	g_pGameView->m_cPlayerDrawManager.SetWinnerPNum(-1);
	g_pGameView->m_cPlayerDrawManager.SetTurn(-1);

	g_SelectCard().Clear();

	m_WinnerPNumData.Clear();

	m_WinnerPNumData.nWinnerNum = pGOR->nWinnerNum;
	m_WinnerPNumData.bOneWinner = m_WinnerPNumData.nWinnerNum > 1 ? FALSE : TRUE;
	memcpy( m_WinnerPNumData.WinnerSPNum , pGOR->nNowWinner , sizeof(m_WinnerPNumData.WinnerSPNum));

	//베팅 커멘드 지움
	g_pGameView->m_cAniSprListManager.BetCmd_DelDelay(55);

	memcpy(&g_Poker, &pGOR->Pg, sizeof(g_Poker));
	memcpy(&m_GameOverResultData, pGOR, sizeof(m_GameOverResultData));

	//규제안 관련하여 예약기능 추가 2014.01
	ReserveState(RESERVESTATE_BET_NONE);
	CanReserve(false);

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

	// 히든카드를 Show플레그 세팅
	g_pGameView->m_cPlayerDrawManager.SetHiddenCardShow(true);

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
			memcpy( &g_PLAYER(pnum)->UI.AllGameRecord, m_GameOverResultData.Ui[i].AllGameRecord, sizeof(m_GameOverResultData.Ui[i].AllGameRecord) );		
			g_PLAYER(pnum)->SetChangeRoomInfo(m_GameOverResultData.Cui[i]);

			//g_PLAYER(pnum)->UI.SetMoney( m_GameOverResultData.Ui[i].PMoney );		
			////g_PLAYER(pnum)->UI.PMoney		 = m_GameOverResultData.Ui[i].PMoney;		
			//Max방 제한 설정
			if( g_RI.bIsMaxRoom )
			{
				g_PLAYER(pnum)->SetNowMoney( m_GameOverResultData.Ui[i].llInMoney );	
			}
			else
			{
				g_PLAYER(pnum)->SetNowMoney( m_GameOverResultData.Ui[i].PMoney );	
			}

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

			TRACE( "%d. Player[ %s ] Fold[ %d ] JoinState[ %d ] PlayState[ %d ] PlayerPNum[ %d ]: ",
				i, pGOR->Ui[ i ].NickName, pGOR->Pg.PS[i].bFold, pGOR->Pg.PS[i].JoinState, pGOR->Pg.PS[i].PlayState, pnum );

			for ( int k = 0; k < CARD_TOTALCOUNT; k++ ) TRACE( "%d, ", m_cPlayer[ pnum ].m_cMyCard[ k ].GetCardNum() );

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
			
			for ( int k = 0; k < 3; k++ )
			{
				if ( g_Poker.BC[ i ].nCard[ k ] > 0 ) nCount++;
			}

			for ( int k = 0; k < 4; k++ )
			{
				if ( g_Poker.PS[ i ].nCard[ k ] > 0 ) nCount++;
			}
			
			if( nCount > 0 ) 
			{
				// 2018.09.27 이슈 발생으로 기권 오픈 기능 제거
				//m_cPlayer[ pnum ].bFoldOpen = true;
				
				//TRACE( "FoldOpen[ %s ] Fold[ %d ] \n", pGOR->Ui[ i ].NickName, pGOR->Pg.PS[ i ].bFold );
			}
		}
	}

	//TRACE( "=======================================================\n\n" );
//#endif
	
	// 7 PROCESS

	for(int i = 0; i < MAX_PLAYER; ++i)
	{
		if(strlen(pGOR->Ui[i].ID) > 0) //  겜참여자.
		{
			int nPlayNum = GetPlayerPNum(pGOR->Ui[i].ID);
			if(nPlayNum == -1)
				continue;	

			// 히든 카드 오픈
			if( g_pGameView->m_cPlayerDrawManager.IsOpen() && !pGOR->Pg.PS[i].bFold )
			{
				// 자기 자신
				if(nPlayNum == 0)
				{
					m_cPlayer[nPlayNum].m_cMyCard[0].SetCardNum(g_Poker.BC[i].nCard[0]); // 값 변경
					m_cPlayer[nPlayNum].m_cMyCard[1].SetCardNum(g_Poker.BC[i].nCard[1]); // 값 변경
					m_cPlayer[nPlayNum].m_cMyCard[6].SetCardNum(g_Poker.BC[i].nCard[2]); // 값 변경

					m_cPlayer[0].SetCardFrontShow(0);
					m_cPlayer[0].SetCardFrontShow(1);
					m_cPlayer[0].SetCardFrontShow(6);
				}
				else 
				{
					m_cPlayer[nPlayNum].m_cMyCard[0].SetCardNum(g_Poker.BC[i].nCard[0]); // 값 변경
					m_cPlayer[nPlayNum].m_cMyCard[1].SetCardNum(g_Poker.BC[i].nCard[1]); // 값 변경
					m_cPlayer[nPlayNum].m_cMyCard[6].SetCardNum(g_Poker.BC[i].nCard[2]); // 값 변경

					m_cPlayer[nPlayNum].SetCardFrontShow(0);
					m_cPlayer[nPlayNum].SetCardFrontShow(1);
					m_cPlayer[nPlayNum].SetCardFrontShow(6);
				}

				m_cPlayer[nPlayNum].SetCardResult();					
				m_cPlayer[nPlayNum].SetCaseSelect((EHIGHLOWSWINGCASE)m_GameOverResultData.Ui[i].CaseSel);
				m_cPlayer[nPlayNum].SetCaseResult((EHIGHLOWSWINGCASE)m_GameOverResultData.Ui[i].WinCase);
				m_cPlayer[nPlayNum].Draw_DarkCard();	


				//하이로우 선택 완료 한 효과(느낌표 이미지) 삭제
				m_cPlayer[nPlayNum].ClearEffectSpr(ANF_SELECTWINNERCASE_HI);
				
				
				//결과때 선택한 case의 이미지 효과 (하이로우 전용)

				//0번 위치 유저이고 승리한 경우는 선택한 case이미지 효과를 보여주지않는다. (승리효과랑 겹치기 때문)
				if (! (nPlayNum==0 && m_WinnerPNumData.IsMyVictory(m_cPlayer[0].ServPNum)))
				{					
					CResultSelectCaseEft_HI *pObj = new CResultSelectCaseEft_HI;
					if( pObj != NULL )
					{
						pObj->Init( nPlayNum, m_cPlayer[nPlayNum].GetCaseSelect() );
						m_cPlayer[nPlayNum].m_cPlayerEffectList.AddObject(pObj);
					}

				}
			} 
			else if( !g_pGameView->m_cPlayerDrawManager.IsOpen() && !pGOR->Pg.PS[i].bFold ) 
			{
				m_cPlayer[nPlayNum].m_cMyCard[0].SetCardNum(g_Poker.BC[i].nCard[0]); // 값 변경
				m_cPlayer[nPlayNum].m_cMyCard[1].SetCardNum(g_Poker.BC[i].nCard[1]); // 값 변경
				m_cPlayer[nPlayNum].m_cMyCard[6].SetCardNum(g_Poker.BC[i].nCard[2]); // 값 변경

				if( nPlayNum == 0 )
				{
					if( !g_ObserverMan.IsMyObserver() )
					{
						m_cPlayer[nPlayNum].SetCardFrontShow(0);
						m_cPlayer[nPlayNum].SetCardFrontShow(1);
						m_cPlayer[nPlayNum].SetCardFrontShow(6);
						m_cPlayer[nPlayNum].SetCardResult();
						m_cPlayer[nPlayNum].Draw_DarkCard();						
					}
					else
					{
						m_cPlayer[nPlayNum].SetCardBackShow(0);
						m_cPlayer[nPlayNum].SetCardBackShow(1);
						m_cPlayer[nPlayNum].SetCardBackShow(6);
					}
				}
				else
				{
					m_cPlayer[nPlayNum].SetCardBackShow(0);
					m_cPlayer[nPlayNum].SetCardBackShow(1);
					m_cPlayer[nPlayNum].SetCardBackShow(6);
				}				
			}

			// 본인카드오픈 상태에 따라서 최종 결과를 더 출력한다.
			if( m_cPlayer[nPlayNum].bFoldOpen )
			{
				m_cPlayer[nPlayNum].m_cMyCard[0].SetCardNum(g_Poker.BC[i].nCard[0]); // 값 변경
				m_cPlayer[nPlayNum].m_cMyCard[1].SetCardNum(g_Poker.BC[i].nCard[1]); // 값 변경
				m_cPlayer[nPlayNum].m_cMyCard[6].SetCardNum(g_Poker.BC[i].nCard[2]); // 값 변경
				
				m_cPlayer[nPlayNum].SetCardFrontShow(0);
				m_cPlayer[nPlayNum].SetCardFrontShow(1);
				m_cPlayer[nPlayNum].SetCardFrontShow(6);
				
				m_cPlayer[nPlayNum].m_cMyCard[2].SetCardNum(g_Poker.PS[i].nCard[0]); // 값 변경
				m_cPlayer[nPlayNum].m_cMyCard[3].SetCardNum(g_Poker.PS[i].nCard[1]); // 값 변경
				m_cPlayer[nPlayNum].m_cMyCard[4].SetCardNum(g_Poker.PS[i].nCard[2]); // 값 변경
				m_cPlayer[nPlayNum].m_cMyCard[5].SetCardNum(g_Poker.PS[i].nCard[3]); // 값 변경
				
				m_cPlayer[nPlayNum].SetCardFrontShow(2);
				m_cPlayer[nPlayNum].SetCardFrontShow(3);
				m_cPlayer[nPlayNum].SetCardFrontShow(4);
				m_cPlayer[nPlayNum].SetCardFrontShow(5);
				
				m_cPlayer[nPlayNum].SetCardResult();
				m_cPlayer[nPlayNum].Draw_DarkCard();
			}
		}
	}

	// 게임 결과를 전광판에 출력한다.
	if (m_GameOverResultData.WinCase==4) 
	{
		//승자가 2명
		g_pGameView->m_cDisplayBoard.SetText(EDBT_RESULT_HILOW_2WIN, (LPARAM)&m_GameOverResultData); 
	}
	else
	{
		//승자가 1명
		g_pGameView->m_cDisplayBoard.SetText(EDBT_RESULT_HILOW_1WIN, (LPARAM)&m_GameOverResultData); 
	}	


	// 결과 출력 Delay를 주기 위함 
	m_nResultDelayCount = 20;
	m_bResultDelayStart = TRUE;

	g_pGameView->m_cPlayerDrawManager.CheckResultValue();	//결과화면에 찍을 때 필요한 획득(차감)머니계산 및 유저들 족보명 구해놓기
}


void CGameProcessHighLow::OnResultGameOver()
{
	GM().SetWaitResponse(FALSE);

	g_RI.State		= EGAME_WAITING;
	m_bGameStart	= FALSE;			// 게임 시작 플래그 세팅	

	//CPlayerDraw::DrawResult를 그리기 시작한다.
	g_pGameView->m_cPlayerDrawManager.SetGameOver(true, timeGetTime());

 

	//케이스별 승리
	int nFirstWinner = -1;
	int nSecondWinner = -1;
	switch(m_GameOverResultData.WinCase)
	{
	case 0:	//기권승
	case 1:	//하이승자 1명
		{
			//  PokerBase쪽 PlayerManger에 승자 번호를 넘겨준다.(서버에서 보내준 값을 넘겨줘야 한다.)
			nFirstWinner = m_GameOverResultData.nNowWinner[IDX_HL_1ST_WINNER];
			g_pGameView->m_cPlayerDrawManager.SetHighWinnerPNum(nFirstWinner);

		} break;
	case 2: //로우승자 1명
		{
			//  PokerBase쪽 PlayerManger에 승자 번호를 넘겨준다.(서버에서 보내준 값을 넘겨줘야 한다.)
			nFirstWinner = m_GameOverResultData.nNowWinner[IDX_HL_1ST_WINNER];
			g_pGameView->m_cPlayerDrawManager.SetLowWinnerPNum(nFirstWinner);

		} break;
	case 3:	//스윙승자 1명
		{
			//  PokerBase쪽 PlayerManger에 승자 번호를 넘겨준다.(서버에서 보내준 값을 넘겨줘야 한다.)
			nFirstWinner = m_GameOverResultData.nNowWinner[IDX_HL_1ST_WINNER];
			g_pGameView->m_cPlayerDrawManager.SetSwingWinnerPNum(nFirstWinner);
		} break;
	case 4: //승자 2명 (하이승자 1명, 로우승자 1명)
		{
			//  PokerBase쪽 PlayerManger에 승자 번호를 넘겨준다.(서버에서 보내준 값을 넘겨줘야 한다.)
			nFirstWinner = m_GameOverResultData.nNowWinner[IDX_HL_1ST_WINNER];
			g_pGameView->m_cPlayerDrawManager.SetHighWinnerPNum(nFirstWinner);

			//  PokerBase쪽 PlayerManger에 승자 번호를 넘겨준다.(서버에서 보내준 값을 넘겨줘야 한다.)
			nSecondWinner = m_GameOverResultData.nNowWinner[IDX_HL_2ND_WINNER];
			g_pGameView->m_cPlayerDrawManager.SetLowWinnerPNum(m_GameOverResultData.nNowWinner[IDX_HL_2ND_WINNER]);

		} break;

	}
	

	// 사이드팟 체크
	g_pGameView->m_cSidePotMoney.Check();


	MAP_RANKING mapRanking;
	for(int i=0; i< GetMaxPlayer(); i++)
	{
		if(strlen(m_GameOverResultData.Ui[i].ID)>0) //  겜참여자.
		{
			int pnum = GetPlayerPNum(m_GameOverResultData.Ui[i].ID);
			if(pnum == -1) 
				continue;

			int nPlayerRank = -1;
			if (i== nSecondWinner || i == nFirstWinner)
			{
				nPlayerRank = 1;
			}
			else
			{
				nPlayerRank = 2;
			}


			if(nPlayerRank < 0) 
				continue;

			mapRanking[nPlayerRank][pnum] = m_GameOverResultData.Ui[i].nResultMoney;
		}
	}
	g_pGameView->m_cSidePotMoney.SetGameOver(mapRanking);

	// 사이드팟 승자 세팅
	m_AllBetChip.SetWinner(&g_pGameView->m_cSidePotMoney);

	CString str  = "";
	CString strM = "";
	//CString strWinnerNick = GetPlayerNickName(nWinPlayerNum);

	//////////////////////////////////////////////////////////////////////////	
	//게임정보를 위한 룰 계산 [주의:전체 족보]	
	for(int i = 0 ; i < GM().GetMaxPlayer() ; ++i)
	{
		if(strlen(m_GameOverResultData.Ui[i].ID) > 0) //  겜참여자.
		{
			int nPlayNum = GetPlayerPNum(m_GameOverResultData.Ui[i].ID);
			if( nPlayNum == -1 )
				continue;

			if( m_cPlayer[nPlayNum].JoinState == EGAME_JOINOUT )
				continue;

			if( m_cPlayer[nPlayNum].PlayState == EPLAYER_GAMEEND )
				continue;

			if( m_cPlayer[nPlayNum].bFold )
				continue;

			int aCard[10] = {0, };

			for( int x = 0; x < m_cPlayer[nPlayNum].GetCardTotal(); ++x )
			{
				aCard[x] = m_cPlayer[nPlayNum].m_cMyCard[x].GetCardNum();
			}

			m_cSevenRuleManager.set_total_Card(nPlayNum, aCard, m_cPlayer[nPlayNum].GetCardTotal());
			m_cSevenRuleManager.get_cal_rule_user(nPlayNum);			
		}
	}

	m_cSevenRuleManager.get_cal_Winner_Client();

	// 윈 효과 
	const int DIE  = 2;
	const int WIN  = 1;
	const int LOSE = 0;
	

	// 승패 창에 들어갈 구조체
	GAMERESULTDATA WinEftData;	
	WinEftData.Clear();
	
	//스윙위너가 있을경우(포카드이상 이펙트 보여주기)
	int nWinPlayerNum = GetPNum_ByServPN(g_pGameView->m_cPlayerDrawManager.GetSwingWinnerPNum());
	if( nWinPlayerNum >= 0 && nWinPlayerNum < GM().GetMaxPlayer())
	{		
		
		WinEftData.wincase	 = WIN;
		WinEftData.pnum		 = nWinPlayerNum;
		WinEftData.valuekind = m_cSevenRuleManager.get_MyRule(nWinPlayerNum);

		//승패 창에 들어갈 족보 명	
		strcpy( WinEftData.str_value, m_cSevenRuleManager.m_cRule[nWinPlayerNum].get_RuleValue_to_Name3().operator LPCTSTR());		

		//아래 정보는 하이 족보 달성시 필요한 정보 
		strcpy( WinEftData.ID, m_cPlayer[nWinPlayerNum].UI.NickName);			
		WinEftData.nResultMoney = m_cPlayer[nWinPlayerNum].m_n64ResultMoney;			
		int cardcnt = 0 ;
		for(int i = 0 ; i < 5 ; ++i)
		{
			int cno = m_cSevenRuleManager.m_cRule[nWinPlayerNum].m_RuleComplteCard[i];
			if( cno > -1 )
			{
				WinEftData.nCard[i] = cno;
				cardcnt++;
			}
		}

		//포카드 이상의 카드를 그리기 위해 장수가 필요
		WinEftData.nTotalCardNum = cardcnt;	

		// 승패 이미지 그리기( 족보완성 이펙트)	
		if( WinEftData.valuekind >= CCardRule::N7C_QUADS && g_pGameView->m_cPlayerDrawManager.IsOpen() )
		{
			CJokboEffect *pJokboEffect = new CJokboEffect;
			if( pJokboEffect != NULL )
			{		
				if(!g_pGameView->m_cPlayerDrawManager.IsOpen() )
				{
					strcpy( WinEftData.str_value, "기권 승");
					WinEftData.valuekind = -1;//일반 승 이미지 
				}

				pJokboEffect->Init(&WinEftData);
				g_pGameView->m_cAniSprListManager.ADDObject(pJokboEffect);
				g_pGameView->m_cPlayerDrawManager.SetDrawMyValue(nWinPlayerNum, true);
			}
		}
		else
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
	
	for(int i = 0; i < MAX_PLAYER; ++i)
	{
		if(strlen(m_GameOverResultData.Ui[i].ID) > 0) //  겜참여자.
		{
			int nPlayNum = GetPlayerPNum(m_GameOverResultData.Ui[i].ID);
			if( nPlayNum == -1 )
			{
				continue;
			}

			if(m_cPlayer[nPlayNum].JoinState == EGAME_JOININ)
			{
				g_pGameView->m_cPlayerDrawManager.SetPlayerGameOver(nPlayNum, true);// 게임 종료 상태
			}
		}		
	}

	if( m_WinnerPNumData.IsMyVictory(m_cPlayer[0].ServPNum))	
	{
		if(WinEftData.valuekind >= CCardRule::N7C_TITLE)
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
		if((m_cPlayer[0].GetNowMoney() < 100 && g_RI.ChipKind == 0)
			|| (m_cPlayer[0].GetNowMoney() < 1 && g_RI.ChipKind == 1))
		{
			PBPlayEffectSound(SND_LOSE_ALLIN);
		}
		else
		{
			PBPlayEffectSound(SND_LOSE_NORMAL);
		}
	}
	

	//종료 딜레이 때문에 일찍 도착한 보디가드 or 슈퍼 아바타  or 골든칩 정보를 수정한다.
	if( m_AfterEndingUserInfo.bChanged == 1 )
	{
		for(int i = 0 ; i < MAX_PLAYER ; ++i)
		{
			if( strlen(m_cPlayer[i].UI.ID) == 0 )
				continue;

			if( strcmp(m_cPlayer[i].UI.ID,m_AfterEndingUserInfo.EndUserInfo[i].ID) == 0 )
			{
				if( m_AfterEndingUserInfo.EndUserInfo[i].nPMoney > 0 )
				{
					//m_cPlayer[i].UI.SetMoney( m_AfterEndingUserInfo.EndUserInfo[i].nPMoney );
					////m_cPlayer[i].UI.PMoney = m_AfterEndingUserInfo.EndUserInfo[i].nPMoney;
					m_cPlayer[i].SetNowMoney( m_AfterEndingUserInfo.EndUserInfo[i].nPMoney );

					if( strcmp(GM().GetMyInfo()->UI.ID , m_AfterEndingUserInfo.EndUserInfo[i].ID) == 0 && !g_ObserverMan.IsMyObserver())
					{
						GM().GetMyInfo()->UI.SetMoney( m_AfterEndingUserInfo.EndUserInfo[i].nPMoney );
						//GM().GetMyInfo()->UI.PMoney = m_AfterEndingUserInfo.EndUserInfo[i].nPMoney;
					}
				}				
			}
		}
	}

	//잭팟 공지나 이벤트 공지 (카드 펼친후에 나와야 하는 공지들)
	Draw_TextInformationMsg();

	// 채팅창에 결과를 출력한다.
	ResultInfoDisplay(nWinPlayerNum, WinEftData);
}


void CGameProcessHighLow::ResultInfoDisplay(int nWinPlayerNum, GAMERESULTDATA& sWinEftData)
{
/*
	[minera]님 하이승 : J 포카드
	◈ 획득금액 : 1902억3082만313원
	[minera]님 로우승 : J TOP
	◈ 획득금액 : 1902억3082만313원
	-------------------------------------------
	purity1924 : -1368억6640만6250원
	오쿵™ : -53억9375만원
	minera2 : -525억8906만2500원
	골프잡아빌딩짓자 : -53억9375만원

*/
	AddGameInfoViewEdit(" ----------------------------\n", GetChatColor(ECC2_RESULT));	

	enum EWINTYPE
	{
		WINTYPE_FOLD=0,
		WINTYPE_HIGH,
		WINTYPE_LOW,
		WINTYPE_SWING,
		MAX_EWINTYPE,
	};
	static const char* szWinTypeText[]={"기권승", "하이승", "로우승", "스윙승", "", ""};
	int i;
	int nWinner[MAX_EWINTYPE];
	for(i=0; i<MAX_EWINTYPE; i++)
	{
		nWinner[i]= -1;
	}

	//승자 서버Pnum 구해놓기
	if (!g_pGameView-> m_cPlayerDrawManager.IsOpen())
	{
		//기권승이다.
		nWinner[WINTYPE_FOLD] = g_pGameView->m_cPlayerDrawManager.GetHighWinnerPNum();
	}
	else
	{
		nWinner[WINTYPE_HIGH] = g_pGameView->m_cPlayerDrawManager.GetHighWinnerPNum();//서버pnum
		nWinner[WINTYPE_LOW] = g_pGameView->m_cPlayerDrawManager.GetLowWinnerPNum();//서버pnum
		nWinner[WINTYPE_SWING] = g_pGameView->m_cPlayerDrawManager.GetSwingWinnerPNum();//서버pnum
	}
	
	
	int nMaxPlayer	= GetMaxPlayer();
	
	
	//승자 부터 찍기
	CString strMsg;
	CString strJokbo;
	for(int i=0; i<MAX_EWINTYPE; i++)
	{
		int nWinnerServPNum = nWinner[i];
		if (nWinnerServPNum<0 || nWinnerServPNum >= MAX_PLAYER)
			continue;

		CString strJokbo;

		int nClientPnum = GetPNum_ByServPN(nWinnerServPNum);
		switch(i)
		{				
			case WINTYPE_HIGH:
				{
					MM().Call(UPG_CMK_GET_JOKBO_NAME, (WPARAM)nClientPnum, (LPARAM)&strJokbo);

				} break;
			case WINTYPE_LOW:
				{
					MM().Call(UPG_CMK_GET_LOW_JOKBO_NAME, (WPARAM)nClientPnum, (LPARAM)&strJokbo);

				} break;			
			case WINTYPE_SWING:
				{					
					CString strHighJokbo, strLowJokbo;
					MM().Call(UPG_CMK_GET_JOKBO_NAME, (WPARAM)nClientPnum, (LPARAM)&strHighJokbo);
					MM().Call(UPG_CMK_GET_LOW_JOKBO_NAME, (WPARAM)nClientPnum, (LPARAM)&strLowJokbo);
					strJokbo.Format("%s / %s", strHighJokbo, strLowJokbo);

				} break;

		}	

		strMsg.Format("[%s]님 %s %s %s\n◈ 획득금액 : %s\n",	m_GameOverResultData.Ui[nWinnerServPNum].NickName,
															szWinTypeText[i],
															(i==WINTYPE_FOLD)?" ":":",
															strJokbo ,
															(CString)NMBASE::UTIL::g_MoneyMark(m_GameOverResultData.Ui[nWinnerServPNum].nResultMoney));

		

		AddGameInfoViewEdit(strMsg, GetChatColor(ECC2_RESULT_WINNER));
		
	}


	CString strLoserMsg;

	//패자찍기		
	for( int i = 0 ; i < nMaxPlayer; ++i)
	{
		if (m_GameOverResultData.Ui[i].ID[0]==0)
			continue;

		///먼저 패자인지 승자인지 판별한다.
		BOOL bLoser = TRUE;
		for(int p = 0; p <m_GameOverResultData.nWinnerNum; p++)
		{
			if (m_GameOverResultData.nNowWinner[p]==i)
			{
				bLoser = FALSE;
				break;
			}
		}

		if (bLoser==FALSE) 
			continue;
		

		strMsg.Format(" %s : %s\n", m_GameOverResultData.Ui[i].NickName, (CString)NMBASE::UTIL::g_MoneyMark(m_GameOverResultData.Ui[i].nResultMoney));
		strLoserMsg += strMsg;
	}


	if (strLoserMsg.IsEmpty()==FALSE)
	{
		//승자/ 패자 구분선
		AddGameInfoViewEdit(" ----------------------------\n", GetChatColor(ECC2_RESULT));	
		AddGameInfoViewEdit(strLoserMsg,  GetChatColor(ECC2_RESULT_LOSER));
	}

	

}


// 삥 버튼
void CGameProcessHighLow::SendPingButton()
{
	if(g_ObserverMan.IsMyObserver() == TRUE) 
		return;

	if( !g_pGameView->m_cGameViewBtnMananager.m_bBPing || g_PLAYER(0)->PlayState == EPLAYER_GAMEEND)
		return;

	// 따당이 예약되어 있어도 상태방이 체크를 선택해서 따당이 실패하면 예약 취소를 위해서 넣어줌.
	SetReserveButton(RESERVESTATE_BET_NONE);

	g_PLAYER(0)->ClearEffectSpr(ANF_PLAYER_TIMEBAR);
	g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();

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
void CGameProcessHighLow::SendDdaButton()
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
	
	g_PLAYER(0)->ClearEffectSpr(ANF_PLAYER_TIMEBAR);

	g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();

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
		pc.nBtn   = BET_CMD_DADANG;//BET_CMD_FULL;//BET_CMD_DADANG ; // 따당에서 풀로교체(규제안관련 작업 2014.01)->추후반영
	}
#else
	pc.nBtn   = BET_CMD_DADANG;//BET_CMD_FULL;//BET_CMD_DADANG ; // 따당에서 풀로교체(규제안관련 작업 2014.01)->추후반영
#endif

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  g_PLAYER(0)->UI.UNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.ID, &pc);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), raise.pData, raise.GetTotalSize());
	g_pGameView->m_cGameViewBtnMananager.SetButtonClick(true);
}

// 하프
void CGameProcessHighLow::SendHalfButton()
{
	if(g_ObserverMan.IsMyObserver() == TRUE)
		return;
	if( !g_pGameView->m_cGameViewBtnMananager.m_bBMax || g_PLAYER(0)->PlayState == EPLAYER_GAMEEND ) 
		return;

	// 2014.2.7 crpark
	// 규제안 베팅예약 공통화
	if(false == CheckMyTurn(GetPlayer(0)->ServPNum, m_nCurPlayer))
	{
		SetReserveButton(RESERVESTATE_BET_BET_CMD_HALF);
		return;
	}
	SetReserveButton(RESERVESTATE_BET_NONE);	

	int spnum       = GetServPNum_ByPN(0);
	INT64 CallMoney = GetRaiseBat() - g_Poker.RU[spnum].nNowBat;
	INT64 nTotal    = GetRealBet();

	g_PLAYER(0)->ClearEffectSpr(ANF_PLAYER_TIMEBAR);
	g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();

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
void CGameProcessHighLow::SendDieButton()
{
	if(g_ObserverMan.IsMyObserver() == TRUE)
		return;
	if(!g_pGameView->m_cGameViewBtnMananager.m_bFold || g_PLAYER(0)->PlayState == EPLAYER_GAMEEND) 
		return;


	if( g_Poker.nAutoBetCmd > 0 && g_RI.nAutoBetKind > 0 )
	{
		return;
	}

// 	CString szTemp;
// 	szTemp.Format("CGameProcessHighLow::SendDieButton() : 유저ID:%s \n", g_PLAYER(0)->UI.ID);
// 	AddGameChatViewEdit(szTemp.GetBuffer(), GetChatColor(ECC2_RESULT_JOKBO));

	// 2014.2.7 crpark
	// 규제안 베팅예약 공통화
	if(false == CheckMyTurn(GetPlayer(0)->ServPNum, m_nCurPlayer))
	{
		SetReserveButton(RESERVESTATE_BET_BET_CMD_FOLD);
		return;
	}
	SetReserveButton(RESERVESTATE_BET_NONE);
	//규제안 :  예약 가능여부 설정
	CanReserve(false);

	g_PLAYER(0)->ClearEffectSpr(ANF_PLAYER_TIMEBAR);

	g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();

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
void CGameProcessHighLow::SendCheckButton()
{
	if(g_ObserverMan.IsMyObserver() == TRUE) 
		return;
	if( !g_pGameView->m_cGameViewBtnMananager.m_bBCheck || g_PLAYER(0)->PlayState == EPLAYER_GAMEEND) 
		return;
	g_PLAYER(0)->ClearEffectSpr(ANF_PLAYER_TIMEBAR);
	g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();

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
void CGameProcessHighLow::SendCallButton()
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

	g_PLAYER(0)->ClearEffectSpr(ANF_PLAYER_TIMEBAR);
	//if(	!m_bOnGame || !m_bTurn || m_nCardWindDelay>0 ){ SetBtn(); return; }
	//m_bTurn = FALSE;
	//m_nTime = 15;
	//m_bTime = FALSE;
	g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();

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
void CGameProcessHighLow::Change_Money(CSV_CHANGE_MONEY *pMsg)
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
void CGameProcessHighLow::SetPlayInfo(int nowpnum, int newpnum, BOOL bParticipation )
{
	if( !IsHighLow() ) 
	{
		return;
	}

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

BOOL CGameProcessHighLow::IsPlayingNow()
{
	return m_bGameStart;
}

//UPG작업 추가
void CGameProcessHighLow::OnEnterRoom(BOOL bCreation)
{
	//내가 방에 들어간다 (bCreation은 방을 생성해서 입장하는지 여부)

	// 본인 카드 오픈 버튼 상태 Off로 초기화(1이면 Off, 0이면 On)
	g_pGameView->m_cGameViewBtnMananager.m_btnCardOpen.SetSprite(&(g_pGameView->m_cGameViewBtnMananager.m_sprUICardOpenBtn), 1);
	
	CString str;
	#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
		if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
			str.Format("톡톡 하이로우");
		else
			str.Format("윈조이 하이로우");
	#else //UPGRADE_20120308_TOCTOC_CHANNELING
		str.Format("윈조이 하이로우");
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
	g_cVipJackpot.OnEnterRoom();


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

	g_cPromotionMan.OnEnterRoom();
}

// 방에서 나간다.
void CGameProcessHighLow::OnExitRoom()
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

	//자리 지킴이
	g_SeatKeeper.OnOutRoom();
	g_cVipJackpot.OnOutRoom();

	g_pChatWinMan->ResetChatting_STR();

	NMBASE::SOUND::g_Sound.Stop(SND_HIDDEN_BGM);
	
	m_i64JackPotMoney = 0;
	m_i64OldJackPotMoney = 0;

	//보유한도
	g_cOwnLimit.OnExitRoom();	
}




//UPG작업 추가 (기존 GameDlg::PreTranslateMessage() 에 있던 부분을 가져옴)
BOOL CGameProcessHighLow::PreTranslateMessage(MSG* pMsg) 
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
BOOL CGameProcessHighLow::IsSacnCode(int nScanCode, int &r_scankey, int &r_extended, int &r_previous)
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

void CGameProcessHighLow::OnInit()
{
	set_GameSpeed();
}

void CGameProcessHighLow::set_GameSpeed()
{
	g_GameSpeed.Clear();

	int i = 0, j = 0;

	g_GameSpeed.set_CardDistributeDelay(i, j, 20);
	g_GameSpeed.set_CardSpeed(i, j, 7.1f);
	g_GameSpeed.set_CardFlyDelay(i, j, 2);
	
	g_GameSpeed.set_BetCmdActiveDelay(i, j, 5); // 게임사업 1팀의 요청으로 변경. 2012. 8. 14 류승민

	g_GameSpeed.set_BetCmdActiveDelayInAutoBet(i, j, 4);
	g_GameSpeed.set_AutoBetActiveDelay(i, j, 13);	 

#ifdef UPGRADE_20120502_TIMING_TEST_CODE
//#ifdef _DEBUG
	int nBetCmdActiveDelay = GetPrivateProfileInt("HighLow", "BetBtnActiveFrameNum", 4, ".\\TimingTest.ini");
	g_GameSpeed.set_BetCmdActiveDelay(i, j, nBetCmdActiveDelay);

	nBetCmdActiveDelay = GetPrivateProfileInt("HighLow", "BetBtnActiveInAutoBetFrameNum", 4, ".\\TimingTest.ini");
	g_GameSpeed.set_BetCmdActiveDelayInAutoBet(i, j, nBetCmdActiveDelay);
//#endif //_DEBUG
#endif //UPGRADE_20120502_TIMING_TEST_CODE

	const int CARDMOVESPEED = 1;
	g_GameSpeed.nDebug_ModeKind = CARDMOVESPEED;
	g_GameSpeed.bShowBetMoney = FALSE;
}


BOOL CGameProcessHighLow::OnCommand(WPARAM wParam, LPARAM  lParam)
{
	g_pGameView->m_cChatWindow.OnCommand(wParam);

	if( g_pGameView->m_cSidePotMoney.OnCommand(wParam,lParam) )
		return TRUE;

	if( g_ObserverMan.OnCommand(wParam, lParam) )
		return TRUE;

	if( g_pGameView->m_cGameViewBtnMananager.OnCommand(wParam,lParam))
		return TRUE;

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
			//AddGameChatViewEdit("CGameProcessHighLow::OnCommand()", GetChatColor(ECC2_RESULT_JOKBO));
			SendDieButton();
		}
		break;

	case IDM_CHECK:
		{
			SendCheckButton();		
		}
		break;

	case IDM_CALL:
		{
			SendCallButton();
		}
		break;

	case IDM_PING:
		{
			SendPingButton();
		}
		break;

	case IDM_DADANG:
		{
			SendDdaButton();
		}
		break;	

	case IDM_FULL:
		{
			SendDdaButton();
		}
		break;	

	case IDM_HALF:
		{
			SendHalfButton();
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

	case IDM_HIGH:
		{
			SendHighCase();			
		} break;

	case IDM_LOW:
		{
			SendLowCase();
		} break;

	case IDM_SWING:
		{
			SendSwingCase();
		} break;
		// 본인 카드 오픈 버튼
	case IDM_CARD_OPEN:
		{
			// 버튼 spr 인덱스가 0이면 본인 카드 오픈 On, 1 이면 Off
			int nPrevBtnNum = g_pGameView->m_cGameViewBtnMananager.m_btnCardOpen.GetButtonNumber(); // 현재 버튼 상태(On or Off)
			//int nBtnNum = 0;
			bool bUIFoldOpen = false;

			// 버튼이 Off 면 On 시켜줌 -> 버튼 상태 버튼은 서버 결과에 따라 하기로 함
			if (nPrevBtnNum == 1)
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
void CGameProcessHighLow::SoundFxKind(int betkind, int sex, bool bMax)
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
			if(CheckReceivedAllCards()){
				PBPlaySound(sex?true:false, SND_CHECK);
			}
			break;
		case RAISE_LAST_CMD_CALL: // 콜
			//규제안 관련 : 카드세팅중에 이펙트 제거
			if(CheckReceivedAllCards()){
				PBPlaySound(sex?true:false, SND_CALL);
			}
			break;
		case RAISE_LAST_CMD_PING: // 삥
			PBPlaySound(sex?true:false, SND_PING);
			break;
		case RAISE_LAST_CMD_DADANG: // 따당
			PBPlaySound(sex?true:false, SND_DDANG);
			break;
		case RAISE_LAST_CMD_HALF: // 하프
			PBPlaySound(sex?true:false, SND_HALF);
			break;
		case RAISE_LAST_CMD_ALLIN: // 올인
			PBPlaySound(sex?true:false, SND_ALLIN);
			break;	
		case RAISE_LAST_CMD_FULL: // 풀
			PBPlaySound(sex?true:false, SND_FULL);
			break;
		}
	}
}

void CGameProcessHighLow::SoundFxKindBig(int betkind, int sex, bool bMax)
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
			if(CheckReceivedAllCards()){
				PBPlaySound(sex?true:false, SND_CHECK_H);
			}
			break;
		case RAISE_LAST_CMD_CALL: // 콜
			//규제안 관련 : 카드세팅중에 이펙트 제거
			if(CheckReceivedAllCards()){
				PBPlaySound(sex?true:false, SND_CALL_H);
			}
			break;
		case RAISE_LAST_CMD_PING: // 삥
			PBPlaySound(sex?true:false, SND_PING_H);
			break;
		case RAISE_LAST_CMD_DADANG: // 따당
			PBPlaySound(sex?true:false, SND_DDANG_H);
			break;
		case RAISE_LAST_CMD_HALF: // 하프
			PBPlaySound(sex?true:false, SND_HALF_H);
			break;
		case RAISE_LAST_CMD_ALLIN: // 올인
			PBPlaySound(sex?true:false, SND_ALLIN_H);
			break;	
		case RAISE_LAST_CMD_FULL: // 풀
			PBPlaySound(sex?true:false, SND_FULL_H);
			break;	
		}
	}
}

// 현재 게임이 7Poker인지 체크한다.
BOOL CGameProcessHighLow::IsHighLow()
{
	if( IDX_GAME_HL == g_RI.nGameIndex )
	{
		return TRUE;
	}
	return FALSE;
}



void CGameProcessHighLow::OnCheckTurnLimitTime()
{
	if( !IsHighLow() ) 
		return;

	if(g_RI.State == EGAME_WAITING)
		return;

	if(g_pGameView->m_cPlayerDrawManager.GetStartTimerTime() <= 0)
		return;

	OnCheckTurnLimitTimeBase();

	if(g_Poker.nState == RSTATE_PREPARESTART || g_Poker.nState == RSTATE_EXTRAFIRSTBET )
	{
		if( g_pGameView->m_cPlayerDrawManager.IsSelectCard(0) == FALSE)
		{
			return;
		}

		int tcnt = g_pGameView->m_cPlayerDrawManager.GetTimerTimeGab() / 1000;		

		if(tcnt >= PBTurnTimeOut(ETTT_CARDCHOICE) && !g_ObserverMan.IsMyObserver() && g_SelectCard().IsChoiceCard() )
		{	
			//2006.3.20
			//카드 선택시 시간이 지나 게임에서 자동으로 선택한다. 3번까지 자동 선택하면 게임 종료 후 강퇴 시킨다.
			if( m_cPlayer[0].m_nCardChoiceLimtCnt < MAX_TURN_TIMEOUT_LIMIT_COUNT && g_pGameView->m_cPlayerDrawManager.IsSelectCard(0) == TRUE )
			{				
				g_SelectCard().EndSelectCard(TRUE);				
				g_pGameView->m_cPlayerDrawManager.SetSelectCard(0, false);
#ifndef _DEBUG
				m_cPlayer[0].m_nCardChoiceLimtCnt ++;
#endif
				g_pGameView->m_cDisplayBoard.SetText(EDBT_LIMITTIME);
				return;
			}
		}
	}
	
	//하이로우 전용
	else if( g_Poker.nState == RSTATE_HIGHLOW_SELECT ){

/*		//기존코드
		int tcnt = ( m_dwCurTime - m_dwBackTime ) / 1000;	
		if(tcnt >= get_TurnTimeOut() && !g_MyObserver && m_bSelectBtnBackImage == TRUE ){	
			//스윙을 해줌.
			SwingCase();
		}
*/

		int tcnt = g_pGameView->m_cPlayerDrawManager.GetTimerTimeGab() / 1000;	
		if(tcnt >= PBTurnTimeOut(ETTT_CARDCHOICE) && !g_ObserverMan.IsMyObserver() && m_bSelectBtnBackImage == TRUE )
		{	
			//타임아웃 시 스윙을 해줌
			SendSwingCase();
		}

	}

}



void CGameProcessHighLow::OnCardEvent(CARDEVENT *pEvent)
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

	case EVENT_ONEFLYEND:	// 한장씩
		{
			int nPlayNum = pEvent->Option;
			int Index    = pEvent->Option2;

			if(nPlayNum < 0 || nPlayNum >= MAX_PLAYER) 
				break;

			if(Index != 6)
			{
				m_cPlayer[nPlayNum].SetCardFrontShow(Index);
				if( nPlayNum == m_cPlayer[0].PNum && !g_ObserverMan.IsMyObserver() )
				{
					//족보 산출		
					int nOldPoint = g_GAMEPROCESS()->GetSevenRuleManager().get_MyRule(0);
					m_cPlayer[0].SetCardResult();
					m_cPlayer[0].DrawEft_CompleteMadeValue( nOldPoint );


					//확률
					//g_pGameView->m_cChatWindow.m_pMyRuleInfoMan->get_My_Pro_Info();
				}

				if(!g_ObserverMan.IsMyObserver() &&  m_cPlayer[0].JoinState == EGAME_JOININ)
				{
					//g_pGameView->m_cChatWindow.m_pMyRuleInfoMan->get_Other_CardInof(nPlayNum);
				}
			}
			else
			{
				if( nPlayNum == 0 && !g_ObserverMan.IsMyObserver() )
				{
					CMyCard* pMyCard = &m_cPlayer[0].m_cMyCard[Index];
					if( pMyCard != NULL )
					{
						//히든카드를 보여줘라 
						if(!pMyCard->IsShowHiddenCard() && pMyCard->IsHiddenCard())
						{
							pMyCard->SetShowHiddenCard(TRUE);
						}
					}
				}

				if(!g_ObserverMan.IsMyObserver() && m_cPlayer[0].JoinState == EGAME_JOININ)
				{
					//g_pGameView->m_cChatWindow.m_pMyRuleInfoMan->get_Other_CardInof(nPlayNum);
				}
			}
			m_cPlayer[nPlayNum].ShowCard(Index);
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

			if(!g_ObserverMan.IsMyObserver() && g_PLAYER(0)->JoinState == 1 && g_PLAYER(0)->PlayState == 1)
			{
				if( g_PLAYER(0)->m_nTotalCardCount <= 6 )
				{
					g_pGameView->m_cGameHelper.CalculationProbability();
				}
				else if( g_PLAYER(0)->m_nTotalCardCount == 7 )
				{
					g_pGameView->m_cGameHelper.ClearProbability();
				}
			}

			m_bWindCardEnd = FALSE;

			if(!g_ObserverMan.IsMyObserver() && g_PLAYER(0)->JoinState == 1)
			{
				if( g_PLAYER(0)->m_nTotalCardCount < GetTotalCardNum() )
				{
					g_pGameView->m_cGameHelper.CheckOtherJokboCheck(0);
				}
			}
		} 
		break;
	}
}


CPlayer* CGameProcessHighLow::GetPlayer(int i)
{
	ASSERT(i<GetMaxNewPlayer() && i>=0);
	return &m_cPlayer[i];
}

CPlayerHighLow* CGameProcessHighLow::GetPlayerNP(int i)
{
	return (CPlayerHighLow *)GetPlayer(i);
}


void CGameProcessHighLow::CheckMinimumUser(void)
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
void  CGameProcessHighLow::SetJackPotMoney(INT64 i64Money)
{
	m_i64JackPotMoney = i64Money;
	g_cVipJackpot.SetShowJackpotMoney(m_i64JackPotMoney);
}


void  CGameProcessHighLow::SetOldJackPotMoney(INT64 i64Money)
{
	m_i64OldJackPotMoney = i64Money;
	g_cVipJackpot.SetShowOldJackpotMoney(m_i64JackPotMoney);
}


void CGameProcessHighLow::JackPotMoneyUpdate(jackpot::JP_MONEYINFO* pInfo)
{
	// 게임 타입
	//IDX_GAME eGameType = (IDX_GAME)g_RI.nGameIndex;

	// 자신이 입장한 방의 최소 금액
	INT64 i64RoomMoney = g_RI.llEnterLimitMoney;

	if(i64RoomMoney <= 0)	///자유방 제외
		return;

	// 테스트 코드
	//	ROOMINFO sRoomInfo;
	//	memcpy(&sRoomInfo, &g_RI, sizeof(ROOMINFO));

	//뉴포커, 로우, 7포커
	//ASSERT (eGameType > IDX_GAME_NULL);
	const int INDEX_ARRAY_JACKPOT_GAME_HIGHLOW = 3; //하이로우 인덱스 3

	INT64 i64JackPotMoney = 0; 

	for(int i = 0; i < jackpot::DEF_MONEYRANGE_NUM; ++i)
	{
		if(LIMITMONEY_INT_INT64(pInfo->stJPBase[ INDEX_ARRAY_JACKPOT_GAME_HIGHLOW].nLimitMoney[i]) == i64RoomMoney)
		{
			i64JackPotMoney = pInfo->stJPBase[ INDEX_ARRAY_JACKPOT_GAME_HIGHLOW ].llJackPotMoney[ i ];//  llJackpotMoney[eGameType][i];  
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


void CGameProcessHighLow::ClearJackpotMoney(int nGameCode, INT64 i64LimitMoney)
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

	case GAME_CODE_HL: eGameType = IDX_GAME_HL;
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


void CGameProcessHighLow::TestCode(MSG* pMsg)
{
	
	return; 
	
	// 메크로 테스트
	if(GetAsyncKeyState(VK_CONTROL))
	{
		//	int scankey = 0, extended = 0, previous = 0;
		//	BOOL bGameKey = IsSacnCode(pMsg->lParam, scankey, extended, previous);	//수정

		//if(scankey == VK_F4)
		if(pMsg->wParam == '1')
		{						
			// 				if( GetCurrentWhere() == IDX_GAMEWHERE_GAME )
			// 				{
			// 					CChance_Msg_EffectSpr *pObj = new CChance_Msg_EffectSpr;
			// 					if( pObj != NULL )
			// 					{				
			// 						pObj->Init(ITEM_SCODE_REWARD_HIGHGRADE);				
			// 						g_PLAYER(0)->m_cPlayerEffectList.AddObject(pObj);
			// 					}
			// 				}	

			CJokboEffect *pJokboEffect = new CJokboEffect;
			if( pJokboEffect != NULL )
			{		
				//if(!g_pGameView->m_cPlayerDrawManager.IsOpen() )
				//{
				//	strcpy( WinEftData.str_value, "기권 승");
				//	WinEftData.valuekind = -1;//일반 승 이미지 
				//}
				//else
				//{
				//	g_VoiceMan.GameEnd_HIValue_Sound();
				//}

				// 승패 창에 들어갈 구조체
				GAMERESULTDATA WinEftData;	
				WinEftData.Clear();
				WinEftData.wincase	 = 1;
				WinEftData.pnum		 = 0;
				WinEftData.valuekind = CCardRule::N7C_BSFLUSH; // N7C_SFULSH / N7C_QUADS

				m_i64JackPotActive = 100;

				//승패 창에 들어갈 족보 명	
				strcpy( WinEftData.str_value, "로얄");		

				//아래 정보는 하이 족보 달성시 필요한 정보 
				strcpy( WinEftData.ID, m_cPlayer[0].UI.NickName);			
				WinEftData.nResultMoney = m_cPlayer[0].m_n64ResultMoney;	

				pJokboEffect->Init(&WinEftData);
				g_pGameView->m_cAniSprListManager.ADDObject(pJokboEffect);
			}
		}	
		if(pMsg->wParam == '2')
		{						
			// 				if( GetCurrentWhere() == IDX_GAMEWHERE_GAME )
			// 				{
			// 					CChance_Msg_EffectSpr *pObj = new CChance_Msg_EffectSpr;
			// 					if( pObj != NULL )
			// 					{				
			// 						pObj->Init(ITEM_SCODE_REWARD_HIGHGRADE);				
			// 						g_PLAYER(0)->m_cPlayerEffectList.AddObject(pObj);
			// 					}
			// 				}	

			CJokboEffect *pJokboEffect = new CJokboEffect;
			if( pJokboEffect != NULL )
			{		
				//if(!g_pGameView->m_cPlayerDrawManager.IsOpen() )
				//{
				//	strcpy( WinEftData.str_value, "기권 승");
				//	WinEftData.valuekind = -1;//일반 승 이미지 
				//}
				//else
				//{
				//	g_VoiceMan.GameEnd_HIValue_Sound();
				//}

				// 승패 창에 들어갈 구조체
				GAMERESULTDATA WinEftData;	
				WinEftData.Clear();
				WinEftData.wincase	 = 1;
				WinEftData.pnum		 = 0;
				WinEftData.valuekind = CCardRule::N7C_SFULSH; //  / N7C_QUADS

				m_i64JackPotActive = 100;

				//승패 창에 들어갈 족보 명	
				strcpy( WinEftData.str_value, "로얄");		

				//아래 정보는 하이 족보 달성시 필요한 정보 
				strcpy( WinEftData.ID, m_cPlayer[0].UI.NickName);			
				WinEftData.nResultMoney = m_cPlayer[0].m_n64ResultMoney;	

				pJokboEffect->Init(&WinEftData);
				g_pGameView->m_cAniSprListManager.ADDObject(pJokboEffect);
			}
		}

		if(pMsg->wParam == '3')
		{						
			// 				if( GetCurrentWhere() == IDX_GAMEWHERE_GAME )
			// 				{
			// 					CChance_Msg_EffectSpr *pObj = new CChance_Msg_EffectSpr;
			// 					if( pObj != NULL )
			// 					{				
			// 						pObj->Init(ITEM_SCODE_REWARD_HIGHGRADE);				
			// 						g_PLAYER(0)->m_cPlayerEffectList.AddObject(pObj);
			// 					}
			// 				}	

			CJokboEffect *pJokboEffect = new CJokboEffect;
			if( pJokboEffect != NULL )
			{		
				//if(!g_pGameView->m_cPlayerDrawManager.IsOpen() )
				//{
				//	strcpy( WinEftData.str_value, "기권 승");
				//	WinEftData.valuekind = -1;//일반 승 이미지 
				//}
				//else
				//{
				//	g_VoiceMan.GameEnd_HIValue_Sound();
				//}

				// 승패 창에 들어갈 구조체
				GAMERESULTDATA WinEftData;	
				WinEftData.Clear();
				WinEftData.wincase	 = 1;
				WinEftData.pnum		 = 0;
				WinEftData.valuekind = CCardRule::N7C_QUADS; //  / 

				m_i64JackPotActive = 100;

				//승패 창에 들어갈 족보 명	
				strcpy( WinEftData.str_value, "로얄");		

				//아래 정보는 하이 족보 달성시 필요한 정보 
				strcpy( WinEftData.ID, m_cPlayer[0].UI.NickName);			
				WinEftData.nResultMoney = m_cPlayer[0].m_n64ResultMoney;	

				pJokboEffect->Init(&WinEftData);
				g_pGameView->m_cAniSprListManager.ADDObject(pJokboEffect);
			}
		}
	} 
}



void CGameProcessHighLow::SelectWinnerCase()
{
	//현재 스테이트를 바꿔준다.
	g_Poker.nState = RSTATE_HIGHLOW_SELECT; //하이, 로우, 스윙 선택 


	//전광판에 "하이,로우,스윙을 선택해주세요" 표시
	g_pGameView->m_cDisplayBoard.SetText(EDBT_SELECT_HLS);


	//물음표 마크를 찍어준다. (관전자나 게임에 참여 하지 않은 유저한테도 메세지가 간다. 
	int i = 0 ;

	//배팅 커멘드를 딜레이 후 지운다.
	g_pGameView->m_cAniSprListManager.BetCmd_DelDelay(1);

	//SetBackTime();	
	g_pGameView->m_cPlayerDrawManager.SetStartTimer();	


	for( i = 0; i < GetMaxPlayer(); i++ )
	{
		if(m_cPlayer[i].JoinState == 1 && m_cPlayer[i].PlayState == 1 && strlen(m_cPlayer[i].UI.ID) > 0 )
		{
			
			CSelectWinnerCaseEft_HI *pNodeEft = new CSelectWinnerCaseEft_HI;
			pNodeEft->Init(i,TRUE);
			m_cPlayer[i].m_cPlayerEffectList.AddObject(pNodeEft);	

// 			CTimeCount *pObj = new CTimeCount;
// 			pObj->Init(i,FRAME_TIMECOUNT,DRAW_TOP);
// 			m_cPlayer[i].m_cPlayerEffectList.AddObject(pObj);	

		}
	}

	//NMBASE::SOUND::g_Sound.PlayWav(SND_BUTTON_CLICK_A);

	// ### [ 관전기능 ] ###
	if(g_ObserverMan.IsMyObserver()) 
		return;

	if(m_cPlayer[0].JoinState == 0 || m_cPlayer[0].PlayState == 0) 
		return;

	//히든 카드 강제 오픈
	if( g_GAMEPROCESS()->IsHiddenClick() == FALSE)
	{
		/* 기존코드
		Play[0].SetFaceUp(6); // 보이기
		Play[0].SetCardResult();
		Play[0].bShowValue = TRUE;
		m_bHiddenCardShow = TRUE;
		*/

		m_cPlayer[0].SetCardFrontShow(6);
		g_pGameView->m_cPlayerDrawManager.SetHiddenCardShow(true);

		int nOldPoint = g_GAMEPROCESS()->GetSevenRuleManager().get_MyRule(0);
		m_cPlayer[0].SetCardResult();
		m_cPlayer[0].DrawEft_CompleteMadeValue( nOldPoint );
		g_pGameView->m_cGameHelper.CalculationProbability();			
	
	}else if(g_pGameView->m_cPlayerDrawManager.IsHiddenCardShow() == FALSE) 	//bts 수정소스(스윙 족보 선택 관련)
	{
		m_cPlayer[0].SetCardFrontShow(6);
		g_pGameView->m_cPlayerDrawManager.SetHiddenCardShow(true);

		int nOldPoint = g_GAMEPROCESS()->GetSevenRuleManager().get_MyRule(0);
		m_cPlayer[0].SetCardResult();
		m_cPlayer[0].DrawEft_CompleteMadeValue( nOldPoint );
		g_pGameView->m_cGameHelper.CalculationProbability();	
	}
	
	m_bSelectBtnBackImage=TRUE;	
	g_pGameView->m_cGameViewBtnMananager.SetBettinBtnDiable();			
	
	//포카드이상일땐 스윙버튼만 보이게한다.
	bool bEnableOnlySwingBtn  = ( m_cSevenRuleManager.get_MyRule(0) >= CCardRule::N7C_QUADS );
	g_pGameView->m_cGameViewBtnMananager.ShowWincaseBtn_HL(true, bEnableOnlySwingBtn);

	//규제안 : 하이로우선택시 예약기능 제거
	g_pGameView->m_cGameViewBtnMananager.m_bBCall = false;
	g_pGameView->m_cGameViewBtnMananager.m_bBCheck = false;
	g_pGameView->m_cGameViewBtnMananager.m_bBDDa = false;
	g_pGameView->m_cGameViewBtnMananager.m_bBMax = false;
	g_pGameView->m_cGameViewBtnMananager.m_bFold = false;
	g_pGameView->m_cGameViewBtnMananager.SetShowBetMony();
	g_pGameView->m_cGameViewBtnMananager.ShowReserveBtn(false);

	//마이 턴 
	g_pGameView->m_cPlayerDrawManager.SetTurn(0);
}

void CGameProcessHighLow::SendHighCase()
{
	if (m_bSentHLS)	//이미 하이/로우/스윙 선택하였음
		return;

	if (g_Poker.nState!=RSTATE_HIGHLOW_SELECT)
		return;

	// ### [ 관전기능 ] ###
	if(g_ObserverMan.IsMyObserver()) 
		return;

	//하이 버튼이 활성화 되었을때만 유효
	if (g_pGameView->m_cGameViewBtnMananager.m_btnHigh.IsEnable()==FALSE)
		return;
	if (g_pGameView->m_cGameViewBtnMananager.m_btnHigh.IsShow()==FALSE)
		return;

	
	m_SelectHighLowSwing=0;

	CCL_HL_SELECTWINCASE selwin;
	selwin.Set(g_RI.RoomNum, m_cPlayer[0].ServPNum, m_cPlayer[0].UI.UNum, m_cPlayer[0].UI.ID, 1);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), selwin.pData, selwin.GetTotalSize());
	//NMBASE::SOUND::g_Sound.PlayWav(SND45);	
	m_bSelectBtnBackImage = FALSE;	

	//타이머 
	//Play[0].ClearEffectSpr(ANF_PLAYER_TIMEBAR);
	m_cPlayer[0].ClearEffectSpr(ANF_PLAYER_TIMEBAR);

	g_pGameView->m_cGameViewBtnMananager.EnableWincaseBtn_HL(false);
	m_bSentHLS=TRUE;
}

void CGameProcessHighLow::SendLowCase()
{
	if (m_bSentHLS)	//이미 하이/로우/스윙 선택하였음
		return;

	if (g_Poker.nState!=RSTATE_HIGHLOW_SELECT)
		return;

	// ### [ 관전기능 ] ###
	if(g_ObserverMan.IsMyObserver()) 
		return;


	//로우 버튼이 활성화 되었을때만 유효
	if (g_pGameView->m_cGameViewBtnMananager.m_btnLow.IsEnable()==FALSE)
		return;
	if (g_pGameView->m_cGameViewBtnMananager.m_btnLow.IsShow()==FALSE)
		return;

	m_SelectHighLowSwing=1;

	CCL_HL_SELECTWINCASE selwin;
	selwin.Set(g_RI.RoomNum, m_cPlayer[0].ServPNum, m_cPlayer[0].UI.UNum, m_cPlayer[0].UI.ID, 2);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), selwin.pData, selwin.GetTotalSize());
	//NMBASE::SOUND::g_Sound.PlayWav(SND45);	
	m_bSelectBtnBackImage = FALSE;	

	//Play[0].ClearEffectSpr(ANF_PLAYER_TIMEBAR);
	m_cPlayer[0].ClearEffectSpr(ANF_PLAYER_TIMEBAR);

	g_pGameView->m_cGameViewBtnMananager.EnableWincaseBtn_HL(false);
	m_bSentHLS=TRUE;


}

void CGameProcessHighLow::SendSwingCase()
{
	if (m_bSentHLS)	//이미 하이/로우/스윙 선택하였음
		return;


	if (g_Poker.nState!=RSTATE_HIGHLOW_SELECT)
		return;

	// ### [ 관전기능 ] ###
	if(g_ObserverMan.IsMyObserver()) 
		return;

	//스윙 버튼이 활성화 되었을때만 유효
	if (g_pGameView->m_cGameViewBtnMananager.m_btnSwing.IsEnable()==FALSE)
		return;
	if (g_pGameView->m_cGameViewBtnMananager.m_btnSwing.IsShow()==FALSE)
		return;


	m_SelectHighLowSwing=2;

	CCL_HL_SELECTWINCASE selwin;
	selwin.Set(g_RI.RoomNum, m_cPlayer[0].ServPNum, m_cPlayer[0].UI.UNum, m_cPlayer[0].UI.ID, 3);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), selwin.pData, selwin.GetTotalSize());
//	NMBASE::SOUND::g_Sound.PlayWav(SND45);	
	m_bSelectBtnBackImage = FALSE;
	//타이머 
	//Play[0].ClearEffectSpr(ANF_PLAYER_TIMEBAR);
	m_cPlayer[0].ClearEffectSpr(ANF_PLAYER_TIMEBAR);

	g_pGameView->m_cGameViewBtnMananager.EnableWincaseBtn_HL(false);
	m_bSentHLS=TRUE;
}

//하이로우 선택 완료 선택 했다는 이미지 출력
void CGameProcessHighLow::HLS_SelectedCase( int pnum )
{
	if(m_cPlayer[pnum].JoinState ==0 || m_cPlayer[pnum].PlayState == 0) return;

	m_cPlayer[pnum].ClearEffectSpr(ANF_PLAYER_TIMEBAR);
	m_cPlayer[pnum].ClearEffectSpr(ANF_SELECTWINNERCASE_HI);


	CSelectWinnerCaseEft_HI *pNodeEft = new CSelectWinnerCaseEft_HI;
	pNodeEft->Init(pnum,FALSE);
	m_cPlayer[pnum].m_cPlayerEffectList.AddObject(pNodeEft);


	g_pGameView->m_cPlayerDrawManager.SetSelectedHLS(pnum, true); //drawmanger에 유저가 하이/로우/스윙을 선택했다고 알려줌
}