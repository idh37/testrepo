#ifndef _BETSTUCT_H
#define _BETSTUCT_H

#include "GameCommon.h"
//////////////////////////////////////////////////////////////////////////
//////////////////////      베팅 로그 관련      //////////////////////////
//////////////////////////////////////////////////////////////////////////

// enum {
// 	GAME_KIND_CLASSIC=0,
// 	GAME_KIND_TOTALPOKER,
// 	GAME_KIND_SUDDA,
// 	GAME_KIND_HOOLA,
// 	GAME_KIND_NEWPOKER,
// 	GAME_KIND_MAX,
// };

#define  LOGSERVER_VERSION  2011


//
//enum eGameType
//{
//	GAME_UNKNOWN = -1,
//	GAME_7POKER,
//	GAME_LOWBD,
//	GAME_NEWPOKER,
//	GAME_HIGHLOW,
//	GAME_DUAL,
//	GAME_SUTDA,
//	GAME_HOOLA,
//
//	GAME_ALL
//};
//
//const struct GAMETYPE_AND_ENTRY
//{
//	eGameType eType;
//	char* pName;
//}	c_aGameTypeAndEntry[] =
//{
//	{GAME_7POKER,   ("7POKER")},
//	{GAME_LOWBD,    ("LOWBD")},
//	{GAME_NEWPOKER, ("NPOKER")},
//	{GAME_HIGHLOW,  ("HIGHLOW")},
//	{GAME_DUAL,		("DUAL")},
//	{GAME_SUTDA,	("SUTDA")},
//	{GAME_HOOLA,	("HOOLA")},
//};


// enum {
// 	GAME_KIND_CLASSIC=0,		// 클래식포커
// 	GAME_KIND_SPOKER2,			// 7포커
// 	GAME_KIND_HILOW,			// 하이로우
// 	GAME_KIND_BADUGI,			// 로우바둑이
// 	GAME_KIND_SUDDA,			// 섯다
// 	GAME_KIND_HOOLA,			// 훌라
// 	GAME_KIND_NEWPOKER,			// 뉴포커
// 	GAME_KIND_VSPOKER,			// 맞포커
// 	OLD_GAME_KIND_MAX,
// };

//#define OLD_GAME_KIND_MAX  8


typedef struct 
{
	BOOL		bCheatPlayLoged;
	BOOL		bPMoneyLoged;
	int			nCheatUserType;
	INT64		i64PrevMoney;
	int			nPlayCount;
	int			nWin;
	int			nDraw;
	int			nLose;
	INT64		i64RoomEnterMoney;

	void Init()
	{
		bCheatPlayLoged = FALSE;
		bPMoneyLoged = FALSE;
		nCheatUserType = 0;
		i64PrevMoney = 0;
		nPlayCount = 0;
		nWin = 0;
		nDraw = 0;
		nLose = 0;
	}

	void IncreasePlayCount()
	{
		nPlayCount++;
	}

	int	GetPlayCount()
	{
		return nPlayCount;
	}

	void ResetCheatPlay()
	{
		bCheatPlayLoged = FALSE;
		nPlayCount = 0;
		i64RoomEnterMoney = 0;
	}


} ST_CHEAT_INFO;


// 로그서버 로그인정보
typedef struct 
{
	short	sType;					// 0 : 서버, 1 : 어드민	
	short 	sGameCode;
	char	szID[ ID_LEN ];
	char	szPass[20];
} ST_LOGSERVER_LOGIN_INFO;


// 의심자 추출 플레이횟수 설정
typedef struct 
{
	short 		sGameCode;
	int			nMaxPlayCount;
} ST_CHEAT_PLAYCOUNT_CONFIG;


// 등록유저 모니터링 머니 설정
typedef struct 
{
	short 		sGameCode;
	INT64		nMaxPMoney;
	INT64		nMaxChangeMoney;
} ST_CHEAT_MONEY_CONFIG;



// 서버에서 로그인시 로그서버로 CHEAT 유저정보 요청
typedef struct	
{
	int			nSocketID;
	short 		sGameCode;
	int			nUserNumber;		// 해당서버에서 유저가 쓰던 번호

	int			nGroupNum;
	int 		nLimitMoneyCode;
	INT64		llLimitMoney;		// 입장 구간 머니
	
	char		szID[ ID_LEN ];
	char		szIP[ IP_LEN ];
} ST_REQ_CHEAT_USERINFO;

