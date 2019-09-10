#include "StdAfx.h"

#include "GameSutda.h"
#include "GlobalGame.h"
#include "SpriteList.h"
#include "SelectCardSutda.h"
#include "GameViewSutda.h"
#include "GameProcessSutda.h"
#include "CommMsgDef_Game.h"
#include "Resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAX_TURN_TIMEOUT_LIMIT_COUNT_FOR_SUTDA (3)

CGameSutda::CGameSutda(void)
{	
}

CGameSutda::~CGameSutda(void)
{
#ifdef TRACE_OUTPUT
	TTrace::Stop();
#endif
}

BOOL CGameSutda::Create(CWnd *pParentWnd)
{
#ifdef TRACE_OUTPUT
	//TTrace::Init();
	//TTrace::ClearAll();
//	TTrace::Options()->sendMode = WinMsg ;
//	TTrace::Show(true) ;
	TTrace::Debug()->Send ("7Poker����-Create", "CGameSutda");   // single byte string	
#endif

	// ���� ���� View�� �����Ѵ�.
	CGameViewSutda* pGameView = new CGameViewSutda(this);
	g_pGameView		= pGameView;
	m_pGameViewBase	= pGameView;

	CRect rect(0, 0, 1000, 740);
	pGameView->Create(NULL, "GameView", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rect, pParentWnd, 12345);
	pGameView->ShowWindow(SW_HIDE);

	// Create GameProcess
	CGameProcessSutda *pGP = new CGameProcessSutda(this);
	m_pGameProcess = pGP;
	m_pGameProcess->Init(&g_pGameView->Page);

	return CGame::Create(pParentWnd);
}


CGameProcessSutda* CGameSutda::GetGameProcess()
{
	return (CGameProcessSutda*) m_pGameProcess;
}

