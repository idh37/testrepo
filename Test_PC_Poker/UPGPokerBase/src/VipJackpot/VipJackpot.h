#pragma once



class CUIInfoData;
#define IDM_VIPJACKPOTEXPLAIN (WM_USER+6303)
#define IDM_VIPJACKPOTBETEXPLAIN (WM_USER+6304)

class UPG_GAMEBASE_EXPORT CVipJackpot
{
public:
	enum{
		VIPJACKPOT_STATE_NONE=-1,
		VIPJACKPOT_STATE_CLEAR=0,
		VIPJACKPOT_STATE_TIMENOTICE=2,
		VIPJACKPOT_STATE_TIMESTARTNOTI=3,
		VIPJACKPOT_STATE_TIMESTART=4,
		VIPJACKPOT_STATE_TIMEING_RESERVE_NOT=6,
		VIPJACKPOT_STATE_TIMEING_RESERVE=7,
		VIPJACKPOT_STATE_TIMEEND_PREV=8,
		VIPJACKPOT_STATE_TIMEEND_RESERVE_NOT=9,
		VIPJACKPOT_STATE_TIMEEND_RESERVE=10,
		VIPJACKPOT_STATE_CHOOSE=11,
		VIPJACKPOT_STATE_CHOOSE_RESERVE_NOT=12,
		VIPJACKPOT_STATE_RESERVE_NOT_RESERVELIST=13,
		VIPJACKPOT_STATE_RESERVE_NOT_RESERVEPRIZE=14,
		VIPJACKPOT_STATE_RESERVE_NOT_END=15,
		VIPJACKPOT_STATE_CHOOSE_RESERVE=16,
		VIPJACKPOT_STATE_RESERVE_START=17,
		VIPJACKPOT_STATE_RESERVE_WINNER=18,
		VIPJACKPOT_STATE_RESERVE_END=20,
		VIPJACKPOT_STAGE_STATE_NONE=0,
		VIPJACKPOT_STAGE_STATE_ING,
		VIPJACKPOT_STAGE_STATE_END,
	};
protected:
	NMBASE::GRAPHICGDI::xSprite		m_sprVipJackpotBack;
	NMBASE::GRAPHICGDI::xSprite		m_sprVipJackpotEffect;
	NMBASE::GRAPHICGDI::xSprite		m_sprVipJackpotBtn;
	NMBASE::GRAPHICGDI::xSprite		m_sprVipJackpotBet;
	
	NMBASE::SKINGDI::CGraphButton	m_btnVipJackpotExplain;
	NMBASE::SKINGDI::CGraphButton	m_btnVipJackpotBetExplain;
	NMBASE::SKINGDI::CNTextScroll  *m_pTxtScroll_List;
	bool m_bShowVipJackpotExplain;
	bool m_bShowVipJackpotBetExplain;
	int m_nTimer;
	int m_nSetTimer;
	int m_nWinnerBackIndex;
	DWORD m_dwWinnerBackTime;
	DWORD m_dwWinnerBackEndTime;
	int m_nEffectIndex;
	DWORD m_dwEffectTime;
	DWORD m_dwEffectEndTime;
	int m_nEffectSound;
	int m_nBetEffectIndex;
	DWORD m_dwBetEffectTime;
	DWORD m_dwBetEffectEndTime;
	int m_nBetPosEffectIndex;
	DWORD m_dwBetPosEffectTime;
	DWORD m_dwBetPosEffectEndTime;
	int m_nTimerEffectIndex;
	DWORD m_dwTimerEffectTime;
	DWORD m_dwTimerEffectEndTime;
	DWORD m_dw5TimerEffectTime;
	int m_n5TimerEffectSound;
	int m_nStartEffectIndex;
	DWORD m_dwStartEffectTime;
	DWORD m_dwStartEffectEndTime;
	vector<int> m_cvtWinnerEffectIndex;
	DWORD m_dwWinnerEffectTime;
	DWORD m_dwWinnerEffectEndTime;
public:
	int m_nVipJackpotState;
	long long m_llVipJackpotMoney;
	string m_sMininumMoney;
	string m_sRuleType;
	string m_sWinNames;
	string m_sWinMoney;
	string m_sWinnerMoney;
	string m_sJackpotList;
	int m_nStage;
	int m_nState;
	int m_nRoomBetPoint;
	int m_nReserveBetPoint;
	vector<int> m_cvtWinnerPos;
	vector<long long> m_cvtWinnerMoney;
	CPoint m_cpBetPos;
	bool m_bBetAccent;
	long long m_llShowJackpotMoney;
	long long m_llShowOldJackpotMoney;
public:
	CVipJackpot();
	virtual ~CVipJackpot();

	virtual void LoadImage();
	virtual void Init(CGameViewBase *pGameView);
	virtual void DrawBottom(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC);
	virtual void DrawMiddle(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC);
	virtual void DrawTop(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC);
public:
	void DrawJackpot(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC);	// ªÛ¥‹¿« ¿Ë∆Ã πË≥ 
	void DrawJackpotNumber(NMBASE::GRAPHICGDI::CPage *pPage);
	void DrawBetPoint(int nBetPoint, NMBASE::GRAPHICGDI::CPage *pPage);
	void DrawReserveBetPoint(int nBetPoint, NMBASE::GRAPHICGDI::CPage *pPage);
	void DrawTimer(int nTimer, NMBASE::GRAPHICGDI::CPage *pPage);
	void DrawMoney(long long llMoney, NMBASE::GRAPHICGDI::CPage *pPage);
	void DrawResult(string sName, string sMoney, CDC *pDC);
public:

	virtual BOOL OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData);
	virtual BOOL OnCommand( WPARAM wParam, LPARAM lParam );

	virtual BOOL OnLButtonDown(int x, int y);
	virtual BOOL OnLButtonUp(int x, int y);
	virtual BOOL OnMouseMove(int x, int y);

	virtual void OnEnterRoom(void);
	virtual void OnStartGame(void);
	virtual void OnBetGame(CPoint cPos, bool bAccent);
	virtual void OnOutRoom(void);
public:
	void SetShowJackpotMoney(long long llShowJackpotMoney){ m_llShowJackpotMoney=llShowJackpotMoney; }
	void SetShowOldJackpotMoney(long long llShowOldJackpotMoney){ m_llShowOldJackpotMoney=llShowOldJackpotMoney; }
	void SetMininumMoney(char* pMininumMoney);
	void SetRuleType(char* pRuleType);
	void SetStage(int nStage);
	void SetStageState(int nStage, int nState);
	void SetVipJackpotState(int nState, bool bClear=false);
	void SetVipJackpotServerState(int nEventState, bool bIsRoom, bool bJackpotRoom, int nSecond);
	void NextVipJackpotState(void);
	void AddVipJackpotBetPoint(int nPoint);
	void MakeVIPJackpotPosPoint(int nPoint);
	void SetVipJackpotNum(int nNum);
public:
	string StringToVipJackpotState(int nState);
	string GetGameTypeString(int eGame);
	string GetChannelString(long long llLimitMoney);
	string GetJackpotMoneyString(long long llJackpotMoney);
	string ChangeID(char* pID);
	int GetIDXToGameType(IDX_GAME eGame);
	IDX_GAME GetIDXFromGameType(int eGame);
	bool IsVipJackpotGame(void);
	int GetLimitMoneyChannel(long long llLimitMoney);
	int GetLimitMoneyChannel(IDX_GAME eGame, long long llLimitMoney, long long llSeedMoney);
public:
	void PlaySound(LPCTSTR lpCommand);
};