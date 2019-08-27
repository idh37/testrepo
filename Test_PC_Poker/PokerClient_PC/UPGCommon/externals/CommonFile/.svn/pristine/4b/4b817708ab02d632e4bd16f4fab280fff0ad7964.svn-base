#ifndef __AGENTMSG_H__
#define __AGENTMSG_H__

#include "CommMsgDef.h"
#include "CommMsgDef_Game.h"

/////////////////////////////////////////////////////
///////////     [관리자 모드 작업]     //////////////
/////////////////////////////////////////////////////

#define MAX_ADMIN_NUM           (40) // 최대 관리자 수

//관리자 정보
typedef struct
{
	char ID[16]; // 관리자 아이디
	char Grade;  // 등급
	char UserIP[24];			// 관리자의 접속 IP정보	
} ADMIN_INFO;

//총관리자
typedef struct
{
	int nTotal;
	ADMIN_INFO admin[MAX_ADMIN_NUM];
} ADMIN_MANAGER; //서버 공통

//에이젼트 관리용
typedef struct
{
	int NowNum;
	ADMIN_MANAGER AdminMan;
} AGENT_ADMIN;

//관리자 메세지
typedef struct
{
	int  UNum;
	char ID[16];
	char Message[130]; // 실제로는 128까지만 사용된다
} ADMIN_MESSAGE;


/////////////////////////////////////////////////////////////////////////////
//통합포커 머니 사용 게임 에이전트 프로토콜
/////////////////////////////////////////////////////////////////////////////

// 서버용 메세지 #########################################################
enum SIGNAL_AGENT
{
	// [서버 에저이전트 작업]
	NM_AGENT_CONNECT = 5005, // 에이전트가 접속되었다
	NM_AGENT_NEWUSER,
	NM_AGENT_OUTUSER,	
	NM_AGENT_NEWUSERERROR,	 // 중복 요청
	NM_AGENT_USERLIST,		 // 유저 목록 요청
	NM_AGENT_DUBLECON,		 // 유저 목록 요청
	NM_AGENT_USERLIST1,
	NM_AGENT_USERLIST2,
	NM_AGENT_USERLIST3,
	NM_AGENT_USERLIST4,
	NM_AGENT_USERLIST5,
	NM_AGENT_USERLIST_R1,
	NM_AGENT_AVERAGE,       // 잭팟 금액의 평균을 구한다.저
	//### [관리자 모드 작업] ###
	NM_AGENT_ADMININFO,		// 관리자 권한 아이디를 전송한다.

	//블랙유저 관리 2006.2.3
	NM_AGENT_ASK_ADDBADUSERID,	// 블랙유저 추가(ID) 마스터에게 전달해 달라 함 각서버들에게
	NM_AGENT_ADD_BADUSERIP,		// 각서버에 불량 아이피로 접속해  있는 유저들에게 불량등록을 하게 한다.
	NM_AGENT_ASK_BADUSERID_GETIP,	// 아이디로 유저 아이피를 얻어 저장한다
	NM_AGENT_RESULT_ADDBADUSER,	// 에이전트가 가 각서버들에게 물어본후 결과값을 전달해준다.

	NM_AGENT_ADDAUTOUSER,		// 훌라
	NM_AGENT_DELAUTOUSER,		// 훌라 

	NM_AGENT_DISCON_NEWUSER,	// 유저 채널(서버간) 이동

// 이중접속 검사	vener, 2010/09/14
	NM_AGENT_ASK_DUPLICATECONNECTION,	// 이중 접속자 검사 요청(서버 -> 에이전트)
	NM_AGENT_RES_DUPLICATECONNECTION,	// 이중 접속자 응답(에이전트 -> 서버)	

//< CYS 111010
	// 오토플레이어 목록 보내기
	NM_AGENT_AUTO_USERLIST_R1,	   // 오토유저 목록	
	NM_AGENT_AUTO_USERLIST,		   // 오토유저 목록 요청 
	NM_AGENT_AUTO_FORCEDISCONNECT, // 오토유저 리스트에서  강제 삭제
//> CYS 111010
};


/////////////////////////////////////////////////////
////////////    [서버 에이전트 작업]   //////////////
/////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// 서버에이전트 작업
// 접속
class CNM_AGENT_CONNECT : public CCommMsg
{
public:	

	int *TotalNum;
	INT64 *JackPot; // 잭팟금액	
	
