
#pragma once

#include "CommMsg.h"

#include "BetStruct.h"

#define POKERLOGMAN_VER	777		// 베팅로그 관리서버 버전


typedef enum _eUserType : unsigned char
{
	USER_TYPE_NORMAL = 0,
	USER_TYPE_SUPER,
	USER_TYPE_MAX

}TYPE_USER;


// 타 게임 호환 메세지 #########################################################

enum
{
	///////// 베팅로그관리서버 <-> 대국실 서버
	LSV_CHECK_VERSION=4000,				// 버전 체크
	LSV_HEARTBEAT,
	LSV_ASK_LOGIN,						// 로그인 서버로 로그인 요청
	LSV_ACCEPT_LOGIN,					// 로그인 허용
	LSV_REFUSE_LOGIN,					// 로그인 거부
	
	LSV_ADMINUSER_LIST_REQ,				// 관리자 목록 요청
	LSV_ADMINUSER_LIST_RES,				// 관리자 목록 응답
	LSV_ADMINUSER_LIST_COMMIT_START,	// 관리자 목록을 새롭게 갱신 시작
	LSV_ADMINUSER_LIST_COMMIT_DATA,		// 관리자 데이터
	LSV_ADMINUSER_LIST_COMMIT_END,		// 관리자 목록을 새롭게 갱신 끝


	LSV_CHEAT_MONEY_CONFIG,				// 로그남길 머니 설정
	LSV_CHEAT_PLAYCOUNT_CONFIG,			// 로그남길 판수 설정

	LSV_CHNENTER_NOFITY,				// 로그인된 사용자의 정보를 로그서버에 보냄 (Server -> LogServer)

	LSV_CHECK_CHEAT_USER,				// 등록되어있는 유지인지 체크		(Server	-> LogServer) 
	LSV_CHEAT_USER_INFO,				// 서버로 CHEAT 유저임을 알림		(LogServer -> Server) 
	LSV_SHEAT_USER_TO_ADMIN,			// 어드민 클리라이언트로 CHEAT 유저임을 알림
	LSV_USER_PLAYCOUNT_OVER,			// 유저가 같은방에서 2명이상이 쳤을때..

	LCL_ADMIN_CHEAT_USER,				// 관리자가 불량 유저 관리 (등록, 삭제)

	LSV_NOTIFY_CHEAT,					// 등록된 유저에 대해 실시간 알림
										// 로그인시 알림
										// 변동머니/보유머니 초과 알림, (Server -> LogServer)

	LSV_NOTIFY_CHEAT_TO_ADMIN,			// 위와 같은 패킷이지만 LogServer ->Admin 구조 약간 틀림

	LCL_SEARCH_CHEAT_INFO,				// Cheat유저 검색 (ID, IP) (Admin -> Server)
	LSV_SEARCH_RESULT_CHEAT_INFO,		// Cheat유저 검색 결과 전송 (ID, IP) (Server -> Admin)

	LCL_SEARCH_CHEAT_PLAY,				// 같은방 플레이 유저 검색 (Admin -> Server)
	LCL_SEARCH_CHEAT_PLAY_IN_ROOM,		// 같은방 플레이 유저 검색 방번호 기준으로 검색 (Admin -> Server)
	LCL_SEARCH_RESULT_CHEAT_PLAY,		// 같은방 플에이 유저검색 결과 전송 (Server -> Admin)

	LSV_SERVERMSG,						// 로그서버 메세지


	LCL_NOTIFY_DOUBT_DATA,				// DOUBT DATA 통보 : 간략화된 패킷
	LCL_NOTIFY_DOUBT_ORG_DATA,			// DOUBT DATA 통보 : 오리지널 데이터
	LCL_SEARCH_DOUBT_LIST_REQ,			// DOUBT LIST 검색 요청 (이건 ID, IP 등으로 검색하는걸 뜻하는 패킷)
	LCL_SEARCH_DOUBT_LIST_RES,			// DOUBT LIST 검색 결과 (이건 ID, IP 등으로 검색하는걸 뜻하는 패킷)
	LCL_GET_DOUBT_LIST,					// DOUBT LIST 가져오기 (이건 미처리 내역 등의 가져오기를 뜻하는 패킷)
	LCL_MODIFY_DOUBT_DATA,				// DOUBT 데이터 변경
	LCL_REMOVE_DOUBT_DATA,				// DOUBT 데이터 삭제


};




