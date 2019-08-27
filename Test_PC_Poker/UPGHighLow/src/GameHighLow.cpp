#include "StdAfx.h"

#include "GameHighLow.h"
#include "GlobalGame.h"
#include "SpriteList.h"
#include "SelectCardHighLow.h"
#include "GameViewHighLow.h"
#include "GameProcessHighLow.h"
#include "CommMsgDef_Game.h"
#include "Resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CGameHighLow::CGameHighLow(void)
{	
}

CGameHighLow::~CGameHighLow(void)
{
#ifdef TRACE_OUTPUT
	TTrace::Stop();
#endif
}

BOOL CGameHighLow::Create(CWnd *pParentWnd)
{
#ifdef TRACE_OUTPUT
	//TTrace::Init();
	//TTrace::ClearAll();
	TTrace::Options()->sendMode = Socket ;
	TTrace::Show(true) ;
	TTrace::Debug()->Send ("7Poker시작-Create", "CGameHighLow");   // single byte string	
#endif

	// 메인 게임 View를 생성한다.
	CGameViewHighLow* pGameView = new CGameViewHighLow(this);
	g_pGameView		= pGameView;
	m_pGameViewBase	= pGameView;

	CRect rect(0, 0, 1000, 740);
	pGameView->Create(NULL, "GameView", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rect, pParentWnd, 12345);
	pGameView->ShowWindow(SW_HIDE);

	// Create GameProcess
	CGameProcessHighLow *pGP = new CGameProcessHighLow(this);
	m_pGameProcess = pGP;
	m_pGameProcess->Init(&g_pGameView->Page);


	//add functor msg
	MM().AddMsgH(UPG_CMK_GET_MY_LOWJOKBO_NAME, Fnt(this, &CGameHighLow::OnGetMyLowJokboName));
	MM().AddMsgH(UPG_CMK_GET_LOW_JOKBO_NAME, Fnt(this, &CGameHighLow::OnGetLowJokboName));
	

	return CGame::Create(pParentWnd);
}


CGameProcessHighLow* CGameHighLow::GetGameProcess()
{
	return (CGameProcessHighLow*) m_pGameProcess;
}

