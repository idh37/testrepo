#include "StdAfx.h"
#include "GameManager.h"
#include "CommMsgDef_Game.h"

CGameManager::CGameManager(void)
:	m_pCurGame(NULL),
	m_nCurGameType(IDX_GAME_NULL),
	m_nMaxRoom(0),
	m_nMaxPlayer(0),
	m_nMaxGamePlayer(0),
	m_nMaxObserver(0),
	m_nMaxNewPlayer(0),
	m_nJokboQuestCount(0),
	m_nTabState(0),
	m_nTabIndex(0),
	m_nPlayTimeSec(0),
	m_nEventMissionStep(0),
	m_nWhere(IDX_GAMEWHERE_FIRST),
	m_bAbnormalOutRoom(false),
	m_bWaitResponse(false),
	m_hMutex(NULL),
	m_bNowLogout(false),
	m_pMainWnd(NULL),
	m_pMyInfo(NULL),
	m_nLoadingType(EGLT_FIRST),	
	m_nGoDirectEnterType(CHOICE_GAME),
	m_nRoomNum_for_Join(0),
	m_llRoomKey_for_Join(0),
	m_bObserve_for_Join(false),
	m_nCurChanNo(-1),
	m_bUseRoomOption(false),
	m_bForceLocalServer(false),
	m_bShowLoginDlg(false),
	m_bQuickJoinForRandom(false),
	m_bSelectQuickJoin(false),
	m_quickJoinOtherRoom(false),
	m_bRequestOtherChannel(false),
	m_enumProfileOpenKind(EPOK_NONE),
	m_bDiaOption(false),
	m_bShowEndEventPopup(false),
	m_nShowFullExplain(0),
	m_nShowExplainNum(0),
	m_bNotBanishment(false),
	m_bEnableOver(false),
	m_bCannotPlayUpp(false),
	m_nMyGold(0),
	m_nGoldRemain(0),
	m_nGoldValue(0),
	m_Club_MemberGrade(CLUB_MEMBER_GRADE_NONE),
	m_Club_Msg(""),
	m_strDayCheck(""),
	m_strCafeNotice(""),
	// 응모권으로 응모할 수 있는 황금 복주머니 상품 리스트
	m_ArrayTicketGiftList_gold(NULL), //등록된 상품 정보
	// 해당 유저의 티켓 응모 현황
	m_MyTicketCount(0), //가지고 있는 응모권(티켓) 갯수 == 나의 황금 복주머니 갯수
	m_ArraySubscriptionList_gold(NULL), //등록된 응모 정보
	m_refreshPopup(false), // 팝업 새로고침
	m_bPlayDailyGoldEffect(false),
	m_bPlayGoldBigWheel(false),
	m_bWaitOwnLimitPopup(false),
	m_bWaitGoldInsuPopup(false),
	m_bQuickStartInRoom(false),
	m_bOverMoneyGB(false),
	m_nInsuLevel(0),
	m_nAttendanceShowCount(0),
	m_llcafeReservedPoint(0),
	m_nMorningCheckDay(0),
	m_nMorningBanCount(0),
	m_bMorningBet(false),
	m_strMorningBetNotice(""),
	m_bShowUppPopup(false),
	m_bShowChuSeokPopup(false)
{
#ifdef _DEBUG
	HINSTANCE hInstance = GetModuleHandle("UPGCommon_D.dll");
#else
	HINSTANCE hInstance = GetModuleHandle("UPGCommon.dll");
#endif
	m_hCurSel = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_SELECT));
	m_hCurHand = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_HAND));
	m_hCurMove = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_MOVE));
	m_hCurSize = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_ADJUSTSIZE));
	m_hCurWait = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_WAIT));

	
	ZeroMemory(&m_RoomOption, sizeof(m_RoomOption));
	m_bDeafultCreateRoom=false;

	//규제안 : 방옵션 
	ZeroMemory(&m_RoomOptionForRefuse, sizeof(m_RoomOptionForRefuse));

	//규제안 : 최초 맥스 활성 여부 초기화
	for (int i=0; i<MAX_PLAYER; i++)
	{
		g_bFirstMax[i] = false;
	}

	for (int i=0; i<6; i++)
	{
		m_CheckLossMoneyOver[i] = false;
	}
	for (int i=0; i<6; i++)
	{
		m_llLimitLoseMoney[i] = 0ll;
		m_bReceiveLimitMoney[i] = false;
	}
	m_tBlockTime=time_t(0);
	m_tPromotionData.Clear();
	ZeroMemory(&m_RETENTION_ALRAM, sizeof(m_RETENTION_ALRAM));

	MM().AddMsgH(UPG_WEB_CALL_LIST, Fnt(this, &CGameManager::OnEndWebCall));
}

CGameManager::~CGameManager(void)
{
	Release();

	if(m_pMyInfo) delete m_pMyInfo;
	m_pMyInfo = NULL;

}

void CGameManager::Release()
{
	map<IDX_GAME, CGame *>::iterator start = m_listGame.begin();
	map<IDX_GAME, CGame *>::iterator end = m_listGame.end();

	for(;start != end;++start)
	{
		delete start->second;
	}

	m_listGame.clear();
}

CGameManager& CGameManager::GetInstance()
{
	static CGameManager s_cGameManager;
	return s_cGameManager;
}

CGameManager& GM()
{
	return CGameManager::GetInstance();
}

CGame *CGameManager::GetGame(IDX_GAME nGame)
{
 	if (nGame<0 || nGame>=MAX_GAME)
	{
		// poker tournament 테스트 도중 assert 때문에 테스트가 안되서 일단은 주석처리...2011. 10. 10 최종학
		//ASSERT(0);
		return NULL;
	}

	map<IDX_GAME, CGame *>::iterator findIt = m_listGame.find(nGame);
	
	if (findIt == m_listGame.end())
		return NULL;
//	ASSERT(findIt != m_listGame.end());
	return findIt->second;
}

void CGameManager::SetMyInfo(CPlayer *pPlayer)
{
	ASSERT(pPlayer != NULL);
	if(!pPlayer)
		return;

	bool bSetUserInfo = false;
	USERINFO cUI;
	if(m_pMyInfo)
	{
		bSetUserInfo = true;
		memcpy(&cUI, &m_pMyInfo->UI, sizeof(USERINFO));
		delete m_pMyInfo;
	}
	m_pMyInfo = pPlayer;
	if(bSetUserInfo)
	{
		m_pMyInfo->SetNewPlayer(&cUI);
		CPlayer *pPlayer2 = m_pCurGame->GetGameProcess()->GetPlayer(0);
		pPlayer2->SetNewPlayer(&cUI);
	}
}


bool CGameManager::IsDiaMemberRooms(void)
{
#if defined(RESTRICTION_LEVEL)
	if((m_pMyInfo->UI.GetRealPMoney() >= MONEY_SEPERATOR_FOR_VIP_DIA)&&(m_pMyInfo->UI.PremMenuNo == CODE_NEW_FAMILY_DIAMOND)){
		return true;
	}
#endif
	return false;
}


bool CGameManager::IsDiaMemberRooms(__int64 llEnterLimitMoney)
{
#if defined(RESTRICTION_LEVEL)
	if((m_pMyInfo->UI.GetRealPMoney() >= MONEY_SEPERATOR_FOR_VIP_DIA)
		&&(MONEY_SEPERATOR_FOR_VIP_DIA<=llEnterLimitMoney)
		&&(m_pMyInfo->UI.PremMenuNo == CODE_NEW_FAMILY_DIAMOND)){
		return true;
	}
#endif
	return false;
}

bool CGameManager::IsChangeFullDDang(void)
{
#if defined(_GAMECHANNEL_CHANGE)
	bool bReturn=false;
	switch(m_nCurGameType){
	case IDX_GAME_SP: return true;
	case IDX_GAME_BD: return true;
	case IDX_GAME_NP: return true;
	case IDX_GAME_HL: return true;
		//case IDX_GAME_SD: return jackpot::GAME_SUTDA;
	case IDX_GAME_HA: return false;
	}
	return false;
#else
	return ((IDX_GAME_SP==m_nCurGameType)||(IDX_GAME_BD==m_nCurGameType));
#endif
}

bool CGameManager::IsShowFullExplain(void)
{
	if(false==IsChangeFullDDang()) return false;
	return ((SHOWFULLEXPLAIN_MAX>m_nShowFullExplain)?(true):(false));
}

void CGameManager::LoadShowFullExplain(void)
{
	m_nShowFullExplain=NMBASE::UTIL::GetProfile_int(GM().GetMyInfo()->UI.ID, "ShowFullExplain", m_nShowFullExplain);
	char* pBuffer=NMBASE::UTIL::GetProfile_string(GM().GetMyInfo()->UI.ID, "ShowFullExplainday","20000101000000");
	if(true==GM().CheckDiffTime(string(pBuffer))){
		m_nShowFullExplain=0;
	}
}

void CGameManager::WriteShowFullExplain(void)
{
	m_nShowFullExplain++;
	if(SHOWFULLEXPLAIN_MAX<=m_nShowFullExplain){
		m_nShowFullExplain=SHOWFULLEXPLAIN_MAX;
	}
	NMBASE::UTIL::WriteProfile_string(GM().GetMyInfo()->UI.ID, "ShowFullExplainday", GM().GetCurrentTimeString().c_str());
	NMBASE::UTIL::WriteProfile_int(GM().GetMyInfo()->UI.ID, "ShowFullExplain", m_nShowFullExplain);
}


void CGameManager::AddGame(CGame *pGame)
{
	ASSERT(pGame != NULL);
	if(!pGame)
		return;

	IDX_GAME nGame = pGame->GetGameType();
	ASSERT(m_listGame.find(nGame) == m_listGame.end());
	m_listGame[nGame] = pGame;
	pGame->Create((CWnd *)GetMainWnd());
	pGame->GetGameView()->ShowWindow(SW_HIDE);
}

void CGameManager::SetCurrentGame(IDX_GAME nGame)
{
	if(m_nCurGameType == nGame) return;

	ASSERT(m_listGame.find(nGame) != m_listGame.end());

	bool bLoad = (m_pCurGame == NULL)?true:false;

	if(m_pCurGame)
	{
		m_pCurGame->OnFocusGame(false);
	}

	m_pCurGame			= m_listGame[nGame];
	m_nCurGameType		= nGame;
	m_nMaxRoom			= m_pCurGame->GetMaxRoom();
	m_nMaxPlayer		= m_pCurGame->GetMaxPlayer();
	m_nMaxGamePlayer    = m_pCurGame->GetMaxGamePlayer();
	m_nMaxObserver		= m_pCurGame->GetMaxObserver();
	m_nMaxNewPlayer		= m_pCurGame->GetMaxNewPlayer();
	m_nJokboQuestCount	= m_pCurGame->GetJokboQuestCount();
	SetMyInfo(m_pCurGame->CreateNewPlayer());
	GetMyInfo()->UI.nGameIndex = nGame;		//플레이어의 게임인덱스를 바꿔줌
	m_pCurGame->OnFocusGame(true);

	if(bLoad)
	{
		ClearLoadImageList();
		m_pCurGame->LoadSprite();
		m_pCurGame->LoadSound();
	}
}

bool CGameManager::CheckLoadGame(void)
{
	ASSERT(m_pCurGame != NULL);

	if(!m_pCurGame)
		return false;

	map<IDX_GAME, CGame *>::iterator start = m_listGame.begin();
	map<IDX_GAME, CGame *>::iterator end = m_listGame.end();

	for(;start != end;start++)
	{
		if(start->first != m_nCurGameType)
		{
			if(start->second->IsLoadSprite()) start->second->ReleaseSprite();
			if(start->second->IsLoadSound()) start->second->ReleaseSound();
		}
	}

	if(!m_pCurGame->IsLoadSound()) m_pCurGame->LoadSound();
	if(!m_pCurGame->IsLoadSprite())
	{
		ClearLoadImageList();
		m_pCurGame->LoadSprite();
		LoadingImageList(0, FALSE);
		return false;
	}
	return true;
}