// ����� ���̾�α�
LRESULT CGameSutda::ShowCreateRooomDlg(ROOMINFO *pRoomInfo, ROOMOPTION *pWebRoomOption)
{
	CCreateRoomDlg dlg(GetGameType(), GM().GetMainWnd());
	dlg.SetWebSelectedRoomOption(pWebRoomOption);
	LRESULT nRet = dlg.DoModal();
	
	if(nRet == IDOK)
	{
		strncpy_s(pRoomInfo->Title, 32, dlg.m_strRoomTitle, dlg.m_strRoomTitle.GetLength());

		pRoomInfo->FormKind			= dlg.m_nGameMode;		
		pRoomInfo->bSuperMaster		= PBIsSuperRoom();
		pRoomInfo->ChipKind			= dlg.m_nChipKind;
		pRoomInfo->bIsFriendRoom	= dlg.m_bFriend;
		pRoomInfo->BetMoney			= dlg.m_i64SeedMoney;
		pRoomInfo->llEnterLimitMoney= dlg.m_i64MinimumMoney;
		//pRoomInfo->bVIPEnterLimitYN = dlg.m_bVIPEnterLimitYN;
		pRoomInfo->nAutoBetKind		= dlg.m_sAutoBet;
		pRoomInfo->sBetRuleKind			= dlg.m_sBetRuleKind;

		//������ : �������߰�
		pRoomInfo->fBetMoneyMulti			= dlg.fBetMoneyMulti;


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

// �ٷ����� �Լ� ( Ŭ�� -> ���� )
int CGameSutda::GetDirectEnterRoomType(int nType)
{
	return CHOICE_GAME;
}

void CGameSutda::CheckDirectEnterRoomInfo(int nType, ROOMINFO *pRoomInfo)
{
	CString strTitle;
	MM().Call(UPG_CMK_GENERATE_RANDOM_ROOM_NAME, 0, (LPARAM)(&strTitle));
	strncpy_s(pRoomInfo->Title, 32, strTitle, strTitle.GetLength());
	pRoomInfo->bSuperMaster = PBIsSuperRoom();
}

// �������� ���̾�α�
LRESULT	CGameSutda::ShowQuickJoinerDlg(void)
{
	CQuickJoinDlg dlg(EQJ_SUTDA, &g_cUIData, GM().GetMainWnd());
	LRESULT nRet = dlg.DoModal();

	if(nRet == IDOK)
	{		
		GM().SendQuickJoiner();
	}

	return FALSE;
}

LRESULT	CGameSutda::SendQuickJoiner(void)
{
	if( OnMacroCheck() )
	{
		return FALSE;
	}
	
	//NMBASE::SOUND::g_Sound.PlayWav(SND45);

	//bool bQJEnable = CQuickJoinDlg::IsEnableQJ(EQJ_SUTDA);

	//if(!bQJEnable)
	//{
	//	GM().AskDirectEnterRoom(false, NULL, -1);
	//	return TRUE;

	//}

	// ���� ������ �۾�
	// �ٷ� ������ �κ񿡼� ȣ���ϸ�, ������ �������� ����.
	GM().AskDirectEnterRoom(false, NULL, GetDefaultFormKind());

	// ���� �� ����ϴ� �κ��� �ϴ� ������.
	//SCONDITION sInfo; 
	//CString strSection;
	//strSection.Format("QJ%02d_%s_%d", QJ_VER, GM().GetMyInfo()->UI.ID, EQJ_SUTDA);

	//if(CQuickJoinDlg::GetSearchCondition(EQJ_SUTDA, strSection, &sInfo))
	//{
	//	//������� �ٷ�����
	//	//CCL_ASK_PREMQUICKJOIN pqj;
	//	//pqj.Set((IDX_GAME)EQJ_SUTDA, bQJEnable, GM().GetMyInfo()->nSndFxKind, sInfo);
	//	//NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), pqj.pData, pqj.GetTotalSize());

	//	CDataRoomOptionList *pDataRoomOptionList = GM().GetDataRoomOptionList();
	//	if(pDataRoomOptionList == NULL) return FALSE;

	//	//������ ��쿡��  ������� �ٷ����� ������ ���÷꿡 ���� ������ ���� �õ�Ӵ� �Ը� �����Ƿ�
	//	//�ٸ����Ӱ� �޸� GetQJRoomOption ȣ��� ù��° ���ڷ� ���÷��뿩�� ��� �õ�ӴϱԸ��� ��뿩�θ� �Ѱ���� ����ε� ����� ���� �� �ִ�.
	//	bool bUseBetRule = sInfo.m_bUseSeedMoney; 

	//	//ROOMOPTION *pResultRoomOption = pDataRoomOptionList->GetQJRoomOption(bUseBetRule, sInfo.m_bAutoBet, sInfo.m_bUseSeedMoney, sInfo.m_nSeedType, sInfo.m_sBetRuleKind );
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
	//	if(sInfo.m_bUseGameMode) nGameMode = sInfo.m_nGameMode;

	//	GM().AskDirectEnterRoom(true, pResultRoomOption, nGameMode, ECALL_BTNTYPE_LOBY_QUICK, true, sInfo.m_bAutoEnterRoom);
	//}
	//else
	//{
	//	// �Ϲ����� �� ����
	//	GM().AskDirectEnterRoom(false, NULL, -1);
	//}

	return TRUE;
}

CPlayer *CGameSutda::CreateNewPlayer(void)
{
	return new CPlayerSutda;
}

void CGameSutda::AddGameChat(const char *str, COLORREF crNewColor, bool bUserChat)
{	
	g_pChatWinMan->AddChat(str, crNewColor, bUserChat);
}

void CGameSutda::AddGameInfo(const char *str, COLORREF crNewColor, bool bUserChat)
{	
	g_pChatWinMan->AddInfo(str, crNewColor, bUserChat);
}


void CGameSutda::OnChangeFullScreen(bool bFullScreen)
{
	// ### [ ������� ] ###
	g_pGameView->m_cGameViewBtnMananager.SetChangeDisplayButton(bFullScreen);
	g_pGameView->Page.ReCreateDIBSection();
}

void CGameSutda::OnCloseOptionDlg(LRESULT nRet)
{
	if(nRet == IDOK) g_pGameView->m_cGameViewBtnMananager.SetBetBtnType(g_Config.eBetBtnType);
}

void CGameSutda::OnLoadingEnd(void)
{
	// ��������Ʈ �ε� �� �ʱ�ȭ ~
	GetGameProcess()->OnInit();
	GetGameView()->OnInit();	
}


CGameViewSutda* CGameSutda::GetGameView()
{
	return (CGameViewSutda*) m_pGameViewBase;
}

void CGameSutda::LoadSprite()
{	
	CGame::LoadSprite();
	LoadSpriteFile();
}

void CGameSutda::LoadSound()
{
	CGame::LoadSound();
	RegistSoundFiles();
}

void CGameSutda::ReleaseSprite()
{
	CGame::ReleaseSprite();
}

void CGameSutda::ReleaseSound()
{
	CGame::ReleaseSound();
}

const IDX_GAME CGameSutda::GetGameType(void)
{
	return IDX_GAME_SD;
}

const int CGameSutda::GetMaxRoom(void)
{
	return 90;
}

const int CGameSutda::GetMaxPlayer(void)
{
	return MAX_PLAYER;
}

const int CGameSutda::GetMaxGamePlayer(void)
{
	return MAX_PLAYER;
}

const int CGameSutda::GetMaxObserver(void)
{
	return MAX_OBSERVER;
}

const int CGameSutda::GetJokboQuestCount(void)
{
	return 6;	//������

}

const CString CGameSutda::GetGameString(void)
{
	return "sutda";
}

const CString CGameSutda::GetGameName(void)
{
	return "����";
}
const COLORREF	CGameSutda::GetGameColor(void)
{	
	return RGB(100,89,0);//�� ���� ������ ���� ��ȯ
}


const int CGameSutda::GetGameCode(void)
{
	return GAME_CODE_SD;
}

const CString CGameSutda::GetLoadingBackPath(void)
{
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		return ".\\sutda\\image\\LoadingBack_toctoc.bmp";
	else
		return ".\\sutda\\image\\LoadingBack.bmp";
#else //UPGRADE_20120308_TOCTOC_CHANNELING
	return ".\\sutda\\image\\LoadingBack.bmp";
#endif //UPGRADE_20120308_TOCTOC_CHANNELING
}

const int CGameSutda::GetDefaultFormKind(void)
{
	return SD_2CARD;
}


// ��Ʈ�� �޽����� �޾� ó���ϴ� �Լ�(CClientDlg::OnPacketNotify ���� ���ӿ� ���õ� ��Ŷ�� ����� �̵�)
LONG CGameSutda::OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData)
{	
	if (CGame::OnPacketNotify(pName, Signal, TotSize, lpData))
		return TRUE;

	if(g_cOneShotCharge.OnPacketNotify(pName,Signal, TotSize, lpData))
		return TRUE;
	
// 	if(g_pGameView->m_cGameHelper.OnPacketNotify(pName,Signal, TotSize, lpData)) 
// 		return TRUE;

	if(g_cOwnLimit.OnPacketNotify(pName,Signal, TotSize, lpData)) 
		return TRUE;

	if(g_cBanishMan.OnPacketNotify(pName, Signal, TotSize, lpData))
		return TRUE;

	if( g_cPromotionMan.OnPacketNotify(pName, Signal, TotSize, lpData) ) return TRUE;

	if( g_cVipJackpot.OnPacketNotify(pName, Signal, TotSize, lpData) ) return TRUE;

#ifdef _DEBUG
	
#define TRACE_PACKET_CASE(sig) case sig: \
	{ COleDateTime time; time=COleDateTime::GetCurrentTime(); CString strTime=time.Format("%H:%M:%S");  \
	TRACE( "[%s] Packet Received : " #sig "\n", strTime ); }
#else
#define TRACE_PACKET_CASE(sig) case sig:
#endif


	switch(Signal)
	{
		// ���ӽ��۹�ư Ȱ��ȭ
	TRACE_PACKET_CASE(SV_STARTBTNENABLE)
		ReceiveSV_STARTBTNENABLE(TotSize, lpData);		
		break;

		// �������
	TRACE_PACKET_CASE(SV_OBSERVER_MESSAGE)
		ReceiveSV_OBSERVER_MESSAGE(TotSize, lpData);
		break;

		// �������(������ ����)
	TRACE_PACKET_CASE(SV_RESERVATIONINFO)
		ReceiveSV_RESERVATIONINFO(TotSize, lpData);		
		break;
		
		// �������(�������� -> ����)
	TRACE_PACKET_CASE(SV_ACCEPT_EMERAGE)
		ReceiveSV_ACCEPT_EMERAGE(TotSize, lpData);
		break;

		// �������(���� -> ��������)
	TRACE_PACKET_CASE(SV_ACCEPT_PARTICIPATION)
		ReceiveSV_ACCEPT_PARTICIPATION(TotSize, lpData);		
		break;

		// �������(�������� -> ����)
	TRACE_PACKET_CASE(SV_ENTER_EMERAGE)
		ReceiveSV_ENTER_EMERAGE(TotSize, lpData);
		break;

		// �������(���� -> ������)
	TRACE_PACKET_CASE(SV_ENTER_PARTICIPATION)
		ReceiveSV_ENTER_PARTICIPATION(TotSize, lpData);
		break;

		// ������ ����Ǿ���
	TRACE_PACKET_CASE(SV_GAMEOVERRESULT) 
		ReceiveSV_GAMEOVERRESULT(TotSize, lpData);			 
		break;

		/////////////////////////// Ŭ���̾�Ʈ�� �޼���  ////////////////////////

		// ���ӹ� ä�õ���Ÿ
	TRACE_PACKET_CASE(CL_CHATDATA)
		ReceiveCL_CHATDATA(TotSize, lpData);		 
		break;		
	
	case SV_CHANGESTYLEROOMINFO:		  
		{	
			return OnReceiveChangeStyleRoomInfo( TotSize, lpData ); 
		} break; //BTS ����͸� �� ���� ���� ��Ŷ. 

		// �������
	TRACE_PACKET_CASE(CL_EXITSUBSCRIPT)
		ReceiveCL_EXITSUBSCRIPT(TotSize, lpData);		
		break;

		/////////////////////////// ���� ����� �޼���  ////////////////////////

		// ���� ����
	TRACE_PACKET_CASE(SV_STARTCARD)
		ReceiveSV_SD_STARTCARD(TotSize, lpData);		
		break;

		// ī�� ����
	TRACE_PACKET_CASE(SV_SELECTCARD)
		ReceiveSV_SELECTCARD(TotSize, lpData);		
		break;

		// ���� ��� ī�峯���� ����
	TRACE_PACKET_CASE(SV_WINDCARDEND) 
		ReceiveSV_WINDCARDEND(TotSize, lpData);		
		break;

		// ���� �����
	TRACE_PACKET_CASE(SV_RAISE)
		ReceiveSV_RAISE(TotSize, lpData);			
		break;

		// ������ ����
	TRACE_PACKET_CASE(SV_FOLDUSER)
		ReceiveSV_DIEUSER(TotSize, lpData);		 
		break;

		// ������ ���� ī��
	TRACE_PACKET_CASE(SV_HIDDENCARD)
		ReceiveSV_HIDDENCARD(TotSize, lpData);		 		
		break;

		// ���� ����
	TRACE_PACKET_CASE(SV_ENDGAME)
		ReceiveSV_ENDGAME(TotSize, lpData);			
		break;

		// ����
	TRACE_PACKET_CASE(SV_SPECIALCARD )
		ReceiveSV_SPECIALCARD(TotSize, lpData);						
		break;

		// �ڸ� ��Ŵ�� ��Ŷ ó��
	TRACE_PACKET_CASE(SV_SEATKEEPER)
		ReceiveSV_SEATKEEPER(TotSize, lpData);		
		break;

		// ���� ���� ��
	TRACE_PACKET_CASE(SV_CHANGE_MONEY)
		ReceiveSV_CHANGE_MONEY(TotSize, lpData);				
		break;

		// ��� ��û �㰡��
	TRACE_PACKET_CASE(SV_ACCEPT_OUTROOM) 
		ReceiveSV_ACCEPT_OUTROOM(TotSize, lpData);		
		break;

		// �� ������ �����
	TRACE_PACKET_CASE(SV_CHANGEMYINFO) 
		ReceiveSV_CHANGEMYINFO(TotSize, lpData);			
		break;

		// ���ο� �÷��̾ �Խ��Ͽ���
	TRACE_PACKET_CASE(SV_USERENTERROOM)  
		ReceiveSV_USERENTERROOM(TotSize, lpData);		
		break;

		// ����ڰ� �濡�� ��������
	TRACE_PACKET_CASE(SV_USEROUTROOM) 
		ReceiveSV_USEROUTROOM(TotSize, lpData);				 
		break;

		// ������ �ٲ�����
	TRACE_PACKET_CASE(SV_CHANGEROOMMASTER) 
		ReceiveSV_CHANGEROOMMASTER(TotSize, lpData);	
		break;

	TRACE_PACKET_CASE(SV_WHISPERCHAT)
		ReceiveSV_WhisperChat(TotSize, lpData);
		break;

	TRACE_PACKET_CASE(NM_ADMIN_WHISPERCHAT)	
		ReceiveSV_AdminWhisperChat(TotSize, lpData);
		break;


	TRACE_PACKET_CASE(SV_SD_NOTIFY_JJOGI)
		RecevieSV_SD_NOTIFY_JJOGI(TotSize, lpData);
		break;

	TRACE_PACKET_CASE(SV_SD_CHOICE)
		RevevieSV_SD_CHOICE(TotSize, lpData);
		break;

	TRACE_PACKET_CASE(SV_SD_ACCEPT_CHOICE)
		RevevieSV_SD_ACCEPT_CHOICE(TotSize, lpData);
		break;
		
	TRACE_PACKET_CASE(SV_SD_RESULTCARD)
		RevevieSV_SD_RESULTCARD(TotSize, lpData);
		break;
		

	TRACE_PACKET_CASE(SV_SD_49REMATCH)						// �������� 49 ���� ���� (��Ŷ ������ Ŭ���̾�Ʈ�� ���� ���� �����)
		RevevieSV_SD_49REMATCH(TotSize, lpData);
		break;		

	TRACE_PACKET_CASE(SV_SD_NOTIFY_REMATCH)				// ���� ���� (49 or ������)
		RevevieSV_SD_NOTIFY_REMATCH(TotSize, lpData);
		break;				

	TRACE_PACKET_CASE(SV_SD_NOTIFY_ASK)						// ���� ���� ����
		RevevieSV_SD_NOTIFY_ASK(TotSize, lpData);
		break;

	case OPCODE_SV_JP_INFO:
		ReceiveSV_JACKPOTMONEYINFO(TotSize, lpData);
		break;

	case SV_DAILY_LOSS_MONEY: // ������ ��Ŷ.
		ReceiveSV_LOSSMONEYINFO(TotSize, lpData);
		break ;

		//������ : ����ϱ� ��������
	case SV_ASK_WAIT_INROOM:
		ReceiveSV_ASK_WAIT_INROOM(TotSize, lpData);
		break ;

	case SV_RECEIVE_CONG_MSG:
		ReceiveSV_CongNoti(TotSize, lpData);
		break;

	case SV_ETCCHIP_PLAYER_INFO:
		ReceiveSV_ETCCHIP_PLAYER_INFO(TotSize, lpData);
		break;

	case SV_REWARD_ALLIN_ANGEL_GOLD:
		ReceiveSV_Reward_Allin_Angel_Gold(TotSize, lpData);
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
			
			g_pGameView->m_cGameViewBtnMananager.m_btnCardOpen.SetSprite(
				&(g_pGameView->m_cGameViewBtnMananager.m_sprUICardOpenBtn), nBtnNum);

			// �ΰ��� ä�ÿ� �˸�
			AddGameChatViewEdit(csMsg, RGB(199, 117, 116)) ;
		}
		break;
	default:
		return CGame::OnPacketNotify(pName, Signal, TotSize, lpData );
		break;
	}

	return TRUE;
}

//������ ���ӳ� ��� ����
void CGameSutda::ReceiveSV_LOSSMONEYINFO(int TotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	CSV_DAILY_LOSS_MONEY msg;
	msg.Get(lpData,TotSize);

	//�� ���ӿ��� �����ϵ��� GM�� ���� ����
	GM().SetLossmoneyInfo(*msg.llLossMoney, *msg.tBlockTime, *msg.eState, *msg.idxGame) ;

	g_pGameView->m_cGameViewBtnMananager.SetLossMoneyWarning() ;
}

void CGameSutda::ReceiveSV_JACKPOTMONEYINFO(int TotSize, char *lpData)
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


void CGameSutda::ReceiveSV_STARTBTNENABLE(int nTotSize, char *lpData)
{
	CSV_STARTBTNENABLE msg;
	msg.Get(lpData, nTotSize);			
	
	if(g_RI.State != EGAME_WAITING)
		return;

	if(g_RI.NowUserNum < LIMITGAMEPLAY_PLAYERNUM ) 
		return; 	
}


void CGameSutda::ReceiveSV_OBSERVER_MESSAGE(int nTotSize, char *lpData)
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

void CGameSutda::ReceiveSV_RESERVATIONINFO(int nTotSize, char *lpData)
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

	//����UI
	g_ObserverMan.OnReceiveReservationInfo(&msg);
}

// �������(�������� -> ����)
void CGameSutda::ReceiveSV_ACCEPT_EMERAGE(int nTotSize, char *lpData)
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
	
	// �÷��̾� ������ �°� �ʱ�ȭ�Ѵ�			
	g_GAMEPROCESS()->SetPlayInfo(nowpnum, newpnum, FALSE);

	// �ߺ��� IP�� �ִ��� üũ�Ѵ�.
	for(int i = 0; i < MAX_PLAYER; ++i)
	{
		int pn = g_GAMEPROCESS()->GetPNum_ByServPN(i);
		if(strlen(g_PLAYER(pn)->UI.ID) > 0)	
		{
			g_PLAYER(pn)->MyIpCheck = msg.IPA->IpIndex[i];
		}
	}
}