// 방생성 다이얼로그
LRESULT CGameHighLow::ShowCreateRooomDlg(ROOMINFO *pRoomInfo, ROOMOPTION *pWebRoomOption)
{
	CCreateRoomDlg dlg(GetGameType(), GM().GetMainWnd());
	dlg.SetWebSelectedRoomOption(pWebRoomOption);
	LRESULT nRet = dlg.DoModal();
	
	if(nRet == IDOK)
	{
		strncpy_s(pRoomInfo->Title, 32, dlg.m_strRoomTitle, dlg.m_strRoomTitle.GetLength());

		pRoomInfo->FormKind			= dlg.m_nGameMode;		
		pRoomInfo->bIsFriendRoom	= dlg.m_bFriend;
		pRoomInfo->bSuperMaster		= PBIsSuperRoom();
		pRoomInfo->BetMoney			= dlg.m_i64SeedMoney;
		pRoomInfo->llEnterLimitMoney= dlg.m_i64MinimumMoney;
		//pRoomInfo->bVIPEnterLimitYN = dlg.m_bVIPEnterLimitYN;
		pRoomInfo->nAutoBetKind		= dlg.m_sAutoBet;
		pRoomInfo->sBetRuleKind			= dlg.m_sBetRuleKind;

		// 규제안 : 카드세팅추가.
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

// 바로입장 함수 ( 클라 -> 서버 )
int CGameHighLow::GetDirectEnterRoomType(int nType)
{
	return CHOICE_GAME;
}

void CGameHighLow::CheckDirectEnterRoomInfo(int nType, ROOMINFO *pRoomInfo)
{
	CString strTitle;
	MM().Call(UPG_CMK_GENERATE_RANDOM_ROOM_NAME, 0, (LPARAM)(&strTitle));
	strncpy_s(pRoomInfo->Title, 32, strTitle, strTitle.GetLength());
	pRoomInfo->bSuperMaster = PBIsSuperRoom();
}

// 빠른입장 다이얼로그
LRESULT	CGameHighLow::ShowQuickJoinerDlg(void)
{
	CQuickJoinDlg dlg(EQJ_7POKER, &g_cUIData, GM().GetMainWnd());
	LRESULT nRet = dlg.DoModal();

	if(nRet == IDOK)
	{		
		GM().SendQuickJoiner();
	}

	return FALSE;
}

LRESULT	CGameHighLow::SendQuickJoiner(void)
{
	if( OnMacroCheck() )
	{
		return FALSE;
	}
	
	//NMBASE::SOUND::g_Sound.PlayWav(SND45);

	/*bool bQJEnable = CQuickJoinDlg::IsEnableQJ(EQJ_7POKER);

	if(!bQJEnable)
	{
		GM().AskDirectEnterRoom(false, NULL, -1);
		return TRUE;

	}*/

	// 규제 개선안 작업
	// 바로 시작을 로비에서 호출하면, 무조건 랜덤으로 입장.
	GM().AskDirectEnterRoom(false, NULL, GetDefaultFormKind());

	// 설정 값 사용하는 부분은 일단 사용안함.
	//SCONDITION sInfo; 
	//CString strSection;
	//strSection.Format("QJ%02d_%s_%d", QJ_VER, GM().GetMyInfo()->UI.ID, EQJ_7POKER);

	//if(CQuickJoinDlg::GetSearchCondition(EQJ_7POKER, strSection, &sInfo))
	//{
	//	//내맘대로 바로입장
	//	//CCL_ASK_PREMQUICKJOIN pqj;
	//	//pqj.Set((IDX_GAME)EQJ_7POKER, bQJEnable, GM().GetMyInfo()->nSndFxKind, sInfo);
	//	//NMBASE::SOCK::GSOCKMAN().SendData( g_MainSrvSID, pqj.pData, pqj.GetTotalSize());

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

CPlayer *CGameHighLow::CreateNewPlayer(void)
{
	return new CPlayerHighLow;
}

void CGameHighLow::AddGameChat(const char *str, COLORREF crNewColor, bool bUserChat)
{	
	g_pChatWinMan->AddChat(str, crNewColor, bUserChat);
}


void CGameHighLow::AddGameInfo(const char *str, COLORREF crNewColor, bool bUserChat)
{	
	g_pChatWinMan->AddInfo(str, crNewColor, bUserChat);
}


void CGameHighLow::OnChangeFullScreen(bool bFullScreen)
{
	// ### [ 관전기능 ] ###
	g_pGameView->m_cGameViewBtnMananager.SetChangeDisplayButton(bFullScreen);
	g_pGameView->Page.ReCreateDIBSection();
}

void CGameHighLow::OnCloseOptionDlg(LRESULT nRet)
{
	if(nRet == IDOK) g_pGameView->m_cGameViewBtnMananager.SetBetBtnType(g_Config.eBetBtnType);
}

void CGameHighLow::OnLoadingEnd(void)
{
	// 스프라이트 로딩 후 초기화 ~
	GetGameProcess()->OnInit();
	GetGameView()->OnInit();	
}


CGameViewHighLow* CGameHighLow::GetGameView()
{
	return (CGameViewHighLow*) m_pGameViewBase;
}

void CGameHighLow::LoadSprite()
{	
	CGame::LoadSprite();
	LoadSpriteFile();
}

void CGameHighLow::LoadSound()
{
	CGame::LoadSound();
	RegistSoundFiles();
}

void CGameHighLow::ReleaseSprite()
{
	CGame::ReleaseSprite();
}

void CGameHighLow::ReleaseSound()
{
	CGame::ReleaseSound();
}

const IDX_GAME CGameHighLow::GetGameType(void)
{
	return IDX_GAME_HL;
}

const int CGameHighLow::GetMaxRoom(void)
{
	return 90;
}

const int CGameHighLow::GetMaxPlayer(void)
{
	return MAX_PLAYER;
}

const int CGameHighLow::GetMaxGamePlayer(void)
{
	return MAX_PLAYER;
}

const int CGameHighLow::GetMaxObserver(void)
{
	return MAX_OBSERVER;
}

const int CGameHighLow::GetJokboQuestCount(void)
{
	return 6;

}

const CString CGameHighLow::GetGameString(void)
{
	return "7cardhighlow";
}

const CString CGameHighLow::GetGameName(void)
{
	return "하이로우";
}
const COLORREF	CGameHighLow::GetGameColor(void)
{	
	return RGB(148,57,62);//각 게임 고유의 색을 반환
}


const int CGameHighLow::GetGameCode(void)
{
	return GAME_CODE_HL;
}

const CString CGameHighLow::GetLoadingBackPath(void)
{
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		return ".\\highlow\\image\\LoadingBack_toctoc.bmp";
	else
		return ".\\highlow\\image\\LoadingBack.bmp";
#else //UPGRADE_20120308_TOCTOC_CHANNELING
	return ".\\highlow\\image\\LoadingBack.bmp";
#endif //UPGRADE_20120308_TOCTOC_CHANNELING
}

const int CGameHighLow::GetDefaultFormKind(void)
{
	return FORMKIND_CHOICE;
}


// 네트웍 메시지를 받아 처리하는 함수(CClientDlg::OnPacketNotify 에서 게임에 관련된 패킷만 여기로 이동)
LONG CGameHighLow::OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData)
{	
	if (CGame::OnPacketNotify(pName, Signal, TotSize, lpData))
		return TRUE;

	if(g_cOneShotCharge.OnPacketNotify(pName,Signal, TotSize, lpData))
		return TRUE;
	
	if(g_pGameView->m_cGameHelper.OnPacketNotify(pName,Signal, TotSize, lpData)) 
		return TRUE;

	if(g_cOwnLimit.OnPacketNotify(pName,Signal, TotSize, lpData)) 
		return TRUE;

	if(g_cBanishMan.OnPacketNotify(pName, Signal, TotSize, lpData))
		return TRUE;

	if( g_cPromotionMan.OnPacketNotify(pName, Signal, TotSize, lpData) ) return TRUE;

	if( g_cVipJackpot.OnPacketNotify(pName, Signal, TotSize, lpData) ) return TRUE;

	switch(Signal)
	{
		// 게임시작버튼 활성화
	case SV_STARTBTNENABLE:
		ReceiveSV_STARTBTNENABLE(TotSize, lpData);		
		break;

		// 관전기능
	case SV_OBSERVER_MESSAGE:
		ReceiveSV_OBSERVER_MESSAGE(TotSize, lpData);
		break;

		// 관전기능(예약자 정보)
	case SV_RESERVATIONINFO:
		ReceiveSV_RESERVATIONINFO(TotSize, lpData);		
		break;
		
		// 관전기능(게임참여 -> 관전)
	case SV_ACCEPT_EMERAGE:
		ReceiveSV_ACCEPT_EMERAGE(TotSize, lpData);
		break;

		// 관전기능(관전 -> 게임참여)
	case SV_ACCEPT_PARTICIPATION:
		ReceiveSV_ACCEPT_PARTICIPATION(TotSize, lpData);		
		break;

		// 관전기능(게임참여 -> 관전)
	case SV_ENTER_EMERAGE:
		ReceiveSV_ENTER_EMERAGE(TotSize, lpData);
		break;

		// 관전기능(관전 -> 겜참여)
	case SV_ENTER_PARTICIPATION:
		ReceiveSV_ENTER_PARTICIPATION(TotSize, lpData);
		break;

		// 게임이 종료되었음
	case SV_GAMEOVERRESULT: 
		ReceiveSV_GAMEOVERRESULT(TotSize, lpData);			 
		break;

		/////////////////////////// 클라이언트용 메세지  ////////////////////////

		// 게임방 채팅데이타
	case CL_CHATDATA:
		ReceiveCL_CHATDATA(TotSize, lpData);		 
		break;		

	case SV_CHANGESTYLEROOMINFO:		  
		{	
			return OnReceiveChangeStyleRoomInfo( TotSize, lpData ); 
		} break; //BTS 모니터링 툴 방제 변경 패킷. 

		// 관전기능
	case CL_EXITSUBSCRIPT:
		ReceiveCL_EXITSUBSCRIPT(TotSize, lpData);		
		break;

		/////////////////////////// 게임 진행용 메세지  ////////////////////////

		// 게임 시작
	case SV_STARTCARD:
		ReceiveSV_STARTCARD(TotSize, lpData);		
		break;

		// 카드 선택
	case SV_SELECTCARD:
		ReceiveSV_SELECTCARD(TotSize, lpData);		
		break;

		// 유저 모두 카드날리기 종료
	case SV_WINDCARDEND: 
		ReceiveSV_WINDCARDEND(TotSize, lpData);		
		break;

		// 게임 경기중
	case SV_RAISE:
		ReceiveSV_RAISE(TotSize, lpData);			
		break;

		// 유저가 폴드
	case SV_FOLDUSER:
		ReceiveSV_DIEUSER(TotSize, lpData);		 
		break;

		// 마지막 히든 카드
	case SV_HIDDENCARD:
		ReceiveSV_HIDDENCARD(TotSize, lpData);		 		
		break;

		// 게임 종료
	case SV_ENDGAME:
		ReceiveSV_ENDGAME(TotSize, lpData);			
		break;

		// 잭팟
	case SV_SPECIALCARD :
		ReceiveSV_SPECIALCARD(TotSize, lpData);						
		break;

		// 자리 지킴이 패킷 처리
	case SV_SEATKEEPER:
		ReceiveSV_SEATKEEPER(TotSize, lpData);		
		break;

		// 돈이 변경 됨
	case SV_CHANGE_MONEY:
		ReceiveSV_CHANGE_MONEY(TotSize, lpData);				
		break;

		// 퇴실 요청 허가됨
	case SV_ACCEPT_OUTROOM: 
		ReceiveSV_ACCEPT_OUTROOM(TotSize, lpData);		
		break;

		// 내 정보가 변경됨
	case SV_CHANGEMYINFO: 
		ReceiveSV_CHANGEMYINFO(TotSize, lpData);			
		break;

		// 새로운 플레이어가 입실하였음
	case SV_USERENTERROOM:  
		ReceiveSV_USERENTERROOM(TotSize, lpData);		
		break;

		// 사용자가 방에서 퇴장했음
	case SV_USEROUTROOM: 
		ReceiveSV_USEROUTROOM(TotSize, lpData);				 
		break;

		// 방장이 바뀌어야함
	case SV_CHANGEROOMMASTER: 
		ReceiveSV_CHANGEROOMMASTER(TotSize, lpData);	
		break;

	case SV_WHISPERCHAT:
		ReceiveSV_WhisperChat(TotSize, lpData);
		break;

	case NM_ADMIN_WHISPERCHAT:	
		ReceiveSV_AdminWhisperChat(TotSize, lpData);
		break;

	case OPCODE_SV_JP_INFO :
		ReceiveSV_JACKPOTMONEYINFO(TotSize, lpData);
		break;

		//규제안 : 손실금액에 따른 경고창 오픈
	case SV_DAILY_LOSS_MONEY: // 규제안 패킷.
		ReceiveSV_LOSSMONEYINFO(TotSize, lpData);
		break ;

	case SV_REWARD_ALLIN_ANGEL:
		ReceiveSV_Reward_Allin_Angel(TotSize, lpData);
		break;

	//-----------------------(하이로우) (추가코드)-----------------------//	

	case SV_HL_ENDRAISE :	// 모든 레이즈 끝 => 하이 로우 스윙 결정 요청	
		ReceiveSV_HL_EndRaise(TotSize, lpData);
		break;

	case SV_HL_HLS_SEL :	// 하이, 로우, 스윙 버튼 클릭했음을 알림
		ReceiveSV_HL_HighLowSelect(TotSize, lpData);
		break;

		//규제안 : 대기하기 서버응답
	case SV_ASK_WAIT_INROOM:
		ReceiveSV_ASK_WAIT_INROOM(TotSize, lpData);
		break ;
	case SV_RECEIVE_CONG_MSG:
		ReceiveSV_CongNoti(TotSize, lpData);
		break;
	//case SV_PM_ODDEVEN_WINNER_NOTI:
	case SV_PM_WHEEL_WINNER_NOTI:
		ReceiveSV_PM_ODDEVEN_WINNER_NOTI(TotSize, lpData);
		break;
	case SV_ASK_CARDOPEN_RESULT:
		{
			CSV_ASK_CARDOPEN_RESULT msg;
			msg.Get(lpData, TotSize);

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
			
			g_pGameView->m_cGameViewBtnMananager.m_btnCardOpen.SetSprite(
				&(g_pGameView->m_cGameViewBtnMananager.m_sprUICardOpenBtn), nBtnNum);
			
			// 인게임 채팅에 알림
			AddGameChatViewEdit(csMsg, RGB(199, 117, 116)) ;
		}
		break;
	default:
		return CGame::OnPacketNotify(pName, Signal, TotSize, lpData );
		break;
	}

	return TRUE;
}

void CGameHighLow::ReceiveSV_Reward_Allin_Angel(int TotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	CSV_REWARD_ALLIN_ANGEL msg;
	msg.Get(lpData, TotSize);
	g_pGameView->m_cPlayerDrawManager.SetPlayAllinAngel((*msg.m_RemainEnableCount), timeGetTime());
}

//규제안 : 손실금액에 따른 경고창 오픈
void CGameHighLow::ReceiveSV_LOSSMONEYINFO(int TotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	CSV_DAILY_LOSS_MONEY msg;
	msg.Get(lpData,TotSize);

	//각 게임에서 참조하도록 GM에 값을 설정
	GM().SetLossmoneyInfo(*msg.llLossMoney, *msg.tBlockTime, *msg.eState, *msg.idxGame) ;

	g_pGameView->m_cGameViewBtnMananager.SetLossMoneyWarning() ;
}

void CGameHighLow::ReceiveSV_JACKPOTMONEYINFO(int TotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;


	packet::TSendPacket< jackpot::JP_MONEYINFO > JP_MONEY_Packet;
	JP_MONEY_Packet.Get( lpData, TotSize );
	jackpot::JP_MONEYINFO* pAlaram = static_cast< jackpot::JP_MONEYINFO* >( JP_MONEY_Packet.Various() );

	g_GAMEPROCESS()->JackPotMoneyUpdate(pAlaram);

// 	CSV_SEND_JACKPOTMONEYINFO msg;
// 	msg.Get(lpData, TotSize);			
// 
// 	g_GAMEPROCESS()->JackPotMoneyUpdate(msg.JackpotMoneyInfo);
}


void CGameHighLow::ReceiveSV_STARTBTNENABLE(int nTotSize, char *lpData)
{
	CSV_STARTBTNENABLE msg;
	msg.Get(lpData, nTotSize);			
	
	if(g_RI.State != EGAME_WAITING)
		return;

	if(g_RI.NowUserNum < LIMITGAMEPLAY_PLAYERNUM ) 
		return; 	
}


void CGameHighLow::ReceiveSV_OBSERVER_MESSAGE(int nTotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	CSV_OBSERVER_MESSAGE msg;
	msg.Get(lpData, nTotSize);

	CString str;
	CString chat;
	chat.Append((LPCTSTR)msg.StrChat, *msg.l_StrChat);
	/*chat = (LPCTSTR)msg.StrChat;
	chat = chat.Mid(0, *msg.l_StrChat);*/
	str.Format("%s", chat.operator LPCTSTR());
	str += "\n";
	AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_SERVMSG));

	int num = *msg.Kind;
	GetGameView()->m_cGameViewBtnMananager.SetObserverBtn(num);
	if(num >= 1 && num < 100)
	{
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
}

void CGameHighLow::ReceiveSV_RESERVATIONINFO(int nTotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	CSV_RESERVATIONINFO msg;
	msg.Get(lpData, nTotSize);

	CObserverDlg *pObserverDlg = GetObserverDlg();
	if(pObserverDlg)
	{
		pObserverDlg->User_ResetReservation();
	}

	for(int i = 0; i < *msg.TotNum; ++i) 
	{
		char *id = msg.ReservedInfo[i].ID;
		char *NickName = msg.ReservedInfo[i].ID;
		int unum = msg.ReservedInfo[i].UNum;
		int seq =  msg.ReservedInfo[i].Seq;
		if(pObserverDlg)
		{
			pObserverDlg->User_SetItemReservation(unum, id, seq);
		}
	}

	//관전UI
	g_ObserverMan.OnReceiveReservationInfo(&msg);
}

// 관전기능(게임참여 -> 관전)
void CGameHighLow::ReceiveSV_ACCEPT_EMERAGE(int nTotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	CSV_ACCEPT_EMERAGE msg;
	msg.Get(lpData, nTotSize);

	int nMyID = -1;
	int nowpnum = *msg.NowPNum;
	int newpnum = *msg.NewPNum;
	
	for(int i = 0; i < GetMaxNewPlayer(); ++i)
	{
		if(strlen(g_PLAYER(i)->UI.ID) > 0)
		{	
			if(strcmp(GM().GetMyInfo()->UI.ID, g_PLAYER(i)->UI.ID) == 0)
			{
				nMyID = i;
				break; 
			}
		}
	}

	BOOL bTrue = TRUE;
	if(newpnum < MAX_PLAYER || newpnum >= GetMaxNewPlayer())
	{
		bTrue = FALSE;
	}

	if(nMyID == -1 || nowpnum != g_PLAYER(0)->ServPNum || !bTrue) 
	{
		CSV_ASK_OUTROOM aormsg;
		aormsg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, REASON_OUTROOM_ABNORMAL);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), aormsg.pData, aormsg.GetTotalSize());
		return;
	}		

	g_pGameView->m_cGameViewBtnMananager.InitObserverBtn();
	
	// 플레이어 순서에 맞게 초기화한다			
	g_GAMEPROCESS()->SetPlayInfo(nowpnum, newpnum, FALSE);

	// 중복된 IP가 있는지 체크한다.
	for(int i = 0; i < MAX_PLAYER; ++i)
	{
		int pn = g_GAMEPROCESS()->GetPNum_ByServPN(i);
		if(strlen(g_PLAYER(pn)->UI.ID) > 0)	
		{
			g_PLAYER(pn)->MyIpCheck = msg.IPA->IpIndex[i];
		}
	}
}

