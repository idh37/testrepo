#include "stdafx.h"
#include "PlayerDraw.h"
#include "PlayerDrawManager.h"
#include "../UIInfoData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CPlayerDrawManager::CPlayerDrawManager(int nMaxPlayer)
{
	m_nMaxPlayer	= nMaxPlayer;
	m_pGameProcess	= NULL;
	m_pGameView		= NULL;
	m_pUIInfoData	= NULL;
	m_bFlyWindCard	= false;
	m_dwTimerStartTime		= 0;
	m_dwTimerCurTime		= 0;
	memset( m_listClockFlag, FALSE, sizeof( BOOL ) * 16 );
	m_nWinnerPNum			= -1;
	m_dwGameOverStartTime	= 0;
	m_bGameOver				= false;
	m_bOpen					= FALSE;
	m_bHiddenCardShow		= false;
	m_pTurnSpr				= NULL;

	m_listPlayer.resize(m_nMaxPlayer);
	m_listResultJokbo.resize(m_nMaxPlayer);
	m_listResultMoney.resize(m_nMaxPlayer);
}

CPlayerDrawManager::~CPlayerDrawManager()
{
	if(m_listPlayer.size())
	{
		std::vector<CPlayerDraw *>::iterator start = m_listPlayer.begin();
		std::vector<CPlayerDraw *>::iterator end = m_listPlayer.end();

		for(;start != end;start++)
		{
			delete *start;
		}

		m_listPlayer.clear();
	}
}

void CPlayerDrawManager::LoadImage(LPCTSTR lpPathTimer, 
								   NMBASE::GRAPHICGDI::xSprite *pBetCmdSpr, 
								   NMBASE::GRAPHICGDI::xSprite *pWinLoseBackSpr, 
								   LPCTSTR lpAllinPath, 
								   NMBASE::GRAPHICGDI::xSprite *pTurnSpr, 
								   LPCTSTR szUserBGPath, 
								   LPCTSTR szLevelPath)
{
	AddLoadImageList(&m_sprChangeGameInfoBack,".\\PokerCommon\\data\\ChangeMoneyBack.spr", true);
	AddLoadImageList(&m_sprMyMoney,".\\PokerCommon\\data\\me_money.spr",true);
	AddLoadImageList(&m_sprMyGold,".\\PokerCommon\\data\\me_gold.spr",true);
	AddLoadImageList(&m_sprUserMoney,".\\PokerCommon\\data\\usermoney.spr",true);
	AddLoadImageList(&m_sprUserGold,".\\PokerCommon\\data\\userGold.spr",true);
	AddLoadImageList(&m_sprWarning,".\\PokerCommon\\data\\Warning.spr",true);
	AddLoadImageList(&m_sprHostBossDealer,".\\PokerCommon\\data\\HostBossDealer.spr", true);
	AddLoadImageList(&m_sprReservation,".\\PokerCommon\\data\\com_reser.spr", true);
	AddLoadImageList(&m_sprHaveMoneyToolTip,".\\PokerCommon\\data\\HaveMoneyToolTip.spr", true);

	if(lpPathTimer)	AddLoadImageList(&m_sprTimer,lpPathTimer, true);
	else AddLoadImageList(&m_sprTimer,".\\PokerCommon\\data\\timer.spr", true);

	if(pTurnSpr)
	{
		m_pTurnSpr = pTurnSpr;
	}
	else
	{
		m_pTurnSpr = &m_sprTurn;
		AddLoadImageList(&m_sprTurn,".\\PokerCommon\\data\\turn.spr", true);
	}

	if (szUserBGPath)
		AddLoadImageList(&m_sprUserBack,szUserBGPath,true);
	else
		AddLoadImageList(&m_sprUserBack,".\\PokerCommon\\data\\bg.spr",true);


	if (szLevelPath)
		AddLoadImageList(&m_sprLevel,szLevelPath,true);
	else
		AddLoadImageList(&m_sprLevel,".\\PokerCommon\\data\\Level.spr",true);

	AddLoadImageList(&m_sprAllin,lpAllinPath, true);
	
	AddLoadImageList(&m_sprLimitIcon, ".\\PokerCommon\\data\\LimitIcon.spr", true);
	AddLoadImageList(&m_sprLimitOverIcon, ".\\PokerCommon\\data\\LimitOverIcon.spr", true);
	
	m_pWinLoseBackSpr = pWinLoseBackSpr;	

	AddLoadImageList(&m_sprAllinAngel, ".\\PokerCommon\\data\\AllinAngel2017.spr", true);
	m_cEffect_AllinAngel.SetSprite(&m_sprAllinAngel);

	// 2017.11 골드 아이콘
	AddLoadImageList(&m_sprGold, ".\\PokerCommon\\data\\gold.spr", true);

	// 2017.12 골드 올인천사
	AddLoadImageList(&m_sprGoldReward, ".\\PokerCommon\\data\\evt_goldreward.spr", true);
	m_cEffect_GoldReward.SetSprite(&m_sprGoldReward);

	// 2018.01 추가 골드 애니메이션
	AddLoadImageList(&m_sprGoldPlus, ".\\PokerCommon\\data\\goldplus.spr", true);
	m_cEffect_GoldPlus.SetSprite(&m_sprGoldPlus);
	
	// 2018.01 추가 골드 애니메이션
	AddLoadImageList(&m_sprGoldMileage, ".\\PokerCommon\\data\\goldmileage.spr", true);

	// 타겟 알림?
	AddLoadImageList(&m_sprTargetNoti, ".\\Common\\data\\dialog\\targetnoti.spr", true);

	// 2018.03 골드출석부 알람.
	AddLoadImageList(&m_sprGoldNoti, ".\\PokerCommon\\data\\goldtarget_noti.spr", true);

	// 2018.06 골드 보험 가입 알람.
	AddLoadImageList(&m_sprGoldInsuNoti, ".\\PokerCommon\\data\\GoldInsurance_noti.spr", true);
	m_cEffect_GoldInsuNoti.SetSprite(&m_sprGoldInsuNoti);
}

