#pragma once

#include "Game.h"
#include <map>
#include "PlayTimeWarning.h"
#include "DataRoomList.h"
#include "DataUserList.h"
#include "PrevRoomInfo.h"
#include "./Community/CommunityManager.h"

#include "LogFile.h"


//#define RESTRICTION_LEVEL

#define BUTTON_CHANGE_FULLTODADANG
//#define HIDE_END_EVENT_POPUP
//#define QA_TEST
#define _VIPJACKPOT
#define _JACKPOT_NON_CHANNEL_MONEY
//2018.04.30 8250억 보유시 선택입장 가능 삭제.
//#define _REGULATION
#define _GAMECHANNEL_CHANGE

#define MONEY_JO  1000000000000ll
#define MONEY_UK  100000000ll
#define MONEY_MAN 10000ll
#define _DEF_MEMO_RESET_TIMER 30001

#define IsVIPJackpot(gameindex)  ( ( (gameindex)==IDX_GAME_SP || (gameindex)==IDX_GAME_BD || (gameindex)==IDX_GAME_NP ) )

typedef vector<pair<string, long long> >::iterator VIterator;

struct comp
{
	comp(std::string const& s) : _s(s) { }

	bool operator () (std::pair<std::string, long long> const& p)
	{
		return (p.first == _s);
	}

	std::string _s;
};

template<template <typename> class P = std::less >
struct compare_pair_second {
	template<class T1, class T2> bool operator()(const std::pair<T1, T2>& left, const std::pair<T1, T2>& right) {
		return P<T2>()(left.second, right.second);
	}
};

typedef struct 
{
	bool	m_bUseQuickOption;		// 바로입장 설정 사용 여부

	bool	m_bUseGameMode;			// 게임모드 사용 여부

	int		m_nGameMode;			// 게임모드

	int		m_nRuleType;			// 훌라의 룰타입(스톱0n/off)

	bool	m_bUseBetRule;			// 베팅룰 사용여부

	bool	m_bAutoBet;				// 자동여부

	bool	m_bUseSeedMoney;		// 게임규모 사용 여부

	bool	m_bMaxPlayer;			// 입장 유저 제한 사용여부

	int		m_nSeedType;			// 게임규모(0: 큰방, 1: 일반방, 2:작은방)

	bool	m_bAutoEnterRoom;		// 자동으로 입장

	short	m_sBetRuleKind;			//리미트머니

	int		m_nCardSeting;			//카드세팅 4~6장

	float	m_nBetMulti;			//섯다 참가비

} SCONDITION2;


typedef struct tagPromotionData{
public:
	enum{
		PROMOTIONSTATE_NONE=0,
		PROMOTIONSTATE_START=1<<0,
		PROMOTIONSTATE_ING=1<<1,
		PROMOTIONSTATE_END=1<<2,
		PROMOTION_CNTS=10,
	};
public:
	tagPromotionData(void){ Clear(); }
public:
	bool IsStart(void){ return (PROMOTIONSTATE_START==(nState&PROMOTIONSTATE_START)); }
	bool IsIng(void){ return (PROMOTIONSTATE_ING==(nState&PROMOTIONSTATE_ING)); }
	bool IsEnd(void){ return (PROMOTIONSTATE_END==(nState&PROMOTIONSTATE_END)); }
	bool IsPromotion(void){ return PROMOTIONSTATE_NONE!=nState; }
public:
	void Clear(void){
		nState=PROMOTIONSTATE_NONE;
		nStep=0;
		nCnt=0;
		llReserved=0ll;
		memset(nCnts, 0, sizeof(int)*PROMOTION_CNTS);
	}
	void SetState(int nstate){ nState=nstate; }
public:
	int nState;
	int nStep;
	int nCnt;
	INT64 llReserved;
	INT64 llEffectReserved;
	int nCnts[PROMOTION_CNTS];
}tPromotionData;


class UPG_COMMON_EXPORT CGameManager
{
protected:
	enum ENUM_GAMELODING_TYPE
	{
		EGLT_FIRST= 0,
		EGLT_CREATE,
		EGLT_DIRECTENTER,
		EGLT_DIRECTCREATE,
		EGLT_ENTERROOM,
		EGLT_ENTERTOURNAMENTROOM,
		EGLT_ENTERDUMMYROOM,
		EGLT_PREMQUICKJOIN,
	};
	enum ENUM_LIMIT_TYPE{
		LIMIT_NONE=0,
		LIMIT_BETTING=1,
		LIMIT_LOSEMONEY=2,
	};
	enum{
		SHOWFULLEXPLAIN_MAX=3,
	};

	map<IDX_GAME, CGame *>	m_listGame;
	list<NMBASE::GRAPHICGDI::xSprite *>			m_listLoadSprite;
	CGame					*m_pCurGame;
	IDX_GAME				m_nCurGameType;
	HANDLE					m_hMutex;

	// 장시간 게임 플레이 경고
	CPlayTimeWarning		m_cPlayTimeWarning;

