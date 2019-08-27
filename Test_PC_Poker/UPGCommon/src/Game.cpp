#include "StdAfx.h"
#include "Game.h"

#include <imm.h>
#pragma comment(lib,"imm32.lib")

CGame::CGame(void)
{
	m_pGameViewBase=NULL;
	m_pGameProcess=NULL;
	m_bLoadSprite = false;
	m_bLoadSound = false;
	m_nSkinKind = -1;
}

CGame::~CGame(void)
{
	if(m_pGameProcess) delete m_pGameProcess;
	if(m_pGameViewBase) delete m_pGameViewBase;

	m_pGameProcess = NULL;
	m_pGameViewBase = NULL;

	Release();
}


CGameProcess* CGame::GetGameProcess()
{
	return m_pGameProcess;

}
CGameViewBase* CGame::GetGameView()
{
	return m_pGameViewBase;
}

BOOL CGame::Create(CWnd *pParentWnd)
{
	return FALSE;
}

void CGame::OnEnterRoom(BOOL bCreation)
{
	ASSERT(m_pGameProcess != NULL);
	if(m_pGameProcess)
		m_pGameProcess->OnEnterRoom(bCreation);
}

void CGame::OnExitRoom()
{
	ASSERT(m_pGameProcess != NULL);

	if(m_pGameProcess)
		m_pGameProcess->OnExitRoom();	
}

LONG CGame::OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData)
{
	switch(Signal)
	{		
	case SV_RES_GAME_CONTINUE:		return OnReceiveResGameContinue(TotSize, lpData);
	}


	return FALSE;
}

void CGame::Release()
{
	ReleaseSprite();
	ReleaseSound();
}

void CGame::LoadSprite()
{
	ASSERT(m_bLoadSprite == false);
	m_bLoadSprite = true;
}

bool CGame::ReLoadSprite(int nSkinKind, bool bFirstLoad/* = false*/)
{
	if(m_nSkinKind == nSkinKind) return false;
	m_nSkinKind = nSkinKind;
	return true;
}

void CGame::LoadSound()
{
	ASSERT(m_bLoadSound == false);
	m_bLoadSound = true;
	NMBASE::SOUND::g_Sound.ClearGameLoadList();
}

void CGame::ReleaseSprite()
{
	if(m_bLoadSprite == false) return;
	m_bLoadSprite = false;
}

void CGame::ReleaseSound()
{
	if(m_bLoadSound == false) return;
	m_bLoadSound = false;
	NMBASE::SOUND::g_Sound.ReleaseGameLoadList();
}

int CGame::GetMaxNewPlayer(void)
{
	return GetMaxGamePlayer() + GetMaxObserver();
}

const int CGame::GetWidth(void)
{
	ASSERT(m_pGameViewBase != NULL);

	if(m_pGameViewBase)
		return m_pGameViewBase->GetWidth();

	return -1;
}

const int CGame::GetHeight(void)
{
	ASSERT(m_pGameViewBase != NULL);
	if(m_pGameViewBase)
		return m_pGameViewBase->GetHeight();

	return -1;
}

void CGame::ShowGameWindow(bool bShow)
{
	UINT nOption = SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW;
	nOption |= bShow?SWP_SHOWWINDOW:SWP_HIDEWINDOW;
	m_pGameViewBase->SetWindowPos(NULL, 0, 0, 0, 0, nOption);	
}



LONG CGame::OnReceiveResGameContinue(int TotSize, char *lpData)
{
	CSV_RES_GAME_CONTINUE msg;
	msg.Get( lpData, TotSize );

	INT64 llSetMoney = *msg.llMoney;
	INT64 llSetRealPMoney = *msg.llRealPMoney;
	INT64 llInMoney = *msg.m_llInMoney;
	INT64 llSafeMoney = *msg.m_llSafeMoney;

	if(llSetMoney < 0 || llSetRealPMoney < 0 || llInMoney < 0 || llSafeMoney < 0 ) return FALSE;

	if(strcmp( GM().GetMyInfo()->UI.ID, msg.ID) == 0) 	//나일 경우
	{
		GM().GetMyInfo()->UI.SetMoney(llSetMoney);
		GM().GetMyInfo()->UI.SetRealPMoney(llSetRealPMoney);
		GM().GetMyInfo()->UI.SetInMoney( llInMoney );
		GM().GetMyInfo()->UI.SetSafeMoney( llSafeMoney );
		
		GM().GetMyInfo()->UI.nIcon = GetMoneyGrade(GM().GetMyInfo()->UI.GetTotalMoney());
		
		GM().GetMyInfo()->UI.nAIN_Insurance_Money = *msg.GradeInsuMoney;// 등급보험[올인보험개편]
		GM().GetMyInfo()->UI.nAIN_InsuranceKind = *msg.GradeInsuKind;
	}
	
	CGameProcess *pGameProcess = GetGameProcess();
	if (pGameProcess)
	{
		int pnum = pGameProcess->GetPlayerPNum(msg.ID);
		if (pnum >= 0 )
		{

			CPlayer *pPlayer = pGameProcess->GetPlayer(pnum);
			if (pPlayer)
			{
				pPlayer->UI.SetMoney( llSetMoney );
				pPlayer->UI.SetRealPMoney( llSetRealPMoney );
				pPlayer->UI.SetInMoney( llInMoney );
				pPlayer->UI.SetSafeMoney( llSafeMoney );

				pPlayer->UI.nIcon = GetMoneyGrade(pPlayer->UI.GetTotalMoney());
			}
		}
	}

	return TRUE;
} 

BOOL CGame::OnReceiveResEndGame(void)
{

	return FALSE;
}