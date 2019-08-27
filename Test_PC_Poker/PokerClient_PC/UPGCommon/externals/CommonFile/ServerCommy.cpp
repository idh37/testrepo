#include"stdafx.h"
#include<stdio.h>

#include "servercommy.h"

// 서버들과 서로의 인식을 위한 메시지 
UINT uCmmHaloMsgSC = 0;	
UINT uCmmHaloMsgCS = 0;
UINT uCmmGotBootMsgSC = 0;

// 서버 상태 메시지 
UINT uCmmGameStatusSC = 0;
UINT uCmmGameStatusCS = 0;

// 서버 메시지 출력
UINT uCmmServerMsgSendSC = 0;
UINT uCmmServerMsgSendCS = 0;

// 서버 신규 접속 막음 
UINT uCmmStopAcceptSC = 0;
UINT uCmmStopAcceptCS = 0;


// 서버 다운 메시지 출력 
UINT uCmmServerDownMsgSC = 0;
UINT uCmmServerDownMsgCS = 0;


UINT uCmmServerUserSizeSC = 0;
UINT uCmmServerUserSizeCS = 0;

UINT uCmmDisbandUserANS = 0;


UINT uCmmClientChecksumSC = 0;
UINT uCmmClientChecksumCS = 0;
HWND _BridgeServerHandle = HWND_BROADCAST;



///////////////////////////////////////////////////////////
// Function
BOOL InitNetmarbleCommn(void)
{
	
	uCmmHaloMsgSC		= RegisterWindowMessage("NetmarbleHaloSC1");
	uCmmHaloMsgCS	    = RegisterWindowMessage("NetmarbleHaloCS2");
	uCmmGotBootMsgSC	= RegisterWindowMessage("NetmarbleGotBootMsg11");

	uCmmGameStatusSC	= RegisterWindowMessage("NetmarbleGameStatusSC3");
	uCmmGameStatusCS	= RegisterWindowMessage("NetmarbleGameStatusSC4");

	uCmmServerMsgSendSC	= RegisterWindowMessage("NetmarbleMsgSendSC5");
	uCmmServerMsgSendCS	= RegisterWindowMessage("NetmarbleMsgSendCS6");

	uCmmStopAcceptSC	= RegisterWindowMessage("NetmarbleAcceptSC7");
	uCmmStopAcceptCS	= RegisterWindowMessage("NetmarbleAcceptCS8");

	uCmmServerDownMsgSC	= RegisterWindowMessage("NetmarbleServerDownNow9");
	uCmmServerDownMsgCS	= RegisterWindowMessage("NetmarbleImDownNow10");

	uCmmServerUserSizeSC = RegisterWindowMessage("NetmarbleServerUserSizeSc12");
	uCmmServerUserSizeCS = RegisterWindowMessage("NetmarbleServerUserSizeCs13");

	uCmmDisbandUserANS = RegisterWindowMessage("Netmarble.Server.User.Disconnect");

	uCmmClientChecksumSC = RegisterWindowMessage("Netmarble.Bridge.CheckSumAns");
	uCmmClientChecksumCS = RegisterWindowMessage("Netmarble.Server.CheckSumRequest");
	return 1;
}

void M_HaloGameServer(HWND myhwnd)
{
	if (FALSE == ::PostMessage(HWND_BROADCAST,uCmmHaloMsgSC,(WPARAM)myhwnd,0))
	{
#if defined(_ONLY_SERVER)
		GLOBALLOG_ERROR("PostMessage Fail.");
#endif
	}
}

void M_GotServerBootMsg(HWND myhwnd,HWND svrhwnd)
{
	if (FALSE == ::PostMessage(svrhwnd,uCmmGotBootMsgSC,(WPARAM)myhwnd,0))
	{
#if defined(_ONLY_SERVER)
		GLOBALLOG_ERROR("PostMessage Fail.");
#endif
	}
}

void M_ImHereBridge(HWND myhwnd,int gamecode,int servercode,int port,char *name)
{
	tag_ServerHalo cData;

	sprintf(cData.name,"%s",name);
	GetModuleFileName(AfxGetInstanceHandle(),cData.path,256);
	cData.gamecode = gamecode;
	cData.port = port;
	cData.servercode = servercode;

	COPYDATASTRUCT SendDataStruct;
	SendDataStruct.cbData = sizeof(tag_ServerHalo);
	SendDataStruct.dwData = 0;
	SendDataStruct.lpData =(void*)&cData;

	::SendMessage( M_FindBridgeHandle(),WM_COPYDATA,(WPARAM)myhwnd,(LPARAM)&SendDataStruct);
}

void M_ImHereBridge2(HWND myhwnd,int gamecode,int servercode,int port,char *name,int ver)
{
	tag_ServerHalo2 cData;

	sprintf(cData.name,"%s",name);
	GetModuleFileName(AfxGetInstanceHandle(),cData.path,256);
	cData.gamecode = gamecode;
	cData.port = port;
	cData.servercode = servercode;
	cData.ver = ver;

	COPYDATASTRUCT SendDataStruct;
	SendDataStruct.cbData = sizeof(tag_ServerHalo2);
	SendDataStruct.dwData = 0;
	SendDataStruct.lpData =(void*)&cData;
	
	::SendMessage( M_FindBridgeHandle(),WM_COPYDATA,(WPARAM)myhwnd,(LPARAM)&SendDataStruct);
}

