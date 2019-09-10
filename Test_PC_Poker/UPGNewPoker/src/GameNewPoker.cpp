#include "StdAfx.h"
#include "GameNewPoker.h"
#include "GameView.h"
#include <CommMsgDef_Game.h>
#include "./AniListMan/AniFileListMan.h"
#include "Resource.h"

CGameNewPoker::CGameNewPoker()
{
}

CGameNewPoker::~CGameNewPoker(void)
{
}

void CGameNewPoker::LoadSprite()
{
	CGame::LoadSprite();

	AddLoadImageList(&GLOBAL_SPR[ spr::GSPR_ET_MADE ], ".\\PokerCommon\\data\\complete_made.spr",true);
	AddLoadImageList(&GLOBAL_SPR[ spr::GSPR_V_H_OPEN ], ".\\newpoker\\data\\HiddenOpen.spr",true);

	AddLoadImageList(&g_sprBombSetAni,".\\newpoker\\data\\Effect\\bombsetani.spr",true);
	// �¸� win
	AddLoadImageList(&g_sprJokboEffect,".\\newpoker\\data\\resulteft\\JokboEffect.spr",true);

	AddLoadImageList(&g_sprBodyItem,".\\newpoker\\data\\BodyItem.spr",true);
	AddLoadImageList(&g_sprAllInEft,".\\newpoker\\data\\Effect\\OtherEffect.spr",true);
	AddLoadImageList(&g_sprBetCmd,".\\PokerCommon\\data\\BettingEffect.spr",true);
	// �ڵ� ����ī�� ����
	AddLoadImageList(&g_sprGrade,".\\newpoker\\data\\grade.spr",true);
	AddLoadImageList(&g_sprAutoBettingSmall,".\\newpoker\\data\\AutoBetSetting_small.spr",true);
	AddLoadImageList(&g_sprAutoBetting,".\\newpoker\\data\\AutoBetSetting.spr",true);
	AddLoadImageList(&g_sprAutoBettingInfoMsg,".\\PokerCommon\\data\\AutobetPanel.spr",true);

	AddLoadImageList(&g_sprBigCard,".\\newpoker\\data\\card\\BigCard.spr",true);
	AddLoadImageList(&g_sprSmallBombCard,".\\newpoker\\data\\card\\MiniCard.spr",true);
	AddLoadImageList(&g_sprDarkBigCard,".\\newpoker\\data\\card\\DarkBigCard.spr",true);
	AddLoadImageList(&g_sprDisCard,".\\newpoker\\data\\card\\discard.spr",true);

	AddLoadImageList(&g_sprLastCardOpen,".\\newpoker\\data\\card\\LastCardOpen.spr",true);

	AddLoadImageList(&g_sprJokerCardEffect,".\\newpoker\\data\\Effect\\JokerEft.spr",true);
	AddLoadImageList(&g_sprSDLG_ProFlie,".\\newpoker\\data\\sdlgfile\\proflie\\profile.spr",true);	
	AddLoadImageList(&g_sprAllinChargeInfo,".\\newpoker\\data\\AllinAngel.spr",true);	

	AddLoadImageList(&g_sprChip,".\\PokerCommon\\data\\Chip.spr",true);

	AddLoadImageList(&g_sprWinLoseBack,".\\newpoker\\data\\Skin\\winlose.spr",true);
	AddLoadImageList(&g_sprBigBombCard,".\\newpoker\\data\\card\\BigBombCard.spr", true);
	AddLoadImageList(&g_sprCardDealing,".\\newpoker\\data\\card\\CardDealing.spr", true);
	AddLoadImageList(&g_sprBombCardDealing,".\\newpoker\\data\\card\\BombCardDealing.spr", true);
	AddLoadImageList(&g_sprClose,".\\PokerCommon\\data\\sp_closeBtn.spr", true);

	AddLoadImageList(&g_ChanceEffectSpr, ".\\newpoker\\data\\Chance.spr", true);

	g_pGameView->m_cGameHelper.LoadImage(".\\newpoker\\data\\gamehelp\\gh_btn.spr", ".\\newpoker\\data\\gamehelp\\doumi.spr", ".\\newpoker\\data\\Skin\\JbTable.spr", ".\\PokerCommon\\data\\jokboinfo.spr");
	g_pChatWinMan->LoadImage(".\\newpoker\\data\\Skin\\ChatBox.spr");
	GAME()->m_cSelectCardMan.LoadImage();

	g_pGameView->m_cSidePotMoney.LoadImage(".\\NewPoker\\data\\side_betting_displayboard.spr");
	g_pGameView->m_cGameViewBtnMan.LoadImage( ".\\NewPoker\\data\\Skin\\BetBtn_N.spr", ".\\NewPoker\\data\\Skin\\BetBtn_H.spr", ".\\NewPoker\\data\\Skin\\BetBtn_P.spr" );
	g_pGameView->m_cPlayerDrawManager.LoadImage(NULL, &g_sprBetCmd, &g_sprWinLoseBack, ".\\NewPoker\\data\\AllinEffect.spr");
	g_pGameView->m_cTotalBetMoney.LoadImage();

#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		g_pGameView->m_cBackGround.LoadImage(".\\newpoker\\data\\Skin\\Gameback_toctoc.spr");
	else
		g_pGameView->m_cBackGround.LoadImage(".\\newpoker\\data\\Skin\\Gameback.spr");
#else //UPGRADE_20120308_TOCTOC_CHANNELING
	g_pGameView->m_cBackGround.LoadImage(".\\newpoker\\data\\Skin\\Gameback.spr");
#endif //UPGRADE_20120308_TOCTOC_CHANNELING

	g_pGameView->m_cTitle.LoadImage(IDX_GAME_NP);
	GAME()->m_cAllBetChip.LoadImage();

	g_ObserverMan.LoadImage();
	g_SeatKeeper.LoadImage();
	g_cOneShotCharge.LoadImage();
	g_cOwnLimit.LoadImage(".\\newpoker\\data\\effect\\HandoEffect.spr");
	ReLoadSprite(1, true);
}

bool CGameNewPoker::ReLoadSprite(int nSkinKind, bool bFirstLoad/* = false*/)
{
	if (!CGame::ReLoadSprite(nSkinKind, bFirstLoad)) return false;

	string str;
	str = ".\\PokerCommon\\data\\";
	
	_LOADSPRITEDEFINE(g_sprJackPot ,str,"Jackpot.spr", bFirstLoad);

	str = ".\\newpoker\\data\\Skin\\";

	// ��� ����
	_LOADSPRITEDEFINE(g_sprResultNumber ,str,"resultnumber.spr", bFirstLoad);

	// ���� ���̺�
	_LOADSPRITEDEFINE(g_sprJBTable ,str,"JbTable.spr", bFirstLoad);

	// ä��,���� ��ư
	_LOADSPRITEDEFINE(g_sprTabBtn ,str,"TabBtn.spr", bFirstLoad);

	return true;
}