	int						m_nMaxRoom;
	int						m_nMaxPlayer;
	int						m_nMaxGamePlayer;
	int						m_nMaxObserver;
	int						m_nMaxNewPlayer;
	int						m_nJokboQuestCount;
	IDX_GAMEWHERE			m_nWhere;
	bool					m_bAbnormalOutRoom;				// 비정상적으로 퇴실해야하는가?
	bool					m_bWaitResponse;				// 응답을 기다리는 중인가?
	bool					m_bNowLogout;					// 지금 로그아웃중인가?

	CWnd					*m_pMainWnd;
	CPlayer					*m_pMyInfo;
	INT64					m_nMyGold;						//골드 재화 추가.
	int						m_nGoldRemain;					//골드 재화 소수점.
	INT64					m_nGoldValue;					//골드 재화 가치.
	ROOMINFO				m_stRoomInfo;					//방생성 옵션
	bool					m_bDeafultCreateRoom;
	ENUM_GAMELODING_TYPE	m_nLoadingType;
	int						m_nGoDirectEnterType;
	
	bool					m_bShowLoginDlg;				// 최초입장시 로그인 팝업을 보여줌.
	
	//방입장 옵션 (저장용)
	int						m_nRoomNum_for_Join;
	UINT64					m_llRoomKey_for_Join;
	bool					m_bObserve_for_Join;
	CString					m_strRoomPass_for_Join;

	// 방입장 옵션 규제안 추가 ( 일반, VIP 탭 인가 자유탭인가를 저장 ) // 0:자유  1:일반	2:골드
	int						m_nTabState;
	// 탭 Index 저장 VIP, NORMAL, VIP_GOLD, NORMAL_GOLD, FREE
	int						m_nTabIndex;

	//바로가기 입장 저장용
	bool					m_bUseRoomOption;
	ROOMOPTION				m_RoomOption;
	bool					m_bMorningBet;
	

	HCURSOR					m_hCurSel, m_hCurHand, m_hCurMove, m_hCurSize, m_hCurWait;


	int						m_nCurChanNo;		//현재 접속한 통합채널번호

	bool					m_bSerchEnterRoom;		//바로입장시 방을 찾아서 입장했는지 여부
	bool					m_bSerchAutoEnterRoom;	//바로입장실패시 자동으로 기본값으로 입장할지의 여부
	int						m_nDirectEnterGameMode;	//바로입장시 생성 게임의 종류, 일반/초이스/폭탄/조커
	int						m_bForceLocalServer;

	// 일이용시간 제한 시간.
	int						m_nPlayTimeSec ;

	//골드 출석부 결과 이펙트 진행 여부.
	bool					m_bPlayDailyGoldEffect;
	//골드 미니게임 결과 이펙트 진행 여부.
	bool					m_bPlayGoldBigWheel;

	//한도 초과 팝업 딜레이.
	bool					m_bWaitOwnLimitPopup;
	
	// 골드 보험 팝업 딜레이.
	bool m_bWaitGoldInsuPopup;

	// (게임/채널 이동시) 다른방 바로가기인지 여부
	bool					m_bQuickStartInRoom;		

	// 규제안 : 일일 손실머니 제한.
	INT64			m_llLossMoney;		// 획득/손실 금액 
	time_t			m_tBlockTime;		// 차단된 시간
	DAILYLOSS_STATE	m_eState;			// 손실 상태
	IDX_GAME		m_gameID;			//게임 아이디
	INT64           m_llLimitLoseMoney[6];   //게임별 손실머니
	bool			m_bReceiveLimitMoney[6];

	bool			m_CheckLossMoneyOver[6]; // 게임은 6개 이지만, index에 맞포커가 있어서 7로 정의.

	std::string		m_clearDateStr;	
	bool			m_bCannotPlayUpp;
	SYSTEMTIME		m_rewardTime;
	bool			m_bEnableOver;

	ENUM_PROFILE_OPEN_KIND	m_enumProfileOpenKind;	//프로필 창 띄울때 어느부분에서 띄우기 요청했는지 저장

	int	m_nEventMissionStep ;
	promotion::_BORNIN1996_ALARM	m_p1995EventInfo ;
	bool m_bShowEndEventPopup;

	///////////////////
	//공용데이터	
	CDataUserList			m_DataUserList;
	CHideChatList			m_HideChat;
	//게임방리스트
	CDataRoomList			m_DataRoomList;
	//머니구간정보(채널정보)
	CDataRoomOptionList		m_DataRoomOptionList;
	//이전방접속 정보저장
	CPrevRoomInfo			m_PrevRoomInfo;

	RESTRAINTINFO m_RestraintInfo;

	bool		m_bSelectQuickJoin; //규제 개선안 : 선택시작으로 방 입장 요청.
	bool		m_bQuickJoinForRandom; //규제안 : 상세구간에서 랜덤입장을 누를 경우 
	bool		m_bRequestOtherChannel; //규제안 : 입장 가능한 방이 다른 채널인 경우

	// 내 클럽 정보
	int			m_Club_ID;
	CString		m_Club_Name;
	int			m_Club_MemberGrade; 
	bool		m_Club_Enable_Create;
	float		m_Member_AddGet_Rate;
	float		m_Boss_PayBack_Rate;
	CString		m_Club_Msg;

