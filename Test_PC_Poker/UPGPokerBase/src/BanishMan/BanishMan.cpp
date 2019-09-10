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

	//������ �ƴϸ� ����
	if(strcmp ( g_RI.ID , GM().GetMyInfo()->UI.ID ) != 0 )
		return;

	set_BanishmentCount(*msg.BanishCount);
}

void CBanishManager::ReceiveSV_ASK_BANISHVOTE(int nTotSize, char *lpData)
{
	CSV_ASK_BANISHVOTE msg;
	msg.Get(lpData, nTotSize);

	// �г��Ӽ��� 	
	CString NickName;	
	CString str;	
	static BOOL bProccessed = FALSE;

	if( m_pObserverMan->IsMyObserver() )
		return;

	// ��ǥ ��û�� Ȥ�� ����ڸ� �޽����� ���
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
			// �г��Ӽ��� 	
			NickName = pPlayer->UI.NickName;
			break;
		}
	}

	CMyVoteDlg dlgMyVote;

	dlgMyVote.m_NickName = NickName;
	dlgMyVote.m_BanishReson = msg.pBV->Reson;

	//UPGCommon �۾� (���̾�α� ���� ������ �ۿ��� ó��)
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
			ShowMainMessageDlg("�ڱ� �ڽ��� ���� �߹��ų �� �����ϴ�.");
			return FALSE;
		}

		int nPNum = pProcess->GetPlayerPNum(pTargetID);
		if(nPNum == -1)
		{
			ShowMainMessageDlg("���ӹ濡 �������� �ʴ� �����Դϴ�.");
			return FALSE;
		}

		//�ڸ���Ŵ�� �߰�
		char ErrMsg[1024]="���� �߹��� �� ���� �����Դϴ�.";
		if(IsImmuneToBanish(nPNum, ErrMsg))
		{
			ShowMainMessageDlg(ErrMsg);			
			return FALSE;
		}


		/*if(pPlayer->UI.GetMoney(IsTournament()) <=0)*/
		if(pPlayer->UI.GetMoney() <=0)
		{
			ShowMainMessageDlg( "������ ���� ���� �� �������� ����� ��� �ߴ� �˴ϴ�.");	
			return FALSE;
		}	

		int targetpnum = pProcess->GetPlayerPNum(pTargetID);
		if(targetpnum<0)
			return FALSE;

		if(targetpnum < MAX_PLAYER && GetBanishmentCount() == 0 )
		{
			ShowMainMessageDlg("���� �߹� ����� ���̻� ��� �� �� �����ϴ�.");
			return FALSE;
		}
	}
	else
	{
		// ������ �ƴϸ�
		ShowMainMessageDlg("������ �ƴϹǷ� ���� �߹��ų �� �����ϴ�.");		
		return FALSE;
	}


	// ���� �߹� ��û
	CSV_ASK_FORCEBANISH fbmsg;
	fbmsg.Set(pTargetID);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), fbmsg.pData, fbmsg.GetTotalSize());		

	return TRUE;
}

BOOL CBanishManager::IsImmuneToBanish(int nPnum_Local, char *ErrMsg)	//�߹��� ������ �ʴ� ���� üũ 
{	
	CGameProcess *pProcess = GM().GetCurrentProcess();
	int nPnum_Server= pProcess->GetServPNum_ByPN(nPnum_Local);

	//�ڸ� ��Ŵ�� ��������� üũ�Ѵ�. 
	if(nPnum_Server == m_pSeatKeepr->GetSeatKeepUser()) //�ڸ���Ŵ�� ����ڴ� �߹����
	{
		if(ErrMsg)
			strcpy(ErrMsg, "�ڸ���Ŵ�� ���񽺸� ������� ��� �߹���ǥ �Ǵ� �����߹��� �Ұ����մϴ�.");
		return TRUE;
	}

	//�߹��� �� ���� ������ ����� �Ʒ��� �� �߰��Ѵ�..

	return FALSE;
}