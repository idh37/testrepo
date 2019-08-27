#include "StdAfx.h"
#include "Game_LowBD.h"
#include "SpriteList.h"
#include "GameView.h"
#include "CommMsgDef_Game.h"
#include "Resource.h"

CGame_LowBD::CGame_LowBD()
:	m_bFlagReceiveLeaveRoomReq(false),
	CGame()
{
}

CGame_LowBD::~CGame_LowBD(void)
{
}

BOOL CGame_LowBD::Create(CWnd *pParentWnd)
{
	//Create GameView
	CGameViewLowBD* pGameView = NULL;
	SAFE_NEW( pGameView, CGameViewLowBD( this ));
	
	m_pGameViewBase = pGameView;
	CRect rect(0, 0, 1000, 740);
	pGameView->Create(NULL, "GameView", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rect, pParentWnd, 12345);
	pGameView->ShowWindow(SW_HIDE);

	//Create GameProcess
	SAFE_NEW( m_pGameProcess, GameProcess_BD( this ) );
	
	return TRUE;
}

void CGame_LowBD::OnLoadingEnd(void)
{
	// 스프라이트 로딩 후 초기화 ~
	GetGameView()->Init();
	GetGameProcess()->Init( &g_pGameView->Page );
}

GameProcess_BD* CGame_LowBD::GetGameProcess()
{
	return (GameProcess_BD*) m_pGameProcess;
}

LRESULT CGame_LowBD::ShowCreateRooomDlg(ROOMINFO *pRoomInfo, ROOMOPTION *pWebRoomOption)
{

	CCreateRoomDlg dlg(GetGameType(), GM().GetMainWnd());
	dlg.SetWebSelectedRoomOption(pWebRoomOption);
	LRESULT nRet = dlg.DoModal();
	if(nRet == IDOK)
	{
		strncpy_s(pRoomInfo->Title, 32, dlg.m_strRoomTitle, dlg.m_strRoomTitle.GetLength());
		pRoomInfo->bSuperMaster		= PBIsSuperRoom();
		pRoomInfo->ChipKind			= dlg.m_nChipKind;
		pRoomInfo->FormKind			= dlg.m_nFormKind;
		pRoomInfo->bIsFriendRoom	= dlg.m_bFriend;
		pRoomInfo->nAutoBetKind		= dlg.m_sAutoBet;
		pRoomInfo->BetMoney			= dlg.m_i64SeedMoney;		
		pRoomInfo->llEnterLimitMoney= dlg.m_i64MinimumMoney;
		//pRoomInfo->bVIPEnterLimitYN	= dlg.m_bVIPEnterLimitYN;
		pRoomInfo->sBetRuleKind		= dlg.m_sBetRuleKind;

		// 규제안 : 카드세팅 추가.(0 : 아침전. 1:아침)
		pRoomInfo->cCardSetting =	dlg.cCardSetting;

		//맥스방
		pRoomInfo->bIsMaxRoom			= dlg.m_bIsMaxRoom;
		pRoomInfo->llInMoney			= dlg.m_i64InMoney;

		//연합방
		pRoomInfo->cafeID_				= dlg.m_bUnionMode? GM().GetMyInfo()->UI.unitedCafeInfo_.cafeID_ : 0;
		//아침벳룰
		pRoomInfo->bMorningBet			= dlg.m_bBetRule;

		//3인 5인 플레이를 위한 추가 코드
		pRoomInfo->MaxUserNum		= dlg.m_nMaxUserNum;
		pRoomInfo->cMemberRoom		= 0;		
	}

	return nRet;

}

int CGame_LowBD::GetDirectEnterRoomType(int nType)
{
	return 0;
}

void CGame_LowBD::CheckDirectEnterRoomInfo(int nType, ROOMINFO *pRoomInfo)
{
	CString strTitle;	

	MM().Call(UPG_CMK_GENERATE_RANDOM_ROOM_NAME, 0, (LPARAM)(&strTitle));

	strncpy_s(pRoomInfo->Title, 32, strTitle, strTitle.GetLength());
	pRoomInfo->bSuperMaster = PBIsSuperRoom();
}

LRESULT CGame_LowBD::ShowQuickJoinerDlg(void)
{
	CQuickJoinDlg dlg(EQJ_LOWBD, &g_cUIData, GM().GetMainWnd());
	LRESULT nRet = dlg.DoModal();

	if(nRet == IDOK)
	{		
		GM().SendQuickJoiner();
	}

	return FALSE;
}

LRESULT	CGame_LowBD::SendQuickJoiner(void)
{
	if( OnMacroCheck() )
	{
		return FALSE;
	}

	//NMBASE::SOUND::g_Sound.PlayWav(SND45);

	//bool bQJEnable = CQuickJoinDlg::IsEnableQJ(EQJ_LOWBD);

	//if(!bQJEnable)
	//{
	//	GM().AskDirectEnterRoom(false, NULL, -1);
	//	return TRUE;
	//}

	// 규제 개선안 작업
	// 바로 시작을 로비에서 호출하면, 무조건 랜덤으로 입장.
	GM().AskDirectEnterRoom(false, NULL, GetDefaultFormKind());

	// 설정 값 사용하는 부분은 일단 사용안함.
	//SCONDITION sInfo; 
	//CString strSection;
	//strSection.Format("QJ%02d_%s_%d", QJ_VER, GM().GetMyInfo()->UI.ID, EQJ_LOWBD);

	//if(CQuickJoinDlg::GetSearchCondition(EQJ_LOWBD, strSection, &sInfo))
	//{
	//	CDataRoomOptionList *pDataRoomOptionList = GM().GetDataRoomOptionList();
	//	if(pDataRoomOptionList == NULL) return FALSE;

	//	//ROOMOPTION *pResultRoomOption = pDataRoomOptionList->GetQJRoomOption(sInfo.m_bUseBetRule, sInfo.m_bAutoBet, sInfo.m_bUseSeedMoney, sInfo.m_nSeedType, sInfo.m_sBetRuleKind );
	//	ROOMOPTION *pResultRoomOption = pDataRoomOptionList->GetQJRoomOption(sInfo.m_bUseGameMode, sInfo.m_nGameMode, sInfo.m_bUseSeedMoney, sInfo.m_nSeedType) ; 

	//	if(pResultRoomOption == NULL)
	//	{
	//		if(sInfo.m_bAutoEnterRoom)
	//		{
	//			GM().AskDirectEnterRoom(false, NULL, -1);
	//			return TRUE;
	//		}
	//		else
	//		{
	//			CRefuseQJDlg dlg(GM().GetMainWnd());
	//			int idResult = dlg.DoModal();
	//			if ( IDOK == idResult )
	//			{
	//				//다시 설정
	//				ShowQuickJoinerDlg();				
	//			}
	//			else if ( IDCANCEL == idResult )
	//			{
	//				//방만들기
	//				GM().ShowCreateRooomDlg();				
	//			}

	//			return TRUE;
	//		}
	//	}

	//	int nGameMode = GetDefaultFormKind();
	//	if(sInfo.m_bUseGameMode) nGameMode = sInfo.m_nGameMode;

	//	GM().AskDirectEnterRoom(true, pResultRoomOption, nGameMode, ECALL_BTNTYPE_LOBY_QUICK, true, sInfo.m_bAutoEnterRoom);
	//}
	//else
	//{
	//	// 일반적인 방 입장
	//	GM().AskDirectEnterRoom(false, NULL, -1);
	//}

	return TRUE;
}

CPlayer *CGame_LowBD::CreateNewPlayer(void)
{
	return new CPlayer_BD;
}

void CGame_LowBD::AddGameChat(const char *str, COLORREF crNewColor, bool bUserChat)
{
	g_pChatWinMan->AddChat( (char *)str, crNewColor, bUserChat);
}

void CGame_LowBD::AddGameInfo(const char *str, COLORREF crNewColor, bool bUserChat)
{
	g_pChatWinMan->AddInfo( (char *)str, crNewColor, bUserChat);
}

void CGame_LowBD::OnChangeFullScreen(bool bFullScreen)
{
	g_pGameView->m_cGameViewBtnMan.SetChangeDisplayButton(bFullScreen);
	g_pGameView->Page.ReCreateDIBSection();
}

void CGame_LowBD::OnCloseOptionDlg(LRESULT nRet)
{
	if(nRet == IDOK) 
	{
		GetGameView()->m_cGameViewBtnMan.SetBetBtnType(g_Config.eBetBtnType);
		GetGameView()->m_cGameViewBtnMan.CuttingKeyValue();
	}
}



