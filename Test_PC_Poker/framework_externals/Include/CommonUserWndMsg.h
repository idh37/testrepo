#pragma once

/**
 *	Custom Widnow Message ID
**/

#define UM_BASE							(WM_USER)

#define UM_DATA_RECEIVE					(UM_BASE+10)	// 자체 버퍼에 데이터가 수신되어 있음
#define UM_DATA_CLOSE					(UM_BASE+11)	// 소켓이 닫혀야할 상황임
#define UM_DATA_CONNECT					(UM_BASE+12)	// 연결 성공
#define UM_DATA_CONNECT_FAILURE			(UM_BASE+13)	// 연결 실패

//#define UM_SERV_ACCEPT				(UM_BASE+100)
//#define UM_SERV_CLOSE					(UM_BASE+101)

// Default Socket
#define UM_SOCKET_NOTIFY				(WM_USER+500)	
#define UM_SOCKET_RECEIVE				(WM_USER+501)
#define UM_SOCKET_CLOSE					(WM_USER+502)
#define UM_SOCKET_CONNECT_FAIL			(WM_USER+503)
#define UM_SOCKET_CONNECT_SUCCESS		(WM_USER+504)

// Camel Socket
#define UM_SOCKETCAMEL_NOTIFY			(WM_USER+510)	
#define UM_SOCKETCAMEL_RECEIVE			(WM_USER+511)
#define UM_SOCKETCAMEL_CLOSE			(WM_USER+512)
#define UM_SOCKETCAMEL_CONNECT_FAIL		(WM_USER+513)
#define UM_SOCKETCAMEL_CONNECT_SUCCESS	(WM_USER+514)
#define UM_SOCKETCAMEL_RECEIVE_ERROR	(WM_USER+515)



