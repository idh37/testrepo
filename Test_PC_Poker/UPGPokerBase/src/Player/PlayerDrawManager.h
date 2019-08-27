#pragma once

#include <vector>

class CUIInfoData;
class CPlayerDraw;
class CPlayerDraw_BD;
class CPlayerDraw_HL;
class CPlayerDraw_SD;

class UPG_GAMEBASE_EXPORT CPlayerDrawManager  
{
protected:
	int m_nMaxPlayer;
	CGameProcess *m_pGameProcess;
	CGameViewBase *m_pGameView;
	CUIInfoData *m_pUIInfoData;

	bool m_bGameOver; // 진정한 게임 오버다
	DWORD m_dwGameOverStartTime;

	NMBASE::GRAPHICGDI::xSprite	m_sprChangeGameInfoBack;
	NMBASE::GRAPHICGDI::xSprite	m_sprMyMoney;
	NMBASE::GRAPHICGDI::xSprite	m_sprMyGold;
	NMBASE::GRAPHICGDI::xSprite	m_sprUserMoney;
	NMBASE::GRAPHICGDI::xSprite	m_sprUserGold;
	NMBASE::GRAPHICGDI::xSprite	m_sprUserBack;
	NMBASE::GRAPHICGDI::xSprite	m_sprWarning;
	NMBASE::GRAPHICGDI::xSprite	m_sprHostBossDealer;
	NMBASE::GRAPHICGDI::xSprite	m_sprTimer;
	NMBASE::GRAPHICGDI::xSprite	m_sprReservation;
	NMBASE::GRAPHICGDI::xSprite	*m_pWinLoseBackSpr;
	NMBASE::GRAPHICGDI::xSprite	m_sprHaveMoneyToolTip;
	NMBASE::GRAPHICGDI::xSprite	m_sprAllin;
	NMBASE::GRAPHICGDI::xSprite	m_sprLevel;

	// yoo 2015.01 규제안
	NMBASE::GRAPHICGDI::xSprite	m_sprLimitIcon;
	// 2017.5 손실한도 초과
	NMBASE::GRAPHICGDI::xSprite	m_sprLimitOverIcon;

	// 2017.8 올인천사 시스템
	NMBASE::GRAPHICGDI::xSprite	m_sprAllinAngel;
	CEffectBase m_cEffect_AllinAngel;

	// 2017.11 골드 아이콘
	NMBASE::GRAPHICGDI::xSprite	m_sprGold;

	// 2017.12 골드 올인천사
	NMBASE::GRAPHICGDI::xSprite	m_sprGoldReward;
	CEffectBase m_cEffect_GoldReward;

	// 2018.01 추가 골드 애니메이션
	NMBASE::GRAPHICGDI::xSprite	m_sprGoldPlus;
	CEffectBase m_cEffect_GoldPlus;
	
	// 2018.02 골드 마일리지
	NMBASE::GRAPHICGDI::xSprite	m_sprGoldMileage;

	// 2018.03 골드출석부 알람.
	NMBASE::GRAPHICGDI::xSprite	m_sprGoldNoti;

	// 2018.06 골드 보험 가입 알람.
	NMBASE::GRAPHICGDI::xSprite	m_sprGoldInsuNoti;
	CEffectBase m_cEffect_GoldInsuNoti;
	
	NMBASE::GRAPHICGDI::xSprite	m_sprTurn; // 승리자에게 보이는 스포트라이트
	NMBASE::GRAPHICGDI::xSprite *m_pTurnSpr;
	
	NMBASE::GRAPHICGDI::xSprite	m_sprTargetNoti;
	
	std::vector<CPlayerDraw *> m_listPlayer;
	
	//게임에서 찾아서 지워줘야 하는 변수들
	bool m_bFlyWindCard; // 카드 날리는 중인가
	BOOL m_listClockFlag[16];
	DWORD m_dwTimerStartTime;
	DWORD m_dwTimerCurTime;
	int	m_nWinnerPNum; // 하이 승자
	BOOL m_bOpen; // 어떻게 이겼는가?(모두 죽어서 홀로 이겼는가 FALSE, 끝까지 베팅했는가 TRUE)
	bool m_bHiddenCardShow;

	std::vector<INT64> m_listResultMoney;
	std::vector<CString> m_listResultJokbo;
public:
	CPlayerDrawManager(int nMaxPlayer);
	virtual ~CPlayerDrawManager();

	virtual void LoadImage(LPCTSTR lpPathTimer, 
		NMBASE::GRAPHICGDI::xSprite *pBetCmdSpr,
		NMBASE::GRAPHICGDI::xSprite *pWinLoseBackSpr,
		LPCTSTR lpAllinPath, 
		NMBASE::GRAPHICGDI::xSprite *pTurnSpr = NULL, 
		LPCTSTR szUserBGPath = NULL, 
		LPCTSTR szLevelPath=NULL);

