#include "stdafx.h"
#include "BanishMan.h"
#include "../seatkeeper/SeatKeeper.h"
#include "../ObserverMan/ObserverMan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CBanishManager::CBanishManager()
:	m_nBanismentcount(0),
	m_pSeatKeepr(NULL),
	m_pObserverMan(NULL)
{
	MM().AddMsgH(UPG_CMK_KICK_OUT, Fnt(this, &CBanishManager::OnKickOut));
}

CBanishManager::~CBanishManager()
{
}

int CBanishManager::GetBanishmentCount()
{
	return m_nBanismentcount;
}

void CBanishManager::set_BanishmentCount(int cnt )
{		
	m_nBanismentcount = cnt;
}

void CBanishManager::Clear(){
	m_nBanismentcount = GetMaxBnishCount();		
}

void CBanishManager::Init(CSeatKeeper *pSeatKeeper, CObserverMan *pObserverMan)
{
	m_pSeatKeepr = pSeatKeeper;
	m_pObserverMan = pObserverMan;
}

int CBanishManager::GetMaxBnishCount()
{
	if (g_RI.RoomMasterType)
	{
		return MAX_KICKOUT_COUNT_SUPER;			
	}
	else
	{
		return MAX_KICKOUT_COUNT_NORMAL;
	}
}

void CBanishManager::ReceiveSV_NOTIFY_BANISHCOUNT(int nTotSize, char *lpData)
{
	CSV_NOTIFY_BANISHCOUNT msg;
	msg.Get(lpData, nTotSize);

	//방장이 아니면 무시
	if(strcmp ( g_RI.ID , GM().GetMyInfo()->UI.ID ) != 0 )
		return;

	set_BanishmentCount(*msg.BanishCount);
}

void CBanishManager::ReceiveSV_ASK_BANISHVOTE(int nTotSize, char *lpData)
{
	CSV_ASK_BANISHVOTE msg;
	msg.Get(lpData, nTotSize);

	// 닉네임수정 	
	CString NickName;	
	CString str;	
	static BOOL bProccessed = FALSE;

	if( m_pObserverMan->IsMyObserver() )
		return;

	// 투표 요청자 혹은 대상자면 메시지만 출력
	CPlayer *pMyInfo = GM().GetMyInfo();
	if(strcmp(pMyInfo->UI.ID, msg.pBV->MyID)==0 || strcmp(pMyInfo->UI.ID, msg.pBV->TarID)==0)
		return;

	if(bProccessed != FALSE) 
		return;

	bProccessed = TRUE;

	CGameProcess *pProcess = GM().GetCurrentProcess();
	CPlayer *pPlayer = NULL;

	for(int i = 1; i < MAX_PLAYER; ++i) 
	{
		pPlayer = pProcess->GetPlayer(i);
		if(strcmp(pPlayer->UI.ID, msg.pBV->TarID)==0)
		{
			// 닉네임수정 	
			NickName = pPlayer->UI.NickName;
			break;
		}
	}

	CMyVoteDlg dlgMyVote;

	dlgMyVote.m_NickName = NickName;
	dlgMyVote.m_BanishReson = msg.pBV->Reson;

	//UPGCommon 작업 (다이얼로그 안의 로직을 밖에서 처리)
	CSV_MYBANISHVOTE mvmsg;
	mvmsg.Set(dlgMyVote.DoModal()==IDOK);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), mvmsg.pData, mvmsg.GetTotalSize());

	bProccessed = FALSE;
}

LONG CBanishManager::OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return FALSE;

	switch(Signal)
	{
	case SV_NOTIFY_BANISHCOUNT:
		ReceiveSV_NOTIFY_BANISHCOUNT(TotSize, lpData);		
		return TRUE;
	case SV_ASK_BANISHVOTE: 
		ReceiveSV_ASK_BANISHVOTE(TotSize, lpData);		 
		break;
	}

	return FALSE;
}

LRESULT CBanishManager::OnKickOut(WPARAM &wParam, LPARAM &lParam)
{
	const char *pTargetID = (const char *)lParam;
	if(pTargetID==NULL)
		return FALSE;

	CGameProcess *pProcess = GM().GetCurrentProcess();
	CPlayer *pMyInfo = GM().GetMyInfo();
	CPlayer *pPlayer = pProcess->GetPlayer(0);

	if(strcmp(pMyInfo->UI.ID, g_RI.ID) == 0) 
	{
		if( strcmp(pMyInfo->UI.ID,pTargetID) == 0)
		{
			ShowMainMessageDlg("자기 자신은 강제 추방시킬 수 없습니다.");
			return FALSE;
		}

		int nPNum = pProcess->GetPlayerPNum(pTargetID);
		if(nPNum == -1)
		{
			ShowMainMessageDlg("게임방에 존재하지 않는 유저입니다.");
			return FALSE;
		}

		//자리지킴이 추가
		char ErrMsg[1024]="강제 추방할 수 없는 유저입니다.";
		if(IsImmuneToBanish(nPNum, ErrMsg))
		{
			ShowMainMessageDlg(ErrMsg);			
			return FALSE;
		}


		/*if(pPlayer->UI.GetMoney(IsTournament()) <=0)*/
		if(pPlayer->UI.GetMoney() <=0)
		{
			ShowMainMessageDlg( "방장이 올인 베팅 시 강제퇴장 기능이 잠시 중단 됩니다.");	
			return FALSE;
		}	

		int targetpnum = pProcess->GetPlayerPNum(pTargetID);
		if(targetpnum<0)
			return FALSE;

		if(targetpnum < MAX_PLAYER && GetBanishmentCount() == 0 )
		{
			ShowMainMessageDlg("강제 추방 기능을 더이상 사용 할 수 없습니다.");
			return FALSE;
		}
	}
	else
	{
		// 방장이 아니면
		ShowMainMessageDlg("방장이 아니므로 강제 추방시킬 수 없습니다.");		
		return FALSE;
	}


	// 강제 추방 요청
	CSV_ASK_FORCEBANISH fbmsg;
	fbmsg.Set(pTargetID);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), fbmsg.pData, fbmsg.GetTotalSize());		

	return TRUE;
}

BOOL CBanishManager::IsImmuneToBanish(int nPnum_Local, char *ErrMsg)	//추방을 당하지 않는 유저 체크 
{	
	CGameProcess *pProcess = GM().GetCurrentProcess();
	int nPnum_Server= pProcess->GetServPNum_ByPN(nPnum_Local);

	//자리 지킴이 사용자인지 체크한다. 
	if(nPnum_Server == m_pSeatKeepr->GetSeatKeepUser()) //자리지킴이 사용자는 추방못함
	{
		if(ErrMsg)
			strcpy(ErrMsg, "자리지킴이 서비스를 사용중인 경우 추방투표 또는 강제추방이 불가능합니다.");
		return TRUE;
	}

	//추방할 수 없는 유저가 생기면 아래에 더 추가한다..

	return FALSE;
}