void CGameNewPoker::OnFocusGame(bool bFocus)
{
	if (bFocus)
	{
		g_pChatWinMan->SetCurrnetChat();
		g_cOwnLimit.Init( &g_cUIData, GAME());
		g_cBanishMan.Init(&g_SeatKeeper, &g_ObserverMan);
		g_SeatKeeper.SetChangeThisPoint();

		MM().AddMsgH(UPG_CMK_SET_LIMIT_MONEY, Fnt(this, &CGameNewPoker::OnSetLimitMoney));
		MM().AddMsgH(UPG_CMK_SET_LIMIT_GOLD,  Fnt(this, &CGameNewPoker::OnSetLimitGold));
		MM().AddMsgH(UPG_CMK_GET_MONEY_GRADE, Fnt(this, &CGameNewPoker::OnGetMoneyGrade));
		MM().AddMsgH(UPG_CMK_GET_JOKBO_NAME, Fnt(this, &CGameNewPoker::OnGetJokboName));
		MM().AddMsgH(UPG_CMK_GET_MYJOKBO_NAME, Fnt(this, &CGameNewPoker::OnGetMyJokboName));		
		MM().AddMsgH(UPG_CMK_GET_RESULT_MONEY, Fnt(this, &CGameNewPoker::OnGetResultMoney));
		MM().AddMsgH(UPG_CMK_CLEAR_JACKPOTMONEY, Fnt(this, &CGameNewPoker::OnClearJackPotMoney));
		MM().AddMsgH(UPG_CMK_SET_PLAY_RAKEBACK_GOLD, Fnt(this, &CGameNewPoker::OnSetPlayRakeBackGold));
		MM().AddMsgH(UPG_CMK_SET_PLAY_RAKEBACK_GOLD_EVENT, Fnt(this, &CGameNewPoker::OnSetPlayRakeBackGoldEvent));				

		MM().AddMsgH(UPG_CMK_GAME_RESULT_OPEN, Fnt(this, &CGameNewPoker::OnGetResultOpen));				

		g_cPromotionMan.Reset();
		g_cPromotionMan.Init(&g_cUIData, this, &g_pGameView->m_cChatWindow);
		g_cPromotionMan.MakePromotion(CPromotionManager::EPT_JOKBOJACKPOT);		// ���������� ������ Ÿ�̹��� ���� �̸������ ���´�.
		g_cVipJackpot.Init(g_pGameView);
	}
	else
	{
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

LRESULT CGameNewPoker::OnGetResultMoney(WPARAM &wParam, LPARAM &lParam)
{
	INT64 *pMoney = (INT64 *)lParam;
	CPlayer_NP *pPlayer = GAME()->GetPlayerNP((int)wParam);
	*pMoney = pPlayer->m_i64ResultMoney;
	return TRUE;
}

LRESULT CGameNewPoker::OnGetJokboName(WPARAM &wParam, LPARAM &lParam)
{
	CString *pStrJokbo = (CString *)lParam;
	*pStrJokbo = GAME()->GetJokboName((int)wParam);
	return TRUE;
}

LRESULT CGameNewPoker::OnGetMyJokboName(WPARAM &wParam, LPARAM &lParam)
{
	CString *pStrJokbo = (CString *)lParam;
	*pStrJokbo = GAME()->GetMyJokboName();
	return TRUE;
}

LRESULT CGameNewPoker::OnGetMoneyGrade(WPARAM &wParam, LPARAM &lParam)
{
	return GetMoneyGrade(*((INT64 *)lParam));
}

LRESULT CGameNewPoker::OnSetLimitMoney(WPARAM &wParam, LPARAM &lParam)
{
	//�����ѵ�
	g_cOwnLimit.SetMaxPremiumMoneyWithOption(*((INT64 *)lParam));
	return TRUE;
}

LRESULT CGameNewPoker::OnSetLimitGold(WPARAM &wParam, LPARAM &lParam)
{
	//�����ѵ�
	g_cOwnLimit.SetMaxPremiumGoldWithOption(*((INT64 *)lParam));
	return TRUE;
}

CGameProcess_NewPoker* CGameNewPoker::GetGameProcess()
{
	return (CGameProcess_NewPoker*) m_pGameProcess;
}

CGameViewNewPoker* CGameNewPoker::GetGameView()
{
	return (CGameViewNewPoker*) m_pGameViewBase;
}

void CGameNewPoker::OnLoadingEnd(void)
{
	GetGameProcess()->OnInit();
	g_pGameView->Init();
}

void CGameNewPoker::LoadSound()
{
	CGame::LoadSound();

	RegistSoundFiles();
}

void CGameNewPoker::ReleaseSprite()
{
	CGame::ReleaseSprite();
	GM().RemoveLoadSprite();
}

void CGameNewPoker::ReleaseSound()
{
	CGame::ReleaseSound();
}

const IDX_GAME CGameNewPoker::GetGameType(void)
{
	return IDX_GAME_NP;
}

const int CGameNewPoker::GetMaxRoom(void)
{
	return 70;
}

const int CGameNewPoker::GetMaxPlayer(void)
{
	return MAX_PLAYER;
}

const int CGameNewPoker::GetMaxGamePlayer(void)
{
	return MAX_PLAYER;
}

const int CGameNewPoker::GetMaxObserver(void)
{
	return MAX_OBSERVER;
}

const int CGameNewPoker::GetJokboQuestCount(void)
{
	return 6;
}

const CString CGameNewPoker::GetGameString(void)
{
	return "newpoker";
}

const int CGameNewPoker::GetGameCode(void)
{
	return GAME_CODE_NP;
}

const CString CGameNewPoker::GetGameName(void)
{
	return "����Ŀ";
}

const COLORREF	CGameNewPoker::GetGameColor(void)
{	
	return RGB(60,97,124);//�� ���� ������ ���� ��ȯ
}

const CString CGameNewPoker::GetLoadingBackPath(void)
{
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		return ".\\NewPoker\\image\\LoadingBack_toctoc.bmp";
	else
		return ".\\NewPoker\\image\\LoadingBack.bmp";	
#else //UPGRADE_20120308_TOCTOC_CHANNELING
	return ".\\NewPoker\\image\\LoadingBack.bmp";
#endif //UPGRADE_20120308_TOCTOC_CHANNELING
}

const int CGameNewPoker::GetDefaultFormKind(void)
{
	return FORMKIND_BOMB;


}
void CGameNewPoker::OnChangeFullScreen(bool bFullScreen) 
{
	g_pGameView->m_cGameViewBtnMan.SetChangeDisplayButton(bFullScreen);

	// ����� ȭ����� ��ġ�ϴ� DIB ������ ����(�ӵ� ���)
	g_pGameView->Page.ReCreateDIBSection();
}

void CGameNewPoker::OnCloseOptionDlg(LRESULT nRet)
{
	if (nRet == IDOK) g_pGameView->m_cGameViewBtnMan.SetBetBtnType(g_Config.eBetBtnType);
}

BOOL CGameNewPoker::Create(CWnd *pParentWnd)
{
	m_pGameProcess = new CGameProcess_NewPoker(this);
	m_pGameViewBase = new CGameViewNewPoker(this);

	CRect rect(0, 0, 1024, 768);
	m_pGameViewBase->Create(NULL, "GameViewNewPoker", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rect, pParentWnd, IDX_GAME_NP);
	m_pGameViewBase->ShowWindow(SW_HIDE);

	return TRUE;
}

LRESULT CGameNewPoker::ShowCreateRooomDlg(ROOMINFO *pRoomInfo, ROOMOPTION *pWebRoomOption)
{
	CCreateRoomDlg dlg(	GM().GetCurrentGameType(), GM().GetMainWnd());
	dlg.SetWebSelectedRoomOption(pWebRoomOption);
	int nRet = dlg.DoModal();

	if (nRet == IDOK)
	{
		strncpy_s(pRoomInfo->Title, 32, (LPCTSTR)dlg.m_strRoomTitle, dlg.m_strRoomTitle.GetLength());
		pRoomInfo->bSuperMaster			= PBIsSuperRoom();
		pRoomInfo->ChipKind				= dlg.m_nChipKind;
		pRoomInfo->FormKind				= dlg.m_nGameMode;
		pRoomInfo->bIsFriendRoom		= dlg.m_bFriend;
		pRoomInfo->nAutoBetKind			= dlg.m_sAutoBet;
		pRoomInfo->BetMoney				= dlg.m_i64SeedMoney;
		pRoomInfo->llEnterLimitMoney	= dlg.m_i64MinimumMoney;
		//pRoomInfo->bVIPEnterLimitYN		= dlg.m_bVIPEnterLimitYN;
		pRoomInfo->sBetRuleKind			= dlg.m_sBetRuleKind;

		// ������ : ī�弼�� �߰�.
		pRoomInfo->cCardSetting =	dlg.cCardSetting;

		//�ƽ���
		pRoomInfo->bIsMaxRoom			= dlg.m_bIsMaxRoom;
		pRoomInfo->llInMoney			= dlg.m_i64InMoney;

		//���չ�
		pRoomInfo->cafeID_				= dlg.m_bUnionMode? GM().GetMyInfo()->UI.unitedCafeInfo_.cafeID_ : 0;
		//��ħ����
		pRoomInfo->bMorningBet			= dlg.m_bBetRule;

		//3�� 5�� �÷��̸� ���� �߰� �ڵ�
		pRoomInfo->MaxUserNum		= dlg.m_nMaxUserNum;
		pRoomInfo->cMemberRoom		= 0;
	}

	return nRet;
}

LRESULT CGameNewPoker::ShowQuickJoinerDlg(void)
{
	CQuickJoinDlg dlg(EQJ_NEWPOKER, &g_cUIData, GM().GetMainWnd());
	LRESULT nRet = dlg.DoModal();

	if (nRet == IDOK)
	{		
		GM().SendQuickJoiner();
	}

	return FALSE;
}

LRESULT	CGameNewPoker::SendQuickJoiner(void)
{
	if ( OnMacroCheck() )
	{
		return FALSE;
	}

	//NMBASE::SOUND::g_Sound.PlayWav(SND45);

	//bool bQJEnable = CQuickJoinDlg::IsEnableQJ(EQJ_NEWPOKER);

	//if (!bQJEnable)
	//{
	//	// �������� - �Ϲ����� �� ����
	//	GM().AskDirectEnterRoom(false, NULL, -1);
	//	return TRUE;
	//}

	// ���� ������ �۾�
	// �ٷ� ������ �κ񿡼� ȣ���ϸ�, ������ �������� ����.
	GM().AskDirectEnterRoom(false, NULL, GetDefaultFormKind());

	// ���� �� ����ϴ� �κ��� �ϴ� ������.
	//SCONDITION sInfo; 
	//CString strSection;
	//strSection.Format("QJ%02d_%s_%d", QJ_VER, GM().GetMyInfo()->UI.ID, EQJ_NEWPOKER);

	//if (CQuickJoinDlg::GetSearchCondition(EQJ_NEWPOKER, strSection, &sInfo))
	//{
	//	CDataRoomOptionList *pDataRoomOptionList = GM().GetDataRoomOptionList();
	//	if (pDataRoomOptionList == NULL) return FALSE;

	//	//ROOMOPTION *pResultRoomOption = pDataRoomOptionList->GetQJRoomOption( sInfo.m_bUseBetRule, sInfo.m_bAutoBet, sInfo.m_bUseSeedMoney, sInfo.m_nSeedType, sInfo.m_sBetRuleKind );
	//	ROOMOPTION *pResultRoomOption = pDataRoomOptionList->GetQJRoomOption(sInfo.m_bUseGameMode, sInfo.m_nGameMode, sInfo.m_bUseSeedMoney, sInfo.m_nSeedType) ; 

	//	if (pResultRoomOption == NULL)
	//	{
	//		if (sInfo.m_bAutoEnterRoom)
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
	//				//�ٽ� ����
	//				ShowQuickJoinerDlg();				
	//			}
	//			else if ( IDCANCEL == idResult )
	//			{
	//				//�游���
	//				GM().ShowCreateRooomDlg();			
	//			}

	//			return TRUE;
	//		}
	//	}

	//	int nGameMode = GetDefaultFormKind();
	//	if (sInfo.m_bUseGameMode) nGameMode = sInfo.m_nGameMode;

	//	GM().AskDirectEnterRoom(true, pResultRoomOption, nGameMode, ECALL_BTNTYPE_LOBY_QUICK, true, sInfo.m_bAutoEnterRoom);
	//}
	//else
	//{
	//	// �Ϲ����� �� ����
	//	GM().AskDirectEnterRoom(false, NULL, -1);
	//}

	return TRUE;
}

int CGameNewPoker::GetDirectEnterRoomType(int nType)
{
	return BOMB_GAME;
}

void CGameNewPoker::CheckDirectEnterRoomInfo(int nType, ROOMINFO *pRoomInfo)
{
	CString strTitle;
	MM().Call(UPG_CMK_GENERATE_RANDOM_ROOM_NAME, 0, (LPARAM)(&strTitle));
	strncpy_s(pRoomInfo->Title, 32, strTitle, strTitle.GetLength());
	pRoomInfo->bSuperMaster = PBIsSuperRoom();
}

CPlayer *CGameNewPoker::CreateNewPlayer(void)
{
	g_pMyInfo_NP = new CPlayer_NP;
	return g_pMyInfo_NP;
}

void CGameNewPoker::AddGameChat(const char *str, COLORREF crNewColor, bool bUserChat)
{
	g_pChatWinMan->AddChat(str, crNewColor, bUserChat);
}

void CGameNewPoker::AddGameInfo(const char *str, COLORREF crNewColor, bool bUserChat)
{
	g_pChatWinMan->AddInfo(str, crNewColor, bUserChat);
}

//���� ����â�� ���ӹ�/���� �����ϰ� �����ʿ� �������� ������ �����ʿ��� ����.
LONG CGameNewPoker::OnReceiveUserInfo(int TotSize, char *lpData)
{
	CSV_USERINFO msg;
	msg.Get(lpData, TotSize);
	
	GM().SetWaitResponse(false);


	if ( GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return FALSE;


	// ���̵� ���̰� 0�̸� ���� ��ܿ� ���� �������
	if (strlen(msg.UI->ID) == 0) return FALSE;

	static BOOL bProccessed = FALSE;
	if (bProccessed != FALSE) return FALSE;
	bProccessed = TRUE;
	ShowProfileDialog(msg.UI, NULL);
	bProccessed = FALSE;
	return TRUE;
}

//���۹�ư Ȱ��ȭ
LONG CGameNewPoker::OnReceiveStartButtonEnable(int TotSize, char *lpData)
{
	CSV_STARTBTNENABLE msg;
	msg.Get(lpData, TotSize);			
	
	g_pGameView->m_cGameViewBtnMan.m_btnGameStart.Show(FALSE);

	if (g_RI.State != 0) return FALSE;

// #ifndef _DEBUG
// 	if ( g_RI.bPlayer == 2 ){
// 		if (g_RI.NowUserNum < 2 ) return FALSE;
// 	}
// 	else{
// 		if (g_RI.NowUserNum < 3 ) return FALSE;
// 	}
// #endif	

	if (strncmp(msg.ID, GM().GetMyInfo()->UI.ID, 15)==0) 
	{
		//���۹�ư Ȱ��ȭ				
		g_pGameView->m_cGameViewBtnMan.m_btnGameStart.Show(TRUE);
		
		// �޼���
		CString str("���۹�ư�� ������ ������ ���۵˴ϴ�.\n");
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
	}

	return TRUE;
}

//���� ,���� ����
LONG CGameNewPoker::OnReceiveObserverMessege(int TotSize, char *lpData)
{
	CSV_OBSERVER_MESSAGE msg;
	msg.Get(lpData, TotSize);
	CString chat;
	chat.Append((LPCTSTR)msg.StrChat, *msg.l_StrChat);
	chat += "\n";
	AddGameInfoViewEdit(chat, GetChatColor(ECC2_SERVMSG));

	int num = *msg.Kind;

	g_pGameView->m_cGameViewBtnMan.SetObserverBtn(num);
	if (num >= 1 && num < 100)
	{
	}
	else if (num == 100)
	{
		g_pGameView->m_cPlayerDrawManager.SetObserverReservation(0, true);
	}
	else if (num == 200)
	{
		g_pGameView->m_cPlayerDrawManager.SetObserverReservation(0, false);
	}
	else if (num == 300)
	{
		g_ObserverMan.OnObserve();
	}

	return TRUE;
}

// ������ ����
LONG CGameNewPoker::OnReceiveReservationInfo(int TotSize, char *lpData)
{
	CSV_RESERVATIONINFO msg;
	msg.Get(lpData, TotSize);

	CObserverDlg *pObserverDlg = GetObserverDlg();
	pObserverDlg->User_ResetReservation();
	
	for (int i=0; i<*msg.TotNum; i++) 
	{
		char *id = msg.ReservedInfo[i].ID;
		char *NickName = msg.ReservedInfo[i].NickName;
		int unum = msg.ReservedInfo[i].UNum;
		int seq =  msg.ReservedInfo[i].Seq;

		pObserverDlg->User_SetItemReservation(unum, id, seq);
	}

	//pObserverDlg->Invalidate(FALSE); 
	
	//����UI
	g_ObserverMan.OnReceiveReservationInfo(&msg);
	return TRUE;
}

//����->����
LONG CGameNewPoker::OnReceiveAcceptEmerage(int TotSize, char *lpData)
{
	CSV_ACCEPT_EMERAGE msg;
	msg.Get(lpData, TotSize);

	int nowpnum = *msg.NowPNum;
	int newpnum = *msg.NewPNum;

	int MyID = -1;
	int i=0;

	const int nMaxPlayer = GAME()->GetMaxPlayer();
	const int nMaxNewPlayer = GAME()->GetMaxNewPlayer();

	CPlayer_NP *pPlayer = NULL;
	for (i=0; i<nMaxNewPlayer; i++)	{
		pPlayer = GAME()->GetPlayerNP(i);
		if (strlen(pPlayer->UI.ID) > 0) {	
			if (strcmp(GM().GetMyInfo()->UI.ID, pPlayer->UI.ID)==0) { MyID = i; break; }
		}
	}

	BOOL bTrue = TRUE;
	if (newpnum < nMaxPlayer || newpnum >= nMaxNewPlayer) bTrue = FALSE;
	
	pPlayer = GAME()->GetPlayerNP(0);
	if (MyID == -1 || nowpnum != pPlayer->ServPNum || !bTrue) 
	{
		CSV_ASK_OUTROOM aormsg;
		aormsg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, REASON_OUTROOM_ABNORMAL);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), aormsg.pData, aormsg.GetTotalSize());
		return TRUE;
	}

	g_pGameView->m_cGameViewBtnMan.InitObserverBtn();
	// �÷��̾� ������ �°� �ʱ�ȭ�Ѵ�			
	GAME()->SetPlayInfo(nowpnum, newpnum, FALSE);
	
	// ### [ �ߺ��� IP�� ] ###
	for (i=0; i<nMaxPlayer; i++) {
		int pn = GAME()->GetPNum_ByServPN(i);
		pPlayer = GAME()->GetPlayerNP(pn);
		if (strlen(pPlayer->UI.ID) > 0)	
		{
			pPlayer->MyIpCheck = msg.IPA->IpIndex[i];
		}
	}
	return TRUE;
}