void CPlayerDrawManager::Init(CUIInfoData *pUIInfoData, CGame *pGame)
{
	m_pGameProcess	= pGame->GetGameProcess();
	m_pGameView		= pGame->GetGameView();
	m_pUIInfoData	= pUIInfoData;
}

void CPlayerDrawManager::AddPlayer(CPlayer *pPlayer, CPlayerDraw *pDraw, int nNum)
{
	ASSERT(pPlayer != NULL && pDraw != NULL);
	m_listPlayer[nNum] = pDraw;
	pDraw->Init(pPlayer, nNum, &m_pGameView->Page, this);
}

void CPlayerDrawManager::SetShow(int nPNum, bool bShow)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	m_listPlayer[nPNum]->SetShow(bShow);
}

bool CPlayerDrawManager::IsShow(int nPNum)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	return m_listPlayer[nPNum]->IsShow();
}

void CPlayerDrawManager::SetLock(int nPNum, bool bLock)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	m_listPlayer[nPNum]->SetLock(bLock);
}

bool CPlayerDrawManager::IsLock(int nPNum)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	return m_listPlayer[nPNum]->IsLock();
}

void CPlayerDrawManager::SetBettingMoney(int nPNum, int nBet, INT64 i64Money, bool bRaise)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);

	m_listPlayer[nPNum]->SetBettingMoney(nBet, i64Money, bRaise);
}

void CPlayerDrawManager::ClearBettingMoney(int nPNum)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	m_listPlayer[nPNum]->ClearBettingMoney();
}

void CPlayerDrawManager::SetTurn(int nPNum)
{
	for(int i=0 ; i<m_nMaxPlayer;i++)
	{
		if(nPNum == i) m_listPlayer[i]->SetTurn(true);
		else m_listPlayer[i]->SetTurn(false);
	}
}

void CPlayerDrawManager::InitUIData(void)
{
	m_listPlayer[0]->InitUIData();
}

void CPlayerDrawManager::OnEnterRoomLimitMoney(int nPNum, INT64 lossmoney)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	CHANGE_USERINFO_ROOM temp = *(m_listPlayer[nPNum]->GetChangeRoomInfo());
	temp.llMaxLossMoney = lossmoney;
	m_listPlayer[nPNum]->SetChangeRoomInfo(temp);
}

void CPlayerDrawManager::OnEnterRoom(int nPNum)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	m_listPlayer[nPNum]->OnEnterRoom();
}

void CPlayerDrawManager::OnExitRoom(int nPNum)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	m_listPlayer[nPNum]->OnExitRoom();
}

