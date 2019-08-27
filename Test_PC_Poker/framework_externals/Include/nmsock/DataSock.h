#pragma once

#include "SockDefine.h"
#include "StreamBuf.h"
#include <afxsock.h>

namespace NMBASE
{
	namespace SOCK
	{
		/**      
		@brief	데이터 소켓(CAsyncSocket 상속)
		*/
		class NMSOCK_CLASS CDataSock : public CAsyncSocket
		{
		public:
			CDataSock();
			~CDataSock();

		public:
			//	BOOL ConnectSocket(HWND hwnd, char* pstrHostAddr, UINT nHostPort, int sid); ///< 클라이언트용 소켓으로 Connect한다
			BOOL ConnectSocket(HWND hwnd, char* pstrHostAddr, UINT nHostPort, char* szSockName); ///< 클라이언트용 소켓으로 Connect한다

			BOOL SendData(void *pdata, int size);	///< 데이터를 보낸다
			int  ReceiveData();						///< 데이터를 받아서 내부 버퍼에 저장한다

			virtual void OnReceive(int nErrorCode);
			virtual void OnSend(int nErrorCode);
			virtual void OnClose(int nErrorCode);
			virtual void OnConnect(int nErrorCode);
			virtual void OnOutOfBandData(int nErrorCode);

			void SetHwnd(HWND hwnd){m_hWND = hwnd;}

			//	void SetSockCID(int nID){m_nSockCID = nID;}
			//	int  GetSockCID(){return m_nSockCID;}
			LPCTSTR GetSockIDName(){return m_szSockName.operator LPCTSTR();}

			void SetSrvSock(BOOL bFlag){m_bIsSrvSock = bFlag;}
			BOOL IsSrvSock(){return m_bIsSrvSock;}

			void SetPostCloseMsg(BOOL bFlag){m_bPostCloseMsg = bFlag;}
			BOOL IsPostCloseMsg(){return m_bPostCloseMsg;}

			void SetAvailable(BOOL bFlag){m_bAvailable = bFlag;}
			BOOL IsAvailable(){return m_bAvailable;}

			LPCTSTR GetErrorString(){return (LPCTSTR)m_strError;}

			CStreamBuf* GetRecvBuf(){return &m_cRecvBuf;}
			CStreamBuf* GetSendBuf(){return &m_cSendBuf;}

			LPCTSTR GetServerIP(){return (LPCTSTR)m_strSockAddr;}
			UINT GetServerPort(){return m_unSockPort;}


		protected:
			HWND m_hWND;						///< 메시지를 보낼 윈도우 핸들
			//	int  m_nSockCID;					///< 소켓 클래스 아이디 (클래스 포인터 배열상에서 구분을 쉽게함)
			CString m_szSockName;
			BOOL m_bIsSrvSock;					///< 소켓이 서버용인가?(소켓의 용도가 서버용인지 클라이언트용인지 구분)

			BOOL m_bAvailable;					///< 사용가능한 소켓인가? (Connect시 TRUE, Close되면서 FALSE로 세팅된다)
			///<  소켓 삭제시 UM_DATA_CLOSE를 PostMessage()하는데...
			///<  받을 데이터가 또 있을 경우 OnReceive()가 작동된다.
			///<  이렇게되면 UM_DATA_CLOSE가 두번 발생하게 되는데
			///<  이 플래그를 이용하여 유효하지 않은 소켓은 처리하지 않을 수 있다.

			BOOL m_bPostCloseMsg;				///<  소켓 닫기 메세지를 이미 보냈는가?

			CString m_strSockAddr;				///<  연결 된후 자신의 IP 어드레스
			UINT m_unSockPort;					///<  연결 된후 자신의 Port 번호

			unsigned short m_usSendPackCnt;		///< 패킷을 보낸 회수 카운터(패킷 스니핑 방지용)
			unsigned short m_usRecvPackCnt;		///< 패킷을 받은 회수 카운터(패킷 스니핑 방지용)

			CString m_strError;					///< 에러 출력용 스트링

			CStreamBuf m_cSendBuf;				///< 데이터 전송용 스트림 버퍼
			CStreamBuf m_cRecvBuf;				///< 데이터 수신용 스트림 버퍼
		};

	} //namespace SOCK

} //namespace NMBASE