void CGame_LowBD::OnFocusGame(bool bFocus)
{
	if(bFocus)
	{
		//토너먼트/바둑이 변경시 g_pGameView 전역변수 변경
		g_pGameView = (CGameViewLowBD*)GM().GetCurrentGameView();

		g_pChatWinMan->SetCurrnetChat();
		g_cOwnLimit.Init( &g_cUIData, g_GAME_BD() );
		g_cBanishMan.Init(&g_SeatKeeper, &g_ObserverMan);
		g_SeatKeeper.SetChangeThisPoint();
		//MM().AppendMsgH(UPG_CMK_SET_LIMIT_MONEY, Fnt(this, &CGame_LowBD::OnSetLimitMoney));

		MM().AddMsgH(UPG_CMK_SET_LIMIT_MONEY, Fnt(this, &CGame_LowBD::OnSetLimitMoney));
		MM().AddMsgH(UPG_CMK_SET_LIMIT_GOLD,  Fnt(this, &CGame_LowBD::OnSetLimitGold));
		MM().AddMsgH(UPG_CMK_GET_MONEY_GRADE, Fnt(this, &CGame_LowBD::OnGetMoneyGrade));
		MM().AddMsgH(UPG_CMK_GET_JOKBO_NAME, Fnt(this, &CGame_LowBD::OnGetJokboName));
		MM().AddMsgH(UPG_CMK_GET_MYJOKBO_NAME, Fnt(this, &CGame_LowBD::OnGetMyJokboName));
		MM().AddMsgH(UPG_CMK_GET_RESULT_MONEY, Fnt(this, &CGame_LowBD::OnGetResultMoney));
		MM().AddMsgH(UPG_CMK_CLEAR_JACKPOTMONEY, Fnt(this, &CGame_LowBD::OnClearJackPotMoney));	
		MM().AddMsgH(UPG_CMK_SET_PLAY_RAKEBACK_GOLD, Fnt(this, &CGame_LowBD::OnSetPlayRakeBackGold));
		MM().AddMsgH(UPG_CMK_SET_PLAY_RAKEBACK_GOLD_EVENT, Fnt(this, &CGame_LowBD::OnSetPlayRakeBackGoldEvent));
		
		MM().AddMsgH(UPG_CMK_GAME_RESULT_OPEN, Fnt(this, &CGame_LowBD::OnGetResultOpen));

		g_cPromotionMan.Reset();
		g_cPromotionMan.Init(&g_cUIData, this, &g_pGameView->m_cChatWindow);
		
		g_cPromotionMan.MakePromotion(CPromotionManager::EPT_JOKBOJACKPOT);		// 족보잭팟은 결과출력 타이밍을 위해 미리등록해 놓는다.
		
		g_cVipJackpot.Init(g_pGameView);
	}
	else
	{	
		//MM().RemoveAppendMsgH(UPG_CMK_SET_LIMIT_MONEY, Fnt(this, &CGame_LowBD::OnSetLimitMoney));
		MM().RemoveMsgH(UPG_CMK_SET_LIMIT_MONEY);
		MM().RemoveMsgH(UPG_CMK_SET_LIMIT_GOLD);
		MM().RemoveMsgH(UPG_CMK_GET_MONEY_GRADE);
		MM().RemoveMsgH(UPG_CMK_GET_JOKBO_NAME);
		MM().RemoveMsgH(UPG_CMK_GET_MYJOKBO_NAME);
		MM().RemoveMsgH(UPG_CMK_GET_RESULT_MONEY);
		MM().RemoveMsgH(UPG_CMK_CLEAR_JACKPOTMONEY);	
		MM().RemoveMsgH(UPG_CMK_SET_PLAY_RAKEBACK_GOLD);
		MM().RemoveMsgH(UPG_CMK_SET_PLAY_RAKEBACK_GOLD_EVENT);	

		MM().RemoveMsgH(UPG_CMK_GAME_RESULT_OPEN);
	}
}

LRESULT CGame_LowBD::OnGetResultMoney(WPARAM &wParam, LPARAM &lParam)
{
	INT64 *pMoney = (INT64 *)lParam;
	*pMoney = g_PLAYER( ( int )wParam )->m_llResultMoney;
	return TRUE;
}

LRESULT CGame_LowBD::OnGetJokboName(WPARAM &wParam, LPARAM &lParam)
{
	CString *pStrJokbo = (CString *)lParam;
	*pStrJokbo = g_PLAYER( ( int )wParam )->GetMyLowBDRule()->GetValueName();
	return TRUE;
}

LRESULT CGame_LowBD::OnGetMyJokboName(WPARAM &wParam, LPARAM &lParam)
{
	CString *pStrJokbo = (CString *)lParam;
	*pStrJokbo = g_PLAYER(0)->GetMyLowBDRule()->GetValueName();
	return TRUE;
}


LRESULT CGame_LowBD::OnGetMoneyGrade(WPARAM &wParam, LPARAM &lParam)
{
	return GetMoneyGrade(*((INT64 *)lParam));
}

LRESULT CGame_LowBD::OnSetLimitMoney(WPARAM &wParam, LPARAM &lParam)
{
	//보유한도
	g_cOwnLimit.SetMaxPremiumMoneyWithOption(*((INT64 *)lParam));
	return TRUE;
}

LRESULT CGame_LowBD::OnSetLimitGold(WPARAM &wParam, LPARAM &lParam)
{
	//보유한도
	g_cOwnLimit.SetMaxPremiumGoldWithOption(*((INT64 *)lParam));
	return TRUE;
}

CGameViewLowBD* CGame_LowBD::GetGameView()
{
	return (CGameViewLowBD*) m_pGameViewBase;
}

void CGame_LowBD::LoadSprite()
{	
	CGame::LoadSprite();

	LoadSpriteFile();
}

void CGame_LowBD::LoadSound()
{
	CGame::LoadSound();
	RegistSoundFiles();
}

void CGame_LowBD::ReleaseSprite()
{
	CGame::ReleaseSprite();
}

void CGame_LowBD::ReleaseSound()
{
	CGame::ReleaseSound();
}

const IDX_GAME CGame_LowBD::GetGameType(void)
{
	return IDX_GAME_BD;
}

const int CGame_LowBD::GetMaxRoom(void)
{
	return 90;
}

const int CGame_LowBD::GetMaxPlayer(void)
{
	return MAX_PLAYER;
}

const int CGame_LowBD::GetMaxGamePlayer(void)
{
	return MAX_PLAYER;
}

const int CGame_LowBD::GetMaxObserver(void)
{
	return MAX_OBSERVER;
}

const int CGame_LowBD::GetJokboQuestCount(void)
{
	return 6;

}

const CString CGame_LowBD::GetGameString(void)
{
	return "low";
}

const CString CGame_LowBD::GetGameName(void)
{
	return "로우바둑이";
}

const COLORREF	CGame_LowBD::GetGameColor(void)
{	
	return RGB(72,95,66);//각 게임 고유의 색을 반환
}


const int CGame_LowBD::GetGameCode(void)
{
	return GAME_CODE_BD;
}

const CString CGame_LowBD::GetLoadingBackPath(void)
{
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		return ".\\Lowbd\\image\\LoadingBack_toctoc.bmp";
	else
		return ".\\Lowbd\\image\\LoadingBack.bmp";
#else //UPGRADE_20120308_TOCTOC_CHANNELING
	return ".\\Lowbd\\image\\LoadingBack.bmp";
#endif //UPGRADE_20120308_TOCTOC_CHANNELING
}

const int CGame_LowBD::GetDefaultFormKind(void)
{
	return FORMKIND_CHOICE;

}
//시작버튼 활성화
LONG CGame_LowBD::OnReceiveStartButtonEnable( int nTotSize, char *lpData )
{
	CSV_STARTBTNENABLE msg;
	msg.Get( lpData, nTotSize);		

	//< CYS 101221
	g_pGameView->m_cGameViewBtnMan.SetShowBtn( CGameButtonBD::BTN_START, FALSE );
	
	if ( g_RI.State != 0 )
	{
		return 1;
	}

	int nNowUserNum = g_RI.NowUserNum;
	if ( g_RI.NowUserNum < LIMITGAMEPLAY_PLAYERNUM ) 
	{
		return 1;
	}

	if ( strncmp( msg.ID, GM().GetMyInfo()->UI.ID, 15 ) == 0 ) 
	{
		//시작버튼 활성화				
		g_pGameView->m_cGameViewBtnMan.SetShowBtn( CGameButtonBD::BTN_START, TRUE );
	}
	//> CYS 101221

	return 1;
}

//관전 ,참여 변경
LONG CGame_LowBD::ReceiveSV_OBSERVER_MESSAGE( int nTotSize, char *lpData )
{
	CSV_OBSERVER_MESSAGE msg;
	msg.Get(lpData, nTotSize);

	if ( GetCurrentWhere() != IDX_GAMEWHERE_GAME ) 
	{
		return 0;
	}
	
	CString str;
	CString chat;
	chat = (LPCTSTR)msg.StrChat;
	chat = chat.Mid(0, *msg.l_StrChat);
	str.Format("%s", chat.operator LPCTSTR());
	str += "\n";

	AddGameInfoViewEdit( str, GetChatColor( ECC2_SERVMSG ) );

	//			g_ObserverMan.IsMyObserver()Step = 0 ;

	int num = *msg.Kind;
	g_pGameView->m_cGameViewBtnMan.SetObserverBtn( num );

	if(num >= 1 && num < 100)
	{
		// 일단... 7Poker에서는 없음(종환)
		//g_VoiceMan.ObserverJoinRankVoice(num-1);
	}
	else if(num == 100)
	{
		g_pGameView->m_cPlayerDrawManager.SetObserverReservation(0, true);
	}
	else if(num == 200)
	{
		g_pGameView->m_cPlayerDrawManager.SetObserverReservation(0, false);
	}
	else if(num == 300)
	{
		g_ObserverMan.OnObserve();
	}
	else if(num == 500)
	{
		g_ObserverMan.OnObserve();
		ShowMsgModelessDlg(NULL, str.GetString());
	}
	return 1;	
}

LONG CGame_LowBD::ReceiveSV_RESERVATIONINFO(int nTotSize, char *lpData)
{
	CSV_RESERVATIONINFO msg;
	msg.Get(lpData, nTotSize);

	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) return 0;

	GetGameProcess()->GetObserverDlgProcess()->User_ResetReservation();

	for(int i=0; i<*msg.TotNum; i++) {
		char *id = msg.ReservedInfo[i].ID;
		char *NickName = msg.ReservedInfo[i].ID;
		int unum = msg.ReservedInfo[i].UNum;
		int seq =  msg.ReservedInfo[i].Seq;

		{
			GetGameProcess()->GetObserverDlgProcess()->User_SetItemReservation(unum, id, seq);
		}
	}

	//관전UI
	g_ObserverMan.OnReceiveReservationInfo(&msg);
	return 1;
}

