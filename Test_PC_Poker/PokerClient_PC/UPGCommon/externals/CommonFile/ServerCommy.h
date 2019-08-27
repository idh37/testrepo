#ifndef __DEFINECOMMYDATA__
#define __DEFINECOMMYDATA__

// 서버들과 서로의 인식을 위한 메시지 
extern UINT uCmmHaloMsgSC;	
extern UINT uCmmHaloMsgCS;
extern UINT uCmmGotBootMsgSC;	// 서버가 부팅된것을 인식했다고 관리기에서 보낸다.

// 서버 상태 메시지 
extern UINT uCmmGameStatusSC;
extern UINT uCmmGameStatusCS;

// 서버 메시지 출력
extern UINT uCmmServerMsgSendSC;
extern UINT uCmmServerMsgSendCS;

// 서버 신규 접속 막음 
extern UINT uCmmStopAcceptSC;
extern UINT uCmmStopAcceptCS;


// 서버 다운 메시지 출력 
extern UINT uCmmServerDownMsgSC;
extern UINT uCmmServerDownMsgCS;


extern UINT uCmmServerUserSizeSC;
extern UINT uCmmServerUserSizeCS;

// DB Agent용 메시지 
extern UINT uCmmDBAgentStatusSC;	// 상태를 요청 한다.
extern UINT uCmmDBAgentStatusCS;	// 

extern UINT uCmmDisbandUserANS;

// 게임 클라이언트 체크섬 관련
extern UINT uCmmClientChecksumSC;
extern UINT uCmmClientChecksumCS;

typedef struct {
	int  gamecode;
	int  servercode;
	int  port;
	char name[64];
	char path[256];
} tag_ServerHalo;
typedef struct {
	int  gamecode;
	int  servercode;
	int  port;
	char name[64];
	char path[256];
	int  ver;
} tag_ServerHalo2;

typedef struct {
	int ServerCode;
	char name[64];
} tag_ServerDown; // 내려갈때 메시지 

typedef struct {
	int nCode;
	char strMsg[256];
}  tag_SvrMsg;


// DB AGENT용 구조체 
typedef struct {
	int GC;
	int SC;
	int Status;
	int Users;
	char Name[64];
} tag_DBAgentitem;

typedef struct {
	char Name[64];
	int  ServerSize;
	tag_DBAgentitem Item[100];
} tag_DBAgentList;

typedef struct {
	int type;// 닉네임 수정 2006/11/24
	char ID[20];
} tag_DisbandUser;

typedef struct {
	int Code;
	int type;// 닉네임 수정 2006/11/24
	char ID[20];
	char strMsg[128];
} tag_SingleMsg;

typedef struct {
	int  Index;
	int type;// 닉네임 수정 2006/11/24
	char ID[20];
} tag_MsgSendOK;


typedef struct {
	INT64	CheckSum;			// 체크섬 
	BOOL	bSumFlag;			// 사용중인지 아닌지
} tag_CheckSumData;

///////////////////////////////////////////////////////////
// Function
extern BOOL InitNetmarbleCommn(void);

extern void M_HaloGameServer(HWND myhwnd = HWND_BROADCAST);			// 서버들에게 관리기의 존재를 알린다.
extern void M_ImHereBridge(HWND myhwnd,int gamecode,int servercode,int port,char *name);
extern void M_ImHereBridge2(HWND myhwnd,int gamecode,int servercode,int port,char *name,int ver);
extern void M_GotServerBootMsg(HWND myhwnd,HWND svrhwnd);

extern void M_GetServerStatus(HWND myhwnd,HWND svrhandle);			// 서버들에게 상태를 물어본다.
extern void M_SendMySetatus(HWND myhwnd,int status,int usersize);	// 자신의 상태를 관리기에게 알린다.


extern void M_ServerDownNow(HWND myhwnd,HWND svrhandle);	// 서버에게 다운 메시지를 준다. 
extern void M_ImDieNow(HWND svrhandle);						// 나 주거유우우 라고 관리기에게 알린다. 3분뒤에 핸들 체크하여 강제 종료.

extern void SetBridgeHandle(HWND hwnd);
extern void SendMsgToServer(HWND svrhwnd,HWND myhwnd,int code,char *msg);

extern void SendUserSizeToBridge(HWND svrhandle,int usersize);

// 서버 접속 허용 / 안허용 플레그 설정 
extern void M_SetStopAccept(HWND myhwnd,HWND svrhandle,int bStopAccept);
extern void M_ServerAcceptStatus(HWND svrhandle,int bStopAccept);

// 브릿지 핸들 찾기.
extern HWND M_FindBridgeHandle();

// DB Agent function
extern void SendDBAList(HWND myhwnd,tag_DBAgentList *pList);	// 전체 리스트 받는다.
extern void SetDBAItem(HWND myhwnd,tag_DBAgentitem *pList);	// 한개에 대해서 받기 

// 사용자 강제 종료 

extern void M_DisConnectAns(HWND svrhandle,BOOL tf);


extern void M_SingleMsgSecc(HWND  myhwnd, int type, char *id,int index);
extern void M_DisconnectUser(HWND svrhandle,HWND  myhwnd,int type, char *id);
extern void SendMsgToServerSingleUser(HWND svrhwnd,HWND myhwnd,int code,int type, char *msg,char *id);

// 클라이언트 체크섬 
extern void M_CheckSumSendToServer(HWND svrhandle,HWND myhwnd,tag_CheckSumData *data);
extern void M_CheckSumReQuestToBridge(HWND svrhandle);

//extern void M_SingleMsgSecc(HWND  myhwnd,char *id,int index);
//extern void M_DisconnectUser(HWND svrhandle,HWND myhwnd,char *id);
//extern void SendMsgToServerSingleUser(HWND svrhwnd,HWND myhwnd,int code,char *msg,char *id);


#endif