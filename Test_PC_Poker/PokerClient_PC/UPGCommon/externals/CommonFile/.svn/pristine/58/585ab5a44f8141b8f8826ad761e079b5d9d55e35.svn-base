#pragma once

// TMS Protocol
#include "./CommMsg.h"
#include "./CommMsgIDHeader.h"


#define TIME_LEN	(20)
#define CN_LEN		(16)

#define MAX_TOURNAMENT_LEVEL	(30)


enum TournamentError
{
	TOURNAMENT_SUCCESS = 0,
	TOURNAMENT_ERROR_PLAYTIME_LIMIT = 1,
	TOURNAMENT_ERROR_DAILYLOSS_BLOCKED = 2,
	TOURNAMENT_ERROR_DAILYLOSS_WARNING = 3,

	// 여기서 부터 토너먼트 서버의 에러 코드 (Tournament.BS.Error.h)
	TOURNAMENT_ERROR_FAIL = 1703936,
	TOURNAMENT_ERROR_SYSTEM = 1703937,
	TOURNAMENT_ERROR_CONFIG = 1703938,
	TOURNAMENT_ERROR_INVALID_HANDLE = 1703939,
	TOURNAMENT_ERROR_NOT_ENOUGH_MEMORY = 1703940,
	TOURNAMENT_ERROR_INVALID_PARAM = 1703941,
	TOURNAMENT_ERROR_NOT_INIT = 1703942,
	TOURNAMENT_ERROR_INVALID_STATE = 1703943,
	TOURNAMENT_ERROR_TIMEOUT = 1703944,
	TOURNAMENT_ERROR_NOT_AFFECTED = 1703945,
	TOURNAMENT_ERROR_NOTFOUND_SESSIONINFO = 1703946,
	TOURNAMENT_ERROR_STARTED = 1704192,
	TOURNAMENT_ERROR_ENDED = 1704193,
	TOURNAMENT_ERROR_CLOSEDESK = 1704194,
	TOURNAMENT_ERROR_INVALID_TOURNAMENTINDEX = 1704195,
	TOURNAMENT_ERROR_INVALID_GAMECODE = 1704196,
	TOURNAMENT_ERROR_TIMEOVER_ORD = 1704197,
	TOURNAMENT_ERROR_ELIMINATED_USER = 1704198,
	TOURNAMENT_ERROR_WAIT_TOURNAMENT = 1704199,
	TOURNAMENT_ERROR_REGISTED_OTHER_TOURNAMENT = 1704200,
	TOURNAMENT_ERROR_INVALID_ROOMID = 1704448,
	TOURNAMENT_ERROR_NOTFOUND_ROOM = 1704449,
	TOURNAMENT_ERROR_EIXT_USER = 1704450,
	TOURNAMENT_ERROR_NOTFOUND_USER = 1704451,
	TOURNAMENT_ERROR_RESERVED_LEAVE = 1704452,
	TOURNAMENT_ERROR_RESERVED_ENTER = 1704453,
	TOURNAMENT_ERROR_FAIL_CREATEROOM = 1704454,
	TOURNAMENT_ERROR_NOTEXIST_RESERVEENTERLIST = 1704455,
	TOURNAMENT_ERROR_NOTEXIST_RESERVELEAVELIST = 1704456,
	TOURNAMENT_ERROR_NOTEXIST_USERLEAVELIST = 1704457,
	TOURNAMENT_ERROR_ROOM_FULL = 1704458,
	TOURNAMENT_ERROR_INVALID_USERID = 1704704,
	TOURNAMENT_ERROR_REGISTED_USER = 1704706,
	TOURNAMENT_ERROR_NOTREGISTED_USER = 1704705,
	TOURNAMENT_ERROR_INVALID_USERSTATE = 1704708,
	TOURNAMENT_ERROR_SHORT_ENTRYFEE = 1704960,
	TOURNAMENT_ERROR_BOUGHT_TOURNAMENT = 1704961,
	TOURNAMENT_ERROR_EMPTY_TICKET = 1704962,
	TOURNAMENT_ERROR_INVALID_TICKETINFO = 1704963,
	TOURNAMENT_ERROR_BUYTICKET_FAILED = 1704964,
	TOURNAMENT_ERROR_FAIL_CREATERANKINGNODE = 1705216,
	TOURNAMENT_ERROR_SHORTAGE_LIST = 1705217,
	TOURNAMENT_ERROR_NOTFOUND_DATA = 1705472,
	TOURNAMENT_ERROR_RD_FAIL_GETCONNECTOR = 1705728,
	TOURNAMENT_ERROR_RD_FAIL_CREATECONNECTOR = 1705729,
	TOURNAMENT_ERROR_RD_NULL_REPLY = 1705730,
	TOURNAMENT_ERROR_RD_ERROR_TYPE = 1705731,
	TOURNAMENT_ERROR_RD_INVALID_TYPE = 1705732,
	TOURNAMENT_ERROR_RD_INVALID_DATA = 1705733,
	TOURNAMENT_ERROR_RD_NULL_DATA = 1705734,
	MAX_TOURNAMENT_ERROR = 1706240,
};