//����->����
LONG CGameNewPoker::OnReceiveAcceptParticipation(int TotSize, char *lpData)
{
	CSV_ACCEPT_PARTICIPATION msg;
	msg.Get(lpData, TotSize);

	int nowpnum = *msg.NowPNum;
	int newpnum = *msg.NewPNum;

	const int nMaxPlayer = GAME()->GetMaxPlayer();
	const int nMaxNewPlayer = GAME()->GetMaxNewPlayer();

	int MyID = -1;
	int i=0;
	CPlayer_NP *pPlayer = NULL;
	for (i=0; i<nMaxNewPlayer; i++)	{
		pPlayer = GAME()->GetPlayerNP(i);
		if (strlen(pPlayer->UI.ID) > 0) 
		{	
			if (strcmp(GM().GetMyInfo()->UI.ID, pPlayer->UI.ID)==0) { MyID = i; break; }
		}
	}

	BOOL bTrue = TRUE;
	if (newpnum < 0 || newpnum >= nMaxPlayer) bTrue = FALSE;
	if (MyID == -1 || nowpnum != MyID || !bTrue) 
	{
		CSV_ASK_OUTROOM aormsg;
		aormsg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, REASON_OUTROOM_ABNORMAL);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), aormsg.pData, aormsg.GetTotalSize());
		return TRUE;
	}

	// ### [���� �߰� �۾�] ###
	pPlayer = GAME()->GetPlayerNP(MyID);
	pPlayer->nSndFxKind = *msg.SndFxKind;
	g_pGameView->m_cGameViewBtnMan.InitObserverBtn();
	// �÷��̾� ������ �°� �ʱ�ȭ�Ѵ�			
	GAME()->SetPlayInfo(nowpnum, newpnum, TRUE);
	
	// ### [ �ߺ��� IP�� ] ###
	for (i=0; i<nMaxPlayer; i++) {
		int pn = GAME()->GetPNum_ByServPN(i);
		pPlayer = GAME()->GetPlayerNP(pn);
		if (strlen(pPlayer->UI.ID) > 0) {
			pPlayer->MyIpCheck = msg.IPA->IpIndex[i];
		}
	}

	//�ڸ� ��Ŵ��
	g_SeatKeeper.OnEnterGame(); // ���� -> ������ �϶� ȣ�� 

	//�����ѵ�
	g_cOwnLimit.OnEnterRoom(&g_cUIData, GAME());	//�����ѵ�
	return TRUE;	
}

//������ -> ����
LONG CGameNewPoker::OnReceiveEnterEmerage(int TotSize, char *lpData)
{
	CSV_ENTER_EMERAGE msg;
	msg.Get(lpData, TotSize);
	
	int nowpnum = *msg.NowPNum;
	int newpnum = *msg.NewPNum;

	const int nMaxPlayer = GAME()->GetMaxPlayer();
	const int nMaxNewPlayer = GAME()->GetMaxNewPlayer();

	if (nowpnum < 0 || nowpnum >= nMaxPlayer) return FALSE;
	if (newpnum < nMaxPlayer || newpnum >= nMaxNewPlayer) return FALSE;

	int pn = GAME()->GetPNum_ByServPN(nowpnum);
	CPlayer_NP *pPlayer = GAME()->GetPlayerNP(pn);
	if (strcmp( pPlayer->UI.ID, msg.ID) != 0) return FALSE;

	CPlayer_NP *pPlayerNew = GAME()->GetPlayerNP(newpnum);
	pPlayerNew->SetNewPlayer(&(pPlayer->UI));
	pPlayerNew->UI.cObserver = 1;
	pPlayerNew->nSndFxKind = pPlayer->nSndFxKind;

	pPlayer->Clear();

	g_RI.NowUserNum -= 1;
	g_RI.NowObserverNum += 1;
	
	CString str;
	str.Format("## <%s>���� ������ ���� �մϴ� ##\n", pPlayerNew->UI.NickName);			
	AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

	CString str1;
	str1.Format( "## ID: %s(%s)���� ������ ���� �մϴ� ##\n", pPlayerNew->UI.ID, pPlayerNew->UI.NickName );
	AddLobbyHideChatText( &str1 );

	// ### [ �ߺ��� IP�� ] ###
	for (int i=0; i<nMaxPlayer; i++)	{
		int pn = GAME()->GetPNum_ByServPN(i);
		pPlayer = GAME()->GetPlayerNP(pn);
		if (strlen(pPlayer->UI.ID) > 0)	{
			pPlayer->MyIpCheck = msg.IPA->IpIndex[i];
		}
	}

	int ounum = pPlayerNew->UI.UNum;
	int sex  = pPlayerNew->UI.Sex;
	int level = pPlayerNew->UI.nIcon;
	INT64 Money = pPlayerNew->UI.GetMoney();
	int seq = 0;
	CObserverDlg *pObserverDlg = GetObserverDlg();
	pObserverDlg->User_InsItem(ounum,pPlayerNew->UI.ID, pPlayerNew->UI.NickName, sex, level, Money ,seq);
	return TRUE;
}