	bool		m_IsShowingClubDlg;
	bool		m_bLobbyFriendTabRequest;
	
	// 선택 시작용 변수값.
	bool m_bSQJMustUseOption;
	INT64 m_llSQJEnterLimitMoney; 
	int m_nSQJGameMode; 
	int m_nSQJGamerule; 
	INT64 m_SQJllSeedMoney;
	bool m_bDiaOption;
	tPromotionData m_tPromotionData;
	promotion::RETENTION_ALRAM m_RETENTION_ALRAM;
	
	// 응모권으로 응모할 수 있는 황금 복주머니 상품 리스트
	TICKET_GIFT_INFO *m_ArrayTicketGiftList_gold; //등록된 상품 정보
	
	// 해당 유저의 티켓 응모 현황
	int m_MyTicketCount; //가지고 있는 응모권(티켓) 갯수 == 나의 황금 복주머니 갯수
	TICKET_SUBSCRIPTION_INFO *m_ArraySubscriptionList_gold; //등록된 응모 정보
	
	// 응모 결과
	int m_GiftIndex; // 상품 고유번호
	int m_ResultCode; // 0이면 응모 성공, 1000이면 응모권 부족, 9999면 상품 찾을 수 없음
	int m_SB_Count; // 해당 상품에 몇번 응모하게 되었는지를 알려줌(응모성공시)
	int m_Count_gold; // 응답 갯수
	bool m_refreshPopup; // 팝업 새로고침

	// 골드 마일리지
	INT64 m_CurTempGold; //현재 보유한 임시 적립된 골드량이다. 이 필드로 현재 적립머니를 동기화 시키면 됨
	INT64 m_ThisAddTempGold; //이번에 변화되는 골드량이다. 
	int m_ThisAddTempGold_Remain; //이번에 변화되는 골드의 소수점 이하 낙전이다.
	//한게임 종료시 위 두개의 값으로 + 되었다는 UI를 구성하면 됨 (예) Temp.Format("+%I64d.%d", m_ThisAddTempGold, m_ThisAddTempGold_Remain);
	INT64 m_MaxTempGold; //최대로 적립받을 수 있는 금액이다. 이 값과 m_CurTempGold 를 비교하여 최대치 도달등의 UI 구성을 해주면 된다.
	bool m_bOverMoneyGB; // 적립 골드 한도 초과
	
	// 출석부 보상
	CString m_strDayCheck;
	
	// 골드 지원금 정보
	bool m_Playing; //현재 진행중인지 여부
	char *m_StartDay;
	char *m_EndDay;
	
	// 골드 보험 정보 - 인게임에 알림을 띄우기 위한 변수
	DWORD m_dwGoldInsuNotiStartTime; // 인게임 보험 가입 노티 애니메이션 시작 시간
	INT64 m_ChangeChip; // 변동금액.. 항상 양수임에 주의하자~!!
	int m_nInsuLevel; // 0이면 미가입 상태, 위 처리이후 확정된 보험등급

	// 카페 공지
	CString m_strCafeNotice;
	// 카페 미가입 유저의 포인트 적립 정보
	INT64	m_llcafeReservedPoint;

	// 아침벳룰 툴팁 금일 확인 여부
	int m_nMorningCheckDay;
	int m_nMorningBanCount;
	SYSTEMTIME m_tmMorningBanTime;
	CString m_strMorningBetNotice;
	
	int m_nShowFullExplain;
	int m_nShowExplainNum;
	bool m_bNotBanishment;

	CString m_strVersionInfo;


public:
	int m_nAttendanceShowCount; // 추석이벤트 인게임에서 뜨는것 방지용 추후 삭제예정

	// 게임 과몰입 주의 팝업
	bool m_bShowUppPopup;
	int m_nUppWParam[3];
	char *m_cUppLParam;

	// 추석 이벤트 팝업
	bool m_bShowChuSeokPopup;

public:
	CGameManager(void);
	virtual ~CGameManager(void);

	void Release();

	void AddGame(CGame *pGame);
	void SetCurrentGame(IDX_GAME nGame);
	bool CheckLoadGame(void);
	CGame *GetGame(IDX_GAME nGame);

	void ExitSubscript();
	void ExitGame(bool bUserAFK);
	void OnExitRoom(REASON_OUTROOM reason);
	void OnEnterRoom(BOOL bCreation);
	
	inline HCURSOR GetCurSelCursor(){return m_hCurSel;}
	inline HCURSOR GetCurHandCursor(){return m_hCurHand;}
	inline HCURSOR GetCurMoveCursor(){return m_hCurMove;}
	inline HCURSOR GetCurSizeCursor(){return m_hCurSize;}
	inline HCURSOR GetCurWaitCursor(){return m_hCurWait;}
	
	//올인시 계속 진행 선택팝업
	void AskContinueGame();
	
	inline void HideChatClear(void){m_HideChat.Clear();}
	