enum TournamentState
{
	TOURNAMENTSTATE_WAIT = 0,			// 대기중
	TOURNAMENTSTATE_OPENDESK = 1,		// 티켓 판매
	TOURNAMENTSTATE_REGIST = 2,			// 참가 중
	TOURNAMENTSTATE_PLAY = 3,			// 플레이중
	TOURNAMENTSTATE_CLOSEDESK = 4,		// 참가 종료
	TOURNAMENTSTATE_CLOSEREBUYIN = 5,	// 리바이인 종료
	TOURNAMENTSTATE_GAMEOVER = 6,		// 토너먼트 종료
	TOURNAMENTSTATE_CANCELGAME = 7,		// 정원 미달로 토너먼트 취소
	MAX_TOURNAMENTSTATE = 8,
};

enum TournamentRankingState
{
	RANKINGSTATE_TOURNAMENT = 0,
	RANKINGSTATE_DAILY = 1,
	RANKINGSTATE_WEEKELY = 2,
	RANKINGSTATE_MONTHLY = 3,
	MAX_RANKINGSTATE = 4,
};

enum TournamentGameState
{
	GAMESTATE_NEWPOKER = 0,
	GAMESTATE_LOWBADUKI = 1,
	GAMESTATE_SUTDA = 2,
	GAMESTATE_ALL = 3,
	MAX_GAMESTATE = 4,
};

enum LeaveRoomState
{
	LEAVEROOMSTATE_ALLIN = 0,			// 올인으로 인한 방 퇴장
	LEAVEROOMSTATE_MERGE = 1,			// 머지로 인한 방 퇴장
	LEAVEROOMSTATE_BREAK = 2,			// 휴식으로 인한 방 퇴장
	LEAVEROOMSTATE_TIMEOVER = 3,		// 토너먼트 시간 종료로 인한 방 퇴장
	LEAVEROOMSTATE_CANCELGAME = 4,		// 토너먼트 경기 취소로 인한 방 퇴장
	MAX_LEAVEROOMSTATE = 5,
};

enum NotificationState
{
	NOTIFICATIONSTATE_FIRST = 0,
	NOTIFICATIONSTATE_SECOND = 1,
	NOTIFICATIONSTATE_THIRD = 2,
	NOTIFICATIONSTATE_FOURTH = 3,
	MAX_NOTIFICATIONSTATE,
};


struct _TournamentInfo
{
	int tournamentIndex;
	char startDate[TIME_LEN];	// 토너먼트 시작 시간
	int startRegist;			// 토너먼트 접수 시작
	int maxReBuyInMulti;		// 최대 리바이인 배수
	__int64 reBuyInMoney;		// 리바이인 금액
	int entryFee;				// 참가비 수수료
	int maxEntryMulti;			// 최대 참가비 배수
	__int64 entryMoney;			// 참가비 무료면 0
	int tournamentType;			// 토너먼트 종류 0 : 무료, 1 : 게임머니, 2 : 티켓, 3 : 게임머니 & 티켓
	int firstNoti;				// 1차 공지 시간
	int thirdNoti;				// 2차 공지 시간
	int secondNoti;				// 3차 공지 시간
	int fourthNoti;				// 4차 공지 시간
	__int64 gameMoney;			// 참가자 시작 머니
	__int64 seedMoney;			// 시작 시드 머니
	int breakCnt;				// 휴식 횟수
	int breakTerm;				// 후식 시간
	int levelTerm;				// 레벨 유지 시간
	int maxReBuyInCnt;			// 최대 리바이인 횟수
	int maxUserCnt;				// 최대 접속 유저
	int stopEntryLevel;			// 신청 마감 레벨
	int gameCode;				// TournamentGameState
	bool activeState;			// 리스트 활성화, (0:false 미노출, 1:true 노출), false 가 되는 순간 timeNode 는 모두 제거
	int reBuyInTerm;			// 리바이인 팝업 노출 시간
	int maxReBuyInLevel;		// 리바이인 가능 레벨
	__int64 guarantee;			// 개런티 머니
	int todayTournamentIndex;
	char updateDate[TIME_LEN];	// 최종 수정 날짜, (수정 날짜가 DB와 서버가 다르면 서버를 업데이트)
	TournamentState tournamentState;	// 토너먼트 상태
	__int64 seedMoneyInfo[MAX_TOURNAMENT_LEVEL];	// 토너먼트 레벨별 시드머니
	bool bRegist;				// 클라에서 사용
};

struct _TournamentLevel
{
	int currentLevel;			// 현재 레벨
	__int64 seedMoney;			// 시드 머니
	char startDate[TIME_LEN];	// 현재 레벨 시작 시간
	__int64 playTime;			// 현재 레벨 진행 시간
};