///////////////////////////// 게임서버용 ////////////////////////////

/////////// 인증키 요구 
class CLSV_CHECK_VERSION : public CCommMsg
{
public:
	int *pnVersion;

	void Set(int nVersion)
	{
		SetHeader(LSV_CHECK_VERSION);
		PushData(_PVAR(pnVersion), &nVersion, sizeof(*pnVersion));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pnVersion), sizeof(*pnVersion));
	}
};

// 로그인 서버로 로그인 요청
class CLSV_ASK_LOGIN : public CCommMsg
{
public:
	ST_LOGSERVER_LOGIN_INFO*	pLoginInfo;

	void Set( ST_LOGSERVER_LOGIN_INFO* plogininfo )
	{
		SetHeader( LSV_ASK_LOGIN );
		PushData( _PVAR(pLoginInfo), plogininfo, sizeof(*pLoginInfo) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(pLoginInfo), sizeof(*pLoginInfo) );
	}
};

/////////// 허트비트
class CLSV_HEARTBEAT : public CCommMsg
{
public:

	void Set() 
	{
		SetHeader(LSV_HEARTBEAT);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};


// 로그인 허용
class CLSV_ACCEPT_LOGIN : public CCommMsg
{
public:
	int *nType;					// 서버인지 유저인지
	BYTE* bSuperUser;	// 슈퍼유저인지 일반 유저인지
	VSTRING szUserName;

	void Set(int ntype, BYTE bSU, char* szName)
	{
		SetHeader(LSV_ACCEPT_LOGIN);
		PushData(_PVAR(nType), &ntype, sizeof(*nType));
		PushData(_PVAR(bSuperUser), &bSU, sizeof(*bSuperUser));
		PushVSTR(szUserName, szName );
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nType), sizeof(*nType));
		TakeData(_PVAR(bSuperUser), sizeof(*bSuperUser));
		TakeVSTR( szUserName );
	}
};

// 로그인 거부 
class CLSV_REFUSE_LOGIN : public CCommMsg
{
public:
	int *nType;			// 거부이유

	void Set(int ntype)
	{
		SetHeader( LSV_REFUSE_LOGIN );
		PushData(_PVAR(nType), &ntype, sizeof(*nType));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nType), sizeof(*nType));
	}
};

// --------------------------------------------
// add by inthesky (2010/6/29-14:19:39) - begin

