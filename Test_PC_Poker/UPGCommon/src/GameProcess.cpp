#include "StdAfx.h"
#include "Game.h"
#include "GameProcess.h"
#include "GlobalBase.h"

CGameProcess::CGameProcess(CGame *pGame)
:	m_pGame(pGame)
{
	m_pPage = NULL;
	m_nNo1PosPNum=m_nMyPosPNum=0;
	m_nGameState=0;
	m_nGameCount=0;
	m_nStartUserNum=0;
	m_nCurPlayer = -1;
	m_i64Double = 0;
	m_i64JackPotActive = 0;


	//규제안 관련하여 예약기능 추가 2014.01
	ReserveState(RESERVESTATE_BET_NONE);

	g_nCardTotalCount = 0;

	ReserveState(RESERVESTATE_BET_NONE);

}

CGameProcess::~CGameProcess(void)
{
}

void CGameProcess::Init(NMBASE::GRAPHICGDI::CPage *ppage) 
{
	m_pPage = ppage;
}

int CGameProcess::GetPNum_ByServPN(int onum) // 서버인덱스 ->  클라이언트 인덱스
{
	int pnum = -1;
	int nMaxPlayer = m_pGame->GetMaxPlayer();

	pnum = (abs(onum-m_nMyPosPNum+nMaxPlayer))%nMaxPlayer;
	return pnum;
}

int CGameProcess::GetServPNum_ByPN(int onum) // 클라이언트 -> 서버인덱스 나를 0으로
{
	int spnum = -1;	
	int nMaxPlayer = m_pGame->GetMaxPlayer();
	spnum = (m_nMyPosPNum+onum)%nMaxPlayer;
	return spnum;
}

int CGameProcess::GetPlayerPNum(const char *id)
{
	int pnum = -1;
	if(id == NULL || strlen(id)==0) return pnum;
	int nMaxNewPlayer = m_pGame->GetMaxNewPlayer();
	CPlayer *pPlayer = NULL;
	for(int i=0; i<nMaxNewPlayer; i++) {
		pPlayer = GetPlayer(i);
		if(strlen(pPlayer->UI.ID) > 0 && strcmp(pPlayer->UI.ID, id) == 0)
		{
			pnum=i; 
			break;
		}
	}
	return pnum;
}

int CGameProcess::GetStartUserNum()
{
	return m_nStartUserNum;
}

CString CGameProcess::GetPlayerNickName(int pnum)
{
	CString strNick;
	int nMaxNewPlayer = m_pGame->GetMaxNewPlayer();
	if (pnum<0 || pnum>=nMaxNewPlayer) return strNick;
	strNick = GetPlayer(pnum)->UI.NickName;
	return strNick;
}

int CGameProcess::GetPlayerPNum_ByNick(const char *nick)
{
	int pnum = -1;
	if(strlen(nick)==0) return pnum;
	int nMaxNewPlayer = m_pGame->GetMaxNewPlayer();
	for(int i=0; i<nMaxNewPlayer; i++) {
		if(strlen(GetPlayer(i)->UI.NickName) > 0)
			if(strcmp(GetPlayer(i)->UI.NickName, nick) == 0) {pnum=i; break;}
	}
	return pnum;
}

CString CGameProcess::GetPlayerNickName(const char *id)
{
	CString strNick = _T("");
	if(strlen(id)==0) return _T("");
	int nMaxNewPlayer = m_pGame->GetMaxNewPlayer();
	for(int i=0; i<nMaxNewPlayer; i++) {
		if(strlen(GetPlayer(i)->UI.ID) > 0)
			if(strcmp(GetPlayer(i)->UI.ID, id) == 0) {
				strNick = GetPlayer(i)->UI.NickName;
				break;
			}
	}	
	return strNick;
}

INT64 CGameProcess::GetRealBet()
{
	INT64 nRealBet = 0;
	const int nMaxPlayer = m_pGame->GetMaxPlayer();
	for(int i=0;i<nMaxPlayer;i++)
		if(g_Poker.RU[i].bInUse) nRealBet+= g_Poker.RU[i].nRealBat;

	return nRealBet;
}


INT64 CGameProcess::GetTotalBet()
{
	INT64 nRealBet = 0;
	const int nMaxPlayer = m_pGame->GetMaxPlayer();
	for(int i=0;i<nMaxPlayer;i++)
		if(g_Poker.RU[i].bInUse) nRealBet+= g_Poker.RU[i].nTotBat;

	return nRealBet;
}

INT64 CGameProcess::GetRaiseBat()
{
	INT64 nRaiseBat = 0;
	const int nMaxPlayer = m_pGame->GetMaxPlayer();
	for(int i=0;i<nMaxPlayer;i++)
		nRaiseBat = max(nRaiseBat,g_Poker.RU[i].nNowBat);

	return nRaiseBat;
}
//half 금액
INT64 CGameProcess::GetMaxMoney(INT64 nTotal)
{
	double fBatRate = 0.5f;	

	int spnum = GetServPNum_ByPN(0);
	INT64 callnMoney = GetRaiseBat() - g_Poker.RU[spnum].nNowBat;
	 
	INT64 TotalMoney = GetTotalBet() + callnMoney;
	INT64 nCalcBat  = 0;
	nCalcBat = (INT64)(TotalMoney*fBatRate);
	nCalcBat += callnMoney;	
	return nCalcBat;
}
//full 금액
INT64 CGameProcess::GetMaxMoneyForFull(INT64 nTotal)
{

	int spnum = GetServPNum_ByPN(0);
	INT64 callnMoney = GetRaiseBat() - g_Poker.RU[spnum].nNowBat;

	INT64 TotalMoney = GetTotalBet() + callnMoney;
	INT64 nCalcBat  = 0;
	nCalcBat = (INT64)(TotalMoney);
	nCalcBat += callnMoney;	
	return nCalcBat;
}