	void BadUserReportLobbyChat(void);		//대기실 채팅창 신고버튼처리	
	void BadUserReportGameChat(bool bOnlyScam = false);		//게임방 채팅창 신고버튼처리
	void BadUserReportRecevieMemo(MEMO &memo);
	void BadUserReportMemoBox(std::list<MEMO *> &listMemo);
	void BadUserReportMUChat(CWnd *pParent, std::list<std::pair<std::string, std::string> > &listIDNickName, std::string &strChatting);
	void SetMyInfo(CPlayer *pPlayer);
	
	inline CPlayer *GetMyInfo(void){return m_pMyInfo;}
	
	bool IsDiaMemberRooms(void);
	bool IsDiaMemberRooms(__int64 llEnterLimitMoney);
	bool IsChangeFullDDang(void);
	bool IsShowFullExplain(void);
	void LoadShowFullExplain(void);
	void WriteShowFullExplain(void);
	
	inline CGame		*GetCurrentGame(void){return m_pCurGame;}
	inline CGameProcess *GetCurrentProcess(void){if(m_pCurGame == NULL) return NULL; return m_pCurGame->GetGameProcess();}
	inline CGameViewBase *GetCurrentGameView(void){if(m_pCurGame == NULL) return NULL; return m_pCurGame->GetGameView();}
	inline IDX_GAME		&GetCurrentGameType(void){return m_nCurGameType;}
		   const CString GetCurrentGameString(void);
		   const CString GetCurrentGameName(void);		   
		   const int	 GetCurrentGameCode(void);
		   const int	 GetCurrentGameArray(void);
		   const int	 GetIDXToGameArray(IDX_GAME eGameType);
		   const CString GetIDXToGameName(IDX_GAME eGameType);
		   inline const CString GetCurrentGameLoadingBackPath(void)
		   {
			   ASSERT(m_pCurGame); 

			   if(NULL == m_pCurGame)
				   return "";

			   return m_pCurGame->GetLoadingBackPath();
		   }

	inline  CWnd	*GetMainWnd(void){return m_pMainWnd;}
	inline	void SetMainWnd(CWnd *pWnd){m_pMainWnd = pWnd;}
	inline	LRESULT SendMainWndMessage(UINT nID, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		ASSERT(m_pMainWnd != NULL); 
		if(NULL == m_pMainWnd)
			return 0;

		return m_pMainWnd->SendMessage(nID, wParam, lParam);
	}
	inline	LRESULT PostMainWndMessage(UINT nID, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		ASSERT(m_pMainWnd != NULL);
		if(NULL == m_pMainWnd)
			return 0;
		return m_pMainWnd->PostMessage(nID, wParam, lParam);
	}


	inline void SetLog(CString str)
	{
#if _DEBUG
		CString id ;
		id.Format(TEXT("%s.txt"), GetMyInfo()->UI.ID) ;

		CLogFile logfile(id);
		logfile.Write(str);
#endif
	};
	
	//1995 이벤트 달성 여부...
	promotion::_BORNIN1996_ALARM						Get1995EventInfo() {	return m_p1995EventInfo;	}
	void					Set1995EventInfo(promotion::_BORNIN1996_ALARM pInfo )	{ m_p1995EventInfo = pInfo; }

	int						GetEventMissionStep() ;
	void					SetEventMissionStep(int nStep )	{ m_nEventMissionStep = nStep; }

	// 규제안 추가.
	void SetRoomListTabState(int nTabState);
	int  getRoomListTabState()				{	return m_nTabState;	}
	void SetRoomListTabIndex(int nTabIndex)	{	m_nTabIndex = nTabIndex; }
	int  GetRoomListTabIndex()				{	return m_nTabIndex;	}
	

	void SetSelectQuickJoin(bool bSelectQuickJoin)	{	m_bSelectQuickJoin = bSelectQuickJoin; }
	void SetQuickJoinForRandom(bool bQuickJoinForRandom)	{	m_bQuickJoinForRandom = bQuickJoinForRandom; }
	void SetRequestOtherChannel(bool bRequestOtherChannel)	{	m_bRequestOtherChannel = bRequestOtherChannel; }

	// 시작 팝업을 봤는지 여부.
	void SetShowLoginDlg(bool isShow)	{	m_bShowLoginDlg = isShow; }
	bool GetShowLoginDlg()				{	return m_bShowLoginDlg;	}	

	// 일이용시간 제한 시간.
	void AskCurrentPlayTime() ;

	bool IsPlayTimeOver() ;
	
	void SetCurrentPlayTime(int nPlayTimeSec) { m_nPlayTimeSec = nPlayTimeSec; }
	int	GetCurrentPlayTime() { return m_nPlayTimeSec; }

	// 현재 골드 출석부 결과 연출중인지 여부.
	void SetPlayDailyGoldEffect(bool isPlay)	{	m_bPlayDailyGoldEffect = isPlay; }
	bool GetPlayDailyGoldEffect()				{	return m_bPlayDailyGoldEffect;	}

