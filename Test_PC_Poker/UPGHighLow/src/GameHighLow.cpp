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
	TTrace::Debug()->Send ("7Poker����-Create", "CGameHighLow");   // single byte string	
#endif

	// ���� ���� View�� �����Ѵ�.
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

// ����� ���̾�α�
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

		// ������ : ī�弼���߰�.
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

// �ٷ����� �Լ� ( Ŭ�� -> ���� )
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

// �������� ���̾�α�
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

	// ���� ������ �۾�
	// �ٷ� ������ �κ񿡼� ȣ���ϸ�, ������ �������� ����.
	GM().AskDirectEnterRoom(false, NULL, GetDefaultFormKind());

	// ���� �� ����ϴ� �κ��� �ϴ� ������.
	//SCONDITION sInfo; 
	//CString strSection;
	//strSection.Format("QJ%02d_%s_%d", QJ_VER, GM().GetMyInfo()->UI.ID, EQJ_7POKER);

	//if(CQuickJoinDlg::GetSearchCondition(EQJ_7POKER, strSection, &sInfo))
	//{
	//	//������� �ٷ�����
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
	// ### [ ������� ] ###
	g_pGameView->m_cGameViewBtnMananager.SetChangeDisplayButton(bFullScreen);
	g_pGameView->Page.ReCreateDIBSection();
}

void CGameHighLow::OnCloseOptionDlg(LRESULT nRet)
{
	if(nRet == IDOK) g_pGameView->m_cGameViewBtnMananager.SetBetBtnType(g_Config.eBetBtnType);
}