// �������(���� -> ��������)
void CGameSutda::ReceiveSV_ACCEPT_PARTICIPATION(int nTotSize, char *lpData)
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

	// ���� ���� ����
	g_PLAYER(nMyID)->nSndFxKind = *msg.SndFxKind;	

	g_pGameView->m_cGameViewBtnMananager.InitObserverBtn();
	// �÷��̾� ������ �°� �ʱ�ȭ�Ѵ�			
	g_GAMEPROCESS()->SetPlayInfo(nowpnum, newpnum, TRUE);

	// �ߺ��� IP�� �ִ��� üũ�Ѵ�.
	for(int i = 0; i < MAX_PLAYER; ++i) 
	{
		int pn = g_GAMEPROCESS()->GetPNum_ByServPN(i);
		if(strlen(g_PLAYER(pn)->UI.ID) > 0) 
		{
			g_PLAYER(pn)->MyIpCheck = msg.IPA->IpIndex[i];
		}
	}

	// �ڸ� ��Ŵ��
	g_SeatKeeper.OnEnterGame(); // ���� -> ������ �϶� ȣ�� 

	// �����ѵ�
	g_cOwnLimit.OnEnterRoom(&g_cUIData, g_GAMEPROCESS());
}


void CGameSutda::ReceiveSV_ENTER_EMERAGE(int nTotSize, char *lpData)
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
	str.Format("������ �� ����: [%s]��\n", g_PLAYER(newpnum)->UI.NickName);	
	AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

	// �ߺ��� IPüũ�Ѵ�.
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