LONG CGameManager::OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData)
{
	switch(Signal)
	{
	case SV_NEW_UPPINFO:
		{
			CSV_NEW_UPPINFO msg;
			msg.Get(lpData, TotSize);
			if ((*msg.m_State) == 0)
			{
				int params[3] = {*msg.m_Level, *msg.m_PlayTime, *msg.m_BillingMoney};
				if ((*msg.m_Level) >= 3)
				{
					m_bCannotPlayUpp = true;
				}
				SetClearDateStr( msg.m_EndDate );

				int nLen = sizeof(params) / sizeof(*params);

				for (int i = 0; i < nLen; i++)
				{
					m_nUppWParam[i] = params[i];
				}

				m_cUppLParam = msg.m_UserCN;
								
				// 팝업을 로그인 하는 부분에서 띄움.
				// -> Lobby.cpp 에서 로그인 패킷 받는 부분
				m_bShowUppPopup = true;
			}
		}
		break;
	case SV_DAILYLOSSLIMIT_USER_SETTING_INFO:
		{
			CSV_DAILYLOSSLIMIT_USER_SETTING_INFO msg;
			msg.Get(lpData, TotSize);
			m_bEnableOver = *msg.m_EnableOver;
			if (m_bEnableOver == true && !IsLimitOver())
			{
				MM().Call(UPG_CMK_SHOW_RESTRICTION_PLAYOVER, FALSE);
			}
			else if(getRoomListTabState() != 2 && (GM().IsGetLimitMoney(GetCurrentGameArray()) || GM().IsLimitOver()))
			{
				MM().Call(UPG_CMK_SHOW_RESTRICTION_PLAYOVER, TRUE);
			}
		}
		break;
	case SV_REWARD_DAILYLOSS_OVER:
		{
			CSV_REWARD_DAILYLOSS_OVER msg;
			msg.Get(lpData, TotSize);
			m_rewardTime = *msg.m_BasicDate;
			//MM().Call(UPG_CMK_SHOW_REWARD_DAILYLOSS_OVER);
		}
		break;
	case SV_ETCCHIP_INFO:
		{
			CSV_ETCCHIP_INFO msg;
			msg.Get(lpData, TotSize);

			if (strcmp(msg.m_ChipType, "GD") == 0)
			{
				m_nMyGold = *msg.m_RealChip;
				m_nGoldValue = *msg.m_ChipValue;

				MM().Call(UPG_CMK_SET_MONEY_NUM);
			}
			else if (strcmp(msg.m_ChipType, "GB") == 0)
			{
				m_nGoldRemain = *msg.m_RemainChip;
			}

			// 골드 적립 통장 출금 팝업에 패킷 전달
			CGoldWithdrawPopup *pPopup = NULL;
			MM().Call(UPG_CMK_GET_GOLDWITHDRAW_POPUP, (WPARAM)0, (LPARAM)&pPopup);
			pPopup->HandleResponse(Signal, TotSize, lpData);
		}
		break;
	case SV_ETCCHIP_CHANGE_INFO:
		{
			//응모 결과를 리턴해 준다.
			CSV_ETCCHIP_CHANGE_INFO msg;
			msg.Get(lpData, TotSize);

			if (strcmp(msg.m_ChipType, "GD") == 0)
			{
			}
			else if (strcmp(msg.m_ChipType, "GB") == 0)
			{
				m_ThisAddTempGold = *msg.m_ChangeChip; //화면에 보여주는 정수 부분만 필터링된 변화금액이다.
				m_CurTempGold += m_ThisAddTempGold;
			}
		}
		break;
	case SV_NOTIFY_CHANGEOVERMONEY:
		{
			//응모 결과를 리턴해 준다.
			CSV_NOTIFY_CHANGEOVERMONEY msg;
			msg.Get(lpData, TotSize);

			int nChipKind = *msg.m_ChipKind;

			// 내 보유한도가 초과했을 경우
			if (strncmp(msg.szID, GM().GetMyInfo()->UI.ID, 15) == 0)
			{
				if (nChipKind == ROOM_CHIPKIND_GOLD_BANK)
				{
					m_bOverMoneyGB = true;
				}
			}
		}
		break;
	case SV_ETCCHIP_RAKEBACK_INFO:
		{
			CSV_ETCCHIP_RAKEBACK_INFO msg;
			msg.Get(lpData, TotSize);

			if(strcmp(msg.m_ChipType, "GB")==0)
			{
				MM().Call(UPG_CMK_SET_PLAY_RAKEBACK_GOLD,(WPARAM)*msg.m_ChangeChip_Int,(LPARAM)*msg.m_ChangeChip_Prime);
				int bounsRate = (int)*msg.m_BonusRate;
				if(bounsRate > 1)
					MM().Call(UPG_CMK_SET_PLAY_RAKEBACK_GOLD_EVENT,(WPARAM)bounsRate);
			}
		}
		break;
	case SV_CHANGE_ETCCHIP_RESULT:
		{
			CSV_CHANGE_ETCCHIP_RESULT msg;
			msg.Get(lpData, TotSize);

			if(*msg.m_Result==0)
			{
				if(strcmp(msg.m_ChipType, "GD")==0)
				{
					//MM().Call(UPG_CMK_SET_MONEY_NUM);
					//CGoldChangeDlg *pGoldChangeDlg = NULL;
					//MM().Call(UPG_CMK_GET_GOLDCHANGE_DLG,(WPARAM)0,(LPARAM)&pGoldChangeDlg);
					//pGoldChangeDlg->Init();
				}
			}
		}
		break;
	case SV_ETCCHIP_OPEN_REWARD:
		{
			//CEventInfopopup dlg(GM().GetMainWnd());
			//dlg.DoModal();
		}
		break;
	case SV_CAFE_ASK_JOIN:
	case SV_CAFE_INVITE:
		{
			CProfileDlg *pProfileDlg = NULL;
			MM().Call(UPG_CMK_GET_PROFILE_DLG,(WPARAM)0,(LPARAM)&pProfileDlg);
			pProfileDlg->HandleResponse(Signal, TotSize, lpData);
		}
		break;
	case SV_CAFE_NOTICE:
		{
			bool bAfterLogin = m_strCafeNotice.GetLength() != 0;
			CSV_CAFE_NOTICE msg;
			msg.Get(lpData, TotSize);
			if (*msg.noticeType_ == CafeSystem::Notice::VIPGRADE_UP_CAFE_BOSS_MORETHAN_4_GRADE)
			{
				m_strCafeNotice.Format( "VIP 등급이 상승하여 카페의 정원이 증가 하였습니다.\nVIP등급이 높을수록 더욱 많은 카페원을 모집할 수 있습니다." );
			}
			else if(*msg.noticeType_ == CafeSystem::Notice::VIPGRADE_UP_UNITEDCAFE_BOSS_MORETHAN_6_GRADE)
			{
				m_strCafeNotice.Format( "VIP 등급이 상승하여 연합원 보유수가 증가 하였습니다.\nVIP등급이 높을수록 더욱 많은 연합원을 모집할 수 있습니다." );
			}
			else if(*msg.noticeType_ == CafeSystem::Notice::VIPGRADE_DOWN_CAFE_BOSS)
			{
				m_strCafeNotice.Format( "VIP 등급이 하락하여 카페의 정원이 감소 하였습니다.\n정원을 초과한 카페원은 접속일자가 오래된 유저부터 탈퇴처리 됩니다.\nVIP 등급이 높을수록 더욱 많은 카페원을 모집할 수 있습니다." );
			}
			else if(*msg.noticeType_ == CafeSystem::Notice::VIPGRADE_DOWN_UNITEDCAFE_BOSS)
			{
				m_strCafeNotice.Format( "VIP 등급이 하락하여 연합의 정원이 감소 하였습니다.\n정원을 초과한 연합원은 접속일자가 오래된 유저부터 탈퇴처리 됩니다.\nVIP 등급이 높을수록 더욱 많은 카페장을 모집할 수 있습니다." );
			}
			else if(*msg.noticeType_ == CafeSystem::Notice::FIRED_CAFE_MEMBER)
			{
				m_strCafeNotice.Format( "카페장 VIP 등급이 하락하여 자동탈퇴 되었습니다.\n정원을 초과한 카페원은 접속일자가 오래된 유저부터 탈퇴처리 됩니다." );
			}
			else if(*msg.noticeType_ == CafeSystem::Notice::FIRED_UNITEDCAFE_MEMBER)
			{
				m_strCafeNotice.Format( "연합장 VIP 등급이 하락하여 자동탈퇴 되었습니다.\n정원을 초과한 연합원은 접속일자가 오래된 유저부터 탈퇴처리 됩니다." );
			}
			else
			{
				break;
			}

			if(bAfterLogin)
			{
				ShowMainMessageDlg(m_strCafeNotice);
			}
		}
		break;
	case SV_CAFE_RESERVED_POINT:
		{
			CSV_CAFE_RESERVED_POINT msg;
			msg.Get(lpData, TotSize);
			SetCafeReservedPoint(*msg.cafePoint_);
		}
		break;
	case SV_CLUB_RESPONSE_DETAIL_INFO:
	case SV_CLUB_RESPONSE_INVITED_LIST:
	case SV_CLUB_RESPONSE_JOIN_LIST:
	case SV_CLUB_RESPONSE_MYSELF_INVITED_LIST:
	case SV_CLUB_RESPONSE_MYSELF_JOIN_LIST:
		{
			if(GetIsShowingClubDlg()==true)
			{
				CClubDlg *pClubDlg = NULL;
				MM().Call(UPG_CMK_GET_CLUB_DLG,(WPARAM)0,(LPARAM)&pClubDlg);
				pClubDlg->HandleResponse(Signal, TotSize, lpData);
			}
		}
		break;
	case SV_CLUB_MYINFO:
		{
			CSV_CLUB_MYINFO msg;
			msg.Get(lpData, TotSize);
			
			m_Club_ID = *msg.m_Club_ID;
			m_Club_Name = msg.m_Club_Name;
			m_Club_MemberGrade = *msg.m_Club_MemberGrade; 
			m_Club_Enable_Create = *msg.m_Club_Enable_Create;
			m_Member_AddGet_Rate = *msg.m_Member_AddGet_Rate;
			m_Boss_PayBack_Rate = *msg.m_Boss_PayBack_Rate;
			
			//// 가입, 탈퇴 등으로 유저의 클럽 관련 정보가 변경되면 SV_CLUB_MYINFO 패킷이 온다.
			//// 이때 변경된 정보를 세팅 해줘야함.
			/*
			// 해산
			if (m_pMyInfo->UI.ClubCode > 0 && *msg.m_Club_ID == 0)
			{
				// 로비
				if (GetCurrentWhere() == IDX_GAMEWHERE_LOBY && GetIsShowingClubDlg()==false)
				{
					m_Club_Msg = "클럽에서 탈퇴되었습니다.\n클럽에 가입하시면 클럽혜택을 받을 수 있습니다.";
					ShowMainMessageDlg(m_Club_Msg.GetString());
				}
				// 인게임
				else if (GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				{
					CString csMsg = "클럽에서 탈퇴되었습니다.\n다음 판부터 클럽 혜택을 받으실 수 없습니다.";
					AddGameInfoViewEdit(csMsg.GetString(), RGB(143, 141, 102));
				}
			}
			
			// 강제 탈퇴
			else 
			*/
			if (m_pMyInfo->UI.ClubMemberGrade == CLUB_MEMBER_GRADE_NORMAL &&
				*msg.m_Club_MemberGrade == CLUB_MEMBER_GRADE_NONE &&
				*msg.m_InfoType == CLUB_MYINFO_OTHER_REQUEST)
			{
				// 로비
				if (GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
				{
					m_Club_Msg = "클럽에서 탈퇴되었습니다. 클럽에 가입하시면 클럽혜택을 받을 수 있습니다.";
					ShowMainMessageDlg(m_Club_Msg);
					m_Club_Msg = "";

					if(GetIsShowingClubDlg()==true)
					{
						CClubDlg *pClubDlg = NULL;
						MM().Call(UPG_CMK_GET_CLUB_DLG,(WPARAM)0,(LPARAM)&pClubDlg);
						pClubDlg->CloseDlg();
					}
				}
				// 인게임
				else if (GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				{
					m_Club_Msg = "클럽에서 탈퇴되었습니다. 클럽에 가입하시면 클럽혜택을 받을 수 있습니다.";
					CString csMsg = "클럽에서 탈퇴되었습니다. 더이상 클럽 혜택을 받으실 수 없습니다.\n";
					AddGameInfoViewEdit(csMsg.GetString(), RGB(143, 141, 102));
				}
			}
			// 가입 승인 완료
			else if (m_pMyInfo->UI.ClubMemberGrade == CLUB_MEMBER_GRADE_NONE &&
				*msg.m_Club_MemberGrade == CLUB_MEMBER_GRADE_NORMAL &&
				*msg.m_InfoType == CLUB_MYINFO_OTHER_REQUEST)
			{
				// 로비
				if (GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
				{
					m_Club_Msg = "클럽에 가입되었습니다. 자세한 내용은 클럽 아이콘을 클릭하여 확인이 가능합니다.";
					ShowMainMessageDlg(m_Club_Msg);
					m_Club_Msg = "";
					
					if(GetIsShowingClubDlg()==true)
					{
						CClubDlg *pClubDlg = NULL;
						MM().Call(UPG_CMK_GET_CLUB_DLG,(WPARAM)0,(LPARAM)&pClubDlg);
						pClubDlg->CloseDlg();
					}
				}
				// 인게임
				else if (GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				{
					m_Club_Msg = "클럽에 가입되었습니다. 자세한 내용은 클럽 아이콘을 클릭하여 확인이 가능합니다.";
					CString csMsg = "클럽에 가입되었습니다. 클럽 혜택을 받으실 수 있습니다.\n";
					AddGameInfoViewEdit(csMsg.GetString(), RGB(143, 141, 102));
				}
			}
			
			// 클럽 코드
			m_pMyInfo->UI.ClubMemberGrade = *msg.m_Club_ID;
			// 클럽 이름
			strncpy(m_pMyInfo->UI.ClubName, msg.m_Club_Name, sizeof m_pMyInfo->UI.ClubName - 1);
			m_pMyInfo->UI.ClubName[sizeof m_pMyInfo->UI.ClubName - 1] = '\0';
			// 클럽 맴버 등급(보스, 일반, 미가입)
			m_pMyInfo->UI.ClubMemberGrade = *msg.m_Club_MemberGrade;
			
			// 클럽 미가입
			if (*msg.m_Club_MemberGrade == CLUB_MEMBER_GRADE_NONE)
			{
				// 클럽 탭이 선택되었을 경우 
				if ((ENUM_SHOW_USERLIST_TYPE)MM().Call(UPG_CMK_GET_USER_LIST_TAB_TYPE) == ESUT_CLUB)
				{
					// 친구 탭 선택
					MM().Call(UPG_CMK_SET_USER_LIST_TAB_TYPE, ESUT_MY_BUDDY);
				}
				
				// 클럽 탭 비활성화
				MM().Call(UPG_CMK_SET_USER_LIST_TAB_ACTIVE, (WPARAM)1, (LPARAM)FALSE);
			}
			else
			{
				MM().Call(UPG_CMK_SET_USER_LIST_TAB_ACTIVE, (WPARAM)1, (LPARAM)TRUE);
			}
		}
		break;
	case SV_CLUB_CREATE_RESULT:
	case SV_CLUB_ACCEPT_INVITE_RESULT:
	case SV_CLUB_WITHDRAW_RESULT:
	case SV_CLUB_MEMBER_FIRE_RESULT:
	case SV_CLUB_MEMBER_FIRE_NOTIFY:
	case SV_CLUB_ACCEPT_JOIN_RESULT:
		{
			CClubDlg *pClubDlg = NULL;
			MM().Call(UPG_CMK_GET_CLUB_DLG,(WPARAM)0,(LPARAM)&pClubDlg);
			pClubDlg->ShowPopupResultMsg(Signal, TotSize, lpData);
		}
		break;
	case SV_CLUB_INVITE_RESULT:
		{
			CString str;
			//0.성공, -1001.클럽정보오류, -1002.가입제한인원초과, -1003.이미초대한유저, 
			//-1004.초대자가 다른클럽 보스임, -1005.초대자가 다른 클럽에 가입중, -1006.존재하지 않는 유저임
			CSV_CLUB_INVITE_RESULT msg;
			msg.Get(lpData, TotSize);
			if (*msg.m_Result==0)
			{
				ShowMainMessageDlg("클럽 초대를 성공적으로 전송하였습니다.");
				
				CCL_CLUB_REQUEST_MYSELF_JOIN_LIST rMsg;
				rMsg.Set(-1);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), rMsg.pData, rMsg.GetTotalSize());
			}
			else if (*msg.m_Result==-1001)
			{
				str.Format( "잘못된 클럽 정보입니다." );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1002)
			{
				str.Format( "1일 초대 10건 제한을 초과하였습니다. 익일 00시부터 초대가 가능합니다." );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1003)
			{
				str.Format( "%s님은 이미 초대중입니다.", msg.m_InviteID);
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1004)
			{
				str.Format( "해당 사용자는 클럽 보스입니다.\n다른 분을 초대하시기 바랍니다." );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1005)
			{
				str.Format( "해당 사용자가 클럽에 가입되어 있습니다." );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1006)
			{
				str.Format( "해당 사용자가 존재하지 않습니다.\n정확한 닉네임을 입력해주세요." );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1007)
			{
				str.Format( "오늘 클럽에서 탈퇴한 유저입니다.\n00시 이후에 다시 시도해주세요." );
				ShowMainMessageDlg(str);
			}
			else
			{
				str.Format( "클럽 초대를 실패했습니다." );
				ShowMainMessageDlg(str);
			}
		}
		break;
	case SV_CLUB_JOIN_RESULT:
		{
			CString str;
			//0.성공, -1000.이미가입된 클럽이 있음, -1001.요청수 제한을 초과함 -1002.이미 해당클럽에 가입신청을 했음
			CSV_CLUB_JOIN_RESULT msg;
			msg.Get(lpData, TotSize);
			if (*msg.m_Result==0)
			{
				ShowMainMessageDlg("클럽 가입 요청을 완료하였습니다.");
			}
			else if (*msg.m_Result==-1000)
			{
				str.Format( "이미 다른 클럽에 가입중입니다." );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1001)
			{
				str.Format( "1일 가입요청 10건 제한을 초과하였습니다. 익일 00시부터 가입요청이 가능합니다." );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1002)
			{
				str.Format( "가입신청은 클럽당 하루에 한 번 가능합니다." );
				ShowMainMessageDlg(str);
			}
			else
			{
				str.Format( "클럽 가입 요청을 실패하였습니다.");
				ShowMainMessageDlg(str);
			}
		}
		break;
	case SV_RESPONSE_PLAYTIMEINFO:
		{
			// 플레이 시간을 받아올 때(인게임에서 로비로 나올때) 클럽 등급 변화에 대한 메시지를 띄운다.
			if (m_Club_Msg.IsEmpty() == false)
			{
				ShowMainMessageDlg(m_Club_Msg);
				m_Club_Msg = "";
			}
			if(GetPromotionData()->nCnts[0] == 1)
			{
				CLobbyEventDlg dlg(GM().GetMainWnd(), EVENT_CHICKENREWARD);
				dlg.DoModal(getGameURL(IDX_GAMEURL_CHICKEN_REWARD));
				GetPromotionData()->nCnts[0] = 0;
			}
		}
		break;
	// 2018 설 이벤트 출석 관련
	case SV_PROMOTION_COMMON_ATTENDANCE_GET_REWARD:
		{
			CSV_PROMOTION_COMMON_ATTENDANCE_GET_REWARD msg;
			msg.Get(lpData, TotSize);
			
			int m_Result = *msg.m_Result; //0.정상지급, 1.해당유저의 DI가 같은 다른 계정이 이미 수령한 보상임
			int m_Attendance_ID = *msg.m_Attendance_ID; //출석부 ID
			int m_Attendance_Type = *msg.m_Attendance_Type; //출석형태, enum PROMOTION_ATTENDANCE_TYPE 에 있는 값이며 ATTENDANCE_NORMAL는 일반누적, ATTENDANCE_CONTINUE는 연속출석
			int m_Attendance_Count = *msg.m_Attendance_Count; //출석일자
			char m_RewardType[CHAR_BUFFER_PROMOTION_COMMON_ATTENDANCE_REWARDTYPE]; 
			memcpy(m_RewardType, msg.m_RewardType, CHAR_BUFFER_PROMOTION_COMMON_ATTENDANCE_REWARDTYPE);
			//GM.게임머니, GD골드, CP.쿠폰, DC.할인권, MBM.멤버스미니, MBS.멤버스 실버, MBG.멤버스골드, MBD.멤버스다이아, GLP.황금복주머니
			INT64 m_RewardAmount = *msg.m_RewardAmount; //보상금액.. 만일 멤버스 그런거면 일자라고 보면 됨
			
			if (m_Result == 0)
			{
				if(m_Attendance_Type == (int)ATTENDANCE_NORMAL)
				{
					CString str;
					if(strcmp(m_RewardType, "GM")==0)
					{
						str.Format("%d일차 보상 %s%s이 지급되었습니다.", m_Attendance_Count, NumberToOrientalString(m_RewardAmount), strChip_Name[(int)ROOM_CHIPKIND_NORMAL]);
					}
					else if(strcmp(m_RewardType, "GD")==0)
					{
						str.Format("%d일차 보상 %s%s이 지급되었습니다.", m_Attendance_Count, NumberToOrientalString(m_RewardAmount), strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
					}
					str.Append(m_strDayCheck);
					ShowMainMessageDlg(str.GetString());
				}
				else if (m_Attendance_Type == (int)ATTENDANCE_CONTINUE)
				{
					m_strDayCheck.Format("\n연속%d일 접속하여 %s%s이 지급되었습니다.", m_Attendance_Count, NumberToOrientalString(m_RewardAmount), strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
				}
			}
			else if (m_Result == 1)
			{
				ShowMainMessageDlg("멀티 계정 중에서 이미 출석 보상을 받은 계정이 있습니다. 출석 보상은 주민등록번호당 1개의 계정으로만 수령이 가능합니다.");
			}
		}
		break;
	// 2018 설 이벤트 출석 관련
	case SV_PROMOTION_COMMON_ATTENDANCE_USERINFO:
		{
			CSV_PROMOTION_COMMON_ATTENDANCE_USERINFO msg;
			msg.Get(lpData, TotSize);
			
			bool bNewDay = *msg.m_NewDayFlag; //이 값이 true이면 이번 접속이 오늘은 처음이라는 의미(보상 지급하는 타이밍이란 의미이다.)
			
			// 오늘 처음 접속이 아니면 출석 팝업을 띄우지 않음
			if (bNewDay == false)
				break;
			
			// 설 출석 이벤트 팝업
			MM().Call(UPG_CMK_CREATE_NEWYEARATTENDANCE_POPUP);
			
			CNewYearAttendancePopup *pAttendancePopup = NULL;
			MM().Call(UPG_CMK_GET_NEWYEARATTENDANCE_POPUP, (WPARAM)0, (LPARAM)&pAttendancePopup);
			pAttendancePopup->HandleResponse(Signal, TotSize, lpData);
			
			MM().Call(UPG_CMK_SHOW_NEWYEARATTENDANCE_POPUP);
		}
		break;
	// 2018 설 응모 관련
	case SV_PROMOTION_COMMON_RESPONSE_TICKET_GIFTLIST:
		{
			//응모권으로 응모할 수 있는 상품 리스트
			CSV_PROMOTION_COMMON_RESPONSE_TICKET_GIFTLIST msg;
			msg.Get(lpData, TotSize);
			
			switch(*msg.m_PM_Code)
			{
			// 2018년 황금 복주머니 이벤트
			case PROMOTION_COMMON_GAMECOUNT_PMCODE_2018_GOLDEN_LUCKYBAG:
				{
					int m_Count_gold = *msg.m_Count; //응답갯수

					if (m_Count_gold != 4)
					{
						ShowMainMessageDlg("황금 복주머니 상품 리스트를 가져올 수 없습니다.");
						break;
					}

					m_ArrayTicketGiftList_gold = new TICKET_GIFT_INFO[m_Count_gold];
					
					for (int i = 0; i < m_Count_gold; i ++)
					{
						strncpy(m_ArrayTicketGiftList_gold[i].PM_Name, msg.m_ArrayTicketGiftList[i].PM_Name, sizeof(m_ArrayTicketGiftList_gold[i].PM_Name));
						strncpy(m_ArrayTicketGiftList_gold[i].GiftName, msg.m_ArrayTicketGiftList[i].GiftName, sizeof(m_ArrayTicketGiftList_gold[i].GiftName));
						m_ArrayTicketGiftList_gold[i].GiftIndex = msg.m_ArrayTicketGiftList[i].GiftIndex;
						m_ArrayTicketGiftList_gold[i].NeedTicketCount = msg.m_ArrayTicketGiftList[i].NeedTicketCount;
					}
				}
				break;
			}
		}
		break;
	case SV_PROMOTION_COMMON_TICKET_RESPONSE_SUBSCRIPTION_LIST:
		{
			//해당 유저의 티켓 응모 현황
			CSV_PROMOTION_COMMON_TICKET_RESPONSE_SUBSCRIPTION_LIST msg;
			msg.Get(lpData, TotSize);
			
			switch(*msg.m_PM_Code)
			{
			// 2018년 황금 복주머니 이벤트
			case PROMOTION_COMMON_GAMECOUNT_PMCODE_2018_GOLDEN_LUCKYBAG:
				{
					m_Count_gold = *msg.m_Count; // 응답 갯수
					
					if (m_Count_gold != 4)
					{
						ShowMainMessageDlg("황금 복주머니 응모 현황을 가져올 수 없습니다.");
						break;
						
						/*
						m_MyTicketCount = 0; //가지고 있는 응모권(티켓) 갯수 == 나의 황금 복주머니 갯수
						m_ArraySubscriptionList_gold = new TICKET_SUBSCRIPTION_INFO[4];
						
						for (int i = 0; i < 4; i++)
						{
							m_ArraySubscriptionList_gold[i].GiftIndex = i;
							m_ArraySubscriptionList_gold[i].SB_Count = 0;
						}
						*/
					}
					else
					{
						m_MyTicketCount = *msg.m_MyTicketCount; //가지고 있는 응모권(티켓) 갯수 == 나의 황금 복주머니 갯수
						m_ArraySubscriptionList_gold = new TICKET_SUBSCRIPTION_INFO[m_Count_gold];
						
						for (int i = 0; i < m_Count_gold; i++)
						{
							m_ArraySubscriptionList_gold[i].GiftIndex = msg.m_ArraySubscriptionList[i].GiftIndex;
							m_ArraySubscriptionList_gold[i].SB_Count = msg.m_ArraySubscriptionList[i].SB_Count;
						}
						
						if (m_refreshPopup)
						{
							CGoldLuckyBagPopup *pGoldLuckyBagPopup = NULL;
							MM().Call(UPG_CMK_GET_GOLDLUCKYBAG_POPUP, (WPARAM)0, (LPARAM)&pGoldLuckyBagPopup);
							pGoldLuckyBagPopup->HandleResponse(Signal, TotSize, lpData);
						}
					}
				}
				break;
			}
		}
		break;
	case SV_PROMOTION_COMMON_TICKET_RESPONSE_SUBSCRIPTION:
		{
			//응모 결과를 리턴해 준다.
			CSV_PROMOTION_COMMON_TICKET_RESPONSE_SUBSCRIPTION msg;
			msg.Get(lpData, TotSize);

			switch(*msg.m_PM_Code)
			{
			// 2018년 황금 복주머니 이벤트
			case PROMOTION_COMMON_GAMECOUNT_PMCODE_2018_GOLDEN_LUCKYBAG:
				{
					m_GiftIndex = *msg.m_GiftIndex; //상품 고유번호
					m_ResultCode = *msg.m_ResultCode; //0이면 응모 성공, 1000이면 응모권 부족, 9999면 상품 찾을 수 없음
					m_SB_Count = *msg.m_SB_Count; //해당 상품에 몇번 응모하게 되었는지를 알려줌(응모성공시)

					switch (m_ResultCode)
					{
					case 0:
						{
							ShowMainMessageDlg("응모 성공!");
							// 티켓으로 응모한 현황을 요청한다.
							CCL_PROMOTION_COMMON_TICKET_REQUEST_SUBSCRIPTION_LIST msg;
							msg.Set(PROMOTION_COMMON_GAMECOUNT_PMCODE_2018_GOLDEN_LUCKYBAG);
							NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
						}
						break;
					case 1000:
						ShowMainMessageDlg("응모권이 부족합니다.");
						break;
					case 9999:
						ShowMainMessageDlg("상품을 찾을 수 없습니다.");
						break;
					default:
						ShowMainMessageDlg("알 수 없는 에러 발생.");
						break;
					}
				}
				break;
			}
		}
		break;
		/*
	// 2018 골드 적립금 정보(게임에서 이길때마다 오는 패킷)
	// 본 패킷은 사용 안할 예정이지만 혹시 몰라서 남겨둠(서버쪽도 마찬가지)
	case SV_GOLD_TEMPMONEY_INFO:
		{
			//응모 결과를 리턴해 준다.
			CSV_GOLD_TEMPMONEY_INFO msg;
			msg.Get(lpData, TotSize);
			
			m_CurTempGold = *msg.m_CurTempGold; //현재 보유한 임시 적립된 골드량이다. 이 필드로 현재 적립머니를 동기화 시키면 됨
			m_ThisAddTempGold = *msg.m_ThisAddTempGold; //이번에 변화되는 골드량이다. 
			m_ThisAddTempGold_Remain = *msg.m_ThisAddTempGold_Remain; //이번에 변화되는 골드의 소수점 이하 낙전이다.
			//한게임 종료시 위 두개의 값으로 + 되었다는 UI를 구성하면 됨 (예) Temp.Format("+%I64d.%d", m_ThisAddTempGold, m_ThisAddTempGold_Remain);
			
			m_MaxTempGold = *msg.m_MaxTempGold; //최대로 적립받을 수 있는 금액이다. 이 값과 m_CurTempGold 를 비교하여 최대치 도달등의 UI 구성을 해주면 된다.
		}
		break;
		*/
	// 골드 적립금 팝업
	case SV_GOLD_TEMPMONEY_REAL_UPDATE:
		{
			CSV_GOLD_TEMPMONEY_REAL_UPDATE msg;
			msg.Get(lpData, TotSize);
			
			// 골드 적립금 팝업
			/* 2018-06-05 골드 적립금 팝업을 띄우지 않음.
			MM().Call(UPG_CMK_CREATE_GOLDMILEAGE_POPUP);
			
			CGoldMileagePopup *pPopup = NULL;
			MM().Call(UPG_CMK_GET_GOLDMILEAGE_POPUP, (WPARAM)0, (LPARAM)&pPopup);
			pPopup->HandleResponse(Signal, TotSize, lpData);
			
			MM().Call(UPG_CMK_SHOW_GOLDMILEAGE_POPUP);
			
			m_CurTempGold = 0;
			*/
		}
		break;
	// 골드 지원금 팝업
	case SV_GOLD_HELP_REWARD:
		{
			CSV_GOLD_HELP_REWARD msg;
			msg.Get(lpData, TotSize);

			int nGrade = *msg.m_Grade; //순위.. 이 값이 -1이면 꽝이다...

			// 기획 정책 변경으로 당첨 시에만 팝업을 띄움
			if (nGrade > 0)
			{
				// 팝업생성
				MM().Call(UPG_CMK_CREATE_GOLDSUPPORTFUND_POPUP);

				// 팝업 설정
				CGoldSupportFundPopup *pPopup = NULL;
				MM().Call(UPG_CMK_GET_GOLDSUPPORTFUND_POPUP, (WPARAM)0, (LPARAM)&pPopup);
				pPopup->HandleResponse(Signal, TotSize, lpData);

				// 팝업 띄우기
				MM().Call(UPG_CMK_SHOW_GOLDSUPPORTFUND_POPUP);
			}
		}
		break;
	// 골드 지원금 정보
	case SV_GOLD_HELP_REWARD_INFO:
		{
			CSV_GOLD_HELP_REWARD_INFO msg;
			msg.Get(lpData, TotSize);
			
			m_Playing = *msg.m_Playing; //현재 진행중인지 여부
			m_StartDay = msg.m_StartDay;
			m_EndDay = msg.m_EndDay;

			MM().Call(UPG_CMK_SET_FUNDEVENT_ENABLE);
		}
		break;
	/*
	//금일 홀짝 게임을 진행할 수 있는지를 알려준다.
	case SV_PM_ODDEVEN_START_INFO:
		{
			CSV_PM_ODDEVEN_START_INFO msg;
			msg.Get(lpData, TotSize);
			
			//이 패킷을 true로 받으면 게임진행을 할 수 있다고 판단하여, 서버에 게임 시작 요청 패킷을 보내면 된다.
			bool bStartEnable = *msg.m_StartEnable;
			bool needCaptcha = *msg.m_NeedCaptcha;

			MM().Call(UPG_CMK_SET_NEED_CAPTCHA, (WPARAM)needCaptcha);

			if (bStartEnable)
			{
				MM().Call(UPG_CMK_SET_LOBBY_BUTTON_ENABLE, (WPARAM)true);
			}
		}
		break;
	//게임시작 요청에 대한 결과이다.
	case SV_PM_ODDEVEN_RESPONSE_START:
		{
			CSV_PM_ODDEVEN_RESPONSE_START msg;
			msg.Get(lpData, TotSize);
			
			//이 패킷을 true로 받으면 게임진행을 할 수 있다고 판단하여, 서버에 게임 시작 요청 패킷을 보내면 된다.
			int nErrCode = *msg.m_ErrCode;
			
			switch (nErrCode)
			{
			// 정상 시작
			case 0:
				// 이 경우 SV_PM_ODDEVEN_START_ROUND 패킷이 온다.
				break;
			// 오늘 이미 진행함
			case 1:
				ShowMainMessageDlg("오늘은 더 이상 참여할 수 없습니다.");
				break;
			// 기타 오류
			case 2:
				ShowMainMessageDlg("기타 오류. 관리자에게 문의 주세요.");
				break;
			default:
				break;
			}
		}
		break;
	//당첨자를 전체 유저에게 노티해 준다~!
	case SV_PM_ODDEVEN_WINNER_NOTI:
	//각 라운드가 진행되었음을 알려준다.
	case SV_PM_ODDEVEN_START_ROUND:
	//유저배팅에 대한 결과이다.
	case SV_PM_ODDEVEN_USER_BET_RESULT:
		{
			// 팝업생성
			MM().Call(UPG_CMK_CREATE_GOLDATTENDANCEBOOK_POPUP);
			
			CGoldAttendanceBookPopup *pPopup = NULL;
			MM().Call(UPG_CMK_GET_GOLDATTENDANCEBOOK_POPUP, (WPARAM)0, (LPARAM)&pPopup);
			pPopup->HandleResponse(Signal, TotSize, lpData);
			
			//MM().Call(UPG_CMK_SHOW_GOLDATTENDANCEBOOK_POPUP);
		}
		break;
		*/
	// 금일 칩 출석부를 진행할 수 있는지를 알려준다.
	case SV_PM_WHEEL_START_INFO:
		{
			CSV_PM_WHEEL_START_INFO msg;
			msg.Get(lpData, TotSize);

			//이 패킷을 true로 받으면 게임진행을 할 수 있다고 판단하여, 서버에 게임 시작 요청 패킷을 보내면 된다.
			bool bStartEnable = *msg.m_StartEnable;
			bool needCaptcha = *msg.m_NeedCaptcha;

			//MM().Call(UPG_CMK_SET_NEED_CAPTCHA, (WPARAM)needCaptcha);

			// Test
			//MM().Call(UPG_CMK_SET_LOBBY_BUTTON_ENABLE, (WPARAM)true);

			if (bStartEnable)
			{
				MM().Call(UPG_CMK_SET_LOBBY_BUTTON_ENABLE, (WPARAM)true);
			}
		}
		break;
	// 게임시작 요청에 대한 결과이다.
	case SV_PM_WHEEL_RESPONSE_START:
	//각 라운드가 진행되었음을 알려준다.
	case SV_PM_WHEEL_START_ROUND:
		//유저배팅에 대한 결과이다.
	case SV_PM_WHEEL_USER_BET_RESULT:
		//당첨자를 전체 유저에게 노티해 준다~!
	case SV_PM_WHEEL_WINNER_NOTI:
		{
			// 팝업생성
			MM().Call(UPG_CMK_CREATE_GOLDATTENDANCEBOOK_POPUP);

			CChipAttendPopup *pPopup = NULL;
			MM().Call(UPG_CMK_GET_GOLDATTENDANCEBOOK_POPUP, (WPARAM)0, (LPARAM)&pPopup);
			pPopup->HandleResponse(Signal, TotSize, lpData);
		}
		break;
	case SV_PM_BIGWHEEL_GET_REMAIN_GAMECOUNT:
		{
			CSV_PM_BIGWHEEL_GET_REMAIN_GAMECOUNT msg;
			msg.Get(lpData, TotSize);

			//빅휠 삭제 2018.07.12
			//MM().Call(UPG_CMK_SHOW_GOLDBIGWHEEL_DIALOG, (WPARAM)*msg.m_nRemainCount, (LPARAM)*msg.m_llUsingGold);
		}
		break;
	case SV_PM_BIGWHEEL_PLAY_GAME:
		{
			CSV_PM_BIGWHEEL_PLAY_GAME msg;
			msg.Get(lpData, TotSize);

			CGoldBigWheelDlg *pGoldBigWheelDlg = NULL;
			MM().Call(UPG_CMK_GET_GOLDBIGWHEEL_DLG,(WPARAM)0,(LPARAM)&pGoldBigWheelDlg);
			if (*msg.m_nErrorCode == 0)
			{
				pGoldBigWheelDlg->StartWheel(*msg.m_nRemainCount, *msg.m_llUsedGold, *msg.m_llAddedMoney);
			}
			// 실패시 에러 팝업 생성 후 팝업 닫기. 일반적인 상황에서는 노출되지 않는다.
			else
			{
				if (*msg.m_nErrorCode == 1)
				{
					CString strMsg;
					strMsg.Format("보유%s이 부족하여 게임을 진행할 수 없습니다.", strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
					ShowMainMessageDlg(strMsg);
				}
				else if (*msg.m_nErrorCode == 2)
				{
					ShowMainMessageDlg("진행 가능한 게임 횟수가 부족합니다.\n06시 이후 다시 진행해 주세요.");
				}
				else
				{
					ShowMainMessageDlg("서비스 이용이 제한되었습니다. 다시 접속해 주세요.");
				}
				pGoldBigWheelDlg->ClosePopup();
			}
		}
		break;
	//골드계좌 인출 요청에 대한 결과이다.
	case SV_GOLDBANK_WITHDRAW_RESULT:
		{
			// 골드 적립 통장 출금 팝업에 패킷 전달
			CGoldWithdrawPopup *pPopup = NULL;
			MM().Call(UPG_CMK_GET_GOLDWITHDRAW_POPUP, (WPARAM)0, (LPARAM)&pPopup);
			pPopup->HandleResponse(Signal, TotSize, lpData);
		}
		break;
		//해당 유저의 기타재화 보험금 가입 정보를 보내준다.
	case SV_ETCCHIP_INSURANCE_USER_INFO:
		{
			CSV_ETCCHIP_INSURANCE_USER_INFO msg;
			msg.Get(lpData, TotSize);

			// 패킷이 골드 재화에 대한 정보일 경우(골드 보험)
			if (strcmp(msg.m_ChipType, "GD") == 0)
			{
				m_nInsuLevel = *msg.m_Level; //0이면 미가입 상태
				//m_chArrName = msg.m_Name; //보험명
				//m_i64SaveMoney = *msg.m_SaveMoney; //누적 적립된 금액
			}
		}
		break;
	//해당 유저의 기타재화 보험금 변동 내역을 보내준다.(가입, 지급등)
	case SV_ETCCHIP_INSURANCE_USER_CHANGE_INFO:
		{
			CSV_ETCCHIP_INSURANCE_USER_CHANGE_INFO msg;
			msg.Get(lpData, TotSize);
			
			if (strcmp(msg.m_ChipType, "GD") == 0)
			{
				int nChangeType = *msg.m_ChangeType; //1.가입, 2.지급
				m_ChangeChip = *msg.m_ChangeChip; //변동금액.. 항상 양수임에 주의하자~!!

				//INT64 *m_CurrentChip; //위 처리 이후 보유금액
				m_nInsuLevel = *msg.m_FixedLevel; //위 처리이후 확정된 보험등급
				char *m_chArrName = msg.m_FixedName; //위 처리이후 확정된 보험이름
				INT64 m_i64SaveMoney = *msg.m_FixedSaveChip; //위 처리이후 남아 있는 적립금

				// 골드 보험 가입
				if (nChangeType == 1)
				{
					// 인게임에 알림을 띄우기 위한 변수 세팅
					// 인게임일 경우 인게임 노티에 m_ChangeChip 값을 표시해 주어야 해서 0으로 세팅하면 안됨.
					m_dwGoldInsuNotiStartTime = timeGetTime(); // 인게임 보험 가입 노티 애니메이션 시작 시간
					
					// 로비일경우 웹 팝업을 띄움
					if (GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
					{
						m_ChangeChip = 0; // 골드 보험 가입 완료 > parameter "gold" 를 0 값
						ShowNewAskChargeDlg(NEW_ASK_CHARGE_DLG_TYPE_INSU_GOLD2, NULL);
					}
				}
				// 골드 보험금 지급
				else if (nChangeType == 2)
				{
					// 로비일경우 팝업을 띄움
					if (GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
					{
						ShowNewAskChargeDlg(NEW_ASK_CHARGE_DLG_TYPE_INSU_GOLD2, NULL);
					}
				}
			}
		}
		break;
	case SV_REPORT_RIGUSER_RESULT:
		{
			CSV_REPORT_RIGUSER_RESULT msg;
			msg.Get(lpData, TotSize);

			int errorCode = *msg.m_nErrorCode;

			if(errorCode)
			{
				ShowMainMessageDlg("무분별한 신고 행위로 일일 신고 횟수 제한되어 더 이상 신고할 수 없습니다.");				
			}
			else
			{
				ShowMainMessageDlg("정상적으로 신고 접수 되었습니다.");
			}
		}
		break;
	case OPCODE_SV_2018THANKSGIVING_ATTENDANCE_INFO:
		{
			CChuSeokDlg* pChuSeokDlg = NULL;
			MM().Call(UPG_CMK_GET_CHUSEOK_ATTENDANCE_DLG, (WPARAM)0, (LPARAM)&pChuSeokDlg);
			
			if( pChuSeokDlg )
			{
				pChuSeokDlg->HandleResponse(Signal, TotSize, lpData);

				if( m_nAttendanceShowCount == 0 )
				{
					MM().Call(UPG_CMK_SHOW_CHUSEOK_ATTENDANCE_DLG, (WPARAM)0, (LPARAM)0);
				}
			}

			m_nAttendanceShowCount++;
		}
		break;
	case SV_PM_2019HAVEST_INFO:
		{
			CChuSeokPopup *pPopup = NULL;
			MM().Call(UPG_CMK_GET_CHUSEOK_POPUP, (WPARAM)0, (LPARAM)&pPopup);
			pPopup->HandleResponse(Signal, TotSize, lpData);

			m_bShowChuSeokPopup = true;
		}
		break;
	case SV_PM_2019HAVEST_REQUEST_RESULT:
		{
			// 골드 적립 통장 출금 팝업에 패킷 전달
			CChuSeokPopup *pPopup = NULL;
			MM().Call(UPG_CMK_GET_CHUSEOK_POPUP, (WPARAM)0, (LPARAM)&pPopup);
			pPopup->HandleResponse(Signal, TotSize, lpData);
		}
		break;
	case SV_REPORT_BANISH_MORNINGBET:
		{
			CSV_REPORT_BANISH_MORNINGBET msg;
			msg.Get(lpData, TotSize);
			
			int nBanishCount = *msg.m_cBanishCount;
			SYSTEMTIME tmSvCurrent = *msg.m_tmCurrent;
			SYSTEMTIME tmBanishEnd = *msg.m_tmBanishEnd;
			SYSTEMTIME tmCurTime;
			GetLocalTime(&tmCurTime);

			COleDateTime CurTime(tmSvCurrent);
			COleDateTime EndTime(tmBanishEnd);

			if (nBanishCount == 0 || EndTime <= CurTime)
				break;

			CTime tmTmp(tmCurTime);
			CTime tmCur(tmSvCurrent);
			CTime tmEnd(tmBanishEnd);
			CTimeSpan leftBlockTime(tmEnd - tmCur);
			tmTmp += leftBlockTime;
			tmTmp.GetAsSystemTime(tmBanishEnd);

			SetMorningBanCount(nBanishCount);
			SetMorningBanTime(tmBanishEnd);

			NMBASE::UTIL::WriteProfile_int(GM().GetMyInfo()->UI.ID, "MorningCheckDay", -1);
						
			bool bAfterLogin = m_strMorningBetNotice.GetLength() != 0;
			
			if (leftBlockTime.GetDays() > 0)
			{
				m_strMorningBetNotice.Format("아침벳 룰 위반 %d회, %02d시간 %02d분간 입장 제한.",
					GM().GetMorningBanCount(), leftBlockTime.GetDays() * 24 + leftBlockTime.GetHours(), leftBlockTime.GetMinutes());
			}
			else if (leftBlockTime.GetHours() > 0)
			{
				m_strMorningBetNotice.Format("아침벳 룰 위반 %d회, %02d시간 %02d분간 입장 제한.",
					nBanishCount, leftBlockTime.GetHours(), leftBlockTime.GetMinutes());
			}
			else
			{
				m_strMorningBetNotice.Format("아침벳 룰 위반 %d회, %02d분간 입장 제한.",
					nBanishCount, leftBlockTime.GetMinutes());
			}

			if(bAfterLogin)
			{
				ShowMainMessageDlg(m_strMorningBetNotice);
			}		
		}
		break;
	}

	ASSERT(m_pCurGame != NULL);
	if(m_pCurGame){
		LONG nReturn=m_pCurGame->OnPacketNotify(pName, Signal, TotSize, lpData);
		return nReturn;
	}
	return FALSE;
}

void CGameManager::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_MAINFRAME_WEBCALL:
		{
			if(m_pMainWnd)
			{
				m_pMainWnd->KillTimer(TIMER_MAINFRAME_WEBCALL);
			}

			if( m_cWebCallThread.m_bMemo )
			{
				m_cWebCallThread.MakeMemoData();
				m_cWebCallThread.m_bMemo = false;
				MM().Call(UPG_MEMO_CALL_LIST);
			}		
		}
		break;
	}
}

//대기실에서 방생성버튼을 눌러 방만들기 옵션창을 활성
LRESULT CGameManager::ShowCreateRooomDlg(ROOMOPTION *pWebSelectedRoomOption)
{
	if(!m_pCurGame)
		return FALSE;

	CDataRoomOptionList *pDataRoomOptionList = GetDataRoomOptionList();
	if(pDataRoomOptionList == NULL) 
		return FALSE;
	ROOMOPTION *pDefaultRoomOption = pDataRoomOptionList->GetDefaultRoomOption();
	if(pDefaultRoomOption == NULL) 
		return FALSE;

	ASSERT(m_pCurGame != NULL);
	CPlayer *pMyInfo = GetMyInfo();
	ZeroMemory(&m_stRoomInfo, sizeof(ROOMINFO));
	m_stRoomInfo.UNum = pMyInfo->UI.UNum;
	strncpy_s(m_stRoomInfo.ID, sizeof(m_stRoomInfo.ID), pMyInfo->UI.ID, 15);
	strncpy_s(m_stRoomInfo.NickName, sizeof(m_stRoomInfo.NickName), pMyInfo->UI.NickName, 19);

	m_stRoomInfo.NowUserNum		= 1;
	m_stRoomInfo.MaxUserNum		= m_pCurGame->GetMaxPlayer();
	//	m_stRoomInfo.CreateRGrade	= 0;
	m_stRoomInfo.nGameIndex		= m_pCurGame->GetGameType();
	m_stRoomInfo.RoomNum		= 0;
	m_stRoomInfo.cRuleType		= 0;
	m_stRoomInfo.State			= 0;
	m_stRoomInfo.ObserverOption	= ROOMINFO::OBSERVER_OPTION_OK;
	m_stRoomInfo.FormKind		= CHOICE_GAME;
	m_stRoomInfo.bSuperMaster	= FALSE;
	//	m_stRoomInfo.bPlayer		= m_pCurGame->GetMaxPlayer();
	//	m_stRoomInfo.nChoiceMode	= CHOICE_MODE;
	m_stRoomInfo.cRuleType		= RULETYPE_7POKER;
	m_stRoomInfo.nAutoBetKind	= pDefaultRoomOption->sAutoBet;
	m_stRoomInfo.BetMoney		= GM().GetRoomOptionSeedMoney(pDefaultRoomOption, true);
	//m_stRoomInfo.llEnterLimitMoney = pDefaultRoomOption->llEnterLimitMoney;
	m_stRoomInfo.llEnterLimitMoney = GM().GetRoomOptionLimitMoney(pDefaultRoomOption, true);
	//m_stRoomInfo.bVIPEnterLimitYN = false;
	m_stRoomInfo.sBetRuleKind	= pDefaultRoomOption->sBetRuleKind;
	
	//규제안 : 참가비 추가
	m_stRoomInfo.fBetMoneyMulti = pDefaultRoomOption->fBetMoneyMulti;

	LRESULT nRet = m_pCurGame->ShowCreateRooomDlg(&m_stRoomInfo, pWebSelectedRoomOption);

	SetWaitResponse(false);
	SetAccessRoomState(false);
	if(nRet == IDOK)
	{
		if(!CheckShowCommunityWindow())
			return IDCANCEL;

		if(CheckLoadGame())
		{
			ForceCreateRooom();
		}
		else
		{
			m_nLoadingType = EGLT_CREATE;
		}
	}

	return nRet;
}


//규제안 : 바로 시작 실패하면 방생성
void CGameManager::QuickCreateRoom(bool bLimitUser)
{
	if(!m_pCurGame)
		return;

	CDataRoomOptionList *pDataRoomOptionList = GetDataRoomOptionList();
	if(pDataRoomOptionList == NULL) 
		return ;

	char cCardSetting=0;
	INT64 llLimitSeedMoney=pDataRoomOptionList->FindMaxSeedMoney(GetLimitLossRestMoney(), GetMyInfo()->UI.GetMoney(), cCardSetting);
	ROOMOPTION *pDefaultRoomOption;

	if(m_RoomOptionForRefuse.ChipKind==0 && false==m_RoomOptionForRefuse.stMoneyRange.IsInRange(GetMyInfo()->UI.GetMoney())){
		pDefaultRoomOption = pDataRoomOptionList->GetDefaultRoomOption();
	}
	else if(m_RoomOptionForRefuse.ChipKind==1 && false==m_RoomOptionForRefuse.stMoneyRange.IsInRange(m_nMyGold)){
		pDefaultRoomOption = pDataRoomOptionList->GetDefaultRoomOption(0, ROOM_CHIPKIND_GOLD);	
	}
	else if(m_RoomOptionForRefuse.ChipKind==1 && m_RoomOptionForRefuse.stMoneyRange.MinMoney == 1 && m_RoomOptionForRefuse.stMoneyRange.MaxMoney == 0){
		// 골드는 자유방 생성이 제한 되기에 max 0일때 일반 머니 디폴트 옵션 받아옴. 2018.12.29
		pDefaultRoomOption = pDataRoomOptionList->GetDefaultFreeRoomOption();
	}
	else{
		pDefaultRoomOption = &m_RoomOptionForRefuse;	
	}
	m_bDeafultCreateRoom=bLimitUser;
	if(pDefaultRoomOption == NULL) {
		return;
	}

	if(false == m_quickJoinOtherRoom) // 다른 방 바로가기 실패 시 방 생성.
	{
		// 규제안 작업 // 자유탭 상태면 제한없음 방 생성.
		if(getRoomListTabState() == 0 )
		{
			pDefaultRoomOption = pDataRoomOptionList->GetDefaultFreeRoomOption();
			if(pDefaultRoomOption == NULL) 
				return ;		
		}
	}

	ASSERT(m_pCurGame != NULL);
	ZeroMemory(&m_stRoomInfo, sizeof(ROOMINFO));

	CPlayer *pMyInfo = GetMyInfo();
	ZeroMemory(&m_stRoomInfo, sizeof(ROOMINFO));
	m_stRoomInfo.UNum = pMyInfo->UI.UNum;
	strncpy_s(m_stRoomInfo.ID, sizeof(m_stRoomInfo.ID), pMyInfo->UI.ID, 15);
	strncpy_s(m_stRoomInfo.NickName, sizeof(m_stRoomInfo.NickName), pMyInfo->UI.NickName, 19);

	m_stRoomInfo.NowUserNum		= 1;
	m_stRoomInfo.MaxUserNum		= m_pCurGame->GetMaxPlayer();
	m_stRoomInfo.nGameIndex		= m_pCurGame->GetGameType();
	m_stRoomInfo.RoomNum		= 0;
	m_stRoomInfo.State			= 0;
	m_stRoomInfo.ObserverOption	= ROOMINFO::OBSERVER_OPTION_OK;
	m_stRoomInfo.bSuperMaster	= FALSE;

	m_stRoomInfo.cCardSetting =	pDefaultRoomOption->cCardSetting;

	if(IDX_GAME_HA == GetCurrentGameType() || m_quickJoinOtherRoom)
	{
		m_stRoomInfo.FormKind = GetCurrentGame()->GetDefaultFormKind();	// 클릭된 바로시작 버튼에 따른 값 셋팅

	}else
	{
		m_stRoomInfo.FormKind =	pDefaultRoomOption->cFormKind;
	}

	if ( pDefaultRoomOption->cRuleType == -1 )
		m_stRoomInfo.cRuleType = hoola::eRULE_TYPE_STOP; 
	else
		m_stRoomInfo.cRuleType = pDefaultRoomOption->cRuleType; 
	
	m_stRoomInfo.fBetMoneyMulti = pDefaultRoomOption->fBetMoneyMulti;
	//m_stRoomInfo.llEnterLimitMoney = pDefaultRoomOption->llEnterLimitMoney;
	m_stRoomInfo.llEnterLimitMoney = GM().GetRoomOptionLimitMoney(pDefaultRoomOption, false);
	m_stRoomInfo.BetMoney = GM().GetRoomOptionSeedMoney(pDefaultRoomOption, false);

	if((true==bLimitUser)&&(0ll!=llLimitSeedMoney)){
		if(m_stRoomInfo.BetMoney>GetLimitLossRestMoney()){
			m_stRoomInfo.BetMoney = llLimitSeedMoney;
			m_stRoomInfo.cCardSetting =	cCardSetting;
		}
	}
	m_stRoomInfo.cMemberRoom = 0;
#if defined(RESTRICTION_LEVEL)
	if(true == IsDiaMemberRooms()){
		m_stRoomInfo.cMemberRoom = 1;
		//if(true == m_bDiaOption){
		//	m_stRoomInfo.cMemberRoom = 1;
		//}
		//else{
		//	m_stRoomInfo.cMemberRoom = 0;
		//}
	}
#else
	m_stRoomInfo.cMemberRoom = 0;
#endif


	m_stRoomInfo.nRaiseStyle = pDefaultRoomOption->nRaiseStyle;
	m_stRoomInfo.nAutoBetKind =	pDefaultRoomOption->sAutoBet;
	m_stRoomInfo.sBetRuleKind =	pDefaultRoomOption->sBetRuleKind;
	m_stRoomInfo.MoneyRange = pDefaultRoomOption->stMoneyRange;
	m_stRoomInfo.ChipKind = pDefaultRoomOption->ChipKind;

	if (IDX_GAME_BD == GetCurrentGameType())
		m_stRoomInfo.bMorningBet = GetMorningBet();

	//임의의 랜덤 타이틀
	CString strTitle;
	MM().Call(UPG_CMK_GENERATE_RANDOM_ROOM_NAME, 0, (LPARAM)(&strTitle));
	strncpy_s(m_stRoomInfo.Title, 32, strTitle, strTitle.GetLength());

	if(true == m_quickJoinOtherRoom) // 다른 방 바로가기 실패 시 방 생성.
	{
		m_quickJoinOtherRoom = false ;
	}

	SetWaitResponse(false);
	SetAccessRoomState(false);

	SetQuickJoinForRandom(false);

	if(CheckLoadGame())
	{
		ForceCreateRooom();
	}
	else
	{
		m_nLoadingType = EGLT_CREATE;
	}

	//// 규제안 첫 적용 로직 일단 임시로 주석처리.
	//if(!m_pCurGame)
	//	return;

	//CDataRoomOptionList *pDataRoomOptionList = GetDataRoomOptionList();
	//if(pDataRoomOptionList == NULL) 
	//	return ;

	//ROOMOPTION *pDefaultRoomOption;
	//if (m_bQuickJoinForRandom)
	//{
	//	pDefaultRoomOption = &m_RoomOptionForRefuse;		
	//}
	//else
	//{
	//	if (m_scondition.m_bUseGameMode && m_scondition.m_bUseSeedMoney)
	//	{
	//		pDefaultRoomOption = pDataRoomOptionList->GetQJRoomOption(m_scondition.m_bUseGameMode, m_scondition.m_nGameMode, m_scondition.m_bUseSeedMoney, m_scondition.m_nSeedType) ; 
	//	}else{
	//		pDefaultRoomOption = pDataRoomOptionList->GetDefaultRoomOption();
	//	}
	//}
	//
	//if(pDefaultRoomOption == NULL) {
	//	return;
	//}

	//// 규제안 작업 // 자유탭 상태면 제한없음 방 생성.
	//if(getRoomListTabState() == 0 )
	//{
	//	pDefaultRoomOption = pDataRoomOptionList->GetDefaultFreeRoomOption();
	//	if(pDefaultRoomOption == NULL) 
	//		return ;		
	//}

	//ASSERT(m_pCurGame != NULL);
	//ZeroMemory(&m_stRoomInfo, sizeof(ROOMINFO));

	//CPlayer *pMyInfo = GetMyInfo();
	//ZeroMemory(&m_stRoomInfo, sizeof(ROOMINFO));
	//m_stRoomInfo.UNum = pMyInfo->UI.UNum;
	//strncpy_s(m_stRoomInfo.ID, sizeof(m_stRoomInfo.ID), pMyInfo->UI.ID, 15);
	//strncpy_s(m_stRoomInfo.NickName, sizeof(m_stRoomInfo.NickName), pMyInfo->UI.NickName, 19);

	//m_stRoomInfo.NowUserNum		= 1;
	//m_stRoomInfo.MaxUserNum		= m_pCurGame->GetMaxPlayer();
	//m_stRoomInfo.nGameIndex		= m_pCurGame->GetGameType();
	//m_stRoomInfo.RoomNum		= 0;
	//m_stRoomInfo.State			= 0;
	//m_stRoomInfo.ObserverOption	= ROOMINFO::OBSERVER_OPTION_OK;
	//m_stRoomInfo.bSuperMaster	= FALSE;

	//m_stRoomInfo.cCardSetting =	pDefaultRoomOption->cCardSetting;
	//m_stRoomInfo.FormKind =	m_scondition.m_nGameMode;//pDefaultRoomOption->cFormKind;
	//m_stRoomInfo.cRuleType = pDefaultRoomOption->cRuleType;
	//m_stRoomInfo.fBetMoneyMulti = pDefaultRoomOption->fBetMoneyMulti;
	//m_stRoomInfo.llEnterLimitMoney = pDefaultRoomOption->llEnterLimitMoney;
	//m_stRoomInfo.llEnterLimitMoney = GM().GetRoomOptionLimitMoney(pDefaultRoomOption, false);
	//m_stRoomInfo.BetMoney = pDefaultRoomOption->llSeedMoney;
	//m_stRoomInfo.nRaiseStyle = pDefaultRoomOption->nRaiseStyle;
	//m_stRoomInfo.nAutoBetKind =	pDefaultRoomOption->sAutoBet;
	//m_stRoomInfo.sBetRuleKind =	pDefaultRoomOption->sBetRuleKind;
	//m_stRoomInfo.MoneyRange = pDefaultRoomOption->stMoneyRange;
	//
	//if (m_bQuickJoinForRandom)
	//{
	//	//뉴포커는 기본 설정 값이 다르다
	//	switch ( GetCurrentGameType() )
	//	{ 
	//	case IDX_GAME_HA:
	//		m_stRoomInfo.FormKind = GetCurrentGame()->GetDefaultFormKind();	// 클릭된 바로시작 버튼에 따른 값 셋팅
	//		m_stRoomInfo.cRuleType = hoola::eRULE_TYPE_STOP;
	//		break;

	//	case IDX_GAME_SD:
	//		//m_stRoomInfo.fBetMoneyMulti = 3.5;
	//		break;

	//	case IDX_GAME_NP:
	//		m_stRoomInfo.FormKind = (m_scondition.m_nGameMode == 0)?FORMKIND_BOMB:m_scondition.m_nGameMode;//FORMKIND_BOMB;
	//		break;
	//	}
	//} 
	//else
	//{
	//	//뉴포커는 기본 설정 값이 다르다
	//	switch ( GetCurrentGameType() )
	//	{ 
	//	case IDX_GAME_HA:
	//		if(m_scondition.m_bUseGameMode)
	//		{
	//			m_stRoomInfo.FormKind = GetCurrentGame()->GetDefaultFormKind();	// 클릭된 바로시작 버튼에 따른 값 셋팅
	//			m_stRoomInfo.cRuleType = m_scondition.m_nRuleType;
	//		}else
	//		{
	//			m_stRoomInfo.FormKind = GetCurrentGame()->GetDefaultFormKind();	// 클릭된 바로시작 버튼에 따른 값 셋팅
	//			m_stRoomInfo.cRuleType = hoola::eRULE_TYPE_STOP;
	//		}

	//		if(m_scondition.m_bMaxPlayer)
	//		{
	//			m_stRoomInfo.MaxUserNum = m_scondition.m_nGameMode; // 인원수.
	//		}
	//		break;
	//	case IDX_GAME_BD:
	//		m_stRoomInfo.FormKind	= GetCurrentGame()->GetDefaultFormKind();
	//		
	//		// 하위방과 상위방 분리.
	//		if(pDefaultRoomOption->llEnterLimitMoney < 1000000000000){

	//			m_stRoomInfo.cCardSetting = 1;
	//		}else{
	//			m_stRoomInfo.cCardSetting = 0;
	//		}

	//		break;

	//	case IDX_GAME_SD:

	//		//규제안 : 섯다 참가비 옵션 설정
	//		//m_stRoomInfo.fBetMoneyMulti = 3.5; 
	//		break;

	//	case IDX_GAME_NP:
	//		m_stRoomInfo.FormKind = (m_scondition.m_nGameMode == 0)?FORMKIND_BOMB:m_scondition.m_nGameMode;//FORMKIND_BOMB;
	//		//규제안 : 입장구간에 따라서 설정할 수 있는 카드수가 다르며 다른 값을 서버에 주면 에러가 발생한다.
	//		if (m_RoomOptionTmp.cCardSetting == -1)
	//		{
	//			m_stRoomInfo.cCardSetting = pDefaultRoomOption->cCardSetting;
	//		}else{
	//			m_stRoomInfo.cCardSetting = m_RoomOptionTmp.cCardSetting;
	//		}
	//		break;
	//	case IDX_GAME_SP:
	//	case IDX_GAME_HL:
	//		//규제안 : 기본값으로 설정
	//		m_stRoomInfo.cCardSetting = pDefaultRoomOption->cCardSetting;
	//		break;
	//	}
	//}

	//m_bQuickJoinForRandom = false;	

	////임의의 랜덤 타이틀
	//CString strTitle;
	//MM().Call(UPG_CMK_GENERATE_RANDOM_ROOM_NAME, 0, (LPARAM)(&strTitle));
	//strncpy_s(m_stRoomInfo.Title, 32, strTitle, strTitle.GetLength());
	//
	//SetWaitResponse(false);
	//SetAccessRoomState(false);

	//if(CheckLoadGame())
	//{
	//	ForceCreateRooom();
	//}
	//else
	//{
	//	m_nLoadingType = EGLT_CREATE;
	//}
}

//규제안 : 게임 중 다른방 바로가기 요청
void CGameManager::AskByQuickStartOtherRoom(INT64 rkey_prev, ROOMOPTION& ro)
{
	m_quickJoinOtherRoom = true;
	CCL_ASK_QUICKSTART_IN_ROOM msg;
	//void Set(int sndKind, int rnum_prev, ROOMOPTION& ro) 
	msg.Set(g_Config.SndFxKind, rkey_prev, ro);

	m_RoomOptionForRefuse = ro;

	//msg.Set(&m_stRoomInfo, g_Config.SndFxKind);	
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	SetWaitResponse(true);
	SetAccessRoomState(true);
}

//규제안 : 다른방 바로가기 실패하면 방생성
void CGameManager::AskCreateRoomByQuickStartOtherRoom(INT64 llRoomKeyPrev, ROOMOPTION roomOption)
{
	if(!m_pCurGame)
		return;

	CDataRoomOptionList *pDataRoomOptionList = GetDataRoomOptionList();
	if(pDataRoomOptionList == NULL) 
		return ;
	ROOMOPTION *pDefaultRoomOption = pDataRoomOptionList->GetDefaultRoomOption();
	if(pDefaultRoomOption == NULL) 
		return ;

	ASSERT(m_pCurGame != NULL);
	ZeroMemory(&m_stRoomInfo, sizeof(ROOMINFO));

	CPlayer *pMyInfo = GetMyInfo();
	ZeroMemory(&m_stRoomInfo, sizeof(ROOMINFO));
	m_stRoomInfo.UNum = pMyInfo->UI.UNum;
	strncpy_s(m_stRoomInfo.ID, sizeof(m_stRoomInfo.ID), pMyInfo->UI.ID, 15);
	strncpy_s(m_stRoomInfo.NickName, sizeof(m_stRoomInfo.NickName), pMyInfo->UI.NickName, 19);

	m_stRoomInfo.NowUserNum		= 1;
	m_stRoomInfo.MaxUserNum		= roomOption.cMaxUserNum;
	m_stRoomInfo.nGameIndex		= m_pCurGame->GetGameType();
	m_stRoomInfo.RoomNum		= 0; 
	m_stRoomInfo.cRuleType		= roomOption.cRuleType;
	m_stRoomInfo.State			= 0;
	m_stRoomInfo.ObserverOption	= ROOMINFO::OBSERVER_OPTION_OK;
	m_stRoomInfo.FormKind		= roomOption.cFormKind == -1 ? GetCurrentGame()->GetDefaultFormKind() : roomOption.cFormKind;
	m_stRoomInfo.bSuperMaster	= FALSE;
	//	m_stRoomInfo.bPlayer		= m_pCurGame->GetMaxPlayer();
	//	m_stRoomInfo.nChoiceMode	= CHOICE_MODE;
	
	//위의 값과 중복
	//m_stRoomInfo.cRuleType		= RULETYPE_7POKER; //1
 
	m_stRoomInfo.nAutoBetKind	=  roomOption.sAutoBet;
	m_stRoomInfo.BetMoney		= roomOption.llSeedMoney;
	m_stRoomInfo.llEnterLimitMoney = roomOption.llEnterLimitMoney;//pDefaultRoomOption->llEnterLimitMoney;
	//m_stRoomInfo.bVIPEnterLimitYN = false;
	m_stRoomInfo.sBetRuleKind	=  roomOption.sBetRuleKind;//pDefaultRoomOption->sBetRuleKind; 

	//규제안 : 입장구간에 따라서 설정할 수 있는 카드수가 다르며 다른 값을 서버에 주면 에러가 발생한다.
	m_stRoomInfo.cCardSetting = roomOption.cCardSetting;

	//임의의 랜덤 타이틀
	CString strTitle;
	MM().Call(UPG_CMK_GENERATE_RANDOM_ROOM_NAME, 0, (LPARAM)(&strTitle));
	strncpy_s(m_stRoomInfo.Title, 32, strTitle, strTitle.GetLength());

	SetWaitResponse(false);
	SetAccessRoomState(false);

	if(CheckLoadGame())
	{
		ForceCreateRooom();
	}
	else 
	{
		m_nLoadingType = EGLT_CREATE;
	}
}

//일반 방생성
LRESULT CGameManager::ForceCreateRooom(void)
{
	CSV_ASK_CREATEROOM msg;

	//yoo
	msg.Set(&m_stRoomInfo, g_Config.SndFxKind, m_bDeafultCreateRoom);	
	m_bDeafultCreateRoom=false;
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	SetWaitResponse(true);
	SetAccessRoomState(true);
	return TRUE;
}

//대기실에서 바로가기를 눌렀을 경우
LRESULT CGameManager::GoDirectEnterRoom(bool bUseRoomOption, ROOMOPTION *pRoomOption, bool bCheckLoad/* = false*/, bool bSecret/* = false*/)
{
	ASSERT(m_pCurGame != NULL);

	if(!CheckShowCommunityWindow())
	{
		return FALSE;	
	}

	if(!m_pCurGame)
		return FALSE;

	if(bCheckLoad && CheckLoadGame() == false)
	{
		m_nLoadingType = EGLT_DIRECTCREATE;
		
		///옵션정보저장
		m_bUseRoomOption = bUseRoomOption;		
		if (pRoomOption)
		{
			m_RoomOption = *pRoomOption;
		}
		else
		{
			m_bUseRoomOption = false;
			pRoomOption = NULL;
		}

		return FALSE;
	}
	
	ROOMOPTION MakeRoomOption = {0,};

	if (bUseRoomOption && pRoomOption )	
	{
		//bUseRoomOption가 TRUE이면 넘겨받은 옵션으로 방을 만든다.
		MakeRoomOption = *pRoomOption;		
	}
	else
	{
		//bUseRoomOption가 FALSE이면 디폴트 옵션으로 방을 만든다.		
		CDataRoomOptionList *pDataRoomOptionList = GetDataRoomOptionList();
		if(pDataRoomOptionList == NULL) return FALSE;		
		MakeRoomOption = *(pDataRoomOptionList->GetDefaultRoomOption());
		MakeRoomOption.cFormKind = m_pCurGame->GetDefaultFormKind();
	}

	CPlayer *pMyInfo = GetMyInfo();
	ZeroMemory(&m_stRoomInfo, sizeof(ROOMINFO));
	m_stRoomInfo.UNum = pMyInfo->UI.UNum;
	strncpy_s(m_stRoomInfo.ID, sizeof(m_stRoomInfo.ID), pMyInfo->UI.ID, 15);
	strncpy_s(m_stRoomInfo.NickName, sizeof(m_stRoomInfo.NickName), pMyInfo->UI.NickName, 19);

	m_stRoomInfo.NowUserNum		= 1;
	m_stRoomInfo.MaxUserNum		= m_pCurGame->GetMaxPlayer();
//	m_stRoomInfo.MaxUserNum		= MakeRoomOption.cMaxUserNum;
	//	m_stRoomInfo.CreateRGrade	= 0;
	m_stRoomInfo.nGameIndex		= m_pCurGame->GetGameType();
	m_stRoomInfo.RoomNum		= 0;
	m_stRoomInfo.cRuleType		= 0;
	m_stRoomInfo.State			= 0;
	m_stRoomInfo.ObserverOption	= ROOMINFO::OBSERVER_OPTION_OK;
	m_stRoomInfo.FormKind		= m_pCurGame->GetDefaultFormKind();
	m_stRoomInfo.ChipKind		= getRoomListTabState()==2?1:0;
	m_stRoomInfo.bSuperMaster	= FALSE;
//	m_stRoomInfo.bPlayer		= 0;
//	m_stRoomInfo.nChoiceMode	= CHOICE_MODE;
	m_stRoomInfo.cRuleType		= MakeRoomOption.cRuleType;

	if ( GetCurrentGameType() == IDX_GAME_HA && pRoomOption != NULL )
	{
		if (pRoomOption->cFormKind!=FORMKIND_DEFAULT)
		{
			m_stRoomInfo.FormKind		= pRoomOption->cFormKind;//MakeRoomOption.cFormKind;
		}
		else
			m_stRoomInfo.FormKind = hoola::eFORM_KIND_JOKER;

		if ( pRoomOption->cMaxUserNum == -1)
			m_stRoomInfo.MaxUserNum	= 5;
		else
			m_stRoomInfo.MaxUserNum	= pRoomOption->cMaxUserNum;
		
		if ( pRoomOption->cRuleType == -1 )
			m_stRoomInfo.cRuleType = hoola::eRULE_TYPE_STOP; 
		else
			m_stRoomInfo.cRuleType = pRoomOption->cRuleType; 
	}
	m_stRoomInfo.nAutoBetKind	= MakeRoomOption.sAutoBet;
	m_stRoomInfo.BetMoney		= MakeRoomOption.llSeedMoney;
	m_stRoomInfo.llEnterLimitMoney = MakeRoomOption.llEnterLimitMoney;
	//m_stRoomInfo.bVIPEnterLimitYN = false;
	m_stRoomInfo.Secret			=  (char)bSecret; //잠금방으로 만들지 여부
	m_stRoomInfo.sBetRuleKind	= MakeRoomOption.sBetRuleKind;

	//각 게임별 특화된 방정보 설정
	m_pCurGame->CheckDirectEnterRoomInfo(0, &m_stRoomInfo);

	CSV_ASK_CREATEROOM msg;
	msg.Set(&m_stRoomInfo, g_Config.SndFxKind, !bUseRoomOption);		//<--------3번째 인자가 true이면 서버에서 디폴트 베팅룰/시드머니을 알아서 만들어준다.
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	SetWaitResponse(true);

	return TRUE;
}

LRESULT CGameManager::GoEnterDummyRoom(SMALLROOMINFO *pSI)
{
	ASSERT(m_pCurGame != NULL);

	if(!m_pCurGame)
		return FALSE;

	if(!CheckShowCommunityWindow())
	{
		return FALSE;
	}
	
	if (pSI)
	{
		CPlayer *pMyInfo = GetMyInfo();
		ZeroMemory(&m_stRoomInfo, sizeof(ROOMINFO));
	
		m_stRoomInfo.UNum = pMyInfo->UI.UNum;
		strncpy_s(m_stRoomInfo.ID, sizeof(m_stRoomInfo.ID), pMyInfo->UI.ID, 15);	
		strncpy_s(m_stRoomInfo.NickName, sizeof(m_stRoomInfo.NickName), pMyInfo->UI.NickName, 19);
		strncpy_s(m_stRoomInfo.Title, sizeof(m_stRoomInfo.Title), pSI->szTitle, 31);
		
		m_stRoomInfo.NowUserNum		= 1;
		m_stRoomInfo.MaxUserNum		= m_pCurGame->GetMaxPlayer();
//		m_stRoomInfo.CreateRGrade	= 0;
		m_stRoomInfo.nGameIndex		= m_pCurGame->GetGameType();
		m_stRoomInfo.RoomNum		= 0;
		m_stRoomInfo.cRuleType		= 0;
		m_stRoomInfo.State			= 0;
		m_stRoomInfo.ObserverOption	= ROOMINFO::OBSERVER_OPTION_OK;
		m_stRoomInfo.FormKind		= pSI->cFormKind;
		m_stRoomInfo.bSuperMaster	= FALSE;
//		m_stRoomInfo.bPlayer		= m_pCurGame->GetMaxPlayer();
		//m_stRoomInfo.nChoiceMode	= CHOICE_MODE;
		m_stRoomInfo.cRuleType		= pSI->cRuleType;
		m_stRoomInfo.nAutoBetKind	= pSI->sAutoBetKind;
		m_stRoomInfo.BetMoney		= pSI->llSeedMoney;
		m_stRoomInfo.sBetRuleKind	= pSI->sBetRuleKind;
		m_stRoomInfo.llEnterLimitMoney = pSI->llEnterLimitMoney;
	}

	if(CheckLoadGame() == false)
	{
		m_nLoadingType = EGLT_ENTERDUMMYROOM;
		return FALSE;
	}


	CSV_ASK_CREATEROOM msg;
	msg.Set(&m_stRoomInfo, g_Config.SndFxKind);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	SetWaitResponse(true);

	return TRUE;
}

             
LRESULT CGameManager::AskDirectEnterRoom(bool bUseRoomOption, ROOMOPTION *pRoomOption, int nGameMode, int nCalBtnType/* =1 */, bool bSerchEnterRoom/* = false*/, bool bSerchAutoEnterRoom/* = false*/, bool bForceLocalServer /*= false*/)
{
	ASSERT(m_pCurGame != NULL);	

	if(!m_pCurGame)
		return FALSE;

	if(!CheckShowCommunityWindow())
	{
		return FALSE;
	}

	m_bSerchEnterRoom = bSerchEnterRoom;
	m_bSerchAutoEnterRoom = bSerchAutoEnterRoom;
	m_bForceLocalServer = bForceLocalServer;

	//m_nGoDirectEnterType = m_pCurGame->GetDefaultFormKind();
	m_bUseRoomOption = bUseRoomOption;
	if (pRoomOption)
	{
		m_RoomOption = *pRoomOption;
		if(nGameMode == -1) m_RoomOption.cFormKind = m_pCurGame->GetDefaultFormKind();
		else m_RoomOption.cFormKind = (char)nGameMode;
	}
	else
	{
		m_bUseRoomOption = false;
		pRoomOption = NULL;
	}

	if ( GetCurrentGameType() == IDX_GAME_HA )
		m_RoomOption.cFormKind = m_pCurGame->GetDefaultFormKind();	// 훌라는 무조건 클릭한 바로입장 버튼에 따라서 넣어줌

	if ( GetCurrentGameType() == IDX_GAME_BD )
		m_RoomOption.bMorningBet = GetMorningBet();

	if(CheckLoadGame())
	{
		AskForceDirectEnterRoom( m_bUseRoomOption, &m_RoomOption, nCalBtnType,  bForceLocalServer);
	}
	else
	{
		m_nLoadingType = EGLT_DIRECTENTER;
	}
	return TRUE;
}


bool CGameManager::IsEnableComeBackPromotion(void)
{
	if(false == m_RETENTION_ALRAM.bToday){
		if(m_RETENTION_ALRAM.nTodayWinCnt>=m_RETENTION_ALRAM.nTotalWinCnt){
			return true;
		}
	}
	return false;	
}

bool CGameManager::IsVIPCouponPromotion(void)
{
	return false;
	return m_tPromotionData.IsPromotion();
}


int CGameManager::GetCouponPromotionImageNum(int nTotalCnt, int &nCount, int &nTotalCount)
{
	int nCouponCnts[6]={10, 30, 50, 70, 90, 110};
	int nCouponAddCnts[6]={10, 10, 10, 15, 15, 40};
	int nCouponImage[6]={0, 1, 1, 2, 2, 3};
	int nFind=-1;
	int nFindCnt=0;
	nTotalCount=20;
	for(int i=0; i<6; i++){
		if(nCouponCnts[i]>nTotalCnt){
			nFind=i;
			nTotalCount=nCouponCnts[i]-nFindCnt;
			break;
		}
		nFindCnt=nCouponCnts[i];
	}
	if(-1!=nFind){
		nCount=nTotalCnt-nFindCnt;
		return nCouponImage[nFind];
	}
	nCount=(nTotalCnt-nCouponCnts[5])%nTotalCount;
	return nCouponImage[5];
}

//서버에 바로 시작 요청을 보낸다.
LRESULT CGameManager::AskForceDirectEnterRoom( bool bUseRoomOption, ROOMOPTION *pRoomOption, int nCalBtnType, bool bForceLocalServer /*= false*/ )
{
	ROOMOPTION ro = {0,};

	if (true == bUseRoomOption)
	{
		ro = *pRoomOption;	
	}
	else
	{
		CDataRoomOptionList *pDataRoomOptionList = GetDataRoomOptionList(); //게임 메니저에 있는 룸옵션 리스트를 받아온다, 만약 없을 경우 리턴
		if(pDataRoomOptionList == NULL) return FALSE;
		
		ROOM_CHIPKIND chipKind = ROOM_CHIPKIND_NORMAL;
		if(getRoomListTabState()==2&&!m_bSelectQuickJoin)
			chipKind = ROOM_CHIPKIND_GOLD;

		ROOMOPTION *pDefaultRoomOption = pDataRoomOptionList->GetDefaultRoomOption( 0, chipKind );//디폴트 룸옵션 받기
		if(pDefaultRoomOption != NULL)
		{
			ro = *pDefaultRoomOption;
		}
	}

	if (IDX_GAME_BD == GetCurrentGameType())
		ro.bMorningBet = GetMorningBet();

	//규제안 : 랜덤입장 기능으로 패킷 선택()
	CSV_ASK_QUICKSTART	msg; //다른방 바로가기
	CSV_ASK_QUICKSTART2	msg2; //로비에 바로입장, 상새표에서 랜덤입장

	CPlayer *pPlayer = GetMyInfo();
	if(pPlayer == NULL) return FALSE;

	//규제안 : 임시방옵션
	m_RoomOptionForRefuse = ro;

	//규제안 : 
	//bMustSameRoomOption : 게임내 다른방 바로가기, 상세표에서 랜덤입장 에서 true로 설정
	//bFindLocalServer :	ERR_ASKOTHERSERVER로 Refuse가 올 경우 true로 설정

	//ROOMOPTION 
	//7포커, 뉴포커, 하이로우, 섯다: 게임모드(사용하지 않으면 -1), 방정원 : = -1설정,  게임규모(큰방/작은방....)(사용하지 않으면 -1), 카드세팅/참가비(사용하지 않으면 -1) ,
	//로우바둑이 : 게임모드 = -1, 게임규모(큰방/작은방....) = -1로 설정,  카드세팅/참가비 -1, 
	//훌라 :  훌라 방정원 설정값으로 설정, 게임옵션 : 사용안하면 -1, 게임규모 : 안쓰면 -1, 카드세팅/참가비 = -1로

	//규제안 : 랜덤입장을 위한 설정 : 바로가기, 랜덤입장
	ro.cMemberRoom = 0;
	if(m_bSelectQuickJoin) // 선택시작이면~
	{
		ro.cFormKind = m_nSQJGameMode;
		ro.llSeedMoney = m_SQJllSeedMoney;
		ro.llEnterLimitMoney = m_llSQJEnterLimitMoney ;
		ro.cMemberRoom = ((true == m_bDiaOption) ? (1) : (0));

		if(GetCurrentGameType() != IDX_GAME_HA)
		{
			ro.cCardSetting = m_nSQJGamerule;

		}else
		{
			if ( m_nSQJGamerule == -1 )
				ro.cRuleType = hoola::eRULE_TYPE_STOP; 
			else
				ro.cRuleType = m_nSQJGamerule; 
		}

		bUseRoomOption = true ; // 선택시작은 무조건 옵션 사용으로 보냄.
		m_RoomOptionForRefuse.llEnterLimitMoney=m_llSQJEnterLimitMoney;
		m_RoomOptionForRefuse.llSeedMoney=m_SQJllSeedMoney;
		m_RoomOptionForRefuse.llDefaultQuickStartLimitMoney=m_llSQJEnterLimitMoney;
		m_RoomOptionForRefuse.llDefaultQuickStart_SeedMoney=m_SQJllSeedMoney;
	
	}else if(!m_quickJoinOtherRoom || m_bQuickJoinForRandom){ //대기실에서 바로가기, 상새표에서 랜덤입장 

		if(GetCurrentGameType() != IDX_GAME_HA)
		{
			ro.cFormKind = -1;

		}else
		{
			ro.cFormKind = m_pCurGame->GetDefaultFormKind();

			if ( m_nSQJGamerule == -1 )
				ro.cRuleType = hoola::eRULE_TYPE_STOP; 
			else
				ro.cRuleType = m_nSQJGamerule; 
		}
		if(true == IsDiaMemberRooms()){
			ro.cMemberRoom = 1;
		}

		//규제안 : 일단 랜덤조건으로 입장구간 선택
		ro.llSeedMoney = -1;

		//대기실에서 바로가기는 입장 구간이 없으므로 -1
		if(!m_quickJoinOtherRoom && !m_bQuickJoinForRandom){
			ro.llEnterLimitMoney = -1;
			ro.stMoneyRange.MinMoney = -1;
		}

		ro.cCardSetting = -1;
	}
#if !defined(RESTRICTION_LEVEL)
	ro.cMemberRoom = 0;
#endif

	/*msg.Set(pPlayer->UI.UNum, pPlayer->UI.ID, g_Config.SndFxKind, bUseRoomOption, ro, nCalBtnType, bForceLocalServer);*/
	
	if(m_bSelectQuickJoin)
	{
		msg2.Set( g_Config.SndFxKind, CSV_ASK_QUICKSTART2::LOBBY_TYPE, m_nTabState, bUseRoomOption, !m_bSQJMustUseOption, m_bRequestOtherChannel, ro );
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg2.pData, msg2.GetTotalSize());
	}
	else if (m_bQuickJoinForRandom) // 규제안 추가  자유방, 일반 방 옵션 추가.
	{
		if(nCalBtnType == 2 )
		{
			// 랜덤 입장을 눌렀을 때는 시드 값과 카드 셋팅을 넘겨줌.
			ro.llSeedMoney = pRoomOption->llSeedMoney;
			ro.cCardSetting = pRoomOption->cCardSetting;
		}
		msg2.Set( g_Config.SndFxKind, CSV_ASK_QUICKSTART2::DETAIL_SETUP_TYPE, m_nTabState, true, true, m_bRequestOtherChannel, ro ); 
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg2.pData, msg2.GetTotalSize()); 
	}
	else if (!m_quickJoinOtherRoom)
	{
		msg2.Set( g_Config.SndFxKind, CSV_ASK_QUICKSTART2::LOBBY_TYPE, m_nTabState, bUseRoomOption, false, m_bRequestOtherChannel, ro );
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg2.pData, msg2.GetTotalSize());
	}
	else
	{
		msg.Set(pPlayer->UI.UNum, pPlayer->UI.ID, g_Config.SndFxKind, m_nTabState, bUseRoomOption, ro, nCalBtnType, bForceLocalServer, 0);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	}

	//규제안 : 사용한 플래그 초기화
	m_quickJoinOtherRoom = false;
	m_bRequestOtherChannel = false;	

	SetWaitResponse(true);

	return TRUE;
}

void CGameManager::AddGameInfo(const char *str, COLORREF crNewColor/*=RGB(0,0,0)*/, bool bUserChat/* = false*/)
{
	ASSERT(m_pCurGame != NULL);
	if(m_pCurGame)
		return m_pCurGame->AddGameInfo(str, crNewColor, bUserChat);
}


void CGameManager::AddGameChat(const char *str, COLORREF crNewColor/*=RGB(0,0,0)*/, bool bUserChat/* = false*/)
{
	ASSERT(m_pCurGame != NULL);
	if(m_pCurGame)
		return m_pCurGame->AddGameChat(str, crNewColor, bUserChat);
}

LRESULT	CGameManager::ShowSelectStarterDlg(void)
{
	CSelectQuickstarterDlg dlg;
	LRESULT nRet = dlg.DoModal(GetMainWnd());

	return 0;
}

LRESULT	CGameManager::ShowResetSelectStarterDlg(void)
{
	CResetQuickStarterDlg dlg;
	LRESULT nRet = dlg.DoModal(GetMainWnd());

	return 0;
}


LRESULT	CGameManager::ShowQuickJoinerDlg(void)
{
	ASSERT(m_pCurGame != NULL);
	if(m_pCurGame)
	{
		return m_pCurGame->ShowQuickJoinerDlg();
	}

	return 0;
}

LRESULT	CGameManager::SendQuickJoiner(void)
{
	//if(!CheckShowCommunityWindow())
	//{
	//	return FALSE;
	//}

	if(CheckLoadGame() == false)
	{
		m_nLoadingType = EGLT_PREMQUICKJOIN;
		return FALSE;
	}

	ASSERT(m_pCurGame != NULL);
	return m_pCurGame->SendQuickJoiner();
}

LRESULT	CGameManager::SendSelectQuickJoiner(void)
{
	if(CheckLoadGame() == false)
	{
		m_nLoadingType = EGLT_PREMQUICKJOIN;
		return FALSE;
	}

	ASSERT(m_pCurGame != NULL);
	return m_pCurGame->SendQuickJoiner();
}

void CGameManager::ShowGameWindow(bool bShow)
{
	ASSERT(m_pCurGame != NULL);
	if(m_pCurGame)
		m_pCurGame->ShowGameWindow(bShow);
}

void CGameManager::AskCurrentPlayTime()
{
	CCL_REQUEST_PLAYTIMEINFO msg;
	msg.Set();
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
}

bool CGameManager::IsPlayTimeOver()
{
	if(m_nPlayTimeSec >= 18000)
		return true ;

	return false ;
}

void CGameManager::SetCurrentWhere(IDX_GAMEWHERE nWhere)
{
	if(m_nWhere == nWhere) return;
	m_nWhere = nWhere;

	if(m_pCurGame == NULL) return;
	CGameViewBase *pViewBase = m_pCurGame->GetGameView();
	if(pViewBase == NULL) return;

	if(m_nWhere == IDX_GAMEWHERE_GAME) pViewBase->StartTimer();
	else pViewBase->StopTimer();
}

IDX_GAMEWHERE CGameManager::GetCurrentWhere(void)
{
	return m_nWhere;
}

void CGameManager::ExitSubscript()
{
	//나가기 예약한다.
	CPlayer *pPlayer = GetMyInfo();
	if (pPlayer==NULL)
		return;

	//CCL_EXITSUBSCRIPT esmsg;
	//esmsg.Set(g_RI.RoomNum, pPlayer->UI.UNum, pPlayer->UI.ID, GetCurrentGameView()->m_bExitSubscript?2:1);
	//NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), esmsg.pData, esmsg.GetTotalSize());

	// 규제안 추가.
	CCL_EXITSUBSCRIPT esmsg;
	esmsg.Set(g_RI.RoomNum, pPlayer->UI.UNum, pPlayer->UI.ID, GetCurrentGameView()->m_bExitSubscript?2:1, 0);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), esmsg.pData, esmsg.GetTotalSize());

}
//UGP작업 추가
void CGameManager::ExitGame(bool bUserAFK)
{
	if(IsWaitResponse() == true) return;

	if(IsAbnormalOutRoom()) // 비정상적인 퇴실이면
	{
		OnExitRoom(REASON_OUTROOM_ABNORMAL);
		return;
	}

	// 방에서 나감을 메인서버에 요청
	CSV_ASK_OUTROOM aormsg;
	// ### [ 관전기능 ] ###
	CPlayer *pPlayer = GetMyInfo();
	aormsg.Set(g_RI.RoomNum, pPlayer->UI.UNum, pPlayer->UI.ID, bUserAFK?REASON_OUTROOM_USER_AFK:REASON_OUTROOM_USER_REQUEST);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), aormsg.pData, aormsg.GetTotalSize());


	SetWaitResponse(true);
}