void CGameHighLow::OnLoadingEnd(void)
{
	// ��������Ʈ �ε� �� �ʱ�ȭ ~
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
	return "���̷ο�";
}
const COLORREF	CGameHighLow::GetGameColor(void)
{	
	return RGB(148,57,62);//�� ���� ������ ���� ��ȯ
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


// ��Ʈ�� �޽����� �޾� ó���ϴ� �Լ�(CClientDlg::OnPacketNotify ���� ���ӿ� ���õ� ��Ŷ�� ����� �̵�)
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
		// ���ӽ��۹�ư Ȱ��ȭ
	case SV_STARTBTNENABLE:
		ReceiveSV_STARTBTNENABLE(TotSize, lpData);		
		break;

		// �������
	case SV_OBSERVER_MESSAGE:
		ReceiveSV_OBSERVER_MESSAGE(TotSize, lpData);
		break;

		// �������(������ ����)
	case SV_RESERVATIONINFO:
		ReceiveSV_RESERVATIONINFO(TotSize, lpData);		
		break;
		
		// �������(�������� -> ����)
	case SV_ACCEPT_EMERAGE:
		ReceiveSV_ACCEPT_EMERAGE(TotSize, lpData);
		break;

		// �������(���� -> ��������)
	case SV_ACCEPT_PARTICIPATION:
		ReceiveSV_ACCEPT_PARTICIPATION(TotSize, lpData);		
		break;

		// �������(�������� -> ����)
	case SV_ENTER_EMERAGE:
		ReceiveSV_ENTER_EMERAGE(TotSize, lpData);
		break;

		// �������(���� -> ������)
	case SV_ENTER_PARTICIPATION:
		ReceiveSV_ENTER_PARTICIPATION(TotSize, lpData);
		break;

		// ������ ����Ǿ���
	case SV_GAMEOVERRESULT: 
		ReceiveSV_GAMEOVERRESULT(TotSize, lpData);			 
		break;

		/////////////////////////// Ŭ���̾�Ʈ�� �޼���  ////////////////////////

		// ���ӹ� ä�õ���Ÿ
	case CL_CHATDATA:
		ReceiveCL_CHATDATA(TotSize, lpData);		 
		break;		

	case SV_CHANGESTYLEROOMINFO:		  
		{	
			return OnReceiveChangeStyleRoomInfo( TotSize, lpData ); 
		} break; //BTS ����͸� �� ���� ���� ��Ŷ. 

		// �������
	case CL_EXITSUBSCRIPT:
		ReceiveCL_EXITSUBSCRIPT(TotSize, lpData);		
		break;

		/////////////////////////// ���� ����� �޼���  ////////////////////////

		// ���� ����
	case SV_STARTCARD:
		ReceiveSV_STARTCARD(TotSize, lpData);		
		break;

		// ī�� ����
	case SV_SELECTCARD:
		ReceiveSV_SELECTCARD(TotSize, lpData);		
		break;

		// ���� ��� ī�峯���� ����
	case SV_WINDCARDEND: 
		ReceiveSV_WINDCARDEND(TotSize, lpData);		
		break;

		// ���� �����
	case SV_RAISE:
		ReceiveSV_RAISE(TotSize, lpData);			
		break;

		// ������ ����
	case SV_FOLDUSER:
		ReceiveSV_DIEUSER(TotSize, lpData);		 
		break;

		// ������ ���� ī��
	case SV_HIDDENCARD:
		ReceiveSV_HIDDENCARD(TotSize, lpData);		 		
		break;

		// ���� ����
	case SV_ENDGAME:
		ReceiveSV_ENDGAME(TotSize, lpData);			
		break;

		// ����
	case SV_SPECIALCARD :
		ReceiveSV_SPECIALCARD(TotSize, lpData);						
		break;

		// �ڸ� ��Ŵ�� ��Ŷ ó��
	case SV_SEATKEEPER:
		ReceiveSV_SEATKEEPER(TotSize, lpData);		
		break;

		// ���� ���� ��
	case SV_CHANGE_MONEY:
		ReceiveSV_CHANGE_MONEY(TotSize, lpData);				
		break;

		// ��� ��û �㰡��
	case SV_ACCEPT_OUTROOM: 
		ReceiveSV_ACCEPT_OUTROOM(TotSize, lpData);		
		break;

		// �� ������ �����
	case SV_CHANGEMYINFO: 
		ReceiveSV_CHANGEMYINFO(TotSize, lpData);			
		break;

		// ���ο� �÷��̾ �Խ��Ͽ���
	case SV_USERENTERROOM:  
		ReceiveSV_USERENTERROOM(TotSize, lpData);		
		break;

		// ����ڰ� �濡�� ��������
	case SV_USEROUTROOM: 
		ReceiveSV_USEROUTROOM(TotSize, lpData);				 
		break;

		// ������ �ٲ�����
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

		//������ : �սǱݾ׿� ���� ���â ����
	case SV_DAILY_LOSS_MONEY: // ������ ��Ŷ.
		ReceiveSV_LOSSMONEYINFO(TotSize, lpData);
		break ;

	case SV_REWARD_ALLIN_ANGEL:
		ReceiveSV_Reward_Allin_Angel(TotSize, lpData);
		break;

	//-----------------------(���̷ο�) (�߰��ڵ�)-----------------------//	

	case SV_HL_ENDRAISE :	// ��� ������ �� => ���� �ο� ���� ���� ��û	
		ReceiveSV_HL_EndRaise(TotSize, lpData);
		break;

	case SV_HL_HLS_SEL :	// ����, �ο�, ���� ��ư Ŭ�������� �˸�
		ReceiveSV_HL_HighLowSelect(TotSize, lpData);
		break;

		//������ : ����ϱ� ��������
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

void CGameHighLow::ReceiveSV_Reward_Allin_Angel(int TotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	CSV_REWARD_ALLIN_ANGEL msg;
	msg.Get(lpData, TotSize);
	g_pGameView->m_cPlayerDrawManager.SetPlayAllinAngel((*msg.m_RemainEnableCount), timeGetTime());
}

//������ : �սǱݾ׿� ���� ���â ����
void CGameHighLow::ReceiveSV_LOSSMONEYINFO(int TotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	CSV_DAILY_LOSS_MONEY msg;
	msg.Get(lpData,TotSize);

	//�� ���ӿ��� �����ϵ��� GM�� ���� ����
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

	//����UI
	g_ObserverMan.OnReceiveReservationInfo(&msg);
}

// �������(�������� -> ����)
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

	// ��ȭ ���� �ź����� ���̵��̸� ����
	int pnum = g_GAMEPROCESS()->GetPlayerPNum(msg.ID);
	BOOL bAdmin = strcmp("������",msg.ID)==0;


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

// ���������� �޾Ƽ� ó���ϴ� �Լ�
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
	//sprintf(szTemp1, "CGameHighLow::ReceiveSV_DIEUSER => �÷��̾�:%d\n", pnum);
	//OutputDebugString(szTemp1);
	//TTrace::Debug()->Send ("CGameHighLow::ReceiveSV_DIEUSER", szTemp1);   // single byte string	
	//AddGameChatViewEdit(szTemp1, GetChatColor(ECC2_RESULT_JOKBO));

//#ifdef TRACE_OUTPUT
//	TTrace::Debug()->Send ("CGameHighLow::ReceiveSV_DIEUSER", szTemp1);   // single byte string	
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
			
			g_SelectCard().EndSelectCard(TRUE);			
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

		g_GAMEPROCESS()->SoundFxKind(1, g_PLAYER(pnum)->UI.Sex);// ### [���� �߰� �۾�] ### ����			

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
		g_PLAYER(pnum)->m_cMyCard[6].SetCardNum(*msg.nCard) ; // �� ����
		g_PLAYER(pnum)->m_cMyCard[6].SetHiddenCard(TRUE);
	}
	
	// �ڽ��� ī������ �������� ������ ����
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
		// ���̵��� �ʱ�ȭ
		g_pGameView->m_cSidePotMoney.Reset();

		// Animan moment destroy
		g_pGameView->m_cAniSprListManager.Destroy(0);

		//yoo [20150120] 
		if(TRUE==CGame::OnReceiveResEndGame())
		{
			ExitGameRequest();
		}
		if(g_pGameView->m_bExitSubscript) // ������ ������¸� ���� ����
		{
			ExitGameRequest();
		}
		else
		{
			// ���� ���� ����
			GetGameProcess()->ResetGame();
		}
	}
	
	if( !g_ObserverMan.IsMyObserver() && g_GAMEPROCESS()->GetPlayerNP(0)->m_nCardChoiceLimtCnt >= MAX_TURN_TIMEOUT_LIMIT_COUNT )
	{
		g_GAMEPROCESS()->GetPlayerNP(0)->m_nCardChoiceLimtCnt = 0;
		ExitGameRequest(true);
		CString strOutput;
		strOutput.Format("���� ���� �� %dȸ �̻� �������� ī�� ������ ���� �ʾ� ���ӹ濡�� �ڵ� ���� �Ͽ����ϴ�.\n", MAX_TURN_TIMEOUT_LIMIT_COUNT);
		ShowMainMessageDlg(strOutput);
	}
	g_cPromotionMan.OnEndGame();

	//��ũ�� ���α׷��� ������ ���� ��Ų��
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
BOOL CGameHighLow::ReceiveSV_WhisperChat(int TotSize, char *lpData)
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

BOOL CGameHighLow::ReceiveSV_AdminWhisperChat(int TotSize, char *lpData)
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
void CGameHighLow::OnFocusGame(bool bFocus)
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
	//���� ���� ���� ��ȯ
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
	//���� �ο� ���� ��ȯ
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
	//�����ѵ�
	g_cOwnLimit.SetMaxPremiumMoneyWithOption(*((INT64 *)lParam));
	return TRUE;
}

LRESULT CGameHighLow::OnSetLimitGold(WPARAM &wParam, LPARAM &lParam)
{
	//�����ѵ�
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

//### [������ ��� �۾�] ###  ������ ����
LONG CGameHighLow::OnReceiveChangeStyleRoomInfo(int TotSize, char *lpData)
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
	strID.Format("���� ��� : %s��", msg.m_Send_ID);
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