	void Set(int tot, INT64 jackpot)
	{
		SetHeader( NM_AGENT_CONNECT);
		PushData(_PVAR(TotalNum), &tot,sizeof(*TotalNum) );		
		PushData(_PVAR(JackPot), &jackpot,sizeof(*JackPot) );		
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TotalNum), sizeof(*TotalNum) );
		TakeData(_PVAR(JackPot), sizeof(*JackPot) );
	}
};

// 유저 목록 요청
class CNM_AGENT_USERLIST : public CCommMsg
{
public:
	
	int *Reserve;	
	void Set(int reserve)
	{
		SetHeader( NM_AGENT_USERLIST);
		PushData(_PVAR(Reserve), &reserve,sizeof(*Reserve) );		
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Reserve), sizeof(*Reserve) );				
	}
};


/////////// 1차 목록 400 명
class CNM_AGENT_USERLIST_R1 : public CCommMsg
{
public:

	int *TotNum;
	int *Count;
	AGENT_USER *ArraySUI;	

	void Set(int totnum, AGENT_USER* ppSUI,int count)
	{
		SetHeader( NM_AGENT_USERLIST_R1);
		PushData(_PVAR(TotNum), &totnum,sizeof(*TotNum) );
		PushData(_PVAR(Count),&count, sizeof(*Count));
		PushData(_PVAR(ArraySUI),ppSUI, sizeof(AGENT_USER) * (*TotNum) );		
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TotNum), sizeof(*TotNum) );		
		TakeData(_PVAR(Count), sizeof(*Count));
		TakeData(_PVAR(ArraySUI), sizeof(AGENT_USER) * (*TotNum) );	
	}
};

//< CYS 111010
// 유저 목록 요청
class CNM_AGENT_AUTO_USERLIST : public CCommMsg
{
public:

	int *Reserve;	
	void Set(int reserve)
	{
		SetHeader( NM_AGENT_AUTO_USERLIST);
		PushData(_PVAR(Reserve), &reserve,sizeof(*Reserve) );		
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Reserve), sizeof(*Reserve) );				
	}
};

/////////// 오토유저 목록
class CNM_AGENT_AUTO_USERLIST_R1 : public CCommMsg
{
public:

	int *TotNum;
	int *Count;
	AGENT_USER *ArraySUI;	

	void Set(int totnum, AGENT_USER* ppSUI,int count)
	{
		SetHeader( NM_AGENT_AUTO_USERLIST_R1);
		PushData(_PVAR(TotNum), &totnum,sizeof(*TotNum) );
		PushData(_PVAR(Count),&count, sizeof(*Count));
		PushData(_PVAR(ArraySUI),ppSUI, sizeof(AGENT_USER) * (*TotNum) );		
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TotNum), sizeof(*TotNum) );		
		TakeData(_PVAR(Count), sizeof(*Count));
		TakeData(_PVAR(ArraySUI), sizeof(AGENT_USER) * (*TotNum) );	
	}
};

/////////// 중복접속자
class CNM_AGENT_AUTO_FORCEDISCONNECT : public CCommMsg
{
public:	

	int *TotNum;
	AGENT_USER *ArraySUI;

	void Set(int totnum, AGENT_USER* ppSUI)
	{
		SetHeader( NM_AGENT_AUTO_FORCEDISCONNECT );
		PushData(_PVAR(TotNum), &totnum,sizeof(*TotNum) );
		PushData(_PVAR(ArraySUI),ppSUI, sizeof(AGENT_USER));
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TotNum), sizeof(*TotNum) );
		TakeData(_PVAR(ArraySUI), sizeof(AGENT_USER));	
	}
};
//> CYS 111010

/////////// 새로운 유저 입장
class CNM_AGENT_NEWUSER : public CCommMsg
{
public:
	int *TotNum;
	AGENT_USER *ArraySUI;
	INT64 *JackPot; // 잭팟금액

	void Set(int totnum, AGENT_USER* ppSUI, INT64 jackpot)
	{
		SetHeader( NM_AGENT_NEWUSER);
		PushData(_PVAR(TotNum), &totnum,sizeof(*TotNum) );
		PushData(_PVAR(ArraySUI),ppSUI, sizeof(AGENT_USER));
		PushData(_PVAR(JackPot),&jackpot, sizeof(*JackPot));
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TotNum), sizeof(*TotNum) );
		TakeData(_PVAR(ArraySUI), sizeof(AGENT_USER));	
		TakeData(_PVAR(JackPot), sizeof(*JackPot));
	}
};