// 로그서버에서 서버로 CHEAT 유저인지 응답 / Admin 요청시 서버로 전송,
typedef struct	
{
	int			nUserNumber;			// 해당서버에서 유저가 쓰던 번호
	short		nCheatType;				// 1 : ID설정 / 2: IP설정 / 3:ID&IP
	BYTE		nStatus;				// 1 : 등록 / 2: 해제
	char		szCheat[20];			// id 일경우엔 id, ip일경우 ip전송
} ST_RES_CHEAT_USERINFO;

// 로그서버에서 admin 클라이언트로 CHEAT 유저가 로그인했음을 알림.
typedef struct 
{
	short 		sGameCode;			// 게임코드
	int 		nGroupNum;			// 그룹번호
	int 		nLimitMoneyCode;		// 머니리미트 코드
	INT64		llLimitMoney;		// 입장 구간 머니

	char		szID[ ID_LEN ];			// 아이디
	char		szIP[ IP_LEN ];			// 아이피

	short		sType;				// 1 : ID설정 / 2: IP설정 / 3:ID&IP

} ST_CHEAT_USER_LOGIN_INFO;

// Admin 클라이언트에서 ID, IP 삭제 추가 요청
typedef struct 
{
	short 		sGameCode;
	char		szCheat[20];		// id 일경우엔 id, ip일경우 ip전송
	short		sType;				// 1:ID설정 / 2:IP설정
	char		szAdminID[ ID_LEN ];
	char		szAdminIP[ IP_LEN ];
	char		szMemo[50];
	BYTE		bAdd;				// 0:삭제, 1:추가
} ST_REQ_ADMIN_CHEAT_USER;

// 한방에서 일정플레이횟수 이상일때 로그 기록..
typedef struct
{
	short 	    sGameCode;
	int			nGroupCode;			// 그룹정보
	int			nLimitMoneyCode;		// 머니리미트 코드
	int			nRoomNumber;		// 방번호
	int			nPlayCount;			// 플레이 횟수
	char		szID[ ID_LEN ];			// 아이디
	char		szIP[ IP_LEN ];			// 아이피
	INT64		i64PMoney;			// 보유머니

	INT64		i64RoomChangeMoney;			// 그방에서 변동된 머니

	BYTE		bIdentity;

	SYSTEMTIME	tRegDate;

} ST_PLAYCOUNT_OVER;


// --------------------------------------------
// add by inthesky (2010/7/6-11:42:48) - begin

typedef struct 
{
	short       sGameCode;
	char		szCN[ CN_LEN ];			// CN코드
	int			nGroupCode;			// 그룹정보			
	int 		nLimitMoneyCode;		// 채널 번호
	INT64		i64PMoney;			// 보유머니
	char		szID[ ID_LEN ];			// 유저의 ID
	char		szIP[ IP_LEN ];			// 유저의 IP
	

}ST_CHNENTER_NOTIFY;

// add by inthesky (2010/7/6-11:42:48) - end
// --------------------------------------------

////////////////////////////////////////////////
// 어드민 클라이언트에서 쓰는 구조체



// 등록 유저실시간 알림을 위한 구조체	(Log
typedef struct {
	short       sGameCode;
	int			nGroupNum;			// 그룹번호
	int			nLimitMoneyCode;		// 머니리미트 코드
	INT64		llLimitMoney;		// 입장 구간 머니

	char		szID[ ID_LEN ];			// 아이디
	char		szIP[ IP_LEN ];			// 아이피

	short		sType;				// 1 : ID설정 / 2: IP설정 / 3:ID&IP
	BYTE		nNotifyType;		// 0:로그인 / 1:머니변동 / 2:보유머니

	INT64		PMoney;				// 보유머니
	INT64		BeforeMoney;
	INT64		AddMoney;

	int			nWin;
	int			nDraw;
	int			nLose;

	BYTE		bIdentity;

	SYSTEMTIME	tRegDate;

} ST_CHEAT_NOTIFY;


// 로그서버에서 ID또는 IP설정이 있을시 사유를 정보를 같이 보내기 위해
typedef struct {
	short       sGameCode;
	int			nGroupNum;			// 그룹번호
	int			nLimitMoneyCode;		// 머니리미트 코드
	INT64		llLimitMoney;		// 입장 구간 머니

	char		szID[ ID_LEN ];			// 아이디
	char		szIP[ IP_LEN ];			// 아이피

	short		sType;				// 1 : ID설정 / 2: IP설정 / 3:ID&IP
	BYTE		nNotifyType;		// 0:로그인 / 1:머니변동 / 2:보유머니

	INT64		PMoney;				// 보유머니
	INT64		BeforeMoney;
	INT64		AddMoney;

	int			nWin;
	int			nDraw;
	int			nLose;

	BYTE		bIdentity;

	SYSTEMTIME	tRegDate;

	char		szMemo[50];

} ST_CHEAT_NOTIFY_TO_ADMIN;