//���� -> ������
LONG CGameNewPoker::OnReceiveEnterParticipation(int TotSize, char *lpData)
{
	CSV_ENTER_PARTICIPATION msg;
	msg.Get(lpData, TotSize);

	const int nMaxPlayer = GAME()->GetMaxPlayer();
	const int nMaxNewPlayer = GAME()->GetMaxNewPlayer();

	int nowpnum = *msg.NowPNum;
	int newpnum = *msg.NewPNum;
	if (newpnum < 0 || newpnum >= nMaxPlayer) return FALSE;
	if (nowpnum < nMaxPlayer || nowpnum >= nMaxNewPlayer) return FALSE;

	//������ pnum �ε����� ������ ��ġ�Ѵ�
	CPlayer_NP *pPlayer = GAME()->GetPlayerNP(nowpnum);
	if (strcmp( pPlayer->UI.ID, msg.ID) != 0) return FALSE; 
	int i=0;
	int pnum = -1; // �÷��̾� ��ȣ
	if ( !g_ObserverMan.IsMyObserver()) // 1) ���� ���������̰� ������ �� ����
	{
		for (i=GAME()->m_nNo1PosPNum; i < nMaxPlayer + GAME()->m_nNo1PosPNum; i++) 
		{
			int index = i;
			if (index>=nMaxPlayer) index = index - nMaxPlayer;
			if (index==0) continue; //��
			pPlayer = GAME()->GetPlayerNP(index);
			if (strlen(pPlayer->UI.ID)==0){pnum=index; break;}
		}
	
		if (pnum == -1) return FALSE; // ����ִ� ���� ã�� ��������
		
		pPlayer = GAME()->GetPlayerNP(nowpnum);
		pPlayer->UI.cObserver = 0;
		CPlayer_NP *pPlayerNew = GAME()->GetPlayerNP(pnum);
		pPlayerNew->SetNewPlayer(&pPlayer->UI);
		pPlayerNew->SetChangeRoomInfo(*pPlayer->GetChangeRoomInfo());
		g_pGameView->m_cPlayerDrawManager.Reset(pnum);		
		pPlayer->Clear();
		g_pGameView->m_cPlayerDrawManager.SetShow(pnum, true);	// �� ���� �����ؾ� ������ ȭ�鿡 ���δ�.
		g_RI.NowUserNum += 1;
		g_RI.NowObserverNum -= 1;
		
		pPlayerNew->ServPNum = GAME()->GetServPNum_ByPN(pnum);
#ifdef _DEBUG
		if (newpnum != pPlayerNew->ServPNum)
		{
			CString str;
			str.Format("newpnum  =%d ,  pnum = %d, ServPNum = %d", newpnum, pnum, pPlayerNew->ServPNum);
			AfxMessageBox(str);
		}
#endif
		CString str;
		str.Format("<%s>���� ���ӿ� ���� �մϴ�.\n", pPlayerNew->UI.NickName);
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

		CString str1;
		str1.Format( "## ID: %s(%s)���� ���ӿ� ���� �մϴ�##\n", pPlayerNew->UI.ID, pPlayerNew->UI.NickName );
		AddLobbyHideChatText( &str1 );
	}
	// 3)���� �������̰�  ������ �ϴ³� ����
	else
	{
		for (i=0; i<nMaxPlayer; i++)	
		{
			pPlayer = GAME()->GetPlayerNP(i);
			if (strlen(pPlayer->UI.ID)==0){pnum=i; break;}
		}
		if (pnum == -1) return FALSE; // ����ִ� ���� ã�� ��������
		
		GAME()->m_nMyPosPNum = 0; // �����ʿ� �ε���
		GAME()->m_nNo1PosPNum = GAME()->GetPNum_ByServPN(0); // �������� 0 �� �ε���
		
		// �� ������ �����ϰ� ���� ������ �÷��̾��� ������ ����
		pPlayer = GAME()->GetPlayerNP(nowpnum);
		pPlayer->UI.cObserver = 0;
		CPlayer_NP *pPlayerNew = GAME()->GetPlayerNP(pnum);
		pPlayerNew->SetNewPlayer(&pPlayer->UI);
		pPlayerNew->SetChangeRoomInfo(*pPlayer->GetChangeRoomInfo());
		g_pGameView->m_cPlayerDrawManager.Reset(pnum);
		pPlayer->Clear();
		g_pGameView->m_cPlayerDrawManager.SetShow(pnum, true);	// �� ���� �����ؾ� ������ ȭ�鿡 ���δ�.

		g_RI.NowUserNum += 1;
		g_RI.NowObserverNum -= 1;
						
		pPlayerNew->ServPNum = GAME()->GetServPNum_ByPN(pnum);// ������ �÷��̾� ��ȣ ����
		
		CString str;
		str.Format("## ## <%s>�� �������� ##\n", pPlayerNew->UI.NickName);
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

		CString str1;
		str1.Format( "## ID: %s(%s)���� ���� ����##\n", pPlayerNew->UI.ID, pPlayerNew->UI.NickName );
		AddLobbyHideChatText( &str1 );	
	}
	
	// ### [ �ߺ��� IP�� ] ###
	for (i=0; i<nMaxPlayer; i++) {
		int pn = GAME()->GetPNum_ByServPN(i);
		pPlayer = GAME()->GetPlayerNP(pn);
		if (strlen(pPlayer->UI.ID) > 0)	{
			pPlayer->MyIpCheck = msg.IPA->IpIndex[i];
		}
	}

	// ### [���� �߰� �۾�] ###
	pPlayer = GAME()->GetPlayerNP(pnum);
	pPlayer->nSndFxKind = *msg.SndFxKind;
	CObserverDlg *pObserverDlg = GetObserverDlg();
	pObserverDlg->User_DelItem(pPlayer->UI.UNum, pPlayer->UI.ID);
	return TRUE;
} 

//������ ����Ǿ���
LONG CGameNewPoker::OnReceiveGameOverResult(int TotSize, char *lpData)
{
	CSV_GAMEOVERRESULT msg;
	msg.Get(lpData, TotSize);
	
	GM().SetWaitResponse(false);
	static BOOL bProccessed = FALSE;
	if (bProccessed != FALSE) return FALSE;
	bProccessed = TRUE;
	g_pGameView->m_cPlayerDrawManager.ClearTimer();
	GAME()->OnGameOver(msg.GOR);
	g_cOwnLimit.OnGameOver();	//�����ѵ�
	bProccessed = FALSE;

	return TRUE;
}

//���ӹ� ä�õ���Ÿ
LONG CGameNewPoker::OnReceiveChatData(int TotSize, char *lpData)
{
	CCL_CHATDATA msg;
	msg.Get(lpData, TotSize);

	if (*msg.RoomNum != g_RI.RoomNum) return FALSE;
	// ��ȭ ���� �ź����� ���̵��̸� ����
	int pnum = GAME()->GetPlayerPNum(msg.ID);

	CString str;
	char chat[256]={0,};
	strncpy_s(chat, sizeof(chat), msg.StrChat, min(*msg.l_StrChat, 255));
	//(�߰�)
	// ������ ���� Ư�����ڸ� ����Ͽ��ٸ� ���鹮�ڷ� �ٲ�
	int slen = strlen(chat);
	for (int s=0; s<slen-1; s++) {
		// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
		if (chat[s] == 8 || chat[s] == 9 || chat[s] == 10 || chat[s] == 13) {
			chat[s] = ' ';
		}
	}

	char szTBuf[512];
	NMBASE::UTIL::NMIsBadToRep( chat, szTBuf, 512 );

	// �����ڸ���۾�
	if ( strcmp("������",msg.ID)==0 )// ### [������ ��� �۾�] ###
	{
		//- lorddan ( 2008.10.07 ) - Bad Filter
		//			str.Format("%s> %s\n", GAME()->GetPlayerNP(pnum)->UI.NickName , chat);	
		str.Format("������> %s\n", szTBuf);
		//- lorddan ( 2008.10.07 ) - Bad Filter				
		AddGameChatViewEdit(str, GetChatColor(ECC2_SERVMSG));
	}
	else
	{
		if (g_cUIData.m_bObserverChat)
		{
			if (g_ObserverMan.IsMyObserver())
			{
				if ( pnum >= GAME()->GetMaxPlayer())
				{
					str.Format("%s> %s\n", GAME()->GetPlayerNP(pnum)->UI.NickName , szTBuf);
					AddGameChatViewEdit(str, GetChatColor(ECC2_CHAT), true);
				}
			}
			else
			{
				if ( pnum < GAME()->GetMaxPlayer())
				{
					str.Format("%s> %s\n", GAME()->GetPlayerNP(pnum)->UI.NickName , szTBuf);
					AddGameChatViewEdit(str, GetChatColor(ECC2_CHAT), true);
				}
			}
		}
		else
		{
			//������ ä�� ���� ���� �߰�
			if ( pnum >= GAME()->GetMaxPlayer() )
			{
				if ( g_Config.bNoObserverChat && strncmp(msg.ID, GM().GetMyInfo()->UI.ID, 15)!=0 )
				{
					return FALSE;
				}
			}

			str.Format("%s> %s\n", GAME()->GetPlayerNP(pnum)->UI.NickName , szTBuf);
			AddGameChatViewEdit(str, GetChatColor(ECC2_CHAT), true);
		}
	}		

	CString str1;
	str1.Format("## ID: %s(%s)> %s\n",GAME()->GetPlayerNP(pnum)->UI.ID , GAME()->GetPlayerNP(pnum)->UI.NickName , szTBuf );
	AddLobbyHideChatText( &str1 );
	return TRUE;
} 

//�����⿹��
LONG CGameNewPoker::OnReceiveExitSubscript(int TotSize, char *lpData)
{
	CCL_EXITSUBSCRIPT msg;
	msg.Get(lpData, TotSize);

	int pnum = GAME()->GetPlayerPNum(msg.ID);
	if (pnum==-1) return FALSE;

	int bexit = *msg.bExit;

	CString str;
	// ### [ ������� ] ###
	if (bexit == 0)
	{
		if (strcmp(GM().GetMyInfo()->UI.ID, msg.ID) == 0)
		{// ������
			g_pGameView->m_bExitSubscript = FALSE;
			ExitGameRequest();
		}
	}
	else if (bexit==1)
	{
		if (strcmp(GM().GetMyInfo()->UI.ID, msg.ID) == 0)
		{// ������
			g_pGameView->m_bExitSubscript = TRUE;
		}
		// �г��Ӽ��� 				
		str.Format("[%s]�� ������ ����\n", GAME()->GetPlayerNP(pnum)->UI.NickName);

		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
		g_pGameView->m_cPlayerDrawManager.SetExitReservation(pnum, true);
	}
	else if (bexit == 2)
	{
		if (strcmp(GM().GetMyInfo()->UI.ID, msg.ID) == 0)
		{// ������
			g_pGameView->m_bExitSubscript = FALSE;
		}
		// �г��Ӽ��� 	
		str.Format("[%s]�� ������ ���\n", GAME()->GetPlayerNP(pnum)->UI.NickName);

		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
		g_pGameView->m_cPlayerDrawManager.SetExitReservation(pnum, false);
	}
	return TRUE;
}

//���� ����
LONG CGameNewPoker::OnReceiveStartGame(int TotSize, char *lpData)
{
	CSV_STARTCARD msg;
	msg.Get(lpData, TotSize);
	if (g_RI.State != 0) return FALSE;

	g_pGameView->m_cDisplayBoard.SetText(EDBT_GAMESTART);
	g_pGameView->m_cSidePotMoney.Reset();

	int pnum = GAME()->GetPlayerPNum(msg.ID);
	if (pnum == -1) return FALSE;
	// ### [ ������� ] ###
	if (strcmp(GM().GetMyInfo()->UI.ID, msg.ID) != 0) return FALSE;
	
	g_pGameView->m_cGameViewBtnMan.m_btnGameStart.Show(FALSE);				

	static BOOL bProccessed = FALSE;
	if (bProccessed) return FALSE;
	bProccessed = TRUE;			
	
	// ���� ���� ����
	GAME()->ResetGame();
	g_pGameView->m_cTotalBetMoney.Start();
	GAME()->m_nCurPlayer = g_Poker.CurPlayer;
	g_RI.State = 1;
	
	GAME()->DoPrepareGame(msg.pSC);					
	
	HWND hWnd = GetForegroundWindow();
	if (hWnd != GM().GetMainWnd()->GetSafeHwnd()){
		GM().GetMainWnd()->FlashWindow(TRUE);
	}

	bProccessed = FALSE;

	CString str;
	str.Format(" ----------------------------\n\n");
	AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_RESULT));	
	return TRUE;
}