void CGameManager::OnExitRoom(REASON_OUTROOM reason)
{
	if(0 != GetMyInfo()->JoinState){
		WriteShowFullExplain();
	}
	SetWaitResponse(false);

	SetAbnormalOutRoom(false);
	SetAccessRoomState(false);

	int chipKind = g_RI.ChipKind;
	// 게임정보 초기화
	ZeroMemory(&g_RI, sizeof(g_RI));

	// 대기실 화면으로 전환
	SetChangeWhere(IDX_GAMEWHERE_LOBY, true);

	m_pCurGame->OnExitRoom();

	//관전자 초기화
	CObserverDlg *pObserverDlg = GetObserverDlg(); 
	if(NULL != pObserverDlg)
	{
		if(pObserverDlg->GetSafeHwnd())
		{
			pObserverDlg->ShowWindow(SW_HIDE);
			pObserverDlg->User_DelAll(); //모두 제거
		}
	}

	// 초대 대화창 감춤
	CInviteDlg *pInviteDlg = GetInviteDlg();
	if(pInviteDlg && pInviteDlg->GetSafeHwnd())		
	{
		pInviteDlg->ShowWindow(SW_HIDE);
	}

	// 일 이용시간 정보 요청.
	AskCurrentPlayTime() ; 

	CPlayer *pPlayer = GetMyInfo();
	/*CCommunityManager::Instance()->SendBuddyInfoChange(GetCurrentGameType(), pPlayer->UI.nRoomNum, pPlayer->UI.GetMoney());*/
	CCommunityManager::Instance()->SendBuddyInfoChange(GetCurrentGameType(), pPlayer->UI.nRoomNum, pPlayer->UI.GetRealPMoney());


	////규제안 : 방에서 나가는 경우 규제안 팝업과 버튼을 활성
	//if (reason != REASON_OUTROOM_QUICKSTART_IN_ROOM)
	//{
	//	switch (GetLossMoneyState())
	//	{
	//	case DAILYLOSS_STATE_NORMAL:

	//		//규제안 이용제한 안내 버튼
	//		MM().Call(UPG_CMK_HIDE_RESTRICTION_BUTTON);
	//		break;

	//		//게임별 경고창
	//	case DAILYLOSS_STATE_WARNING:
	//	case DAILYLOSS_STATE_WARNING_IN_ROOM:
	//		{
	//			// 규제안 on/off 기능을 추가하면서 원안인 3.3조 요율일 때만 경고창이 제대로 노출되도록 수정.
	//			// 그 이외의 요율은 off 버전으로 생각하고 경고창은 노출되지 않음.
	//			if(GetLossMaxMoney() == (INT64)33000000000000) // 10만원 기준 33조
	//			{
	//				MM().Call(UPG_CMK_SHOW_RESTRICTION_BUTTON);
	//				MM().Call(UPG_CMK_SHOW_RESTRICTION_DIALOG);
	//			}
	//		}				
	//		break;
	//		//33조를 도달
	//	case DAILYLOSS_STATE_BLOCKED:
	//		{
	//			// 규제안 on/off 기능을 추가하면서 원안인 3.3조 요율일 때만 경고창이 제대로 노출되도록 수정.
	//			// 그 이외의 요율은 off 버전으로 생각하고 경고창은 노출되지 않음.
	//			if(GetLossMaxMoney() == (INT64)33000000000000) // 10만원 기준 33조
	//			{
	//				MM().Call(UPG_CMK_SHOW_RESTRICTION_BUTTON);
	//				MM().Call(UPG_CMK_SHOW_RESTRICTION_DIALOG);
	//			}
	//		}
	//		break;
	//		//33조를 초과
	//	case DAILYLOSS_STATE_BLOCKED_24H:
	//		MM().Call(UPG_CMK_SHOW_RESTRICTION_BUTTON);
	//		MM().Call(UPG_CMK_SHOW_RESTRICTION_DIALOG);
	//		break;
	//	}
	//}

	if(getRoomListTabState()!=2 && chipKind==0)	
		MM().Call(UPG_CMK_SHOW_RESTRICTION);
}