	// 현재 골드 미니게임 결과 연출중인지 여부.
	void SetPlayGoldBigWheel(bool isPlay)	{	m_bPlayGoldBigWheel = isPlay; }
	bool GetPlayGoldBigWheel()				{	return m_bPlayGoldBigWheel;	}	

	void SetWaitOwnLimitPopup(bool isWait)	{	m_bWaitOwnLimitPopup = isWait; }
	bool GetWaitOwnLimitPopup()				{	return m_bWaitOwnLimitPopup;	}

	void SetWaitGoldInsuPopup(bool isWait) { m_bWaitGoldInsuPopup = isWait; }
	bool GetWaitGoldInsuPopup() { return m_bWaitGoldInsuPopup; }

	void SetQuickStartInRoom(bool isStart)	{	m_bQuickStartInRoom = isStart; }
	int  GetQuickStartInRoom()				{	return m_bQuickStartInRoom;	}

	bool IsJoinViewRoom(SMALLROOMINFO* pSRI);
	bool SetLossmoneyInfo(INT64 llLossMoney, time_t tBlockTime, DAILYLOSS_STATE	eState , IDX_GAME gameID) ;
	INT64	GetLossMoney() { return m_llLossMoney; }
	//yoo 2015.01 규제안
	INT64   GetLimitRestMoney(int nGameType);
	INT64   GetLimitLossRestMoney();
	/*INT64	GetLossMaxMoney() { return (INT64)33000000000000; }
	INT64	GetLossMaxMoneyForGame() { return (INT64)9900000000000; }*/
	INT64	GetLossMaxMoney();
	INT64   GetLossMaxMoneyBase();
	INT64	GetLossMaxMoneyForGame();
	/*INT64	GetLossMaxMoney() { return (INT64)3300000000000000; }
	INT64	GetLossMaxMoneyForGame() { return (INT64)15600000000000; }*/

	bool IsCurLimitLossOver(void);
	bool IsLimitOver(void);
	bool IsGetLimitMoney(int nGameType);
	bool IsGetLimitMoney(int nGameType, INT64 llAddMovey);
	bool IsGetLimitOverMoney(INT64 llBetMoney);
	bool IsShowLimitICon(void);
	int GetLevelLimitMoney(int nGameType);
	int GetLimitToCheckMoney(int nGameType, int nMyIndex, INT64 &llBetMoney);
	int GetLimitToBetMoney(int nGameType, int nMyIndex, INT64 llMoney, INT64 &llBetMoney);
	// 현재 손실머니가 기준이 넘은 게임이 있는지..
	void SetLossMoneyCheck(INT64 lossMoney, IDX_GAME gameID) ;
	bool GetLossMoneyCheck() ;
	bool IsOverDay(int nYear, int nMonth, int nDay);
	
	time_t	GetBlockTime() { return m_tBlockTime; }
	DAILYLOSS_STATE	GetLossMoneyState() { return m_eState; }
	IDX_GAME	GetLossMoneyCurGame() { return m_gameID; }
	SYSTEMTIME	GetRewardOverTime() { return m_rewardTime; }
	bool	GetEnableOver()	{ return m_bEnableOver; }

	void SetRestraintInfo(RESTRAINTINFO* RestraintInfo);
	RESTRAINTINFO GetRestraintInfo() { return m_RestraintInfo; }

	void SetClearDateStr(const char* clearDateStr)	{	m_clearDateStr = clearDateStr;	}
	const std::string &GetClearDateStr()			{	return m_clearDateStr;	}
	bool IsCannotPlayUpp()					{	return m_bCannotPlayUpp;		}
	
	//View 변경 관련 함수
	void SetCurrentWhere(IDX_GAMEWHERE nWhere);
	IDX_GAMEWHERE GetCurrentWhere(void);

	//비정상 종료 플래그 관련 함수들
	inline void SetAbnormalOutRoom(bool bAbnormalOutRoom){m_bAbnormalOutRoom = bAbnormalOutRoom;}
	inline bool IsAbnormalOutRoom(void){return m_bAbnormalOutRoom;}

	//응답을 기다리는 중인가를 가지고 있는 플래그 관련 함수들
	inline void SetWaitResponse(bool bWaitResponse){m_bWaitResponse = bWaitResponse;}
	inline bool IsWaitResponse(void){return m_bWaitResponse;}

	//지금 로그아웃 중인가를 가지고 있는 플래그 관련 함수들
	inline void SetNowLogout(bool bNowLogout){m_bNowLogout = bNowLogout;}
	inline bool IsNowLogout(void){return m_bNowLogout;}
	
	//프로필창 요청 위치 관련 함수
	inline void SetProfileOpenKind(ENUM_PROFILE_OPEN_KIND enumKind){m_enumProfileOpenKind = enumKind;}
	ENUM_PROFILE_OPEN_KIND GetProfileOpenKind() {return m_enumProfileOpenKind;}

	void SetCurrentChanNo(int nChanNo) { m_nCurChanNo = nChanNo; }
	int	GetCurrentChanNo() { return m_nCurChanNo; }