// 관전기능(관전 -> 게임참여)
void CGameHighLow::ReceiveSV_ACCEPT_PARTICIPATION(int nTotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	CSV_ACCEPT_PARTICIPATION msg;
	msg.Get(lpData, nTotSize);

	int  nMyID   = -1;
	BOOL bTrue   = TRUE;
	int  nowpnum = *msg.NowPNum;
	int  newpnum = *msg.NewPNum;
	
	for(int i = 0; i < GetMaxNewPlayer(); ++i)
	{
		if(strlen(g_PLAYER(i)->UI.ID) > 0)
		{	
			if(strcmp(GM().GetMyInfo()->UI.ID, g_PLAYER(i)->UI.ID)==0) 
			{ 
				nMyID = i; 
				break;
			}
		}
	}

	
	if(newpnum < 0 || newpnum >= MAX_PLAYER)
	{
		bTrue = FALSE;
	}

	if(nMyID == -1 || nowpnum != nMyID || !bTrue) 
	{
		CSV_ASK_OUTROOM aormsg;
		aormsg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, REASON_OUTROOM_ABNORMAL);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), aormsg.pData, aormsg.GetTotalSize());
		return;
	}

	// 사운드 종류 세팅
	g_PLAYER(nMyID)->nSndFxKind = *msg.SndFxKind;	

	g_pGameView->m_cGameViewBtnMananager.InitObserverBtn();
	// 플레이어 순서에 맞게 초기화한다			
	g_GAMEPROCESS()->SetPlayInfo(nowpnum, newpnum, TRUE);

	// 중복된 IP가 있는지 체크한다.
	for(int i = 0; i < MAX_PLAYER; ++i) 
	{
		int pn = g_GAMEPROCESS()->GetPNum_ByServPN(i);
		if(strlen(g_PLAYER(pn)->UI.ID) > 0) 
		{
			g_PLAYER(pn)->MyIpCheck = msg.IPA->IpIndex[i];
		}
	}

	// 자리 지킴이
	g_SeatKeeper.OnEnterGame(); // 관전 -> 겜참여 일때 호출 

	// 보유한도
	g_cOwnLimit.OnEnterRoom(&g_cUIData, g_GAMEPROCESS());
}