void CGameSutda::ReceiveSV_ENTER_PARTICIPATION(int nTotSize, char *lpData)
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

	// ������ pnum �ε����� ������ ��ġ���� üũ�Ѵ�.
	if(strcmp( g_PLAYER(nowpnum)->UI.ID, msg.ID) != 0)
		return; 

	int pnum = -1; // �÷��̾� ��ȣ
	if( !g_ObserverMan.IsMyObserver()) // 1) ���� ���������̰� ������ �� ����
	{
		for(int i = g_GAMEPROCESS()->m_nNo1PosPNum; i < MAX_PLAYER + g_GAMEPROCESS()->m_nNo1PosPNum; ++i) 
		{	
			int index = i;
			
			if(index>=MAX_PLAYER)
			{
				index = index - MAX_PLAYER;
			}
			
			//��
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

		// ����ִ� ���� ã�� ��������
		if(pnum == -1)
		{
			return; 
		}

		g_pGameView->m_cPlayerDrawManager.Reset(pnum);
		g_PLAYER(nowpnum)->UI.cObserver = 0;		
		g_PLAYER(pnum)->SetNewPlayer(&g_PLAYER(nowpnum)->UI);
		g_PLAYER(pnum)->SetChangeRoomInfo(*g_PLAYER(nowpnum)->GetChangeRoomInfo());
		g_PLAYER(nowpnum)->Clear();
		g_pGameView->m_cPlayerDrawManager.SetShow(pnum, true);	// �� ���� �����ؾ� ������ ȭ�鿡 ���δ�.

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
		str.Format("������ �� ����: [%s]��\n", g_PLAYER(pnum)->UI.NickName);
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
	}	
	else	// 3)���� �������̰�  ������ �ϴ³� ����
	{
		for(int i = 0; i < MAX_PLAYER; ++i)	
		{
			if(strlen(g_PLAYER(i)->UI.ID)==0)
			{
				pnum=i;
				break;
			}
		}

		// ����ִ� ���� ã�� ��������
		if(pnum == -1)
		{
			return; 
		}

		g_GAMEPROCESS()->m_nMyPosPNum = 0; // �����ʿ� �ε���
		g_GAMEPROCESS()->m_nNo1PosPNum = g_GAMEPROCESS()->GetPNum_ByServPN(0); // �������� 0 �� �ε���

		// �� ������ �����ϰ� ���� ������ �÷��̾��� ������ ����
		g_pGameView->m_cPlayerDrawManager.Reset(pnum);
		g_PLAYER(nowpnum)->UI.cObserver = 0;		
		g_PLAYER(pnum)->SetNewPlayer(&g_PLAYER(nowpnum)->UI);
		g_PLAYER(pnum)->SetChangeRoomInfo(*g_PLAYER(nowpnum)->GetChangeRoomInfo());
		g_PLAYER(nowpnum)->Clear();
		g_pGameView->m_cPlayerDrawManager.SetShow(pnum, true);	// �� ���� �����ؾ� ������ ȭ�鿡 ���δ�.
		g_RI.NowUserNum += 1;
		g_RI.NowObserverNum -= 1;

		g_PLAYER(pnum)->ServPNum	   = g_GAMEPROCESS()->GetServPNum_ByPN(pnum);	// ������ �÷��̾� ��ȣ ����
		
		CString str;
		str.Format("������ �� ����: [%s]��\n", g_PLAYER(pnum)->UI.NickName);
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
	}

	// �ߺ��� IP�� üũ�Ѵ�.
	for(int i = 0; i < MAX_PLAYER; ++i)
	{
		int pn = g_GAMEPROCESS()->GetPNum_ByServPN(i);
		if(strlen(g_PLAYER(pn)->UI.ID) > 0)	
		{
			g_PLAYER(pn)->MyIpCheck = msg.IPA->IpIndex[i];
		}
	}

	// ���带 �߰��Ѵ�.
	g_PLAYER(pnum)->nSndFxKind = *msg.SndFxKind;

	CObserverDlg *pObserverDlg = GetObserverDlg();
	if(pObserverDlg)
	{
		pObserverDlg->User_DelItem(g_PLAYER(pnum)->UI.UNum, g_PLAYER(pnum)->UI.ID);
	}

}

// ������� �޾� ó���ϴ� �Լ�.
void CGameSutda::ReceiveSV_GAMEOVERRESULT(int nTotSize, char *lpData)
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
	

	g_cOwnLimit.OnGameOver();
	

	bProccessed = FALSE;
}