LONG CGame_LowBD::ReceiveSV_ACCEPT_EMERAGE(int nTotSize, char *lpData)
{
	CSV_ACCEPT_EMERAGE msg;
	msg.Get(lpData, nTotSize);

	if ( GetCurrentWhere() != IDX_GAMEWHERE_GAME ) 
	{
		return 0;
	}

	int nowpnum = *msg.NowPNum;
	int newpnum = *msg.NewPNum;

	int MyID = -1;
	int i;
	for( i=0; i<GetMaxNewPlayer(); i++)	{
		if(strlen(g_PLAYER(i)->UI.ID) > 0) {	
			if(strcmp(GM().GetMyInfo()->UI.ID, g_PLAYER(i)->UI.ID)==0) { MyID = i; break; }
		}
	}

	BOOL bTrue = TRUE;
	if(newpnum < MAX_PLAYER || newpnum >= GetMaxNewPlayer()) bTrue = FALSE;

	if(MyID == -1 || nowpnum != g_PLAYER(0)->ServPNum || !bTrue) 
	{
		CSV_ASK_OUTROOM aormsg;
		aormsg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, REASON_OUTROOM_ABNORMAL);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), aormsg.pData, aormsg.GetTotalSize());
		return 0;
	}		

	g_pGameView->m_cGameViewBtnMan.InitObserverBtn();

	//g_MyObserverStep = 0;
	// 플레이어 순서에 맞게 초기화한다			
	g_GAME_BD()->SetPlayInfo(nowpnum, newpnum, FALSE);

	// ### [ 중복된 IP가 ] ###
	for( i=0; i<MAX_PLAYER; i++) {
		int pn = g_GAME_BD()->GetPNum_ByServPN(i);
		if(strlen(g_PLAYER(pn)->UI.ID) > 0)	{
			g_PLAYER(pn)->MyIpCheck = msg.IPA->IpIndex[i];
		}
	}

	if(g_RI.FormKind == 5)
	{
		if(g_Poker.nRound <= 1 && g_Poker.nState <= 4)
		{
			g_GAME_BD()->BlindBetEffectDraw();
		}
	}
	return 1;
}

LONG CGame_LowBD::ReceiveSV_ACCEPT_PARTICIPATION(int nTotSize, char *lpData)
{
	CSV_ACCEPT_PARTICIPATION msg;
	msg.Get(lpData, nTotSize);

	if( GetCurrentWhere() != IDX_GAMEWHERE_GAME )
	{
		return 0;
	}
	int nowpnum = *msg.NowPNum;
	int newpnum = *msg.NewPNum;

	int MyID = -1;
	int i;
	for( i=0; i<GetMaxNewPlayer(); i++)	{
		if(strlen(g_PLAYER(i)->UI.ID) > 0) {	
			if(strcmp(GM().GetMyInfo()->UI.ID, g_PLAYER(i)->UI.ID)==0) { MyID = i; break; }
		}
	}

	BOOL bTrue = TRUE;
	if(newpnum < 0 || newpnum >= MAX_PLAYER) bTrue = FALSE;
	if(MyID == -1 || nowpnum != MyID || !bTrue) 
	{
		CSV_ASK_OUTROOM aormsg;
		aormsg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, REASON_OUTROOM_ABNORMAL);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), aormsg.pData, aormsg.GetTotalSize());
		return 0;
	}

	// ### [사운드 추가 작업] ###
	g_PLAYER(MyID)->nSndFxKind = *msg.SndFxKind;
	//GM().GetMyInfo()->nSndFxKind = *msg.SndFxKind;

	g_pGameView->m_cGameViewBtnMan.InitObserverBtn();
	//			g_MyObserverStep = 0;
	// 플레이어 순서에 맞게 초기화한다			
	g_GAME_BD()->SetPlayInfo(nowpnum, newpnum, TRUE);

	// ### [ 중복된 IP가 ] ###
	for( i=0; i<MAX_PLAYER; i++) {
		int pn = g_GAME_BD()->GetPNum_ByServPN(i);
		if(strlen(g_PLAYER(pn)->UI.ID) > 0) {
			g_PLAYER(pn)->MyIpCheck = msg.IPA->IpIndex[i];
		}
	}

	//자리 지킴이
	g_SeatKeeper.OnEnterGame(); // 관전 -> 겜참여 일때 호출 

	//보유한도
	g_cOwnLimit.OnEnterRoom(&g_cUIData, g_GAME_BD());	

	return 1;
}

LONG CGame_LowBD::ReceiveSV_ENTER_EMERAGE(int nTotSize, char *lpData)
{
	CSV_ENTER_EMERAGE msg;
	msg.Get(lpData, nTotSize);
	if( GetCurrentWhere() != IDX_GAMEWHERE_GAME )
	{
		return 0;
	}

	int nowpnum = *msg.NowPNum;
	int newpnum = *msg.NewPNum;

	if(nowpnum < 0 || nowpnum >= MAX_PLAYER)
	{
		return 0;
	}

	if(newpnum < MAX_PLAYER || newpnum >= GetMaxNewPlayer()) 
	{
		return 0;
	}

	int pn = g_GAME_BD()->GetPNum_ByServPN(nowpnum);
	if(strcmp( g_PLAYER(pn)->UI.ID, msg.ID) != 0)
	{
		return 0;
	}

	g_PLAYER(newpnum)->SetNewPlayer(&g_PLAYER(pn)->UI);
	g_PLAYER(newpnum)->UI.cObserver = 1;
	g_PLAYER(newpnum)->nSndFxKind = g_PLAYER(pn)->nSndFxKind;

	g_PLAYER(pn)->Clear();
	g_RI.NowUserNum -= 1;
	g_RI.NowObserverNum += 1;

	CString str;
	str.Format("◁참여 → 관전: [%s]님\n", g_PLAYER(newpnum)->UI.NickName);
	AddGameInfoViewEdit( str, GetChatColor( ECC2_NOTIFY ) );

	// ### [ 중복된 IP가 ] ###
	for(int i=0; i<MAX_PLAYER; i++)	{
		int pn = g_GAME_BD()->GetPNum_ByServPN(i);
		if(strlen(g_PLAYER(pn)->UI.ID) > 0)	{
			g_PLAYER(pn)->MyIpCheck = msg.IPA->IpIndex[i];
		}
	}

	int ounum = g_PLAYER(newpnum)->UI.UNum;
	int sex  = g_PLAYER(newpnum)->UI.Sex;
	int level = g_PLAYER(newpnum)->UI.nIcon;
	/*INT64 Money = g_PLAYER(newpnum)->UI.GetMoney(IsTournament());*/
	INT64 Money = g_PLAYER(newpnum)->UI.GetMoney();
	int seq = 0;
	GetGameProcess()->GetObserverDlgProcess()->User_InsItem(ounum,g_PLAYER(newpnum)->UI.ID, g_PLAYER(newpnum)->UI.NickName, sex, level, Money ,seq);

	return 1;
}

LONG CGame_LowBD::ReceiveSV_ENTER_PARTICIPATION(int nTotSize, char *lpData)
{
	CSV_ENTER_PARTICIPATION msg;
	msg.Get(lpData, nTotSize);

	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
	{
		return 0;
	}
	int nowpnum = *msg.NowPNum;
	int newpnum = *msg.NewPNum;
	if(newpnum < 0 || newpnum >= MAX_PLAYER) 
	{
		return 0;
	}
	if(nowpnum < MAX_PLAYER || nowpnum >= GetMaxNewPlayer()) 
	{
		return 0;
	}

	//관전의 pnum 인덱스는 서버와 일치한다
	if(strcmp( g_PLAYER(nowpnum)->UI.ID, msg.ID) != 0) 
	{
		return 0;
	}

	int i=0;
	int pnum = -1; // 플레이어 번호
	if( !g_ObserverMan.IsMyObserver()) // 1) 내가 겜참여중이고 겜참여 넘 입장
	{
		for(i=g_GAME_BD()->m_nNo1PosPNum; i<MAX_PLAYER+g_GAME_BD()->m_nNo1PosPNum; i++) {	
			int index = i;
			if(index>=MAX_PLAYER) index = index - MAX_PLAYER;
			if(index==0) continue; //나
			if(strlen(g_PLAYER(index)->UI.ID)==0){pnum=index; break;}
		}

		if ( pnum == -1)
		{
			return 0; // 비어있는 곳을 찾지 못했으면
		}

		g_pGameView->m_cPlayerDrawManager.Reset(pnum);
		g_PLAYER(nowpnum)->UI.cObserver = 0;
		g_PLAYER(pnum)->SetNewPlayer(&g_PLAYER(nowpnum)->UI);		
		g_PLAYER(pnum)->SetChangeRoomInfo(*g_PLAYER(nowpnum)->GetChangeRoomInfo());
		g_PLAYER(nowpnum)->Clear();
		g_pGameView->m_cPlayerDrawManager.SetShow(pnum, true);	// 이 값을 세팅해야 유저가 화면에 보인다.
		g_RI.NowUserNum += 1;
		g_RI.NowObserverNum -= 1;

		g_PLAYER(pnum)->ServPNum = g_GAME_BD()->GetServPNum_ByPN(pnum);
#ifdef _DEBUG
		if(newpnum != g_PLAYER(pnum)->ServPNum)
		{
			CString str;
			str.Format("newpnum  =%d ,  pnum = %d, ServPNum = %d", newpnum, pnum, g_PLAYER(pnum)->ServPNum);
			AfxMessageBox(str);
		}
#endif
		CString str;				
		str.Format("▶관전 → 참여: [%s]님\n", g_PLAYER(pnum)->UI.NickName);
		AddGameInfoViewEdit( str, GetChatColor( ECC2_NOTIFY ) );
	}
	// 3)내가 관전중이고  겜참여 하는넘 입장
	else
	{
		for(i=0; i<MAX_PLAYER; i++)	{
			if(strlen(g_PLAYER(i)->UI.ID)==0){pnum=i; break;}
		}
		if(pnum == -1)
		{
			return 0; // 비어있는 곳을 찾지 못했으면
		}

		g_GAME_BD()->m_nMyPosPNum = 0; // 서버쪽에 인덱스
		g_GAME_BD()->m_nNo1PosPNum = g_GAME_BD()->GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스

		// 방 정보를 수정하고 새로 참여한 플레이어의 정보를 저장
		g_pGameView->m_cPlayerDrawManager.Reset(pnum);
		g_PLAYER(nowpnum)->UI.cObserver = 0;
		g_PLAYER(pnum)->SetNewPlayer(&g_PLAYER(nowpnum)->UI);
		g_PLAYER(pnum)->SetChangeRoomInfo(*g_PLAYER(nowpnum)->GetChangeRoomInfo());
		g_PLAYER(nowpnum)->Clear();
		g_pGameView->m_cPlayerDrawManager.SetShow(pnum, true);	// 이 값을 세팅해야 유저가 화면에 보인다.
		g_RI.NowUserNum += 1;
		g_RI.NowObserverNum -= 1;

		g_PLAYER(pnum)->ServPNum = g_GAME_BD()->GetServPNum_ByPN(pnum);// 서버측 플레이어 번호 저장

		CString str;
		str.Format("▶관전 → 참여: [%s]님\n", g_PLAYER(pnum)->UI.NickName);
		AddGameInfoViewEdit( str, GetChatColor( ECC2_NOTIFY ) );

	}

	// ### [ 중복된 IP가 ] ###
	for( i=0; i<MAX_PLAYER; i++) {
		int pn = g_GAME_BD()->GetPNum_ByServPN(i);
		if(strlen(g_PLAYER(pn)->UI.ID) > 0)	{
			g_PLAYER(pn)->MyIpCheck = msg.IPA->IpIndex[i];
		}
	}

	// ### [사운드 추가 작업] ###
	g_PLAYER(pnum)->nSndFxKind = *msg.SndFxKind;
	GetGameProcess()->GetObserverDlgProcess()->User_DelItem(g_PLAYER(pnum)->UI.UNum, g_PLAYER(pnum)->UI.ID);

	return 1;
}