struct _TournamentSituation
{
	int totalSellTicketCnt;		// 티켓 구매수
	int totalRegisterCnt;		// 입장 수
	int totalEliminatedCnt;		// 참가자
	int totalReBuyInCnt;		// 리바이인 횟수
	__int64 totalPrizeMoney;	// 총 상금
};

struct _TournamentPlay
{
	int tournamentIndex;
	_TournamentLevel level;
	_TournamentSituation situation;
};

struct _TournamentTicket
{
	int tournamentIndex;
	__int64 ticketPrice;	// 티켓 가격
	int multi;				// 티켓 구매 배수
	__int64 GameMoney;		// 지급 게임 머니
};

struct MyTournament
{
	int tournamentIndex;
	bool used;				// 사용 유무
};

struct _CheckReBuyInInfo
{
	__int64 reBuyInMoney;		// 리바이인에 필요한 금액
	int reBuyInCnt;				// 리바이인 횟수
	int maxReBuyInCnt;			// 최대 리바이인 가능 횟수
	int maxMulti;				// 최대 리바이인 배수
	int multi;					// 리바이인 배수
	bool isAccept;				// 리바이인 수락 유무, true 면 선택, false 면 거부

};

struct _RankingInfo
{
	char cn[CN_LEN];
	char ID[ID_LEN];
	char Nick[NICK_LEN];
	int ranking;
	__int64 userMoney;
};

inline int TournamentGameStateToGameCode(TournamentGameState gamestate)
{
	switch (gamestate)
	{
	case GAMESTATE_NEWPOKER:	return GAME_CODE_NPT;
	case GAMESTATE_LOWBADUKI:	return GAME_CODE_BDT;
	case GAMESTATE_SUTDA:		return GAME_CODE_SDT;
	default:
		return 0;
	}
}


// 토너먼트 리스트 요청/응답
class CCL_REQUEST_TOURNAMENT_LIST : public CCommMsg
{
public:
	int *iTournamentIndex;
	void Set(int tournament_idx)
	{
		SetHeader(CL_REQUEST_TOURNAMENT_LIST);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
	}
};

class CSV_RESPONSE_TOURNAMENT_LIST : public CCommMsg
{
public:
	int *nTournamentCount;
	_TournamentInfo *TI;
	void Set(const std::vector<_TournamentInfo> &tl)
	{
		SetHeader(SV_RESPONSE_TOURNAMENT_LIST);
		int nCnt = (int)tl.size();
		PushData(_PVAR(nTournamentCount), &nCnt, sizeof(*nTournamentCount));
		for (std::vector<_TournamentInfo>::const_iterator itr = tl.begin(); itr != tl.end(); ++itr)
		{
			PushData(_PVAR(TI), &(*itr), sizeof(*TI));
		}
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nTournamentCount), sizeof(*nTournamentCount));
		TakeData(_PVAR(TI), sizeof(*TI) * (*nTournamentCount));
	}
};


// 토너먼트 정보 요청/응답
class CCL_REQUEST_TOURNAMENT_INFO : public CCommMsg
{
public:
	int *iTournamentIndex;
	void Set(int tournament_idx)
	{
		SetHeader(CL_REQUEST_TOURNAMENT_INFO);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
	}
};

class CSV_RESPONSE_TOURNAMENT_INFO : public CCommMsg
{
public:
	_TournamentInfo *TI;
	_TournamentPlay *TP;
	void Set(const _TournamentInfo &ti, const _TournamentPlay &tp)
	{
		SetHeader(SV_RESPONSE_TOURNAMENT_INFO);
		PushData(_PVAR(TI), &ti, sizeof(*TI));
		PushData(_PVAR(TP), &tp, sizeof(*TP));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TI), sizeof(*TI));
		TakeData(_PVAR(TP), sizeof(*TP));
	}
};

class CSV_NOTIFY_TOURNAMENT_INFO : public CCommMsg
{
public:
	_TournamentInfo *TI;
	_TournamentPlay *TP;
	void Set(const _TournamentInfo &ti, const _TournamentPlay &tp)
	{
		SetHeader(SV_NOTIFY_TOURNAMENT_INFO);
		PushData(_PVAR(TI), &ti, sizeof(*TI));
		PushData(_PVAR(TP), &tp, sizeof(*TP));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TI), sizeof(*TI));
		TakeData(_PVAR(TP), sizeof(*TP));
	}
};

// 티켓 판매 시작/종료 공지
class CSV_OPEN_DESK : public CCommMsg
{
public:
	_TournamentInfo *TI;
	void Set(const _TournamentInfo &ti)
	{
		SetHeader(SV_OPEN_DESK);
		PushData(_PVAR(TI), &ti, sizeof(*TI));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TI), sizeof(*TI));
	}
};

class CSV_CLOSE_DESK : public CCommMsg
{
public:
	_TournamentInfo *TI;
	void Set(const _TournamentInfo &ti)
	{
		SetHeader(SV_CLOSE_DESK);
		PushData(_PVAR(TI), &ti, sizeof(*TI));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TI), sizeof(*TI));
	}
};