	virtual void Init(CUIInfoData *pUIInfoData, CGame *pGame);
	virtual void AddPlayer(CPlayer *pPlayer, CPlayerDraw *pDraw, int nNum);

	virtual void SetShow(int nPNum, bool bShow);
	virtual bool IsShow(int nPNum);

	virtual void SetLock(int nPNum, bool bLock);
	virtual bool IsLock(int nPNum);

	virtual void SetBettingMoney(int nPNum, int nBet, INT64 i64Money, bool bRaise=false);
	virtual void ClearBettingMoney(int nPNum);
	virtual void SetTurn(int nPNum);

	virtual void SetWinnerPNum(int nPNum){m_nWinnerPNum = nPNum;} // 서버에서 보내준 WinnerPlayerNumber를 넘겨 줘야 합니다.
	virtual int  GetWinnerPNum(void){return m_nWinnerPNum;}

	virtual void Reset(void);
	virtual void Reset(int nPNum);

	virtual void SetFlyWindCard(bool bFlyWind){m_bFlyWindCard = bFlyWind;}
	virtual bool IsFlyWindCard(void){return m_bFlyWindCard;}
	virtual void SetStartTimer(void);
	virtual void ClearTimer(void){m_dwTimerStartTime = 0;}

	virtual DWORD GetCurTimerTime(void){return m_dwTimerCurTime;}
	virtual void SetCurTimerTime(DWORD dwCurTime){m_dwTimerCurTime = dwCurTime;}
	virtual DWORD GetStartTimerTime(void){return m_dwTimerStartTime;}
	virtual DWORD GetTimerTimeGab(void){return m_dwTimerCurTime - m_dwTimerStartTime;}

	virtual void SetPlayerGameOver(int nPNum, bool bGameOver);
	virtual bool IsPlayerGameOver(int nPNum);

	virtual void SetHiddenCardShow(bool bShow){m_bHiddenCardShow = bShow;}
	virtual bool IsHiddenCardShow(void){return m_bHiddenCardShow;}

	virtual void SetOpen(BOOL bOpen){m_bOpen = bOpen;}
	virtual BOOL IsOpen(void){return m_bOpen;}

	virtual void SetGameOver(bool bGameOver, DWORD dwStartTime = 0);
	virtual bool IsGameOver(void){return m_bGameOver;}
	virtual DWORD GetGameOverStartTime(void){return m_dwGameOverStartTime;}
	virtual void CheckResultValue(void);

	virtual void SetExitReservation(int nPNum, bool bExit);
	virtual bool IsExitReservation(int nPNum);
	//yoo 2015.01 규제안
	virtual void SetLimitMoney( int nPNum, bool bPlay );

	virtual void SetObserverReservation(int nPNum, bool bReservation);
	virtual bool IsObserverReservation(int nPNum);

	virtual void SetPlayAllinAni(int nPNum, DWORD dwStartTime);
	virtual void SetDrawMyValue(int nPNum, bool bDraw);

	virtual void SetPlayAllinAngel(int remainCount, DWORD dwStartTime);

	virtual void SetPlayCongNoti(std::string id, std::string data, int playTime);

	virtual void SetPlayGoldReward(INT64 rewardGold, DWORD dwStartTime);
	virtual void SetPlayRakeBackGold(INT64 chipInt, int chipPrime);
	virtual void SetPlayRakeBackGoldEvent(float eventRate);

	virtual void SetPlayGoldNoti(std::string id, INT64 rewardGold, DWORD dwStartTime);

	virtual void SetSelectCard(int nPNum, bool bSelectCard);
	virtual bool IsSelectCard(int nPNum);

	virtual void OnEnterRoomLimitMoney(int nPNum, INT64 lossmoney);

	virtual void OnEnterRoom(int nPNum);
	virtual void OnExitRoom(int nPNum);

	virtual void OnEnterRoom(void);
	virtual void OnExitRoom(void);

	virtual BOOL OnLButtonDown(int x, int y);
	virtual BOOL OnLButtonUp(int x, int y);
	virtual BOOL OnMouseMove(int x, int y);

	virtual void DrawBottom(CDC *pDC);
	virtual void DrawMiddle(CDC *pDC);
	virtual void DrawTop(CDC *pDC);
	virtual void DrawTopMost(CDC *pDC);

	//맞포커에서 자신의 머니 표시 위치, 자신의 족보 표시 위치가 방입장시 게임 종류에 따라 변경될수 있음
	virtual void InitUIData(void);

	virtual void MyJokboNameUpdate();

	friend CPlayerDraw;
	friend CPlayerDraw_BD;
	friend CPlayerDraw_HL;
	friend CPlayerDraw_SD;
};