//유저 정보창은 게임방/대기실 동일하고 게임쪽에 존재하지 때문에 게임쪽에서 띄운다.
LONG CGame_LowBD::OnReceiveUserInfo(int TotSize, char *lpData)
{

	CSV_USERINFO msg;
	msg.Get(lpData, TotSize);

	GM().SetWaitResponse(false);

	if ( GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return FALSE;


	// 아이디 길이가 0이면 현재 명단에 없는 사용자임
	if(strlen(msg.UI->ID) == 0) return FALSE;

	static BOOL bProccessed = FALSE;
	if(bProccessed != FALSE) return FALSE;
	bProccessed = TRUE;
	ShowProfileDialog(msg.UI, NULL);
	bProccessed = FALSE;
	return TRUE;
}


LONG CGame_LowBD::OnReceiveNotifyChangeOverMoney(int TotSize, char *lpData)
{	
	CSV_NOTIFY_CHANGEOVERMONEY msg;
	msg.Get(lpData,TotSize);

	if (strncmp(msg.szID, GM().GetMyInfo()->UI.ID, 15)==0)
	{
		if (*msg.llOverMoney>0)
		{
			//초과금
			g_cOwnLimit.SetOverMoney(*msg.llOverMoney, *msg.m_ChipKind, msg.stOverMoneyEndDate);
		}

		if (*msg.llRevisionMoney>0)
		{
			//초과금
			g_cOwnLimit.SetRevisionMoney(*msg.llRevisionMoney,*msg.m_ChipKind,  msg.stRevisionMoneyEndDate);
		}			
	}
	else
	{
		g_cOwnLimit.SetOtherUserOverMoney(msg.szID, *msg.llOverMoney, *msg.m_ChipKind, msg.stOverMoneyEndDate);
	}

	if(GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME)
	{
		g_cOwnLimit.ShowPopup();
	}

	return TRUE;
}

//귓속말
LONG CGame_LowBD::OnReceiveWhisperChat(int TotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) return FALSE;

	CSV_WHISPERCHAT msg;
	msg.Get(lpData, TotSize);

	//귓말 수신 거부일때 모두 무시
	if(CCommunityManager::Instance()->GetOption(COM_ECOT_WHISPER) == COM_ECOPT_OFF) return FALSE;
	//블랙리스트의 유저가 보낸 귓말은 무시
	if(CCommunityManager::Instance()->IsExistBlackBuddy(msg.m_szNickName)) return TRUE;
	//친구에게만 귓말을 받기 옵션일 때 친구가 아닌사람이 보낸 귓말이면 무시
	if((CCommunityManager::Instance()->GetOption(COM_ECOT_WHISPER) == COM_ECOPT_MYBUDDY) && !CCommunityManager::Instance()->IsExistBuddy(msg.m_szNickName)) return TRUE;

	// 대화 수신 거부중인 아이디이면 무시
	int pnum = g_GAME_BD()->GetPlayerPNum(msg.m_szID);

	CString str;
	CString chat;
	chat.Append((LPCTSTR)msg.m_StrChat, *msg.m_sStrChatLen);
	//(추가)
	// 허용되지 않은 특수문자를 사용하였다면 공백문자로 바꿈
	for(int s=0; s<chat.GetLength()-1; s++) {
		// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
		if(chat.GetAt(s) == 8 || chat.GetAt(s) == 9 || chat.GetAt(s) == 10 || chat.GetAt(s) == 13) {
			chat.SetAt(s, ' ');
		}
	}

	// 닉네임 작업 2006/11/08		
	char szTBuf[512];
	NMBASE::UTIL::NMIsBadToRep((char *)(LPCTSTR)chat, szTBuf, 512 );
	str.Format("[귓말도착:%s] %s\n", msg.m_szNickName, szTBuf);

	// 귓말 알림 효과음 출력
	NMBASE::SOUND::g_Sound.PlayWav(SND_WHISPER);
	AddGameChatViewEdit(str, GetChatColor(ECC2_WHISPER_RECEIVE));

	//귓말사용자추가
	AddChattingWhisperList( msg.m_szNickName, false, true, false );
	//g_pChatWinMan->AddWhipserUser( msg.m_szNickName );

	// 닉네임 작업 2006/11/08
	str.Format("귓말: ID : %s(%s)> %s\n", msg.m_szID ,  msg.m_szNickName , szTBuf );
	AddLobbyHideChatText(&str);

	return TRUE;
} 


LONG CGame_LowBD::OnRecevieAdminWhisperChat(int TotSize, char *lpData)
{
	// 관리자모드작업
	CNM_ADMIN_WHISPERCHAT msg;
	msg.Get(lpData, TotSize);

	CString str;
	CString chat;
	chat.Append((LPCTSTR)msg.StrChat, *msg.l_StrChat);

	//(추가)
	// 허용되지 않은 특수문자를 사용하였다면 공백문자로 바꿈
	for(int s=0; s<chat.GetLength()-1; s++) 
	{
		// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
		if(chat.GetAt(s) == 8 || chat.GetAt(s) == 9 || chat.GetAt(s) == 10 || chat.GetAt(s) == 13) 
		{
			chat.SetAt(s, ' ');
		}
	}
	str.Format("[귓말:%s] %s\n", "관리자", chat.operator LPCTSTR());
	// 귓말 알림 효과음 출력
	NMBASE::SOUND::g_Sound.PlayWav(SND_WHISPER);		
	AddGameChatViewEdit(str.GetString(), GetChatColor(ECC2_WHISPER_RECEIVE));

	return TRUE; 
} 

//나가기예약
LONG CGame_LowBD::OnReceiveExitSubscript(int TotSize, char *lpData)
{
	CCL_EXITSUBSCRIPT msg;
	msg.Get(lpData, TotSize);

	int pnum = g_GAME_BD()->GetPlayerPNum( msg.ID );
	if(pnum==-1) return FALSE;

	int bexit = *msg.bExit;

	CString str;
	// ### [ 관전기능 ] ###
	if(bexit == 0)
	{
		if(strcmp(GM().GetMyInfo()->UI.ID, msg.ID) == 0)
		{// 나가기
			g_pGameView->m_bExitSubscript = FALSE;
			ExitGameRequest();
		}
	}
	else if(bexit==1)
	{
		if(strcmp(GM().GetMyInfo()->UI.ID, msg.ID) == 0)
		{// 나가기
			g_pGameView->m_bExitSubscript = TRUE;
		}
		// 닉네임수정 				
		str.Format("[%s]님 나가기 예약\n", g_PLAYER( pnum )->UI.NickName);

		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
		g_pGameView->m_cPlayerDrawManager.SetExitReservation(pnum, true);
	}
	else if(bexit == 2)
	{
		if(strcmp(GM().GetMyInfo()->UI.ID, msg.ID) == 0)
		{// 나가기
			g_pGameView->m_bExitSubscript = FALSE;
		}
		// 닉네임수정 	
		str.Format("[%s]님 나가기 취소\n", g_PLAYER( pnum )->UI.NickName);

		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
		g_pGameView->m_cPlayerDrawManager.SetExitReservation(pnum, false);
	}
	return TRUE;
}