	//풀스크린/창모드 변경시 호출되는 함수
		   void OnChangeFullScreen(bool bFullScreen);
	//옵션창을 보여주는 함수.. 옵션창이 닫히면 CGame의OnCloseOptionDlg함수를 호출해준다.
	//옵션창이 닫혔을때 동작을 구현해 주려면 CGame을 상속 받은 클래서에서 OnCloseOptionDlg함수를 오버로드해주면 된다.
		   LRESULT ShowOptionDlg(bool bGameOptionTab = true);

   //현재 설정된 게임의 리소스를 읽어들인다.
		   void LoadSprite(void);
		   void ReLoadSprite(int nSkinType);

	//중복 실행을 검사하기 위한 뮤텍스 관련 함수들
	BOOL CheckMutex(LPCSTR lpAppName);
	void DestroyMutex(void);
	void InitGoGameDirect(void);

	inline int		&GetMaxRoom(void){return m_nMaxRoom;}
	inline int		&GetMaxPlayer(void){return m_nMaxPlayer;}
	inline int		&GetMaxGamePlayer(void){return m_nMaxGamePlayer;}
	inline int		&GetMaxObserver(void){return m_nMaxObserver;}
	inline int		&GetMaxNewPlayer(void){return m_nMaxNewPlayer;}
	inline int		&GetJokboQuestCount(void){return m_nJokboQuestCount;}

	inline INT64	&GetMyGold(void){return m_nMyGold;}
	inline int		&GetGoldRemain(void){return m_nGoldRemain;}
	inline INT64	&GetGoldValue(void){return m_nGoldValue;}

	//게임 윈도우의 크기를 가져오는 함수
	const int GetWidth(void);
	const int GetHeight(void);

	CPlayTimeWarning &GetPlayTimeWarning(void){return m_cPlayTimeWarning;}

	LRESULT ShowCreateRooomDlg(ROOMOPTION *pWebSelectedRoomOption=NULL);
	
	// 선택 시작 창.
	LRESULT	ShowSelectStarterDlg(void);
	LRESULT	ShowResetSelectStarterDlg(void);	

	LRESULT	ShowQuickJoinerDlg(void);
	LRESULT	SendQuickJoiner(void);
	LRESULT	SendSelectQuickJoiner(void);
	LRESULT AskDirectEnterRoom(bool bUseRoomOption, ROOMOPTION *pRoomOption, int nGameMode, int nCalBtnType = 1, bool bSerchEnterRoom = false, bool bSerchAutoEnterRoom = false, bool bForceLocalServer = false);
	LRESULT GoDirectEnterRoom(bool bUseRoomOption, ROOMOPTION *pRoomOption, bool bCheckLoad = false, bool bSecret=false);
	LRESULT AskEnterRoom(int nRoomNum, UINT64 llRoomKey, bool bObserve=false, const char *pRoomPass = NULL);
	LRESULT GoEnterDummyRoom(SMALLROOMINFO *pSI);
	void OnLoadingEnd(void);
	void ShowGameWindow(bool bShow);
	void AddGameInfo(const char *str, COLORREF crNewColor=RGB(0,0,0), bool bUserChat = false);
	void AddGameChat(const char *str, COLORREF crNewColor=RGB(0,0,0), bool bUserChat = false);
	bool SendWhisper(const char *szMessage, const char *szTargetNick, UINT64 llTargetUserKey=-1, const char *szTargetID=NULL);		//귓속말 보내는 함수

	bool IsSerchEnterRoom(void){return m_bSerchEnterRoom;}
	bool IsSerchAutoEnterRoom(void){return m_bSerchAutoEnterRoom;}	
	ROOMOPTION *GetMakeRoomOption(void){return &m_RoomOption;}

	//공용데이터 함수
	CDataRoomList *GetDataRoomList() { return &m_DataRoomList; /*ASSERT(m_pCurGame != NULL); return m_pCurGame->GetDataRoomList();*/}
	CDataUserList *GetDataUserList() { return &m_DataUserList;}
	CDataRoomOptionList *GetDataRoomOptionList() { return &m_DataRoomOptionList; /* ASSERT(m_pCurGame != NULL); return m_pCurGame->GetDataRoomOptionList();*/}
	CPrevRoomInfo *GetPrevRoomInfo() { return &m_PrevRoomInfo; }
	SMALLROOMINFO* FindDataRoomList(int nRoomNum);
	
	void AddHideChatText(CString *str);
	void ClearHideChat();
	CString &GetHideChatData();

	void AppendLoadSprite(NMBASE::GRAPHICGDI::xSprite *pSprite);
	void RemoveLoadSprite(void);