//  ID, IP 검색
typedef struct
{
	short       sGameCode;
	char		szCheat[20];		// id 일경우엔 id, ip일경우 ip전송
	short		sType;				// 1:ID검색 / 2:IP검색
	SYSTEMTIME	tStartTime;
	SYSTEMTIME	tEndTime;
} ST_SEARCH_CHEAT_INFO;

//  ID, IP 검색 결과 전송
typedef struct 
{
	short       sGameCode;
	char		szCheat[20];		// id 일경우엔 id, ip일경우 ip전송
	short		sType;				// 1:ID설정 / 2:IP설정 / 3:ID&IP
	char		szAdminID[ ID_LEN ];
	char		szAdminIP[ IP_LEN ];
	char		szMemo[50];
	SYSTEMTIME	tRegDate;
} ST_SERCH_RESULT_CHEAT_INFO;

// 특정방을 기준으로 에서 1시간 안팍으로 검색 
typedef struct {
	int			nGroupCode;			// 그룹정보
	int			nLimitMoneyCode;		// 머니리미트 코드
	int			nRoomNumber;		// 방번호
} ST_SAME_ROOM_INFO;

// 의심자 검색(같은방에서 일정횟수 같이 플레이)
typedef struct
{
	short				sGameCode;
	char				szCheat[20];		// id 일경우엔 id, ip일경우 ip전송
	short				sType;				// 1:ID / 2:IP / 3: sameRoom
	SYSTEMTIME			tStartTime;
	SYSTEMTIME			tEndTime;
	ST_SAME_ROOM_INFO	stRoom;
} ST_SEARCH_CHEAT_PLAY;

// 의심자 검색 결과 전송(같은방에서 일정횟수 같이 플레이)
typedef struct
{
	short       sGameCode;
	int			nGroupCode;			// 그룹정보
	int			nLimitMoneyCode;		// 머니리미트 코드
	int			nRoomNumber;		// 방번호
	int			nPlayCount;			// 플레이 횟수
	char		szID[ ID_LEN ];			// 아이디
	char		szIP[ IP_LEN ];			// 아이피
	INT64		i64PMoney;			// 보유머니
	INT64		i64RoomChangeMoney;			// 방에서 변동된 머니
	SYSTEMTIME	tRegDate;
} ST_SEARCH_RESULT_CHEAT_PLAY;


// 접속 정보를 DB에 저장
typedef struct
{
	char		szCN[ CN_LEN ];
	char		szID[ ID_LEN ];
	char		szIP[ IP_LEN ];
	short       sGameCode;
	int			nGroupCode;			// 그룹정보
	SYSTEMTIME	tRegDate;
	
}ST_ADD_CONNECT_LOG;

typedef struct  
{
	int			nIDX;
	char		szCN[ CN_LEN ];
	char		szID[ ID_LEN ];
	char		szIP[ IP_LEN ];
	short       sGameCode;
	int			nGroupCode;			// 그룹정보	
	SYSTEMTIME	tRegDate;

}ST_DOUBT_NOTIFY_DATA;

typedef struct  
{
	int			nIDX;
	char		szCN[ CN_LEN ];
	char		szID[ ID_LEN ];
	char		szIP[ IP_LEN ];
	short       sGameCode;
	int			nGroupCode;			// 그룹정보
	int			nConfirmType;		// 컨펌타입
	char		szAdminName[ ID_LEN ];	// 어드민 이름
	SYSTEMTIME	tRegDate;
	SYSTEMTIME	tConfirmDate;
	char		szMemo[200];		// 메모!
	
}ST_DOUBT_ORG_DATA;

typedef struct  
{
	int			nIDX;
}ST_DOUBT_REMOVE_DATA;

typedef struct  
{
	short				sGameCode;
	char				szCheat[20];		// id 일경우엔 id, ip일경우 ip전송
	short				sType;				// 1:ID / 2:IP 
	SYSTEMTIME			tStartTime;		
	SYSTEMTIME			tEndTime;

}ST_SEARCH_DOUBT_DATA_PARAM;

typedef struct  
{
	int			nGetType;			// 가져오기 타입 - 0:전체 , 1:미처리내역, 2. 확인중인내역, 3:처리내역
	SYSTEMTIME	tRegDate;			// 등록일 기준으로 가져오기

}ST_GET_DOUBT_LIST;	


#endif //_BETSTUCT_H