void CGameHighLow::ReceiveSV_ENTER_EMERAGE(int nTotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	CSV_ENTER_EMERAGE msg;
	msg.Get(lpData, nTotSize);

	int nowpnum = *msg.NowPNum;
	int newpnum = *msg.NewPNum;

	if(nowpnum < 0 || nowpnum >= MAX_PLAYER) 
		return;

	if(newpnum < MAX_PLAYER || newpnum >= GetMaxNewPlayer())
		return;

	int pn = g_GAMEPROCESS()->GetPNum_ByServPN(nowpnum);
	if(strcmp( g_PLAYER(pn)->UI.ID, msg.ID) != 0) 
		return;

	g_PLAYER(newpnum)->SetNewPlayer(&g_PLAYER(pn)->UI);
	g_PLAYER(newpnum)->SetChangeRoomInfo(*g_PLAYER(pn)->GetChangeRoomInfo());
	g_PLAYER(newpnum)->UI.cObserver = 1;
	g_PLAYER(newpnum)->nSndFxKind = g_PLAYER(pn)->nSndFxKind;

	g_PLAYER(pn)->Clear();
	g_RI.NowUserNum -= 1;
	g_RI.NowObserverNum += 1;

	CString str;
	str.Format("◁참여 → 관전: [%s]님\n", g_PLAYER(newpnum)->UI.NickName);	
	AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

	// 중복된 IP체크한다.
	for(int i = 0; i < MAX_PLAYER; ++i)	
	{
		int nPlayerNumber = g_GAMEPROCESS()->GetPNum_ByServPN(i);
		if(strlen(g_PLAYER(nPlayerNumber)->UI.ID) > 0)	
		{
			g_PLAYER(nPlayerNumber)->MyIpCheck = msg.IPA->IpIndex[i];
		}
	}

	int ounum	= g_PLAYER(newpnum)->UI.UNum;
	int sex		= g_PLAYER(newpnum)->UI.Sex;
	int level	= g_PLAYER(newpnum)->UI.nIcon;
	INT64 Money = g_PLAYER(newpnum)->UI.GetMoney();
	int seq = 0;

	CObserverDlg *pObserverDlg = GetObserverDlg();
	if(pObserverDlg)
	{
		pObserverDlg->User_InsItem(ounum,g_PLAYER(newpnum)->UI.ID, g_PLAYER(newpnum)->UI.NickName, sex, level, Money , seq);
	}
} 