INT64 CGameProcess::GetLimitBetMoney( int nLimitX )
{
// 	int spnum = GetServPNum_ByPN(0);
// 	INT64 callnMoney = GetRaiseBat() - g_Poker.RU[spnum].nNowBat;
// 
// 	INT64 TotalMoney = GetTotalBet() + callnMoney;
	INT64 nCalcBat  = 0;

	nCalcBat = (INT64)( g_RI.CurSeedMoney * nLimitX );
	//nCalcBat += callnMoney;	
	return nCalcBat;
}

INT64 CGameProcess::GetLimitRaiseMoney( int nLimitX )
{
	int spnum = GetServPNum_ByPN(0);
	INT64 callnMoney = GetRaiseBat() - g_Poker.RU[spnum].nNowBat;

	INT64 TotalMoney = GetTotalBet() + callnMoney;
	INT64 nCalcBat  = 0;

	nCalcBat = (INT64)( g_RI.CurSeedMoney * nLimitX );
	nCalcBat += callnMoney;	
	return nCalcBat;
}


void CGameProcess::OnEnterRoom(BOOL bCreation)
{
	//bCreation : 내가 방생성해서 들어온지 여부
	// 한줄 추가
	VipService().OnEnterGame();

	//UPGCommon로 이동
	HIMC Mode = ImmGetContext( GM().GetMainWnd()->GetSafeHwnd() );
	ImmSetConversionStatus( Mode, IME_CMODE_HANGEUL, IME_CMODE_HANGEUL ); 

	//UPGCommon로 이동
//	GoldenChipMan().OnEnterRoom();  // 골든칩
}

void CGameProcess::OnExitRoom()
{
	// ### [ 관전기능 ] ###
	const int nMaxNewPlayer = GetMaxNewPlayer();
	for(int i=1; i<nMaxNewPlayer; i++) 
		GetPlayer(i)->Clear();
}

const IDX_GAME CGameProcess::GetGameType(void)
{
	ASSERT(m_pGame != NULL);

	if(m_pGame)
	{
		return m_pGame->GetGameType();
	}

	return IDX_GAME_NULL;
}

const int CGameProcess::GetMaxRoom(void)
{
	ASSERT(m_pGame != NULL);

	if(m_pGame)
	{
		return m_pGame->GetMaxRoom();
	}

	return -1;
}

const int CGameProcess::GetMaxPlayer(void)
{
	ASSERT(m_pGame != NULL);

	if(m_pGame)
	{
		return m_pGame->GetMaxPlayer();
	}

	return -1;
}

const int CGameProcess::GetMaxGamePlayer(void)
{
	ASSERT(m_pGame != NULL);

	if(m_pGame)
	{
		return m_pGame->GetMaxGamePlayer();
	}

	return -1;
}

const int CGameProcess::GetMaxObserver(void)
{
	ASSERT(m_pGame != NULL);

	if(m_pGame)
	{
		return m_pGame->GetMaxObserver();
	}

	return -1;	
}

const int CGameProcess::GetJokboQuestCount(void)
{
	ASSERT(m_pGame != NULL);

	if(m_pGame)
	{
		return m_pGame->GetJokboQuestCount();
	}

	return -1;	
}

const int CGameProcess::GetMaxNewPlayer(void)
{
	ASSERT(m_pGame != NULL);
	if(m_pGame)
	{
		return m_pGame->GetMaxNewPlayer();
	}

	return -1;	
}

// 2014.2.7 crpark
// 규제안 베팅예약 공통화
bool CGameProcess::CheckMyTurn(const int nMyIndex, const int m_nCurPlayer)
{
	if(m_nCurPlayer == nMyIndex)
	{
		return true;
	}
	return false;
}

bool CGameProcess::IsReservedButton(const RESERVESTATE reserveState_)
{
	if (reserveState_ == reserveState)
	{
		return true;
	}
	return false;
}

void CGameProcess::SetReserveButton(const RESERVESTATE reserveState_ /*= RESERVESTATE_BET_NONE*/)
{
	if (reserveState == reserveState_)
	{
		reserveState = RESERVESTATE_BET_NONE;
	}else
	{
		reserveState = reserveState_;
	}
}


//규제안 : 카드세팅
bool CGameProcess::CheckReceivedAllCards()
{
	//test i
	int setNum =  (int)g_RI.cCardSetting;
	int total  = g_nCardTotalCount;

	switch( GM().GetCurrentGameType() ){
		case IDX_GAME_NP:
			//setNum = (int)g_RI.cCardSetting - 1;
			break;

	}
	if(setNum <= total)
	{
		return true;
	}
	return false;
}


//규제안 : 로우바둑이 카드세팅(아침상태)
bool CGameProcess::CheckRoundForCardSet()
{
	int setNum =  (int)g_RI.cCardSetting;//0:기본, 1:아침
	int sindex  = g_RI.nChangeType;

	if(setNum == 0){
		return true;
	}

	// 아침, 점심, 저녁
	if ( sindex > 0 )
	{
		return true;
	}

	return false;
}