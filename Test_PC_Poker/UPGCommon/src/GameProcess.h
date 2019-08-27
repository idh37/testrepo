#pragma once

#include "../include/GameDef.h"
#include "CommMsgDef.h"

class CPlayer;
class CGame;

enum RESERVESTATE
{
	RESERVESTATE_BET_CMD_PING=0,			// 삥	
	RESERVESTATE_BET_BET_CMD_DADANG,			// 따당
	RESERVESTATE_BET_BET_CMD_ALLIN,			// 올인
	RESERVESTATE_BET_BET_CMD_QUARTER,		// 쿼터 
	RESERVESTATE_BET_BET_CMD_HALF,			// 하프 
	RESERVESTATE_BET_BET_CMD_FULL,			// 풀
	RESERVESTATE_BET_BET_CMD_RACE,			// Race베팅 	
	RESERVESTATE_BET_BET_CMD_SMALL_BLIND,	//스몰 블라인드 [4구실전]
	RESERVESTATE_BET_BET_CMD_BIG_BLIND,		//빅 블라인드 [4구실전]
	RESERVESTATE_BET_BET_CMD_FOLD,			//FOLD
	RESERVESTATE_BET_BET_CMD_CALL,			// CALL
	RESERVESTATE_BET_NONE,
	MAX_RESERVESTATE_BET
};

class UPG_COMMON_EXPORT CGameProcess
{
public:
	NMBASE::GRAPHICGDI::CPage			*m_pPage;
	CGame			*m_pGame;
	// 클라이언트 현재 상태
	int				m_nGameState;				// 0 없음, 1, 2, 3, 4, 5, 6 승결정중 7 승결정(게임 오버)
	int				m_nNo1PosPNum;
	int				m_nMyPosPNum;
	// 타이머 카운트	
	int				m_nGameCount;	
	int				m_nStartUserNum;			// 현재 게임 시작 인원

	int				m_nCurPlayer;				// 현재 순서
	INT64			m_i64Double;				//따당값 미리 구함

	INT64			m_i64JackPotActive;

	//규제안 카드세팅 :  현재 보유카드 개수
	int				g_nCardTotalCount;

public:
	CGameProcess(CGame *pGame);
	virtual ~CGameProcess(void);

	//절대로 상속 받지 마세요...
	//반드시 CGame쪽에서 상속 받으시기 바랍니다..
	const IDX_GAME	GetGameType(void);
	const int		GetMaxRoom(void);
	const int		GetMaxPlayer(void);
	const int		GetMaxGamePlayer(void);
	const int		GetMaxObserver(void);
	const int		GetJokboQuestCount(void);
	const int		GetMaxNewPlayer(void);
 
	inline CGame *GetGame(void){return m_pGame;}

	//플레이어 변환
	int		GetPNum_ByServPN(int onum);
	int		GetServPNum_ByPN(int onum);	
	int		GetPlayerPNum(const char *id);	
	CString GetPlayerNickName(int pnum);
	CString GetPlayerNickName(const char *id);
	int		GetPlayerPNum_ByNick(const char *nick);	
	int		GetStartUserNum();

	INT64	GetRealBet();
	INT64	GetRaiseBat();
	INT64	GetTotalBet();
	INT64	GetMaxMoney(INT64 nTotal);
	INT64	GetMaxMoneyForFull(INT64 nTotal);
	INT64	GetLimitBetMoney( int nLimitX );
	INT64	GetLimitRaiseMoney( int nLimitX );

	virtual void Init(NMBASE::GRAPHICGDI::CPage *ppage);
	virtual void OnTimer()=0;
	virtual void ResetGame()=0;
	virtual BOOL OnMouseMove(int x, int y)=0;
	virtual BOOL OnLButtonDblclk(int x, int y)=0;
	virtual BOOL OnLButtonUp(int x, int y)=0;
	virtual BOOL OnLButtonDown(int x, int y)=0;
	virtual void OnGameKey(int nVKey, int nextended, int repeat)=0;

	virtual BOOL IsPlayingNow()=0;

	// 게임 진행 함수들...	
	virtual bool Accept_CreateRoom(void *pMsg)=0;	// 방만들기 허가
	virtual bool Accept_EnterRoom(void *pMsg)=0;	// 방에 입장 허가 받음
	virtual void User_EnterRoom(void *pMsg)=0;		// 유저가 방에 입장 알림
	virtual void User_OutRoom(void *pMsg)=0;		// 유저가 방에서 나감 알림
	virtual void SetPlayInfo(int nowpnum, int newpnum, BOOL bParticipation )=0;

	virtual CPlayer *GetPlayer(int i)=0;

	//virtual int  GetStartPlayer()=0;

	//순수게임진행이외의 기능 처리 (자리지킴이. 이벤트..등등)
	virtual void OnEnterRoom(BOOL bCreation);
	virtual void OnExitRoom();

	//
	virtual void SendCallButton() = 0;
	virtual void SendCheckButton() = 0;
	virtual void SendDieButton() = 0; 
	virtual void SendHalfButton() = 0;
	virtual void SendDdaButton() = 0;
	virtual void SendPingButton() = 0;

	// 2014.2.7 crpark
	// 규제안 베팅예약 공통화
	// 내 턴이면 true, 아니면 false
	bool CheckMyTurn(const int nMyIndex, const int m_nCurPlayer);
	bool IsReservedButton(const RESERVESTATE reserveState_);
	void SetReserveButton(const RESERVESTATE reserveState_ = RESERVESTATE_BET_NONE);

public:
	RESERVESTATE ReserveState() const { return reserveState; }
	void ReserveState(RESERVESTATE val) { reserveState = val; }
	bool CanReserve() const { return m_bCanReserve; }
	void CanReserve(bool val) { m_bCanReserve = val; }

	//규제안 : 카드세팅
	bool CheckReceivedAllCards();

	//규제안 : 로우바둑이 카드세팅
	bool CheckRoundForCardSet(); 

private:
	RESERVESTATE reserveState;
	//규제안 관련 예약 기능 2014.01
	bool			m_bCanReserve;
	
};