void CGameManager::OnEnterRoom(BOOL bCreation)
{
	ASSERT(m_pCurGame != NULL);

	if(m_pCurGame)
		m_pCurGame->OnEnterRoom(bCreation);
}

BOOL CGameManager::CheckMutex(LPCSTR lpAppName)
{
	// 중복 실행 방지
	//m_hMutex=::CreateMutex(NULL, TRUE, _T("WoolalaMutex")); // 같은 뮤택스가 존재하면 이미 다른 프로그램이 실행중임
	//if(ERROR_ALREADY_EXISTS==::GetLastError())
	//{       
	//	HWND hWnd=::FindWindow(_T("#32770 (Dialog)"), lpAppName); // ClassName, WindowsName
	//	::SetForegroundWindow(hWnd);

	//	if(m_hMutex)
	//		::CloseHandle(m_hMutex);		

	//	AfxMessageBox( "이미 다른 게임이 실행중입니다");

	//	return FALSE;
	//}

	// QA팀 요청으로 QA존에서는 중복실행이 가능하도록 수정. 2013.11.4 류승민
	// 중복 실행 방지
	if(g_ServIP != "183.110.61.192")
	{
		// 중복 실행 방지
		m_hMutex=::CreateMutex(NULL, TRUE, _T("WoolalaMutex")); // 같은 뮤택스가 존재하면 이미 다른 프로그램이 실행중임
		if(ERROR_ALREADY_EXISTS==::GetLastError())
		{       
			HWND hWnd=::FindWindow(_T("#32770 (Dialog)"), lpAppName); // ClassName, WindowsName
			::SetForegroundWindow(hWnd);

			if(m_hMutex)
				::CloseHandle(m_hMutex);		

			AfxMessageBox( "이미 다른 게임이 실행중입니다");

			return FALSE;
		}
	}
	else if(g_ServIP == "183.110.61.192")
	{
		AfxMessageBox( "중복 접속 가능" );
	}

	return TRUE;
}

