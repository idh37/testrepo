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
//2018.04.30 8250�� ������ �������� ���� ����.
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
	bool	m_bUseQuickOption;		// �ٷ����� ���� ��� ����

	bool	m_bUseGameMode;			// ���Ӹ�� ��� ����

	int		m_nGameMode;			// ���Ӹ��

	int		m_nRuleType;			// �Ƕ��� ��Ÿ��(����0n/off)

	bool	m_bUseBetRule;			// ���÷� ��뿩��

	bool	m_bAutoBet;				// �ڵ�����

	bool	m_bUseSeedMoney;		// ���ӱԸ� ��� ����

	bool	m_bMaxPlayer;			// ���� ���� ���� ��뿩��

	int		m_nSeedType;			// ���ӱԸ�(0: ū��, 1: �Ϲݹ�, 2:������)

	bool	m_bAutoEnterRoom;		// �ڵ����� ����

	short	m_sBetRuleKind;			//����Ʈ�Ӵ�

	int		m_nCardSeting;			//ī�弼�� 4~6��

	float	m_nBetMulti;			//���� ������

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

	// ��ð� ���� �÷��� ���
	CPlayTimeWarning		m_cPlayTimeWarning;

	int						m_nMaxRoom;
	int						m_nMaxPlayer;
	int						m_nMaxGamePlayer;
	int						m_nMaxObserver;
	int						m_nMaxNewPlayer;
	int						m_nJokboQuestCount;
	IDX_GAMEWHERE			m_nWhere;
	bool					m_bAbnormalOutRoom;				// ������������ ����ؾ��ϴ°�?
	bool					m_bWaitResponse;				// ������ ��ٸ��� ���ΰ�?
	bool					m_bNowLogout;					// ���� �α׾ƿ����ΰ�?

	CWnd					*m_pMainWnd;
	CPlayer					*m_pMyInfo;
	INT64					m_nMyGold;						//��� ��ȭ �߰�.
	int						m_nGoldRemain;					//��� ��ȭ �Ҽ���.
	INT64					m_nGoldValue;					//��� ��ȭ ��ġ.
	ROOMINFO				m_stRoomInfo;					//����� �ɼ�
	bool					m_bDeafultCreateRoom;
	ENUM_GAMELODING_TYPE	m_nLoadingType;
	int						m_nGoDirectEnterType;
	
	bool					m_bShowLoginDlg;				// ��������� �α��� �˾��� ������.
	
	//������ �ɼ� (�����)
	int						m_nRoomNum_for_Join;
	UINT64					m_llRoomKey_for_Join;
	bool					m_bObserve_for_Join;
	CString					m_strRoomPass_for_Join;

	// ������ �ɼ� ������ �߰� ( �Ϲ�, VIP �� �ΰ� �������ΰ��� ���� ) // 0:����  1:�Ϲ�	2:���
	int						m_nTabState;
	// �� Index ���� VIP, NORMAL, VIP_GOLD, NORMAL_GOLD, FREE
	int						m_nTabIndex;

	//�ٷΰ��� ���� �����
	bool					m_bUseRoomOption;
	ROOMOPTION				m_RoomOption;
	bool					m_bMorningBet;
	

	HCURSOR					m_hCurSel, m_hCurHand, m_hCurMove, m_hCurSize, m_hCurWait;


	int						m_nCurChanNo;		//���� ������ ����ä�ι�ȣ

	bool					m_bSerchEnterRoom;		//�ٷ������ ���� ã�Ƽ� �����ߴ��� ����
	bool					m_bSerchAutoEnterRoom;	//�ٷ�������н� �ڵ����� �⺻������ ���������� ����
	int						m_nDirectEnterGameMode;	//�ٷ������ ���� ������ ����, �Ϲ�/���̽�/��ź/��Ŀ
	int						m_bForceLocalServer;

	// ���̿�ð� ���� �ð�.
	int						m_nPlayTimeSec ;

	//��� �⼮�� ��� ����Ʈ ���� ����.
	bool					m_bPlayDailyGoldEffect;
	//��� �̴ϰ��� ��� ����Ʈ ���� ����.
	bool					m_bPlayGoldBigWheel;

	//�ѵ� �ʰ� �˾� ������.
	bool					m_bWaitOwnLimitPopup;
	
	// ��� ���� �˾� ������.
	bool m_bWaitGoldInsuPopup;

	// (����/ä�� �̵���) �ٸ��� �ٷΰ������� ����
	bool					m_bQuickStartInRoom;		

	// ������ : ���� �սǸӴ� ����.
	INT64			m_llLossMoney;		// ȹ��/�ս� �ݾ� 
	time_t			m_tBlockTime;		// ���ܵ� �ð�
	DAILYLOSS_STATE	m_eState;			// �ս� ����
	IDX_GAME		m_gameID;			//���� ���̵�
	INT64           m_llLimitLoseMoney[6];   //���Ӻ� �սǸӴ�
	bool			m_bReceiveLimitMoney[6];

	bool			m_CheckLossMoneyOver[6]; // ������ 6�� ������, index�� ����Ŀ�� �־ 7�� ����.

	std::string		m_clearDateStr;	
	bool			m_bCannotPlayUpp;
	SYSTEMTIME		m_rewardTime;
	bool			m_bEnableOver;

	ENUM_PROFILE_OPEN_KIND	m_enumProfileOpenKind;	//������ â ��ﶧ ����κп��� ���� ��û�ߴ��� ����

	int	m_nEventMissionStep ;
	promotion::_BORNIN1996_ALARM	m_p1995EventInfo ;
	bool m_bShowEndEventPopup;

	///////////////////
	//���뵥����	
	CDataUserList			m_DataUserList;
	CHideChatList			m_HideChat;
	//���ӹ渮��Ʈ
	CDataRoomList			m_DataRoomList;
	//�Ӵϱ�������(ä������)
	CDataRoomOptionList		m_DataRoomOptionList;
	//���������� ��������
	CPrevRoomInfo			m_PrevRoomInfo;

	RESTRAINTINFO m_RestraintInfo;

	bool		m_bSelectQuickJoin; //���� ������ : ���ý������� �� ���� ��û.
	bool		m_bQuickJoinForRandom; //������ : �󼼱������� ���������� ���� ��� 
	bool		m_bRequestOtherChannel; //������ : ���� ������ ���� �ٸ� ä���� ���

	// �� Ŭ�� ����
	int			m_Club_ID;
	CString		m_Club_Name;
	int			m_Club_MemberGrade; 
	bool		m_Club_Enable_Create;
	float		m_Member_AddGet_Rate;
	float		m_Boss_PayBack_Rate;
	CString		m_Club_Msg;

	bool		m_IsShowingClubDlg;
	bool		m_bLobbyFriendTabRequest;
	
	// ���� ���ۿ� ������.
	bool m_bSQJMustUseOption;
	INT64 m_llSQJEnterLimitMoney; 
	int m_nSQJGameMode; 
	int m_nSQJGamerule; 
	INT64 m_SQJllSeedMoney;
	bool m_bDiaOption;
	tPromotionData m_tPromotionData;
	promotion::RETENTION_ALRAM m_RETENTION_ALRAM;
	
	// ��������� ������ �� �ִ� Ȳ�� ���ָӴ� ��ǰ ����Ʈ
	TICKET_GIFT_INFO *m_ArrayTicketGiftList_gold; //��ϵ� ��ǰ ����
	
	// �ش� ������ Ƽ�� ���� ��Ȳ
	int m_MyTicketCount; //������ �ִ� �����(Ƽ��) ���� == ���� Ȳ�� ���ָӴ� ����
	TICKET_SUBSCRIPTION_INFO *m_ArraySubscriptionList_gold; //��ϵ� ���� ����
	
	// ���� ���
	int m_GiftIndex; // ��ǰ ������ȣ
	int m_ResultCode; // 0�̸� ���� ����, 1000�̸� ����� ����, 9999�� ��ǰ ã�� �� ����
	int m_SB_Count; // �ش� ��ǰ�� ��� �����ϰ� �Ǿ������� �˷���(���𼺰���)
	int m_Count_gold; // ���� ����
	bool m_refreshPopup; // �˾� ���ΰ�ħ

	// ��� ���ϸ���
	INT64 m_CurTempGold; //���� ������ �ӽ� ������ ��差�̴�. �� �ʵ�� ���� �����Ӵϸ� ����ȭ ��Ű�� ��
	INT64 m_ThisAddTempGold; //�̹��� ��ȭ�Ǵ� ��差�̴�. 
	int m_ThisAddTempGold_Remain; //�̹��� ��ȭ�Ǵ� ����� �Ҽ��� ���� �����̴�.
	//�Ѱ��� ����� �� �ΰ��� ������ + �Ǿ��ٴ� UI�� �����ϸ� �� (��) Temp.Format("+%I64d.%d", m_ThisAddTempGold, m_ThisAddTempGold_Remain);
	INT64 m_MaxTempGold; //�ִ�� �������� �� �ִ� �ݾ��̴�. �� ���� m_CurTempGold �� ���Ͽ� �ִ�ġ ���޵��� UI ������ ���ָ� �ȴ�.
	bool m_bOverMoneyGB; // ���� ��� �ѵ� �ʰ�
	
	// �⼮�� ����
	CString m_strDayCheck;
	
	// ��� ������ ����
	bool m_Playing; //���� ���������� ����
	char *m_StartDay;
	char *m_EndDay;
	
	// ��� ���� ���� - �ΰ��ӿ� �˸��� ���� ���� ����
	DWORD m_dwGoldInsuNotiStartTime; // �ΰ��� ���� ���� ��Ƽ �ִϸ��̼� ���� �ð�
	INT64 m_ChangeChip; // �����ݾ�.. �׻� ����ӿ� ��������~!!
	int m_nInsuLevel; // 0�̸� �̰��� ����, �� ó������ Ȯ���� ������

	// ī�� ����
	CString m_strCafeNotice;
	// ī�� �̰��� ������ ����Ʈ ���� ����
	INT64	m_llcafeReservedPoint;

	// ��ħ���� ���� ���� Ȯ�� ����
	int m_nMorningCheckDay;
	int m_nMorningBanCount;
	SYSTEMTIME m_tmMorningBanTime;
	CString m_strMorningBetNotice;
	
	int m_nShowFullExplain;
	int m_nShowExplainNum;
	bool m_bNotBanishment;

	CString m_strVersionInfo;