//ī�� ����
LONG CGameNewPoker::OnReceiveSelectCard(int TotSize, char *lpData)
{
	CSV_SELECTCARD msg;
	msg.Get(lpData, TotSize);
	
	if (g_RI.State != 1) return FALSE;

	int pnum = GAME()->GetPlayerPNum(msg.ID);
	if (pnum == -1) return FALSE;

	const int nMaxPlayer = GAME()->GetMaxPlayer();
	if (pnum < 0  || pnum >= nMaxPlayer) return FALSE;
	if ( GAME()->GetPlayerNP(pnum)->JoinState==0 ) return FALSE;
	if ( !g_pGameView->m_cPlayerDrawManager.IsSelectCard(pnum) ) return FALSE;
		
	if (GAME()->GetPlayerNP(pnum)->m_nTotalCardNum == 3) 
	{
		g_pGameView->m_cPlayerDrawManager.SetSelectCard(pnum, false);
		return FALSE;
	}

	/////////////////////////////////////////////////////////////////////
	// ����ī�� ���ý� ���� Ÿ�̸� �����ش�.
	// ### [ ������� ] ###
	if ( g_ObserverMan.IsMyObserver() ) {
		if (GAME()->m_bGameStart && pnum != 0 && GAME()->GetPlayerNP(pnum)->PlayState == 1) {
			// �߰� ���̽� ī��
			GAME()->GetPlayerNP(pnum)->m_listCard[3].Clear();
			GAME()->GetPlayerNP(pnum)->m_nTotalCardNum -= 1;
		}
	}
	else
	{
		if (GAME()->m_bGameStart && pnum != 0  && GAME()->GetPlayerNP(pnum)->PlayState == 1) {
			// �߰� ���̽� ī��
			GAME()->GetPlayerNP(pnum)->m_listCard[3].Clear();
			GAME()->GetPlayerNP(pnum)->m_nTotalCardNum -= 1;					
		}
	}
	/////////////////////////////////////////////////////////////////////
	g_pGameView->m_cPlayerDrawManager.SetSelectCard(pnum, false);
	if (pnum != 0)
	GAME()->GetPlayerNP(pnum)->SetChioceCardMove(TRUE);
	return TRUE;
} 

//���� ��� ī�峯���� ����
LONG CGameNewPoker::OnReceiveWindCardEnd(int TotSize, char *lpData)
{
	CSV_WINDCARDEND msg;
	msg.Get(lpData, TotSize);
	if (g_RI.State != 1) return FALSE;

	int pnum = *msg.Reserve;
	const int nMaxPlayer = GAME()->GetMaxPlayer();
	if (pnum < 0 || pnum >= nMaxPlayer) return FALSE;
			
	GAME()->EndWindCard();
	return TRUE;
}

LONG CGameNewPoker::OnReceiveRaise(int TotSize, char *lpData)
{
	CSV_RAISE msg;
	msg.Get(lpData, TotSize);
	
	if (g_RI.State != 1)
		return FALSE;
	
	g_pGameView->m_cPlayerDrawManager.ClearTimer();
	GAME()->Raise(msg.pPG);
	
	return TRUE;
}

//������ ����
LONG CGameNewPoker::OnReceiveFoldUser(int TotSize, char *lpData)
{
	CSV_FOLDUSER msg;
	msg.Get(lpData, TotSize);
	
	// �������� �ƴϸ� ����
	if (g_RI.State != 1) return FALSE;

	// ���� ������ ������ ����
	if (strlen(msg.ID)==0) return FALSE;

	int pnum  = -1;
	const int nMaxPlayer = GAME()->GetMaxPlayer();

	for (int i=0; i<nMaxPlayer; i++) {
		if (strcmp(msg.ID, GAME()->GetPlayerNP(i)->UI.ID) == 0) { pnum = i; break; }
	}
	if (pnum == -1) return FALSE;
//	GDM().SetINT64(GDK_JACKPOT_MONEY_TEMP, *msg.JackPotMoney);

	int k=0;
	if (GAME()->GetPlayerNP(pnum)->PlayState == 1 && GAME()->GetPlayerNP(pnum)->JoinState == 1) { // ���� ������ ���ΰ�
		GAME()->GetPlayerNP(pnum)->bFold = TRUE;
		GAME()->GetPlayerNP(pnum)->PlayState = 0;
		if ( !g_ObserverMan.IsMyObserver() && pnum== 0) { // ### [ ������� ] ###
			g_pMyInfo_NP->bFold = TRUE;
			g_pMyInfo_NP->PlayState = 0;
			GAME()->GetPlayerNP(pnum)->m_nFoldRound = GAME()->GetPlayerNP(pnum)->m_nCardTotal;					
		}

		//���� ������ �� ������.
		GAME()->m_nStartUserNum--;
		if (GAME()->m_nStartUserNum <0) GAME()->m_nStartUserNum = 0;
	
		if (pnum == 0 && GAME()->GetPlayerNP(0)->m_nCardTotal>3) {// ########### ���� ����� ī�带 �ٽ� �׸���
			for (k = 2; k < GAME()->GetPlayerNP(0)->m_nCardTotal; k++)
			{
				CPoint pt = GAME()->GetPlayerNP(0)->m_listCard[k].GetPos();
				if ( !g_ObserverMan.IsMyObserver() ) pt.y += 7;
				GAME()->GetPlayerNP(0)->m_listCard[k].GetMove().SetCurPos(pt);
			}
			g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();
		}				
		
		if (GAME()->GetPlayerNP(pnum)->m_nCardTotal > 0)
		{
			for (k = 0; k < GAME()->GetPlayerNP(pnum)->m_nTotalCardNum; k++)
			{
				GAME()->GetPlayerNP(pnum)->SetFaceDown(k);
			}
		}				

		GAME()->SoundFxKind(1, GAME()->GetPlayerNP(pnum)->UI.Sex);// ### [���� �߰� �۾�] ### ����
		CString str;
		str.Format(g_StrMan.Get(_T("FOLD_USER")),GAME()->GetPlayerNP(pnum)->UI.NickName);
		AddGameInfoViewEdit(str.GetString(),GetChatColor(ECC2_NOTIFY));

		//��ź ī�� ���� �Ҷ� �� ���� ���� Ŀ��尡 �ȳ���ͼ�..,,
		if (GAME()->m_cAniSprObjectMan.pFirst != NULL)
		{
			CBaseObject *pNow = NULL;			
			CBaseObject* pNext = GAME()->m_cAniSprObjectMan.pFirst;	
			
			while (pNext)
			{
				CBaseObject* pNow = pNext;			
				
				if (pNow->m_nObjID == ANF_BET_CMD)
				{
					CBetCmdAni* pObj = (CBetCmdAni*)pNow;

					// ���� ���� �Ҷ��� �����Ŵ� ����� �ٽ� ��´�.
					// bDistCard �� ���ǿ� ������ ���� ������ ��ġ�� ���� �߻�
					// (���� ���� ������ �������� �ʰ� ��������)
					//if (pObj->m_nPNum  == pnum && g_Poker.bDistCard == FALSE)
					if (pObj->m_nPNum  == pnum)
					{
						pObj->Destroy();
					}
				}

				pNext = pNow->pNextNode;
			}
		}

		CBetCmdAni *pObj = new CBetCmdAni();
		pObj->Init(pnum,ANF_BET_CMD,ANF_BET_DIE,false);
		GAME()->m_cAniSprObjectMan.AddObject(pObj);
	}

	return TRUE;
}

//������ ���� ī��
LONG CGameNewPoker::OnReceiveHiddenCard(int TotSize, char *lpData)
{
	CSV_HIDDENCARD msg;
	msg.Get(lpData, TotSize);
	
	if (g_RI.State != 1)
		return FALSE;

	if (strlen(msg.ID)==0)
		return FALSE;

	if ( g_ObserverMan.IsMyObserver() )
		return FALSE;

	int pnum = -1;
	const int nMaxPlayer = GAME()->GetMaxPlayer();

	for (int i=0; i<nMaxPlayer; i++)
	{
		if (strcmp(msg.ID, GAME()->GetPlayerNP(i)->UI.ID) == 0)
		{
			pnum = i; break;
		}
	}

	if (pnum == -1) 
	{
#ifdef _DEBUG
		AfxMessageBox("Hidden Error");			
#endif			
		return FALSE;
	}

	if (GAME()->GetPlayerNP(pnum)->PlayState == 1 && GAME()->GetPlayerNP(pnum)->JoinState == 1)
	{
		if (g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME)
		{
			GAME()->GetPlayerNP(0)->m_listCard[7].SetCardNo(*msg.nCard); // �� ����
		}
		else
			GAME()->GetPlayerNP(0)->m_listCard[6].SetCardNo(*msg.nCard); // �� ����
	}

	//NMBASE::SOUND::g_Sound.PlayOggStream(SND_HIDDEN_BGM);

	return TRUE;
} 

LONG CGameNewPoker::OnReceiveEndGame(int TotSize, char *lpData)
{
	CSV_ENDGAME msg;
	msg.Get(lpData, TotSize);
	
	GAME()->m_cAniSprObjectMan.Destroy();	
	g_pGameView->m_cSidePotMoney.Reset();
	
	//yoo [20150120] 
	if (TRUE==CGame::OnReceiveResEndGame())
	{
		ExitGameRequest();
	}
	else if (g_pGameView->m_bExitSubscript)  // ������ ������¸� ���� ����
	{
		ExitGameRequest();
	}
	else 
	{
		// ���� ���� ����
		GAME()->ResetGame();
		GAME()->m_nCurPlayer = g_Poker.CurPlayer;
	}							
	
//	g_RI.bNightEvActive = 0;	
//	g_RI.nGoldEventActive = 0;

	g_cPromotionMan.OnEndGame();

	//��ũ�� ���α׷��� ������ ���� ��Ų��
	OnMacroCheck();

	return TRUE;
}

//[ ###����### ]���ӹ� & ����
LONG CGameNewPoker::OnReceiveSpecialCard(int TotSize, char *lpData)
{
	CSV_SPECIALCARD msg;
	msg.Get(lpData, TotSize);

	CString spec("");
	if (msg.pSC->nCard==1) spec = g_StrMan.Get(_T("SPECIAL0"));
	else if (msg.pSC->nCard==2) spec = g_StrMan.Get(_T("SPECIAL1"));
	else if (msg.pSC->nCard==3) spec = g_StrMan.Get(_T("SPECIAL2"));

	CString str("");
	if (msg.pSC->JackPotPlus>0)
	{
		CString strM = NumberToOrientalString(msg.pSC->JackPotPlus);
		strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
		str.Format(g_StrMan.Get(_T("MES_SPE2")),
		msg.pSC->ChanName, msg.pSC->nRoom, msg.pSC->NickName, spec, strM);
	}
	else
	{
		str.Format(g_StrMan.Get(_T("MES_SPE3")),
		msg.pSC->ChanName, msg.pSC->nRoom, msg.pSC->NickName, spec);
	}		

	// �����濡�� ���Դ�
	if (msg.pSC->JackPotPlus > 0 && msg.pSC->nRoom == g_RI.RoomNum )
	{			
		GAME()->m_i64JackPotActive = msg.pSC->JackPotPlus; // ���� ���ӹ濡�� ���Դ�.				
	}

	//GDM().SetINT64(GDK_JACKPOT_MONEY_TEMP, msg.pSC->NowJackPot);
	AddGameInfoViewEdit(str.GetString(),GetChatColor(ECC2_NOTIFY));
	return TRUE;
}