//올인보험 지급 요청 결과
//case SV_AIN_INSURANCE_SUPPLY_RESULT:{ return OnReceiveInsuranceSupplyResult( nTotSize, lpData );  } break;
LONG CGame_LowBD::OnReceiveInsuranceSupplyResult(int TotSize, char *lpData)
{
	CSV_AIN_INSURANCE_SUPPLY_RESULT msg;
	msg.Get(lpData,TotSize);

	if(msg.AIN_DATA->UNum != GM().GetMyInfo()->UI.UNum) return FALSE;			
	if(strlen(msg.AIN_DATA->ID) == 0 || strcmp(msg.AIN_DATA->ID,GM().GetMyInfo()->UI.ID) != 0) return FALSE;			
	if(msg.AIN_DATA->nAIN_Insurance_Result == 0){			
		ShowMainMessageDlg("올인보험 적용을 실패 하였습니다. 다시 시도해 주세요");
		return FALSE;
	}

	/*GM().GetMyInfo()->UI.SetMoney(msg.AIN_DATA->nApplyCurPMoney, IsTournament());*/
	GM().GetMyInfo()->UI.SetMoney(msg.AIN_DATA->nApplyCurPMoney);
	GM().GetMyInfo()->UI.nAIN_Insurance_Money = msg.AIN_DATA->nCurAIN_InsuranceMoney;
	GM().GetMyInfo()->UI.nAIN_InsuranceKind = msg.AIN_DATA->nCurAIN_InsuranceKind;
	/*GM().GetMyInfo()->UI.nIcon = GetMoneyGrade(GM().GetMyInfo()->UI.GetTotalMoney(IsTournament()));*/
	GM().GetMyInfo()->UI.nIcon = GetMoneyGrade(GM().GetMyInfo()->UI.GetTotalMoney());

	/*g_PLAYER( 0 )->UI.SetMoney(GM().GetMyInfo()->UI.GetMoney(IsTournament()), IsTournament());*/
	g_PLAYER( 0 )->UI.SetMoney(GM().GetMyInfo()->UI.GetMoney());
	g_PLAYER( 0 )->UI.nAIN_Insurance_Money = GM().GetMyInfo()->UI.nAIN_Insurance_Money;
	g_PLAYER( 0 )->UI.nAIN_InsuranceKind = GM().GetMyInfo()->UI.nAIN_InsuranceKind;
	g_PLAYER( 0 )->UI.nIcon = GM().GetMyInfo()->UI.nIcon;
	return TRUE;
}

LONG CGame_LowBD::OnReceiveSV_Reward_Allin_Angel(int TotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return FALSE;

	CSV_REWARD_ALLIN_ANGEL msg;
	msg.Get(lpData, TotSize);
	g_pGameView->m_cPlayerDrawManager.SetPlayAllinAngel((*msg.m_RemainEnableCount), timeGetTime());
	return TRUE;
}

LONG CGame_LowBD::OnReceiveSV_Reward_Allin_Angel_Gold(int TotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return FALSE;

	CSV_REWARD_ALLIN_ANGEL_GOLD msg;
	msg.Get(lpData, TotSize);
	g_pGameView->m_cPlayerDrawManager.SetPlayGoldReward((*msg.m_RewardMoney), timeGetTime());
	return TRUE;
}
/*
case SV_CHANGE_GAMEUSERINFO:		{ return OnReceiveChangeGameUserInfo( nTotSize, lpData ); } break;
LONG  CGame_LowBD::OnReceiveChangeGameUserInfo(int TotSize, char *lpData)
{
	CSV_CHANGE_GAMEUSERINFO msg;
	msg.Get(lpData,TotSize);

	const int nMaxPlayer = GAME()->GetMaxPlayer();
	for( int i=0; i < nMaxPlayer; i++ )
	{
		if (g_PLAYER( i )->PNum == msg.pCGUI->nPNum) 
		{
			g_PLAYER( i )->UI.DS_IDColor = msg.pCGUI->nIDColor;
			break;
		}
	}
	return TRUE;
}
*/