// 토너먼트 시작/종료 공지
class CSV_START_TOURNAMENT : public CCommMsg
{
public:
	_TournamentInfo *TI;
	void Set(const _TournamentInfo &ti)
	{
		SetHeader(SV_START_TOURNAMENT);
		PushData(_PVAR(TI), &ti, sizeof(*TI));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TI), sizeof(*TI));
	}
};

class CSV_END_TOURNAMENT : public CCommMsg
{
public:
	_TournamentInfo *TI;
	_TournamentPlay *TP;
	void Set(const _TournamentInfo &ti, const _TournamentPlay &tp)
	{
		SetHeader(SV_END_TOURNAMENT);
		PushData(_PVAR(TI), &ti, sizeof(*TI));
		PushData(_PVAR(TP), &tp, sizeof(*TP));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TI), sizeof(*TI));
		TakeData(_PVAR(TP), sizeof(*TP));
	}
};


// 레벨업 공지
class CSV_LEVEL_UP_TOURNAMENT : public CCommMsg
{
public:
	int *iTournamentIndex;
	_TournamentLevel *TL;
	void Set(int tournament_idx, const _TournamentLevel &tl)
	{
		SetHeader(SV_LEVEL_UP_TOURNAMENT);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(TL), &tl, sizeof(*TL));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(TL), sizeof(*TL));
	}
};


// 티켓 구매
class CCL_BUY_TOURNAMENT_TICKET : public CCommMsg
{
public:
	_TournamentTicket *TT;
	void Set(const _TournamentTicket &ticket)
	{
		SetHeader(CL_BUY_TOURNAMENT_TICKET);
		PushData(_PVAR(TT), &ticket, sizeof(*TT));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TT), sizeof(*TT));
	}
};

class CSV_RESULT_BUY_TOURNAMENT_TICKET : public CCommMsg
{
public:
	int *iTournamentIndex;
	int *nResultCode;
	_TournamentTicket *TT;
	void Set(int tournament_idx, int result, const _TournamentTicket &tt)
	{
		SetHeader(SV_RESULT_BUY_TOURNAMENT_TICKET);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(nResultCode), &result, sizeof(*nResultCode));
		PushData(_PVAR(TT), &tt, sizeof(*TT));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(nResultCode), sizeof(*nResultCode));
		TakeData(_PVAR(TT), sizeof(*TT));
	}
};

class CSV_NOTIFY_TOURNAMENT_TICKET : public CCommMsg
{
public:
	_TournamentPlay *TP;
	void Set(const _TournamentPlay &tp)
	{
		SetHeader(SV_NOTIFY_TOURNAMENT_TICKET);
		PushData(_PVAR(TP), &tp, sizeof(*TP));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TP), sizeof(*TP));
	}
};

class CSV_MY_TOURNAMENT_LIST : public CCommMsg
{
public:
	int *nCount;
	MyTournament *arrMyTournament;
	void Set(const vector<MyTournament> &mt)
	{
		SetHeader(SV_MY_TOURNAMENT_LIST);
		int cnt = mt.size();
		PushData(_PVAR(nCount), &cnt, sizeof(*nCount));
		for (vector<MyTournament>::const_iterator itr = mt.begin(); itr != mt.end(); ++itr)
		{
			PushData(_PVAR(arrMyTournament), &(*itr), sizeof(*arrMyTournament));
		}
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nCount), sizeof(*nCount));
		TakeData(_PVAR(arrMyTournament), sizeof(*arrMyTournament) * (*nCount));
	}
};


// 티켓 환불
class CCL_REFUND_TOURNAMENT_TICKET : public CCommMsg
{
public:
	_TournamentTicket *TT;
	void Set(const _TournamentTicket &ticket)
	{
		SetHeader(CL_REFUND_TOURNAMENT_TICKET);
		PushData(_PVAR(TT), &ticket, sizeof(*TT));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TT), sizeof(*TT));
	}
};

class CSV_RESULT_REFUND_TOURNAMENT_TICKET : public CCommMsg
{
public:
	int *iTournamentIndex;
	int *nResultCode;
	int *nRefundType;	// 0: 환불 요청에 대한 응답, 1: 그외(겜 취소)
	void Set(int tournament_idx, int result, int refund_type)
	{
		SetHeader(SV_RESULT_REFUND_TOURNAMENT_TICKET);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(nResultCode), &result, sizeof(*nResultCode));
		PushData(_PVAR(nRefundType), &refund_type, sizeof(*nRefundType));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(nResultCode), sizeof(*nResultCode));
		TakeData(_PVAR(nRefundType), sizeof(*nRefundType));
	}
};


// 토너먼트 입장
class CSV_NOTIFY_REGIST_TOURNAMENT : public CCommMsg
{
public:
	_TournamentPlay *TP;
	void Set(const _TournamentPlay &tp)
	{
		SetHeader(SV_NOTIFY_REGIST_TOURNAMENT);
		PushData(_PVAR(TP), &tp, sizeof(*TP));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TP), sizeof(*TP));
	}
};