///////////  유저 아웃
class CNM_AGENT_OUTUSER : public CCommMsg
{
public:
	
	int *TotNum;
	AGENT_USER *ArraySUI;
	INT64 *JackPot; // 잭팟금액	
	
	void Set(int totnum, AGENT_USER* ppSUI, INT64 jackpot)
	{
		SetHeader( NM_AGENT_OUTUSER);
		PushData(_PVAR(TotNum), &totnum,sizeof(*TotNum) );
		PushData(_PVAR(ArraySUI),ppSUI, sizeof(AGENT_USER));
		PushData(_PVAR(JackPot),&jackpot, sizeof(*JackPot));
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TotNum), sizeof(*TotNum) );
		TakeData(_PVAR(ArraySUI), sizeof(AGENT_USER));	
		TakeData(_PVAR(JackPot), sizeof(*JackPot));
	}
};


/////////// 중복접속 확인
class CNM_AGENT_DUBLECON : public CCommMsg
{
public:
	int *TotNum;
	AGENT_USER *ArraySUI;

	void Set(int totnum, AGENT_USER* ppSUI)
	{
		SetHeader(NM_AGENT_DUBLECON);
		PushData(_PVAR(TotNum), &totnum,sizeof(*TotNum) );
		PushData(_PVAR(ArraySUI),ppSUI, sizeof(AGENT_USER));
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TotNum), sizeof(*TotNum) );
		TakeData(_PVAR(ArraySUI), sizeof(AGENT_USER));	
	}
};

/////////// 중복접속자
class CNM_AGENT_NEWUSERERROR : public CCommMsg
{
public:	

	int *TotNum;
	AGENT_USER *ArraySUI;
	
	void Set(int totnum, AGENT_USER* ppSUI)
	{
		SetHeader( NM_AGENT_NEWUSERERROR);
		PushData(_PVAR(TotNum), &totnum,sizeof(*TotNum) );
		PushData(_PVAR(ArraySUI),ppSUI, sizeof(AGENT_USER));
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TotNum), sizeof(*TotNum) );
		TakeData(_PVAR(ArraySUI), sizeof(AGENT_USER));	
	}
};


// 잭팟 평균
class CNM_AGENT_AVERAGE : public CCommMsg
{
public:


	INT64 *JackPotMoney;
	void Set(INT64 jackpot)
	{
		SetHeader(NM_AGENT_AVERAGE);
		PushData(_PVAR(JackPotMoney), &jackpot, sizeof(*JackPotMoney));		
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(JackPotMoney), sizeof(*JackPotMoney));
	}
};

/////////// 관리자 아디를 전송한다
class CNM_AGENT_ADMININFO : public CCommMsg
{
public:

	int *TotNum;
	ADMIN_INFO *AdminInfo;
	void Set(int totnum, ADMIN_INFO* ppCI)
	{
		SetHeader(NM_AGENT_ADMININFO);
		PushData(_PVAR(TotNum), &totnum, sizeof(*TotNum));
		PushData(_PVAR(AdminInfo), ppCI, sizeof(ADMIN_INFO)*(*TotNum));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TotNum), sizeof(*TotNum));
		TakeData(_PVAR(AdminInfo),  sizeof(ADMIN_INFO)*(*TotNum));
	}
};

// 블랙유저 추가 마스터에게 전달해 달라 함 각서버들에게 2006.2.3
class CNM_AGENT_ASK_ADDBADUSERID : public CCommMsg
{
public:
	ADMIN_ADDBADUSER_DATA *pADBad;

	void Set(ADMIN_ADDBADUSER_DATA *padmsg) 
	{		
		SetHeader(NM_AGENT_ASK_ADDBADUSERID);	
		PushData(_PVAR(pADBad), padmsg, sizeof(*pADBad));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pADBad), sizeof(*pADBad));
	}
};

// 각서버에 불량 아이피로 접속해  있는 유저들에게 불량등록을 하게 한다. 2006.2.3
class CNM_AGENT_ADD_BADUSERIP : public CCommMsg
{
public:
	ADMIN_ADDBADUSER_DATA *pADBad;

