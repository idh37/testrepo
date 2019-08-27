/**    
@file    SockDefine.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Socket Import / Export Define
*/
#pragma once

namespace NMBASE
{
	namespace SOCK
	{
		#ifndef NMSOCK_API
			#ifdef NMSOCK_EXPORTS
				#define NMSOCK_API		__declspec(dllexport )
			#else
				#define NMSOCK_API		__declspec(dllimport )
			#endif 
		#endif

		#ifndef NMSOCK_CLASS 
			#define NMSOCK_CLASS NMSOCK_API
		#endif

		///< DataSock.h 사용
		/*----------------------------------------------------------------------------------------
		<참고 사항>

		Winsock.h나 AfxSock.h에는 #define FD_SETSIZE 64로 정의되어 있다.
		따라서 더 많은 소켓을 생성하려면 위의 헤더를 인클루드하기전에
		#define FD_SETSIZE 500 과 같이 define해 주어야 한다.
		그러나 Win98에서는 자원 관리 문제로 80-90개 정도의 소켓만 생성 가능하다.
		(NT의 경우 더 많은 소켓을 생성 시킬 수 있음)
		----------------------------------------------------------------------------------------*/
		#define MAX_SOCK_SENDBUFFER		250000			// 최대 송신 버퍼
		#define MAX_SOCK_RECVBUFFER		5000000			// 최대 수신 버퍼

		#ifdef FD_SETSIZE
			#undef FD_SETSIZE
			#define FD_SETSIZE			500
		#endif /* FD_SETSIZE */


		///< SockMan.h 사용
		#define MAX_DATA_SOCKET			20				// 최대 데이터 소켓 개수
		#define CONNECT_FAIL			"connect_fail"	// Connect 실패

		/**      
		@brief	소켓 정보(IP, PORT)
		*/
		typedef struct _tagSOCKINFO
		{
			long Port;
			long IPAdr;
		} SSOCKINFO;


		///< CommUtil.h 사용
 		#define SOCK_MAX_MSGBUFFERNUM		20				// 메시지 버퍼 개수
 		#define SOCK_MINIMUM_MSGBUFFERSIZE	36000			// 메시지 버퍼의 기본 사이즈

		///< CommMsg.h 사용
		#define MSG_ID_LEN				2				// 메세지 인식문자 길이
		#define MSG_ID					("NP")			// 메세지 인식문자
		#define _PVAR(x)				((void**)&(x))	// 포인터 변환용 매크로

		/**      
		@brief	패킷 헤더 구조체
		*/
		typedef struct _tagPACKETHEADER
		{
			char	MsgID[2];
			short	Signal;				// 메시지 시그널
			int		MsgLen;				// 메시지 길이 unsigned short -> UINT
			char	Encode;				// Encode 상태
			char	Compress;			// 압축 상태 1:압축없음, 2~5:압축 예약, 10:압축하면 더커지는상태, 20~50:압축된 데이터
			unsigned short PackCnt;		// 패킷 카운터 UINT -> unsigned short
			UINT	PackCRC;
		} SPACKETHEADER;


		#define SOCK_PACKET_HEADERSIZE sizeof(SPACKETHEADER)



		///< SockCmnd.h
		#define SOCKNAME_LEN	32		// 소켓이름 사이즈

		///< SockCmnd.h
		/**      
		@brief	게임 패킷 데이터 구조체
		*/
		typedef struct _tagGAMENOTIFY
		{
			int Sid;
			char szSockName[SOCKNAME_LEN];	
			int Signal;
			int TotSize;
			char *lpData;
		}SGAMENOTIFY;

	
	} //namespace SOCK
} //namespace NMBASE