void CGameManager::DestroyMutex(void)
{
	if( m_hMutex != NULL )		
	{
		::CloseHandle( m_hMutex );
		m_hMutex= NULL;
	}
}

void CGameManager::InitGoGameDirect(void)
{
	NMBASE::UTIL::NMGoGameDirect_Init(&m_hMutex,GetCurrentGameString());
}

void CGameManager::OnChangeFullScreen(bool bFullScreen)
{
	map<IDX_GAME, CGame *>::iterator start = m_listGame.begin();
	map<IDX_GAME, CGame *>::iterator end = m_listGame.end();

	CGameViewBase *pGameView = NULL;
	CRect MovRect;

	for(;start != end;start++)
	{
		pGameView = start->second->GetGameView();
		MovRect.SetRect(0,0,GetWidth(),GetHeight());

		if(bFullScreen)	MovRect.OffsetRect(CPoint((1024 - GetWidth())/2, (768 - GetHeight())/2));

		pGameView->MoveWindow(MovRect);

		start->second->OnChangeFullScreen(bFullScreen);
	}
}

void CGameManager::LoadSprite(void)
{
	ASSERT(m_pCurGame != NULL);

	if(m_pCurGame)
		m_pCurGame->LoadSprite();
}

LRESULT CGameManager::ShowOptionDlg(bool bGameOptionTab/* = true*/)
{
	COptionDlg Dlg(GetMainWnd());
	LRESULT nRet = Dlg.DoModal(GetCurrentGameType(), (GetMyInfo()->UI.Sex!=0));
	if (nRet == IDOK)
	{
		//  세이브 시키고 적용
		SaveGameConfig(GetMyInfo()->UI.ID);
	}
	
// 	map<IDX_GAME, CGame *>::iterator start = m_listGame.begin();
// 	map<IDX_GAME, CGame *>::iterator end = m_listGame.end();
// 
// 	for(;start != end;++start)
// 	{
// 		start->second->OnCloseOptionDlg(nRet);
// 	}
	CGame *pCurrentGame = GetCurrentGame();
	if(pCurrentGame)
		pCurrentGame->OnCloseOptionDlg(nRet);

	return nRet;
}