class CLSV_ADMINUSER_LIST_REQ : public CCommMsg
{
public:
	void Set() 
	{
		SetHeader(LSV_ADMINUSER_LIST_REQ);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

class CLSV_ADMINUSER_LIST_RES : public CCommMsg
{
public:
	VSTRING strIP;
	VSTRING strName;
	BYTE* bSuperUser;	// 슈퍼유저인지 일반 유저인지

	void Set(char* szIP, char* szName, BYTE bSU)
	{
		SetHeader(LSV_ADMINUSER_LIST_RES);
		PushVSTR(strIP, szIP );
		PushVSTR(strName, szName );
		PushData(_PVAR(bSuperUser), &bSU, sizeof(*bSuperUser));
		
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeVSTR( strIP );
		TakeVSTR( strName );
		TakeData(_PVAR(bSuperUser), sizeof(*bSuperUser));
	}
};

class CLSV_ADMINUSER_LIST_COMMIT_START : public CCommMsg
{
public:
	void Set() 
	{
		SetHeader(LSV_ADMINUSER_LIST_COMMIT_START);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

class CLSV_ADMINUSER_LIST_COMMIT_DATA : public CCommMsg
{
public:
	VSTRING strIP;
	VSTRING strName;
	BYTE* bSuperUser;	// 슈퍼유저인지 일반 유저인지
	INT* nUserIdx;

	void Set(char* szIP, char* szName, BYTE bSU, INT nIdx)
	{
		SetHeader(LSV_ADMINUSER_LIST_COMMIT_DATA);
		PushVSTR(strIP, szIP );
		PushVSTR(strName, szName );
		PushData(_PVAR(bSuperUser), &bSU, sizeof(*bSuperUser));
		PushData(_PVAR(nUserIdx), &nIdx, sizeof(*nUserIdx));

	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeVSTR( strIP );
		TakeVSTR( strName );
		TakeData(_PVAR(bSuperUser), sizeof(*bSuperUser));
		TakeData(_PVAR(nUserIdx), sizeof(*nUserIdx));
	}
};

class CLSV_ADMINUSER_LIST_COMMIT_END : public CCommMsg
{
public:
	void Set() 
	{
		SetHeader(LSV_ADMINUSER_LIST_COMMIT_END);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

// add by inthesky (2010/6/29-14:19:39) - end
// --------------------------------------------


// 로그남길 머니 설정
class CLSV_CHEAT_MONEY_CONFIG : public CCommMsg
{
public:
	ST_CHEAT_MONEY_CONFIG*		m_pConfig;

	void Set( ST_CHEAT_MONEY_CONFIG* config )
	{
		SetHeader( LSV_CHEAT_MONEY_CONFIG );
		PushData( _PVAR(m_pConfig), config, sizeof(*m_pConfig) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(m_pConfig), sizeof(*m_pConfig) );
	}
};


// 로그남길 판수 설정
class CLSV_CHEAT_PLAYCOUNT_CONFIG : public CCommMsg
{
public:
	ST_CHEAT_PLAYCOUNT_CONFIG*		m_pConfig;

	void Set( ST_CHEAT_PLAYCOUNT_CONFIG* config )
	{
		SetHeader( LSV_CHEAT_PLAYCOUNT_CONFIG );
		PushData( _PVAR(m_pConfig), config, sizeof(*m_pConfig) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(m_pConfig), sizeof(*m_pConfig) );
	}
};





// 등록되어있는 유지인지 체크		(Server -> LogServer) 
class CLSV_CHECK_CHEAT_USER : public CCommMsg
{
public:
	int*					m_nServerNumber;			// 서버 번호

	ST_REQ_CHEAT_USERINFO*			m_pCheatUserInfo;			// 불량유저 정보, 아이디, 아이피

	void Set( int nServer, ST_REQ_CHEAT_USERINFO *pInfo )
	{
		SetHeader( LSV_CHECK_CHEAT_USER );
		PushData( _PVAR(m_nServerNumber), &nServer, sizeof(*m_nServerNumber) );
		PushData( _PVAR(m_pCheatUserInfo), pInfo, sizeof(ST_REQ_CHEAT_USERINFO) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_nServerNumber), sizeof(*m_nServerNumber) );
		TakeData( _PVAR(m_pCheatUserInfo), sizeof(ST_REQ_CHEAT_USERINFO) );
	}
};

// 등록된 유저이거나 관리자가 등록 또는 삭제 요청을 하면 전송 (LogServer -> Server)
class CLSV_CHEAT_USER_INFO : public CCommMsg
{
public:
	ST_RES_CHEAT_USERINFO*			m_pCheatUserInfo;			// 불량유저 정보, 아이디, 아이피

	void Set( ST_RES_CHEAT_USERINFO *pInfo )
	{
		SetHeader( LSV_CHEAT_USER_INFO );
		PushData( _PVAR(m_pCheatUserInfo), pInfo, sizeof(ST_RES_CHEAT_USERINFO) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pCheatUserInfo), sizeof(ST_RES_CHEAT_USERINFO) );
	}

};

// 어드민 클리라이언트로 CHEAT 유저임을 알림
class CLSV_SHEAT_USER_TO_ADMIN : public CCommMsg
{
public:
	ST_CHEAT_USER_LOGIN_INFO*			m_pCheatUserInfo;			// 불량유저 정보, 아이디, 아이피

	void Set( ST_CHEAT_USER_LOGIN_INFO *pInfo )
	{
		SetHeader( LSV_SHEAT_USER_TO_ADMIN );
		PushData( _PVAR(m_pCheatUserInfo), pInfo, sizeof(ST_CHEAT_USER_LOGIN_INFO) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pCheatUserInfo), sizeof(ST_CHEAT_USER_LOGIN_INFO) );
	}

};


// 유저가 같은방에서 2명이상이 쳤을때..
class CLSV_USER_PLAYCOUNT_OVER : public CCommMsg
{
public:
	ST_PLAYCOUNT_OVER*			m_pPlayCountOver;			// 불량유저 정보, 아이디, 아이피

	void Set( ST_PLAYCOUNT_OVER *pPlayOver)
	{
		SetHeader( LSV_USER_PLAYCOUNT_OVER );
		PushData( _PVAR( m_pPlayCountOver ), pPlayOver, sizeof( ST_PLAYCOUNT_OVER ) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR( m_pPlayCountOver ), sizeof( ST_PLAYCOUNT_OVER ) );
	}
};


// --------------------------------------------
// add by inthesky (2010/7/6-13:10:21) - begin
// comment : 채널입장시 게임서버가 AbuserMonitor Server에 넘겨주는 패킷. 사용자 정보가 담겨있다.
class CLSV_CHNENTER_NOTIFY : public CCommMsg
{
public:
	ST_CHNENTER_NOTIFY*			m_pChnEnterNotify;

	void Set( ST_CHNENTER_NOTIFY *pChnEnter)
	{
		SetHeader( LSV_CHNENTER_NOFITY );
		PushData( _PVAR( m_pChnEnterNotify ), pChnEnter, sizeof( ST_CHNENTER_NOTIFY ) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR( m_pChnEnterNotify ), sizeof( ST_CHNENTER_NOTIFY ) );
	}
};


class CLCL_NOTIFY_DOUBT_DATA : public CCommMsg
{
public:
	ST_DOUBT_NOTIFY_DATA*		m_pDoubtUserInfo;			// 불량유저 정보, 아이디, 아이피

	void Set( ST_DOUBT_NOTIFY_DATA *pInfo )
	{
		SetHeader( LCL_NOTIFY_DOUBT_DATA );
		PushData( _PVAR(m_pDoubtUserInfo), pInfo, sizeof(ST_DOUBT_NOTIFY_DATA) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pDoubtUserInfo), sizeof(ST_DOUBT_NOTIFY_DATA) );
	}
 
};

class CLCL_NOTIFY_DOUBT_ORG_DATA : public CCommMsg
{
public:
	ST_DOUBT_ORG_DATA*		m_pDoubtUserInfo;			// 불량유저 정보, 아이디, 아이피

	void Set( ST_DOUBT_ORG_DATA *pInfo )
	{
		SetHeader( LCL_NOTIFY_DOUBT_ORG_DATA );
		PushData( _PVAR(m_pDoubtUserInfo), pInfo, sizeof(ST_DOUBT_ORG_DATA) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pDoubtUserInfo), sizeof(ST_DOUBT_ORG_DATA) );
	}

};


class CLCL_MODIFY_DOUBT_DATA : public CCommMsg
{
public:
	ST_DOUBT_ORG_DATA*		m_pDoubtUserInfo;