void CGameHighLow::ReceiveSV_ENTER_PARTICIPATION(int nTotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	CSV_ENTER_PARTICIPATION msg;
	msg.Get(lpData, nTotSize);

	int nowpnum = *msg.NowPNum;
	int newpnum = *msg.NewPNum;
	if(newpnum < 0 || newpnum >= MAX_PLAYER) 
		return;

	if(nowpnum < MAX_PLAYER || nowpnum >= GetMaxNewPlayer()) 
		return;

	// 관전의 pnum 인덱스가 서버와 일치한지 체크한다.
	if(strcmp( g_PLAYER(nowpnum)->UI.ID, msg.ID) != 0)
		return; 

	int pnum = -1; // 플레이어 번호
	if( !g_ObserverMan.IsMyObserver()) // 1) 내가 겜참여중이고 겜참여 넘 입장
	{
		for(int i = g_GAMEPROCESS()->m_nNo1PosPNum; i < MAX_PLAYER + g_GAMEPROCESS()->m_nNo1PosPNum; ++i) 
		{	
			int index = i;
			
			if(index>=MAX_PLAYER)
			{
				index = index - MAX_PLAYER;
			}
			
			//나
			if(index == 0) 
			{
				continue; 
			}

			if(strlen(g_PLAYER(index)->UI.ID)==0)
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

		g_pGameView->m_cPlayerDrawManager.Reset(pnum);
		g_PLAYER(nowpnum)->UI.cObserver = 0;		
		g_PLAYER(pnum)->SetNewPlayer(&g_PLAYER(nowpnum)->UI);
		g_PLAYER(pnum)->SetChangeRoomInfo(*g_PLAYER(nowpnum)->GetChangeRoomInfo());
		g_PLAYER(nowpnum)->Clear();
		g_pGameView->m_cPlayerDrawManager.SetShow(pnum, true);	// 이 값을 세팅해야 유저가 화면에 보인다.

		g_RI.NowUserNum += 1;
		g_RI.NowObserverNum -= 1;
		g_PLAYER(pnum)->ServPNum = g_GAMEPROCESS()->GetServPNum_ByPN(pnum);

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
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
	}	
	else	// 3)내가 관전중이고  겜참여 하는넘 입장
	{
		for(int i = 0; i < MAX_PLAYER; ++i)	
		{
			if(strlen(g_PLAYER(i)->UI.ID)==0)
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

		g_GAMEPROCESS()->m_nMyPosPNum = 0; // 서버쪽에 인덱스
		g_GAMEPROCESS()->m_nNo1PosPNum = g_GAMEPROCESS()->GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스

		// 방 정보를 수정하고 새로 참여한 플레이어의 정보를 저장
		g_pGameView->m_cPlayerDrawManager.Reset(pnum);
		g_PLAYER(nowpnum)->UI.cObserver = 0;		
		g_PLAYER(pnum)->SetNewPlayer(&g_PLAYER(nowpnum)->UI);
		g_PLAYER(pnum)->SetChangeRoomInfo(*g_PLAYER(nowpnum)->GetChangeRoomInfo());
		g_PLAYER(nowpnum)->Clear();
		g_pGameView->m_cPlayerDrawManager.SetShow(pnum, true);	// 이 값을 세팅해야 유저가 화면에 보인다.
		g_RI.NowUserNum += 1;
		g_RI.NowObserverNum -= 1;

		g_PLAYER(pnum)->ServPNum	   = g_GAMEPROCESS()->GetServPNum_ByPN(pnum);	// 서버측 플레이어 번호 저장
		
		CString str;
		str.Format("▶관전 → 참여: [%s]님\n", g_PLAYER(pnum)->UI.NickName);
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
	}

	// 중복된 IP를 체크한다.
	for(int i = 0; i < MAX_PLAYER; ++i)
	{
		int pn = g_GAMEPROCESS()->GetPNum_ByServPN(i);
		if(strlen(g_PLAYER(pn)->UI.ID) > 0)	
		{
			g_PLAYER(pn)->MyIpCheck = msg.IPA->IpIndex[i];
		}
	}

	// 사운드를 추가한다.
	g_PLAYER(pnum)->nSndFxKind = *msg.SndFxKind;

	CObserverDlg *pObserverDlg = GetObserverDlg();
	if(pObserverDlg)
	{
		pObserverDlg->User_DelItem(g_PLAYER(pnum)->UI.UNum, g_PLAYER(pnum)->UI.ID);
	}

}

// 결과값을 받아 처리하는 함수.
void CGameHighLow::ReceiveSV_GAMEOVERRESULT(int nTotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	CSV_GAMEOVERRESULT msg;
	msg.Get(lpData, nTotSize);

	GM().SetWaitResponse(FALSE);

	static BOOL bProccessed = FALSE;
	if(bProccessed != FALSE)
	{
		return;
	}
	bProccessed = TRUE;

	g_pGameView->m_cPlayerDrawManager.ClearTimer();
	
	g_GAMEPROCESS()->OnGameOver(msg.GOR);
	//g_GAMEPROCESS()->OnResultGameOver();

	g_cOwnLimit.OnGameOver();

	bProccessed = FALSE;
}


void CGameHighLow::ReceiveCL_CHATDATA(int nTotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	CCL_CHATDATA msg;
	msg.Get(lpData, nTotSize);
	
	if(*msg.RoomNum != g_RI.RoomNum) 
		return;

	// 대화 수신 거부중인 아이디이면 무시
	int pnum = g_GAMEPROCESS()->GetPlayerPNum(msg.ID);
	BOOL bAdmin = strcmp("관리자",msg.ID)==0;


	if(g_cUIData.m_bObserverChat)
	{
	}
	else
	{
		//관전자 채팅 끄기 
		if(strncmp(msg.ID, GM().GetMyInfo()->UI.ID, 15)==0)
		{
			/*if(g_ObserverMan.IsMyObserver() && g_GAMEPROCESS()->m_bObserverChatWarning==FALSE)
			{
				AddGameChatViewEdit("[알림] 관전자 채팅끄기를 설정하신 분께는 관전채팅 내용이 보여지지 않습니다.\n", GetChatColor(ECC2_NOTIFY));
				g_GAMEPROCESS()->m_bObserverChatWarning=TRUE;
			}*/
		}
		else
		{			
			//관전 채팅  0 보기 1 안보기
			if(g_Config.bNoObserverChat && !bAdmin )
			{					
				if(pnum>=MAX_PLAYER && pnum < GetMaxNewPlayer()-1)
				{					
					return;
				}					
			}
		}
	}


	CString str;
	char chat[256]={0,};
	strncpy(chat, msg.StrChat, min(*msg.l_StrChat, 255));

	//(추가)
	// 허용되지 않은 특수문자를 사용하였다면 공백문자로 바꿈
	int slen = strlen(chat);
	for(int s = 0; s < slen-1; ++s) 
	{
		// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
		if(chat[s] == 8 || chat[s] == 9 || chat[s] == 10 || chat[s] == 13)
		{
			chat[s] = ' ';
		}
	}	

	char szTBuf[512] = {0, };
	NMBASE::UTIL::NMIsBadToRep( chat, szTBuf, 512 );

	if( bAdmin )// ### [관리자 모드 작업] ###
	{				
		str.Format("관리자> %s\n",szTBuf);
		AddGameChatViewEdit(str.GetString(), GetChatColor(ECC2_SERVMSG));
	}
	else
	{	

		if(pnum < 0 )
		{
			return;
		}

		if(g_cUIData.m_bObserverChat)
		{
			if(g_ObserverMan.IsMyObserver())
			{
				if( pnum >= g_GAMEPROCESS()->GetMaxPlayer())
				{
					str.Format("%s> %s\n", g_PLAYER(pnum)->UI.NickName , szTBuf);
					AddGameChatViewEdit(str, GetChatColor(ECC2_CHAT), true);
				}
			}
			else
			{
				if( pnum < g_GAMEPROCESS()->GetMaxPlayer())
				{
					str.Format("%s> %s\n", g_PLAYER(pnum)->UI.NickName , szTBuf);
					AddGameChatViewEdit(str, GetChatColor(ECC2_CHAT), true);
				}
			}
		}
		else
		{
			str.Format("%s> %s\n", g_PLAYER(pnum)->UI.NickName , szTBuf);
			AddGameChatViewEdit(str.GetString(), GetChatColor(ECC2_CHAT), true);
		}

		str.Format("## ID: %s(%s)> %s\n",g_PLAYER(pnum)->UI.ID , g_PLAYER(pnum)->UI.NickName , szTBuf );
	}

	
	
	AddLobbyHideChatText(&str);
}


void CGameHighLow::ReceiveCL_EXITSUBSCRIPT(int nTotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	CCL_EXITSUBSCRIPT msg;
	msg.Get(lpData, nTotSize);
	
	int pnum = g_GAMEPROCESS()->GetPlayerPNum(msg.ID);
	if(pnum==-1)
		return;

	int bexit = *msg.bExit;

	CString str;

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
		//게임 중 나가기 예약 상태
		if(strcmp(GM().GetMyInfo()->UI.ID, msg.ID) == 0)
		{
			g_pGameView->m_bExitSubscript = TRUE;
		}		
					
		str.Format("[%s]님 나가기 예약\n", g_PLAYER(pnum)->UI.NickName);
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

		g_pGameView->m_cPlayerDrawManager.SetExitReservation(pnum, true);		
	}
	else if(bexit == 2)
	{
		//나가기 취소 
		if(strcmp(GM().GetMyInfo()->UI.ID, msg.ID) == 0)
		{
			g_pGameView->m_bExitSubscript = FALSE;
		}				

		str.Format("[%s]님 나가기 취소\n", g_PLAYER(pnum)->UI.NickName);
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

		g_pGameView->m_cPlayerDrawManager.SetExitReservation(pnum, false);
	}
}

void CGameHighLow::ReceiveSV_STARTCARD(int nTotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	CSV_STARTCARD msg;
	msg.Get(lpData, nTotSize);

	int pnum = g_GAMEPROCESS()->GetPlayerPNum(msg.ID);
	if(pnum == -1) 
		return;

	if(strcmp(GM().GetMyInfo()->UI.ID, msg.ID) != 0)
		return;

	// 전광판 출력
	g_pGameView->m_cDisplayBoard.SetText(EDBT_GAMESTART);
	
	// 사이드팟 초기화
	if(g_pGameView)
	{
		g_pGameView->m_cSidePotMoney.Reset();
	}

	static BOOL bProccessed = FALSE;
	if(bProccessed)
		return;
	bProccessed = TRUE;						

	// 게임 리셋 예약
	g_GAMEPROCESS()->ResetGame();
	g_GAMEPROCESS()->GameStart(msg.pSC);
	g_RI.State = EGAME_PLAYING;			

	CWnd * pWnd = GM().GetMainWnd()->GetForegroundWindow();
	if(pWnd != NULL)
	{
		if(pWnd != GM().GetMainWnd())
		{
			GM().GetMainWnd()->FlashWindow(TRUE);
		}				
	}

	bProccessed = FALSE;
}

void CGameHighLow::ReceiveSV_SELECTCARD(int nTotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	if(g_RI.State == EGAME_WAITING) 
		return;

	CSV_SELECTCARD msg;
	msg.Get(lpData, nTotSize);

	int pnum = g_GAMEPROCESS()->GetPlayerPNum(msg.ID);
	if(pnum == -1) 
		return;

	if(pnum < 0  || pnum >= MAX_PLAYER) 
		return;
	
	if( g_PLAYER(pnum)->JoinState == EGAME_JOINOUT )
		return;
	
	if(!g_pGameView->m_cPlayerDrawManager.IsSelectCard(pnum))
		return;

	if(g_RI.FormKind == CHOICE_GAME && g_PLAYER(pnum)->m_nTotalCardCount == 3)
	{		
		g_pGameView->m_cPlayerDrawManager.SetSelectCard(pnum, false);
		return;
	}

	if(!g_ObserverMan.IsMyObserver())
	{
		///////////////////////////////////////////////////////////////////
		// 오픈카드 선택시 각각 타이머 보여준다.
		if(g_GAMEPROCESS()->m_bGameStart && pnum != 0 && g_RI.FormKind == CHOICE_GAME && g_PLAYER(pnum)->PlayState == EPLAYER_GAMEPLAYING)
		{
			// 추가 초이스 카드
			g_PLAYER(pnum)->m_cMyCard[3].Clear();
			g_PLAYER(pnum)->m_nTotalCardCount -= 1;
		}
	}	

	/////////////////////////////////////////////////////////////////////	
	g_pGameView->m_cPlayerDrawManager.SetSelectCard(pnum, false);

	if(pnum != 0)
	{
		g_PLAYER(pnum)->SetChioceCardMove();
	}
}

// 카드 분배가 다 끝났다는 메시지를 처리 한다.
void CGameHighLow::ReceiveSV_WINDCARDEND(int nTotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	if(g_RI.State == EGAME_WAITING) 
		return;

	CSV_WINDCARDEND msg;
	msg.Get(lpData, nTotSize);
	
	int pnum = *msg.Reserve;
	if(pnum < 0 || pnum >= MAX_PLAYER) 
		return;

	static BOOL bProccessed = FALSE;
	if(bProccessed)
		return;

	bProccessed = TRUE;

	g_GAMEPROCESS()->EndWindCard();

	bProccessed = FALSE;
}


void CGameHighLow::ReceiveSV_RAISE(int nTotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	if(g_RI.State == EGAME_WAITING)
		return;		

	CSV_RAISE msg;
	msg.Get(lpData, nTotSize);

	g_pGameView->m_cPlayerDrawManager.ClearTimer();
	
	g_GAMEPROCESS()->Raise(msg.pPG);
}

// 죽은유저를 받아서 처리하는 함수
void CGameHighLow::ReceiveSV_DIEUSER(int nTotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	if(g_RI.State == EGAME_WAITING)
		return;

	CSV_FOLDUSER msg;
	msg.Get(lpData, nTotSize);
	
	if(strlen(msg.ID)==0) 
		return;

	int pnum  = -1;
	for(int i = 0; i < MAX_PLAYER; ++i) 
	{
		if(strcmp(msg.ID, g_PLAYER(i)->UI.ID) == 0) 
		{ 
			pnum = i; 
			break; 
		}
	}

	if(pnum == -1) 
	{
		return;
	}


	//char szTemp1[256] = {0, };
	//sprintf(szTemp1, "CGameHighLow::ReceiveSV_DIEUSER => 플레이어:%d\n", pnum);
	//OutputDebugString(szTemp1);
	//TTrace::Debug()->Send ("CGameHighLow::ReceiveSV_DIEUSER", szTemp1);   // single byte string	
	//AddGameChatViewEdit(szTemp1, GetChatColor(ECC2_RESULT_JOKBO));

//#ifdef TRACE_OUTPUT
//	TTrace::Debug()->Send ("CGameHighLow::ReceiveSV_DIEUSER", szTemp1);   // single byte string	
//#endif

	//g_pGameView->m_JackpotMan.m_JackPotData.nJackPotMoney = *msg.JackPotMoney;

	// 현재 겜참여 중인가
	if(g_PLAYER(pnum)->PlayState == EPLAYER_GAMEPLAYING && g_PLAYER(pnum)->JoinState == EGAME_JOININ) 
	{ 
		g_PLAYER(pnum)->bFold	  = TRUE;
		g_PLAYER(pnum)->PlayState = EPLAYER_GAMEEND;
		
		if( !g_ObserverMan.IsMyObserver() && pnum == 0) 
		{ 
			GM().GetMyInfo()->bFold		= TRUE;
			GM().GetMyInfo()->PlayState = EPLAYER_GAMEEND;			
			
			g_SelectCard().EndSelectCard(TRUE);			
			g_pGameView->m_cPlayerDrawManager.SetSelectCard(pnum, false);
		}

		g_PLAYER(pnum)->m_cPlayerEffectList.Destroy();

		//현재 겜중인 넘 나갔다.
		g_GAMEPROCESS()->m_nStartUserNum--;
		if(g_GAMEPROCESS()->m_nStartUserNum <0)
		{
			g_GAMEPROCESS()->m_nStartUserNum = 0;				
		}

		if(g_PLAYER(pnum)->GetCardTotal() > 0) 
		{
			for(int k = 0; k < g_PLAYER(pnum)->GetCardTotal(); ++k) 
			{
				if( pnum == 0 && !g_ObserverMan.IsMyObserver() )
				{							
					g_PLAYER(pnum)->SetCardBackShow(k);							
				}						
				else
				{
					g_PLAYER(pnum)->SetCardBackShow(k);							
				}
			}
		}

		g_GAMEPROCESS()->SoundFxKind(1, g_PLAYER(pnum)->UI.Sex);// ### [사운드 추가 작업] ### 다이			

		char szMsg[ MAX_PATH ] = { 0,};
		CString str;
		sprintf_s( szMsg, g_StrMan.Get(_T("FOLD_USER")), g_PLAYER(pnum)->UI.NickName );
		AddGameInfoViewEdit( szMsg, GetChatColor(ECC2_NOTIFY));

		//카드 선택시 시간 초과로 다이 했을때 이미지를 따로 찍어준다.
		if( g_Poker.nState == RSTATE_PREPARESTART || g_Poker.nState == RSTATE_EXTRAFIRSTBET)
		{
			CBettingEffect *pObj = new CBettingEffect();
			pObj->Init(pnum, ANF_BET_CMD, ANF_BET_DIE);
			g_pGameView->m_cAniSprListManager.ADDObject(pObj);
		}
	}
}

// 히든카드를 감춘다.
void CGameHighLow::ReceiveSV_HIDDENCARD(int nTotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	if(g_RI.State != EGAME_PLAYING) 
		return;

	CSV_HIDDENCARD msg;
	msg.Get(lpData, nTotSize);

	if(strlen(msg.ID)==0)
		return;
	if( g_ObserverMan.IsMyObserver() ) 
		return;

	int pnum = -1;
	for(int i = 0; i < MAX_PLAYER; ++i) 
	{
		if(strcmp(msg.ID, g_PLAYER(i)->UI.ID) == 0) 
		{ 
			pnum = i;
			break; 
		}
	}

	if(pnum != 0) 
	{
#ifdef _DEBUG
		AfxMessageBox("Hidden Error");			
#endif
		return;
	}

	if(g_PLAYER(pnum)->PlayState == EPLAYER_GAMEPLAYING && g_PLAYER(pnum)->JoinState == EGAME_JOININ)
	{
		g_PLAYER(pnum)->m_cMyCard[6].SetCardNum(*msg.nCard) ; // 값 변경
		g_PLAYER(pnum)->m_cMyCard[6].SetHiddenCard(TRUE);
	}
	
	// 자신의 카드위에 족보명을 보위기 위함
	g_pGameView->m_cPlayerDrawManager.MyJokboNameUpdate();
	//NMBASE::SOUND::g_Sound.PlayOggStream(SND_HIDDEN_BGM);
}


void CGameHighLow::ReceiveSV_ENDGAME(int nTotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
	{
		return;
	}

	CSV_ENDGAME msg;
	msg.Get(lpData, nTotSize);
	
	if(g_pGameView)
	{
		// 사이드팟 초기화
		g_pGameView->m_cSidePotMoney.Reset();

		// Animan moment destroy
		g_pGameView->m_cAniSprListManager.Destroy(0);

		//yoo [20150120] 
		if(TRUE==CGame::OnReceiveResEndGame())
		{
			ExitGameRequest();
		}
		if(g_pGameView->m_bExitSubscript) // 나가기 예약상태면 게임 종료
		{
			ExitGameRequest();
		}
		else
		{
			// 게임 리셋 예약
			GetGameProcess()->ResetGame();
		}
	}
	
	if( !g_ObserverMan.IsMyObserver() && g_GAMEPROCESS()->GetPlayerNP(0)->m_nCardChoiceLimtCnt >= MAX_TURN_TIMEOUT_LIMIT_COUNT )
	{
		g_GAMEPROCESS()->GetPlayerNP(0)->m_nCardChoiceLimtCnt = 0;
		ExitGameRequest(true);
		CString strOutput;
		strOutput.Format("게임 시작 후 %d회 이상 수동으로 카드 선택을 하지 않아 게임방에서 자동 퇴장 하였습니다.\n", MAX_TURN_TIMEOUT_LIMIT_COUNT);
		ShowMainMessageDlg(strOutput);
	}
	g_cPromotionMan.OnEndGame();

	//매크로 프로그램이 있으면 종료 시킨다
	OnMacroCheck();
}

void CGameHighLow::ReceiveSV_SPECIALCARD(int nTotSize, char *lpData)
{	
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	CSV_SPECIALCARD msg;
	msg.Get(lpData, nTotSize);		

	CString spec("");

	if(msg.pSC->nCard==CODE_SPECIAL_FCARD)
		spec = "☆포카드☆";
	else if(msg.pSC->nCard==CODE_SPECIAL_SF)
		spec = "☆스트레이트 플러쉬☆";
	else if(msg.pSC->nCard==CODE_SPECIAL_RSF)
		spec ="★로얄 스트레이트 플러쉬★";    


	CString str("");
	CString strNick = msg.pSC->NickName;
	//같은방에서 나왔는지 체크		

	if(msg.pSC->JackPotPlus>0)
	{
		CString strM = NumberToOrientalString(msg.pSC->JackPotPlus);
		strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
		str.Format(g_StrMan.Get(_T("MES_SPE2")), msg.pSC->ChanName, msg.pSC->nRoom, strNick, spec, strM);
	}
	else
	{
		str.Format(g_StrMan.Get(_T("MES_SPE3")), msg.pSC->ChanName, msg.pSC->nRoom, strNick, spec);
	}

	AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
}

void CGameHighLow::ReceiveSV_SEATKEEPER(int nTotSize, char *lpData)
{
	CSV_SEATKEEPER msg;
	msg.Get( lpData, nTotSize );
	g_SeatKeeper.ProcessPacket(&msg);
}

void CGameHighLow::ReceiveSV_CHANGE_MONEY(int nTotSize, char *lpData)
{
	CSV_CHANGE_MONEY msg;
	msg.Get(lpData,nTotSize);
	g_GAMEPROCESS()->Change_Money(&msg);
}

void CGameHighLow::ReceiveSV_ACCEPT_OUTROOM(int nTotSize, char *lpData)
{
	CSV_ACCEPT_OUTROOM msg;
	msg.Get(lpData, nTotSize);
	
	GM().SetWaitResponse(false);
	GM().OnExitRoom((REASON_OUTROOM)*msg.Reason);		

	g_cPromotionMan.Accept_OutRoom();
} 


void CGameHighLow::ReceiveSV_CHANGEMYINFO(int nTotSize, char *lpData)
{
	CSV_CHANGEMYINFO msg;
	msg.Get(lpData, nTotSize);

	memcpy( &GM().GetMyInfo()->UI.AllGameRecord, &msg.pCMI->AllGameRecord, sizeof(msg.pCMI->AllGameRecord) );
	memcpy( &g_PLAYER(0)->UI.AllGameRecord, &msg.pCMI->AllGameRecord, sizeof(msg.pCMI->AllGameRecord) );		
	GM().GetMyInfo()->UI.SetMoney( msg.pCMI->PMoney );
	//GM().GetMyInfo()->UI.PMoney = msg.pCMI->PMoney;
	if(GM().GetMyInfo()->UI.GetMoney() < 0)
	{
		GM().GetMyInfo()->UI.SetMoney( 0 );
	}

	g_PLAYER(0)->UI.SetMoney( GM().GetMyInfo()->UI.GetMoney() );
	g_PLAYER(0)->UI.nIcon  = GM().GetMyInfo()->UI.nIcon = GetMoneyGrade( GM().GetMyInfo()->UI.GetTotalMoney() );
}

void CGameHighLow::ReceiveSV_USERENTERROOM(int nTotSize, char *lpData)
{
	CSV_USERENTERROOM msg;  
	msg.Get(lpData, nTotSize);

	g_GAMEPROCESS()->User_EnterRoom(&msg);
}

void CGameHighLow::ReceiveSV_USEROUTROOM(int nTotSize, char *lpData)
{
	CSV_USEROUTROOM msg;
	msg.Get(lpData, nTotSize);

	g_GAMEPROCESS()->User_OutRoom(&msg);
}

void CGameHighLow::ReceiveSV_CHANGEROOMMASTER(int nTotSize, char *lpData)
{
	CSV_CHANGEROOMMASTER msg;
	msg.Get(lpData, nTotSize);

	if(strlen(msg.ID)==0)
		return;

	BOOL bBreak = TRUE;

	// 닉네임수정 	
	CString NickName="";
	for(int i = 0; i < MAX_PLAYER; ++i)
	{
		if(strcmp(msg.ID, g_PLAYER(i)->UI.ID) == 0) 
		{ 
			NickName = g_PLAYER(i)->UI.NickName;
			bBreak = FALSE; 
			break; 
		}
	} 

	if(bBreak == TRUE)
		return;

	// 방 정보를 변경한다
	strcpy(g_RI.ID, msg.ID);
	// 닉네임수정 	
	strcpy(g_RI.NickName , NickName );		
	//여기까지
	g_RI.UNum = *msg.UNum;			
	g_RI.RoomMasterType = *msg.RoomMasterType;

	if(strcmp(msg.ID, GM().GetMyInfo()->UI.ID) == 0) // 내가 방장이 된다
	{
		g_MastPN = 0;

		
	}
	else
	{
		int newMastPN = -1;
		for(int i = 1; i < MAX_PLAYER; ++i) 
		{
			if(strcmp(g_PLAYER(i)->UI.ID, msg.ID)==0) 
			{
				newMastPN=i; 
				break;
			}
		}

		if(newMastPN == -1) 
			return;

		g_MastPN = newMastPN;		

		
	}

	CString str;
	str.Format("방장변경: [%s]님\n", NickName);
	AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

	str.Format("방장변경:[ID: %s(%s)]님\n",msg.ID , NickName );
	AddLobbyHideChatText( &str );

	g_cBanishMan.Clear();
}

//귓속말
BOOL CGameHighLow::ReceiveSV_WhisperChat(int TotSize, char *lpData)
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
	int pnum = g_GAMEPROCESS()->GetPlayerPNum(msg.m_szID);

	CString str;
	CString chat;
	chat.Append((LPCTSTR)msg.m_StrChat, *msg.m_sStrChatLen);
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

	// 닉네임 작업 2006/11/08		
	char szTBuf[512] = {0, };
	NMBASE::UTIL::NMIsBadToRep((char *)(LPCTSTR)chat, szTBuf, 512 );
	str.Format("[귓말도착:%s] %s\n", msg.m_szNickName, szTBuf);

	// 귓말 알림 효과음 출력
	NMBASE::SOUND::g_Sound.PlayWav(SND_WHISPER);
	AddGameChatViewEdit(str, GetChatColor(ECC2_WHISPER_RECEIVE), true);

	//귓말사용자추가
	AddChattingWhisperList( msg.m_szNickName, false, true, false );
	//g_pChatWinMan->AddWhipserUser( msg.m_szNickName );

	// 닉네임 작업 2006/11/08
	str.Format("귓말: ID : %s(%s)> %s\n", msg.m_szID ,  msg.m_szNickName , szTBuf );
	AddLobbyHideChatText(&str);

	return TRUE;
} 