HWND M_FindBridgeHandle()
{
	char pcname[256] = {0,}; DWORD namesize = 255;
	GetComputerName( pcname, &namesize );

	char classname[256];
	strcpy( classname, "#32770" );

	// FindWindow로 찾으면 좋긴한데 이전 bridge와 호환이 안되는 관계로
	// 모든 윈도우를 뒤지는 방법으로만 해야한다. -_-;;

	int max_wnd_count = 1024;
	HWND hWnd = ::GetTopWindow( NULL );
		
	char strBuf[1024];
	CString str = _T("");
	memset( strBuf, 0, sizeof(strBuf) );

	HWND BridgeHandle = NULL;

	while( max_wnd_count > 0 && hWnd )
	{
		max_wnd_count--;

		::GetClassName( hWnd, strBuf, 1023);
		if( strcmp( strBuf, classname ) == 0 )
		{
			::GetWindowText( hWnd, strBuf, 1023 );
			str = strBuf;
			if( str.Find( pcname, 0 ) >= 0 )
			{
				BridgeHandle = hWnd;
				break;
			}
		}

		hWnd = ::GetNextWindow( hWnd, GW_HWNDNEXT );
	}

	return BridgeHandle;
}


void M_GetServerStatus(HWND myhwnd,HWND svrhandle)		// 서버들에게 상태를 물어본다.
{
	if (FALSE == ::PostMessage(svrhandle,uCmmGameStatusSC,(WPARAM)myhwnd,0))
	{
#if defined(_ONLY_SERVER)
		GLOBALLOG_ERROR("PostMessage Fail.");
#endif
	}
}

void M_SendMySetatus(HWND myhwnd,int status,int usersize) // 자신의 상태를 관리기에게 알린다.
{
	int sz = (status << 16) + usersize;
	if (FALSE == ::PostMessage(_BridgeServerHandle,uCmmGameStatusCS,(WPARAM)myhwnd,sz))
	{
#if defined(_ONLY_SERVER)
		GLOBALLOG_ERROR("PostMessage Fail.");
#endif
	}
}


void M_ServerDownNow(HWND myhwnd,HWND svrhandle)		// 서버에게 다운 메시지를 준다. 
{
	if (FALSE == PostMessage(svrhandle,uCmmServerDownMsgSC,(WPARAM)myhwnd,0))
	{
#if defined(_ONLY_SERVER)
		GLOBALLOG_ERROR("PostMessage Fail.");
#endif
	}
}

void M_ImDieNow(HWND svrhandle)				// 나 주거유우우 라고 관리기에게 알린다. 3분뒤에 핸들 체크하여 강제 종료.
{
	if (FALSE == PostMessage(_BridgeServerHandle,uCmmServerDownMsgCS,(WPARAM)svrhandle,0))
	{
#if defined(_ONLY_SERVER)
		GLOBALLOG_ERROR("PostMessage Fail.");
#endif
	}
}

void SetBridgeHandle(HWND hwnd)
{
	_BridgeServerHandle = hwnd;
}


void SendMsgToServer(HWND svrhwnd,HWND myhwnd,int code,char *msg)
{
	tag_SvrMsg cData;

	sprintf(cData.strMsg,"%s",msg);
	cData.nCode = code;

	COPYDATASTRUCT SendDataStruct;
	SendDataStruct.cbData = sizeof(tag_SvrMsg);
	SendDataStruct.dwData = 0;
	SendDataStruct.lpData =(void*)&cData;
	if(svrhwnd != NULL)	::SendMessageTimeout(svrhwnd,WM_COPYDATA,(WPARAM)myhwnd,(LPARAM)&SendDataStruct,SMTO_NORMAL,12000,NULL);
}

// 닉네임 수정 2006/11/24
void SendMsgToServerSingleUser(HWND svrhwnd,HWND myhwnd,int code,int type, char *msg,char *id)
{
	tag_SingleMsg cData;
	sprintf(cData.ID,"%s",id);
	sprintf(cData.strMsg,"%s",msg);
	cData.Code = code;
	cData.type = type;

	COPYDATASTRUCT SendDataStruct;
	SendDataStruct.cbData = sizeof(tag_SingleMsg);
	SendDataStruct.dwData = 0;
	SendDataStruct.lpData =(void*)&cData;
	if(svrhwnd != NULL)	::SendMessageTimeout(svrhwnd,WM_COPYDATA,(WPARAM)myhwnd,(LPARAM)&SendDataStruct,SMTO_NORMAL,12000,NULL);
}
void SendUserSizeToBridge(HWND svrhandle,int usersize)
{
	if(_BridgeServerHandle != HWND_BROADCAST && _BridgeServerHandle)
	{
		::SendMessage(_BridgeServerHandle,uCmmServerUserSizeCS,(WPARAM)svrhandle,(LPARAM)usersize);		
	}
}

