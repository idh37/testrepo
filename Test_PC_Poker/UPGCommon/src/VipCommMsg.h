
#pragma once

//#include <UPGPlatform.h>

#include "VipStruct.h"

#define POKERVIP_VER	590		// 베팅로그 관리서버 버전



// 타 게임 호환 메세지 #########################################################

enum
{
	///////// 베팅로그관리서버 <-> 대국실 서버
	VIPSV_HEARTBEAT = 5000,

	VIPSV_CHECK_VERSION,				// 버전 체크
	VIPSV_ASK_LOGIN,					// 로그인 서버로 로그인 요청
	VIPSV_ACCEPT_LOGIN,					// 로그인 허용
	VIPSV_REFUSE_LOGIN,					// 로그인 거부

	VIPSV_SERVICE_CFG,					// 상담설정 정보

	VIPSV_CHATDATA,

	VIPSV_SERVERMSG,

	VIPSV_REQUEST_COUNSEL,				// c->s 상담 요청
	VIPSV_REFUSE_COUNSEL,				// s->c 상담 거부

	VIPSV_WAITING_COUNSEL,				// waiting
	VIPSV_WAITING_CANCEL,				// 기다리는거 취소

	VIPSV_CONNECT_COUNSEL,				// 운영자와 연결
	VIPSV_CONNECT_COUNSEL_FAIL,			// 유저와 연결 실패 

	VIPSV_EXIT_COUNSEL,					// 상담종료

	VIPSV_WAITING_LIST,

};


/////////// 허트비트
class CVIPSV_HEARTBEAT : public CCommMsg
{
public:

	void Set() 
	{
		SetHeader(VIPSV_HEARTBEAT);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};


/////////// 인증키 요구 
class CVIPSV_CHECK_VERSION : public CCommMsg
{
public:
	int *pnVersion;

	void Set(int nVersion)
	{
		SetHeader(VIPSV_CHECK_VERSION);
		PushData(_PVAR(pnVersion), &nVersion, sizeof(*pnVersion));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pnVersion), sizeof(*pnVersion));
	}
};

// 로그인 서버로 로그인 요청
class CVIPSV_ASK_LOGIN : public CCommMsg
{
public:
	ST_VIPSERVER_LOGIN_INFO*	pLoginInfo;

	void Set( ST_VIPSERVER_LOGIN_INFO* plogininfo )
	{
		SetHeader( VIPSV_ASK_LOGIN );
		PushData( _PVAR(pLoginInfo), plogininfo, sizeof(*pLoginInfo) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(pLoginInfo), sizeof(*pLoginInfo) );
	}
};



// 로그인 허용
class CVIPSV_ACCEPT_LOGIN : public CCommMsg
{
public:
	int *nType;			// 서버인지 유저인지

	void Set(int ntype)
	{
		SetHeader(VIPSV_ACCEPT_LOGIN);
		PushData(_PVAR(nType), &ntype, sizeof(*nType));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nType), sizeof(*nType));
	}
};

// 로그인 거부 
class CVIPSV_REFUSE_LOGIN : public CCommMsg
{
public:
	int *nType;			// 거부이유

	void Set(int ntype)
	{
		SetHeader( VIPSV_REFUSE_LOGIN );
		PushData(_PVAR(nType), &ntype, sizeof(*nType));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nType), sizeof(*nType));
	}
};

class CVIPSV_SERVICE_CFG : public CCommMsg
{
public:
	ST_SERVICE_ON_OFF *stSerceInfo;

	void Set( ST_SERVICE_ON_OFF* pserviceinfo )
	{
		SetHeader( VIPSV_SERVICE_CFG );
		PushData( _PVAR(stSerceInfo), pserviceinfo, sizeof(*stSerceInfo) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(stSerceInfo), sizeof(*stSerceInfo) );
	}

};

class CVIPSV_CHATDATA : public CCommMsg
{
public:

	char *ID;			// 아이디 스트링
	short *l_StrChat;	// 채팅 스트링의 길이
	char *StrChat;		// 채팅 스트링
	int	*nRoom;

