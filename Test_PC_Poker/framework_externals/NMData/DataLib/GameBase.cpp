#include "StdAfx.h"
#include "CGameBase.h"

#include <imm.h>
#pragma comment(lib,"imm32.lib")

CGameBase::CGameBase(void)
{
	m_pGameViewBase=NULL;
	m_pGameProcess=NULL;
	m_bLoadSprite = false;
	m_bLoadSound = false;
	m_nSkinKind = -1;
}

CGameBase::~CGameBase(void)
{
	if(m_pGameProcess) delete m_pGameProcess;
	if(m_pGameViewBase) delete m_pGameViewBase;

	m_pGameProcess = NULL;
	m_pGameViewBase = NULL;

	Release();
}


CGameProcess* CGameBase::GetGameProcess()
{
	return m_pGameProcess;

}
CGameViewBase* CGameBase::GetGameView()
{
	return m_pGameViewBase;
}

BOOL CGameBase::Create(CWnd *pParentWnd)
{
	return FALSE;
}

void CGameBase::OnEnterRoom(BOOL bCreation)
{
	ASSERT(m_pGameProcess != NULL);
	if(m_pGameProcess)
		m_pGameProcess->OnEnterRoom(bCreation);
}

void CGameBase::OnExitRoom()
{
	ASSERT(m_pGameProcess != NULL);

	if(m_pGameProcess)
		m_pGameProcess->OnExitRoom();	
}

LONG CGameBase::OnPacketNotify(int Sid, int Signal, int TotSize, char *lpData)
{
	switch(Signal)
	{		
	case SV_RES_GAME_CONTINUE:		return OnReceiveResGameContinue(TotSize, lpData);
	}


	return FALSE;
}

void CGameBase::Release()
{
	ReleaseSprite();
	ReleaseSound();
}

void CGameBase::LoadSprite()
{
	ASSERT(m_bLoadSprite == false);
	m_bLoadSprite = true;
}

bool CGameBase::ReLoadSprite(int nSkinKind, bool bFirstLoad/* = false*/)
{
	if(m_nSkinKind == nSkinKind) return false;
	m_nSkinKind = nSkinKind;
	return true;
}

void CGameBase::LoadSound()
{
	ASSERT(m_bLoadSound == false);
	m_bLoadSound = true;
	Sound.ClearGameLoadList();
}

void CGameBase::ReleaseSprite()
{
	if(m_bLoadSprite == false) return;
	m_bLoadSprite = false;
}

void CGameBase::ReleaseSound()
{
	if(m_bLoadSound == false) return;
	m_bLoadSound = false;
	Sound.ReleaseGameLoadList();
}

int CGameBase::GetMaxNewPlayer(void)
{
	return GetMaxGamePlayer() + GetMaxObserver();
}

const int CGameBase::GetWidth(void)
{
	ASSERT(m_pGameViewBase != NULL);

	if(m_pGameViewBase)
		return m_pGameViewBase->GetWidth();

	return -1;
}

const int CGameBase::GetHeight(void)
{
	ASSERT(m_pGameViewBase != NULL);
	if(m_pGameViewBase)
		return m_pGameViewBase->GetHeight();

	return -1;
}

void CGameBase::ShowGameWindow(bool bShow)
{
	UINT nOption = SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW;
	nOption |= bShow?SWP_SHOWWINDOW:SWP_HIDEWINDOW;
	m_pGameViewBase->SetWindowPos(NULL, 0, 0, 0, 0, nOption);	
}



LONG CGameBase::OnReceiveResGameContinue(int TotSize, char *lpData)
{
	CSV_RES_GAME_CONTINUE msg;
	msg.Get( lpData, TotSize );

	INT64 llSetMoney = *msg.Money;
	INT64 llInMoney = *msg.m_llInMoney;
	INT64 llSafeMoney = *msg.m_llSafeMoney;

	if(llSetMoney < 0 || llInMoney < 0 || llSafeMoney < 0 ) return FALSE;



	if(strcmp( GM().GetMyInfo()->UI.ID, msg.ID) == 0) 	//나일 경우
	{		

		GM().GetMyInfo()->UI.SetMoney(llSetMoney);
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
				pPlayer->UI.SetInMoney( llInMoney );
				pPlayer->UI.SetSafeMoney( llSafeMoney );

				pPlayer->UI.nIcon = GetMoneyGrade(pPlayer->UI.GetTotalMoney());
			}
		}
	}



	return TRUE;
} 