	void Set( ST_DOUBT_ORG_DATA *pInfo )
	{
		SetHeader( LCL_MODIFY_DOUBT_DATA );
		PushData( _PVAR(m_pDoubtUserInfo), pInfo, sizeof(ST_DOUBT_ORG_DATA) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pDoubtUserInfo), sizeof(ST_DOUBT_ORG_DATA) );
	}
};

class CLCL_REMOVE_DOUBT_DATA : public CCommMsg
{
public:
	ST_DOUBT_REMOVE_DATA*		m_pDoubtRemoveInfo;			// CheatLopg_DoubtList 에서의 IDX 번호
	
	void Set( ST_DOUBT_REMOVE_DATA *pInfo )
	{
		SetHeader( LCL_REMOVE_DOUBT_DATA );
		PushData( _PVAR(m_pDoubtRemoveInfo), pInfo, sizeof(ST_DOUBT_REMOVE_DATA) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pDoubtRemoveInfo), sizeof(ST_DOUBT_REMOVE_DATA) );
	}
};

class CLCL_GET_DOUBT_LIST : public CCommMsg
{
public:
	ST_GET_DOUBT_LIST*		m_pGetDoubtParam;			// 가져오기 파라미터

	void Set( ST_GET_DOUBT_LIST *pInfo )
	{
		SetHeader( LCL_GET_DOUBT_LIST );
		PushData( _PVAR(m_pGetDoubtParam), pInfo, sizeof(ST_GET_DOUBT_LIST) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pGetDoubtParam), sizeof(ST_GET_DOUBT_LIST) );
	}
};

