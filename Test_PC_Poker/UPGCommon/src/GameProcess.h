#pragma once

#include "../include/GameDef.h"
#include "CommMsgDef.h"

class CPlayer;
class CGame;

enum RESERVESTATE
{
	RESERVESTATE_BET_CMD_PING=0,			// ��	
	RESERVESTATE_BET_BET_CMD_DADANG,			// ����
	RESERVESTATE_BET_BET_CMD_ALLIN,			// ����
	RESERVESTATE_BET_BET_CMD_QUARTER,		// ���� 
	RESERVESTATE_BET_BET_CMD_HALF,			// ���� 
	RESERVESTATE_BET_BET_CMD_FULL,			// Ǯ
	RESERVESTATE_BET_BET_CMD_RACE,			// Race���� 	
	RESERVESTATE_BET_BET_CMD_SMALL_BLIND,	//���� ����ε� [4������]
	RESERVESTATE_BET_BET_CMD_BIG_BLIND,		//�� ����ε� [4������]
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
	// Ŭ���̾�Ʈ ���� ����
	int				m_nGameState;				// 0 ����, 1, 2, 3, 4, 5, 6 �°����� 7 �°���(���� ����)
	int				m_nNo1PosPNum;
	int				m_nMyPosPNum;
	// Ÿ�̸� ī��Ʈ	
	int				m_nGameCount;	
	int				m_nStartUserNum;			// ���� ���� ���� �ο�

	int				m_nCurPlayer;				// ���� ����
	INT64			m_i64Double;				//���簪 �̸� ����

	INT64			m_i64JackPotActive;

	//������ ī�弼�� :  ���� ����ī�� ����
	int				g_nCardTotalCount;

public:
	CGameProcess(CGame *pGame);
	virtual ~CGameProcess(void);

	//����� ��� ���� ������...
	//�ݵ�� CGame�ʿ��� ��� �����ñ� �ٶ��ϴ�..
	const IDX_GAME	GetGameType(void);
	const int		GetMaxRoom(void);
	const int		GetMaxPlayer(void);
	const int		GetMaxGamePlayer(void);
	const int		GetMaxObserver(void);
	const int		GetJokboQuestCount(void);
	const int		GetMaxNewPlayer(void);
 
	inline CGame *GetGame(void){return m_pGame;}

	//�÷��̾� ��ȯ
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

	// ���� ���� �Լ���...	
	virtual bool Accept_CreateRoom(void *pMsg)=0;	// �游��� �㰡
	virtual bool Accept_EnterRoom(void *pMsg)=0;	// �濡 ���� �㰡 ����
	virtual void User_EnterRoom(void *pMsg)=0;		// ������ �濡 ���� �˸�
	virtual void User_OutRoom(void *pMsg)=0;		// ������ �濡�� ���� �˸�
	virtual void SetPlayInfo(int nowpnum, int newpnum, BOOL bParticipation )=0;

	virtual CPlayer *GetPlayer(int i)=0;

	//virtual int  GetStartPlayer()=0;

	//�������������̿��� ��� ó�� (�ڸ���Ŵ��. �̺�Ʈ..���)
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
	// ������ ���ÿ��� ����ȭ
	// �� ���̸� true, �ƴϸ� false
	bool CheckMyTurn(const int nMyIndex, const int m_nCurPlayer);
	bool IsReservedButton(const RESERVESTATE reserveState_);
	void SetReserveButton(const RESERVESTATE reserveState_ = RESERVESTATE_BET_NONE);

public:
	RESERVESTATE ReserveState() const { return reserveState; }
	void ReserveState(RESERVESTATE val) { reserveState = val; }
	bool CanReserve() const { return m_bCanReserve; }
	void CanReserve(bool val) { m_bCanReserve = val; }

	//������ : ī�弼��
	bool CheckReceivedAllCards();

	//������ : �ο�ٵ��� ī�弼��
	bool CheckRoundForCardSet(); 

private:
	RESERVESTATE reserveState;
	//������ ���� ���� ��� 2014.01
	bool			m_bCanReserve;
	
};