LONG CGame_LowBD::OnPacketNotify(char* pName, int nSignal, int nTotSize, char *lpData)
{
	if (CGame::OnPacketNotify(pName, nSignal, nTotSize, lpData))
		return TRUE;

	if(g_cOneShotCharge.OnPacketNotify(pName, nSignal, nTotSize, lpData)) return TRUE;
	if(g_pGameView->m_cGameHelper.OnPacketNotify(pName, nSignal, nTotSize, lpData)) return TRUE;
	if(g_cOwnLimit.OnPacketNotify(pName, nSignal, nTotSize, lpData))	return TRUE;
	if(g_cBanishMan.OnPacketNotify(pName, nSignal, nTotSize, lpData))	return TRUE;
	if( g_cPromotionMan.OnPacketNotify(pName, nSignal, nTotSize, lpData) ) return TRUE;

	if( g_cVipJackpot.OnPacketNotify(pName, nSignal, nTotSize, lpData) ) return TRUE;


	/*switch( nSignal )
	{
	case SV_USERINFO:				{ return OnReceiveUserInfo( nTotSize, lpData ); } break;
	case SV_NOTIFY_CHANGEOVERMONEY:	{ return OnReceiveNotifyChangeOverMoney( nTotSize, lpData ); } break;
	}*/

	//CClientDlg::OnPacketNotify 에서 게임에 관련된 패킷만 여기로 이동
	switch( nSignal)
	{
	case SV_STARTBTNENABLE:	      {	return OnReceiveStartButtonEnable( nTotSize, lpData );	} break;
	case SV_OBSERVER_MESSAGE:     { return ReceiveSV_OBSERVER_MESSAGE( nTotSize, lpData );  } break;// ### [ 관전기능 ] ###
	case SV_RESERVATIONINFO:      {	return ReceiveSV_RESERVATIONINFO( nTotSize, lpData );	} break; // 예약자 정보
	case SV_ACCEPT_EMERAGE:		  { return ReceiveSV_ACCEPT_EMERAGE( nTotSize, lpData );    } break;  // 겜참여 -> 관전
	case SV_ACCEPT_PARTICIPATION: { return ReceiveSV_ACCEPT_PARTICIPATION(  nTotSize, lpData ); } break; // 관전 -> 겜참여
	case SV_ENTER_EMERAGE:		  {	return ReceiveSV_ENTER_EMERAGE(  nTotSize, lpData ); } break; // 겜참여 -> 관전
	case SV_ENTER_PARTICIPATION:  {	return ReceiveSV_ENTER_PARTICIPATION(  nTotSize, lpData ); } break; // 관전 -> 겜참여
	case SV_WHISPERCHAT:		  {	return OnReceiveWhisperChat( nTotSize, lpData ); } break; //귓속말
	case NM_ADMIN_WHISPERCHAT:	  {	return OnRecevieAdminWhisperChat( nTotSize, lpData ); } break; //귓속말
	case CL_EXITSUBSCRIPT:		  { return OnReceiveExitSubscript( nTotSize, lpData ); } break;
	case SV_REWARD_ALLIN_ANGEL:	  { return OnReceiveSV_Reward_Allin_Angel( nTotSize, lpData ); } break;
	case SV_REWARD_ALLIN_ANGEL_GOLD:	{ return OnReceiveSV_Reward_Allin_Angel_Gold( nTotSize, lpData ); } break;
	case SV_GAMEOVERRESULT: // 게임이 종료되었음
		{
			CSV_GAMEOVERRESULT msg;
			msg.Get(lpData, nTotSize);

			if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) break;
			GM().SetWaitResponse(FALSE);
			static BOOL bProccessed = FALSE;
			if(bProccessed != FALSE) break;
			bProccessed = TRUE;
			g_pGameView->m_cPlayerDrawManager.ClearTimer();
			g_GAME_BD()->OnGameOver(msg.GOR);
			g_GAME_BD()->DellBlindBetEffect();
			g_cOwnLimit.OnGameOver();
			bProccessed = FALSE;
		} break;

		// 게임방 채팅데이타
	case CL_CHATDATA:
		{
			CCL_CHATDATA msg;
			msg.Get(lpData, nTotSize);

			if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) break;
			if(*msg.RoomNum != g_RI.RoomNum) break;

			// 대화 수신 거부중인 아이디이면 무시
			int pnum = g_GAME_BD()->GetPlayerPNum(msg.ID);
			BOOL bAdmin  = (strcmp("관리자",msg.ID)==0);


			if(g_cUIData.m_bObserverChat)
			{

			}
			else
			{
				//관전자 채팅 끄기 
				if (strncmp(msg.ID, GM().GetMyInfo()->UI.ID, 15)==0)
				{
					/*if (g_ObserverMan.IsMyObserver() && g_GAME_BD()->m_bObserverChatWarning==FALSE)
					{
						AddGameChatViewEdit("[알림] 관전자 채팅끄기를 설정하신 분께는 관전채팅 내용이 보여지지 않습니다.\n", GetChatColor(ECC2_NOTIFY));
						g_GAME_BD()->m_bObserverChatWarning=TRUE;
					}*/
				}
				else
				{			
					if (g_Config.bNoObserverChat && !bAdmin)
					{					
						if (pnum>=MAX_PLAYER && pnum < GetMaxNewPlayer()-1)
						{					
							break;
						}					
					}
				}
			}

			if(pnum < 0 && !bAdmin)
			{
				break;
			}


			CString str;
			char chat[256]={0,};
			strncpy(chat, msg.StrChat, min(*msg.l_StrChat, 255));

			//(추가)
			// 허용되지 않은 특수문자를 사용하였다면 공백문자로 바꿈
			int slen = strlen(chat);
			for(int s=0; s<slen-1; s++) {
				// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
				if(chat[s] == 8 || chat[s] == 9 || chat[s] == 10 || chat[s] == 13) {
					chat[s] = ' ';
				}
			}	

			char szTBuf[512];
			NMBASE::UTIL::NMIsBadToRep( chat, szTBuf, 512 );

			if( bAdmin )// ### [관리자 모드 작업] ###
			{				
				str.Format("관리자> %s\n",szTBuf);
				AddGameChatViewEdit( str, GetChatColor( ECC2_SERVMSG ) );
				
			}
			else
			{	
				if(g_cUIData.m_bObserverChat)
				{
					if(g_ObserverMan.IsMyObserver())
					{
						if( pnum >= g_GAME_BD()->GetMaxPlayer())
						{
							str.Format("%s> %s\n", g_PLAYER(pnum)->UI.NickName , szTBuf);
							AddGameChatViewEdit(str, GetChatColor(ECC2_CHAT), true);
						}
					}
					else
					{
						if( pnum < g_GAME_BD()->GetMaxPlayer())
						{
							str.Format("%s> %s\n", g_PLAYER(pnum)->UI.NickName , szTBuf);
							AddGameChatViewEdit(str, GetChatColor(ECC2_CHAT), true);
						}
					}
				}
				else
				{
					str.Format("%s> %s\n", g_PLAYER(pnum)->UI.NickName , szTBuf);
					AddGameChatViewEdit( str, GetChatColor( ECC2_CHAT ), true );
				}

				// 닉네임수정 	2006/10/30
				//	str.Format("## ID: %s(%s)> %s\n",g_PLAYER(pnum)->UI.ID , g_PLAYER(pnum)->UI.NickName , chat );
				str.Format("## ID: %s(%s)> %s\n",g_PLAYER(pnum)->UI.ID , g_PLAYER(pnum)->UI.NickName , szTBuf );
			}

			

			//UPG작업 한 줄 교체
			//GetGameProcess()->AddHideChatText( &str );
			AddLobbyHideChatText(&str);


			//여기까지

		} break;

		/////////////////////////// 클라이언트용 메세지  ////////////////////////

		case SV_CHANGESTYLEROOMINFO:		  
			{	
				return OnReceiveChangeStyleRoomInfo( nTotSize, lpData ); 
			} break; //BTS 모니터링 툴 방제 변경 패킷. 

		/////////////////////////// 게임 진행용 메세지  ////////////////////////

		// 게임 시작
		case SV_STARTCARD:
			{
				CSV_STARTCARD msg;
				msg.Get(lpData, nTotSize);
				if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) break;

				int pnum = g_GAME_BD()->GetPlayerPNum(msg.ID);
				if(pnum == -1) break;
				// ### [ 관전기능 ] ###
				if(strcmp(GM().GetMyInfo()->UI.ID, msg.ID) != 0) break;

				g_pGameView->m_cGameViewBtnMan.SetShowBtn( CGameButtonBD::BTN_START, FALSE );	//< CYS 101221 >

				// 전광판 출력
				g_pGameView->m_cDisplayBoard.SetText(EDBT_GAMESTART);
				g_pGameView->m_cSidePotMoney.Reset();

				static BOOL bProccessed = FALSE;
				if(bProccessed) break;
				bProccessed = TRUE;						

				// 게임 리셋 예약
				g_GAME_BD()->ResetGame();
				g_pGameView->m_cTotalBetMoney.Start();
				g_GAME_BD()->DoPrepareGame(msg.pSC);
				g_RI.State = 1;			

				CWnd * pWnd = GM().GetMainWnd()->GetForegroundWindow();
				if(pWnd != NULL){
					if(pWnd != GM().GetMainWnd()){
						GM().GetMainWnd()->FlashWindow(TRUE);
					}				
				}
				
				GetGameProcess()->m_bBlind = *msg.m_BlindBet;
				if(GetGameProcess()->m_bBlind)
				{
					GetGameProcess()->BlindBetEffectDraw();
				}

				bProccessed = FALSE;
				
				// 게임 시작과 동시 딜러 정보 표시용
				g_Poker.nImageBoss = msg.pSC->nImageBoss;

				//ZeroMemory(&m_RewardItemData , sizeof(REWARDITEM_DATA));						

			}break;
		case SV_BLINDSHOW:
			{
				CSV_BLINDSHOW msg;
				msg.Get(lpData, nTotSize);
				if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) break;

				GetGameProcess()->DellBlindBetEffect();
				GetGameProcess()->ShowCard(msg.pBlindShowInfo->nCard);
				GetGameProcess()->AutoSort_Recommand();
				GetGameProcess()->AutoUp_Recommand();
				g_PLAYER( 0 )->SetCardResult();
			}break;
		case SV_SELECTCARD:
			{
				CSV_SELECTCARD msg;
				msg.Get(lpData, nTotSize);

				if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) break;
				if(g_RI.State == 0) break;

				int pnum = g_GAME_BD()->GetPlayerPNum(msg.ID);
				if(pnum == -1) break;

				if(pnum < 0  || pnum >= MAX_PLAYER) break;
				if( g_PLAYER(pnum)->JoinState==0 ) break;
				if( !g_PLAYER(pnum)->m_bSelectCard ) break;

				if(g_RI.FormKind == CHOICE_GAME && g_PLAYER(pnum)->m_nTotalCardNum == 3) {
					g_PLAYER(pnum)->m_bSelectCard = FALSE;
					break;
				}

				/////////////////////////////////////////////////////////////////////
				// 오픈카드 선택시 각각 타이머 보여준다.
				// ### [ 관전기능 ] ###			
				if ( g_GAME_BD()->m_bGameStart && pnum != 0 && g_RI.FormKind == CHOICE_GAME && g_PLAYER(pnum)->PlayState == 1) 
				{
					// 추가 초이스 카드
					g_PLAYER(pnum)->m_cMyCard[ 3].Clear();
					g_PLAYER(pnum)->m_nTotalCardNum -= 1;
				}

				/////////////////////////////////////////////////////////////////////
				g_PLAYER(pnum)->m_bSelectCard = FALSE;			

				if(pnum != 0){
					g_PLAYER(pnum)->SetChioceCardMove(TRUE);
				}

			} break;

			// 유저 모두 카드날리기 종료
		case SV_WINDCARDEND : 
			{
				CSV_WINDCARDEND msg;
				msg.Get(lpData, nTotSize);
				if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) break;
				if(g_RI.State == 0) break;

				int pnum = *msg.Reserve;
				if(pnum < 0 || pnum >= MAX_PLAYER) break;

				static BOOL bProccessed = FALSE;
				if(bProccessed) break;
				bProccessed = TRUE;

				g_GAME_BD()->EndWindCard();

				bProccessed = FALSE;

			}
			break;

		case SV_RAISE:
			{
				CSV_RAISE msg;
				msg.Get(lpData, nTotSize);
				if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) break;
				if(g_RI.State == 0) break;		

				//g_GAME_BD()->m_dwBackTime = 0;
				g_pGameView->m_cPlayerDrawManager.ClearTimer();
				g_GAME_BD()->Raise(msg.pPG);
				g_GAME_BD()->SetCardExchangeLog(msg);

			}
			break;

			// 유저가 폴드
		case SV_FOLDUSER:
			{
				CSV_FOLDUSER msg;
				msg.Get(lpData, nTotSize);

				if ( GetCurrentWhere() != IDX_GAMEWHERE_GAME ) break;

				if ( g_RI.State == 0 ) break;
				if ( strlen( msg.ID )==0 ) break;
				int pnum  = -1;
				for( int i = 0; i < MAX_PLAYER; i++ ) 
				{
					if ( strcmp( msg.ID, g_PLAYER( i )->UI.ID ) == 0 ) 
					{ 
						pnum = i; 
						break; 
					}
				}

				if ( pnum == -1) break;		

				// 바둑이 다이 시 카드 정렬 시킴, 게임에서 포기한사람 카드를 접는다.
				g_GAME_BD()->FoldUserProcess( pnum );
			} break;

			// 마지막 히든 카드
		case SV_HIDDENCARD:
			{
				CSV_HIDDENCARD msg;
				msg.Get(lpData, nTotSize);

				if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) break;
				if(g_RI.State != 1) break;

				if(strlen(msg.ID)==0) break;
				if( g_ObserverMan.IsMyObserver() ) break;

				int pnum = -1;
				for(int i=0; i<MAX_PLAYER; i++) {
					if(strcmp(msg.ID, g_PLAYER(i)->UI.ID) == 0) { pnum = i; break; }
				}
				if(pnum != 0) {
#ifdef _DEBUG
					AfxMessageBox("Hidden Error");			
#endif			break;
				}

				if(g_PLAYER(pnum)->PlayState == 1 && g_PLAYER(pnum)->JoinState == 1)
				{
					g_PLAYER(pnum)->m_cMyCard[ 6].SetCardNo( *msg.nCard ); // 값 변경
					g_PLAYER(pnum)->m_cMyCard[ 6].SetHiddenCard( true );
				}

				// 자신의 카드위에 족보명을 보위기 위함
				g_pGameView->m_cPlayerDrawManager.MyJokboNameUpdate();
				//NMBASE::SOUND::g_Sound.PlayOggStream(SND_HIDDEN_BGM);

			} break;

		case SV_ENDGAME:
			{
				CSV_ENDGAME msg;
				msg.Get(lpData, nTotSize);
				if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) break;

				//animan moment destroy
				g_pGameView->m_cAniSprListManager.Destroy(0);
				// 나가기 예약상태면 게임 종료

				BOOL bExitSubscript = m_pGameViewBase->m_bExitSubscript;

				//yoo [20150120] 
				if(TRUE==CGame::OnReceiveResEndGame())
				{
					ExitGameRequest();
				}
				else if(bExitSubscript) 
				{
					ExitGameRequest();
				}
				else 
				{
					// 게임 리셋 예약
					GetGameProcess()->ResetGame();
				}

		
				int nTimeoutCount;
				nTimeoutCount = MAX_TURN_TIMEOUT_LIMIT_COUNT;

				if ( !g_ObserverMan.IsMyObserver() && g_PLAYER( 0 )->GetTurnLimitCnt() >= nTimeoutCount )
				{
					g_PLAYER( 0 )->ResetTurnLimitCnt();
					ExitGameRequest(true);
					CString strOutput;
					strOutput.Format("게임 시작 후 %d회 이상 수동으로 버튼 클릭을 하지 않아 게임방에서 자동 퇴장 하였습니다.\n", nTimeoutCount);
					ShowMainMessageDlg(strOutput);
				}
		
				g_cPromotionMan.OnEndGame();

				//매크로 프로그램이 있으면 종료 시킨다
				OnMacroCheck();
			}
			break;
			// [ ###잭팟### ] 
		case SV_SPECIALCARD :
			{	
				CSV_SPECIALCARD msg;
				msg.Get(lpData, nTotSize);		

				if( GetCurrentWhere() != IDX_GAMEWHERE_GAME)break;

				CString spec("");

				if(msg.pSC->nCard==CODE_SPECIAL_FCARD) spec = "☆포카드☆";
				else if(msg.pSC->nCard==CODE_SPECIAL_SF) spec = "☆스트레이트 플러쉬☆";
				else if(msg.pSC->nCard==CODE_SPECIAL_RSF) spec ="★로얄 스트레이트 플러쉬★";    


				CString str("");
				CString strNick = msg.pSC->NickName;
				//같은방에서 나왔는지 체크		

				if(msg.pSC->JackPotPlus>0)
				{
					CString strM = NumberToOrientalString(msg.pSC->JackPotPlus);
					strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
					str.Format(g_StrMan.Get(_T("MES_SPE2")),
						msg.pSC->ChanName, msg.pSC->nRoom, strNick, spec, strM);

					//하단 광고 
					// 				if (msg.pSC->nCard==1)
					// 					ChampClient().AddSpecialCard(strNick);

				}
				else
				{
					str.Format(g_StrMan.Get(_T("MES_SPE3")),
						msg.pSC->ChanName, msg.pSC->nRoom, strNick, spec);
				}

				AddGameInfoViewEdit( str, GetChatColor( ECC2_NOTIFY ) );


			}			
			break;


			//자리 지킴이	 패킷 처리
		case SV_SEATKEEPER:
			{
				CSV_SEATKEEPER msg;
				msg.Get( lpData, nTotSize );
				g_SeatKeeper.ProcessPacket(&msg);

			} break;

		case SV_CHANGE_MONEY:
			{				
				CSV_CHANGE_MONEY msg;
				msg.Get(lpData,nTotSize);

				g_GAME_BD()->Change_Money(&msg);

			}break;

		case SV_ACCEPT_OUTROOM: // 퇴실 요청 허가됨
			{
				CSV_ACCEPT_OUTROOM msg;
				msg.Get(lpData, nTotSize);			

				GetGameProcess()->DellBlindBetEffect();
				GM().SetWaitResponse(false);
				GM().OnExitRoom((REASON_OUTROOM)*msg.Reason);

				g_cPromotionMan.Accept_OutRoom();
			} break;

		case SV_CHANGEMYINFO: // 내 정보가 변경됨
			{
				CSV_CHANGEMYINFO msg;
				msg.Get(lpData, nTotSize);

				// ### [ 관전기능 ] ###
				memcpy( &GM().GetMyInfo()->UI.AllGameRecord, &msg.pCMI->AllGameRecord, sizeof(msg.pCMI->AllGameRecord) );
				memcpy( &g_PLAYER(0)->UI.AllGameRecord, &msg.pCMI->AllGameRecord, sizeof(msg.pCMI->AllGameRecord) );		
				/*GM().GetMyInfo()->UI.SetMoney(msg.pCMI->PMoney, IsTournament());*/
				GM().GetMyInfo()->UI.SetMoney(msg.pCMI->PMoney);
				/*if(GM().GetMyInfo()->UI.GetMoney(IsTournament()) < 0) GM().GetMyInfo()->UI.SetMoney(0, IsTournament());*/
				if(GM().GetMyInfo()->UI.GetMoney() < 0) GM().GetMyInfo()->UI.SetMoney(0);
				/*g_PLAYER(0)->UI.SetMoney(GM().GetMyInfo()->UI.GetMoney(IsTournament()), IsTournament());*/
				g_PLAYER(0)->UI.SetMoney(GM().GetMyInfo()->UI.GetMoney());
				/*g_PLAYER(0)->UI.nIcon = GM().GetMyInfo()->UI.nIcon = GetMoneyGrade( GM().GetMyInfo()->UI.GetTotalMoney(IsTournament()) );*/
				g_PLAYER(0)->UI.nIcon = GM().GetMyInfo()->UI.nIcon = GetMoneyGrade( GM().GetMyInfo()->UI.GetTotalMoney());

				//				g_pLobyDlg->UserInfoInvalidate();

			} break;

		case SV_USERENTERROOM:  // 새로운 플레이어가 입실하였음
			{
				CSV_USERENTERROOM msg;  
				msg.Get(lpData, nTotSize);

				//	if(g_Where != IDX_GAMEWHERE_GAME) break;
				// ### [ 관전기능 ] ###
				g_GAME_BD()->User_EnterRoom(&msg);
			} break;

		case SV_USEROUTROOM: // 사용자가 방에서 퇴장했음
			{
				CSV_USEROUTROOM msg;
				msg.Get(lpData, nTotSize);
				//	if(g_Where != IDX_GAMEWHERE_GAME) break;

				// ### [ 관전기능 ] ### 
				g_GAME_BD()->User_OutRoom(&msg);
			} break;

		case SV_CHANGEROOMMASTER: // 방장이 바뀌어야함
			{
				CSV_CHANGEROOMMASTER msg;
				msg.Get(lpData, nTotSize);

				if(strlen(msg.ID)==0) break;

				BOOL bBreak = TRUE;
				/*
				for(int i=0; i<g_Max_Player; i++)
				{
				if(strcmp(msg.ID, g_PLAYER(i)->UI.ID) == 0) { bBreak = FALSE; break; }
				}
				*/
				// 닉네임수정 	
				CString NickName="";
				for(int i=0; i<MAX_PLAYER; i++)
				{
					if(strcmp(msg.ID, g_PLAYER(i)->UI.ID) == 0) 
					{ 
						NickName = g_PLAYER(i)->UI.NickName;
						bBreak = FALSE; break; 
					}
				} 

				if(bBreak == TRUE) break;

				// 방 정보를 변경한다
				strcpy(g_RI.ID, msg.ID);
				// 닉네임수정 	
				strcpy(g_RI.NickName , NickName );		
				//여기까지
				g_RI.UNum = *msg.UNum;			
				g_RI.RoomMasterType = *msg.RoomMasterType;

				// ### [ 관전기능 ] ###
				if(strcmp(msg.ID, GM().GetMyInfo()->UI.ID) == 0) // 내가 방장이 된다
				{
					g_MastPN = 0;
					
				}
				else
				{
					int newMastPN = -1;
					for(int i=1; i<MAX_PLAYER; i++) 
						if(strcmp(g_PLAYER(i)->UI.ID, msg.ID)==0) {newMastPN=i; break;}

						if(newMastPN == -1) break;

						g_MastPN = newMastPN;				
				}

				CString str;
				str.Format("방장변경: [%s]님\n", NickName);
				AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

				str.Format("방장변경:[ID: %s(%s)]님\n",msg.ID , NickName );
				AddLobbyHideChatText( &str );


				g_cBanishMan.Clear();
			} break;

			//-----------------------(바둑이) (추가코드)-----------------------//	
			//아침 점심 저녁 적용 

		case SV_BD_NOTIFY_CARDEXCHANGE:
			{
				CSV_BD_NOTIFY_CARDEXCHANGE msg;
				msg.Get( lpData, nTotSize );

				//if(g_Where != WH_GAME) break;
				if( GetCurrentWhere() != IDX_GAMEWHERE_GAME)break;

				//if( IDX_GAME_BD != g_RI.nGameIndex )break;

				GameProcess_BD *pGPro =  (GameProcess_BD *)GetGameProcess();
				pGPro->OnNotifyCardExChange(&msg);		
				pGPro->DellBlindBetEffect();

			}break;

			//자리 지킴이	 패킷 처리
		case SV_BD_GIVETOUSER_NEWCHANGECARD:
			{
				CSV_BD_GIVETOUSER_NEWCHANGECARD msg;
				msg.Get( lpData , nTotSize );
				
				if( GetCurrentWhere() != IDX_GAMEWHERE_GAME)break;
				//if( IDX_GAME_BD != g_RI.nGameIndex )break;


#ifndef _DEBUG
				if(g_RI.State == 0) break;
#endif
				GameProcess_BD *pGPro =  (GameProcess_BD *)GetGameProcess();
				pGPro->OnReceiveUserCutting(&msg);

			}break;
		case NM_ADMIN_SETTINGMAN:
			{
				//관리자 챔피언쉽 세팅
				CNM_ADMIN_SETTINGMAN msg;
				msg.Get(lpData, nTotSize);

				if (g_bAdminUser==FALSE) break;

// 				if (g_pLobyDlg->m_pAdminSettingDlg)
// 				{
// 					g_pLobyDlg->m_pAdminSettingDlg->OnReceive(&msg);
// 				}

			} break;

			//규제안 : 손실금액에 따른 경고창 오픈
		case SV_DAILY_LOSS_MONEY: // 규제안 패킷.
			{
				if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
					break;

				CSV_DAILY_LOSS_MONEY msg;
				msg.Get(lpData,nTotSize);

				//각 게임에서 참조하도록 GM에 값을 설정
				GM().SetLossmoneyInfo(*msg.llLossMoney, *msg.tBlockTime, *msg.eState, *msg.idxGame) ;

				g_pGameView->m_cGameViewBtnMan.SetLossMoneyWarning() ;
			}
			break ;


			//규제안 : 대기하기 서버응답
		case SV_ASK_WAIT_INROOM:
			ReceiveSV_ASK_WAIT_INROOM(nTotSize, lpData);
			break ;


		case OPCODE_SV_JP_INFO :
			{
				if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
					break;

				packet::TSendPacket< jackpot::JP_MONEYINFO > JP_MONEY_Packet;
				JP_MONEY_Packet.Get( lpData, nTotSize );
				jackpot::JP_MONEYINFO* pAlaram = static_cast< jackpot::JP_MONEYINFO* >( JP_MONEY_Packet.Various() );

				g_GAME_BD()->JackPotMoneyUpdate(pAlaram);

// 				CSV_SEND_JACKPOTMONEYINFO msg;
// 				msg.Get(lpData, nTotSize);			
// 
// 				g_GAME_BD()->JackPotMoneyUpdate(msg.JackpotMoneyInfo);
			} break;
		case SV_RECEIVE_CONG_MSG:
			ReceiveSV_CongNoti(nTotSize, lpData);
			break;
		case SV_ETCCHIP_PLAYER_INFO:
			ReceiveSV_ETCCHIP_PLAYER_INFO(nTotSize, lpData);
			break;
		//case SV_PM_ODDEVEN_WINNER_NOTI:
		case SV_PM_WHEEL_WINNER_NOTI:
			ReceiveSV_PM_ODDEVEN_WINNER_NOTI(nTotSize, lpData);
			break;
		case SV_ASK_CARDOPEN_RESULT:
			{
				CSV_ASK_CARDOPEN_RESULT msg;
				msg.Get(lpData, nTotSize);

				bool bCardOpen = *msg.m_bCardOpen;

				// 본인 카드 오픈 버튼 상태 인덱스(0이면 Off, 1이면 On)
				int nBtnNum;
				CString csMsg;
				
				if (bCardOpen)
				{
					nBtnNum = 0;
					csMsg = "[서버 알림] 카드 OPEN을 사용합니다.\n";
				}
				else
				{
					nBtnNum = 1;
					csMsg = "[서버 알림] 카드 OPEN 사용을 취소 합니다.\n";
				}
				
				g_pGameView->m_cGameViewBtnMan.m_btnCardOpen.SetSprite(
					&(g_pGameView->m_cGameViewBtnMan.m_sprUICardOpenBtn), nBtnNum);
				
				// 인게임 채팅에 알림
				AddGameChatViewEdit(csMsg, RGB(199, 117, 116)) ;
			}
			break;
		default:
			{
				break;
			}

	}

	return CGame::OnPacketNotify( pName, nSignal, nTotSize, lpData );
}

