#include "StdAfx.h"

#include "GameHoola.h"
#include "GlobalGame.h"
#include "SpriteList.h"
#include "GameViewHoola.h"
#include "GameProcessHoola.h"
#include "CommMsgDef_Game.h"
#include "Resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CGameHoola::CGameHoola(void)
{
	m_bIsGiveUpRoom = false;
}

CGameHoola::~CGameHoola(void)
{
#ifdef TRACE_OUTPUT
	TTrace::Stop();
#endif
}

BOOL CGameHoola::Create(CWnd *pParentWnd)
{
#ifdef TRACE_OUTPUT
	//TTrace::Init();
	//TTrace::ClearAll();
//	TTrace::Options()->sendMode = WinMsg;
//	TTrace::Show(true);
	TTrace::Debug()->Send ("7Poker시작-Create", "CGameHoola");   // single byte string	
#endif

	// 메인 게임 View를 생성한다.
	CGameViewHoola* pGameView = new CGameViewHoola(this);
	g_pGameView		= pGameView;
	m_pGameViewBase	= pGameView;

	CRect rect(0, 0, 1000, 740);
	pGameView->Create(NULL, "GameView", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rect, pParentWnd, 12345);
	pGameView->ShowWindow(SW_HIDE);

	// Create GameProcess
	CGameProcessHoola *pGP = new CGameProcessHoola(this);
	m_pGameProcess = pGP;
	m_pGameProcess->Init(&g_pGameView->Page);

	m_eForm_KindType = hoola::eFORM_KIND_NORMAL;

	return CGame::Create(pParentWnd);
}


CGameProcessHoola* CGameHoola::GetGameProcess()
{
	return (CGameProcessHoola*) m_pGameProcess;
}

// 방생성 다이얼로그
LRESULT CGameHoola::ShowCreateRooomDlg(ROOMINFO *pRoomInfo, ROOMOPTION *pWebRoomOption)
{
	CCreateRoomDlg dlg(GetGameType(), GM().GetMainWnd());
	dlg.SetWebSelectedRoomOption(pWebRoomOption);
	LRESULT nRet = dlg.DoModal();
	
	// 방 만들기
	if (nRet == IDOK)
	{
		strncpy_s(pRoomInfo->Title, 32, dlg.m_strRoomTitle, dlg.m_strRoomTitle.GetLength());
		
		pRoomInfo->FormKind			= dlg.m_nGameMode;
		pRoomInfo->bIsFriendRoom	= dlg.m_bFriend;
		pRoomInfo->bSuperMaster		= PBIsSuperRoom();
		pRoomInfo->BetMoney			= dlg.m_i64SeedMoney;
		pRoomInfo->llEnterLimitMoney= dlg.m_i64MinimumMoney;
		//pRoomInfo->bVIPEnterLimitYN = dlg.m_bVIPEnterLimitYN;
		pRoomInfo->nAutoBetKind		= dlg.m_sAutoBet;
		pRoomInfo->sBetRuleKind		= dlg.m_sBetRuleKind;
		pRoomInfo->cRuleType			= dlg.m_nGameType;
		
		//훌라 2인 플레이를 위한 추가 코드
		pRoomInfo->MaxUserNum		= dlg.m_nMaxUserNum;
		pRoomInfo->cMemberRoom		= 0;

		//연합방
		pRoomInfo->cafeID_				= dlg.m_bUnionMode? GM().GetMyInfo()->UI.unitedCafeInfo_.cafeID_ : 0;
	}
 
	return nRet;
}

// 바로입장 함수 ( 클라 -> 서버 )
int CGameHoola::GetDirectEnterRoomType(int nType)
{
	return CHOICE_GAME;
}

void CGameHoola::CheckDirectEnterRoomInfo(int nType, ROOMINFO *pRoomInfo)
{
	CString strTitle;
	MM().Call(UPG_CMK_GENERATE_RANDOM_ROOM_NAME, 0, (LPARAM)(&strTitle));
	strncpy_s(pRoomInfo->Title, 32, strTitle, strTitle.GetLength());
	pRoomInfo->bSuperMaster = PBIsSuperRoom();
}

// 빠른입장 다이얼로그
LRESULT	CGameHoola::ShowQuickJoinerDlg(void)
{
	CQuickJoinDlg dlg(EQJ_HOOLA, &g_cUIData, GM().GetMainWnd());
	LRESULT nRet = dlg.DoModal();

	if (nRet == IDOK)
	{		
		GM().SendQuickJoiner();
	}

	return FALSE;
}

void CGameHoola::SetFormKindType(hoola::EFORM_KIND_TYPE form_KindType)
{
	m_eForm_KindType = form_KindType;
}

LRESULT	CGameHoola::SendQuickJoiner(void)
{
	if ( OnMacroCheck() )
	{
		return FALSE;
	}
	
	NMBASE::SOUND::g_Sound.PlayWav(SND45);

	bool bQJEnable = CQuickJoinDlg::IsEnableQJ(EQJ_HOOLA);
	int nGameMode = (int)m_eForm_KindType;

	//규제안 : 훌라도 바로입장 설정 값을 가져온다,
	/*
	if (!bQJEnable)
	{
		GM().AskDirectEnterRoom(false, NULL, nGameMode);
		return TRUE;
	}
	*/
	
	// 규제 개선안 작업
	// 바로 시작을 로비에서 호출하면, 무조건 랜덤으로 입장.
	GM().AskDirectEnterRoom(false, NULL, nGameMode);

	// 설정 값 사용하는 부분은 일단 사용안함.
//	SCONDITION sInfo; 
//	CString strSection;
//	strSection.Format("QJ%02d_%s_%d", QJ_VER, GM().GetMyInfo()->UI.ID, EQJ_HOOLA);
//
//	CQuickJoinDlg::GetSearchCondition(EQJ_HOOLA, strSection, &sInfo);
//// 	if (CQuickJoinDlg::GetSearchCondition(EQJ_HOOLA, strSection, &sInfo))
//// 	{
//		CDataRoomOptionList *pDataRoomOptionList = GM().GetDataRoomOptionList();
//		if (pDataRoomOptionList == NULL) return FALSE;
//
//		//ROOMOPTION *pResultRoomOption = pDataRoomOptionList->GetQJRoomOption(false, false, sInfo.m_bUseSeedMoney, sInfo.m_nSeedType, sInfo.m_sBetRuleKind );
//		ROOMOPTION *pResultRoomOption = pDataRoomOptionList->GetQJRoomOption(GM().m_scondition.m_bUseQuickOption, GM().m_scondition.m_nGameMode, GM().m_scondition.m_bUseSeedMoney, GM().m_scondition.m_nSeedType); 
//
//		if (pResultRoomOption == NULL)
//		{
//			if (sInfo.m_bAutoEnterRoom)
//			{
//				GM().AskDirectEnterRoom(false, NULL, nGameMode);
//				return TRUE;
//			}
//			else
//			{
//				CRefuseQJDlg dlg(GM().GetMainWnd());
//				int idResult = dlg.DoModal();
//				if ( IDOK == idResult )
//				{
//					//다시 설정
//					ShowQuickJoinerDlg();
//				}
//				else if ( IDCANCEL == idResult )
//				{
//					//방만들기
//					GM().ShowCreateRooomDlg();
//				}
//
//				return TRUE;
//			}
//		}
//		
//		if ( true == sInfo.m_bUseBetRule )
//		{
//			if ( false == sInfo.m_bAutoBet )
//			{
//				pResultRoomOption->cRuleType = hoola::eRULE_TYPE_STOP;
//			}
//			else
//			{
//				pResultRoomOption->cRuleType = hoola::eRULE_TYPE_WAR;
//			}
//		}
//		else
//		{
//			pResultRoomOption->cRuleType = -1;
//		}
//
//		//방정원 설정 상황에서
//		//2인 5인에 해당하는 데이터를 받아온다
//		if (sInfo.m_nGameMode == 2)
//		{
//			pResultRoomOption->cMaxUserNum = 2;
//		}
//		else
//		{
//			pResultRoomOption->cMaxUserNum = 5;
//		}
//
//		/*
//		//규제안 작업중 방정원을 설정하였는데 상관없는 게임모드 값에 의해 방정원이 작동하지 않는다.
//		if (true == sInfo.m_bUseGameMode)
//		{
//			//방정원 설정 상황에서
//			//2인 5인에 해당하는 데이터를 받아온다
//			if (sInfo.m_nGameMode == 2)
//			{
//				pResultRoomOption->cMaxUserNum = 2;
//			}
//			else
//			{
//				pResultRoomOption->cMaxUserNum = 5;
//			}
//		}
//		else
//		{
//			pResultRoomOption->cMaxUserNum = -1;
//		}
//		*/
//
//		//규제안 : 방옵션에 클릭된 게임모드 설정
//		pResultRoomOption->cFormKind = nGameMode;
//		GM().AskDirectEnterRoom(GM().m_scondition.m_bUseQuickOption, pResultRoomOption, nGameMode, ECALL_BTNTYPE_LOBY_QUICK, true, sInfo.m_bAutoEnterRoom);
////		GM().AskDirectEnterRoom(bQJEnable, pResultRoomOption, nGameMode, ECALL_BTNTYPE_LOBY_QUICK, true, sInfo.m_bAutoEnterRoom);
//// 	}
//// 	else
//// 	{
//// 		// 일반적인 방 입장
//// 		GM().AskDirectEnterRoom(false, NULL, -1);
//// 	}

	return TRUE;
}

CPlayer *CGameHoola::CreateNewPlayer(void)
{
	return new CPlayerHoola;
}
void CGameHoola::AddGameChat(const char *str, COLORREF crNewColor, bool bUserChat)
{	
	g_pChatWinMan->AddChat(str, crNewColor, bUserChat);
}
void CGameHoola::AddGameInfo(const char *str, COLORREF crNewColor, bool bUserChat)
{	
	g_pChatWinMan->AddInfo(str, crNewColor, bUserChat);
}
void CGameHoola::OnChangeFullScreen(bool bFullScreen)
{
	// ### [ 관전기능 ] ###
	g_pGameView->m_cGameViewBtnMananager.SetChangeDisplayButton(bFullScreen);
	g_pGameView->Page.ReCreateDIBSection();
}
void CGameHoola::OnCloseOptionDlg(LRESULT nRet)
{
	if (nRet == IDOK) g_pGameView->m_cGameViewBtnMananager.SetBetBtnType(g_Config.eBetBtnType);
}
void CGameHoola::OnLoadingEnd(void)
{
	// 스프라이트 로딩 후 초기화 ~
	GetGameProcess()->OnInit();
	GetGameView()->OnInit();
}
CGameViewHoola* CGameHoola::GetGameView()
{
	return (CGameViewHoola*) m_pGameViewBase;
}
void CGameHoola::LoadSprite()
{	
	CGame::LoadSprite();
	LoadSpriteFile();
}
void CGameHoola::LoadSound()
{
	CGame::LoadSound();
	RegistSoundFiles();
}
void CGameHoola::ReleaseSprite()
{
	CGame::ReleaseSprite();
}
void CGameHoola::ReleaseSound()
{
	CGame::ReleaseSound();
}
const IDX_GAME CGameHoola::GetGameType(void)
{
	return IDX_GAME_HA;
}
const int CGameHoola::GetMaxRoom(void)
{
	return 90;
}
const int CGameHoola::GetMaxPlayer(void)
{
	return MAX_PLAYER;
}
const int CGameHoola::GetMaxGamePlayer(void)
{
	return MAX_PLAYER;
}
const int CGameHoola::GetMaxObserver(void)
{
	return MAX_OBSERVER;
}