void M_SetStopAccept(HWND myhwnd,HWND svrhandle,int bStopAccept)
{
	if (FALSE == ::PostMessage(svrhandle,uCmmStopAcceptSC,(WPARAM)myhwnd,(LPARAM)bStopAccept))
	{
#if defined(_ONLY_SERVER)
		GLOBALLOG_ERROR("PostMessage Fail.");
#endif
	}
}

void M_ServerAcceptStatus(HWND svrhandle,int bStopAccept)
{
	if (FALSE == ::PostMessage(_BridgeServerHandle,uCmmStopAcceptCS,(WPARAM)svrhandle,(LPARAM)bStopAccept))
	{
#if defined(_ONLY_SERVER)
		GLOBALLOG_ERROR("PostMessage Fail.");
#endif
	}
}

////////////////////////////////////////////////////////////////////////////////
// DB AGent 용 
void SendDBAList(HWND myhwnd,tag_DBAgentList *pList)
{
	tag_DBAgentList ListData;
	memcpy(&ListData,pList, sizeof(tag_DBAgentList));

	COPYDATASTRUCT SendDataStruct;
	SendDataStruct.cbData = sizeof(tag_DBAgentList);
	SendDataStruct.dwData = 0;
	SendDataStruct.lpData =(void*)&ListData;
	::SendMessage(_BridgeServerHandle,WM_COPYDATA,(WPARAM)myhwnd,(LPARAM)&SendDataStruct);
}

void SetDBAItem(HWND myhwnd,tag_DBAgentitem *pList)
{
	tag_DBAgentitem ListData;
	memcpy(&ListData,pList, sizeof(tag_DBAgentitem));

	COPYDATASTRUCT SendDataStruct;
	SendDataStruct.cbData = sizeof(tag_DBAgentitem);
	SendDataStruct.dwData = 0;
	SendDataStruct.lpData =(void*)&ListData;

	::SendMessage(_BridgeServerHandle,WM_COPYDATA,(WPARAM)myhwnd,(LPARAM)&SendDataStruct);
}


////////////////////////////////////////////////////////////////////////////////
// 사용자 접속 끊기 
// 닉네임 수정 2006/11/24
void M_DisconnectUser(HWND svrhandle,HWND  myhwnd,int type, char *id)
{
	tag_DisbandUser cData;
	cData.type = type;
	sprintf(cData.ID,"%s",id);
	COPYDATASTRUCT SendDataStruct;
	SendDataStruct.cbData = sizeof(tag_DisbandUser);
	SendDataStruct.dwData = 0;
	SendDataStruct.lpData =(void*)&cData;
	if(svrhandle != NULL)	::SendMessageTimeout(svrhandle,WM_COPYDATA,(WPARAM)myhwnd,(LPARAM)&SendDataStruct,SMTO_NORMAL,12000,NULL);
}

void M_DisConnectAns(HWND svrhandle,BOOL tf)
{
	if (FALSE == ::PostMessage(_BridgeServerHandle,uCmmDisbandUserANS,(WPARAM)svrhandle,(LPARAM)tf))
	{
#if defined(_ONLY_SERVER)
		GLOBALLOG_ERROR("PostMessage Fail.");
#endif
	}
}


// 클라이언트 체크섬 
void M_CheckSumSendToServer(HWND svrhandle,HWND myhwnd,tag_CheckSumData *data)
{
	tag_CheckSumData cData;
	cData.bSumFlag = data->bSumFlag;
	cData.CheckSum = data->CheckSum;

	COPYDATASTRUCT SendDataStruct;
	SendDataStruct.cbData = sizeof(tag_CheckSumData);
	SendDataStruct.dwData = 0;
	SendDataStruct.lpData =(void*)&cData;
	if(svrhandle != NULL)	::SendMessageTimeout(svrhandle,WM_COPYDATA,(WPARAM)myhwnd,(LPARAM)&SendDataStruct,SMTO_NORMAL,12000,NULL);
}

void M_CheckSumReQuestToBridge(HWND svrhandle)
{
	if (FALSE == ::PostMessage(_BridgeServerHandle,uCmmClientChecksumCS,(WPARAM)svrhandle,(LPARAM)0))
	{
#if defined(_ONLY_SERVER)
		GLOBALLOG_ERROR("PostMessage Fail.");
#endif
	}
}
// 닉네임 수정 2006/11/24
void M_SingleMsgSecc(HWND  myhwnd, int type, char *id,int index)
{
	/*
		index -  0: 짜르기 성공,  1: 메시지보내기 성공 
                type-    0: 아이디로 작업함, 1: 닉네임으로 작업함 

	*/
	tag_MsgSendOK cData;
	sprintf(cData.ID,"%s",id);
	cData.type = type;	// 닉네임 수정 2006/11/24
	cData.Index = index;
	COPYDATASTRUCT SendDataStruct;
	SendDataStruct.cbData = sizeof(tag_MsgSendOK);
	SendDataStruct.dwData = 0;
	SendDataStruct.lpData =(void*)&cData;
	if(myhwnd != NULL)	::SendMessage(_BridgeServerHandle,WM_COPYDATA,(WPARAM)myhwnd,(LPARAM)&SendDataStruct);
}