void CGame_LowBD::OnSetCurrentGame(void)
{
	g_pChatWinMan->SetCurrnetChat();
}

CString CGame_LowBD::GetLobbyEnterSound(bool bMan)
{	
	if(bMan) return ".\\Lowbd\\sound\\enterlobby_m.wav";
	return ".\\Lowbd\\sound\\enterlobby_f.wav";
}

LRESULT CGame_LowBD::OnSetPlayRakeBackGold(WPARAM &wParam, LPARAM &lParam)
{
	GetGameView()->m_cPlayerDrawManager.SetPlayRakeBackGold((INT64)wParam, (int)lParam);
	return TRUE;
}

LRESULT CGame_LowBD::OnSetPlayRakeBackGoldEvent(WPARAM &wParam, LPARAM &lParam)
{
	GetGameView()->m_cPlayerDrawManager.SetPlayRakeBackGoldEvent((int)wParam);
	return TRUE;
}

LRESULT CGame_LowBD::OnClearJackPotMoney(WPARAM &wParam, LPARAM &lParam)
{
	int  nGameCode     = *((int *)wParam);
	INT64 llLimitMoney = *((INT64 *)lParam);	

	g_GAME_BD()->ClearJackpotMoney(nGameCode, llLimitMoney);

	return TRUE;
}