class CCL_REQUEST_REGIST_TOURNAMENT : public CCommMsg
{
public:
	int *iTournamentIndex;
	void Set(int tournament_idx)
	{
		SetHeader(CL_REQUEST_REGIST_TOURNAMENT);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
	}
};

class CSV_RESPONSE_REGIST_TOURNAMENT: public CCommMsg
{
public:
	int *iTournamentIndex;
	int *nResultCode;
	_TournamentPlay *TP;
	void Set(int tournament_idx, int result, const _TournamentPlay &tp)
	{
		SetHeader(SV_RESPONSE_REGIST_TOURNAMENT);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(nResultCode), &result, sizeof(*nResultCode));
		PushData(_PVAR(TP), &tp, sizeof(*TP));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(nResultCode), sizeof(*nResultCode));
		TakeData(_PVAR(TP), sizeof(*TP));
	}
};


// 토너먼트 포기
class CCL_REQUEST_DROP_TOURNAMENT : public CCommMsg
{
public:
	int *iTournamentIndex;
	void Set(int tournament_idx)
	{
		SetHeader(CL_REQUEST_DROP_TOURNAMENT);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
	}
};

class CSV_RESPONSE_DROP_TOURNAMENT: public CCommMsg
{
public:
	int *iTournamentIndex;
	int *nResultCode;
	_TournamentPlay *TP;
	void Set(int tournament_idx, int result, const _TournamentPlay &tp)
	{
		SetHeader(SV_RESPONSE_DROP_TOURNAMENT);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(nResultCode), &result, sizeof(*nResultCode));
		PushData(_PVAR(TP), &tp, sizeof(*TP));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(nResultCode), sizeof(*nResultCode));
		TakeData(_PVAR(TP), sizeof(*TP));
	}
};

/************************************************************************/
/* 방 생성/입장/퇴장
/************************************************************************/
class CSV_CREATE_TOURNAMENT_ROOM: public CCommMsg
{
public:
	int *iTournamentIndex;
	void Set(int tournament_idx)
	{
		SetHeader(SV_CREATE_TOURNAMENT_ROOM);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
	}
};

class CSV_ENTER_TOURNAMENT_ROOM: public CCommMsg
{
public:
	int *iTournamentIndex;
	SMALLROOMINFO *SRI;
	void Set(int tournament_idx, SMALLROOMINFO &sri)
	{
		SetHeader(SV_ENTER_TOURNAMENT_ROOM);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(SRI), &sri, sizeof(*SRI));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(SRI), sizeof(*SRI));
	}
};

class CSV_LEAVE_TOURNAMENT_ROOM: public CCommMsg
{
public:
	int *iTournamentIndex;
	LeaveRoomState *leaveRoomState;
	void Set(int tournament_idx, LeaveRoomState lrs)
	{
		SetHeader(SV_LEAVE_TOURNAMENT_ROOM);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(leaveRoomState), &lrs, sizeof(*leaveRoomState));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(leaveRoomState), sizeof(*leaveRoomState));
	}
};


/************************************************************************/
/* 리바이인                                                             */
/************************************************************************/
class CSV_CHECK_REBUYIN: public CCommMsg
{
public:
	int *iTournamentIndex;
	_CheckReBuyInInfo *info;
	void Set(int tournament_idx, const _CheckReBuyInInfo &crbi)
	{
		SetHeader(SV_CHECK_REBUYIN);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(info), &crbi, sizeof(*info));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(info), sizeof(*info));
	}
};

class CCL_ASK_REBUYIN: public CCommMsg
{
public:
	int *iTournamentIndex;
	_CheckReBuyInInfo *info;
	void Set(int tournament_idx, const _CheckReBuyInInfo &crbi)
	{
		SetHeader(CL_ASK_REBUYIN);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(info), &crbi, sizeof(*info));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(info), sizeof(*info));
	}
};

class CSV_RESULT_REBUYIN : public CCommMsg
{
public:
	int *iTournamentIndex;
	int *nResultCode;
	_TournamentSituation *TS;
	void Set(int tournament_idx, int error_code, const _TournamentSituation &ts)
	{
		SetHeader(SV_RESULT_REBUYIN);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(nResultCode), &error_code, sizeof(*nResultCode));
		PushData(_PVAR(TS), &ts, sizeof(*TS));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(nResultCode), sizeof(*nResultCode));
		TakeData(_PVAR(TS), sizeof(*TS));
	}
};