void CGameManager::ReLoadSprite(int nSkinType)
{
	ASSERT(m_pCurGame != NULL);

	if(m_pCurGame)
	{
		m_pCurGame->ReLoadSprite(nSkinType);
	}
}

const int CGameManager::GetWidth(void)
{
	ASSERT(m_pCurGame != NULL);

	if(m_pCurGame)
	{
		return m_pCurGame->GetWidth();
	}

	return -1;
}

const int CGameManager::GetHeight(void)
{
	ASSERT(m_pCurGame != NULL);

	if(m_pCurGame)
	{
		return m_pCurGame->GetHeight();
	}

	return -1;
}

const CString CGameManager::GetCurrentGameString(void)
{
	ASSERT(m_pCurGame != NULL);

	if(m_pCurGame)
	{
		return m_pCurGame->GetGameString();
	}

	return "";
}

const CString CGameManager::GetCurrentGameName(void)
{
	ASSERT(m_pCurGame != NULL);

	if(m_pCurGame)
	{
		return m_pCurGame->GetGameName();
	}

	return "";
}

const int CGameManager::GetCurrentGameCode(void)
{
	ASSERT(m_pCurGame != NULL);

	if(m_pCurGame)
	{
		return m_pCurGame->GetGameCode();
	}

	return -1;
}