void CPlayerDrawManager::OnEnterRoom(void)
{
	for(int i=0;i<m_nMaxPlayer;i++)
	{
		m_listPlayer[i]->OnEnterRoom();
	}
}

void CPlayerDrawManager::OnExitRoom(void)
{
	for(int i=0;i<m_nMaxPlayer;i++)
	{
		m_listPlayer[i]->OnExitRoom();
	}
}

void CPlayerDrawManager::SetPlayerGameOver(int nPNum, bool bGameOver)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	m_listPlayer[nPNum]->SetGameOver(bGameOver);
}

bool CPlayerDrawManager::IsPlayerGameOver(int nPNum)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	return m_listPlayer[nPNum]->IsGameOver();
}

void CPlayerDrawManager::SetExitReservation(int nPNum, bool bExit)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	m_listPlayer[nPNum]->SetExitReservation(bExit);
}

bool CPlayerDrawManager::IsExitReservation(int nPNum)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	return m_listPlayer[nPNum]->IsExitReservation();
}

void CPlayerDrawManager::SetLimitMoney( int nPNum, bool bPlay )
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	m_listPlayer[nPNum]->SetLimitMoney(bPlay);
}

void CPlayerDrawManager::SetObserverReservation(int nPNum, bool bReservation)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	m_listPlayer[nPNum]->SetObserverReservation(bReservation);
}

bool CPlayerDrawManager::IsObserverReservation(int nPNum)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	return m_listPlayer[nPNum]->IsObserverReservation();
}

void CPlayerDrawManager::SetPlayAllinAni(int nPNum, DWORD dwStartTime)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	m_listPlayer[nPNum]->SetPlayAllinAni(dwStartTime);
}

void CPlayerDrawManager::SetPlayAllinAngel(int remainCount, DWORD dwStartTime)
{
	m_listPlayer[0]->SetPlayAllinAngel(remainCount, dwStartTime);
}

void CPlayerDrawManager::SetPlayCongNoti(std::string id, std::string data, int playTime)
{
	m_listPlayer[0]->SetPlayCongNoti(id, data, playTime);
}

void CPlayerDrawManager::SetPlayGoldReward(INT64 rewardGold, DWORD dwStartTime)
{
	m_listPlayer[0]->SetPlayGoldReward(rewardGold, dwStartTime);
}

void CPlayerDrawManager::SetPlayRakeBackGold(INT64 chipInt, int chipPrime)
{
	m_listPlayer[0]->SetPlayRakeBackGold(chipInt, chipPrime);
}

void CPlayerDrawManager::SetPlayRakeBackGoldEvent(float eventRate)
{
	m_listPlayer[0]->SetPlayRakeBackGoldEvent(eventRate);
}

void CPlayerDrawManager::SetPlayGoldNoti(std::string id, INT64 rewardGold, DWORD dwStartTime)
{
	m_listPlayer[0]->SetPlayGoldNoti(id, rewardGold, dwStartTime);
}

void CPlayerDrawManager::SetDrawMyValue(int nPNum, bool bDraw)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	m_listPlayer[nPNum]->SetDrawMyValue(bDraw);

	if(bDraw)
	{
		CString strJokbo="";
		CPlayer *pPlayer = GM().GetCurrentProcess()->GetPlayer(nPNum);
		if(pPlayer && pPlayer->JoinState == 1 && pPlayer->UI.ID[0] != 0 && !pPlayer->bFold && m_bOpen)
		{
			strJokbo = "";
			MM().Call(UPG_CMK_GET_JOKBO_NAME, (WPARAM)nPNum, (LPARAM)&strJokbo);
			int nServPNum = GM().GetCurrentProcess()->GetServPNum_ByPN(nPNum);
			m_listResultJokbo[nServPNum] = strJokbo;
		}
	}
}