	void Set(ADMIN_ADDBADUSER_DATA *padmsg) 
	{		
		SetHeader(NM_AGENT_ADD_BADUSERIP);	
		PushData(_PVAR(pADBad), padmsg, sizeof(*pADBad));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pADBad), sizeof(*pADBad));
	}
};

class CNM_AGENT_ASK_BADUSERID_GETIP : public CCommMsg
{
public:
	ADMIN_ADDBADUSER_DATA *pADBad;

	void Set(ADMIN_ADDBADUSER_DATA *padmsg) 
	{		
		SetHeader(NM_AGENT_ASK_BADUSERID_GETIP);	
		PushData(_PVAR(pADBad), padmsg, sizeof(*pADBad));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pADBad), sizeof(*pADBad));
	}
};

// 마스터가 각서버들에게 물어본후 결과값을 전달해준다. 2006.2.3
class CNM_AGENT_RESULT_ADDBADUSER : public CCommMsg
{
public:
	int	*nResultFlg;	// 0 : 정상 등록 1:아이디등록 실패, 2 아이피 등록 실패 3: 쓰레기로 다 실패
	ADMIN_ADDBADUSER_DATA *pADBad;

	void Set(int nresult , ADMIN_ADDBADUSER_DATA *padmsg) 
	{		
		SetHeader(NM_AGENT_RESULT_ADDBADUSER);	
		PushData(_PVAR(nResultFlg), &nresult, sizeof(*nResultFlg));
		PushData(_PVAR(pADBad), padmsg, sizeof(*pADBad));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nResultFlg), sizeof(*nResultFlg));		
		TakeData(_PVAR(pADBad), sizeof(*pADBad));
	}
};	



// 아래 추가
class CNM_AGENT_ADDAUTOUSER : public CCommMsg
{
public:
	char	*ID;	// 0 : 정상 등록 1:아이디등록 실패, 2 아이피 등록 실패 3: 쓰레기로 다 실패

	void Set(char *pID) 
	{		
		char strid[16]={0,};
		if(pID != NULL) strncpy(strid, pID, 15);

		SetHeader(NM_AGENT_ADDAUTOUSER);
		PushData(_PVAR(ID), strid, 16);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ID), 16);
	}
};	

class CNM_AGENT_DELAUTOUSER : public CCommMsg
{
public:
	char	*ID;	// 0 : 정상 등록 1:아이디등록 실패, 2 아이피 등록 실패 3: 쓰레기로 다 실패

	void Set(char *pID) 
	{		
		char strid[16]={0,};
		if(pID != NULL) strncpy(strid, pID, 15);

		SetHeader(NM_AGENT_DELAUTOUSER);
		PushData(_PVAR(ID), strid, 16);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ID), 16);
	}
};	

class CNM_AGENT_DISCON_NEWUSER : public CCommMsg
{
public:
	AGENT_USER *ArraySUI;

	void Set(AGENT_USER* pSUI)
	{
		SetHeader( NM_AGENT_DISCON_NEWUSER);
		PushData(_PVAR(ArraySUI),pSUI, sizeof(*pSUI));
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ArraySUI), sizeof(*ArraySUI));	
	}
};


// 이중접속 검사	vener, 2010/09/14
/////////// 중복접속인지 확인 요청/응답
template <SIGNAL_AGENT _eSignal = NM_AGENT_RES_DUPLICATECONNECTION>
class CNM_AGENT_CHECK_DUPLICATECONNECTION : public CCommMsg
{
public:
	int* nSid;
	char* szID;
	bool* bIsDupUser;

	void Set(int in_nSid, const char* in_szID, bool in_bIsDupUser = false)
	{
		char szIDBuf[ID_LEN] = {0, };
		::strncpy(szIDBuf, in_szID, ID_LEN-1);

		SetHeader(_eSignal);	// NM_AGENT_ASK_DUPLICATECONNECTION, NM_AGENT_RES_DUPLICATECONNECTION
		PushData(_PVAR(nSid), &in_nSid,sizeof(*nSid) );
		PushData(_PVAR(szID), szIDBuf, ID_LEN);
		PushData(_PVAR(bIsDupUser), &in_bIsDupUser, sizeof(*bIsDupUser));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nSid), sizeof(*nSid) );
		TakeData(_PVAR(szID), ID_LEN);
		TakeData(_PVAR(bIsDupUser), sizeof(*bIsDupUser));
	}
};

#endif