//�ڸ� ��Ŵ�� ��Ŷ ó��
LONG CGameNewPoker::OnReceiveSeatKeeper(int TotSize, char *lpData)
{
	CSV_SEATKEEPER msg;
	msg.Get( lpData, TotSize );
	g_SeatKeeper.ProcessPacket(&msg);
	return TRUE;	
}

LONG CGameNewPoker::OnReceiveChangeMoney(int TotSize, char *lpData)
{				
	CSV_CHANGE_MONEY msg;
	msg.Get(lpData,TotSize);

	GAME()->Change_Money(&msg);
	return TRUE;
}

// ��� ��û �㰡��
LONG CGameNewPoker::OnReceiveAcceptOutRoom(int TotSize, char *lpData)
{
	CSV_ACCEPT_OUTROOM msg;
	msg.Get(lpData, TotSize);

	GM().SetWaitResponse(false);
	GM().OnExitRoom((REASON_OUTROOM)*msg.Reason);

	g_cPromotionMan.Accept_OutRoom();
	
	return TRUE;
} 

//���ο� �÷��̾ �Խ��Ͽ���
LONG CGameNewPoker::OnReceiveUserEnterRoom(int TotSize, char *lpData)
{
	CSV_USERENTERROOM msg;
	msg.Get(lpData, TotSize);

	// ### [ ������� ] ###
	m_pGameProcess->User_EnterRoom(&msg);
	return TRUE;
} 

//����ڰ� �濡�� ��������
LONG CGameNewPoker::OnReceiveUserOutRoom(int TotSize, char *lpData)
{
	CSV_USEROUTROOM msg;
	msg.Get(lpData, TotSize);
	
	// ### [ ������� ] ###
	m_pGameProcess->User_OutRoom(&msg);
	return TRUE;
} 

// ������ �ٲ�����
LONG CGameNewPoker::OnReceiveChangeRoomMaster(int TotSize, char *lpData)
{
	CSV_CHANGEROOMMASTER msg;
	msg.Get(lpData, TotSize);
	
	if (strlen(msg.ID)==0) return FALSE;

	BOOL bBreak = TRUE;

	// �г��Ӽ��� 	
	CString NickName="";
	const int nMaxPlayer = GAME()->GetMaxPlayer();
	for (int i=0; i<nMaxPlayer; i++)
	{
		if (strcmp(msg.ID, GAME()->GetPlayerNP(i)->UI.ID) == 0) 
		{ 
			NickName = GAME()->GetPlayerNP(i)->UI.NickName;
			bBreak = FALSE; break; 
		}
	}

	if (bBreak == TRUE) return FALSE;
	
	// �� ������ �����Ѵ�
	strcpy_s(g_RI.ID, sizeof(g_RI.ID), msg.ID);
	// �г��Ӽ��� 	
	strcpy_s(g_RI.NickName, sizeof(g_RI.NickName), NickName );		
	g_RI.UNum = *msg.UNum;
	g_RI.RoomMasterType = *msg.RoomMasterType;
	// �Ŀ� �����Ұ�
	CString str;
	
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		str="���� ����Ŀ";
	else
		str="������ ����Ŀ";
#else //UPGRADE_20120308_TOCTOC_CHANNELING
	str="������ ����Ŀ";
#endif //UPGRADE_20120308_TOCTOC_CHANNELING
	SetMainWindowText(str);

	// ### [ ������� ] ###
	if (strcmp(msg.ID, GM().GetMyInfo()->UI.ID) == 0) // ���� ������ �ȴ�
	{
		g_MastPN = 0;

		
	}
	else
	{
		int newMastPN = -1;
		for (int i=1; i<nMaxPlayer; i++) 
			if (strcmp(GAME()->GetPlayerNP(i)->UI.ID, msg.ID)==0) {newMastPN=i; break;}

		if (newMastPN == -1) return FALSE;

		g_MastPN = newMastPN;
		
	}

	
	str.Format("���庯��: [%s]��\n", NickName);
	AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

	str.Format("���庯��:[ID: %s(%s)]��\n",msg.ID , NickName );
	AddLobbyHideChatText( &str );

	g_cBanishMan.Clear();
	return TRUE;
}

//������������ ������
LONG CGameNewPoker::OnReceiveAbnormalOutRoom(int TotSize, char *lpData)
{
	CSV_ABNORMALOUTROOM msg;
	msg.Get(lpData, TotSize);
	
	GM().SetWaitResponse(false);
	
	// ���������� ������� �����ϰ� ������ ����
	GM().SetAbnormalOutRoom(true);
	ExitGameRequest();
	return TRUE;
} 

//�ӼӸ�
LONG CGameNewPoker::OnReceiveWhisperChat(int TotSize, char *lpData)
{
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME) return FALSE;

	CSV_WHISPERCHAT msg;
	msg.Get(lpData, TotSize);

	//�Ӹ� ���� �ź��϶� ��� ����
	if (CCommunityManager::Instance()->GetOption(COM_ECOT_WHISPER) == COM_ECOPT_OFF) return FALSE;
	//������Ʈ�� ������ ���� �Ӹ��� ����
	if (CCommunityManager::Instance()->IsExistBlackBuddy(msg.m_szNickName)) return TRUE;
	//ģ�����Ը� �Ӹ��� �ޱ� �ɼ��� �� ģ���� �ƴѻ���� ���� �Ӹ��̸� ����
	if ((CCommunityManager::Instance()->GetOption(COM_ECOT_WHISPER) == COM_ECOPT_MYBUDDY) && !CCommunityManager::Instance()->IsExistBuddy(msg.m_szNickName)) return TRUE;

	// ��ȭ ���� �ź����� ���̵��̸� ����
	int pnum = GAME()->GetPlayerPNum(msg.m_szID);

	CString str;
	CString chat;
	chat.Append((LPCTSTR)msg.m_StrChat, *msg.m_sStrChatLen);
	//(�߰�)
	// ������ ���� Ư�����ڸ� ����Ͽ��ٸ� ���鹮�ڷ� �ٲ�
	for (int s=0; s<chat.GetLength()-1; s++)
	{
		// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
		if (chat.GetAt(s) == 8 || chat.GetAt(s) == 9 || chat.GetAt(s) == 10 || chat.GetAt(s) == 13)
		{
			chat.SetAt(s, ' ');
		}
	}

	// �г��� �۾� 2006/11/08		
	char szTBuf[512];
	NMBASE::UTIL::NMIsBadToRep((char *)(LPCTSTR)chat, szTBuf, 512 );
	str.Format("[�Ӹ�����:%s] %s\n", msg.m_szNickName, szTBuf);

	// �Ӹ� �˸� ȿ���� ���
	NMBASE::SOUND::g_Sound.PlayWav(SND_WHISPER);
	AddGameChatViewEdit(str, GetChatColor(ECC2_WHISPER_RECEIVE), true);

	//�Ӹ�������߰�
	AddChattingWhisperList( msg.m_szNickName, false, true, false );
	//g_pChatWinMan->AddWhipserUser( msg.m_szNickName );

	// �г��� �۾� 2006/11/08
	str.Format("�Ӹ�: ID : %s(%s)> %s\n", msg.m_szID ,  msg.m_szNickName , szTBuf );
	AddLobbyHideChatText(&str);

	return TRUE;
} 

LONG CGameNewPoker::OnReceiveAdminWhisperChat(int TotSize, char *lpData)
{
	// �����ڸ���۾�
	CNM_ADMIN_WHISPERCHAT msg;
	msg.Get(lpData, TotSize);
	
	CString str;
	CString chat;
	chat.Append((LPCTSTR)msg.StrChat, *msg.l_StrChat);
	
	//(�߰�)
	// ������ ���� Ư�����ڸ� ����Ͽ��ٸ� ���鹮�ڷ� �ٲ�
	for (int s=0; s<chat.GetLength()-1; s++)
	{
		// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
		if (chat.GetAt(s) == 8 || chat.GetAt(s) == 9 || chat.GetAt(s) == 10 || chat.GetAt(s) == 13)
		{
			chat.SetAt(s, ' ');
		}
	}

	str.Format("[�Ӹ�:%s] %s\n", "������", chat.operator LPCTSTR());
	// �Ӹ� �˸� ȿ���� ���
	NMBASE::SOUND::g_Sound.PlayWav(SND_WHISPER);		
	AddGameChatViewEdit(str.GetString(), GetChatColor(ECC2_WHISPER_RECEIVE));

	return TRUE;
} 

//### [������ ��� �۾�] ###  ������ ����
LONG CGameNewPoker::OnReceiveChangeStyleRoomInfo(int TotSize, char *lpData)
{
	// ���� ��ġ�� �׹��ΰ�, ���� �ΰ�
	CSV_CHANGESTYLEROOMINFO msg;
	msg.Get(lpData, TotSize);
	
	if (msg.SRI->nMaxUserNum < 100)
	{	
		strncpy_s(g_RI.Title, sizeof(g_RI.Title), msg.SRI->Title, 30);				
		
		CString str;

	#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
		if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
			str="���� ����Ŀ";
		else
			str="������ ����Ŀ";
	#else //UPGRADE_20120308_TOCTOC_CHANNELING
		str="������ ����Ŀ";
	#endif //UPGRADE_20120308_TOCTOC_CHANNELING
		
		SetMainWindowText(str);

		g_pGameView->m_cTitle.MakeTitle();
		
		str.Format("�������� ����Ǿ����ϴ�.\n");				

		AddGameInfoViewEdit(str.GetString(),GetChatColor(ECC2_NOTIFY));
	}
	else// �� ���� ������ ���� �Ǿ���.
	{					
		CString str;
		str.Format("������ ������ ���� �Ǿ����ϴ�.\n");				
		AddGameInfoViewEdit(str.GetString(),GetChatColor(ECC2_NOTIFY));
	}				

	NMBASE::SOUND::g_Sound.PlayWav(SND_WHISPER);

	return TRUE;
} 

LONG CGameNewPoker::OnReceiveAddBombCard(int TotSize, char *lpData)
{
	CSV_ADD_BOMBCARD msg;
	msg.Get(lpData, TotSize);
	
	if (g_RI.FormKind < BOMB_GAME) return FALSE;
	if (g_RI.State != 1) return FALSE;			

	if (msg.pBC->nState != 0) return FALSE;
	if (msg.pBC->nRoomNum != g_RI.RoomNum) return FALSE;
	if (strcmp(msg.pBC->ID ,g_RI.ID)!=0) return FALSE;

	g_Poker.nReMainTotalCardNum = msg.pBC->nReMainTotalCardNum;
	GAME()->SetADDBombCard();			

	CCL_ADD_BOMBCARD_RESULT rmsg;
	rmsg.Set(g_RI.RoomNum,g_pMyInfo_NP->ServPNum,GM().GetMyInfo()->UI.ID);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), rmsg.pData, rmsg.GetTotalSize());

	return TRUE;
}