LRESULT CGame_LowBD::OnGetResultOpen( WPARAM &wParam, LPARAM &lParam )
{
	if (g_pGameView==NULL)
		return FALSE;

	return g_pGameView->m_cPlayerDrawManager.IsOpen();
	//return TRUE;
}

const HICON CGame_LowBD::GetGameIcon(void)
{
	return AfxGetApp()->LoadIcon(IDI_ICON_LOWBD);
}

const HICON CGame_LowBD::GetChatIcon(void)
{
	return AfxGetApp()->LoadIcon(IDI_ICON_CHAT_LOWBD);
}

const CString	CGame_LowBD::GetModuleName(void)
{
	char szFileName[MAX_PATH+1]={0,};
	GetModuleFileName(g_hInstGameDLL, szFileName, MAX_PATH);	
	return CString(szFileName);
}

LRESULT CGame_LowBD::OnSetExitReserve(WPARAM &wParam, LPARAM &lParam)
{
	if (GetGameProcess() != NULL)
		GetGameProcess()->DoExitSubscript(1);

	return TRUE;
}

//### [관리자 모드 작업] ###  방정보 변경
LONG CGame_LowBD::OnReceiveChangeStyleRoomInfo(int TotSize, char *lpData)
{
	// 현재 위치가 겜방인가, 대기실 인가
	CSV_CHANGESTYLEROOMINFO msg;
	msg.Get(lpData, TotSize);
	
	if(msg.SRI->nMaxUserNum < 100)
	{	
		strncpy_s(g_RI.Title, sizeof(g_RI.Title), msg.SRI->Title, 30);				
		
		CString str;

	#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
		if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
			str="톡톡 뉴포커";
		else
			str="윈조이 뉴포커";
	#else //UPGRADE_20120308_TOCTOC_CHANNELING
		str="윈조이 뉴포커";
	#endif //UPGRADE_20120308_TOCTOC_CHANNELING
		
		SetMainWindowText(str);

		g_pGameView->m_cTitle.MakeTitle();
		
		str.Format("방정보가 변경되었습니다.\n");				

		AddGameInfoViewEdit(str.GetString(),GetChatColor(ECC2_NOTIFY));
	}
	else// 방 정보 변경이 예약 되었다.
	{					
		CString str;
		str.Format("방정보 변경이 예약 되었습니다.\n");				
		AddGameInfoViewEdit(str.GetString(),GetChatColor(ECC2_NOTIFY));
	}				

	NMBASE::SOUND::g_Sound.PlayWav(SND_WHISPER);
	return TRUE;
} 

//규제안 : 대기하기 기능 추가
void CGame_LowBD::ReceiveSV_ASK_WAIT_INROOM(int TotSize, char *lpData)
{	
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	CSV_WAIT_INROOM msg;
	msg.Get(lpData, TotSize);
	g_pGameView->m_cGameViewBtnMan.SetWaitFlag(*msg.m_bWait);
}

void CGame_LowBD::ReceiveSV_CongNoti(int TotSize, char *lpData)
{	

	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	CSV_RECEIVE_CONG_MSG msg;
	msg.Get(lpData, TotSize);

	CString strID;
	strID.Format("보낸 사람 : %s님", msg.m_Send_ID);
	CString strChat;
	strChat.Format("%s", msg.StrChat);
	g_pGameView->m_cPlayerDrawManager.SetPlayCongNoti(strID.GetString(), strChat.GetString(), *msg.m_ShowTime);
}

void CGame_LowBD::ReceiveSV_ETCCHIP_PLAYER_INFO(int TotSize, char *lpData)
{	
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	CSV_ETCCHIP_PLAYER_INFO msg;
	msg.Get(lpData, TotSize);

	if(strcmp(msg.m_ChipType, "GD")==0)
	{	
		int pnum = *msg.m_pnum;
		if(pnum >= MAX_PLAYER && g_RI.ChipKind == 1) 
		{
			// 관전자 리스트 출력
			int ounum   = g_PLAYER(pnum)->UI.UNum;
			int sex     = g_PLAYER(pnum)->UI.Sex;
			int level   = g_PLAYER(pnum)->UI.nIcon;
			INT64 Money = *msg.m_RealChip;
			int seq		= 0;

			CObserverDlg *pObserverDlg = GetObserverDlg();
			if(NULL != pObserverDlg)
			{
				pObserverDlg->User_DelItem(ounum, g_PLAYER(pnum)->UI.ID);
				pObserverDlg->User_InsItem(ounum, g_PLAYER(pnum)->UI.ID, g_PLAYER(pnum)->UI.NickName, sex, level, Money, seq);
			}			
		}
		else
		{
			g_PLAYER(g_GAME_BD()->GetPNum_ByServPN(pnum))->SetGold(*msg.m_RealChip);
		}
	}
}

void CGame_LowBD::ReceiveSV_PM_ODDEVEN_WINNER_NOTI(int TotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	//CSV_PM_ODDEVEN_WINNER_NOTI msg;
	CSV_PM_WHEEL_WINNER_NOTI msg;
	msg.Get(lpData, TotSize);

	CString strID;
	strID.Format("%s", msg.m_UserID);
	g_pGameView->m_cPlayerDrawManager.SetPlayGoldNoti(strID.GetString(), *msg.m_RewardMoney, timeGetTime());
}