void CPlayerDrawManager::CheckResultValue(void)
{
	CString strJokbo="";
	INT64 i64ResultMoney = 0;
	
	for(int i=0;i<m_nMaxPlayer;i++)
	{
		int nPNum = GM().GetCurrentProcess()->GetPNum_ByServPN(i);
		CPlayer *pPlayer = GM().GetCurrentProcess()->GetPlayer(nPNum);

		if( pPlayer && pPlayer->JoinState == 1 && pPlayer->UI.ID[0] != 0 )
		{
			if( !pPlayer->bFold && m_bOpen )
			{
				strJokbo = "";
				MM().Call(UPG_CMK_GET_JOKBO_NAME, (WPARAM)nPNum, (LPARAM)&strJokbo);
				m_listResultJokbo[i] = strJokbo;
			}
			
			// 기권상태지만 기권오픈을 했다면 족보 정보를 넣어준다.
			if( pPlayer->bFoldOpen )
			{
				strJokbo = "";
				MM().Call(UPG_CMK_GET_JOKBO_NAME, (WPARAM)nPNum, (LPARAM)&strJokbo);
				m_listResultJokbo[i] = strJokbo;
			}
			
			i64ResultMoney = 0;
			MM().Call(UPG_CMK_GET_RESULT_MONEY, (WPARAM)nPNum, (LPARAM)&i64ResultMoney);
			m_listResultMoney[i] = i64ResultMoney;
		}
	}
}

void CPlayerDrawManager::DrawBottom(CDC *pDC)
{
	for(int i=0;i<m_nMaxPlayer;i++)
	{
		m_listPlayer[i]->DrawBottom(pDC);
	}
}

void CPlayerDrawManager::DrawMiddle(CDC *pDC)
{
	for(int i=0;i<m_nMaxPlayer;i++)
	{
		m_listPlayer[i]->DrawMiddle(pDC);
	}
}

void CPlayerDrawManager::DrawTop(CDC *pDC)
{
	for(int i=0;i<m_nMaxPlayer;i++)
	{
		m_listPlayer[i]->DrawTop(pDC);
	}
}

void CPlayerDrawManager::DrawTopMost(CDC *pDC)
{
	for(int i=0;i<m_nMaxPlayer;i++)
	{
		m_listPlayer[i]->DrawTopMost(pDC);
	}
}

BOOL CPlayerDrawManager::OnLButtonDown(int x, int y)
{
	for(int i=0;i<m_nMaxPlayer;i++)
	{
		if(m_listPlayer[i]->OnLButtonDown(x, y)) return TRUE;
	}
	return FALSE;
}

BOOL CPlayerDrawManager::OnLButtonUp(int x, int y)
{
	for (int i = 0; i < m_nMaxPlayer; i++)
	{
		if (m_listPlayer[i]->OnLButtonUp(x, y))
			return TRUE;
	}

	return FALSE;
}

BOOL CPlayerDrawManager::OnMouseMove(int x, int y)
{
	for (int i = 0; i < m_nMaxPlayer; i++)
	{
		if (m_listPlayer[i]->OnMouseMove(x, y))
			return TRUE;
	}
	
	return FALSE;
}

void CPlayerDrawManager::SetStartTimer()
{
	m_dwTimerStartTime = timeGetTime();
	m_dwTimerCurTime = m_dwTimerStartTime;
	memset( m_listClockFlag, FALSE, sizeof( BOOL ) * 16 );
}

void CPlayerDrawManager::Reset(void)
{
	for(int i=0;i<m_nMaxPlayer;i++)
	{
		m_listPlayer[i]->Reset();
		m_listResultJokbo[i] = "";
		m_listResultMoney[i] = 0;
	}

	m_bFlyWindCard = false;
	memset( m_listClockFlag, FALSE, sizeof( BOOL ) * 16 );
	m_dwTimerStartTime = 0;
	m_dwTimerCurTime = 0;
	m_nWinnerPNum = -1;
	m_bHiddenCardShow = false;
	m_bGameOver = false;
	m_bOpen = false;
	m_dwGameOverStartTime = 0;
}

void CPlayerDrawManager::Reset(int nPNum)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	m_listPlayer[nPNum]->Reset();
}

void CPlayerDrawManager::SetGameOver(bool bGameOver, DWORD dwStartTime /*=0*/)
{
	m_bGameOver = bGameOver;
	m_dwGameOverStartTime = dwStartTime;
}


void CPlayerDrawManager::SetSelectCard(int nPNum, bool bSelectCard)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	m_listPlayer[nPNum]->SetSelectCard(bSelectCard);
}

bool CPlayerDrawManager::IsSelectCard(int nPNum)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	return m_listPlayer[nPNum]->IsSelectCard();
}


void CPlayerDrawManager::MyJokboNameUpdate()
{
	// 카드 날아가는것이 끝나면 족보이름을 한번씩 업데이트 해준다.
	if(m_listPlayer.size() > 0)
	{
		 m_listPlayer[0]->UpdateJokboName();
	}
}