const int CGameManager::GetCurrentGameArray(void)
{
	return GetIDXToGameArray(GetCurrentGameType());
}

const int CGameManager::GetIDXToGameArray(IDX_GAME eGameType)
{
	int nGameArray=-1;
	switch(eGameType){
	case IDX_GAME_SP: //포커
		nGameArray = 0;
		break;
	case IDX_GAME_BD: //바둑이
		nGameArray = 1;
		break;
	case IDX_GAME_NP: //뉴포커
		nGameArray = 2;
		break;
	case IDX_GAME_HL: //하이로우
		nGameArray = 3;
		break;
	case IDX_GAME_SD: //섯다
		nGameArray = 4;
		break;
	case IDX_GAME_HA: //훌라
		nGameArray = 5;
		break;
	}
	return nGameArray;
}

const CString CGameManager::GetIDXToGameName(IDX_GAME eGameType)
{
	int nGameArray=-1;
	switch(eGameType){
	case IDX_GAME_SP: return CString("7포커");
	case IDX_GAME_BD: return CString("로우바둑이"); 
	case IDX_GAME_NP: return CString("뉴포커"); 
	case IDX_GAME_HL: return CString("하이로우");
	case IDX_GAME_SD: return CString("섯다");
	case IDX_GAME_HA: return CString("훌라");
	}
	return "None";
}


SMALLROOMINFO* CGameManager::FindDataRoomList(int nRoomNum)
{
	CDataRoomList::DATAPOSITION pos;
	SMALLROOMINFO *pSRI = m_DataRoomList.GetFirstData(pos);
	while(pSRI)
	{
		if(nRoomNum==pSRI->sRoomNum){
			return pSRI;
		}
		pSRI = m_DataRoomList.GetNextData(pos);
	}
	return NULL;
}

void CGameManager::AddHideChatText(CString *str)
{
	m_HideChat.AddData(str);

}
void CGameManager::ClearHideChat()
{
	m_HideChat.Clear();
}
CString &CGameManager::GetHideChatData()
{
	return m_HideChat.GetData();
}

void CGameManager::BadUserReport(const CBadUserDlg &dlg, const std::string &strMessage, CString strSaveName)
{
	//신고할 내용이 너무 짧다
	CString strwatchid, strchat;
	strchat = strMessage.c_str();

	if( dlg.m_nKind != REPORT_KIND_SCAM && ( strchat.GetLength()<30) )	//불량행위신고는검사안한다.
	{
		ShowMainMessageDlg("신고할 내용이 너무 짧습니다.");	
		return;
	}

	// 4000자 이상은 잘라냄
	if(strchat.GetLength() > 4000)
	{
		int nDeleteCount = strchat.GetLength() - 4000;
		int i=0;
		//한글등 Multibyte 문자를 고려한다.
		while( i < nDeleteCount )
		{
			if(IsDBCSLeadByte(strchat.GetAt(i))) i += 2;
			else i += 1;
		}

		strchat = strchat.Mid(i);
	}

	//신고의 종류에 따라서 다르게 처리한다
	CPlayer *pMyInfo = GetMyInfo();
	if( dlg.m_nKind == REPORT_KIND_MONEYDEAL )	//머니신고 
	{
		ResetBadUserTimer();

		CSV_ASK_BADUSER abumsg;
		abumsg.Set(FALSE, dlg.m_nKind, g_RI.RoomNum, pMyInfo->UI.UNum, pMyInfo->UI.ID,
			(char*)dlg.m_strTargetID.operator LPCTSTR(),
			(char*)dlg.m_strTargetNickName.operator LPCTSTR(),
			(char*)dlg.m_strSubmitReason.operator LPCTSTR(),
			(char*)strwatchid.operator LPCTSTR(), (char*)strchat.operator LPCTSTR());
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), abumsg.pData, abumsg.GetTotalSize());

		// 신고기능 이용회수 카운터 증가
		dlg.ms_nSubmitCnt++;
		// 신고기능 딜레이 타이머 세팅
		ResetBadUserTimer();
// 		if( GetCurrentWhere() != IDX_GAMEWHERE_GAME)
// 		{
// 			MM().Call(UPG_CMK_BADUSERCOMPLETE);
// 		}
// 		else
// 		{
			CInGameBadUserComplainMsgBox msgDlg( GM().GetMainWnd() );
			msgDlg.m_strName = strSaveName;
			if(msgDlg.DoModal() == IDCANCEL) return;
//		}		
	}
	else if( dlg.m_nKind == REPORT_KIND_SWEAR )	//욕설신고
	{
		CSV_ASK_BADUSER abumsg;
		abumsg.Set(FALSE, dlg.m_nKind, g_RI.RoomNum, pMyInfo->UI.UNum, pMyInfo->UI.ID,
			(char*)dlg.m_strTargetID.operator LPCTSTR(),
			(char*)dlg.m_strTargetNickName.operator LPCTSTR(),
			(char*)dlg.m_strSubmitReason.operator LPCTSTR(),
			(char*)strwatchid.operator LPCTSTR(), (char*)strchat.operator LPCTSTR());
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), abumsg.pData, abumsg.GetTotalSize());

		// 신고기능 이용회수 카운터 증가
		dlg.ms_nSubmitCnt++;
		// 신고기능 딜레이 타이머 세팅
		ResetBadUserTimer();
// 		if( GetCurrentWhere() != IDX_GAMEWHERE_GAME)
// 		{
// 			MM().Call(UPG_CMK_BADUSERCOMPLETE);
// 		}
// 		else
// 		{
			CInGameBadUserComplainMsgBox msgDlg( GM().GetMainWnd() );
			msgDlg.m_strName = strSaveName;
			if(msgDlg.DoModal() == IDCANCEL) return;
//		}
	}
	else if( dlg.m_nKind == REPORT_KIND_SCAM ) //112불량행위 짜고치기
	{
		CSV_ASK_BADUSER abumsg;
		abumsg.Set(FALSE, dlg.m_nKind, g_RI.RoomNum, pMyInfo->UI.UNum, pMyInfo->UI.ID,
			(char*)dlg.m_strTargetID.operator LPCTSTR(),
			(char*)dlg.m_strTargetNickName.operator LPCTSTR(),
			(char*)dlg.m_strSubmitReason.operator LPCTSTR(),
			(char*)strwatchid.operator LPCTSTR(), (char*)strchat.operator LPCTSTR());
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), abumsg.pData, abumsg.GetTotalSize());

		// 신고기능 이용회수 카운터 증가
		dlg.ms_nSubmitCnt++;
		// 신고기능 딜레이 타이머 세팅
		ResetBadUserTimer();			

		CInGameBadUserComplainMsgBox msgDlg( GM().GetMainWnd() );
		msgDlg.m_strName = strSaveName;
		if(msgDlg.DoModal() == IDCANCEL) return;
	}
	else if(dlg.m_nKind == REPORT_KIND_ONLY_SCAM)
	{
		CSV_ASK_BADUSER abumsg;
		abumsg.Set(FALSE, dlg.m_nKind, g_RI.RoomNum, pMyInfo->UI.UNum, pMyInfo->UI.ID,
			(char*)dlg.m_strTargetID.operator LPCTSTR(),
			(char*)dlg.m_strTargetNickName.operator LPCTSTR(),
			(char*)dlg.m_strSubmitReason.operator LPCTSTR(),
			(char*)strwatchid.operator LPCTSTR(), (char*)strchat.operator LPCTSTR());
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), abumsg.pData, abumsg.GetTotalSize());

		// 신고기능 이용회수 카운터 증가
		dlg.ms_nSubmitCnt++;
		// 신고기능 딜레이 타이머 세팅
		ResetBadUserTimer();	
	}
}


//buy in, 입장머니 제한-올인시 계속 진행 선택팝업
void CGameManager::AskContinueGame()
{
	if (m_pCurGame==NULL) return;

	CAskContinueGameDlg dlg(GetMainWnd());
	switch(m_pCurGame->GetGameType()){
		case IDX_GAME_SP:
			dlg.DoModal("보유했던 입장머니가 올인 되었습니다.\n계속해서 해당 게임 방에서 플레이 하시겠습니까?\n('확인' 버튼을 누르시면 자동으로 참여예약이 됩니다)");
			break;

		case IDX_GAME_BD:
			dlg.DoModal("로우바둑이 확인창");
			break;
	}
}