BOOL CGameHighLow::ReceiveSV_AdminWhisperChat(int TotSize, char *lpData)
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

// 게임이 바뀔때마다 호출이 되는 함수.
void CGameHighLow::OnFocusGame(bool bFocus)
{
	if(bFocus)
	{
		// 채팅
		if(g_pChatWinMan)
		{
			g_pChatWinMan->SetCurrnetChat();
		}
		
		// 자리지킴이		
		g_cOwnLimit.Init( &g_cUIData, g_GAMEPROCESS());
		g_cBanishMan.Init(&g_SeatKeeper, &g_ObserverMan);
		g_SeatKeeper.SetChangeThisPoint();

		MM().AddMsgH(UPG_CMK_SET_LIMIT_MONEY,  Fnt(this, &CGameHighLow::OnSetLimitMoney));
		MM().AddMsgH(UPG_CMK_SET_LIMIT_GOLD,  Fnt(this, &CGameHighLow::OnSetLimitGold));
		MM().AddMsgH(UPG_CMK_GET_MONEY_GRADE,  Fnt(this, &CGameHighLow::OnGetMoneyGrade));
		MM().AddMsgH(UPG_CMK_GET_JOKBO_NAME,   Fnt(this, &CGameHighLow::OnGetJokboName));
		MM().AddMsgH(UPG_CMK_GET_MYJOKBO_NAME, Fnt(this, &CGameHighLow::OnGetMyJokboName));
		MM().AddMsgH(UPG_CMK_GET_RESULT_MONEY, Fnt(this, &CGameHighLow::OnGetResultMoney));
		MM().AddMsgH(UPG_CMK_CLEAR_JACKPOTMONEY, Fnt(this, &CGameHighLow::OnClearJackPotMoney));
		MM().AddMsgH(UPG_CMK_SET_PLAY_RAKEBACK_GOLD, Fnt(this, &CGameHighLow::OnSetPlayRakeBackGold));
		MM().AddMsgH(UPG_CMK_SET_PLAY_RAKEBACK_GOLD_EVENT, Fnt(this, &CGameHighLow::OnSetPlayRakeBackGoldEvent));				

		MM().AddMsgH(UPG_CMK_GAME_RESULT_OPEN, Fnt(this, &CGameHighLow::OnGetResultOpen));	

		g_cPromotionMan.Reset();
		g_cPromotionMan.Init(&g_cUIData, this, &g_pGameView->m_cChatWindow);
		g_cPromotionMan.MakePromotion(CPromotionManager::EPT_JOKBOJACKPOT);		// 족보잭팟은 결과출력 타이밍을 위해 미리등록해 놓는다.
		
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

LRESULT CGameHighLow::OnSetPlayRakeBackGold(WPARAM &wParam, LPARAM &lParam)
{
	GetGameView()->m_cPlayerDrawManager.SetPlayRakeBackGold((INT64)wParam, (int)lParam);
	return TRUE;
}

LRESULT CGameHighLow::OnSetPlayRakeBackGoldEvent(WPARAM &wParam, LPARAM &lParam)
{
	GetGameView()->m_cPlayerDrawManager.SetPlayRakeBackGoldEvent((int)wParam);
	return TRUE;
}

LRESULT CGameHighLow::OnClearJackPotMoney(WPARAM &wParam, LPARAM &lParam)
{
	int  nGameCode     = *((int *)wParam);
	INT64 llLimitMoney = *((INT64 *)lParam);	

	g_GAMEPROCESS()->ClearJackpotMoney(nGameCode, llLimitMoney);

	return TRUE;
}


LRESULT CGameHighLow::OnGetResultMoney(WPARAM &wParam, LPARAM &lParam)
{
	INT64 *pMoney = (INT64 *)lParam;
	CPlayerHighLow *pPlayer = g_PLAYER((int)wParam);
	*pMoney = pPlayer->m_n64ResultMoney;
	return TRUE;
}

LRESULT CGameHighLow::OnGetMyJokboName(WPARAM &wParam, LPARAM &lParam)
{
	//나의 하이 족보 반환
	CString *pStrJokbo = (CString *)lParam;	
	int *nSevenValue = (int*) wParam;

	if( g_GAMEPROCESS() == NULL )
		return FALSE;

	if(g_GAMEPROCESS()->GetSevenRuleManager().m_cRule[0].isValid())
	{
		*pStrJokbo = g_GAMEPROCESS()->GetSevenRuleManager().m_cRule[0].get_RuleValue_to_Name3();		
	}
	else
		*pStrJokbo = "";

	if (nSevenValue)
		*nSevenValue = g_GAMEPROCESS()->GetSevenRuleManager().m_cRule[0].get_sevenValue();

	return TRUE;
}

LRESULT CGameHighLow::OnGetMyLowJokboName(WPARAM &wParam, LPARAM &lParam)
{
	//나의 로우 족보 반환
	CString *pStrJokbo = (CString *)lParam;	
	int *nSevenValue = (int*) wParam;

	if( g_GAMEPROCESS() == NULL )
		return FALSE;

	if(g_GAMEPROCESS()->GetSevenRuleManager().m_cRule[0].isValid())
	{
		*pStrJokbo = g_GAMEPROCESS()->GetSevenRuleManager().m_cRule[0].get_LowRuleValue_to_Name();		
	}
	else
		*pStrJokbo = "";

	if (nSevenValue)
		*nSevenValue = g_GAMEPROCESS()->GetSevenRuleManager().m_cRule[0].get_sevenLowValue();

	return TRUE;
}


LRESULT CGameHighLow::OnGetJokboName(WPARAM &wParam, LPARAM &lParam)
{
	CString *pStrJokbo = (CString *)lParam;	

	if(NULL == g_GAMEPROCESS())
		return FALSE;

	int nPlayerNum = (int)wParam;

	if(nPlayerNum < 0 || nPlayerNum >= GetMaxPlayer())
		return FALSE;

	*pStrJokbo = g_GAMEPROCESS()->GetSevenRuleManager().m_cRule[nPlayerNum].get_RuleValue_to_Name3();

// 	pStrJokbo->Format("%s / %s", 
// 						g_GAMEPROCESS()->GetSevenRuleManager().m_cRule[nPlayerNum].get_RuleValue_to_Name3(), 
// 						g_GAMEPROCESS()->GetSevenRuleManager().m_cRule[nPlayerNum].get_LowRuleValue_to_Name());

	return TRUE;
}


LRESULT CGameHighLow::OnGetLowJokboName(WPARAM &wParam, LPARAM &lParam)
{
	CString *pStrJokbo = (CString *)lParam;	

	if(NULL == g_GAMEPROCESS())
		return FALSE;

	int nPlayerNum = (int)wParam;

	if(nPlayerNum < 0 || nPlayerNum >= GetMaxPlayer())
		return FALSE;

	*pStrJokbo = g_GAMEPROCESS()->GetSevenRuleManager().m_cRule[nPlayerNum].get_LowRuleValue_to_Name();


	return TRUE;
}



LRESULT CGameHighLow::OnGetMoneyGrade(WPARAM &wParam, LPARAM &lParam)
{
	return GetMoneyGrade(*((INT64 *)lParam));
}

LRESULT CGameHighLow::OnSetLimitMoney(WPARAM &wParam, LPARAM &lParam)
{
	//보유한도
	g_cOwnLimit.SetMaxPremiumMoneyWithOption(*((INT64 *)lParam));
	return TRUE;
}

LRESULT CGameHighLow::OnSetLimitGold(WPARAM &wParam, LPARAM &lParam)
{
	//보유한도
	g_cOwnLimit.SetMaxPremiumGoldWithOption(*((INT64 *)lParam));
	return TRUE;
}

CString CGameHighLow::GetLobbyEnterSound(bool bMan)
{
	if(bMan) return ".\\highlow\\sound\\enterlobby_m.wav";
	return ".\\highlow\\sound\\enterlobby_f.wav";
}

LRESULT CGameHighLow::OnGetResultOpen( WPARAM &wParam, LPARAM &lParam )
{

	if (g_pGameView==NULL)
		return FALSE;

	return g_pGameView->m_cPlayerDrawManager.IsOpen();
	//return TRUE;
}



void CGameHighLow::ReceiveSV_HL_EndRaise(int nTotSize, char *lpData)
{

	CSV_HL_ENDRAISE msg;
	msg.Get( lpData, nTotSize );	

	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;	

	if(NULL == g_GAMEPROCESS())
		return;	

	g_GAMEPROCESS()->SelectWinnerCase();


}
void CGameHighLow::ReceiveSV_HL_HighLowSelect(int nTotSize, char *lpData)
{
	CSV_HL_HLS_SEL msg;
	msg.Get(lpData, nTotSize);
	
	int spnum = *msg.UserNum;

	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;	

	if(NULL == g_GAMEPROCESS())
		return;	

	int pnum = g_GAMEPROCESS()->GetPNum_ByServPN(spnum);	
	g_GAMEPROCESS()->HLS_SelectedCase(pnum);


}


const HICON CGameHighLow::GetGameIcon(void)
{
	return AfxGetApp()->LoadIcon(IDI_ICON_HIGHLOW);
}

const HICON CGameHighLow::GetChatIcon(void)
{
	return AfxGetApp()->LoadIcon(IDI_ICON_CHAT_HIGHLOW);
}

const CString	CGameHighLow::GetModuleName(void)
{
	char szFileName[MAX_PATH+1]={0,};
	GetModuleFileName(g_hInstGameDLL, szFileName, MAX_PATH);	
	return CString(szFileName);

}

//### [관리자 모드 작업] ###  방정보 변경
LONG CGameHighLow::OnReceiveChangeStyleRoomInfo(int TotSize, char *lpData)
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
void CGameHighLow::ReceiveSV_ASK_WAIT_INROOM(int TotSize, char *lpData)
{	
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	CSV_WAIT_INROOM msg;
	msg.Get(lpData, TotSize);
	g_pGameView->m_cGameViewBtnMananager.SetWaitFlag(*msg.m_bWait); 
}

void CGameHighLow::ReceiveSV_CongNoti(int TotSize, char *lpData)
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

void CGameHighLow::ReceiveSV_PM_ODDEVEN_WINNER_NOTI(int TotSize, char *lpData)
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