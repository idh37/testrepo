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

		///< DataSock.h ���
		/*----------------------------------------------------------------------------------------
		<���� ����>

		Winsock.h�� AfxSock.h���� #define FD_SETSIZE 64�� ���ǵǾ� �ִ�.
		���� �� ���� ������ �����Ϸ��� ���� ����� ��Ŭ����ϱ�����
		#define FD_SETSIZE 500 �� ���� define�� �־�� �Ѵ�.
		�׷��� Win98������ �ڿ� ���� ������ 80-90�� ������ ���ϸ� ���� �����ϴ�.
		(NT�� ��� �� ���� ������ ���� ��ų �� ����)
		----------------------------------------------------------------------------------------*/
		#define MAX_SOCK_SENDBUFFER		250000			// �ִ� �۽� ����
		#define MAX_SOCK_RECVBUFFER		5000000			// �ִ� ���� ����

		#ifdef FD_SETSIZE
			#undef FD_SETSIZE
			#define FD_SETSIZE			500
		#endif /* FD_SETSIZE */


		///< SockMan.h ���
		#define MAX_DATA_SOCKET			20				// �ִ� ������ ���� ����
		#define CONNECT_FAIL			"connect_fail"	// Connect ����

		/**      
		@brief	���� ����(IP, PORT)
		*/
		typedef struct _tagSOCKINFO
		{
			long Port;
			long IPAdr;
		} SSOCKINFO;


		///< CommUtil.h ���
 		#define SOCK_MAX_MSGBUFFERNUM		20				// �޽��� ���� ����
 		#define SOCK_MINIMUM_MSGBUFFERSIZE	36000			// �޽��� ������ �⺻ ������

		///< CommMsg.h ���
		#define MSG_ID_LEN				2				// �޼��� �νĹ��� ����
		#define MSG_ID					("NP")			// �޼��� �νĹ���
		#define _PVAR(x)				((void**)&(x))	// ������ ��ȯ�� ��ũ��

		/**      
		@brief	��Ŷ ��� ����ü
		*/
		typedef struct _tagPACKETHEADER
		{
			char	MsgID[2];
			short	Signal;				// �޽��� �ñ׳�
			int		MsgLen;				// �޽��� ���� unsigned short -> UINT
			char	Encode;				// Encode ����
			char	Compress;			// ���� ���� 1:�������, 2~5:���� ����, 10:�����ϸ� ��Ŀ���»���, 20~50:����� ������
			unsigned short PackCnt;		// ��Ŷ ī���� UINT -> unsigned short
			UINT	PackCRC;
		} SPACKETHEADER;


		#define SOCK_PACKET_HEADERSIZE sizeof(SPACKETHEADER)



		///< SockCmnd.h
		#define SOCKNAME_LEN	32		// �����̸� ������

		///< SockCmnd.h
		/**      
		@brief	���� ��Ŷ ������ ����ü
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