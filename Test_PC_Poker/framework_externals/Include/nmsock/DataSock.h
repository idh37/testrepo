#pragma once

#include "SockDefine.h"
#include "StreamBuf.h"
#include <afxsock.h>

namespace NMBASE
{
	namespace SOCK
	{
		/**      
		@brief	������ ����(CAsyncSocket ���)
		*/
		class NMSOCK_CLASS CDataSock : public CAsyncSocket
		{
		public:
			CDataSock();
			~CDataSock();

		public:
			//	BOOL ConnectSocket(HWND hwnd, char* pstrHostAddr, UINT nHostPort, int sid); ///< Ŭ���̾�Ʈ�� �������� Connect�Ѵ�
			BOOL ConnectSocket(HWND hwnd, char* pstrHostAddr, UINT nHostPort, char* szSockName); ///< Ŭ���̾�Ʈ�� �������� Connect�Ѵ�

			BOOL SendData(void *pdata, int size);	///< �����͸� ������
			int  ReceiveData();						///< �����͸� �޾Ƽ� ���� ���ۿ� �����Ѵ�

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
			HWND m_hWND;						///< �޽����� ���� ������ �ڵ�
			//	int  m_nSockCID;					///< ���� Ŭ���� ���̵� (Ŭ���� ������ �迭�󿡼� ������ ������)
			CString m_szSockName;
			BOOL m_bIsSrvSock;					///< ������ �������ΰ�?(������ �뵵�� ���������� Ŭ���̾�Ʈ������ ����)

			BOOL m_bAvailable;					///< ��밡���� �����ΰ�? (Connect�� TRUE, Close�Ǹ鼭 FALSE�� ���õȴ�)
			///<  ���� ������ UM_DATA_CLOSE�� PostMessage()�ϴµ�...
			///<  ���� �����Ͱ� �� ���� ��� OnReceive()�� �۵��ȴ�.
			///<  �̷��ԵǸ� UM_DATA_CLOSE�� �ι� �߻��ϰ� �Ǵµ�
			///<  �� �÷��׸� �̿��Ͽ� ��ȿ���� ���� ������ ó������ ���� �� �ִ�.

			BOOL m_bPostCloseMsg;				///<  ���� �ݱ� �޼����� �̹� ���´°�?

			CString m_strSockAddr;				///<  ���� ���� �ڽ��� IP ��巹��
			UINT m_unSockPort;					///<  ���� ���� �ڽ��� Port ��ȣ

			unsigned short m_usSendPackCnt;		///< ��Ŷ�� ���� ȸ�� ī����(��Ŷ ������ ������)
			unsigned short m_usRecvPackCnt;		///< ��Ŷ�� ���� ȸ�� ī����(��Ŷ ������ ������)

			CString m_strError;					///< ���� ��¿� ��Ʈ��

			CStreamBuf m_cSendBuf;				///< ������ ���ۿ� ��Ʈ�� ����
			CStreamBuf m_cRecvBuf;				///< ������ ���ſ� ��Ʈ�� ����
		};

	} //namespace SOCK

} //namespace NMBASE