	LONG OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData);
	void OnTimer(UINT nIDEvent);

	static CGameManager& GetInstance();

	//규제안 관련하여 바로시작 실패시에 방생성 함수 2014.01
	void QuickCreateRoom(bool bLimitUser=false);

	//규제안 : 다른방 바로가기 요청(게임중 요청->대기실로 이동->서버에서 SV_ORDER_QUICKSTART_IN_ROOM가 오면 실행)
	void AskByQuickStartOtherRoom(INT64 rkey_prev, ROOMOPTION& ro);

	//규제안 : 다른방 바로가기 요청(게임중 요청->대기실로 이동->위의 바로가기 요청 실패하면 서버에서 SV_REFUSE_QUICKSTART_IN_ROOM가 오면 실행)
	void AskCreateRoomByQuickStartOtherRoom(INT64 llRoomKeyPrev, ROOMOPTION roomOption);

	//규제안 : 선택시작 옵션 저장.
	void SaveSelectQuickStartOption(bool bMustUseOption, INT64 llEnterLimitMoney, int nGameMode, int nGamerule, INT64 llSeedMoney, bool bDiaOption);

	//규제안 :	게임중 다른방 바로가기
	bool		m_quickJoinOtherRoom;

	//규제안 : 맥스 관련하여 최초 1회만 사용하기 위한 플래그
	bool		g_bFirstMax[MAX_PLAYER];
	tPromotionData	*GetPromotionData(void){return &m_tPromotionData;}
	promotion::RETENTION_ALRAM *GetRETENTION_ALRAM(void){ return &m_RETENTION_ALRAM; }
	bool IsEnableComeBackPromotion(void);
	bool IsVIPCouponPromotion(void);
	int GetCouponPromotionImageNum(int nTotalCnt, int &nCount, int &nTotalCount);
	bool IsShowEndEventPopup(void){ return m_bShowEndEventPopup; }
	void SetShowEndEventPopup(bool bShowEndEventPopup){ m_bShowEndEventPopup=bShowEndEventPopup; }

	long long GetRoomOptionLimitMoney(ROOMOPTION* pRoomOption, bool bCreate);
	long long GetRoomOptionSeedMoney(ROOMOPTION* pRoomOption, bool bCreate);

	void SetNotBanishment(bool bNotBanishment){ m_bNotBanishment=bNotBanishment; }
	bool IsNotBanishment(){ return m_bNotBanishment; }
	string GetCurrentTimeString(void);
	bool CheckDiffTime(string sTime);
	
	// 내 클럽 정보
	int GetClubID() { return m_Club_ID; };
	CString GetClubName() { return m_Club_Name; };
	int GetClubMemberGrade() { return m_Club_MemberGrade; };
	bool GetClubEnableCreate() { return m_Club_Enable_Create; };
	float GetMemberAddGetRate() { return m_Member_AddGet_Rate; };
	float GetBossPayBackRate() { return m_Boss_PayBack_Rate; };

	bool GetIsShowingClubDlg()	{	return m_IsShowingClubDlg; }
	void SetIsShowingClubDlg(bool IsShowingClubDlg)	{	m_IsShowingClubDlg = IsShowingClubDlg; }
	
	void SetLobbyFriendTabRequest(bool bLobbyFriendTabRequest) { m_bLobbyFriendTabRequest = bLobbyFriendTabRequest; }
	bool GetLobbyFriendTabRequest() { return m_bLobbyFriendTabRequest; }

	// 응모권으로 응모할 수 있는 황금 복주머니 상품 리스트
	void SetTicketGiftList(TICKET_GIFT_INFO *ArrayTicketGiftList_gold) { m_ArrayTicketGiftList_gold = ArrayTicketGiftList_gold; }
	TICKET_GIFT_INFO* GetTicketGiftList() { return m_ArrayTicketGiftList_gold; }
	// 해당 유저의 티켓 응모 현황
	void SetMyTicketCount(int MyTicketCount) { m_MyTicketCount = MyTicketCount; }
	int GetMyTicketCount() { return m_MyTicketCount; }
	void SetSubscriptionList(TICKET_SUBSCRIPTION_INFO *ArraySubscriptionList_gold) { m_ArraySubscriptionList_gold = ArraySubscriptionList_gold; }
	TICKET_SUBSCRIPTION_INFO* GetSubscriptionList() { return m_ArraySubscriptionList_gold; }
	// 응모 결과
	void SetGiftIndex(int GiftIndex) { m_GiftIndex = GiftIndex; }
	int GetGiftIndex() { return m_GiftIndex; }
	void SetResultCode(int ResultCode) { m_ResultCode = ResultCode; }
	int GetResultCode() { return m_ResultCode; }
	void SetSBCount(int SB_Count) { m_SB_Count = SB_Count; }
	int GetSBCount() { return m_SB_Count; }
	void SetCountGold(int Count_gold) { m_Count_gold = Count_gold; }
	int GetCountGold() { return m_Count_gold; }
	void SetRefreshPopup(bool refreshPopup) { m_refreshPopup = refreshPopup; }
	bool GetRefreshPopup() { return m_refreshPopup; }
	
	// 골드 마일리지
	void SetCurTempGold(INT64 nCurTempGold) { m_CurTempGold = nCurTempGold; } //현재 보유한 임시 적립된 골드량이다. 이 필드로 현재 적립머니를 동기화 시키면 됨
	INT64 GetCurTempGold() { return m_CurTempGold; } //현재 보유한 임시 적립된 골드량이다. 이 필드로 현재 적립머니를 동기화 시키면 됨
	void SetThisAddTempGold(INT64 nThisAddTempGold) { m_ThisAddTempGold = nThisAddTempGold; } //이번에 변화되는 골드량이다. 
	INT64 GetThisAddTempGold() { return m_ThisAddTempGold; } //이번에 변화되는 골드량이다. 
	void SetThisAddTempGold_Remain(int nThisAddTempGold_Remain) { m_ThisAddTempGold_Remain = nThisAddTempGold_Remain; } //이번에 변화되는 골드의 소수점 이하 낙전이다.
	int GetThisAddTempGold_Remain() { return m_ThisAddTempGold_Remain; } //이번에 변화되는 골드의 소수점 이하 낙전이다.
	//한게임 종료시 위 두개의 값으로 + 되었다는 UI를 구성하면 됨 (예) Temp.Format("+%I64d.%d", m_ThisAddTempGold, m_ThisAddTempGold_Remain);
	void SetMaxTempGold(INT64 nMaxTempGold) { m_MaxTempGold = nMaxTempGold; } //최대로 적립받을 수 있는 금액이다. 이 값과 m_CurTempGold 를 비교하여 최대치 도달등의 UI 구성을 해주면 된다.
	INT64 GetMaxTempGold() { return m_MaxTempGold; } //최대로 적립받을 수 있는 금액이다. 이 값과 m_CurTempGold 를 비교하여 최대치 도달등의 UI 구성을 해주면 된다.
	void SetOverMoneyGB(bool bOver) { m_bOverMoneyGB = bOver; } // 적립 골드 한도 초과
	bool GetOverMoneyGB() { return m_bOverMoneyGB; } // 적립 골드 한도 초과

	// 골드 보험 정보 - 인게임에 알림을 띄우기 위한 변수
	void SetGoldInsuNotiStartTime(DWORD t) { m_dwGoldInsuNotiStartTime = t; }
	DWORD GetGoldInsuNotiStartTime() { return m_dwGoldInsuNotiStartTime; }
	void SetGoldInsuChangeChip(INT64 chip) { m_ChangeChip = chip; }
	INT64 GetGoldInsuChangeChip() { return m_ChangeChip; }
	void SetInsuLevel(int lv) { m_nInsuLevel = lv; }
	DWORD GetInsuLevel() { return m_nInsuLevel; }

	void SetPlaying(bool Playing) { m_Playing = Playing; }
	bool GetPlaying() { return m_Playing; }
	void SetStartDay(char* StartDay) { m_StartDay = StartDay; }
	char* GetStartDay() { return m_StartDay; }
	void SetEndDay(char* EndDay) { m_EndDay = EndDay; }
	char* GetEndDay() { return m_EndDay; }

	CString GetCafeNotice() { return m_strCafeNotice; }
	void SetCafeNotice(CString strCafeNotice) { m_strCafeNotice = strCafeNotice; }
	
	INT64 GetCafeReservedPoint() { return m_llcafeReservedPoint; }
	void SetCafeReservedPoint(INT64 cafeReservedPoint) { m_llcafeReservedPoint = cafeReservedPoint; }

	int GetMorningCheckDay() { return m_nMorningCheckDay; }
	void SetMorningCheckDay(int nMorningCheckDay) { m_nMorningCheckDay = nMorningCheckDay; }
	
	int GetMorningBanCount() { return m_nMorningBanCount; }
	void SetMorningBanCount(int nMorningBanCount) { m_nMorningBanCount = nMorningBanCount; }
	SYSTEMTIME GetMorningBanTime() { return m_tmMorningBanTime; }
	void SetMorningBanTime(SYSTEMTIME& tmMorningBanTime) { m_tmMorningBanTime = tmMorningBanTime; }
	bool GetMorningBet() { return m_bMorningBet; }
	void SetMorningBet(bool bMorningBet) { m_bMorningBet = bMorningBet; }	

	CString GetMorningBetNotice() { return m_strMorningBetNotice; }
	void SetMorningBetNotice(CString strMorningBetNotice) { m_strMorningBetNotice = strMorningBetNotice; }
	
	void SetVersionInfo(CString strVersionInfo) { m_strVersionInfo = strVersionInfo; }
	CString  GetVersionInfo() { return m_strVersionInfo; }
	
protected:
	LRESULT AskForceDirectEnterRoom( bool bUseRoomOption, ROOMOPTION *pRoomOption, int nCalBtnType , bool bForceLocalServer = false);
	LRESULT ForceCreateRooom(void);

	void BadUserReport(const CBadUserDlg &dlg, const std::string &strMessage, CString strSaveName );

	//규제안 : 룸옵션 추가 : 랜덤입장용
	ROOMOPTION	m_RoomOptionForRefuse;
public:

	//webcall
	CWebCallThread m_cWebCallThread;
	LRESULT OnEndWebCall(WPARAM &wParam, LPARAM &lParam);

	void OnEndGame(void);
	int GetTodayFrom2019();
	int GetMinuteOfDay();
	bool GetAcceptMorningCheck();
};

UPG_COMMON_EXPORT CGameManager& GM();