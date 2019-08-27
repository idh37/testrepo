#pragma once


///< Controller To Model
#define MSG_SOCKETINIT					"socketinit"					///< 소켓 초기화(Old-Default)
#define MSG_SOCKETCAMELINIT				"socketcamelinit"				///< 소켓 초기화(Camel)

// LogIn
#define MSG_MASTER_CHECKVERSION			"masterserver_checkversion"		///< 마스터서버 체크버젼 
#define MSG_MASTER_DESTSERVERINFO		"masterserver_destserverinfo"	///< 목적지 서버 정보

#define MSG_GAME_CHECKVERSION			"gameserver_checkversion"		///< 게임서버 체크버젼 
#define MSG_GAME_SERVERINFO				"gameserver_serverinfo"			///< 게임서버 정보
#define MSG_GAME_ACCEPTLOGIN			"gameserver_acceptlogin"		///< 게임서버 로그인 허용
#define MSG_GAME_REFUSELOGIN			"gameserver_refuselogin"		///< 게임서버 로그인 거부

// lobby
#define MSG_GAME_PINGCHECK				"gameserver_pingcheck"			///< 핑 체크

#define MSG_GAME_LOGOUT					"gameserver_logout"				///< 로그아웃

#define MSG_GAME_ALLUSERINFO			"gameserver_alluserinfo"		///< 모든 유저 정보
#define MSG_GAME_ALLROOMINFO			"gameserver_allroominfo"		///< 모든 방 정보

// channel
#define MSG_GAME_ALLCHANNELINFO			"gameserver_allchannelinfo"		///< 채널 정보


// channel-view
#define MSG_VIEW_ALLCHANNELINFO			"allchannelinfo"				///< 채널View 처리

// lobby-view
#define MSG_VIEW_ACCEPTLOGIN			"updateviewacceptlogin"			///< 로비쪽View 처리

 

///< Event
#define EVT_MAINFRAM_ONEXITBTN			"OnExitButtonDown"				///< 종료버튼 처리
#define EVT_MAINFRAM_ONALTF4			"OnExitAltF4"					///< 종료버튼 처리
#define EVT_MAINFRAM_ONDESTROY			"OnDestroy"						///< Destroy 처리


///< Socket
#define MSG_SOCKET_CLOSE				"socketclose"					///< 소켓 Close
#define MSG_SOCKETCAMEL_CLOSE			"socketcamelclose"				///< 소켓 Camel Close

#define MSG_SOCKET_SUCCESSCONNECT		"socketsuccess_connect"			///< 소켓 Success Connect
#define MSG_SOCKETCAMEL_SUCCESSCONNECT	"socketcamelsuccess_connect"	///< 소켓 Camel Success Connect


#define MSG_SOCKET_FAILCONNECT			"socket_failconnect"			///< 소켓 Fail Connect	
#define MSG_SOCKETCAMEL_FAILCONNECT		"socketcamel_failconnect"		///< 소켓 Camel Fail Connect	

#define MSG_SOCKETCAMEL_RECEIVEERROR	"socketcamel_receiveerror"		///< 소켓 Camel Receive Error