class CSV_ELIMINATED : public CCommMsg
{
public:
	int *iTournamentIndex;
	char *szID;
	char *szNick;
	int *nTotalUsers;		// 전체 참가자
	int *iRanking;			// 당사자 랭킹
	__int64 *nPrizeMoney;	// 수령 상금
	char *eliminatedTime;	// 탈락 시간
	void Set(int tournament_idx, const char *id, const char *nick, int total_users, int ranking, __int64 prize_money, const char *elimi_time)
	{
		char null = '\0';
		SetHeader(SV_ELIMINATED);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(szID), id, ID_LEN);
		PushData(_PVAR(szID), &null, 1);
		PushData(_PVAR(szNick), nick, NICK_LEN);
		PushData(_PVAR(szNick), &null, 1);
		PushData(_PVAR(nTotalUsers), &total_users, sizeof(*nTotalUsers));
		PushData(_PVAR(iRanking), &ranking, sizeof(*iRanking));
		PushData(_PVAR(nPrizeMoney), &prize_money, sizeof(*nPrizeMoney));
		PushData(_PVAR(eliminatedTime), elimi_time, TIME_LEN);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(szID), ID_LEN + 1);
		TakeData(_PVAR(szNick), NICK_LEN + 1);
		TakeData(_PVAR(nTotalUsers), sizeof(*nTotalUsers));
		TakeData(_PVAR(iRanking), sizeof(*iRanking));
		TakeData(_PVAR(nPrizeMoney), sizeof(*nPrizeMoney));
		TakeData(_PVAR(eliminatedTime), TIME_LEN);
	}
};

class CSV_NOTIFY_ELIMINATED : public CCommMsg
{
public:
	int *iTournamentIndex;
	_TournamentSituation *TS;
	void Set(int tournament_idx, const _TournamentSituation &ts)
	{
		SetHeader(SV_NOTIFY_ELIMINATED);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(TS), &ts, sizeof(*TS));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(TS), sizeof(*TS));
	}
};


/************************************************************************/
/* 랭킹                                                                 */
/************************************************************************/
class CCL_REQUEST_RANKING : public CCommMsg
{
public:
	int *iTournamentIndex;
	int *iStartRanking;
	int *nRankingCount;
	TournamentGameState *eGameState;
	TournamentRankingState *eRankingState;
	void Set(int tournament_idx, int start, int count, TournamentGameState game_state, TournamentRankingState ranking_state)
	{
		SetHeader(CL_REQUEST_RANKING);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(iStartRanking), &start, sizeof(*iStartRanking));
		PushData(_PVAR(nRankingCount), &count, sizeof(*nRankingCount));
		PushData(_PVAR(eGameState), &game_state, sizeof(*eGameState));
		PushData(_PVAR(eRankingState), &ranking_state, sizeof(*eRankingState));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(iStartRanking), sizeof(*iStartRanking));
		TakeData(_PVAR(nRankingCount), sizeof(*nRankingCount));
		TakeData(_PVAR(eGameState), sizeof(*eGameState));
		TakeData(_PVAR(eRankingState), sizeof(*eRankingState));
	}
};

class CSV_RESPONSE_RANKING : public CCommMsg
{
public:
	int *iTournamentIndex;
	int *iStartRanking;
	TournamentRankingState *eRankingState;
	TournamentGameState *eGameState;
	int *nRankingCount;
	_RankingInfo *Ranking;
	void Set(int tournament_idx, int start, TournamentRankingState ranking_state, TournamentGameState game_state, const vector<_RankingInfo> &ranking)
	{
		SetHeader(SV_RESPONSE_RANKING);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(iStartRanking), &start, sizeof(*iStartRanking));
		PushData(_PVAR(eRankingState), &ranking_state, sizeof(*eRankingState));
		PushData(_PVAR(eGameState), &game_state, sizeof(*eGameState));
		int nCnt = (int)ranking.size();
		PushData(_PVAR(nRankingCount), &nCnt, sizeof(*nRankingCount));
		for (std::vector<_RankingInfo>::const_iterator itr = ranking.begin(); itr != ranking.end(); ++itr)
		{
			PushData(_PVAR(Ranking), &(*itr), sizeof(*Ranking));
		}
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(iStartRanking), sizeof(*iStartRanking));
		TakeData(_PVAR(eRankingState), sizeof(*eRankingState));
		TakeData(_PVAR(eGameState), sizeof(*eGameState));
		TakeData(_PVAR(nRankingCount), sizeof(*nRankingCount));
		TakeData(_PVAR(Ranking), sizeof(*Ranking) * (*nRankingCount));
	}
};

class CCL_REQUEST_MY_RANKING : public CCommMsg
{
public:
	int *iTournamentIndex;
	TournamentGameState *eGameState;
	TournamentRankingState *eRankingState;
	void Set(int tournament_idx, TournamentGameState game_state, TournamentRankingState ranking_state)
	{
		SetHeader(CL_REQUEST_MY_RANKING);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(eGameState), &game_state, sizeof(*eGameState));
		PushData(_PVAR(eRankingState), &ranking_state, sizeof(*eRankingState));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(eGameState), sizeof(*eGameState));
		TakeData(_PVAR(eRankingState), sizeof(*eRankingState));
	}
};

