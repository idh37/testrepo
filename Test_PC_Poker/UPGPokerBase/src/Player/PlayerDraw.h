#pragma once

#include "SprMoneyDraw.h"
#include "../UIInfoData.h"
#include "../Promotion/PromotionBase.h"

class CPlayerDrawManager;

struct CongDrawData
{
	CString id;
	CString message;
	int time;
};


class UPG_GAMEBASE_EXPORT CPlayerDraw
{
protected:
	ENUM_POSTYPE	m_nPosType;
	CPlayer			*m_pPlayer;		//���� �����͸� ������ �ִ�
	int				m_nPNum;		//������ ��ġ �ε����� �ǹ��Ѵ�.
	
	NMBASE::GRAPHICGDI::CPage				*m_pPage;
	CGameProcess		*m_pGameProcess;
	CUIInfoData			*m_pUIInfoData;
	CPlayerDrawManager	*m_pManager;

	bool			m_bShow;
	bool			m_bLock;

	NMBASE::SKINGDI::CNText			*m_pName;
	
	CPoint			m_ptLevel;
	CPoint			m_ptChar;
	CPoint			m_ptName;
	CPoint			m_ptBack;
	CPoint			m_ptBoss;
	CPoint			m_ptMaster;
	CPoint			m_ptGold;

	CSprMoneyDraw	m_cMoneyDraw;

	CRect			m_rtAvatar;						//�ƹ�Ÿ�� ����
	CPoint			m_ptReservation;
	CPoint			m_ptChangeUserInfoBack;			//�����ݾ�/�����Ǽ� ǥ�� ��ġ
	CPoint			m_ptSameIPMark;
	bool			m_bSameIPMarkRight;
	
	bool			m_bClickAvatar;
	bool			m_bShowChangeUserInfo;
	int				m_nBackIndex;
	CPoint			m_ptCardStartPos;				// �÷��̾� ī���� ���� ��ġ
	bool			m_bOverHaveMoney;

	// ���� ���� ���� 
	DWORD			m_dwAllinAniStartTime;			// ���� �ִ� ���� Ÿ�� ����
	DWORD			m_dwAllinAngelStartTime;		// ����õ�� �ִ� ���� Ÿ�� ����

	DWORD			m_dwCongNotiStartTime;			// Ÿ�پ˸� ���� Ÿ��
	queue<CongDrawData>	m_CongQueData;
	CRITICAL_SECTION	m_cLockQueue;

	//�����ʿ��� ã�Ƽ� ������� �ϴ� ������
	bool			m_bExitReservation;
	bool			m_bObserverReservation;
	bool			m_bGameOver;					// ���� ���� �����ΰ�?
	bool			m_bSelectCard;
	bool			m_bDrawMyValue;					// ��ǽ����� ���� ��ī�� �̻��� ������ �̰����� ��ī�带 �����ְ� ������ ǥ�����ش�
	bool			m_bTurn;
	//yoo 2015.01 ������
	bool			m_bLimitMoney;
	NMBASE::SKINGDI::CNText			*m_pTotalBet;					// �Ѻ��ñݾ�
	NMBASE::SKINGDI::CNText			*m_pJokboName;					// �����̸�
	NMBASE::SKINGDI::CNText			*m_pBetMoney;					// �̹� ���� �ݾ�
	NMBASE::SKINGDI::CNText			*m_pGold;						// ���� ���
	NMBASE::SKINGDI::CNText			*m_pRakeBackGold;				// ������ ���
	NMBASE::SKINGDI::CNText			*m_pRakeBackGoldEvent;			// ��� ���� ��� ���� �̺�Ʈ
	
	NMBASE::SKINGDI::CNText			*m_pMsgID;						// ���ϸ޽��� ������
	NMBASE::SKINGDI::CNText			*m_pMsgData;					// ���ϸ޽��� ����
	
	NMBASE::SKINGDI::CNText			*m_pGoldMileage;				// ��� ���ϸ��� �ؽ�Ʈ
	bool							m_bShowGoldMileageInfo;
	bool							m_bShowOverMoneyGB;				// ��� ���� ���� �ѵ� �ʰ� ����
	
	CString							m_strRakeBackGold;
	DWORD							m_dwRakeBackGoldStartTime;		// ��� ���� �ִ� ���� Ÿ�� ����
	DWORD							m_dwRakeBackGoldEventStartTime;	// ��� ���� �̺�Ʈ �ִ� ���� Ÿ�� ����
	DWORD							m_dwGoldRewardStartTime;		// ��帮���� �ִ� ���� Ÿ�� ����
	NMBASE::SKINGDI::CNText			*m_pGoldReward;					// ��帮���� ȹ�� �Ӵ�

	DWORD							m_dwGoldNotiStartTime;			// ����⼮�� ��Ƽ �ִ� Ÿ�� ����
	NMBASE::SKINGDI::CNText			*m_pGoldNotiId;					// ����⼮�� ȹ�� ���� ID
	NMBASE::SKINGDI::CNText			*m_pGoldNotiValue;				// ����⼮�� ȹ�� �Ӵ�

	DWORD							m_dwGoldInsuNotiStartTime;		// ��� ���� ��Ƽ �ִ� Ÿ�� ����
	
	// ī�� �̰��� ������ ����Ʈ ���� ����
	bool							m_bShowCafeReservedInfo;
	bool							m_bShowCafeReservedPoint;
	NMBASE::SKINGDI::CNText			*m_pCafeReservedPoint;

public:
	CPlayerDraw();
	virtual ~CPlayerDraw();