LONG CGameNewPoker::OnReceiveOpenBombCard(int TotSize, char *lpData)
{
	CSV_OPEN_BOMBCARD msg;
	msg.Get(lpData, TotSize);
	
	if (g_RI.FormKind < BOMB_GAME) return FALSE;
	if (g_RI.State != 1) return FALSE;					

	if (msg.pBC->nState != 1) return FALSE;
	if (msg.pBC->nRoomNum != g_RI.RoomNum) return FALSE;
	if (strcmp(msg.pBC->ID ,g_RI.ID)!=0) return FALSE;

	GAME()->m_nCurPlayer = -1;
	g_Poker.nBossPNum = msg.pBC->nBossNum;
	GAME()->OnSetMoveChip();
	g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(true);
	g_Poker.nReMainTotalCardNum = msg.pBC->nReMainTotalCardNum;
	
	if (msg.pBC->nBombCard == 100)g_bGetJokerCard = TRUE;
	GAME()->SetOPenBombCard(msg.pBC->nBombCard);

	return TRUE;
}



//���κ��� ���� ��û ���
LONG CGameNewPoker::OnReceiveInsuranceSupplyResult(int TotSize, char *lpData)
{
	CSV_AIN_INSURANCE_SUPPLY_RESULT msg;
	msg.Get(lpData,TotSize);

	if (msg.AIN_DATA->UNum != GM().GetMyInfo()->UI.UNum) return FALSE;			
	if (strlen(msg.AIN_DATA->ID) == 0 || strcmp(msg.AIN_DATA->ID,GM().GetMyInfo()->UI.ID) != 0) return FALSE;			
	if (msg.AIN_DATA->nAIN_Insurance_Result == 0){			
		ShowMainMessageDlg("���κ��� ������ ���� �Ͽ����ϴ�. �ٽ� �õ��� �ּ���");
		return FALSE;
	}

	GM().GetMyInfo()->UI.SetMoney(msg.AIN_DATA->nApplyCurPMoney);
	GM().GetMyInfo()->UI.nAIN_Insurance_Money = msg.AIN_DATA->nCurAIN_InsuranceMoney;
	GM().GetMyInfo()->UI.nAIN_InsuranceKind = msg.AIN_DATA->nCurAIN_InsuranceKind;
	GM().GetMyInfo()->UI.nIcon = GetMoneyGrade(GM().GetMyInfo()->UI.GetTotalMoney());

	GAME()->GetPlayerNP(0)->UI.SetMoney(GM().GetMyInfo()->UI.GetMoney());
	GAME()->GetPlayerNP(0)->UI.nAIN_Insurance_Money = GM().GetMyInfo()->UI.nAIN_Insurance_Money;
	GAME()->GetPlayerNP(0)->UI.nAIN_InsuranceKind = GM().GetMyInfo()->UI.nAIN_InsuranceKind;
	GAME()->GetPlayerNP(0)->UI.nIcon = GM().GetMyInfo()->UI.nIcon;
	return TRUE;
}

LONG  CGameNewPoker::OnReceiveChangeGameUserInfo(int TotSize, char *lpData)
{
	CSV_CHANGE_GAMEUSERINFO msg;
	msg.Get(lpData,TotSize);

	const int nMaxPlayer = GAME()->GetMaxPlayer();
	for (int i=0; i < nMaxPlayer; i++)
	{
		if (GAME()->GetPlayerNP(i)->PNum == msg.pCGUI->nPNum) {
			GAME()->GetPlayerNP(i)->UI.DS_IDColor = msg.pCGUI->nIDColor;
			break;
		}
	}
	return TRUE;
}

LONG CGameNewPoker::OnReceiveNotifyChangeOverMoney(int TotSize, char *lpData)
{	
	CSV_NOTIFY_CHANGEOVERMONEY msg;
	msg.Get(lpData,TotSize);
	
	if (strncmp(msg.szID, GM().GetMyInfo()->UI.ID, 15)==0)
	{
		if (*msg.llOverMoney>0)
		{
			//�ʰ���
			g_cOwnLimit.SetOverMoney(*msg.llOverMoney, *msg.m_ChipKind, msg.stOverMoneyEndDate);
		}

		if (*msg.llRevisionMoney>0)
		{
			//�ʰ���
			g_cOwnLimit.SetRevisionMoney(*msg.llRevisionMoney, *msg.m_ChipKind, msg.stRevisionMoneyEndDate);
		}			
	}
	else
	{
		g_cOwnLimit.SetOtherUserOverMoney(msg.szID, *msg.llOverMoney, *msg.m_ChipKind, msg.stOverMoneyEndDate);
	}

	if (GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME)
	{
		g_cOwnLimit.ShowPopup();
	}

	return TRUE;
}


LONG CGameNewPoker::OnReceiveGiveJokerCard(int TotSize, char *lpData)
{
	CSV_GIVE_JOKERCARD msg;
	msg.Get( lpData,TotSize);

	if (!g_ObserverMan.IsMyObserver()){
		memset(&g_JokerCardDeck,-1,sizeof(g_JokerCardDeck));		
		memcpy(&g_JokerCardDeck,msg.pJD,sizeof(g_JokerCardDeck));		
	}
	g_bGetJokerCard = TRUE;
	return TRUE;
}

//### [���� �߰� �۾�] ###
LONG CGameNewPoker::OnReceiveAskChangeSoundFX(int TotSize, char *lpData)
{
	CSV_ASK_CHANGESNDFX msg;
	msg.Get(lpData, TotSize);

	if (g_RI.RoomNum	!= *msg.RoomNum) return FALSE;

	int spnum = *msg.ServPNum;
	const int nMaxNewPlayer = GM().GetMaxNewPlayer();
	if (spnum < 0 || spnum >= nMaxNewPlayer) return FALSE;// ### [ ������� ] ###

	int pn = GM().GetCurrentProcess()->GetPlayerPNum(msg.ID);
	if (pn == -1) return FALSE;

	// ### [���� �߰� �۾�] ###
	CPlayer *pPlayer = GM().GetCurrentProcess()->GetPlayer(pn);
	pPlayer->nSndFxKind = *msg.SndFX;		

	// ### [ ������� ] ###
	if (strcmp(msg.ID, GM().GetMyInfo()->UI.ID)== 0) {
		GM().GetMyInfo()->nSndFxKind = *msg.SndFX;
	}
	return TRUE;
}

//�÷��̾ ��û �޼����� �ź���
LONG CGameNewPoker::OnReceiveRefuseInvite(int TotSize, char *lpData)
{
	CSV_REFUSE_INVITE msg;
	msg.Get(lpData, TotSize);

	char strmsg[256]={0,};
	strncpy_s(strmsg, sizeof(strmsg), msg.StrMsg, *msg.l_StrMsg);

	static BOOL bProccessed = FALSE;
	if (bProccessed != FALSE) return FALSE;
	bProccessed = TRUE;

	// �г��Ӽ��� 	2006/10/30
	CString NickName;
	SMALLUSERINFO* pSUI = GetUserSmallInfo( msg.MyID );
	if ( pSUI )
	{
		NickName = pSUI->NickName;
	}
	else
	{				
		return FALSE;
	}

	CString str;
	// �г��Ӽ��� 	2006/10/30
	str.Format("## [ID: %s(%s)]���� �ʴ븦 �����ϼ̽��ϴ�##\n",pSUI->ID , pSUI->NickName );
	AddLobbyHideChatText( &str );

	if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
	{
		str.Format("[%s]�Բ��� �ʴ븦\n�����ϼ̽��ϴ�\n",pSUI->NickName );
		AddGameInfoViewEdit(str);
	}
	bProccessed = FALSE;
	return TRUE;
} 

LONG CGameNewPoker::OnReceiveResGameContinue(int TotSize, char *lpData)
{

	CGame::OnReceiveResGameContinue(TotSize, lpData);

	CSV_RES_GAME_CONTINUE msg;
	msg.Get( lpData, TotSize ); 

	const int nMaxPlayer = GAME()->GetMaxPlayer();
	for (int i = 0; i < nMaxPlayer; i++ )
	{
		if ( strcmp( GAME()->m_cGameOverResultData.Ui[i].ID, msg.ID ) == 0 )
		{
			GAME()->m_cGameOverResultData.Ui[i].PMoney = *msg.llMoney;
			break;
		}
	}

	return TRUE;
} 


LONG CGameNewPoker::OnReceiveJACKPOTMONEYINFO(int TotSize, char *lpData)
{

	packet::TSendPacket< jackpot::JP_MONEYINFO > JP_MONEY_Packet;
	JP_MONEY_Packet.Get( lpData, TotSize );
	jackpot::JP_MONEYINFO* pAlaram = static_cast< jackpot::JP_MONEYINFO* >( JP_MONEY_Packet.Various() );

	GAME()->JackPotMoneyUpdate(pAlaram);

// 	CSV_SEND_JACKPOTMONEYINFO msg;
// 	msg.Get(lpData, TotSize);			
// 
// 	GAME()->JackPotMoneyUpdate(msg.JackpotMoneyInfo);

	return TRUE;
}

//������ : �սǱݾ� ���ӳ� �����
LONG CGameNewPoker::OnReceiveSV_LOSSMONEYINFO(int TotSize, char *lpData)
{
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return TRUE;


	CSV_DAILY_LOSS_MONEY msg;
	msg.Get(lpData,TotSize);

	//�� ���ӿ��� �����ϵ��� GM�� ���� ����
	GM().SetLossmoneyInfo(*msg.llLossMoney, *msg.tBlockTime, *msg.eState, *msg.idxGame) ;

	g_pGameView->m_cGameViewBtnMan.SetLossMoneyWarning() ;

	return TRUE;
}

//������ : ����ϱ� ��� �߰�
LONG CGameNewPoker::OnReceiveSV_ASK_WAIT_INROOM(int TotSize, char *lpData)
{	
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return TRUE;

	CSV_WAIT_INROOM msg;
	msg.Get(lpData, TotSize);
	g_pGameView->m_cGameViewBtnMan.SetWaitFlag(*msg.m_bWait); 
	
	return TRUE;
}

LONG CGameNewPoker::OnReceiveSV_Reward_Allin_Angel(int TotSize, char *lpData)
{	
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return TRUE;

	CSV_REWARD_ALLIN_ANGEL msg;
	msg.Get(lpData, TotSize);
	g_pGameView->m_cPlayerDrawManager.SetPlayAllinAngel((*msg.m_RemainEnableCount), timeGetTime());

	return TRUE;
}

LONG CGameNewPoker::OnReceiveSV_Reward_Allin_Angel_Gold(int TotSize, char *lpData)
{	
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return TRUE;

	CSV_REWARD_ALLIN_ANGEL_GOLD msg;
	msg.Get(lpData, TotSize);
	g_pGameView->m_cPlayerDrawManager.SetPlayGoldReward((*msg.m_RewardMoney), timeGetTime());

	return TRUE;
}

LONG CGameNewPoker::OnReceiveSV_Reward_CongNoti(int TotSize, char *lpData)
{	
	
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return TRUE;

	CSV_RECEIVE_CONG_MSG msg;
	msg.Get(lpData, TotSize);

	CString strID;
	strID.Format("���� ��� : %s��", msg.m_Send_ID);
	CString strChat;
	strChat.Format("%s", msg.StrChat);
	g_pGameView->m_cPlayerDrawManager.SetPlayCongNoti(strID.GetString(), strChat.GetString(), *msg.m_ShowTime);

	return TRUE;
}