public:
	int m_nAttendanceShowCount; // �߼��̺�Ʈ �ΰ��ӿ��� �ߴ°� ������ ���� ��������

	// ���� ������ ���� �˾�
	bool m_bShowUppPopup;
	int m_nUppWParam[3];
	char *m_cUppLParam;

	// �߼� �̺�Ʈ �˾�
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
	
	//���ν� ��� ���� �����˾�
	void AskContinueGame();
	
	inline void HideChatClear(void){m_HideChat.Clear();}
	
	void BadUserReportLobbyChat(void);		//���� ä��â �Ű��ưó��	
	void BadUserReportGameChat(bool bOnlyScam = false);		//���ӹ� ä��â �Ű��ưó��
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
	
	//1995 �̺�Ʈ �޼� ����...
	promotion::_BORNIN1996_ALARM						Get1995EventInfo() {	return m_p1995EventInfo;	}
	void					Set1995EventInfo(promotion::_BORNIN1996_ALARM pInfo )	{ m_p1995EventInfo = pInfo; }

	int						GetEventMissionStep() ;
	void					SetEventMissionStep(int nStep )	{ m_nEventMissionStep = nStep; }

	// ������ �߰�.
	void SetRoomListTabState(int nTabState);
	int  getRoomListTabState()				{	return m_nTabState;	}
	void SetRoomListTabIndex(int nTabIndex)	{	m_nTabIndex = nTabIndex; }
	int  GetRoomListTabIndex()				{	return m_nTabIndex;	}
	

	void SetSelectQuickJoin(bool bSelectQuickJoin)	{	m_bSelectQuickJoin = bSelectQuickJoin; }
	void SetQuickJoinForRandom(bool bQuickJoinForRandom)	{	m_bQuickJoinForRandom = bQuickJoinForRandom; }
	void SetRequestOtherChannel(bool bRequestOtherChannel)	{	m_bRequestOtherChannel = bRequestOtherChannel; }

	// ���� �˾��� �ô��� ����.
	void SetShowLoginDlg(bool isShow)	{	m_bShowLoginDlg = isShow; }
	bool GetShowLoginDlg()				{	return m_bShowLoginDlg;	}	

	// ���̿�ð� ���� �ð�.
	void AskCurrentPlayTime() ;

	bool IsPlayTimeOver() ;
	
	void SetCurrentPlayTime(int nPlayTimeSec) { m_nPlayTimeSec = nPlayTimeSec; }
	int	GetCurrentPlayTime() { return m_nPlayTimeSec; }

	// ���� ��� �⼮�� ��� ���������� ����.
	void SetPlayDailyGoldEffect(bool isPlay)	{	m_bPlayDailyGoldEffect = isPlay; }
	bool GetPlayDailyGoldEffect()				{	return m_bPlayDailyGoldEffect;	}

	// ���� ��� �̴ϰ��� ��� ���������� ����.
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
	//yoo 2015.01 ������
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
	// ���� �սǸӴϰ� ������ ���� ������ �ִ���..
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
	
	//View ���� ���� �Լ�
	void SetCurrentWhere(IDX_GAMEWHERE nWhere);
	IDX_GAMEWHERE GetCurrentWhere(void);

	//������ ���� �÷��� ���� �Լ���
	inline void SetAbnormalOutRoom(bool bAbnormalOutRoom){m_bAbnormalOutRoom = bAbnormalOutRoom;}
	inline bool IsAbnormalOutRoom(void){return m_bAbnormalOutRoom;}

	//������ ��ٸ��� ���ΰ��� ������ �ִ� �÷��� ���� �Լ���
	inline void SetWaitResponse(bool bWaitResponse){m_bWaitResponse = bWaitResponse;}
	inline bool IsWaitResponse(void){return m_bWaitResponse;}

	//���� �α׾ƿ� ���ΰ��� ������ �ִ� �÷��� ���� �Լ���
	inline void SetNowLogout(bool bNowLogout){m_bNowLogout = bNowLogout;}
	inline bool IsNowLogout(void){return m_bNowLogout;}
	
	//������â ��û ��ġ ���� �Լ�
	inline void SetProfileOpenKind(ENUM_PROFILE_OPEN_KIND enumKind){m_enumProfileOpenKind = enumKind;}
	ENUM_PROFILE_OPEN_KIND GetProfileOpenKind() {return m_enumProfileOpenKind;}

	void SetCurrentChanNo(int nChanNo) { m_nCurChanNo = nChanNo; }
	int	GetCurrentChanNo() { return m_nCurChanNo; }

	//Ǯ��ũ��/â��� ����� ȣ��Ǵ� �Լ�
		   void OnChangeFullScreen(bool bFullScreen);
	//�ɼ�â�� �����ִ� �Լ�.. �ɼ�â�� ������ CGame��OnCloseOptionDlg�Լ��� ȣ�����ش�.
	//�ɼ�â�� �������� ������ ������ �ַ��� CGame�� ��� ���� Ŭ�������� OnCloseOptionDlg�Լ��� �����ε����ָ� �ȴ�.
		   LRESULT ShowOptionDlg(bool bGameOptionTab = true);

   //���� ������ ������ ���ҽ��� �о���δ�.
		   void LoadSprite(void);
		   void ReLoadSprite(int nSkinType);

	//�ߺ� ������ �˻��ϱ� ���� ���ؽ� ���� �Լ���
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

	//���� �������� ũ�⸦ �������� �Լ�
	const int GetWidth(void);
	const int GetHeight(void);

	CPlayTimeWarning &GetPlayTimeWarning(void){return m_cPlayTimeWarning;}

	LRESULT ShowCreateRooomDlg(ROOMOPTION *pWebSelectedRoomOption=NULL);
	
	// ���� ���� â.
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
	bool SendWhisper(const char *szMessage, const char *szTargetNick, UINT64 llTargetUserKey=-1, const char *szTargetID=NULL);		//�ӼӸ� ������ �Լ�

	bool IsSerchEnterRoom(void){return m_bSerchEnterRoom;}
	bool IsSerchAutoEnterRoom(void){return m_bSerchAutoEnterRoom;}	
	ROOMOPTION *GetMakeRoomOption(void){return &m_RoomOption;}

	//���뵥���� �Լ�
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

	//������ �����Ͽ� �ٷν��� ���нÿ� ����� �Լ� 2014.01
	void QuickCreateRoom(bool bLimitUser=false);

	//������ : �ٸ��� �ٷΰ��� ��û(������ ��û->���Ƿ� �̵�->�������� SV_ORDER_QUICKSTART_IN_ROOM�� ���� ����)
	void AskByQuickStartOtherRoom(INT64 rkey_prev, ROOMOPTION& ro);

	//������ : �ٸ��� �ٷΰ��� ��û(������ ��û->���Ƿ� �̵�->���� �ٷΰ��� ��û �����ϸ� �������� SV_REFUSE_QUICKSTART_IN_ROOM�� ���� ����)
	void AskCreateRoomByQuickStartOtherRoom(INT64 llRoomKeyPrev, ROOMOPTION roomOption);

	//������ : ���ý��� �ɼ� ����.
	void SaveSelectQuickStartOption(bool bMustUseOption, INT64 llEnterLimitMoney, int nGameMode, int nGamerule, INT64 llSeedMoney, bool bDiaOption);

	//������ :	������ �ٸ��� �ٷΰ���
	bool		m_quickJoinOtherRoom;

	//������ : �ƽ� �����Ͽ� ���� 1ȸ�� ����ϱ� ���� �÷���
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
	
	// �� Ŭ�� ����
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

	// ��������� ������ �� �ִ� Ȳ�� ���ָӴ� ��ǰ ����Ʈ
	void SetTicketGiftList(TICKET_GIFT_INFO *ArrayTicketGiftList_gold) { m_ArrayTicketGiftList_gold = ArrayTicketGiftList_gold; }
	TICKET_GIFT_INFO* GetTicketGiftList() { return m_ArrayTicketGiftList_gold; }
	// �ش� ������ Ƽ�� ���� ��Ȳ
	void SetMyTicketCount(int MyTicketCount) { m_MyTicketCount = MyTicketCount; }
	int GetMyTicketCount() { return m_MyTicketCount; }
	void SetSubscriptionList(TICKET_SUBSCRIPTION_INFO *ArraySubscriptionList_gold) { m_ArraySubscriptionList_gold = ArraySubscriptionList_gold; }
	TICKET_SUBSCRIPTION_INFO* GetSubscriptionList() { return m_ArraySubscriptionList_gold; }
	// ���� ���
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
	
	// ��� ���ϸ���
	void SetCurTempGold(INT64 nCurTempGold) { m_CurTempGold = nCurTempGold; } //���� ������ �ӽ� ������ ��差�̴�. �� �ʵ�� ���� �����Ӵϸ� ����ȭ ��Ű�� ��
	INT64 GetCurTempGold() { return m_CurTempGold; } //���� ������ �ӽ� ������ ��差�̴�. �� �ʵ�� ���� �����Ӵϸ� ����ȭ ��Ű�� ��
	void SetThisAddTempGold(INT64 nThisAddTempGold) { m_ThisAddTempGold = nThisAddTempGold; } //�̹��� ��ȭ�Ǵ� ��差�̴�. 
	INT64 GetThisAddTempGold() { return m_ThisAddTempGold; } //�̹��� ��ȭ�Ǵ� ��差�̴�. 
	void SetThisAddTempGold_Remain(int nThisAddTempGold_Remain) { m_ThisAddTempGold_Remain = nThisAddTempGold_Remain; } //�̹��� ��ȭ�Ǵ� ����� �Ҽ��� ���� �����̴�.
	int GetThisAddTempGold_Remain() { return m_ThisAddTempGold_Remain; } //�̹��� ��ȭ�Ǵ� ����� �Ҽ��� ���� �����̴�.
	//�Ѱ��� ����� �� �ΰ��� ������ + �Ǿ��ٴ� UI�� �����ϸ� �� (��) Temp.Format("+%I64d.%d", m_ThisAddTempGold, m_ThisAddTempGold_Remain);
	void SetMaxTempGold(INT64 nMaxTempGold) { m_MaxTempGold = nMaxTempGold; } //�ִ�� �������� �� �ִ� �ݾ��̴�. �� ���� m_CurTempGold �� ���Ͽ� �ִ�ġ ���޵��� UI ������ ���ָ� �ȴ�.
	INT64 GetMaxTempGold() { return m_MaxTempGold; } //�ִ�� �������� �� �ִ� �ݾ��̴�. �� ���� m_CurTempGold �� ���Ͽ� �ִ�ġ ���޵��� UI ������ ���ָ� �ȴ�.
	void SetOverMoneyGB(bool bOver) { m_bOverMoneyGB = bOver; } // ���� ��� �ѵ� �ʰ�
	bool GetOverMoneyGB() { return m_bOverMoneyGB; } // ���� ��� �ѵ� �ʰ�

	// ��� ���� ���� - �ΰ��ӿ� �˸��� ���� ���� ����
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

	//������ : ��ɼ� �߰� : ���������
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