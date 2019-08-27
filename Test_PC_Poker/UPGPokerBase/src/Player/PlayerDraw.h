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
	CPlayer			*m_pPlayer;		//유저 데이터를 가지고 있다
	int				m_nPNum;		//유저의 위치 인덱스를 의미한다.
	
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

	CRect			m_rtAvatar;						//아바타의 영역
	CPoint			m_ptReservation;
	CPoint			m_ptChangeUserInfoBack;			//변동금액/진행판수 표시 위치
	CPoint			m_ptSameIPMark;
	bool			m_bSameIPMarkRight;
	
	bool			m_bClickAvatar;
	bool			m_bShowChangeUserInfo;
	int				m_nBackIndex;
	CPoint			m_ptCardStartPos;				// 플레이어 카드의 시작 위치
	bool			m_bOverHaveMoney;

	// 올인 관련 변수 
	DWORD			m_dwAllinAniStartTime;			// 올인 애닉 관련 타임 변수
	DWORD			m_dwAllinAngelStartTime;		// 올인천사 애니 관련 타임 변수

	DWORD			m_dwCongNotiStartTime;			// 타겟알림 종료 타임
	queue<CongDrawData>	m_CongQueData;
	CRITICAL_SECTION	m_cLockQueue;

	//게임쪽에서 찾아서 지워줘야 하는 변수들
	bool			m_bExitReservation;
	bool			m_bObserverReservation;
	bool			m_bGameOver;					// 게임 오버 상태인가?
	bool			m_bSelectCard;
	bool			m_bDrawMyValue;					// 기권승으로 내가 포카드 이상의 족보로 이겼을때 내카드를 보여주고 족보를 표시해준다
	bool			m_bTurn;
	//yoo 2015.01 규제안
	bool			m_bLimitMoney;
	NMBASE::SKINGDI::CNText			*m_pTotalBet;					// 총베팅금액
	NMBASE::SKINGDI::CNText			*m_pJokboName;					// 족보이름
	NMBASE::SKINGDI::CNText			*m_pBetMoney;					// 이번 베팅 금액
	NMBASE::SKINGDI::CNText			*m_pGold;						// 현재 골드
	NMBASE::SKINGDI::CNText			*m_pRakeBackGold;				// 적립된 골드
	NMBASE::SKINGDI::CNText			*m_pRakeBackGoldEvent;			// 골드 적립 몇배 증가 이벤트
	
	NMBASE::SKINGDI::CNText			*m_pMsgID;						// 축하메시지 보낸이
	NMBASE::SKINGDI::CNText			*m_pMsgData;					// 축하메시지 내용
	
	NMBASE::SKINGDI::CNText			*m_pGoldMileage;				// 골드 마일리지 텍스트
	bool							m_bShowGoldMileageInfo;
	bool							m_bShowOverMoneyGB;				// 골드 적립 통장 한도 초과 툴팁
	
	CString							m_strRakeBackGold;
	DWORD							m_dwRakeBackGoldStartTime;		// 골드 적립 애니 관련 타임 변수
	DWORD							m_dwRakeBackGoldEventStartTime;	// 골드 적립 이벤트 애니 관련 타임 변수
	DWORD							m_dwGoldRewardStartTime;		// 골드리워드 애니 관련 타임 변수
	NMBASE::SKINGDI::CNText			*m_pGoldReward;					// 골드리워드 획득 머니

	DWORD							m_dwGoldNotiStartTime;			// 골드출석부 노티 애니 타임 변수
	NMBASE::SKINGDI::CNText			*m_pGoldNotiId;					// 골드출석부 획득 유저 ID
	NMBASE::SKINGDI::CNText			*m_pGoldNotiValue;				// 골드출석부 획득 머니

	DWORD							m_dwGoldInsuNotiStartTime;		// 골드 보험 노티 애니 타임 변수
	
	// 카페 미가입 유저의 포인트 적립 정보
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

	//yoo 2015.01 규제안
	virtual void	SetLimitMoney(bool bPlay){ m_bLimitMoney = bPlay; }

	virtual void	SetObserverReservation(bool bExit){m_bObserverReservation = bExit;}
	virtual bool	IsObserverReservation(void){return m_bObserverReservation;}

	virtual void	SetPlayAllinAni(DWORD dwStartTime){m_dwAllinAniStartTime = dwStartTime;}
	virtual void	SetDrawMyValue(bool bDraw){m_bDrawMyValue = bDraw;}

	virtual void	SetSelectCard(bool bSelectCard){m_bSelectCard = bSelectCard;}
	virtual bool	IsSelectCard(void){return m_bSelectCard;}

	virtual void SetChangeRoomInfo(CHANGE_USERINFO_ROOM changeInfo) { m_pPlayer->SetChangeRoomInfo(changeInfo);}
	virtual CHANGE_USERINFO_ROOM *GetChangeRoomInfo(void){return m_pPlayer->GetChangeRoomInfo();}

	//shisky 2017.08 올인천사
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

	//DrawBottom에서 그리는 함수들
	virtual void DrawTotalBetting(CDC *pDC);
	virtual void DrawBetting(CDC *pDC);
	virtual void DrawUserBack(CDC *pDC);
	virtual void DrawTurn(CDC *pDC);
	virtual void DrawAvatar(CDC *pDC);
	virtual void DrawNickName(CDC *pDC);
	virtual void DrawLevel(CDC *pDC);
	virtual void DrawMoney(CDC *pDC);
	//yoo 2015.01 규제안
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

	//DrawMiddle에서 그리는 함수들
	virtual void DrawTimer(CDC *pDC);
	virtual void DrawBossMark(CDC *pDC);
	virtual void DrawAllinAni(CDC *pDC);
	virtual void DrawAllinAngel(CDC *pDC);
	virtual void DrawGoldReward(CDC *pDC);	

	//DrawTop에서 그리는 함수들
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

	// 자신의 족보를 화면에 보이기 위해 족보를 갱신한다.
	virtual void UpdateJokboName();	

	int GetGoldMileageIdx();
	int GetGoldMileageIdx2();
	int GetCafeReservedPointIdx();
};