LONG CGameNewPoker::OnReceiveSV_ETCCHIP_PLAYER_INFO(int TotSize, char *lpData)
{	
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return TRUE;

	CSV_ETCCHIP_PLAYER_INFO msg;
	msg.Get(lpData, TotSize);

	if (strcmp(msg.m_ChipType, "GD")==0)
	{
		int pnum = *msg.m_pnum;	
		if (pnum >= MAX_PLAYER && g_RI.ChipKind == 1) 
		{
			// ������ ����Ʈ ���
			int ounum   = g_PLAYER(pnum)->UI.UNum;
			int sex     = g_PLAYER(pnum)->UI.Sex;
			int level   = g_PLAYER(pnum)->UI.nIcon;
			INT64 Money = *msg.m_RealChip;
			int seq		= 0;

			CObserverDlg *pObserverDlg = GetObserverDlg();
			if (NULL != pObserverDlg)
			{
				pObserverDlg->User_DelItem(ounum, g_PLAYER(pnum)->UI.ID);
				pObserverDlg->User_InsItem(ounum, g_PLAYER(pnum)->UI.ID, g_PLAYER(pnum)->UI.NickName, sex, level, Money, seq);
			}			
		}	
		else
		{
			g_PLAYER(g_GAMEPROCESS()->GetPNum_ByServPN(pnum))->SetGold(*msg.m_RealChip);
		}
	}

	return TRUE;
}

LONG CGameNewPoker::OnReceiveSV_PM_ODDEVEN_WINNER_NOTI(int TotSize, char *lpData)
{
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return TRUE;

	//CSV_PM_ODDEVEN_WINNER_NOTI msg;
	CSV_PM_WHEEL_WINNER_NOTI msg;
	msg.Get(lpData, TotSize);

	CString strID;
	strID.Format("%s", msg.m_UserID);
	g_pGameView->m_cPlayerDrawManager.SetPlayGoldNoti(strID.GetString(), *msg.m_RewardMoney, timeGetTime());

	return TRUE;
}

LONG CGameNewPoker::OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData)
{
	if (CGame::OnPacketNotify(pName, Signal, TotSize, lpData))
		return TRUE;

	if (g_cOneShotCharge.OnPacketNotify(pName,Signal, TotSize, lpData))	return TRUE;
	if (g_pGameView->m_cGameHelper.OnPacketNotify(pName,Signal, TotSize, lpData))	return TRUE;
	if (g_cOwnLimit.OnPacketNotify(pName,Signal, TotSize, lpData))	return TRUE;
	if (g_cBanishMan.OnPacketNotify(pName, Signal, TotSize, lpData)) return TRUE;
	if ( g_cPromotionMan.OnPacketNotify(pName, Signal, TotSize, lpData) ) return TRUE;

	if ( g_cVipJackpot.OnPacketNotify(pName, Signal, TotSize, lpData) ) return TRUE;

	switch(Signal)
	{
	case SV_SEATKEEPER:				return OnReceiveSeatKeeper(TotSize, lpData);
	case SV_CHANGE_MONEY:			return OnReceiveChangeMoney(TotSize, lpData);
	}

	if (GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME) return FALSE;

	switch(Signal)
	{
	case SV_STARTBTNENABLE:			return OnReceiveStartButtonEnable(TotSize, lpData);
	case SV_OBSERVER_MESSAGE:		return OnReceiveObserverMessege(TotSize, lpData);
	case SV_RESERVATIONINFO:		return OnReceiveReservationInfo(TotSize, lpData);
	case SV_ACCEPT_PARTICIPATION:	return OnReceiveAcceptParticipation(TotSize, lpData);
	case SV_ACCEPT_EMERAGE:			return OnReceiveAcceptEmerage(TotSize, lpData);
	case SV_ENTER_PARTICIPATION:	return OnReceiveEnterParticipation(TotSize, lpData);
	case SV_ENTER_EMERAGE:			return OnReceiveEnterEmerage(TotSize, lpData);
	case SV_GAMEOVERRESULT:			return OnReceiveGameOverResult(TotSize, lpData);
	case CL_CHATDATA:				return OnReceiveChatData(TotSize, lpData);
	case CL_EXITSUBSCRIPT:			return OnReceiveExitSubscript(TotSize, lpData);
	case SV_STARTCARD:				return OnReceiveStartGame(TotSize, lpData);
	case SV_SELECTCARD:				return OnReceiveSelectCard(TotSize, lpData);
	case SV_WINDCARDEND :			return OnReceiveWindCardEnd(TotSize, lpData);
	case SV_RAISE:					return OnReceiveRaise(TotSize, lpData);
	case SV_FOLDUSER:				return OnReceiveFoldUser(TotSize, lpData);
	case SV_HIDDENCARD:				return OnReceiveHiddenCard(TotSize, lpData);
	case SV_ENDGAME:				return OnReceiveEndGame(TotSize, lpData);
	case SV_SPECIALCARD :			return OnReceiveSpecialCard(TotSize, lpData);	
	case SV_ACCEPT_OUTROOM:			return OnReceiveAcceptOutRoom(TotSize, lpData);
	case SV_USERENTERROOM:			return OnReceiveUserEnterRoom(TotSize, lpData);
	case SV_USEROUTROOM:			return OnReceiveUserOutRoom(TotSize, lpData);
	case SV_CHANGEROOMMASTER:		return OnReceiveChangeRoomMaster(TotSize, lpData);
	case SV_ABNORMALOUTROOM:		return OnReceiveAbnormalOutRoom(TotSize, lpData);
	case SV_WHISPERCHAT:			return OnReceiveWhisperChat(TotSize, lpData);
	case NM_ADMIN_WHISPERCHAT:		return OnReceiveAdminWhisperChat(TotSize, lpData);
	case SV_CHANGESTYLEROOMINFO:	return OnReceiveChangeStyleRoomInfo(TotSize, lpData);
	case SV_ADD_BOMBCARD:			return OnReceiveAddBombCard(TotSize, lpData);
	case SV_OPEN_BOMBCARD:			return OnReceiveOpenBombCard(TotSize, lpData);	
	case SV_AIN_INSURANCE_SUPPLY_RESULT:return OnReceiveInsuranceSupplyResult(TotSize, lpData);
	case SV_CHANGE_GAMEUSERINFO:	return OnReceiveChangeGameUserInfo(TotSize, lpData);	
	case SV_GIVE_JOKERCARD:			return OnReceiveGiveJokerCard(TotSize, lpData);
	case SV_ASK_CHANGESNDFX:		return OnReceiveAskChangeSoundFX(TotSize, lpData);
	case SV_REFUSE_INVITE:			return OnReceiveRefuseInvite(TotSize, lpData);
	case OPCODE_SV_JP_INFO:			return OnReceiveJACKPOTMONEYINFO(TotSize, lpData);
	case SV_DAILY_LOSS_MONEY:		return OnReceiveSV_LOSSMONEYINFO(TotSize, lpData);// ������ ��Ŷ.
	case SV_ASK_WAIT_INROOM:		return OnReceiveSV_ASK_WAIT_INROOM(TotSize, lpData);// ������ ��Ŷ.
	case SV_REWARD_ALLIN_ANGEL:		return OnReceiveSV_Reward_Allin_Angel(TotSize, lpData);
	case SV_REWARD_ALLIN_ANGEL_GOLD:	return OnReceiveSV_Reward_Allin_Angel_Gold(TotSize, lpData);
	case SV_RECEIVE_CONG_MSG:		return OnReceiveSV_Reward_CongNoti(TotSize, lpData);
	case SV_ETCCHIP_PLAYER_INFO:	return OnReceiveSV_ETCCHIP_PLAYER_INFO(TotSize, lpData);
	//case SV_PM_ODDEVEN_WINNER_NOTI:	return OnReceiveSV_PM_ODDEVEN_WINNER_NOTI(TotSize, lpData);
	case SV_PM_WHEEL_WINNER_NOTI:	return OnReceiveSV_PM_ODDEVEN_WINNER_NOTI(TotSize, lpData);
		break;
	// �ΰ��� ���� ī�� ���� ��ư ������ �� ����
	case SV_ASK_CARDOPEN_RESULT:
		{
			CSV_ASK_CARDOPEN_RESULT msg;
			msg.Get(lpData, TotSize);

			bool bCardOpen = *msg.m_bCardOpen;

			// ���� ī�� ���� ��ư ���� �ε���(0�̸� Off, 1�̸� On)
			int nBtnNum;
			CString csMsg;
			
			if (bCardOpen)
			{
				nBtnNum = 0;
				csMsg = "[���� �˸�] ī�� OPEN�� ����մϴ�.\n";
			}
			else
			{
				nBtnNum = 1;
				csMsg = "[���� �˸�] ī�� OPEN ����� ��� �մϴ�.\n";
			}
			
			g_pGameView->m_cGameViewBtnMan.m_btnCardOpen.SetSprite(
				&(g_pGameView->m_cGameViewBtnMan.m_sprUICardOpenBtn), nBtnNum);
			
			// �ΰ��� ä�ÿ� �˸�
			AddGameChatViewEdit(csMsg, RGB(199, 117, 116)) ;
		}
		break;
	default:
		return FALSE;
	}

	return FALSE;
}

CString CGameNewPoker::GetLobbyEnterSound(bool bMan)
{
	if (bMan) return ".\\NewPoker\\sound\\enterlobby_m.wav";
	return ".\\NewPoker\\sound\\enterlobby_f.wav";
}

LRESULT CGameNewPoker::OnSetPlayRakeBackGold(WPARAM &wParam, LPARAM &lParam)
{
	GetGameView()->m_cPlayerDrawManager.SetPlayRakeBackGold((INT64)wParam, (int)lParam);
	return TRUE;
}

LRESULT CGameNewPoker::OnSetPlayRakeBackGoldEvent(WPARAM &wParam, LPARAM &lParam)
{
	GetGameView()->m_cPlayerDrawManager.SetPlayRakeBackGoldEvent((int)wParam);
	return TRUE;
}

LRESULT CGameNewPoker::OnClearJackPotMoney(WPARAM &wParam, LPARAM &lParam)
{
	int  nGameCode     = *((int *)wParam);
	INT64 llLimitMoney = *((INT64 *)lParam);	

	GAME()->ClearJackpotMoney(nGameCode, llLimitMoney);

	return TRUE;
}

LRESULT CGameNewPoker::OnGetResultOpen( WPARAM &wParam, LPARAM &lParam )
{
	if (g_pGameView==NULL)
		return FALSE;

	return g_pGameView->m_cPlayerDrawManager.IsOpen();
	//return TRUE;
}

const HICON CGameNewPoker::GetGameIcon(void)
{
	return AfxGetApp()->LoadIcon(IDI_ICON_NEWPOKER);
}

const HICON CGameNewPoker::GetChatIcon(void)
{
	return AfxGetApp()->LoadIcon(IDI_ICON_CHAT_NEWPOKER);
}

const CString	CGameNewPoker::GetModuleName(void)
{
	char szFileName[MAX_PATH+1]={0,};
	GetModuleFileName(g_hInstGameDLL, szFileName, MAX_PATH);	
	return CString(szFileName);
}