	virtual ENUM_POSTYPE	GetPosType(int nPNum);
	virtual void			Init(CPlayer *pPlayer, int nNum, NMBASE::GRAPHICGDI::CPage *pPage, CPlayerDrawManager *pManager);

	virtual void	OnExitRoom(void);
	virtual void	OnEnterRoom(void);

	virtual void	Reset(void);

	virtual void	SetBettingMoney(int nBet, INT64 i64Money, bool bRaise=false);
	virtual void	ClearBettingMoney(void);

	virtual void	SetTurn(bool bTurn){m_bTurn = bTurn;}
	virtual bool	IsTurn(void){return m_bTurn;}

	virtual void	SetShow(bool bShow){m_bShow = bShow;}
	virtual bool	IsShow(void){return m_bShow;}

	virtual void	SetLock(bool bLock){m_bLock = bLock;}
	virtual bool	IsLock(void){return m_bLock;}

	virtual void	SetGameOver(bool bGameOver){m_bGameOver = bGameOver;}
	virtual bool	IsGameOver(void){return m_bGameOver;}

	virtual void	SetExitReservation(bool bExit){m_bExitReservation = bExit;}
	virtual bool	IsExitReservation(void){return m_bExitReservation;}

	//yoo 2015.01 ������
	virtual void	SetLimitMoney(bool bPlay){ m_bLimitMoney = bPlay; }

	virtual void	SetObserverReservation(bool bExit){m_bObserverReservation = bExit;}
	virtual bool	IsObserverReservation(void){return m_bObserverReservation;}

	virtual void	SetPlayAllinAni(DWORD dwStartTime){m_dwAllinAniStartTime = dwStartTime;}
	virtual void	SetDrawMyValue(bool bDraw){m_bDrawMyValue = bDraw;}

	virtual void	SetSelectCard(bool bSelectCard){m_bSelectCard = bSelectCard;}
	virtual bool	IsSelectCard(void){return m_bSelectCard;}

	virtual void SetChangeRoomInfo(CHANGE_USERINFO_ROOM changeInfo) { m_pPlayer->SetChangeRoomInfo(changeInfo);}
	virtual CHANGE_USERINFO_ROOM *GetChangeRoomInfo(void){return m_pPlayer->GetChangeRoomInfo();}

	//shisky 2017.08 ����õ��
	virtual void	SetPlayAllinAngel(int remainCount, DWORD dwStartTime);
	
	virtual void	SetPlayCongNoti(std::string id, std::string data, int playTime);

	virtual void	SetPlayGoldReward(INT64 rewardGold, DWORD dwStartTime);
	virtual void	SetPlayRakeBackGold(INT64 nGoldInt, int nGoldPrime);
	virtual void	SetPlayRakeBackGoldEvent(int nGoldMultiply);

	virtual void	SetPlayGoldNoti(std::string id, INT64 rewardGold, DWORD dwStartTime);

	virtual void InitUIData(void);

	virtual BOOL OnLButtonDown(int x, int y);
	virtual BOOL OnLButtonUp(int x, int y);
	virtual BOOL OnMouseMove(int x, int y);

	//DrawBottom���� �׸��� �Լ���
	virtual void DrawTotalBetting(CDC *pDC);
	virtual void DrawBetting(CDC *pDC);
	virtual void DrawUserBack(CDC *pDC);
	virtual void DrawTurn(CDC *pDC);
	virtual void DrawAvatar(CDC *pDC);
	virtual void DrawNickName(CDC *pDC);
	virtual void DrawLevel(CDC *pDC);
	virtual void DrawMoney(CDC *pDC);
	//yoo 2015.01 ������
	virtual void DrawLimitIcon(CDC* pDC);
	virtual void DrawLimitOverIcon(CDC* pDC);
	virtual void DrawChangeInfo(CDC *pDC);
	virtual void DrawIPCheck(CDC *pDC);
	virtual void DrawMasterMark(CDC *pDC);
	virtual void DrawObserverReservationMark(CDC *pDC);
	virtual void DrawExitReservationMark(CDC *pDC);
	virtual void DrawRealMoney(CDC *pDC);
	virtual void DrawJokboName(CDC *pDC);
	virtual void DrawGold(CDC *pDC);
	virtual void DrawRakeBackGold(CDC *pDC);
	virtual void DrawGoldEvent(CDC *pDC);
	virtual void DrawGoldMileage(CDC *pDC);

	//DrawMiddle���� �׸��� �Լ���
	virtual void DrawTimer(CDC *pDC);
	virtual void DrawBossMark(CDC *pDC);
	virtual void DrawAllinAni(CDC *pDC);
	virtual void DrawAllinAngel(CDC *pDC);
	virtual void DrawGoldReward(CDC *pDC);	

	//DrawTop���� �׸��� �Լ���
	virtual void DrawCongNoti(CDC *pDC);
	virtual void DrawResult(CDC *pDC);
	virtual void DrawGoldMileageTooltip(CDC *pDC);
	virtual void DrawGoldNoti(CDC *pDC);
	virtual void DrawGoldInsuNoti(CDC *pDC);
	virtual void DrawCafeReservedPoint(CDC *pDC);

	virtual void DrawBottom(CDC *pDC);
	virtual void DrawMiddle(CDC *pDC);
	virtual void DrawTop(CDC *pDC);
	virtual void DrawTopMost(CDC *pDC);

	// �ڽ��� ������ ȭ�鿡 ���̱� ���� ������ �����Ѵ�.
	virtual void UpdateJokboName();	

	int GetGoldMileageIdx();
	int GetGoldMileageIdx2();
	int GetCafeReservedPointIdx();
};