class CSV_RESPONSE_MY_RANKING : public CCommMsg
{
public:
	int *iTournamentIndex;
	int *iMyRanking;
	__int64 *llPrizeMoney;
	TournamentGameState *eGameState;
	TournamentRankingState *eRankingState;
	void Set(int tournament_idx, int my_ranking, __int64 prize_money, TournamentGameState game_state, TournamentRankingState ranking_state)
	{
		SetHeader(SV_RESPONSE_MY_RANKING);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(iMyRanking), &my_ranking, sizeof(*iMyRanking));
		PushData(_PVAR(llPrizeMoney), &prize_money, sizeof(*llPrizeMoney));
		PushData(_PVAR(eGameState), &game_state, sizeof(*eGameState));
		PushData(_PVAR(eRankingState), &ranking_state, sizeof(*eRankingState));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(iMyRanking), sizeof(*iMyRanking));
		TakeData(_PVAR(llPrizeMoney), sizeof(*llPrizeMoney));
		TakeData(_PVAR(eGameState), sizeof(*eGameState));
		TakeData(_PVAR(eRankingState), sizeof(*eRankingState));
	}
};

// 게임 결과
class CSV_TOURNAMENT_GAME_RESULT : public CCommMsg
{
public:
	int *iTournamentIndex;
	int *nResultCode;
	int *nRoomNum;
	int *nCurrentRanking;
	__int64 *nFirstMoney;
	__int64 *nLastMoney;
	int *nRankCount;
	void Set(int tournament_idx, int result, int rnum, int current_ranking, __int64 first, __int64 last, int rank_count)
	{
		SetHeader(SV_TOURNAMENT_GAME_RESULT);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(nResultCode), &result, sizeof(*nResultCode));
		PushData(_PVAR(nRoomNum), &rnum, sizeof(*nRoomNum));
		PushData(_PVAR(nCurrentRanking), &current_ranking, sizeof(*nCurrentRanking));
		PushData(_PVAR(nFirstMoney), &first, sizeof(*nFirstMoney));
		PushData(_PVAR(nLastMoney), &last, sizeof(*nLastMoney));
		PushData(_PVAR(nRankCount), &rank_count, sizeof(*nRankCount));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(nResultCode), sizeof(*nResultCode));
		TakeData(_PVAR(nRoomNum), sizeof(*nRoomNum));
		TakeData(_PVAR(nCurrentRanking), sizeof(*nCurrentRanking));
		TakeData(_PVAR(nFirstMoney), sizeof(*nFirstMoney));
		TakeData(_PVAR(nLastMoney), sizeof(*nLastMoney));
		TakeData(_PVAR(nRankCount), sizeof(*nRankCount));
	}
};

/************************************************************************/
/* 토너먼트 참가 노티 (MS를 통해서 다른 게임 서버로 전송)               */
/************************************************************************/
class CSV_NOTIFY_TOURNAMENT : public CCommMsg
{
public:
	int *iTournamentIndex;
	_TournamentInfo *TI;
	NotificationState *eNotificationState;
	void Set(int tournament_idx, const _TournamentInfo &ti, int noti_state)
	{
		SetHeader(SV_NOTIFY_TOURNAMENT);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(TI), &ti, sizeof(*TI));
		PushData(_PVAR(eNotificationState), &noti_state, sizeof(*eNotificationState));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(TI), sizeof(*TI));
		TakeData(_PVAR(eNotificationState), sizeof(*eNotificationState));
	}
};


/************************************************************************/
/* 휴식시간               */
/************************************************************************/
class CSV_NOTIFY_START_BREAK : public CCommMsg
{
public:
	int *iTournamentIndex;
	void Set(int tournament_idx)
	{
		SetHeader(SV_NOTIFY_START_BREAK);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
	}
};

class CSV_NOTIFY_STOP_BREAK : public CCommMsg
{
public:
	int *iTournamentIndex;
	void Set(int tournament_idx)
	{
		SetHeader(SV_NOTIFY_STOP_BREAK);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
	}
};


/************************************************************************/
/* 토너먼트 유저 참가 정보               */
/************************************************************************/
class CCL_REQUEST_TOURNAMENT_USER_INFO : public CCommMsg
{
public:
	void Set()
	{
		SetHeader(CL_REQUEST_TOURNAMENT_USER_INFO);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

class CSV_RESPONSE_TOURNAMENT_USER_INFO : public CCommMsg
{
public:
	int *buyTicketCnt;			// 전체 참가수
	int *rankedCnt;				// 입상 횟수
	__int64 *totalPrizeMoney;	// 누적 상금
	void Set(int buy_ticket_cnt, int ranked_cnt, __int64 total_prize_money)
	{
		SetHeader(SV_RESPONSE_TOURNAMENT_USER_INFO);
		PushData(_PVAR(buyTicketCnt), &buy_ticket_cnt, sizeof(*buyTicketCnt));
		PushData(_PVAR(rankedCnt), &ranked_cnt, sizeof(*rankedCnt));
		PushData(_PVAR(totalPrizeMoney), &total_prize_money, sizeof(*totalPrizeMoney));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(buyTicketCnt), sizeof(*buyTicketCnt));
		TakeData(_PVAR(rankedCnt), sizeof(*rankedCnt));
		TakeData(_PVAR(totalPrizeMoney), sizeof(*totalPrizeMoney));
	}
};


/************************************************************************/
/* 특정 유저가 경기중인 방으로 관전 입장
/************************************************************************/
class CCL_ASK_WATCH_TOURNAMENT_ROOM : public CCommMsg
{
public:
	int *iTournamentIndex;
	char *szCN;
	int *sndFxKind;
	void Set(int tournament_idx, const char *cn, int sex)
	{
		SetHeader(CL_ASK_WATCH_TOURNAMENT_ROOM);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(szCN), cn, CN_LEN);
		PushData(_PVAR(sndFxKind), &sex, sizeof(*sndFxKind));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(szCN), CN_LEN);
		TakeData(_PVAR(sndFxKind), sizeof(*sndFxKind));
	}
};