	void Set(char* id, char *chat, int room) 
	{
		char strId[16]={0,};
		CString szChat;
		szChat.Format("%s", chat);
		short l_strchat = (short) szChat.GetLength();
	
		SetHeader(VIPSV_CHATDATA);
	
		if(strlen(id) < 16) strcpy(strId, id);
	
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(l_StrChat), &l_strchat, sizeof(l_strchat));
		PushData(_PVAR(StrChat), (void*)szChat.operator LPCTSTR(), l_strchat);
		PushData(_PVAR(nRoom), &room, sizeof(*nRoom));

	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(l_StrChat), sizeof(*l_StrChat));
		TakeData(_PVAR(StrChat), *l_StrChat);
		TakeData(_PVAR(nRoom), sizeof(*nRoom));
	}

};


class CVIPSV_SERVERMSG : public CCommMsg
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
	
		SetHeader(VIPSV_SERVERMSG);
	
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

class CVIPSV_REQUEST_COUNSEL : public CCommMsg
{
public:
	void Set() 
	{
		SetHeader(VIPSV_REQUEST_COUNSEL);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

class CVIPSV_REFUSE_COUNSEL : public CCommMsg
{
public:
	void Set() 
	{
		SetHeader(VIPSV_REFUSE_COUNSEL);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

class CVIPSV_WAITING_COUNSEL : public CCommMsg
{
public:
	int *nCount;			// 거부이유

	void Set( int ncount) 
	{
		SetHeader(VIPSV_WAITING_COUNSEL);
		PushData(_PVAR(nCount), &ncount, sizeof(*nCount));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nCount), sizeof(*nCount));
	}
};

class CVIPSV_WAITING_CANCEL : public CCommMsg
{
public:
	void Set() 
	{
		SetHeader(VIPSV_WAITING_CANCEL);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

class CVIPSV_CONNECT_COUNSEL : public CCommMsg
{
public:
	ST_WAITING_USERINFO		*pInfo;

	void Set(ST_WAITING_USERINFO* pinfo) 
	{
		SetHeader(VIPSV_CONNECT_COUNSEL);
		PushData(_PVAR(pInfo), pinfo, sizeof(ST_WAITING_USERINFO));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pInfo), sizeof(ST_WAITING_USERINFO));
	}
};

class CVIPSV_CONNECT_COUNSEL_FAIL : public CCommMsg
{
public:
	ST_WAITING_USERINFO		*pInfo;

	void Set(ST_WAITING_USERINFO* pinfo) 
	{
		SetHeader(VIPSV_CONNECT_COUNSEL_FAIL);
		PushData(_PVAR(pInfo), pinfo, sizeof(ST_WAITING_USERINFO));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pInfo), sizeof(ST_WAITING_USERINFO));
	}
};

class CVIPSV_EXIT_COUNSEL : public CCommMsg
{
public:
	ST_WAITING_USERINFO		*pInfo;
	short *l_StrChat;	// 채팅 스트링의 길이
	char *StrChat;		// 채팅 스트링
	int	*nType;

	void Set(ST_WAITING_USERINFO* pinfo, char *chat, int type ) 
	{
		CString szChat;
		szChat.Format("%s", chat);
		short l_strchat = (short) szChat.GetLength();

		SetHeader(VIPSV_EXIT_COUNSEL);
		PushData(_PVAR(pInfo), pinfo, sizeof(ST_WAITING_USERINFO));
		PushData(_PVAR(l_StrChat), &l_strchat, sizeof(l_strchat));
		PushData(_PVAR(StrChat), (void*)szChat.operator LPCTSTR(), l_strchat);
		PushData(_PVAR(nType), &type, sizeof(*nType));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pInfo), sizeof(ST_WAITING_USERINFO));
		TakeData(_PVAR(l_StrChat), sizeof(*l_StrChat));
		TakeData(_PVAR(StrChat), *l_StrChat);
		TakeData(_PVAR(nType), sizeof(*nType));
	}
};

class CVIPSV_WAITING_LIST : public CCommMsg
{
public:
	
	int						*TotUser;
	ST_WAITING_USERINFO		*ArrayUI;

	void Set(int totuser, ST_WAITING_USERINFO* pArrayUI) 
	{
		SetHeader(VIPSV_WAITING_LIST);
		PushData(_PVAR(TotUser), &totuser, sizeof(*TotUser));
		PushData(_PVAR(ArrayUI), pArrayUI, sizeof(ST_WAITING_USERINFO) * totuser);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TotUser), sizeof(*TotUser));
		TakeData(_PVAR(ArrayUI), sizeof(ST_WAITING_USERINFO) * (*TotUser));
	}
};