void CGameSutda::ReceiveCL_CHATDATA(int nTotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	CCL_CHATDATA msg;
	msg.Get(lpData, nTotSize);
	
	if(*msg.RoomNum != g_RI.RoomNum) 
		return;

	// ��ȭ ���� �ź����� ���̵��̸� ����
	int pnum = g_GAMEPROCESS()->GetPlayerPNum(msg.ID);
	BOOL bAdmin  = (strcmp("������",msg.ID)==0);


	if(g_cUIData.m_bObserverChat)
	{
	}
	else
	{
		//������ ä�� ���� 
		if(strncmp(msg.ID, GM().GetMyInfo()->UI.ID, 15)==0)
		{
			/*if(g_ObserverMan.IsMyObserver() && g_GAMEPROCESS()->m_bObserverChatWarning==FALSE)
			{
				AddGameChatViewEdit("[�˸�] ������ ä�ò��⸦ �����Ͻ� �в��� ����ä�� ������ �������� �ʽ��ϴ�.\n", GetChatColor(ECC2_NOTIFY));
				g_GAMEPROCESS()->m_bObserverChatWarning=TRUE;
			}*/
		}
		else
		{			
			//���� ä��  0 ���� 1 �Ⱥ���
			if(g_Config.bNoObserverChat  && !bAdmin )
			{					
				if(pnum>=MAX_PLAYER && pnum < GetMaxNewPlayer()-1)
				{					
					return;
				}					
			}
		}
	}

	if(pnum < 0  && !bAdmin)
	{
		return;
	}

	CString str;
	char chat[256]={0,};
	strncpy(chat, msg.StrChat, min(*msg.l_StrChat, 255));

	//(�߰�)
	// ������ ���� Ư�����ڸ� ����Ͽ��ٸ� ���鹮�ڷ� �ٲ�
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

	if( bAdmin )// ### [������ ��� �۾�] ###
	{				
		str.Format("������> %s\n",szTBuf);
		AddGameChatViewEdit(str.GetString(), GetChatColor(ECC2_SERVMSG));
	}
	else
	{	
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


void CGameSutda::ReceiveCL_EXITSUBSCRIPT(int nTotSize, char *lpData)
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
		{// ������
			g_pGameView->m_bExitSubscript = FALSE;
			ExitGameRequest();
		}
	}
	else if(bexit==1)
	{
		//���� �� ������ ���� ����
		if(strcmp(GM().GetMyInfo()->UI.ID, msg.ID) == 0)
		{
			g_pGameView->m_bExitSubscript = TRUE;
		}		
					
		str.Format("[%s]�� ������ ����\n", g_PLAYER(pnum)->UI.NickName);
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

		g_pGameView->m_cPlayerDrawManager.SetExitReservation(pnum, true);		
	}
	else if(bexit == 2)
	{
		//������ ��� 
		if(strcmp(GM().GetMyInfo()->UI.ID, msg.ID) == 0)
		{
			g_pGameView->m_bExitSubscript = FALSE;
		}				

		str.Format("[%s]�� ������ ���\n", g_PLAYER(pnum)->UI.NickName);
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

		g_pGameView->m_cPlayerDrawManager.SetExitReservation(pnum, false);
	}
}

void CGameSutda::ReceiveSV_SD_STARTCARD(int nTotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	CSV_SD_STARTCARD msg;
	msg.Get(lpData, nTotSize);

	int pnum = g_GAMEPROCESS()->GetPlayerPNum(msg.szID);
	if(pnum == -1) 
		return;

	if(strcmp(GM().GetMyInfo()->UI.ID, msg.szID) != 0)
		return;

	// ������ ���
	g_pGameView->m_cDisplayBoard.SetText(EDBT_GAMESTART);
	
	// ���̵��� �ʱ�ȭ
 	if(g_pGameView)
 	{
		g_pGameView->m_cSidePotMoney.Reset();
 	}



	static BOOL bProccessed = FALSE;
	if(bProccessed)
		return;
	bProccessed = TRUE;	

	// ���� ���� ����
	g_GAMEPROCESS()->SetRematchGame(*msg.bRematch);
	g_GAMEPROCESS()->ResetGame();
	g_GAMEPROCESS()->GameStart(msg.stSDStartInfo, *msg.bRematch);
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

void CGameSutda::ReceiveSV_SELECTCARD(int nTotSize, char *lpData)
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
		// ����ī�� ���ý� ���� Ÿ�̸� �����ش�.
		if(g_GAMEPROCESS()->m_bGameStart && pnum != 0 && g_RI.FormKind == CHOICE_GAME && g_PLAYER(pnum)->PlayState == EPLAYER_GAMEPLAYING)
		{
			// �߰� ���̽� ī��
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

// ī�� �й谡 �� �����ٴ� �޽����� ó�� �Ѵ�.
void CGameSutda::ReceiveSV_WINDCARDEND(int nTotSize, char *lpData)
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


void CGameSutda::ReceiveSV_RAISE(int nTotSize, char *lpData)
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

// ���������� �޾Ƽ� ó���ϴ� �Լ�
void CGameSutda::ReceiveSV_DIEUSER(int nTotSize, char *lpData)
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
	//sprintf(szTemp1, "CGameSutda::ReceiveSV_DIEUSER => �÷��̾�:%d\n", pnum);
	//OutputDebugString(szTemp1);
	//TTrace::Debug()->Send ("CGameSutda::ReceiveSV_DIEUSER", szTemp1);   // single byte string	
	//AddGameChatViewEdit(szTemp1, GetChatColor(ECC2_RESULT_JOKBO));

//#ifdef TRACE_OUTPUT
//	TTrace::Debug()->Send ("CGameSutda::ReceiveSV_DIEUSER", szTemp1);   // single byte string	
//#endif
 
	//g_pGameView->m_JackpotMan.m_JackPotData.nJackPotMoney = *msg.JackPotMoney;

	// ���� ������ ���ΰ�
	if(g_PLAYER(pnum)->PlayState == EPLAYER_GAMEPLAYING && g_PLAYER(pnum)->JoinState == EGAME_JOININ) 
	{ 
		g_PLAYER(pnum)->bFold	  = TRUE;
		g_PLAYER(pnum)->PlayState = EPLAYER_GAMEEND;
		
		if( !g_ObserverMan.IsMyObserver() && pnum == 0) 
		{ 
			GM().GetMyInfo()->bFold		= TRUE;
			GM().GetMyInfo()->PlayState = EPLAYER_GAMEEND;			
			
			g_SelectCard().EndSelectCard(FALSE);			
			g_pGameView->m_cPlayerDrawManager.SetSelectCard(pnum, false);
		}

		g_PLAYER(pnum)->m_cPlayerEffectList.Destroy();

		//���� ������ �� ������.
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

		//g_GAMEPROCESS()->SoundFxKind(1, g_PLAYER(pnum)->UI.Sex);// ### [���� �߰� �۾�] ### ����			
		SutdaGameSound().OnBetVoice(g_PLAYER(pnum)->UI.Sex, g_PLAYER(pnum)->GetSndFxKind(), RAISE_LAST_CMD_FOLD, FALSE);

		char szMsg[ MAX_PATH ] = { 0,};
		CString str;
		sprintf_s( szMsg, g_StrMan.Get(_T("FOLD_USER")), g_PLAYER(pnum)->UI.NickName );
		AddGameInfoViewEdit( szMsg, GetChatColor(ECC2_NOTIFY));

		//ī�� ���ý� �ð� �ʰ��� ���� ������ �̹����� ���� ����ش�.
		if( g_Poker.nState == RSTATE_PREPARESTART || g_Poker.nState == RSTATE_EXTRAFIRSTBET)
		{
			CBettingEffect *pObj = new CBettingEffect();
			pObj->Init(pnum, ANF_BET_CMD, ANF_BET_DIE);
			g_pGameView->m_cAniSprListManager.ADDObject(pObj);
		}
	}
}

// ����ī�带 �����.
void CGameSutda::ReceiveSV_HIDDENCARD(int nTotSize, char *lpData)
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

	ASSERT(pnum==0);
	if (pnum!=0)
		return;

	int nHiddenCardIndex = g_PLAYER(pnum)->m_nTotalCardCount-1;
	int nPrevCardIndex = nHiddenCardIndex -1;
	ASSERT(nHiddenCardIndex+1==(CARD_TOTALCOUNT));

	if(g_PLAYER(pnum)->PlayState == EPLAYER_GAMEPLAYING && g_PLAYER(pnum)->JoinState == EGAME_JOININ)
	{
		g_PLAYER(pnum)->m_cMyCard[nHiddenCardIndex].SetCardNum(*msg.nCard) ; // �� ����
		g_PLAYER(pnum)->m_cMyCard[nHiddenCardIndex].SetHiddenCard(TRUE);
		g_PLAYER(pnum)->m_cMyCard[nHiddenCardIndex].SetCardFront(FALSE);		
		
	}
	
	// �ڽ��� ī������ �������� ������ ����
	//g_pGameView->m_cPlayerDrawManager.MyJokboNameUpdate();
	//NMBASE::SOUND::g_Sound.PlayOggStream(SND_HIDDEN_BGM);

	g_PLAYER(0)->BackupOriginalCard();	//ī�� 3�� ���	
}