class CSV_WATCH_TOURNAMENT_ROOM: public CCommMsg
{
public:
	int *iTournamentIndex;
	SMALLROOMINFO *SRI;
	void Set(int tournament_idx, SMALLROOMINFO &sri)
	{
		SetHeader(SV_WATCH_TOURNAMENT_ROOM);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(SRI), &sri, sizeof(*SRI));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(SRI), sizeof(*SRI));
	}
};

/************************************************************************/
/* 게임 결과 패킷에도 포함되어 있는 현재 랭크 정보
/************************************************************************/
class CCL_REQUEST_CURRENT_USER_RANK : public CCommMsg
{
public:
	int *iTournamentIndex;
	void Set(int tournament_idx)
	{
		SetHeader(CL_REQUEST_CURRENT_USER_RANK);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
	}
};

class CSV_RESPONSE_CURRENT_USER_RANK : public CCommMsg
{
public:
	int *iTournamentIndex;
	int *nCurrentRanking;		// 현재 등수
	__int64 *nFirstMoney;		// 1등 머니
	__int64 *nLastMoney;		// 꼴등 머니
	int *nRankCount;			// 상금 자격
	char *szBreakEndTime;		// 휴식 종료 시간
	void Set(int tournament_idx, int current_ranking, __int64 first, __int64 last, int rank_count, const char *breakend)
	{
		SetHeader(SV_RESPONSE_CURRENT_USER_RANK);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(nCurrentRanking), &current_ranking, sizeof(*nCurrentRanking));
		PushData(_PVAR(nFirstMoney), &first, sizeof(*nFirstMoney));
		PushData(_PVAR(nLastMoney), &last, sizeof(*nLastMoney));
		PushData(_PVAR(nRankCount), &rank_count, sizeof(*nRankCount));
		PushData(_PVAR(szBreakEndTime), breakend, TIME_LEN);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(nCurrentRanking), sizeof(*nCurrentRanking));
		TakeData(_PVAR(nFirstMoney), sizeof(*nFirstMoney));
		TakeData(_PVAR(nLastMoney), sizeof(*nLastMoney));
		TakeData(_PVAR(nRankCount), sizeof(*nRankCount));
		TakeData(_PVAR(szBreakEndTime), TIME_LEN);
	}
};


/************************************************************************/
/* 상금/환불금 등 미지급금을 지급함
/************************************************************************/
class CSV_TOURNAMENT_RETURN_UNPAID : public CCommMsg
{
public:
	int *iTournamentIndex;
	int *nErrorCode;		// 지급 실패 에러 코드
	int *nRanking;			// 랭킹
	int *nGameCode;			// 게임 코드
	int *nUnpaidType;		// 0:상금, 1:환불
	__int64 *llMoney;		// 지급된 머니
	void Set(int tournament_idx, int err, int ranking, int game_code, int kind, __int64 money)
	{
		SetHeader(SV_TOURNAMENT_RETURN_UNPAID);
		PushData(_PVAR(iTournamentIndex), &tournament_idx, sizeof(*iTournamentIndex));
		PushData(_PVAR(nErrorCode), &err, sizeof(*nErrorCode));
		PushData(_PVAR(nRanking), &ranking, sizeof(*nRanking));
		PushData(_PVAR(nGameCode), &game_code, sizeof(*nGameCode));
		PushData(_PVAR(nUnpaidType), &kind, sizeof(*nUnpaidType));
		PushData(_PVAR(llMoney), &money, sizeof(*llMoney));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(iTournamentIndex), sizeof(*iTournamentIndex));
		TakeData(_PVAR(nErrorCode), sizeof(*nErrorCode));
		TakeData(_PVAR(nRanking), sizeof(*nRanking));
		TakeData(_PVAR(nGameCode), sizeof(*nGameCode));
		TakeData(_PVAR(nUnpaidType), sizeof(*nUnpaidType));
		TakeData(_PVAR(llMoney), sizeof(*llMoney));
	}
};