void CGameManager::BadUserReportGameChat(bool bOnlyScam)
{
	//게임방 채팅창 신고버튼처리
	//게임방 신고대상자 노출 조건
	//-> 채팅창에 내용을 입력한 아이디 + 게임 Play중인 아이디 + 관전자 아이디

	if (m_pCurGame==NULL)
		return;

	//대기실 채팅창 신고버튼처리
	std::list<std::pair<std::string, std::string> > listIDNickName;
	m_HideChat.MAKE_IDList( listIDNickName );
	
	std::string strMessage = m_HideChat.GetData().GetString();

	CTime time = CTime::GetCurrentTime();
	IDX_GAME GameType = GM().GetCurrentGameType();
	CString strName;
	strName.Format("Poker_%d_112_%d_%d_%d_%d_%d.jpg", GameType, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	NMBASE::GRAPHICGDI::ScreenCapture(GetMainWnd()->GetSafeHwnd(), strName);

	CBadUserDlg dlg(GetMainWnd());
// 	CInGameBadUserComplainMsgBox dlg(GetMainWnd());
// 	if(dlg.DoModal() == IDCANCEL) return;

	//채팅창에 내용을 입력한 아이디/닉넴 추가
	std::list<std::pair<std::string, std::string> >::iterator start = listIDNickName.begin();
	std::list<std::pair<std::string, std::string> >::iterator end = listIDNickName.end();

	for(;start != end;++start)
	{
		dlg.AddUser((char *)start->first.c_str(), (char *)start->second.c_str());
	}

	//게임참가자(관전자포함) 아이디/닉넴 추가
	for(int p=0; p<(m_pCurGame->GetMaxNewPlayer()-1); p++)
	{
		CPlayer *pPlayer = m_pCurGame->GetGameProcess()->GetPlayer(p);

		if (pPlayer==NULL)
			continue;
		if (pPlayer->UI.ID[0]==0)
			continue;
		dlg.AddUser(pPlayer->UI.ID, pPlayer->UI.NickName);
	}

	if(bOnlyScam)
	{
		dlg.m_nKind = REPORT_KIND_ONLY_SCAM;
	}

	if(dlg.DoModal() == IDCANCEL) return;

	BadUserReport(dlg, strMessage, strName);
}

void CGameManager::BadUserReportLobbyChat(void)
{
	//대기실 채팅창 신고버튼처리
	std::list<std::pair<std::string, std::string> > listIDNickName;

	m_HideChat.MAKE_IDList( listIDNickName );
	std::string strMessage = m_HideChat.GetData().GetString();

	CTime time = CTime::GetCurrentTime();
	IDX_GAME GameType = GM().GetCurrentGameType();
	CString strName;
	strName.Format("Poker_%d_112_%d_%d_%d_%d_%d.jpg", GameType, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	NMBASE::GRAPHICGDI::ScreenCapture(GetMainWnd()->GetSafeHwnd(), strName);

	CBadUserDlg dlg(GetMainWnd());

	std::list<std::pair<std::string, std::string> >::iterator start = listIDNickName.begin();
	std::list<std::pair<std::string, std::string> >::iterator end = listIDNickName.end();

	for(;start != end;++start)
	{
		dlg.AddUser((char *)start->first.c_str(), (char *)start->second.c_str());
	}

	if(dlg.DoModal() == IDCANCEL) return;

	BadUserReport(dlg, strMessage, strName);
}

void CGameManager::BadUserReportRecevieMemo(MEMO &memo)
{
	CTime time = CTime::GetCurrentTime();
	IDX_GAME GameType = GM().GetCurrentGameType();
	CString strName;
	strName.Format("Poker_%d_112_%d_%d_%d_%d_%d.jpg", GameType, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	NMBASE::GRAPHICGDI::ScreenCapture(GetMainWnd()->GetSafeHwnd(), strName);

	CBadUserDlg dlg(GetMainWnd());
	dlg.AddUser((char *)memo.senderID.c_str(), (char *)memo.senderNickname.c_str());
	if(dlg.DoModal() == IDCANCEL) return;
	std::string strMessage = "[쪽지 : ";
	strMessage += memo.senderID;
	strMessage += "]\n";
	strMessage += "[node : ";
	strMessage += memo.node;
	strMessage += "] ";
	strMessage += memo.content;

	BadUserReport(dlg, strMessage, strName);
}

void CGameManager::BadUserReportMemoBox(std::list<MEMO *> &listMemo)
{
	CTime time = CTime::GetCurrentTime();
	IDX_GAME GameType = GM().GetCurrentGameType();
	CString strName;
	strName.Format("Poker_%d_112_%d_%d_%d_%d_%d.jpg", GameType, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	NMBASE::GRAPHICGDI::ScreenCapture(GetMainWnd()->GetSafeHwnd(), strName);

	CBadUserDlg dlg(GetMainWnd());

	std::list<std::pair<std::string, std::string> > listIDNickName;
	
	std::list<MEMO *>::iterator start_memo = listMemo.begin();
	std::list<MEMO *>::iterator end_memo = listMemo.end();
	MEMO *pMemo = NULL;
	for(;start_memo != end_memo;++start_memo)
	{
		pMemo = *start_memo;
		dlg.AddUser((char *)pMemo->senderID.c_str(), (char *)pMemo->senderNickname.c_str());
	}

	if(dlg.DoModal() == IDCANCEL) return;

	std::string strMessage = "[쪽지 : ";
	strMessage += dlg.m_strTargetID.GetString();
	strMessage += "]";

	start_memo = listMemo.begin();
	for(;start_memo != end_memo;++start_memo)
	{
		pMemo = *start_memo;
		if(dlg.m_strTargetID == pMemo->senderID.c_str())
		{
			strMessage += "\n[node : ";
			strMessage += pMemo->node;
			strMessage += "] ";
			strMessage += pMemo->content;
		}
	}

	BadUserReport(dlg, strMessage, strName);
}

void CGameManager::BadUserReportMUChat(CWnd *pParent, std::list<std::pair<std::string, std::string> > &listIDNickName, std::string &strChatting)
{
	CTime time = CTime::GetCurrentTime();
	IDX_GAME GameType = GM().GetCurrentGameType();
	CString strName;
	strName.Format("Poker_%d_112_%d_%d_%d_%d_%d.jpg", GameType, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	NMBASE::GRAPHICGDI::ScreenCapture(GetMainWnd()->GetSafeHwnd(), strName);

	CBadUserDlg dlg(pParent);

	std::list<std::pair<std::string, std::string> >::iterator start = listIDNickName.begin();
	std::list<std::pair<std::string, std::string> >::iterator end = listIDNickName.end();

	for(;start != end;++start)
	{
		dlg.AddUser((char *)start->first.c_str(), (char *)start->second.c_str());
	}

	if(dlg.DoModal() == IDCANCEL) return;

	BadUserReport(dlg, strChatting, strName);
}

void CGameManager::AppendLoadSprite(NMBASE::GRAPHICGDI::xSprite *pSprite)
{
	list<NMBASE::GRAPHICGDI::xSprite *>::iterator findIt = find(m_listLoadSprite.begin(), m_listLoadSprite.end(), pSprite);
	if(findIt != m_listLoadSprite.end()) return;
	m_listLoadSprite.push_back(pSprite);
}

void CGameManager::RemoveLoadSprite(void)
{
	list<NMBASE::GRAPHICGDI::xSprite *>::iterator start = m_listLoadSprite.begin();
	list<NMBASE::GRAPHICGDI::xSprite *>::iterator end = m_listLoadSprite.end();

	for(;start != end;start++)
	{
		(*start)->Remove();
	}

	m_listLoadSprite.clear();
}

void CGameManager::OnLoadingEnd(void)
{
	ASSERT(m_pCurGame != NULL);

	if(m_pCurGame)
		m_pCurGame->OnLoadingEnd();

	switch(m_nLoadingType)
	{
	case EGLT_CREATE:
		ForceCreateRooom();
		break;
	case EGLT_DIRECTENTER:
		AskForceDirectEnterRoom( m_bUseRoomOption, &m_RoomOption, ECALL_BTNTYPE_LOBY_QUICK, m_bForceLocalServer?true:false );		
		break;
	case EGLT_DIRECTCREATE:
		GoDirectEnterRoom(m_bUseRoomOption, &m_RoomOption, false, false);
		break;
	case EGLT_ENTERROOM:
		AskEnterRoom(m_nRoomNum_for_Join, m_llRoomKey_for_Join, m_bObserve_for_Join, m_strRoomPass_for_Join);
		break;	
	case EGLT_ENTERDUMMYROOM:
		GoEnterDummyRoom(NULL);
		break;
	case EGLT_PREMQUICKJOIN:
		SendQuickJoiner();
		break;
	}
}

LRESULT CGameManager::AskEnterRoom(int nRoomNum, UINT64 llRoomKey, bool bObserve, const char *pRoomPass)
{
	//참여접속
	if(IsWaitResponse() == true) 
		return FALSE;
	if(IsAccessRoomState()) 
		return FALSE;

	if(!(GM().getRoomListTabState()==2 && GM().GetMyGold() > 0)
		&& GetMyInfo()->UI.GetRealPMoney() <= 0) 
	{	
		ShowNewAskChargeDlg(NEW_ASK_CHARGE_DLG_TYPE_FREE_CHARGE_INSU, "");
		return FALSE;
	}

	if(nRoomNum < 0 ) 
		return FALSE;

	CDataRoomList *pDataRoomList = GM().GetDataRoomList();
	SMALLROOMINFO *pSRI = pDataRoomList->GetData(llRoomKey);
	int chipKind = 0;
	if(pSRI==NULL)
	{
		return FALSE;
	}
	else
	{
		chipKind = pSRI->ChipKind;
	}


	if(!CheckShowCommunityWindow())
	{
		return FALSE;
	}

	if (pSRI->cafeID_ > 0 && (pSRI->cafeID_ != GetMyInfo()->UI.unitedCafeInfo_.cafeID_ || GetMyInfo()->UI.cafeMemberGrade_ < CafeSystem::MemberGrade::CAFE_MEMBER))
	{
		ShowMainMessageDlg("같은 연합의 정회원 이상만 입장 가능합니다.");
		return FALSE;
	}

	if(CheckLoadGame() == false)
	{
		m_nLoadingType = EGLT_ENTERROOM;

		//들어갈 방의 정보를 저장해 놓는다.
		m_nRoomNum_for_Join = nRoomNum,
		m_llRoomKey_for_Join = llRoomKey,
		m_bObserve_for_Join = bObserve;	
		
		if (pRoomPass)
			m_strRoomPass_for_Join = pRoomPass;
		else 
			m_strRoomPass_for_Join.Empty();
		
		return FALSE;
	}

	SetWaitResponse(true);
	SetAccessRoomState(true);

	// 방에 입장하기를 요구
	CSV_ASK_ENTERROOM aermsg;
	aermsg.Set(nRoomNum, llRoomKey, GetMyInfo()->UI.UNum, g_Config.SndFxKind, GetMyInfo()->UI.ID, (char*) pRoomPass, (char)bObserve, chipKind);// ### [ 관전기능 ] ###  // ### [사운드 추가 작업] ###
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), aermsg.pData, aermsg.GetTotalSize());

	ShowMainMessageDlg("요청한 방에 연결중입니다");	

	GM().m_nAttendanceShowCount++;

	return TRUE;
}


bool CGameManager::SendWhisper(const char *szMessage, const char *szTargetNick, UINT64 llTargetUserKey, const char *szTargetID)
{
	//llTargetUserKey와 sztargetID를 모를경우 각각 -1 , 비워둔다.

	if ( szTargetNick==NULL || szMessage==NULL)
		return false;

	if (strlen(szMessage)==0)
		return false;

	CString strTargetID;
	if (szTargetID)
	{
		strTargetID = szTargetID;
	}

	//귓말 보내기 처리
	USERINFO *pMyUI = &GetMyInfo()->UI;
	
	CSV_WHISPERCHAT wcmsg;
	wcmsg.Set(	pMyUI->ID,				//보내는 사람 아이디
				pMyUI->NickName,		//보내는 사람 닉네임
				pMyUI->llUserKey,		//보내는 사람 유저키
				strTargetID,			//받는 사람 아이디
				szTargetNick,			//받는 사람 닉네임
				llTargetUserKey,		//받는 사람 유저키
				szMessage );			//메세지
		
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), wcmsg.pData, wcmsg.GetTotalSize());

	return true;
}

int	CGameManager::GetEventMissionStep() 
{
	return m_nEventMissionStep ;
}

void CGameManager::SetRoomListTabState(int nTabState)
{	
	if(IsGetLimitMoney(GetCurrentGameArray()) || IsLimitOver())
	{
		if(nTabState==2)
		{
			MM().Call(UPG_CMK_SHOW_RESTRICTION_PLAYOVER, FALSE);
		}
		else if(nTabState!=2 && (!m_bEnableOver || IsLimitOver()))
		{
			MM().Call(UPG_CMK_SHOW_RESTRICTION_PLAYOVER, TRUE);
		}
	}

	m_nTabState = nTabState;
}

void CGameManager::SaveSelectQuickStartOption(bool bMustUseOption, INT64 llEnterLimitMoney, int nGameMode, int nGamerule, INT64 llSeedMoney, bool bDiaOption)
{
	m_bSQJMustUseOption = bMustUseOption;
	m_llSQJEnterLimitMoney = llEnterLimitMoney; 
	m_nSQJGameMode = nGameMode; 
	m_nSQJGamerule = nGamerule; 
	m_SQJllSeedMoney = llSeedMoney;
	m_bDiaOption = bDiaOption;
}

bool CGameManager::IsJoinViewRoom(SMALLROOMINFO *pSRI)
{
	if (pSRI==NULL) 
		return false;

	if(pSRI->ChipKind==ROOM_CHIPKIND_GOLD && pSRI->stMoneyRange.IsInRange(GM().GetMyGold()) && pSRI->llSeedMoney <= GM().GetMyGold())
		return true;
	else if(pSRI->bIsFriendRoom && pSRI->stMoneyRange.IsInRange(GM().GetMyInfo()->UI.GetRealPMoney()) && pSRI->llSeedMoney <= GM().GetMyInfo()->UI.GetRealPMoney() && !IsLimitOver())
		return true;

	return false;
}

bool CGameManager::SetLossmoneyInfo(INT64 llLossMoney, time_t tBlockTime, DAILYLOSS_STATE	eState , IDX_GAME gameID) 
{ 
	m_llLossMoney = llLossMoney; 
	m_tBlockTime = tBlockTime;
	m_eState = eState ;
	m_gameID = gameID ;
	int nArray=GetIDXToGameArray(m_gameID);
	m_llLimitLoseMoney[nArray] = llLossMoney; 
	m_bReceiveLimitMoney[nArray] = true;
	m_CheckLossMoneyOver[nArray]=IsGetLimitMoney(nArray);
	ErrorLogConsole(" Limit[%d][%d][%d]  = %I64d", nArray, (int)tBlockTime, (int)eState, m_llLimitLoseMoney[nArray]);
	//if(DAILYLOSS_STATE::DAILYLOSS_STATE_BLOCKED_24H != eState)
	//{
	//	SetLossMoneyCheck(llLossMoney, gameID);
	//}
	for(int i=0; i<6; i++){
		if(false==m_bReceiveLimitMoney[i]){
			return false;
		}
	}
	return true;
}
INT64 CGameManager::GetLimitRestMoney(int nGameType)
{
	return m_llLimitLoseMoney[nGameType];
}

INT64 CGameManager::GetLimitLossRestMoney()
{
	int nGameType=GetCurrentGameArray();
	if(0<=m_llLimitLoseMoney[nGameType]) return m_RestraintInfo.LossMoneyLimit;
	return m_RestraintInfo.LossMoneyLimit+GetLimitRestMoney(nGameType);
}

bool CGameManager::IsCurLimitLossOver(void)
{
	int nGameType=GetCurrentGameArray();
	if(0<=m_llLimitLoseMoney[nGameType]) return false;
	if(m_RestraintInfo.LossMoneyLimit<-GetLimitRestMoney(nGameType)){
		return true;
	}
	return false;
}

bool CGameManager::IsLimitOver(void)
{
	if(DAILYLOSS_STATE_BLOCKED_24H==m_eState){
		return true;
	}
	if(0!=m_tBlockTime){
		return true;
	}
	//for(int i=0; i<5; i++){
	//	if(0<=m_llLimitLoseMoney[i]) continue;
	//	if(m_RestraintInfo.BasicLossMoneyLimit<=(-m_llLimitLoseMoney[i])){
	//		return true;
	//	}		
	//}
	return false;
}

bool CGameManager::IsGetLimitMoney(int nGameType)
{
	//return (0!=nGameType%2);
	//if(true==IsLimitOver()) return true;
	if(0<=m_llLimitLoseMoney[nGameType]) return false;
	if(m_RestraintInfo.LossMoneyLimit<=(-m_llLimitLoseMoney[nGameType])){
		return true;
	}
	return false;
}

bool CGameManager::IsGetLimitOverMoney(INT64 llBetMoney)
{
	if(0<=m_llLimitLoseMoney[GetCurrentGameArray()]) return false;
	if(m_RestraintInfo.BasicLossMoneyLimit<=(-m_llLimitLoseMoney[GetCurrentGameArray()]+llBetMoney)){
		return true;
	}
	return false;
}

bool CGameManager::IsGetLimitMoney(int nGameType, INT64 llAddMovey)
{
	if(DAILYLOSS_STATE_BLOCKED_24H==m_eState){
		return true;
	}
	if(0!=m_tBlockTime){
		return true;
	}
	if(0<=m_llLimitLoseMoney[nGameType]) return false;
	if(m_RestraintInfo.LossMoneyLimit<=(-m_llLimitLoseMoney[nGameType]+llAddMovey)){
		return true;
	}
	return false;
}

bool CGameManager::IsShowLimitICon(void)
{
	int nGameType=GetCurrentGameArray();
	if((0<=nGameType)&&(5>nGameType) && g_RI.ChipKind==0) return true;
	return false;
}

int CGameManager::GetLevelLimitMoney(int nGameType)
{
	if(0<=m_llLimitLoseMoney[nGameType]) return 0;
	INT64 llLimitMoney=_abs64(m_llLimitLoseMoney[nGameType]);
	if(true==IsGetLimitMoney(nGameType)){
		return 3;
	}
	long long ll30=m_RestraintInfo.LossMoneyLimit/100ll*30ll;
	long long ll80=m_RestraintInfo.LossMoneyLimit/100ll*80ll;
	if((0ll<=llLimitMoney)&&(ll30>=llLimitMoney)){
		return 0;
	}
	if((ll30<llLimitMoney)&&(ll80>=llLimitMoney)){
		return 1;
	}
	//double dPersent=fabs((double)(llLimitMoney)/(double)(m_RestraintInfo.LossMoneyLimit))*100.0;
	//if((0.0<=dPersent)&&(30.0>=dPersent)){
	//	return 0;
	//}
	//if((30.0<dPersent)&&(80.0>=dPersent)){
	//	return 1;
	//}
	return 2;
	//if((80.0<dPersent)&&(99.0>=dPersent)){
	//	return 2;
	//}
	//return 3;
	//m_RestraintInfo.LossMoneyLimit
	//→ 여유 : 0% ~ 30%
	//→ 보통 : 31% ~ 80%
	//→ 불안 : 81% ~ 99%
	//→ 쉬는날 : 손실한도 전 머니 구간
	
	return rand()%4;
}


INT64 CGameManager::GetLossMaxMoney()
{
	return (INT64)m_RestraintInfo.LossMoneyLimit;
} //일일 손실 제한


INT64 CGameManager::GetLossMaxMoneyBase()
{
	return (INT64)m_RestraintInfo.BasicLossMoneyLimit;
}

INT64 CGameManager::GetLossMaxMoneyForGame()
{
	if(g_RI.bIsFriendRoom){
		return (INT64)m_RestraintInfo.MaxBetPerFriendRoomRound;
	}
	if(0!=g_RI.cMemberRoom){
		return (INT64)m_RestraintInfo.nDiamondLimit;
	}
	return (INT64)m_RestraintInfo.BettingLimit;
} //한판 베팅 제한

int CGameManager::GetLimitToCheckMoney(int nGameType, int nMyIndex, INT64 &llBetMoney)
{
	int nReturn=LIMIT_BETTING;
	INT64 llRestLimitMoney=m_RestraintInfo.LossMoneyLimit+m_llLimitLoseMoney[nGameType]-g_Poker.RU[nMyIndex].nRealBat; //일일 손실 제한
	llBetMoney=GetLossMaxMoneyForGame()-g_Poker.RU[nMyIndex].nRealBat; //한판 베팅 제한
	if((llRestLimitMoney<llBetMoney)&&(0>m_llLimitLoseMoney[nGameType])&&GetEnableOver()==false){
		llBetMoney=llRestLimitMoney;
		nReturn=LIMIT_LOSEMONEY;
	}
	return nReturn;
}

int CGameManager::GetLimitToBetMoney(int nGameType, int nMyIndex, INT64 llMoney, INT64 &llBetMoney)
{
	INT64 llCheckMoney=0ll;
	int nReturn=GetLimitToCheckMoney(nGameType, nMyIndex, llCheckMoney);
	if(llMoney>llCheckMoney){
		llBetMoney=((llCheckMoney>g_Poker.RU[nMyIndex].nPMoney)?(g_Poker.RU[nMyIndex].nPMoney):(llCheckMoney));
		return nReturn;
	}
	llBetMoney=((llMoney>g_Poker.RU[nMyIndex].nPMoney)?(g_Poker.RU[nMyIndex].nPMoney):(llMoney));
	return LIMIT_NONE;
	//g_Poker.RU[nMyIndex].nRealBat
	//LIMIT_NONE
	//LIMIT_BETTING
	//LIMIT_LOSEMONEY
}

// 현재 손실머니가 기준이 넘은 게임이 있는지..
void CGameManager::SetLossMoneyCheck(INT64 lossMoney, IDX_GAME gameID)
{
	// 현재 게임을 얻어옴.
	int count = (int)gameID;
	// 손실머니 제한에 걸렸으면...
	/*if(lossMoney < -(INT64)52000000000000)*/
	if((INT64)33000000000000 != (INT64)GetLossMaxMoney())
	{
		if(lossMoney < -(INT64)52000000000000)
		{
			m_CheckLossMoneyOver[count] = true ;
		}else
		{
			m_CheckLossMoneyOver[count] = false ;
		}
	}
}

bool CGameManager::GetLossMoneyCheck()
{
	return false;
	for(int i =0 ; i< 6; i++)
	{
		if(true == m_CheckLossMoneyOver[i])
		{
			return true ;
		}
	}

	return false ;
}


bool CGameManager::IsOverDay(int nYear, int nMonth, int nDay)
{
	struct tm tm = { 0, 0, 0, nDay, nMonth, nYear-1900 };  // (초, 분, 시, 일, 월, 년) 년의 경우 109가 2009년이다.
	time_t	blockTime=mktime( &tm);
	COleDateTime time = COleDateTime::GetCurrentTime() - g_CSTimeGap;
	SYSTEMTIME st; 
	time.GetAsSystemTime(st);
	COleDateTime CurTime(st);
	COleDateTime EndTime(blockTime) ;
	//int nCurYear=CurTime.GetYear();
	//int nCurMonth=CurTime.GetMonth();
	//int nCurDay=CurTime.GetDay();
	//nYear=EndTime.GetYear();
	//nMonth=EndTime.GetMonth();
	//nDay=EndTime.GetDay();
	if(EndTime<=CurTime){
		return true;
	}
	return false;
}


void CGameManager::SetRestraintInfo(RESTRAINTINFO* RestraintInfo) 
{ 
	ASSERT(RestraintInfo != NULL);
	memcpy(&m_RestraintInfo, RestraintInfo, sizeof(RESTRAINTINFO));
	ErrorLogConsole(" LossMoneyLimit = %I64d", m_RestraintInfo.LossMoneyLimit);
	ErrorLogConsole(" BasicLossMoneyLimit = %I64d", m_RestraintInfo.BasicLossMoneyLimit);
	ErrorLogConsole(" BettingLimit = %I64d", m_RestraintInfo.BettingLimit);
	ErrorLogConsole(" DiamondLimit = %I64d", m_RestraintInfo.nDiamondLimit);
}

long long CGameManager::GetRoomOptionLimitMoney(ROOMOPTION* pRoomOption, bool bCreate)
{
	if(NULL==pRoomOption) return 0ll;
	if(true==bCreate){
		if(0ll!=pRoomOption->llDefaultCreateRoomLimitMoney){
			return pRoomOption->llDefaultCreateRoomLimitMoney;
		}
	}
	else{
		if(0ll!=pRoomOption->llDefaultQuickStartLimitMoney){
			return pRoomOption->llDefaultQuickStartLimitMoney;
		}
	}
	return pRoomOption->llEnterLimitMoney;
}

long long CGameManager::GetRoomOptionSeedMoney(ROOMOPTION* pRoomOption, bool bCreate)
{
	if(NULL==pRoomOption) return 0ll;
	if(true==bCreate){
		if(0ll!=pRoomOption->llDefaultCreateRoom_SeedMoney){
			return pRoomOption->llDefaultCreateRoom_SeedMoney;
		}
	}
	else{
		if(0ll!=pRoomOption->llDefaultQuickStart_SeedMoney){
			return pRoomOption->llDefaultQuickStart_SeedMoney;
		}
	}
	return pRoomOption->llSeedMoney;

}

string CGameManager::GetCurrentTimeString(void)
{
	CTime time = CTime::GetCurrentTime();
	string sStr=NMBASE::UTIL::format("%04d%02d%02d%02d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	return sStr;
}

bool CGameManager::CheckDiffTime(string sTime)
{
	if(true==sTime.empty()) return true;
	int nYear=0, nMonth=0, nDay=0, nHour=0, nMinute=0, nSecond=0;
	sscanf(sTime.c_str(), "%04d%02d%02d%02d%02d%02d", &nYear, &nMonth, &nDay, &nHour, &nMinute, &nSecond);
	CTime time = CTime::GetCurrentTime();
	if((nYear==time.GetYear())&&(nMonth==time.GetMonth())&&(nDay==time.GetDay())){
		return false;
	}
	return true;
}

LRESULT CGameManager::OnEndWebCall(WPARAM &wParam, LPARAM &lParam)
{
	if(m_pMainWnd)
	{
		m_pMainWnd->SetTimer(TIMER_MAINFRAME_WEBCALL, 500, NULL);
		ErrorLogConsole("SetTimer : WEBCALL_TIMER");
	}

	return 0;
}

int CGameManager::GetTodayFrom2019()
{
	// 2020년부터 1000씩 증가. ex)2021년 1월 21일은 2021
	return (COleDateTime::GetCurrentTime().GetYear()-2019) * 1000 + COleDateTime::GetCurrentTime().GetDayOfYear();
}

int CGameManager::GetMinuteOfDay()
{
	return COleDateTime::GetCurrentTime().GetHour()*60 + COleDateTime::GetCurrentTime().GetMinute();
}

bool CGameManager::GetAcceptMorningCheck()
{
	int acceptDay = NMBASE::UTIL::GetProfile_int(GM().GetMyInfo()->UI.ID, "AcceptMorningCheckDay",-1);
	int acceptMinute = NMBASE::UTIL::GetProfile_int(GM().GetMyInfo()->UI.ID, "AcceptMorningCheckMinute",-1);

	if(GM().GetTodayFrom2019() - acceptDay == 1 && GM().GetMinuteOfDay() > acceptMinute)
		return false;

	if(GM().GetTodayFrom2019() - acceptDay > 1)
		return false;

	return true;
}