void CGameSutda::ReceiveSV_ENDGAME(int nTotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
	{
		return;
	}

	CSV_ENDGAME msg;
	msg.Get(lpData, nTotSize);

	if(g_pGameView)
	{
		// ���̵��� �ʱ�ȭ
		g_pGameView->m_cSidePotMoney.Reset();

		// Animan moment destroy
		g_pGameView->m_cAniSprListManager.Destroy(0);
		
		// ������ ������¸� ���� ����
		if(g_pGameView->m_bExitSubscript)
		{
			ExitGameRequest();
		}
		else
		{
			// ���� ���� ����
			GetGameProcess()->SetRematchGame(false);
			GetGameProcess()->ResetGame();			
		}
	}	
// 	if( !g_ObserverMan.IsMyObserver() && g_GAMEPROCESS()->GetPlayerNP(0)->m_nCardChoiceLimtCnt >= MAX_TURN_TIMEOUT_LIMIT_COUNT_FOR_SUTDA )
// 	{
// 		g_GAMEPROCESS()->GetPlayerNP(0)->m_nCardChoiceLimtCnt = 0;
// 		ExitGameRequest();
// 		CString strOutput;
// 		strOutput.Format("���� ���� �� %dȸ �̻� �������� ī�� ������ ���� �ʾ� ���ӹ濡�� �ڵ� ���� �Ͽ����ϴ�.\n", MAX_TURN_TIMEOUT_LIMIT_COUNT_FOR_SUTDA);
// 		ShowMainMessageDlg(strOutput);
// 	}

	if ( !g_ObserverMan.IsMyObserver() && g_PLAYER( 0 )->GetTurnLimitCnt() >= MAX_TURN_TIMEOUT_LIMIT_COUNT_FOR_SUTDA )
	{
		g_PLAYER( 0 )->ResetTurnLimitCnt();
		ExitGameRequest(true);
		CString strOutput;
		strOutput.Format("���� ���� �� %dȸ �̻� �������� ��ư Ŭ���� ���� �ʾ� ���ӹ濡�� �ڵ� ���� �Ͽ����ϴ�.\n", MAX_TURN_TIMEOUT_LIMIT_COUNT_FOR_SUTDA);
		ShowMainMessageDlg(strOutput);
	}



	g_cPromotionMan.OnEndGame();

	//��ũ�� ���α׷��� ������ ���� ��Ų��
	OnMacroCheck();
}