class CLCL_SEARCH_DOUBT_DATA_REQ : public CCommMsg
{
public:
	ST_SEARCH_DOUBT_DATA_PARAM* m_pInfo;

	void Set( ST_SEARCH_DOUBT_DATA_PARAM*	pInfo )
	{
		SetHeader( LCL_SEARCH_DOUBT_LIST_REQ );
		PushData( _PVAR(m_pInfo), pInfo, sizeof(ST_SEARCH_DOUBT_DATA_PARAM) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pInfo), sizeof(ST_SEARCH_DOUBT_DATA_PARAM) );
	}
};

class CLCL_SEARCH_DOUBT_DATA_RES : public CCommMsg
{
public:
	ST_DOUBT_ORG_DATA*		m_pDoubtUserInfo;			

	void Set( ST_DOUBT_ORG_DATA *pInfo )
	{
		SetHeader( LCL_SEARCH_DOUBT_LIST_RES );
		PushData( _PVAR(m_pDoubtUserInfo), pInfo, sizeof(ST_DOUBT_ORG_DATA) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pDoubtUserInfo), sizeof(ST_DOUBT_ORG_DATA) );
	}

};

// add by inthesky (2010/7/6-13:10:21) - end
// --------------------------------------------

// 관리자가 불량 유저 등록 / 삭제
class CLCL_ADMIN_CHEAT_USER : public CCommMsg
{
public:
	BYTE*							m_nResult;				// 0:실패 / 1:성공 / 로그서버에서 응답할때만 사용
	ST_REQ_ADMIN_CHEAT_USER*		m_pCheatUserInfo;		// 불량유저 정보, 아이디, 아이피

	void Set( ST_REQ_ADMIN_CHEAT_USER *pInfo, BYTE nresult )
	{
		SetHeader( LCL_ADMIN_CHEAT_USER );
		PushData( _PVAR(m_nResult), &nresult, sizeof(*m_nResult) );
		PushData( _PVAR(m_pCheatUserInfo), pInfo, sizeof(ST_REQ_ADMIN_CHEAT_USER) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_nResult), sizeof(*m_nResult) );
		TakeData( _PVAR(m_pCheatUserInfo), sizeof(ST_REQ_ADMIN_CHEAT_USER) );
	}
};

class CLSV_NOTIFY_CHEAT : public CCommMsg
{
public:
	ST_CHEAT_NOTIFY*		m_pUserInfo;			// 불량유저 정보, 아이디, 아이피

	void Set( ST_CHEAT_NOTIFY *pInfo )
	{
		SetHeader( LSV_NOTIFY_CHEAT );
		PushData( _PVAR(m_pUserInfo), pInfo, sizeof(ST_CHEAT_NOTIFY) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pUserInfo), sizeof(ST_CHEAT_NOTIFY) );
	}

};

class CLSV_NOTIFY_CHEAT_TO_ADMIN : public CCommMsg
{
public:
	ST_CHEAT_NOTIFY_TO_ADMIN*		m_pUserInfo;			// 불량유저 정보, 아이디, 아이피

	void Set( ST_CHEAT_NOTIFY_TO_ADMIN *pInfo )
	{
		SetHeader( LSV_NOTIFY_CHEAT_TO_ADMIN );
		PushData( _PVAR(m_pUserInfo), pInfo, sizeof(ST_CHEAT_NOTIFY_TO_ADMIN) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pUserInfo), sizeof(ST_CHEAT_NOTIFY_TO_ADMIN) );
	}

};

	// Cheat유저 검색 (ID, IP) (Admin -> Server)
class CLCL_SEARCH_CHEAT_INFO : public CCommMsg
{
public:
	ST_SEARCH_CHEAT_INFO*	m_pInfo;

	void Set( ST_SEARCH_CHEAT_INFO*	pInfo )
	{
		SetHeader( LCL_SEARCH_CHEAT_INFO );
		PushData( _PVAR(m_pInfo), pInfo, sizeof(ST_SEARCH_CHEAT_INFO) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pInfo), sizeof(ST_SEARCH_CHEAT_INFO) );
	}
};