const int CGameHoola::GetJokboQuestCount(void)
{
	return 6;
}
const CString CGameHoola::GetGameString(void)
{
	return "hoola";
}
const CString CGameHoola::GetGameName(void)
{
	return "훌라";
}
const COLORREF	CGameHoola::GetGameColor(void)
{	
	return RGB(66,79,113);//각 게임 고유의 색을 반환
}
const int CGameHoola::GetGameCode(void)
{
	return GAME_CODE_HA;
}
const CString CGameHoola::GetLoadingBackPath(void)
{
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		return ".\\Hoola\\image\\LoadingBack_toctoc.bmp";
	else
		return ".\\Hoola\\image\\LoadingBack.bmp";
#else //UPGRADE_20120308_TOCTOC_CHANNELING
	return ".\\Hoola\\image\\LoadingBack.bmp";
#endif //UPGRADE_20120308_TOCTOC_CHANNELING

}
const int CGameHoola::GetDefaultFormKind(void)
{
	return m_eForm_KindType;
}


// 네트웍 메시지를 받아 처리하는 함수(CClientDlg::OnPacketNotify 에서 게임에 관련된 패킷만 여기로 이동)
LONG CGameHoola::OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData)
{	
	if (CGame::OnPacketNotify(pName, Signal, TotSize, lpData))
		return TRUE;

	if (g_cOneShotCharge.OnPacketNotify(pName,Signal, TotSize, lpData))
		return TRUE;
	
// 	if (g_pGameView->m_cGameHelper.OnPacketNotify(pName,Signal, TotSize, lpData)) 
// 		return TRUE;

	if (g_cOwnLimit.OnPacketNotify(pName,Signal, TotSize, lpData)) 
		return TRUE;

	if (g_cBanishMan.OnPacketNotify(pName, Signal, TotSize, lpData))
		return TRUE;

	if ( g_cPromotionMan.OnPacketNotify(pName, Signal, TotSize, lpData) ) return TRUE;

	if ( g_cVipJackpot.OnPacketNotify(pName, Signal, TotSize, lpData) ) return TRUE;
	

#ifdef _DEBUG

#define TRACE_PACKET_CASE(sig) case sig: \
	{ COleDateTime time; time=COleDateTime::GetCurrentTime(); CString strTime=time.Format("%H:%M:%S");  \
	TRACE( "[%s] Packet Received : " #sig "\n", strTime ); }
#else
#define TRACE_PACKET_CASE(sig) case sig:
#endif
	switch(Signal)
	{
		// 게임시작버튼 활성화
	TRACE_PACKET_CASE(SV_STARTBTNENABLE)
		ReceiveSV_STARTBTNENABLE(TotSize, lpData);
		break;
		// 관전기능
	TRACE_PACKET_CASE(SV_OBSERVER_MESSAGE)
		ReceiveSV_OBSERVER_MESSAGE(TotSize, lpData);
		break;
		// 관전기능(예약자 정보)
	TRACE_PACKET_CASE(SV_RESERVATIONINFO)
		ReceiveSV_RESERVATIONINFO(TotSize, lpData);
		break;
		// 관전기능(게임참여 -> 관전)
	TRACE_PACKET_CASE(SV_ACCEPT_EMERAGE)
		ReceiveSV_ACCEPT_EMERAGE(TotSize, lpData);
		break;
		// 관전기능(관전 -> 게임참여)
	TRACE_PACKET_CASE(SV_ACCEPT_PARTICIPATION)
		ReceiveSV_ACCEPT_PARTICIPATION(TotSize, lpData);
		break;
		// 관전기능(게임참여 -> 관전)
	TRACE_PACKET_CASE(SV_ENTER_EMERAGE)
		ReceiveSV_ENTER_EMERAGE(TotSize, lpData);
		break;
		// 관전기능(관전 -> 겜참여)
	TRACE_PACKET_CASE(SV_ENTER_PARTICIPATION)
		ReceiveSV_ENTER_PARTICIPATION(TotSize, lpData);
		break;
		// 게임이 종료되었음
	TRACE_PACKET_CASE(SV_GAMEOVERRESULT) 
		ReceiveSV_GAMEOVERRESULT(TotSize, lpData); 
		break;
		/////////////////////////// 클라이언트용 메세지  ////////////////////////

		// 게임방 채팅데이타
	TRACE_PACKET_CASE(CL_CHATDATA)
		ReceiveCL_CHATDATA(TotSize, lpData); 
		break;
	case SV_CHANGESTYLEROOMINFO:		  
		{	
			return OnReceiveChangeStyleRoomInfo( TotSize, lpData ); 
		}
		break; //BTS 모니터링 툴 방제 변경 패킷. 
		// 관전기능
	TRACE_PACKET_CASE(CL_EXITSUBSCRIPT)
		ReceiveCL_EXITSUBSCRIPT(TotSize, lpData);
		break;
		/////////////////////////// 게임 진행용 메세지  ////////////////////////

		// 게임 시작
	TRACE_PACKET_CASE(SV_STARTCARD)
		ReceiveSV_STARTCARD(TotSize, lpData);
		break;

		// 게임 종료
	TRACE_PACKET_CASE(SV_ENDGAME)
		ReceiveSV_ENDGAME(TotSize, lpData);
		break;
		// 잭팟
	TRACE_PACKET_CASE(SV_SPECIALCARD )
		ReceiveSV_SPECIALCARD(TotSize, lpData);
		break;
		// 자리 지킴이 패킷 처리
	TRACE_PACKET_CASE(SV_SEATKEEPER)
		ReceiveSV_SEATKEEPER(TotSize, lpData);
		break;
		// 돈이 변경 됨
	TRACE_PACKET_CASE(SV_CHANGE_MONEY)
		ReceiveSV_CHANGE_MONEY(TotSize, lpData);
		break;
		// 퇴실 요청 허가됨
	TRACE_PACKET_CASE(SV_ACCEPT_OUTROOM) 
		ReceiveSV_ACCEPT_OUTROOM(TotSize, lpData);
		break;
		// 내 정보가 변경됨
	TRACE_PACKET_CASE(SV_CHANGEMYINFO) 
		ReceiveSV_CHANGEMYINFO(TotSize, lpData);
		break;
		// 새로운 플레이어가 입실하였음
	TRACE_PACKET_CASE(SV_USERENTERROOM)  
		ReceiveSV_USERENTERROOM(TotSize, lpData);
		break;
		// 사용자가 방에서 퇴장했음
	TRACE_PACKET_CASE(SV_USEROUTROOM) 
		ReceiveSV_USEROUTROOM(TotSize, lpData); 
		break;
		// 방장이 바뀌어야함
	TRACE_PACKET_CASE(SV_CHANGEROOMMASTER) 
		ReceiveSV_CHANGEROOMMASTER(TotSize, lpData);
		break;
	TRACE_PACKET_CASE(SV_WHISPERCHAT)
		ReceiveSV_WhisperChat(TotSize, lpData);
		break;
	TRACE_PACKET_CASE(NM_ADMIN_WHISPERCHAT)	
		ReceiveSV_AdminWhisperChat(TotSize, lpData);
		break;
	TRACE_PACKET_CASE(OPCODE_SV_JP_INFO)
		ReceiveSV_JACKPOTMONEYINFO(TotSize, lpData);
		break;
	TRACE_PACKET_CASE(SV_DAILY_LOSS_MONEY)
		ReceiveSV_LOSSMONEYINFO(TotSize, lpData);
		break;
		//이하 훌라용 코드
	TRACE_PACKET_CASE(SV_DISCARD)
		ReceiveSV_DISCARD(TotSize, lpData);
		break;
	TRACE_PACKET_CASE(SV_REGRESULT)
		ReceiveSV_REGRESULT(TotSize, lpData);
		break;
	TRACE_PACKET_CASE(SV_STOPGAMERESULT)
		ReceiveSV_STOPGAMERESULT(TotSize, lpData);
		break;
	// 땡큐를 해서 성공을 하면 오는 패킷
	TRACE_PACKET_CASE(SV_THANKRESULT)
		ReceiveSV_THANKRESULT(TotSize, lpData);
		break;
	TRACE_PACKET_CASE(SV_SPECIALRULERESULT)
		ReceiveSV_SPECIALRULERESULT(TotSize, lpData);
		break;
	// 턴이 넘어간 시점에서 땡큐가 발생하면 오는 패킷
	TRACE_PACKET_CASE(SV_TURNTHANK)
		ReceiveSV_TURNTHANK(TotSize, lpData);
		break;
	// 턴이 넘어갈때마다 오는 패킷(땡큐 일때는 안옴)
	TRACE_PACKET_CASE(SV_TURNGET)
		ReceiveSV_TURNGET(TotSize, lpData);
		break;
	// 받기, 등록/붙이기 할 때 오는 패킷(버리기때는 안옴)
	TRACE_PACKET_CASE(SV_TURNREG)
		ReceiveSV_TURNREG(TotSize, lpData);
		break;
	TRACE_PACKET_CASE(SV_CANTHANK)
		ReceiveSV_CANTHANK(TotSize, lpData);
		break;
	TRACE_PACKET_CASE(SV_ADDEDCARDDATA)
		ReceiveSV_ADDEDCARDDATA(TotSize, lpData);
		break;
	TRACE_PACKET_CASE(SV_FIXREGIST)
		ReceiveSV_FIXREGIST(TotSize, lpData);
		break;
	TRACE_PACKET_CASE(SV_MISSION)
		ReceiveSV_MISSION(TotSize, lpData);
		break;
	TRACE_PACKET_CASE(SV_MISSIONCOMPLETE)
		ReceiveSV_MISSIONCOMPLETE(TotSize, lpData);
		break;
	TRACE_PACKET_CASE(SV_MAGICCARD)
		ReceiveSV_MAGICCARD(TotSize, lpData);
		break;
	TRACE_PACKET_CASE(CL_BRINGINCARD)
		ReceiveCL_BRINGINCARD(TotSize, lpData);
		break;
	TRACE_PACKET_CASE(CL_SUN_PICKUP_START)
		ReceiveCL_SUN_PICKUP_START(TotSize, lpData);
		break;
	TRACE_PACKET_CASE(CL_SUN_PICKUP)
		ReceiveCL_SUN_PICKUP(TotSize, lpData);
		break;
	TRACE_PACKET_CASE(CL_SUN_PICKUP_COMPLETE)
		ReceiveCL_SUN_PICKUP_COMPLETE(TotSize, lpData);
		break;
	TRACE_PACKET_CASE(SV_JACKPOTCOUNT)
		ReceiveSV_JACKPOTCOUNT(TotSize, lpData);
		break;
	TRACE_PACKET_CASE(OPCODE_SV_JP_WINNER_ALRAM)
		ReceiveOPCODE_SV_JP_WINNER_ALRAM(TotSize, lpData);
		break;
	TRACE_PACKET_CASE(CL_GOLDSTAGE_INFO)
		ReceiveCL_GOLDSTAGE_INFO(TotSize, lpData);
		break;
	case SV_ACCEPT_CHANGESNDFX:
		TRACE("SV_ACCEPT_CHANGESNDFX\n");
		break;
	case SV_ASK_CHANGESNDFX:
		TRACE("SV_ASK_CHANGESNDFX\n");
		ReceiveSV_ASK_CHANGESNDFX(TotSize, lpData);
		break;
	case SV_RECEIVE_CONG_MSG:
		ReceiveSV_CongNoti(TotSize, lpData);
		break;
	//case SV_PM_ODDEVEN_WINNER_NOTI:
	case SV_PM_WHEEL_WINNER_NOTI:
		ReceiveSV_PM_ODDEVEN_WINNER_NOTI(TotSize, lpData);
		break;
	case SV_GIVEUP_RULE_NOTI:
		ReceiveSV_GIVEUP_RULE_NOTI(TotSize, lpData);
		break;
	case SV_GIVEUP_LOSE_MONEY_NOTI:
		ReceiveSV_GIVEUP_LOSE_MONEY_NOTI(TotSize, lpData);
		break;
	default:
		return CGame::OnPacketNotify(pName, Signal, TotSize, lpData );
		break;
	}

	return TRUE;
}


//규제안 : 손실금액에 따른 경고창 오픈
void CGameHoola::ReceiveSV_LOSSMONEYINFO(int TotSize, char *lpData)
{
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	CSV_DAILY_LOSS_MONEY msg;
	msg.Get(lpData,TotSize);

	//각 게임에서 참조하도록 GM에 값을 설정
	GM().SetLossmoneyInfo(*msg.llLossMoney, *msg.tBlockTime, *msg.eState, *msg.idxGame);

	//규제안 : 게임내 경고 문구 노출
	g_pGameView->m_cGameViewBtnMananager.SetLossMoneyWarning();
}

void CGameHoola::ReceiveCL_GOLDSTAGE_INFO(int nTotSize, char *lpData)
{
	packet::TSendPacket< GOLD_STAGE_INFO  > MISSION_Packet;
	MISSION_Packet.Get( lpData, nTotSize );
	GOLD_STAGE_INFO * pGInfo = static_cast< GOLD_STAGE_INFO * >( MISSION_Packet.Various() );


// bGoldHoola
// nGoldStage

	if (false == pGInfo->bGoldHoola)
	{
		g_GAMEPROCESS()->SetGoldStage(pGInfo->bGoldHoola, pGInfo->nGoldStage);
		g_GAMEPROCESS()->GetPlayScr()->m_nGoldStage0 = 0;
	}
	
// 
// 	if (false == pMInfo->bMission)
// 	{
// // 		g_GAMEPROCESS()->GetPlayScr()->m_bGoldState = FALSE;
// // 		g_GAMEPROCESS()->GetPlayScr()->m_nGoldStage = 1;
// 		g_GAMEPROCESS()->SetGoldStage(FALSE, 1);
// 	}
	

}

void CGameHoola::ReceiveSV_ASK_CHANGESNDFX(int nTotSize, char *lpData)
{
	CSV_ASK_CHANGESNDFX msg;
	msg.Get(lpData, nTotSize);

	for ( int i = 0; i < MAX_PLAYER; i++ )
	{
		if ( strcmp(g_PLAYER(i)->UI.ID, msg.ID) == 0 )
		{
			g_PLAYER(i)->nSndFxKind = *msg.SndFX;
		}
	}

}

void CGameHoola::ReceiveSV_CongNoti(int nTotSize, char *lpData)
{
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	CSV_RECEIVE_CONG_MSG msg;
	msg.Get(lpData, nTotSize);

	CString strID;
	strID.Format("보낸 사람 : %s님", msg.m_Send_ID);
	CString strChat;
	strChat.Format("%s", msg.StrChat);
	g_pGameView->m_cPlayerDrawManager.SetPlayCongNoti(strID.GetString(), strChat.GetString(), *msg.m_ShowTime);
}

void CGameHoola::ReceiveSV_PM_ODDEVEN_WINNER_NOTI(int TotSize, char *lpData)
{
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	//CSV_PM_ODDEVEN_WINNER_NOTI msg;
	CSV_PM_WHEEL_WINNER_NOTI msg;
	msg.Get(lpData, TotSize);

	CString strID;
	strID.Format("%s", msg.m_UserID);
	g_pGameView->m_cPlayerDrawManager.SetPlayGoldNoti(strID.GetString(), *msg.m_RewardMoney, timeGetTime());
}

void CGameHoola::ReceiveOPCODE_SV_JP_WINNER_ALRAM(int nTotSize, char *lpData)
{
	if ( GetCurrentWhere() != IDX_GAMEWHERE_GAME ) return;
 
	packet::TSendPacket< jackpot::JP_WIN > JP_WIN_Packet;
	JP_WIN_Packet.Get( lpData, nTotSize );
	jackpot::JP_WIN* pAlaram = static_cast< jackpot::JP_WIN* >( JP_WIN_Packet.Various() );

	if ( g_RI.RoomNum == pAlaram->JData.sRoomNum ) 
	{
		g_GAMEPROCESS()->SetJacpotAnimationState(pAlaram);
		g_GAMEPROCESS()->SetStopJackpotMsgTitle(pAlaram);
		
	}
	else if (pAlaram->JData.llLimitMoney >= 3000000000000)
	{
		g_GAMEPROCESS()->SetStopJackpotMsgTitle(pAlaram);
	}


	//같은 그룹의 잭팟 당첨자인가 확인한다.
// 	if ( m_JackPokWinnerEft == 1 && g_RI.llEnterLimitMoney == pAlaram->JData.llLimitMoney && GM().GetCurrentGameCode() == pAlaram->JData.sGameCode )
// 	{
// 		m_AniIndex_DP = m_AniIndex = 8;
// 		m_JackPokWinnerEft = 2;
// 
// 		m_bReleaseInfo = TRUE;
// 		m_dwRelaseInfoTime = timeGetTime();
// 	}
}
void CGameHoola::ReceiveOPCODE_SV_JP_EVENTTIME_ROOM(int nTotSize, char *lpData)
{
// 	packet::TSendPacket< jackpot::JP_EVENTTIME_ROOM > JP_EventTime_Packet;
// 	JP_EventTime_Packet.Get( lpData, nTotSize );
// 	jackpot::JP_EVENTTIME_ROOM* pAlaram = static_cast< jackpot::JP_EVENTTIME_ROOM* >( JP_EventTime_Packet.Various() );
// 
// 	//잭팟타임을 켜거나 끈다.
// 	g_RI.bEventJackpotTime = pAlaram->bJackPotEventTime;
// 
// 	m_Jackpot_Info.bCurrentRound = false;
// 	m_Jackpot_Info.bJackPotEventGiftTime = false;
// 	m_Jackpot_Info.nRoundCount = 0;
}
void CGameHoola::ReceiveSV_JACKPOTMONEYINFO(int TotSize, char *lpData)
{
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;
	
	packet::TSendPacket< jackpot::JP_MONEYINFO > JP_MONEY_Packet;
	JP_MONEY_Packet.Get( lpData, TotSize );
	jackpot::JP_MONEYINFO* pAlaram = static_cast< jackpot::JP_MONEYINFO* >( JP_MONEY_Packet.Various() );

	g_GAMEPROCESS()->JackPotMoneyUpdate(pAlaram);
}


void CGameHoola::ReceiveSV_STARTBTNENABLE(int nTotSize, char *lpData)
{
	CSV_STARTBTNENABLE msg;

	msg.Get(lpData, nTotSize);

	// 디버깅 편리를 위해 아이디와 내 아이디를 지역변수에 저장
	char* cID = msg.ID;
	char* cMyID = GM().GetMyInfo()->UI.ID;
	
	// 내가 방장일 경우
	if (strcmp( cID, cMyID ) == 0)
	{
		// 기권방 일 경우
		//if (m_bIsGiveUpRoom)
		//{
		//}
		
		if (TRUE == g_GAMEPROCESS()->GetResultDelay())
		{
			g_GAMEPROCESS()->SetStartBtnDelay(*msg.Kind);
		}
		else
		{
			if (*msg.Kind == 1)
			{
				// 게임 시작 버튼을 (n / 1000)초뒤에 나타나도록 함
				Wait(2000);
				
				// 방에 나밖에 없을 경우 게임시작 버튼을 활성화 하지 않음
				if (g_RI.NowUserNum > 1)
				{
					// 게임 시작 버튼 활성화
					g_GAMEPROCESS()->GetPlayScr()->EnableGameStartBtn();
					g_GAMEPROCESS()->GetPlayScr()->DisableGamePracticsBtn();
				}
			}
			else if (*msg.Kind == 2)
			{
				// 연습 하기 버튼 활성화
				g_GAMEPROCESS()->GetPlayScr()->EnableGamePracticsBtn();
				g_GAMEPROCESS()->GetPlayScr()->DisableGameStartBtn();
			}
			else if (*msg.Kind == 0)
			{
				// 게임 시작, 연습 하기 버튼 비활성화
				g_GAMEPROCESS()->GetPlayScr()->DisableGameStartBtn();
				g_GAMEPROCESS()->GetPlayScr()->DisableGamePracticsBtn();
			}
		}
	}
	// 내가 방장이 아닐 경우
	else
	{
		g_GAMEPROCESS()->GetPlayScr()->DisableGameStartBtn();
		g_GAMEPROCESS()->GetPlayScr()->DisableGamePracticsBtn();
	}
	
	//msg.Get(lpData, nTotSize);
	//
	//if (g_RI.State != EGAME_WAITING)
	//	return;
	//
	//if (g_RI.NowUserNum < LIMITGAMEPLAY_PLAYERNUM )
	//	return;
}

void CGameHoola::ReceiveSV_OBSERVER_MESSAGE(int nTotSize, char *lpData)
{
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
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
	if (num >= 1 && num < 100)
	{
		//g_VoiceMan.ObserverJoinRankVoice(num-1);
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
	else if(num == 500)
	{
		g_ObserverMan.OnObserve();
		ShowMsgModelessDlg(NULL, str.GetString());
	}
}

void CGameHoola::ReceiveSV_RESERVATIONINFO(int nTotSize, char *lpData)
{
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	CSV_RESERVATIONINFO msg;
	msg.Get(lpData, nTotSize);

	CObserverDlg *pObserverDlg = GetObserverDlg();
	if (pObserverDlg)
	{
		pObserverDlg->User_ResetReservation();
	}

	for (int i = 0; i < *msg.TotNum; ++i) 
	{
		char *id = msg.ReservedInfo[i].ID;
		char *NickName = msg.ReservedInfo[i].ID;
		int unum = msg.ReservedInfo[i].UNum;
		int seq =  msg.ReservedInfo[i].Seq;
		if (pObserverDlg)
		{
			pObserverDlg->User_SetItemReservation(unum, id, seq);
		}
	}

	//관전UI
	g_ObserverMan.OnReceiveReservationInfo(&msg);
}

// 관전기능(게임참여 -> 관전)
void CGameHoola::ReceiveSV_ACCEPT_EMERAGE(int nTotSize, char *lpData)
{
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	CSV_ACCEPT_EMERAGE msg;
	msg.Get(lpData, nTotSize);

	int nMyID = -1;
	int nowpnum = *msg.NowPNum;
	int newpnum = *msg.NewPNum;
	
	for (int i = 0; i < GetMaxNewPlayer(); ++i)
	{
		if (strlen(g_PLAYER(i)->UI.ID) > 0)
		{	
			if (strcmp(GM().GetMyInfo()->UI.ID, g_PLAYER(i)->UI.ID) == 0)
			{
				nMyID = i;
				break; 
			}
		}
	}

	BOOL bTrue = TRUE;

	if (newpnum < MAX_PLAYER || newpnum >= GetMaxNewPlayer())
	{
		bTrue = FALSE;
	}
	
	if (nMyID == -1 || nowpnum != g_PLAYER(0)->ServPNum || !bTrue) 
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
	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		int pn = g_GAMEPROCESS()->GetPNum_ByServPN(i);

		if (strlen(g_PLAYER(pn)->UI.ID) > 0)	
		{
			g_PLAYER(pn)->MyIpCheck = msg.IPA->IpIndex[i];
		}
	}

// 	g_GAMEPROCESS()->GetPlayScr()->DisableGamePracticsBtn();
// 	g_GAMEPROCESS()->GetPlayScr()->DisableGameStartBtn();
}

// 관전기능(관전 -> 게임참여)
void CGameHoola::ReceiveSV_ACCEPT_PARTICIPATION(int nTotSize, char *lpData)
{
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;
	
	CSV_ACCEPT_PARTICIPATION msg;
	msg.Get(lpData, nTotSize);
	
	int  nMyID   = -1;
	BOOL bTrue   = TRUE;
	int  nowpnum = *msg.NowPNum;
	int  newpnum = *msg.NewPNum;
	
	for (int i = 0; i < GetMaxNewPlayer(); ++i)
	{
		if (strlen(g_PLAYER(i)->UI.ID) > 0)
		{	
			if (strcmp(GM().GetMyInfo()->UI.ID, g_PLAYER(i)->UI.ID)==0) 
			{ 
				nMyID = i; 
				break;
			}
		}
	}
	
	if (newpnum < 0 || newpnum >= MAX_PLAYER)
	{
		bTrue = FALSE;
	}
	
	if (nMyID == -1 || nowpnum != nMyID || !bTrue) 
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
	for (int i = 0; i < MAX_PLAYER; ++i) 
	{
		int pn = g_GAMEPROCESS()->GetPNum_ByServPN(i);

		if (strlen(g_PLAYER(pn)->UI.ID) > 0) 
		{
			g_PLAYER(pn)->MyIpCheck = msg.IPA->IpIndex[i];
		}
	}

	g_pGameView->GetGameButtonMan()->ResetGameButton();
	
	// 자리 지킴이
	g_SeatKeeper.OnEnterGame(); // 관전 -> 겜참여 일때 호출 
	
	// 보유한도
	g_cOwnLimit.OnEnterRoom(&g_cUIData, g_GAMEPROCESS());
}

void CGameHoola::ReceiveSV_ENTER_EMERAGE(int nTotSize, char *lpData)
{
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;
	
	CSV_ENTER_EMERAGE msg;
	msg.Get(lpData, nTotSize);
	
	int nowpnum = *msg.NowPNum;
	int newpnum = *msg.NewPNum;
	
	if (nowpnum < 0 || nowpnum >= MAX_PLAYER) 
		return;
	
	if (newpnum < MAX_PLAYER || newpnum >= GetMaxNewPlayer())
		return;
	
	int pn = g_GAMEPROCESS()->GetPNum_ByServPN(nowpnum);

	if (strcmp( g_PLAYER(pn)->UI.ID, msg.ID) != 0) 
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
	for (int i = 0; i < MAX_PLAYER; ++i)	
	{
		int nPlayerNumber = g_GAMEPROCESS()->GetPNum_ByServPN(i);
		if (strlen(g_PLAYER(nPlayerNumber)->UI.ID) > 0)	
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

	if (pObserverDlg)
	{
		pObserverDlg->User_InsItem(ounum,g_PLAYER(newpnum)->UI.ID, g_PLAYER(newpnum)->UI.NickName, sex, level, Money, seq);
	}
	
	int un = g_RI.NowUserNum;
	
	//관전 상태가 되었을때 버튼 생성 처리
// 	if ( g_RI.NowUserNum == 2 && g_pGameView->m_cGameViewBtnMananager.IsShowBtn(CGameButtonHA::BTN_GAMESTART))
// 	{
// 		g_pGameView->m_cGameViewBtnMananager.SetShowBtn(CGameButtonHA::BTN_GAMESTART, FALSE);
// 		if ( g_GAMEPROCESS()->GetPlayScr()->m_bPracticsGame == FALSE )
// 		{
// 			g_pGameView->m_cGameViewBtnMananager.SetShowBtn(CGameButtonHA::BTN_PRACTICS, TRUE);
// 		}
// 	}
}


void CGameHoola::ReceiveSV_ENTER_PARTICIPATION(int nTotSize, char *lpData)
{
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;
	
	CSV_ENTER_PARTICIPATION msg;
	msg.Get(lpData, nTotSize);
	
	int nowpnum = *msg.NowPNum;
	int newpnum = *msg.NewPNum;

	if (newpnum < 0 || newpnum >= MAX_PLAYER) 
		return;
	
	if (nowpnum < MAX_PLAYER || nowpnum >= GetMaxNewPlayer()) 
		return;
	
	// 관전의 pnum 인덱스가 서버와 일치한지 체크한다.
	if (strcmp( g_PLAYER(nowpnum)->UI.ID, msg.ID) != 0)
		return; 
	
	int pnum = -1; // 플레이어 번호

	if ( !g_ObserverMan.IsMyObserver()) // 1) 내가 겜참여중이고 겜참여 넘 입장
	{
		for (int i = g_GAMEPROCESS()->m_nNo1PosPNum; i < MAX_PLAYER + g_GAMEPROCESS()->m_nNo1PosPNum; ++i) 
		{	
			int index = i;
			
			if (index>=MAX_PLAYER)
			{
				index = index - MAX_PLAYER;
			}
			
			//나
			if (index == 0) 
			{
				continue; 
			}
			
			if (strlen(g_PLAYER(index)->UI.ID)==0)
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
		
		g_pGameView->m_cPlayerDrawManager.Reset(pnum);
		g_PLAYER(nowpnum)->UI.cObserver = 0;
		g_PLAYER(pnum)->SetNewPlayer(&g_PLAYER(nowpnum)->UI);
		g_PLAYER(pnum)->SetChangeRoomInfo(*g_PLAYER(nowpnum)->GetChangeRoomInfo());
		g_PLAYER(nowpnum)->Clear();
		g_pGameView->m_cPlayerDrawManager.SetShow(pnum, true);// 이 값을 세팅해야 유저가 화면에 보인다.
		
		g_RI.NowUserNum += 1;
		g_RI.NowObserverNum -= 1;
		g_PLAYER(pnum)->ServPNum = g_GAMEPROCESS()->GetServPNum_ByPN(pnum);
		
#ifdef _DEBUG
		if (newpnum != g_PLAYER(pnum)->ServPNum)
		{
			CString str;
			str.Format("newpnum  =%d,  pnum = %d, ServPNum = %d", newpnum, pnum, g_PLAYER(pnum)->ServPNum);
			AfxMessageBox(str);
		}
#endif
		
		CString str;
		str.Format("▶관전 → 참여: [%s]님\n", g_PLAYER(pnum)->UI.NickName);
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
	}
	else // 3)내가 관전중이고  겜참여 하는넘 입장
	{
		for (int i = 0; i < MAX_PLAYER; ++i)	
		{
			if (strlen(g_PLAYER(i)->UI.ID)==0)
			{
				pnum = i;
				
				break;
			}
		}

		// 비어있는 곳을 찾지 못했으면
		if (pnum == -1)
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
		g_pGameView->m_cPlayerDrawManager.SetShow(pnum, true);// 이 값을 세팅해야 유저가 화면에 보인다.
		g_RI.NowUserNum += 1;
		g_RI.NowObserverNum -= 1;

		g_PLAYER(pnum)->ServPNum = g_GAMEPROCESS()->GetServPNum_ByPN(pnum);// 서버측 플레이어 번호 저장
		
		CString str;
		str.Format("▶관전 → 참여: [%s]님\n", g_PLAYER(pnum)->UI.NickName);
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
	}

	// 중복된 IP를 체크한다.
	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		int pn = g_GAMEPROCESS()->GetPNum_ByServPN(i);
		if (strlen(g_PLAYER(pn)->UI.ID) > 0)
		{
			g_PLAYER(pn)->MyIpCheck = msg.IPA->IpIndex[i];
		}
	}

	// 사운드를 추가한다.
	g_PLAYER(pnum)->nSndFxKind = *msg.SndFxKind;

	CObserverDlg *pObserverDlg = GetObserverDlg();

	if (pObserverDlg)
	{
		pObserverDlg->User_DelItem(g_PLAYER(pnum)->UI.UNum, g_PLAYER(pnum)->UI.ID);
	}
}

// 결과값을 받아 처리하는 함수.
void CGameHoola::ReceiveSV_GAMEOVERRESULT(int nTotSize, char *lpData)
{
	// 인게임이 아닐 시 해당 함수 로직 진행 안함
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;
	
	// 기권하기 버튼 비활성화
	g_pGameView->m_cGameViewBtnMananager.m_btnGiveUp.Enable(false);
	g_pGameView->m_cGameViewBtnMananager.CloseGiveUpDlg();
	
	CSV_GAMEOVERRESULT msg;
	msg.Get(lpData, nTotSize);
	
	//게임오버구조체 저장
	g_GAMEPROCESS()->GetPlayScr()->SetGameOverResult( msg.GOR);
	g_GAMEPROCESS()->m_GameOverResultData = *msg.GOR;
	g_pGameView->m_cPlayerDrawManager.SetTurn(-1);
	g_GAMEPROCESS()->GetPlayScr()->ResetLimitTime();
	
	if ( FALSE == g_GAMEPROCESS()->GetPlayScr()->IsMyPracticeGameObserver() )
	if (msg.GOR->Hg.sWinnerCase == 1 || msg.GOR->Hg.sWinnerCase == 2)
	{
		//훌라일경우 애니메이션과 사운드 우선 출력
//		g_GAMEPROCESS()->SetResultDelay(5000);
		g_GAMEPROCESS()->GetPlayScr()->PlayHoolaAnimation();

		for ( int i = 0; i < MAX_PLAYER; i++)
		{
			if ( msg.GOR->Ui[i].cRanking == 1 )
			{
				int nPnum = g_GAMEPROCESS()->GetPNum_ByServPN(i);
				PlayVoice( g_PLAYER(nPnum)->UI.Sex, g_PLAYER(nPnum)->nSndFxKind, HOOLA);
			}
		}
	}
	
	//사용자가 나가기 전에 사운드 데이터를 저장해둔다
	g_GAMEPROCESS()->SetPlayerSndKindArray();
	GM().SetWaitResponse(FALSE);
	
	static BOOL bProccessed = FALSE;
	
	if (bProccessed != FALSE)
	{
		return;
	}
	
	bProccessed = TRUE;
	
	//g_pGameView->m_cPlayerDrawManager.ClearTimer();
	
	if ( !g_GAMEPROCESS()->GetResultDelay() )
	{
		g_GAMEPROCESS()->OnGameOver(msg.GOR);
	}
	
	g_cOwnLimit.OnGameOver();
	bProccessed = FALSE;
}


void CGameHoola::ReceiveCL_CHATDATA(int nTotSize, char *lpData)
{
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;
	
	CCL_CHATDATA msg;
	msg.Get(lpData, nTotSize);
	
	if (*msg.RoomNum != g_RI.RoomNum) 
		return;
	
	// 대화 수신 거부중인 아이디이면 무시
	int pnum = g_GAMEPROCESS()->GetPlayerPNum(msg.ID);
	BOOL bAdmin  = (strcmp("관리자",msg.ID)==0);
	
	if (g_cUIData.m_bObserverChat)
	{
	}
	else
	{
		//관전자 채팅 끄기 
		if (strncmp(msg.ID, GM().GetMyInfo()->UI.ID, 15)==0)
		{
			/*if (g_ObserverMan.IsMyObserver() && g_GAMEPROCESS()->m_bObserverChatWarning==FALSE)
			{
				AddGameChatViewEdit("[알림] 관전자 채팅끄기를 설정하신 분께는 관전채팅 내용이 보여지지 않습니다.\n", GetChatColor(ECC2_NOTIFY));
				g_GAMEPROCESS()->m_bObserverChatWarning=TRUE;
			}*/
		}
		else
		{
			//관전 채팅  0 보기 1 안보기
			if (g_Config.bNoObserverChat  && !bAdmin )
			{
				if (pnum>=MAX_PLAYER && pnum < GetMaxNewPlayer()-1)
				{					
					return;
				}
			}
		}
	}

	if (pnum < 0  && !bAdmin)
	{
		return;
	}
	
	CString str;
	char chat[256]={0,};
	strncpy(chat, msg.StrChat, min(*msg.l_StrChat, 255));

	//(추가)
	// 허용되지 않은 특수문자를 사용하였다면 공백문자로 바꿈
	int slen = strlen(chat);
	for (int s = 0; s < slen-1; ++s) 
	{
		// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
		if (chat[s] == 8 || chat[s] == 9 || chat[s] == 10 || chat[s] == 13)
		{
			chat[s] = ' ';
		}
	}
	
	char szTBuf[512] = {0, };
	NMBASE::UTIL::NMIsBadToRep( chat, szTBuf, 512 );
	
	if ( bAdmin )// ### [관리자 모드 작업] ###
	{				
		str.Format("관리자> %s\n",szTBuf);
		AddGameChatViewEdit(str.GetString(), GetChatColor(ECC2_SERVMSG));
	}
	else
	{
		if (g_cUIData.m_bObserverChat)
		{
			if (g_ObserverMan.IsMyObserver())
			{
				if ( pnum >= g_GAMEPROCESS()->GetMaxPlayer())
				{
					str.Format("%s> %s\n", g_PLAYER(pnum)->UI.NickName, szTBuf);
					AddGameChatViewEdit(str, GetChatColor(ECC2_CHAT), true);
				}
			}
			else
			{
				if ( pnum < g_GAMEPROCESS()->GetMaxPlayer())
				{
					str.Format("%s> %s\n", g_PLAYER(pnum)->UI.NickName, szTBuf);
					AddGameChatViewEdit(str, GetChatColor(ECC2_CHAT), true);
				}
			}
		}
		else
		{
			str.Format("%s> %s\n", g_PLAYER(pnum)->UI.NickName, szTBuf);
			AddGameChatViewEdit(str.GetString(), GetChatColor(ECC2_CHAT), true);
		}
		
		str.Format("## ID: %s(%s)> %s\n",g_PLAYER(pnum)->UI.ID, g_PLAYER(pnum)->UI.NickName, szTBuf );
	}
	
	AddLobbyHideChatText(&str);
}

void CGameHoola::ReceiveCL_EXITSUBSCRIPT(int nTotSize, char *lpData)
{
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;
	
	CCL_EXITSUBSCRIPT msg;
	msg.Get(lpData, nTotSize);
	
	int pnum = g_GAMEPROCESS()->GetPlayerPNum(msg.ID);

	if (pnum==-1)
		return;
	
	int bexit = *msg.bExit;

	CString str;

	if (bexit == 0)
	{
		if (strcmp(GM().GetMyInfo()->UI.ID, msg.ID) == 0)
		{
			// 나가기
			g_pGameView->m_bExitSubscript = FALSE;
			ExitGameRequest();
		}
	}
	else if (bexit==1)
	{
		//게임 중 나가기 예약 상태
		if (strcmp(GM().GetMyInfo()->UI.ID, msg.ID) == 0)
		{
			g_pGameView->m_bExitSubscript = TRUE;
		}
		
		str.Format("[%s]님 나가기 예약\n", g_PLAYER(pnum)->UI.NickName);
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
		
		g_pGameView->m_cPlayerDrawManager.SetExitReservation(pnum, true);
	}
	else if (bexit == 2)
	{
		//나가기 취소 
		if (strcmp(GM().GetMyInfo()->UI.ID, msg.ID) == 0)
		{
			g_pGameView->m_bExitSubscript = FALSE;
		}
		
		str.Format("[%s]님 나가기 취소\n", g_PLAYER(pnum)->UI.NickName);
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
		
		g_pGameView->m_cPlayerDrawManager.SetExitReservation(pnum, false);
	}
}

void CGameHoola::ReceiveSV_STARTCARD(int nTotSize, char *lpData)
{
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;
	
	//결과창 다 꺼준다
	g_pGameView->m_cGameResultViewMan.StopResultTimer();
	g_GAMEPROCESS()->StopResultDelay();
	
	g_GAMEPROCESS()->SetStartMsgTitle();
	
	CSV_STARTCARD msg;
	msg.Get(lpData, nTotSize);
	
	g_GAMEPROCESS()->GetPlayScr()->DisableGameStartBtn();
	g_GAMEPROCESS()->GetPlayScr()->DisableGamePracticsBtn();
	
	STARTINFO* pSI = msg.pSC;
	g_RI.State = 1;
	
	//BTS대응 게임종료후 등록플레그 남아있는문제
	for ( int i = 0; i < MAX_PLAYER; i++ )
	{
		g_PLAYER(i)->m_bRegist = FALSE;
		//g_PLAYER(i)->ResetPlayerCard();
	}

	ZeroMemory(g_GAMEPROCESS()->GetPlayScr()->m_PlayerCard, sizeof(char) * MAX_PLAYER * MAX_CARDCOUNT);
	
	g_GAMEPROCESS()->DoPrepareGame(pSI);

	g_GAMEPROCESS()->GetPlayScr()->m_bStartFrame = TRUE;
	g_GAMEPROCESS()->GetPlayScr()->m_nStartFrameStep = 1;
	g_GAMEPROCESS()->GetPlayScr()->m_dwStartFrame = timeGetTime();

	if ( g_GAMEPROCESS()->GetPlayScr()->m_bGoldState )
	{
		BOOL bSex = g_PLAYER(0)->UI.Sex;
		int nSndFxKind = g_PLAYER(0)->nSndFxKind;
		CONFIG cnfig = g_Config;

		if ( g_GAMEPROCESS()->GetPlayScr()->m_nGoldStage == 4 )
		{
 			AutoPlayMSceneSound( &g_sprGold, 2 );
			//PlayVoice( g_PLAYER(0)->UI.Sex, g_PLAYER(0)->nSndFxKind, GOLDSTAGE4);
			PlayVoice( bSex, nSndFxKind, GOLDSTAGE4);
		}
		else
		{
 			AutoPlayMSceneSound( &g_sprGold, 0 );
			//PlayVoice( g_PLAYER(0)->UI.Sex, g_PLAYER(0)->nSndFxKind, GOLDSTAGE2);
			PlayVoice( bSex, nSndFxKind, GOLDSTAGE2);
		}
		
		//g_GAMEPROCESS()->PlayMusic();
	}
	else
	{
		if ( g_GAMEPROCESS()->GetPlayScr()->m_bPlayMusicAfter )
		{
			//g_GAMEPROCESS()->PlayMusic();
			g_GAMEPROCESS()->GetPlayScr()->m_bPlayMusicAfter = FALSE;
		}
	}

	g_GAMEPROCESS()->PlayMusic();

	if ( !g_GAMEPROCESS()->GetPlayScr()->m_bGoldState )
		NMBASE::SOUND::g_Sound.PlayWav( SND58 );
}

void CGameHoola::ReceiveSV_ENDGAME(int nTotSize, char *lpData)
{
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME)
	{
		return;
	}
	
	CSV_ENDGAME msg;
	msg.Get(lpData, nTotSize);
	
	if (g_pGameView)
	{
		// 사이드팟 초기화
		//g_pGameView->m_cSidePotMoney.Reset();
		
		// Animan moment destroy
		g_pGameView->m_cAniSprListManager.Destroy(0);
		
		g_pGameView->m_cGameViewBtnMananager.ResetGameButton();
		
		// 나가기 예약상태면 게임 종료
		if (g_pGameView->m_bExitSubscript)
		{
			ExitGameRequest();
		}
		else
		{
			// 게임 리셋 예약
			GetGameProcess()->ResetGame();
		}
	}

	//훌라코드
	g_GAMEPROCESS()->GetPlayScr()->m_bResult = FALSE;

	for ( int i = 0; i < MAX_PLAYER; i++ )
	{
		if ( strcmp( g_PLAYER(i)->UI.ID, "_PRACTICSUSER_" ) == 0 )
		{
			g_RI.NowUserNum = *msg.Reserve;
			g_PLAYER(i)->Clear();
		}
	}

	g_cPromotionMan.OnEndGame();

	// 게임 상태 변경
	g_GAMEPROCESS()->m_bGameStart = false;

	//매크로 프로그램이 있으면 종료 시킨다
	OnMacroCheck();
}

void CGameHoola::ReceiveSV_SPECIALCARD(int nTotSize, char *lpData)
{	
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	CSV_SPECIALCARD msg;
	msg.Get(lpData, nTotSize);

	CString spec("");

	if (msg.pSC->nCard==CODE_SPECIAL_FCARD)
		spec = "☆포카드☆";
	else if (msg.pSC->nCard==CODE_SPECIAL_SF)
		spec = "☆스트레이트 플러쉬☆";
	else if (msg.pSC->nCard==CODE_SPECIAL_RSF)
		spec ="★로얄 스트레이트 플러쉬★";    
	
	CString str("");
	CString strNick = msg.pSC->NickName;
	//같은방에서 나왔는지 체크		

	if (msg.pSC->JackPotPlus>0)
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

void CGameHoola::ReceiveSV_SEATKEEPER(int nTotSize, char *lpData)
{
	CSV_SEATKEEPER msg;
	msg.Get( lpData, nTotSize );
	g_SeatKeeper.ProcessPacket(&msg);
}

void CGameHoola::ReceiveSV_CHANGE_MONEY(int nTotSize, char *lpData)
{
	CSV_CHANGE_MONEY msg;
	msg.Get(lpData,nTotSize);
	g_GAMEPROCESS()->Change_Money(&msg);
}

void CGameHoola::ReceiveSV_ACCEPT_OUTROOM(int nTotSize, char *lpData)
{
	CSV_ACCEPT_OUTROOM msg;
	msg.Get(lpData, nTotSize);
	
	m_bIsGiveUpRoom = false;
	
	// 기권하기 버튼 감추기
	g_pGameView->m_cGameViewBtnMananager.m_btnGiveUp.Show(false);
	
	GM().SetWaitResponse(false);
	GM().OnExitRoom((REASON_OUTROOM)*msg.Reason);
	
	g_cPromotionMan.Accept_OutRoom();
	
	//방에 나갈때도 게임을 초기화 시킨다
	g_GAMEPROCESS()->ResetGame();
	g_GAMEPROCESS()->GetPlayScr()->m_bShowGameResult = FALSE;
	g_GAMEPROCESS()->GetPlayScr()->DisableGamePracticsBtn();
	g_GAMEPROCESS()->GetPlayScr()->DisableGameStartBtn();
}

void CGameHoola::ReceiveSV_CHANGEMYINFO(int nTotSize, char *lpData)
{
	CSV_CHANGEMYINFO msg;
	msg.Get(lpData, nTotSize);

	memcpy( &GM().GetMyInfo()->UI.AllGameRecord, &msg.pCMI->AllGameRecord, sizeof(msg.pCMI->AllGameRecord) );
	memcpy( &g_PLAYER(0)->UI.AllGameRecord, &msg.pCMI->AllGameRecord, sizeof(msg.pCMI->AllGameRecord) );
	GM().GetMyInfo()->UI.SetMoney( msg.pCMI->PMoney );

	if (GM().GetMyInfo()->UI.GetMoney() < 0)
	{
		GM().GetMyInfo()->UI.SetMoney( 0 );
	}

	g_PLAYER(0)->UI.SetMoney( GM().GetMyInfo()->UI.GetMoney() );
	g_PLAYER(0)->UI.nIcon  = GM().GetMyInfo()->UI.nIcon = GetMoneyGrade( GM().GetMyInfo()->UI.GetTotalMoney() );
}

void CGameHoola::ReceiveSV_USERENTERROOM(int nTotSize, char *lpData)
{
	CSV_USERENTERROOM msg;  
	msg.Get(lpData, nTotSize);

	g_GAMEPROCESS()->User_EnterRoom(&msg);
}

void CGameHoola::ReceiveSV_USEROUTROOM(int nTotSize, char *lpData)
{
	CSV_USEROUTROOM msg;
	msg.Get(lpData, nTotSize);

	g_GAMEPROCESS()->User_OutRoom(&msg);
}

void CGameHoola::ReceiveSV_CHANGEROOMMASTER(int nTotSize, char *lpData)
{
	CSV_CHANGEROOMMASTER msg;
	msg.Get(lpData, nTotSize);

	if (strlen(msg.ID) == 0)
		return;

	BOOL bBreak = TRUE;

	// 닉네임수정 	
	CString NickName = "";

	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		if (strcmp(msg.ID, g_PLAYER(i)->UI.ID) == 0)
		{
			NickName = g_PLAYER(i)->UI.NickName;
			bBreak = FALSE;
			break;
		}
	}

	if (bBreak == TRUE)
		return;

	// 방 정보를 변경한다
	strcpy(g_RI.ID, msg.ID);
	// 닉네임수정 	
	strcpy(g_RI.NickName, NickName);
	//여기까지
	g_RI.UNum = *msg.UNum;
	g_RI.RoomMasterType = *msg.RoomMasterType;
	
	if (strcmp(msg.ID, GM().GetMyInfo()->UI.ID) == 0) // 내가 방장이 된다
	{
		g_MastPN = 0;
	}
	else
	{
		int newMastPN = -1;

		for (int i = 1; i < MAX_PLAYER; ++i) 
		{
			if (strcmp(g_PLAYER(i)->UI.ID, msg.ID)==0) 
			{
				newMastPN = i;
				break;
			}
		}
		
		if (newMastPN == -1)
			return;
		
		g_MastPN = newMastPN;
		
		CString str;
		str.Format("방장변경: [%s]님\n", NickName);
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
		
		str.Format("방장변경:[ID: %s(%s)]님\n",msg.ID, NickName);
		AddLobbyHideChatText(&str);
	}

	g_cBanishMan.Clear();
}

//귓속말
BOOL CGameHoola::ReceiveSV_WhisperChat(int TotSize, char *lpData)
{
	if (GetCurrentWhere() != IDX_GAMEWHERE_GAME) return FALSE;

	CSV_WHISPERCHAT msg;
	msg.Get(lpData, TotSize);

	//귓말 수신 거부일때 모두 무시
	if (CCommunityManager::Instance()->GetOption(COM_ECOT_WHISPER) == COM_ECOPT_OFF)
		return FALSE;
	//블랙리스트의 유저가 보낸 귓말은 무시
	if (CCommunityManager::Instance()->IsExistBlackBuddy(msg.m_szNickName))
		return TRUE;
	//친구에게만 귓말을 받기 옵션일 때 친구가 아닌사람이 보낸 귓말이면 무시
	if ((CCommunityManager::Instance()->GetOption(COM_ECOT_WHISPER) == COM_ECOPT_MYBUDDY) && !CCommunityManager::Instance()->IsExistBuddy(msg.m_szNickName))
		return TRUE;

	// 대화 수신 거부중인 아이디이면 무시
	int pnum = g_GAMEPROCESS()->GetPlayerPNum(msg.m_szID);

	CString str;
	CString chat;
	chat.Append((LPCTSTR)msg.m_StrChat, *msg.m_sStrChatLen);

	//(추가)
	// 허용되지 않은 특수문자를 사용하였다면 공백문자로 바꿈
	for (int s = 0; s<chat.GetLength() - 1; s++)
	{
		// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
		if (chat.GetAt(s) == 8 || chat.GetAt(s) == 9 || chat.GetAt(s) == 10 || chat.GetAt(s) == 13)
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
	str.Format("귓말: ID : %s(%s)> %s\n", msg.m_szID,  msg.m_szNickName, szTBuf );
	AddLobbyHideChatText(&str);
	
	return TRUE;
}

BOOL CGameHoola::ReceiveSV_AdminWhisperChat(int TotSize, char *lpData)
{
	// 관리자모드작업
	CNM_ADMIN_WHISPERCHAT msg;
	msg.Get(lpData, TotSize);

	CString str;
	CString chat;
	chat.Append((LPCTSTR)msg.StrChat, *msg.l_StrChat);

	//(추가)
	// 허용되지 않은 특수문자를 사용하였다면 공백문자로 바꿈
	for (int s = 0; s<chat.GetLength() - 1; s++) 
	{
		// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
		if (chat.GetAt(s) == 8 || chat.GetAt(s) == 9 || chat.GetAt(s) == 10 || chat.GetAt(s) == 13) 
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
void CGameHoola::OnFocusGame(bool bFocus)
{
	if (bFocus)
	{
		// 채팅
		if (g_pChatWinMan)
		{
			g_pChatWinMan->SetCurrnetChat();
		}
		
		// 자리지킴이		
		g_cOwnLimit.Init( &g_cUIData, g_GAMEPROCESS());
		g_cBanishMan.Init(&g_SeatKeeper, &g_ObserverMan);
		g_SeatKeeper.SetChangeThisPoint();

		MM().AddMsgH(UPG_CMK_SET_LIMIT_MONEY,  Fnt(this, &CGameHoola::OnSetLimitMoney));
		MM().AddMsgH(UPG_CMK_SET_LIMIT_GOLD,  Fnt(this, &CGameHoola::OnSetLimitGold));
		MM().AddMsgH(UPG_CMK_GET_MONEY_GRADE,  Fnt(this, &CGameHoola::OnGetMoneyGrade));
		MM().AddMsgH(UPG_CMK_GET_JOKBO_NAME,   Fnt(this, &CGameHoola::OnGetJokboName));
		MM().AddMsgH(UPG_CMK_GET_MYJOKBO_NAME, Fnt(this, &CGameHoola::OnGetMyJokboName));
		MM().AddMsgH(UPG_CMK_GET_RESULT_MONEY, Fnt(this, &CGameHoola::OnGetResultMoney));
		MM().AddMsgH(UPG_CMK_CLEAR_JACKPOTMONEY, Fnt(this, &CGameHoola::OnClearJackPotMoney));
		MM().AddMsgH(UPG_CMK_SET_PLAY_RAKEBACK_GOLD, Fnt(this, &CGameHoola::OnSetPlayRakeBackGold));
		MM().AddMsgH(UPG_CMK_SET_PLAY_RAKEBACK_GOLD_EVENT, Fnt(this, &CGameHoola::OnSetPlayRakeBackGoldEvent));

		MM().AddMsgH(UPG_CMK_GAME_RESULT_OPEN, Fnt(this, &CGameHoola::OnGetResultOpen));

		g_cPromotionMan.Reset();
		g_cPromotionMan.Init(&g_cUIData, this, &g_pGameView->m_cChatWindow);
		g_cPromotionMan.MakePromotion(CPromotionManager::EPT_JOKBOJACKPOT);// 족보잭팟은 결과출력 타이밍을 위해 미리등록해 놓는다.
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

LRESULT CGameHoola::OnSetPlayRakeBackGold(WPARAM &wParam, LPARAM &lParam)
{
	GetGameView()->m_cPlayerDrawManager.SetPlayRakeBackGold((INT64)wParam, (int)lParam);

	return TRUE;
}

LRESULT CGameHoola::OnSetPlayRakeBackGoldEvent(WPARAM &wParam, LPARAM &lParam)
{
	GetGameView()->m_cPlayerDrawManager.SetPlayRakeBackGoldEvent((int)wParam);

	return TRUE;
}

LRESULT CGameHoola::OnClearJackPotMoney(WPARAM &wParam, LPARAM &lParam)
{
	int  nGameCode     = *((int *)wParam);
	INT64 llLimitMoney = *((INT64 *)lParam);

	g_GAMEPROCESS()->ClearJackpotMoney(nGameCode, llLimitMoney);

	return TRUE;
}


LRESULT CGameHoola::OnGetResultMoney(WPARAM &wParam, LPARAM &lParam)
{
	INT64 *pMoney = (INT64 *)lParam;
	CPlayerHoola *pPlayer = g_PLAYER((int)wParam);
	*pMoney = pPlayer->m_n64ResultMoney;

	return TRUE;
}

LRESULT CGameHoola::OnGetMyJokboName(WPARAM &wParam, LPARAM &lParam)
{
	return FALSE;
}

LRESULT CGameHoola::OnGetJokboName(WPARAM &wParam, LPARAM &lParam)
{
	return FALSE;
}


LRESULT CGameHoola::OnGetMoneyGrade(WPARAM &wParam, LPARAM &lParam)
{
	return GetMoneyGrade(*((INT64 *)lParam));
}

LRESULT CGameHoola::OnSetLimitMoney(WPARAM &wParam, LPARAM &lParam)
{
	//보유한도
	g_cOwnLimit.SetMaxPremiumMoneyWithOption(*((INT64 *)lParam));

	return TRUE;
}

LRESULT CGameHoola::OnSetLimitGold(WPARAM &wParam, LPARAM &lParam)
{
	//보유한도
	g_cOwnLimit.SetMaxPremiumGoldWithOption(*((INT64 *)lParam));

	return TRUE;
}

CString CGameHoola::GetLobbyEnterSound(bool bMan)
{
	if (bMan)
		return ".\\Hoola\\sound\\enterlobby_m.wav";

	return ".\\Hoola\\sound\\enterlobby_f.wav";
}

LRESULT CGameHoola::OnGetResultOpen( WPARAM &wParam, LPARAM &lParam )
{

	if (g_pGameView==NULL)
		return FALSE;

	return g_pGameView->m_cPlayerDrawManager.IsOpen();
}

const HICON CGameHoola::GetGameIcon(void)
{
	return AfxGetApp()->LoadIcon(IDI_ICON_HOOLA);
}

const HICON CGameHoola::GetChatIcon(void)
{
	return AfxGetApp()->LoadIcon(IDI_ICON_CHAT_HOOLA);
}


const CString	CGameHoola::GetModuleName(void)
{
	char szFileName[MAX_PATH+1]={0,};
	GetModuleFileName(g_hInstGameDLL, szFileName, MAX_PATH);

	return CString(szFileName);
}

void CGameHoola::ReceiveSV_DISCARD(int nTotSize, char *lpData)
{
	CSV_DISCARD msg;
	msg.Get(lpData, nTotSize);

	g_GAMEPROCESS()->GetPlayScr()->m_dqNewCard.clear();
	g_GAMEPROCESS()->GetPlayScr()->SetMoveDumpCard( msg.pTC->CardNum, msg.pTC->ThankNum, *msg.PNum);

	int pnum = g_GAMEPROCESS()->GetPlayScr()->m_nTurnCnt - g_PLAYER(0)->ServPNum;
	if ( pnum < 0 ) pnum += MAX_PLAYER;

	if ( pnum == 0 )
	{
		g_GAMEPROCESS()->GetPlayScr()->m_dwHelpTime = 0;
		g_GAMEPROCESS()->GetPlayScr()->m_nHelpIndex = -1;
	}
}

void CGameHoola::ReceiveSV_REGRESULT(int nTotSize, char *lpData)
{
	CSV_REGRESULT msg;
	msg.Get(lpData, nTotSize);
	g_GAMEPROCESS()->GetPlayScr()->SetMoveRegCard( &msg );
}

void CGameHoola::ReceiveSV_STOPGAMERESULT(int nTotSize, char *lpData)
{
	CSV_STOPGAMERESULT msg;
	msg.Get(lpData, nTotSize);

	int pnum  = *msg.PNum - g_PLAYER(0)->ServPNum;
	if ( pnum < 0 ) pnum += MAX_PLAYER;

// 	if ( pnum < 3 )
// 		g_GAMEPROCESS()->GetPlayScr()->SetEffect( msg.ID, 2 );
// 	else
// 		g_GAMEPROCESS()->GetPlayScr()->SetEffect( msg.ID, 1 );

	if ( FALSE == g_GAMEPROCESS()->GetPlayScr()->IsMyPracticeGameObserver() )
	{
		g_pGameView->m_cEffectDrawMan.PlayAnimation( eNUM_EFFECT_STOP, msg.ID );
		PlayVoice( g_PLAYER(pnum)->UI.Sex, g_PLAYER(pnum)->nSndFxKind, STOP );
	}

// 	g_GAMEPROCESS()->GetPlayScr()->SetResultDelay(TRUE);
// 	g_pGameView->m_cGameResultViewMan.SetResultTimer();
	g_GAMEPROCESS()->SetResultDelay(2000);
	g_GAMEPROCESS()->GetPlayScr()->ResetLimitTime();
}

void CGameHoola::ReceiveSV_THANKRESULT(int nTotSize, char *lpData)
{
	CSV_THANKRESULT msg;
	msg.Get(lpData, nTotSize);

	g_GAMEPROCESS()->GetPlayScr()->SetNullButton();
	g_GAMEPROCESS()->GetPlayScr()->ResetTurnFlag();
	g_GAMEPROCESS()->GetPlayScr()->ResetSelect();
	g_GAMEPROCESS()->GetPlayScr()->m_nTurnCnt = *msg.PNum;

	char cDumpCard = g_GAMEPROCESS()->GetPlayScr()->m_DisCard[g_GAMEPROCESS()->GetPlayScr()->m_nDisCardCnt-1];

	int pnum  = *msg.PNum - g_PLAYER(0)->ServPNum;

	if ( pnum < 0 ) 
		pnum += MAX_PLAYER;

	g_pGameView->m_cEffectDrawMan.PlayAnimation( eNUM_EFFECT_THANK, msg.ID );
	g_GAMEPROCESS()->GetPlayScr()->SetMoveThankCard( msg.pRT->nThankYouCnt, msg.pRT->ThankCard, cDumpCard, pnum, msg.pRT->ThankCase );
	g_GAMEPROCESS()->GetPlayScr()->ResetLimitTime();

	g_PLAYER(pnum)->SetRegist();
}

void CGameHoola::ReceiveSV_SPECIALRULERESULT(int nTotSize, char *lpData)
{
	CSV_SPECIALRULERESULT msg;
	msg.Get(lpData, nTotSize);
	
	if ( *msg.Kind == 4 || *msg.Kind == 5 )
	{
		g_GAMEPROCESS()->GetPlayScr()->SetRuleWinEffect( *msg.Kind, msg.Card );
	}
	else if ( *msg.Kind == 9 ) // 7포카드
	{
		g_GAMEPROCESS()->GetPlayScr()->Set7cardEffect();
	}

//	g_GAMEPROCESS()->GetPlayScr()->SetResultDelay(TRUE);
//	g_GAMEPROCESS()->SetResultDelay(1880);
	g_GAMEPROCESS()->GetPlayScr()->ResetLimitTime();
}

void CGameHoola::ReceiveSV_TURNTHANK(int nTotSize, char *lpData)
{
	CSV_TURNTHANK msg;
	msg.Get( lpData, nTotSize );

	if ( *msg.CardNum >= 0 )
	{
		if ( *msg.FirstTurn )
		{
			g_GAMEPROCESS()->GetPlayScr()->m_DisCard[g_GAMEPROCESS()->GetPlayScr()->m_nDisCardCnt++] = *msg.CardNum;
		}

		if ( *msg.CanThank )
		{
			// 땡큐 로우 족보 체크
			if ( g_PLAYER(0)->m_bRegist == FALSE && g_GAMEPROCESS()->GetPlayScr()->IsLowCardByDumpCard( *msg.CardNum ) )
			{
				g_GAMEPROCESS()->GetPlayScr()->SetThankButton(CPlayScr::eTHANK_LOW);
				g_GAMEPROCESS()->GetPlayScr()->SetLimitTime( CPlayScr::eTIMER_THANK_SPECIAL );
			}
			else if ( g_PLAYER(0)->m_bRegist == FALSE && g_GAMEPROCESS()->GetPlayScr()->IsSevenFourCardByDumpCard( *msg.CardNum ) )
			{
				g_GAMEPROCESS()->GetPlayScr()->SetThankButton(CPlayScr::eTHANK_SEVEN4CARD);
				g_GAMEPROCESS()->GetPlayScr()->SetLimitTime( CPlayScr::eTIMER_THANK_SPECIAL );
			}
			else if ( g_PLAYER(0)->m_bRegist == FALSE && g_GAMEPROCESS()->GetPlayScr()->IsHighCardByDumpCard( *msg.CardNum ) )
			{
				g_GAMEPROCESS()->GetPlayScr()->SetThankButton(CPlayScr::eTHANK_HIGH);
				g_GAMEPROCESS()->GetPlayScr()->SetLimitTime( CPlayScr::eTIMER_THANK_SPECIAL );
			}
			else
			{
				g_GAMEPROCESS()->GetPlayScr()->CheckThank( *msg.CardNum );
				g_GAMEPROCESS()->GetPlayScr()->SetThankButton(CPlayScr::eTHANK_NORMAL);
				g_GAMEPROCESS()->GetPlayScr()->SetLimitTime( CPlayScr::eTIMER_THANK_NORMAL );
			}
			if ( strcmp(msg.ID, GM().GetMyInfo()->UI.ID) == 0 )
				g_GAMEPROCESS()->GetPlayScr()->SetSortBtn();
		}
		else
		{
			g_GAMEPROCESS()->GetPlayScr()->SetNullButton();
		}
	}
	
	// 내 턴일때 기권하기 버튼 활성화
	if (*msg.CanThank && (strcmp(msg.ID, GM().GetMyInfo()->UI.ID) == 0))
	{
		g_pGameView->m_cGameViewBtnMananager.m_btnGiveUp.Enable(true);
	}
	else
	{
		g_pGameView->m_cGameViewBtnMananager.m_btnGiveUp.Enable(false);
		g_pGameView->m_cGameViewBtnMananager.CloseGiveUpDlg();
	}
}

void CGameHoola::ReceiveSV_TURNGET(int nTotSize, char *lpData)
{
	if ( g_GAMEPROCESS()->GetPlayScr()->m_bPracticsGame && !g_ObserverMan.IsMyObserver() && strcmp( g_PLAYER(0)->UI.ID, GM().GetMyInfo()->UI.ID ) != 0 )
	{
		return;
	}

	CSV_TURNGET msg;
	msg.Get( lpData, nTotSize );

	g_GAMEPROCESS()->GetPlayScr()->m_dqNewCard.clear();

	if ( *msg.CardNum >= 0 )
	{
		g_GAMEPROCESS()->GetPlayScr()->m_DisCard[g_GAMEPROCESS()->GetPlayScr()->m_nDisCardCnt++] = *msg.CardNum;
	}

	g_GAMEPROCESS()->GetPlayScr()->SetNullButton();
	g_GAMEPROCESS()->GetPlayScr()->ResetSelect();

	g_GAMEPROCESS()->GetPlayScr()->ResetTurnFlag();
	g_GAMEPROCESS()->GetPlayScr()->SetTurnID( msg.TurnID );
	g_GAMEPROCESS()->GetPlayScr()->m_nTurnIndex = 0;

	if ( *msg.Magic == FALSE )
	{
		if ( !g_ObserverMan.IsMyObserver() && g_GAMEPROCESS()->GetPlayScr()->m_nTurnCnt == g_PLAYER(0)->ServPNum )
		{
			g_GAMEPROCESS()->GetPlayScr()->SetGetCardButton();
		}

		g_GAMEPROCESS()->GetPlayScr()->CheckReg();

		// 받기 타임
		g_GAMEPROCESS()->GetPlayScr()->SetLimitTime( CPlayScr::eTIMER_GET );
	}
	
	// 내 턴일때 기권하기 버튼 활성화
	if ( strcmp(msg.TurnID, GM().GetMyInfo()->UI.ID) == 0 )
	{
		g_pGameView->m_cGameViewBtnMananager.m_btnGiveUp.Enable(true);
	}
	else
	{
		g_pGameView->m_cGameViewBtnMananager.m_btnGiveUp.Enable(false);
		g_pGameView->m_cGameViewBtnMananager.CloseGiveUpDlg();
	}
}

void CGameHoola::ReceiveSV_TURNREG(int nTotSize, char *lpData)
{
	if ( g_GAMEPROCESS()->GetPlayScr()->m_bPracticsGame && !g_ObserverMan.IsMyObserver() && strcmp( g_PLAYER(0)->UI.ID, GM().GetMyInfo()->UI.ID ) != 0 )
	{
		return;
	}

	CSV_TURNREG msg;
	msg.Get( lpData, nTotSize );

	g_GAMEPROCESS()->GetPlayScr()->SetTurnID( msg.TurnID );
	g_GAMEPROCESS()->GetPlayScr()->SetNullButton();

	if ( !g_ObserverMan.IsMyObserver() && g_GAMEPROCESS()->GetPlayScr()->m_nTurnCnt == g_PLAYER(0)->ServPNum )
	{
		g_GAMEPROCESS()->GetPlayScr()->m_nTurnIndex = 1;

		if ( g_GAMEPROCESS()->GetPlayScr()->CheckReg() )
		{
			g_GAMEPROCESS()->GetPlayScr()->SetRegCardButton();
		}
		else
		{
			g_GAMEPROCESS()->GetPlayScr()->SetDumpCardButton();
		}
	}

	g_GAMEPROCESS()->GetPlayScr()->SetLimitTime( CPlayScr::eTIMER_REGIST );

	if ( strcmp(msg.TurnID, GM().GetMyInfo()->UI.ID) == 0 )
		g_GAMEPROCESS()->GetPlayScr()->SetSortBtn();
}

void CGameHoola::ReceiveSV_CANTHANK(int nTotSize, char *lpData)
{
	CSV_CANTHANK msg;
	msg.Get( lpData, nTotSize );
	g_GAMEPROCESS()->GetPlayScr()->SendThank( *msg.PNum );
}

void CGameHoola::ReceiveSV_ADDEDCARDDATA(int nTotSize, char *lpData)
{
	CSV_ADDEDCARDDATA msg;
	msg.Get( lpData, nTotSize );
	g_GAMEPROCESS()->GetPlayScr()->SetAddedCardData( msg.StCards, msg.ToCards, msg.ID, *msg.DisCard, msg.PlayerCard );
}

void CGameHoola::ReceiveSV_FIXREGIST(int nTotSize, char *lpData)
{
	CSV_FIXREGIST msg;
	msg.Get( lpData, nTotSize );
	/*int nPnum = g_GAMEPROCESS()->GetPNum_ByServPN(*msg.PNum);*/
	g_GAMEPROCESS()->GetPlayScr()->SetFixCardData( msg );
	g_GAMEPROCESS()->GetPlayScr()->FixRegist( *msg.PNum );
}

void CGameHoola::ReceiveSV_MISSION(int nTotSize, char *lpData)
{
	CSV_MISSION msg;
	msg.Get( lpData, nTotSize );
	g_GAMEPROCESS()->GetPlayScr()->SetMission( *msg.MissionNum, *msg.MissionBonus, *msg.MaxMission );
}

void CGameHoola::ReceiveSV_MISSIONCOMPLETE(int nTotSize, char *lpData)
{
	CSV_MISSIONCOMPLETE msg;
	msg.Get( lpData, nTotSize );
	if ( FALSE == g_GAMEPROCESS()->GetPlayScr()->IsMyPracticeGameObserver() )
	{
		g_GAMEPROCESS()->SetMissionCompleteMsgTitle( msg.ID );
		g_GAMEPROCESS()->GetPlayScr()->CompMission( msg.ID, *msg.GetMoney );
	}
}

void CGameHoola::ReceiveSV_MAGICCARD(int nTotSize, char *lpData)
{
	if ( g_GAMEPROCESS()->GetPlayScr()->m_bPracticsGame && !g_ObserverMan.IsMyObserver() && strcmp( g_PLAYER(0)->UI.ID, GM().GetMyInfo()->UI.ID ) != 0 )
	{
		return;
	}
	CSV_MAGICCARD msg;
	msg.Get( lpData, nTotSize );
//	g_GAMEPROCESS()->GetPlayScr()->SetMagic( *msg.CardNum, msg.CardList );
}

void CGameHoola::ReceiveCL_BRINGINCARD(int nTotSize, char *lpData)
{
	CCL_BRINGINCARD msg;
	msg.Get(lpData, nTotSize);
	g_GAMEPROCESS()->GetPlayScr()->SetMoveGetCard( *msg.CardNum, *msg.PNum );
}

void CGameHoola::ReceiveCL_SUN_PICKUP_START(int nTotSize, char *lpData)
{
	CCL_SUN_PICKUP_START msg;
	msg.Get( lpData,nTotSize );

	g_pGameView->m_cPlayerDrawManager.SetTurn(-1);

	if (g_pGameView)
	{

		g_pGameView->m_cGameViewBtnMananager.ResetGameButton();

		// 나가기 예약상태면 게임 종료
		if (g_pGameView->m_bExitSubscript)
		{
			ExitGameRequest();
		}
		else
		{
			// 게임 리셋 예약
			GetGameProcess()->ResetGame();
		}
	}

	//훌라코드
	g_GAMEPROCESS()->GetPlayScr()->m_bResult = FALSE;
	g_GAMEPROCESS()->m_bGameStart = true;
	for ( int i = 0; i < MAX_PLAYER; i++ )
	{
		if ( strcmp( g_PLAYER(i)->UI.ID, "_PRACTICSUSER_" ) == 0 )
		{
			g_PLAYER(i)->Clear();
		}
	}

	//선정하기가 시작되면 시작버튼이 노출되서는 안된다.
	g_GAMEPROCESS()->GetPlayScr()->m_cSunPickup.StartInfo( *msg.CardNum );
	g_GAMEPROCESS()->GetPlayScr()->DisableGameStartBtn();
	g_GAMEPROCESS()->GetPlayScr()->DisableGamePracticsBtn();
}

void CGameHoola::ReceiveCL_SUN_PICKUP(int nTotSize, char *lpData)
{
	CCL_SUN_PICKUP msg;
	msg.Get( lpData,nTotSize );
	g_GAMEPROCESS()->GetPlayScr()->m_cSunPickup.PickupUser( *msg.CardIndex, *msg.CardNo, *msg.pnum );
}

void CGameHoola::ReceiveCL_SUN_PICKUP_COMPLETE(int nTotSize, char *lpData)
{
	CCL_SUN_PICKUP_COMPLETE msg;
	msg.Get( lpData,nTotSize );
	g_GAMEPROCESS()->GetPlayScr()->m_cSunPickup.PickUpComplete( *msg.pnum);
}

void CGameHoola::ReceiveSV_JACKPOTCOUNT(int nTotSize, char *lpData)
{
	if (g_RI.cRuleType != hoola::eRULE_TYPE_STOP )
	{
		return;
	}

	CSV_JACKPOTCOUNT msg;
	msg.Get( lpData, nTotSize );       
	g_GAMEPROCESS()->GetPlayScr()->SetJackpotState( msg.ID, *msg.Count );

	if (*msg.Count == 3)
	{
//		g_GAMEPROCESS()->SetResultDelay(9860);
		g_pGameView->m_cGameResultViewMan.SetJackpotDelay();
	}
}

void CGameHoola::SetBGMVolume(int nVolume)
{
	//0-100 구간의 볼륨을 60-100 구간으로 바꾼다/
	//100:40 = nVolume : nRelativeVolume;
	//nRelativeVolume = 40*nVolume /100
// 	const int MIN_VOLUME = 60;
//  
// 	int nRelativeVolume = 0;
// 	if (nVolume>0)
// 		nRelativeVolume = ((100-MIN_VOLUME) * nVolume) / 100;
// 
// 	nRelativeVolume += MIN_VOLUME;
// 
// 	NMBASE::SOUND::g_Sound.SetVolumeAllMusic(nRelativeVolume, -1);

	if ( g_GAMEPROCESS()->m_eMusicType == eMUSIC_NULL )
		return;

	//0-100 구간의 볼륨을 60-100 구간으로 바꾼다/
	//100:40 = nVolume : nRelativeVolume;
	//nRelativeVolume = 40*nVolume /100
	/*const int MIN_VOLUME = 60;

	int nRelativeVolume = 0;
	if (nVolume>0)
		nRelativeVolume = ((100-MIN_VOLUME) * nVolume) / 100;
	nRelativeVolume += MIN_VOLUME;*/

	if ( g_GAMEPROCESS()->m_eMusicType == eMUSIC_NORMAL )
		NMBASE::SOUND::g_Sound.SetVolume( MUSIC_NORMAL, nVolume);
	else if ( g_GAMEPROCESS()->m_eMusicType == eMUSIC_GOLD )
		NMBASE::SOUND::g_Sound.SetVolume( MUSIC_GOLD, nVolume);
}

void CGameHoola::SetupByConfig()
{
	CPlayer *pPlayer = GM().GetMyInfo();
	if (pPlayer == NULL) return;

	if ( g_PLAYER(0) != NULL )
	{
		if ( strcmp(g_PLAYER(0)->UI.ID, GM().GetMyInfo()->UI.ID) == 0 )
		{
			g_PLAYER(0)->nSndFxKind = g_Config.SndFxKind;
		}
	}

	//설정된 나의 보이스 타입을 서버로 보낸다.	
	pPlayer->nSndFxKind = g_Config.SndFxKind;

	CSV_ASK_CHANGESNDFX msg;
	msg.Set(pPlayer->UI.UNum, pPlayer->ServPNum, g_RI.RoomNum, pPlayer->UI.ID, g_Config.SndFxKind);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
}

//### [관리자 모드 작업] ###  방정보 변경
LONG CGameHoola::OnReceiveChangeStyleRoomInfo(int TotSize, char *lpData)
{
	// 현재 위치가 겜방인가, 대기실 인가
	CSV_CHANGESTYLEROOMINFO msg;
	msg.Get(lpData, TotSize);
	
	if (msg.SRI->nMaxUserNum < 100)
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

void CGameHoola::ReceiveSV_GIVEUP_RULE_NOTI(int TotSize, char *lpData)
{
	//if (GetCurrentWhere() != IDX_GAMEWHERE_GAME)
	//	return;
	
	m_bIsGiveUpRoom = true;
	
	// 기권방에 들어갔을 경우 기권하기 버튼을 Show 한다.
	g_pGameView->m_cGameViewBtnMananager.m_btnGiveUp.Show(true);
	g_pGameView->m_cGameViewBtnMananager.m_btnGiveUp.Enable(false);
}

void CGameHoola::ReceiveSV_GIVEUP_LOSE_MONEY_NOTI(int TotSize, char *lpData)
{
	//if (GetCurrentWhere() != IDX_GAMEWHERE_GAME)
	//	return;
	
	CSV_GIVEUP_LOSE_MONEY_NOTI msg;
	msg.Get(lpData, TotSize);
	
	INT64 i64LoseMoney = *msg.m_LoseMoney;
	
	g_pGameView->GetGameButtonMan()->SetGiveUpMoney(i64LoseMoney);
}

void CGameHoola::Wait(DWORD dwMillisecond)
{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();
	
	while(GetTickCount() - dwStart < dwMillisecond)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}