void CGameSutda::ReceiveSV_SPECIALCARD(int nTotSize, char *lpData)
{	
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	CSV_SPECIALCARD msg;
	msg.Get(lpData, nTotSize);		

	CString spec("");

	if(msg.pSC->nCard==CODE_SPECIAL_FCARD)
		spec = "����ī���";
	else if(msg.pSC->nCard==CODE_SPECIAL_SF)
		spec = "�ٽ�Ʈ����Ʈ �÷�����";
	else if(msg.pSC->nCard==CODE_SPECIAL_RSF)
		spec ="�ڷξ� ��Ʈ����Ʈ �÷�����";    


	CString str("");
	CString strNick = msg.pSC->NickName;
	//�����濡�� ���Դ��� üũ		

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

void CGameSutda::ReceiveSV_SEATKEEPER(int nTotSize, char *lpData)
{
	CSV_SEATKEEPER msg;
	msg.Get( lpData, nTotSize );
	g_SeatKeeper.ProcessPacket(&msg);
}

void CGameSutda::ReceiveSV_CHANGE_MONEY(int nTotSize, char *lpData)
{
	CSV_CHANGE_MONEY msg;
	msg.Get(lpData,nTotSize);
	g_GAMEPROCESS()->Change_Money(&msg);
}

void CGameSutda::ReceiveSV_ACCEPT_OUTROOM(int nTotSize, char *lpData)
{
	CSV_ACCEPT_OUTROOM msg;
	msg.Get(lpData, nTotSize);

	GM().SetWaitResponse(false);
	GM().OnExitRoom((REASON_OUTROOM)*msg.Reason);			
	
	g_GAMEPROCESS()->SetRematchGame(false);

	g_cPromotionMan.Accept_OutRoom();
} 


void CGameSutda::ReceiveSV_CHANGEMYINFO(int nTotSize, char *lpData)
{
	CSV_CHANGEMYINFO msg;
	msg.Get(lpData, nTotSize);

	memcpy( &GM().GetMyInfo()->UI.AllGameRecord, &msg.pCMI->AllGameRecord, sizeof(msg.pCMI->AllGameRecord)); 
	memcpy( &g_PLAYER(0)->UI.AllGameRecord, &msg.pCMI->AllGameRecord, sizeof(msg.pCMI->AllGameRecord));		
	GM().GetMyInfo()->UI.SetMoney( msg.pCMI->PMoney );
	//GM().GetMyInfo()->UI.PMoney = msg.pCMI->PMoney;
	if(GM().GetMyInfo()->UI.GetMoney() < 0)
	{
		GM().GetMyInfo()->UI.SetMoney( 0 );
	}

	g_PLAYER(0)->UI.SetMoney( GM().GetMyInfo()->UI.GetMoney() );
	g_PLAYER(0)->UI.nIcon  = GM().GetMyInfo()->UI.nIcon = GetMoneyGrade( GM().GetMyInfo()->UI.GetTotalMoney() );
}

void CGameSutda::ReceiveSV_USERENTERROOM(int nTotSize, char *lpData)
{
	CSV_USERENTERROOM msg;  
	msg.Get(lpData, nTotSize);

	g_GAMEPROCESS()->User_EnterRoom(&msg);
}

void CGameSutda::ReceiveSV_USEROUTROOM(int nTotSize, char *lpData)
{
	CSV_USEROUTROOM msg;
	msg.Get(lpData, nTotSize);

	g_GAMEPROCESS()->User_OutRoom(&msg);
}

void CGameSutda::ReceiveSV_CHANGEROOMMASTER(int nTotSize, char *lpData)
{
	CSV_CHANGEROOMMASTER msg;
	msg.Get(lpData, nTotSize);

	if(strlen(msg.ID)==0)
		return;

	BOOL bBreak = TRUE;

	// �г��Ӽ��� 	
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

	// �� ������ �����Ѵ�
	strcpy(g_RI.ID, msg.ID);
	// �г��Ӽ��� 	
	strcpy(g_RI.NickName , NickName );		
	//�������
	g_RI.UNum = *msg.UNum;			
	g_RI.RoomMasterType = *msg.RoomMasterType;

	if(strcmp(msg.ID, GM().GetMyInfo()->UI.ID) == 0) // ���� ������ �ȴ�
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
	str.Format("���庯��: [%s]��\n", NickName);
	AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

	str.Format("���庯��:[ID: %s(%s)]��\n",msg.ID , NickName );
	AddLobbyHideChatText( &str );

	g_cBanishMan.Clear();
}

//�ӼӸ�
BOOL CGameSutda::ReceiveSV_WhisperChat(int TotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) return FALSE;

	CSV_WHISPERCHAT msg;
	msg.Get(lpData, TotSize);

	//�Ӹ� ���� �ź��϶� ��� ����
	if(CCommunityManager::Instance()->GetOption(COM_ECOT_WHISPER) == COM_ECOPT_OFF) return FALSE;
	//������Ʈ�� ������ ���� �Ӹ��� ����
	if(CCommunityManager::Instance()->IsExistBlackBuddy(msg.m_szNickName)) return TRUE;
	//ģ�����Ը� �Ӹ��� �ޱ� �ɼ��� �� ģ���� �ƴѻ���� ���� �Ӹ��̸� ����
	if((CCommunityManager::Instance()->GetOption(COM_ECOT_WHISPER) == COM_ECOPT_MYBUDDY) && !CCommunityManager::Instance()->IsExistBuddy(msg.m_szNickName)) return TRUE;

	// ��ȭ ���� �ź����� ���̵��̸� ����
	int pnum = g_GAMEPROCESS()->GetPlayerPNum(msg.m_szID);

	CString str;
	CString chat;
	chat.Append((LPCTSTR)msg.m_StrChat, *msg.m_sStrChatLen);
	//(�߰�)
	// ������ ���� Ư�����ڸ� ����Ͽ��ٸ� ���鹮�ڷ� �ٲ�
	for(int s=0; s<chat.GetLength()-1; s++)
	{
		// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
		if(chat.GetAt(s) == 8 || chat.GetAt(s) == 9 || chat.GetAt(s) == 10 || chat.GetAt(s) == 13)
		{
			chat.SetAt(s, ' ');
		}
	}

	// �г��� �۾� 2006/11/08		
	char szTBuf[512] = {0, };
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

BOOL CGameSutda::ReceiveSV_AdminWhisperChat(int TotSize, char *lpData)
{
	// �����ڸ���۾�
	CNM_ADMIN_WHISPERCHAT msg;
	msg.Get(lpData, TotSize);

	CString str;
	CString chat;
	chat.Append((LPCTSTR)msg.StrChat, *msg.l_StrChat);

	//(�߰�)
	// ������ ���� Ư�����ڸ� ����Ͽ��ٸ� ���鹮�ڷ� �ٲ�
	for(int s=0; s<chat.GetLength()-1; s++) 
	{
		// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
		if(chat.GetAt(s) == 8 || chat.GetAt(s) == 9 || chat.GetAt(s) == 10 || chat.GetAt(s) == 13) 
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

// ������ �ٲ𶧸��� ȣ���� �Ǵ� �Լ�.
void CGameSutda::OnFocusGame(bool bFocus)
{
	if(bFocus)
	{
		// ä��
		if(g_pChatWinMan)
		{
			g_pChatWinMan->SetCurrnetChat();
		}
		
		// �ڸ���Ŵ��		
		g_cOwnLimit.Init( &g_cUIData, g_GAMEPROCESS());
		g_cBanishMan.Init(&g_SeatKeeper, &g_ObserverMan);
		g_SeatKeeper.SetChangeThisPoint();

		MM().AddMsgH(UPG_CMK_SET_LIMIT_MONEY,  Fnt(this, &CGameSutda::OnSetLimitMoney));
		MM().AddMsgH(UPG_CMK_SET_LIMIT_GOLD,  Fnt(this, &CGameSutda::OnSetLimitGold));
		MM().AddMsgH(UPG_CMK_GET_MONEY_GRADE,  Fnt(this, &CGameSutda::OnGetMoneyGrade));
		MM().AddMsgH(UPG_CMK_GET_JOKBO_NAME,   Fnt(this, &CGameSutda::OnGetJokboName));
		MM().AddMsgH(UPG_CMK_GET_MYJOKBO_NAME, Fnt(this, &CGameSutda::OnGetMyJokboName));
		MM().AddMsgH(UPG_CMK_GET_RESULT_MONEY, Fnt(this, &CGameSutda::OnGetResultMoney));
		MM().AddMsgH(UPG_CMK_CLEAR_JACKPOTMONEY, Fnt(this, &CGameSutda::OnClearJackPotMoney));	
		MM().AddMsgH(UPG_CMK_SET_PLAY_RAKEBACK_GOLD, Fnt(this, &CGameSutda::OnSetPlayRakeBackGold));
		MM().AddMsgH(UPG_CMK_SET_PLAY_RAKEBACK_GOLD_EVENT, Fnt(this, &CGameSutda::OnSetPlayRakeBackGoldEvent));			

		MM().AddMsgH(UPG_CMK_GAME_RESULT_OPEN, Fnt(this, &CGameSutda::OnGetResultOpen));	

		g_cPromotionMan.Reset();
		g_cPromotionMan.Init(&g_cUIData, this, &g_pGameView->m_cChatWindow);
		g_cPromotionMan.MakePromotion(CPromotionManager::EPT_JOKBOJACKPOT);		// ���������� ������ Ÿ�̹��� ���� �̸������ ���´�.
		
		//���� ���̽� Ÿ�� ����
		SutdaGameSound().SetMyVoiceType(GM().GetMyInfo()->UI.Sex, g_Config.SndFxKind);
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

LRESULT CGameSutda::OnSetPlayRakeBackGold(WPARAM &wParam, LPARAM &lParam)
{
	GetGameView()->m_cPlayerDrawManager.SetPlayRakeBackGold((INT64)wParam, (int)lParam);
	return TRUE;
}

LRESULT CGameSutda::OnSetPlayRakeBackGoldEvent(WPARAM &wParam, LPARAM &lParam)
{
	GetGameView()->m_cPlayerDrawManager.SetPlayRakeBackGoldEvent((int)wParam);
	return TRUE;
}

LRESULT CGameSutda::OnClearJackPotMoney(WPARAM &wParam, LPARAM &lParam)
{
	int  nGameCode     = *((int *)wParam);
	INT64 llLimitMoney = *((INT64 *)lParam);	

	g_GAMEPROCESS()->ClearJackpotMoney(nGameCode, llLimitMoney);

	return TRUE;
}


LRESULT CGameSutda::OnGetResultMoney(WPARAM &wParam, LPARAM &lParam)
{
	INT64 *pMoney = (INT64 *)lParam;
	CPlayerSutda *pPlayer = g_PLAYER((int)wParam);
	*pMoney = pPlayer->m_n64ResultMoney;
	return TRUE;
}

LRESULT CGameSutda::OnGetMyJokboName(WPARAM &wParam, LPARAM &lParam)
{
	CString *pStrJokbo = (CString *)lParam;	

	if( g_GAMEPROCESS() == NULL )
		return FALSE;


	//�������� �˾ƿ´�.
	if (g_ObserverMan.IsMyObserver())
		return FALSE;

	//�������� �˾ƿ´�.
	*pStrJokbo = g_PLAYER(0)->GetJokboName();	

	return TRUE;
}

LRESULT CGameSutda::OnGetJokboName(WPARAM &wParam, LPARAM &lParam)
{
	CString *pStrJokbo = (CString *)lParam;	

	if(NULL == g_GAMEPROCESS())
		return FALSE;

	int nPlayerNum = (int)wParam;

	if(nPlayerNum < 0 || nPlayerNum >= GetMaxPlayer())
		return FALSE;

	//�������� �˾ƿ´�.
	*pStrJokbo = g_PLAYER(nPlayerNum)->GetJokboName();	

	//*pStrJokbo = g_GAMEPROCESS()->GetSevenRuleManager().m_cRule[nPlayerNum].get_RuleValue_to_Name3();

	return TRUE;
}


LRESULT CGameSutda::OnGetMoneyGrade(WPARAM &wParam, LPARAM &lParam)
{
	return GetMoneyGrade(*((INT64 *)lParam));
}

LRESULT CGameSutda::OnSetLimitMoney(WPARAM &wParam, LPARAM &lParam)
{
	//�����ѵ�
	g_cOwnLimit.SetMaxPremiumMoneyWithOption(*((INT64 *)lParam));
	return TRUE;
}

LRESULT CGameSutda::OnSetLimitGold(WPARAM &wParam, LPARAM &lParam)
{
	//�����ѵ�
	g_cOwnLimit.SetMaxPremiumGoldWithOption(*((INT64 *)lParam));
	return TRUE;
}

CString CGameSutda::GetLobbyEnterSound(bool bMan)
{
	if(bMan) return ".\\sutda\\sound\\enterlobby_m.wav";
	return ".\\sutda\\sound\\enterlobby_f.wav";
}

LRESULT CGameSutda::OnGetResultOpen( WPARAM &wParam, LPARAM &lParam )
{

	if (g_pGameView==NULL)
		return FALSE;

	return g_pGameView->m_cPlayerDrawManager.IsOpen();
	//return TRUE;
}

const HICON CGameSutda::GetGameIcon(void)
{
	return AfxGetApp()->LoadIcon(IDI_ICON_SUTDA);
}

const HICON CGameSutda::GetChatIcon(void)
{
	return AfxGetApp()->LoadIcon(IDI_ICON_CHAT_SUTDA);
}


const CString	CGameSutda::GetModuleName(void)
{
	char szFileName[MAX_PATH+1]={0,};
	GetModuleFileName(g_hInstGameDLL, szFileName, MAX_PATH);	
	return CString(szFileName);

}




void CGameSutda::RecevieSV_SD_NOTIFY_JJOGI(int nTotSize, char *lpData)
{
	//������ ������ �Ϸ����� �� ó��
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	CSV_SD_NOTIFY_JJOGI msg;
	msg.Get(lpData, nTotSize);
	

	int pnum = g_GAMEPROCESS()->GetPlayerPNum(msg.szID);
	if(pnum == -1) 
		return;

	if(pnum == 0 && !g_ObserverMan.IsMyObserver())	//�� ������� ����
		return;
	
	g_PLAYER(pnum)->CompleteJJogi(FALSE);
	
}


void CGameSutda::RevevieSV_SD_CHOICE(int nTotSize, char* lpData)
{
	//�������� ���� ���� ��û(3�� ���ٿ����� ����)
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	CSV_SD_CHOICE msg;
	msg.Get(lpData, nTotSize);


	g_GAMEPROCESS()->OnAskChoiceCard(NULL);


}
void CGameSutda::RevevieSV_SD_ACCEPT_CHOICE(int nTotSize, char* lpData)
{
	// �������� ���� ���� ����
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	CSV_SD_ACCEPT_CHOICE msg;
	msg.Get(lpData, nTotSize);


	

}

void CGameSutda::RevevieSV_SD_RESULTCARD(int nTotSize, char* lpData)
{
	// �������� ���� ���� ����
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	CSV_SD_RESULTCARD msg;
	msg.Get(lpData, nTotSize);

	
	g_GAMEPROCESS()->OnResultCard(msg.ResultInfo);
}


void CGameSutda::RevevieSV_SD_NOTIFY_REMATCH(int nTotSize, char *lpData)
{
	// �������� ����(49 or ������) ���� (��Ŷ ������ Ŭ���̾�Ʈ�� ���� ���� �����)
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	CSV_SD_NOTIFY_REMATCH msg;
	msg.Get(lpData, nTotSize);


	if (*msg.nState == RSTATE_49)
	{
		g_GAMEPROCESS()->OnRematch_by_49(*msg.bWinner, *msg.bReEntry);
	}
	else if (*msg.nState == RSTATE_DRAW)
	{
		g_GAMEPROCESS()->OnRematch_by_Draw(*msg.bWinner);
	}

}


void CGameSutda::RevevieSV_SD_49REMATCH(int nTotSize, char *lpData)
{
	// ���� ���� ����
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	CSV_SD_49REMATCH msg;
	msg.Get(lpData, nTotSize);


	g_pGameView->m_cAskRematchPopup.Show(*msg.nEntryFee);

}

void CGameSutda::RevevieSV_SD_NOTIFY_ASK(int nTotSize, char *lpData)
{

	// ���� ���� ����
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	CSV_SD_NOTIFY_ASK msg;
	msg.Get(lpData, nTotSize);

	
	g_GAMEPROCESS()->OnJoinRematchGame(msg.szID);
	

}

void CGameSutda::SetBGMVolume(int nVolume)
{

	g_pGameView->m_Radiophonograph.SetRadioVolume(nVolume);

	CGame::SetBGMVolume(nVolume);
}

//### [������ ��� �۾�] ###  ������ ����
LONG CGameSutda::OnReceiveChangeStyleRoomInfo(int TotSize, char *lpData)
{
	// ���� ��ġ�� �׹��ΰ�, ���� �ΰ�
	CSV_CHANGESTYLEROOMINFO msg;
	msg.Get(lpData, TotSize);
	
	if(msg.SRI->nMaxUserNum < 100)
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



//������ : ����ϱ� ��� �߰�
void CGameSutda::ReceiveSV_ASK_WAIT_INROOM(int TotSize, char *lpData)
{	
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	CSV_WAIT_INROOM msg;
	msg.Get(lpData, TotSize);
	g_pGameView->m_cGameViewBtnMananager.SetWaitFlag(*msg.m_bWait);
}

void CGameSutda::ReceiveSV_CongNoti(int TotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	CSV_RECEIVE_CONG_MSG msg;
	msg.Get(lpData, TotSize);

	CString strID;
	strID.Format("���� ��� : %s��", msg.m_Send_ID);
	CString strChat;
	strChat.Format("%s", msg.StrChat);
	g_pGameView->m_cPlayerDrawManager.SetPlayCongNoti(strID.GetString(), strChat.GetString(), *msg.m_ShowTime);
}

void CGameSutda::ReceiveSV_ETCCHIP_PLAYER_INFO(int TotSize, char *lpData)
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
			// ������ ����Ʈ ���
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
			g_PLAYER(g_GAMEPROCESS()->GetPNum_ByServPN(pnum))->SetGold(*msg.m_RealChip);
		}
	}
}

void CGameSutda::ReceiveSV_Reward_Allin_Angel_Gold(int TotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	CSV_REWARD_ALLIN_ANGEL_GOLD msg;
	msg.Get(lpData, TotSize);
	g_pGameView->m_cPlayerDrawManager.SetPlayGoldReward((*msg.m_RewardMoney), timeGetTime());
}

void CGameSutda::ReceiveSV_PM_ODDEVEN_WINNER_NOTI(int TotSize, char *lpData)
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