class CLSV_SEARCH_RESULT_CHEAT_INFO : public CCommMsg
{
public:
	ST_SERCH_RESULT_CHEAT_INFO*	m_pInfo;

	void Set( ST_SERCH_RESULT_CHEAT_INFO*	pInfo )
	{
		SetHeader( LSV_SEARCH_RESULT_CHEAT_INFO );
		PushData( _PVAR(m_pInfo), pInfo, sizeof(ST_SERCH_RESULT_CHEAT_INFO) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pInfo), sizeof(ST_SERCH_RESULT_CHEAT_INFO) );
	}
};


// 같은방 플레이 유저 검색 (Admin -> Server)
class CLCL_SEARCH_CHEAT_PLAY : public CCommMsg
{
public:
	ST_SEARCH_CHEAT_PLAY* m_pInfo;

	void Set( ST_SEARCH_CHEAT_PLAY*	pInfo )
	{
		SetHeader( LCL_SEARCH_CHEAT_PLAY );
		PushData( _PVAR(m_pInfo), pInfo, sizeof(ST_SEARCH_CHEAT_PLAY) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pInfo), sizeof(ST_SEARCH_CHEAT_PLAY) );
	}
};

class CLCL_SEARCH_CHEAT_PLAY_IN_ROOM : public CCommMsg
{
// public:
// 	ST_SEARCH_CHEAT_PLAY_IN_ROOM* m_pInfo;
// 
// 	void Set( ST_SEARCH_CHEAT_PLAY_IN_ROOM*	pInfo )
// 	{
// 		SetHeader( LCL_SEARCH_CHEAT_PLAY_IN_ROOM );
// 		PushData( _PVAR(m_pInfo), pInfo, sizeof(ST_SEARCH_CHEAT_PLAY_IN_ROOM) );
// 	}
// 	void Get( char* lpdata, int size )
// 	{
// 		if( !GetHeader( lpdata, size ) ) return;
// 		TakeData( _PVAR(m_pInfo), sizeof(ST_SEARCH_CHEAT_PLAY_IN_ROOM) );
// 	}
};

// 같은방 플에이 유저검색 결과 전송 (Server -> Admin)
class CLCL_SEARCH_RESULT_CHEAT_PLAY : public CCommMsg
{
public:
	ST_SEARCH_RESULT_CHEAT_PLAY* m_pInfo;

	void Set( ST_SEARCH_RESULT_CHEAT_PLAY*	pInfo )
	{
		SetHeader( LCL_SEARCH_RESULT_CHEAT_PLAY );
		PushData( _PVAR(m_pInfo), pInfo, sizeof(ST_SEARCH_RESULT_CHEAT_PLAY) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pInfo), sizeof(ST_SEARCH_RESULT_CHEAT_PLAY) );
	}
};

class CLSV_SERVERMSG : public CCommMsg
{
public:

	int *Kind;				// 종류(0:기본 출력 1:메세지 박스 출력)
	short *l_StrChat;		// 채팅 스트링의 길이
	char *StrChat;			// 채팅 스트링
	BOOL *bColorAssigned;	// 텍스트 컬러가 지정되었는가?
	COLORREF *Color;		// 텍스트 컬러

	void Set(int kind, char *chat, BOOL bcolorassigned = FALSE, COLORREF color = RGB(255,0,0)) 
	{
		CString szChat;
		szChat.Format("%s", chat);
		short l_strchat = (short) szChat.GetLength();
	
		SetHeader(LSV_SERVERMSG);
	
		PushData(_PVAR(Kind), &kind, sizeof(*Kind));
		PushData(_PVAR(l_StrChat), &l_strchat, sizeof(l_strchat));
		PushData(_PVAR(StrChat), (void*)szChat.operator LPCTSTR(), l_strchat);
		PushData(_PVAR(bColorAssigned), &bcolorassigned, sizeof(*bColorAssigned));
		PushData(_PVAR(Color), &color, sizeof(*Color));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Kind), sizeof(*Kind));
		TakeData(_PVAR(l_StrChat), sizeof(*l_StrChat));
		TakeData(_PVAR(StrChat), *l_StrChat);
		TakeData(_PVAR(bColorAssigned), sizeof(*bColorAssigned));
		TakeData(_PVAR(Color), sizeof(*Color));
	}
};