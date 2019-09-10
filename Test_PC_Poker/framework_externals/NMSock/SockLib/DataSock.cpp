#include "stdafx.h"
#include "DataSock.h"
#include "CommMessage.h"
#include "CommonUserWndMsg.h"

#include "SockMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace NMBASE
{
	namespace SOCK
	{

		static CMsgBuffer g_sMsgBuf;

		class CPacketPool
		{
		public:
			CBufferUnit* m_pBufUnit;

			CPacketPool(int size)
			{
				m_pBufUnit = g_sMsgBuf.GetFreeBuf();
				m_pBufUnit->Prepare(size);
			}

			~CPacketPool()
			{
				if(m_pBufUnit) 
					m_pBufUnit->SetFree();
			}

			char* GetBufPtr()
			{
				if(!m_pBufUnit) 
				{
					return NULL;
				}
				return m_pBufUnit->GetBuffer();
			}
		};


		CDataSock::CDataSock()
		{
			//m_nSockCID = 0;
			m_hWND = NULL;
			m_bAvailable = FALSE;

			//(�Ʒ� ���� �߰�)
			m_usSendPackCnt = 0; 	// ��Ŷ ������ ���� ī���� �ʱ�ȭ
			m_usRecvPackCnt = 0;	// ��Ŷ ������ ���� ī���� �ʱ�ȭ

			m_bPostCloseMsg = FALSE;

			m_cSendBuf.Init(MAX_SOCK_SENDBUFFER);
			m_cRecvBuf.Init(MAX_SOCK_RECVBUFFER);
		}

		CDataSock::~CDataSock()
		{
			//m_nSockCID = 0;
			m_hWND = NULL;
			m_bAvailable = FALSE;
		}

		/*======================================================================================
		Ŭ���̾�Ʈ�� ������ �������� �ʱ�ȭ(Connect��û�Ѵ�)

		��û�� �����ߴ��� ���� ���� ���δ� OnConnect()�� �ڵ����� ȣ��Ǹ鼭
		�ش� ������� ���� ����� �޼����� �̿��Ͽ� ������ �ȴ�.
		(�񵿱� ������ Ư���� ����� ���߿� �߻��Ѵ�)
		���ῡ ������ ������ UM_DATA_CONNECT_FAILURE �޼��� ó������ �����ؾ��Ѵ�.
		(Ŭ������ delete�ϸ� �ڵ����� Close()�� ȣ��ȴ�)
		PostMessage()�� ����ϴ� ������ �Լ�ó���� Ŭ���� �����Ͱ� �����Ǹ� �ȵǱ� ������
		======================================================================================*/
		//BOOL CDataSock::ConnectSocket(HWND hwnd, char* pstrHostAddr, UINT nHostPort, int sid)
		//{
		//	if(m_hWND) 
		//	{
		//		Close();
		//	}
		//
		//	//m_nSockCID		= sid;			// ���� Ŭ���� ���̵�
		//	m_bIsSrvSock	= FALSE;		// Ŭ���̾�Ʈ�� �������� �˸� 
		//	m_hWND			= hwnd;			// �޼����� ���� ������ �ڵ�
		//	m_bPostCloseMsg = FALSE;		// ���� �ݱ� �޽��� ���� �÷��� �ʱ�ȭ
		//	m_bAvailable	= FALSE;		// ��ȿ���� ���� �������� ��Ÿ��
		//	//(��� ���� �߰�)
		//	m_usSendPackCnt = 0;			// ��Ŷ ��Ű�� ���� ī���� �ʱ�ȭ
		//	m_usRecvPackCnt = 0;			// ��Ŷ ������ ���� ī���� �ʱ�ȭ
		//
		//	if(m_cSendBuf.GetFreeSize() == 0 || m_cRecvBuf.GetFreeSize() == 0)
		//	{
		//		m_strError.Format("CDataSock::Connect() error : Can't Create Class (Socket ID=%d)", sid);
		//		PostMessage(m_hWND, UM_DATA_CONNECT_FAILURE, m_nSockCID, (long)this); // ���� ���� �޼����� ����
		//		return FALSE;
		//	}
		//
		//	if(Create() == 0) // ���� ���� ����
		//	{
		//		m_strError.Format("CDataSock::Create() error : %d (Socket ID=%d)", ::WSAGetLastError(), sid);
		//		PostMessage(m_hWND, UM_DATA_CONNECT_FAILURE, m_nSockCID, (long)this); // ���� ���� �޼����� ����
		//		return FALSE;
		//	}
		//
		//	if(Connect(pstrHostAddr, nHostPort) == 0) // Ȯ���� ���ؼ� ���� ���δ� �ڵ����� ȣ��Ǵ� OnConnect()�� Ȯ��
		//	{	
		//		int nECode = GetLastError();
		//		if(nECode  != WSAEWOULDBLOCK) // ��ŷ�� �ƴ� ������ ���� ��� ���� ���� �޼����� ����
		//		{
		//			m_strError.Format("CDataSock::Connect() error : %d (Socket ID=%d)", nECode, sid);
		//			PostMessage(m_hWND, UM_DATA_CONNECT_FAILURE, m_nSockCID, (long)this); // ���� ���� �޼����� ����
		//			return FALSE;
		//		}
		//	}
		//
		//	m_bAvailable = TRUE;	// ��ȿ�� �������� ��Ÿ��
		//
		//	return TRUE;
		//}


		BOOL CDataSock::ConnectSocket(HWND hwnd, char* pstrHostAddr, UINT nHostPort, char* szSockName) ///< Ŭ���̾�Ʈ�� �������� Connect�Ѵ�
		{
			if(m_hWND) 
			{
				Close();
			}

			m_szSockName	= szSockName;	// ���� Ŭ���� �̸�
			m_bIsSrvSock	= FALSE;		// Ŭ���̾�Ʈ�� �������� �˸� 
			m_hWND			= hwnd;			// �޼����� ���� ������ �ڵ�
			m_bPostCloseMsg = FALSE;		// ���� �ݱ� �޽��� ���� �÷��� �ʱ�ȭ
			m_bAvailable	= FALSE;		// ��ȿ���� ���� �������� ��Ÿ��

			//(��� ���� �߰�)
			m_usSendPackCnt = 0;			// ��Ŷ ��Ű�� ���� ī���� �ʱ�ȭ
			m_usRecvPackCnt = 0;			// ��Ŷ ������ ���� ī���� �ʱ�ȭ

			if(m_cSendBuf.GetFreeSize() == 0 || m_cRecvBuf.GetFreeSize() == 0)
			{
				m_strError.Format("CDataSock::Connect() error : Can't Create Class (Socket Name=%s)", szSockName);
				PostMessage(m_hWND, UM_DATA_CONNECT_FAILURE, (WPARAM)(LPSTR)(LPCTSTR)m_szSockName, (long)this); // ���� ���� �޼����� ����
				return FALSE;
			}

			if(Create() == 0) // ���� ���� ����
			{
				m_strError.Format("CDataSock::Create() error : %d (Socket Name=%s)", ::WSAGetLastError(), szSockName);
				PostMessage(m_hWND, UM_DATA_CONNECT_FAILURE, (WPARAM)(LPSTR)(LPCTSTR)m_szSockName, (long)this); // ���� ���� �޼����� ����
				return FALSE;
			}

			if(Connect(pstrHostAddr, nHostPort) == 0) // Ȯ���� ���ؼ� ���� ���δ� �ڵ����� ȣ��Ǵ� OnConnect()�� Ȯ��
			{	
				// �߿��� KeyPoint
				// ���� �������� �񵿱� ����(connection-oriented nonblocking sockets)�� ��� ���� �ٷ� ������ �̷�� ���� �ʽ��ϴ�. 
				// �̷���쿡 Connect �Լ��� WSAEWOULDBLOCK �����ڵ带 �߻��ϰ� �˴ϴ�. 
				// ������, �����ϴ� ������ ��� ����ǹǷ� �̷� ��Ȳ�� �ΰ� ���ӿ��� ��� ������ �����ϴ�.

				int nECode = ::WSAGetLastError();
				if(nECode != WSAEWOULDBLOCK) // ��ŷ�� �ƴ� ������ ���� ��� ���� ���� �޼����� ����
				{
					m_strError.Format("CDataSock::Connect() error : %d (Socket Name=%s)", nECode, szSockName);
					PostMessage(m_hWND, UM_DATA_CONNECT_FAILURE, (WPARAM)(LPSTR)(LPCTSTR)m_szSockName, (long)this); // ���� ���� �޼����� ����
					return FALSE;
				}
			}

			m_bAvailable = TRUE;	// ��ȿ�� �������� ��Ÿ��

			return TRUE;
		}

		// �����͸� ����
		// (������ �� �����͸� ���ۿ� ����ϰ� OnSend()�� ȣ��)
		BOOL CDataSock::SendData(void *pdata, int size)
		{
			if(m_bAvailable == FALSE)
			{
				return FALSE;
			}

			if(size <= 0)
			{
				//TRACE("CDataSock::SendData() - ������ ����� 0�̰ų� ������(SockName=%s, AddSize=%d)\n", m_szSockName, size);
#ifdef TRACE_OUTPUT		
				char szTemp[256] = {0, };
				sprintf(szTemp, "CDataSock::SendData() - ������ ����� 0�̰ų� ������(SockName=%s, AddSize=%d)", m_szSockName, size);
				TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT

				return FALSE;
			}

			// �ӽ� ���ۿ� �����͸� ����(�� �ӽù��۴� ��Ƽ ������κ��� ������ ���������� �ʴ� - �ӵ��� ���Ͽ� ��� ����)	
			CPacketPool pool(size);
			char* pbuf = pool.GetBufPtr();
			memcpy(pbuf, pdata, size);

			// ��Ŷ ��ȣ�� ���� ����(��Ŷ ������ üũ��)
			SPACKETHEADER* phead = (SPACKETHEADER*)pbuf;
			m_usSendPackCnt++;
			phead->PackCnt = m_usSendPackCnt;

			// ��Ŷ ��ȣȭ
			CCommMessage::EncodeAll((char *)pbuf);

			if(m_cSendBuf.AddData(pbuf, size) == 0)
			{
				//TRACE("Send Buffer�� �������� ���� ����(SockName=%s)\n", m_szSockName);
#ifdef TRACE_OUTPUT		
				char szTemp[256] = {0, };
				sprintf(szTemp, "Send Buffer�� �������� ���� ����(SockName=%s)", m_szSockName);
				TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT

				m_bAvailable = FALSE;	// ��ȿ���� ���� �������� ��Ÿ��

				if(m_bPostCloseMsg == FALSE)
				{
					m_bPostCloseMsg = TRUE;

					// �ݽ��� ���.

					CString postMessageLog;
					postMessageLog.Format("PostMessage Call.[File: %s][Function:%s][Line: %d] \r\n",static_cast<const char*>(__FILE__),static_cast<const char*>(__FUNCTION__),static_cast<const int>(__LINE__));
					NMBASE::SOCK::CSockMan::m_sSocketLogTxt.append(postMessageLog);

					PostMessage(m_hWND, UM_DATA_CLOSE, (WPARAM)(LPSTR)(LPCTSTR)m_szSockName, (long)this); 			
				}

				return FALSE;
			}

			OnSend(0);

			return TRUE;
		}

		int CDataSock::ReceiveData()
		{
			if(FALSE == m_bAvailable) 
			{
				return 0;  // ��ȿ�� ������ �ƴϸ� ����
			}

			int nECode = 0;

			/*
			int totbyte=0;
			if(IOCtl(FIONREAD, &totbyte)==0) // �ѹ��� ������ �ִ� �� ����Ʈ ���� ���Ѵ�
			{
			nECode = GetLastError();
			m_strError.Format("CDataSock::ReceiveData()-IOCtl() error : %d (Socket ID=%d)", nECode, sid);
			PostMessage(m_hWND, UM_DATA_RECEIVE_FAILURE, NULL, (long)this); // ���� ���� �޼����� ����
			//CAsyncSocket::OnReceive(nErrorCode);
			return;
			}
			*/

			// ��ȣȭ ���ڵ� ������ġ ���
			int nDecodeStart = m_cRecvBuf.GetUsedSize(); 

			// ������ ���� ����� 0�̸� nByte=0 �̵ȴ�
			// ���� �޾ƾ��� �����Ͱ� ���� ���� ������� ũ�ٸ� ���ʺ��� �߷��� ���ۿ� ����.
			int nSize = m_cRecvBuf.GetFreeSize();					// ������ ���� ����� ����
			int nByte = Receive(m_cRecvBuf.GetEndPointer(), nSize);	// ���ۿ� �����͸� ����

			// ���� : UM_DATA_CLOSE �޼����� �ι� �߻��Ѵ�.
			// ���۰� �۾Ƽ� Receive()�� �ѹ��� ������ �ʰ� �ٷ� �ڸ� �̾�
			// OnReceive()�� �Ǵٽ� ȣ��Ǳ� �����̴�. PostMessage()���ڸ��� �ٷ� �߻��Ѵ�
			// �׷��� ���� ���ᰡ �ι� �Ͼ�Ƿ� �̸� �������� m_bAvailable �÷��׸� �ξ���.

			if(SOCKET_ERROR == nByte)
			{
				nECode = ::WSAGetLastError();
				if(nECode == WSAEWOULDBLOCK) // ��ŷ ���̸� ������ OnReceive()���� �ٽ� �޴´�
				{
					return 0;
				}
				else // ��ŷ�� �ƴ϶� ���� �������
				{
					// ���� ���� �޼����� ����
					//TRACE("CDataSock::ReceiveData() - ���� ���� �߻�\n");
#ifdef TRACE_OUTPUT		
					char szTemp[256] = {0, };
					sprintf(szTemp, "CDataSock::ReceiveData() - ���� ���� �߻�");
					TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT


					m_bAvailable = FALSE;	// ��ȿ���� ���� �������� ��Ÿ��
					if(FALSE == m_bPostCloseMsg)
					{
						m_bPostCloseMsg = TRUE;

						CString postMessageLog;
						postMessageLog.Format("PostMessage Call.[File: %s][Function:%s][Line: %d] \r\n",static_cast<const char*>(__FILE__),static_cast<const char*>(__FUNCTION__),static_cast<const int>(__LINE__));
						NMBASE::SOCK::CSockMan::m_sSocketLogTxt.append(postMessageLog);

						PostMessage(m_hWND, UM_DATA_CLOSE, (WPARAM)(LPSTR)(LPCTSTR)m_szSockName, (long)this); 
					}
					return 0;
				}
			}
			else if(0 == nByte) // ������ ������ ���¶��(Ȥ�� ������ ���� ����� 0�� ���)
			{
				// ���� ���� �޼����� ����
				//TRACE("CDataSock::ReceiveData() - ���� ������ (���� ���ۻ�����=%d, ���� ������ ������=%d)\n", nSize, nByte);

#ifdef TRACE_OUTPUT		
				char szTemp[256] = {0, };
				sprintf(szTemp, "CDataSock::ReceiveData() - ���� ������ (���� ���ۻ�����=%d, ���� ������ ������=%d)\n", nSize, nByte);
				TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT


				m_bAvailable = FALSE;	// ��ȿ���� ���� �������� ��Ÿ��
				if(FALSE == m_bPostCloseMsg)
				{
					m_bPostCloseMsg = TRUE;

					CString postMessageLog;
					postMessageLog.Format("PostMessage Call.[File: %s][Function:%s][Line: %d] \r\n",static_cast<const char*>(__FILE__),static_cast<const char*>(__FUNCTION__),static_cast<const int>(__LINE__));
					NMBASE::SOCK::CSockMan::m_sSocketLogTxt.append(postMessageLog);

					PostMessage(m_hWND, UM_DATA_CLOSE, (WPARAM)(LPSTR)(LPCTSTR)m_szSockName, (long)this); 
				}
				return 0;
			}
			else // ���������� �����͸� �޾Ҵٸ�
			{
				if(FALSE == m_cRecvBuf.AddUsedSize(nByte)) // ���۰� ��ģ ����� ���� ���� ��Ų��.
				{
					// ���� ���� �޼����� ����
					//TRACE("CDataSock::ReceiveData() - ���ú� ���� ��ħ\n");
#ifdef TRACE_OUTPUT		
					char szTemp[256] = {0, };
					sprintf(szTemp, "CDataSock::ReceiveData() - ���ú� ���� ��ħ");
					TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT

					m_bAvailable = FALSE;	// ��ȿ���� ���� �������� ��Ÿ��
					if(FALSE == m_bPostCloseMsg)
					{
						m_bPostCloseMsg=TRUE;

						CString postMessageLog;
						postMessageLog.Format("PostMessage Call.[File: %s][Function:%s][Line: %d] \r\n",static_cast<const char*>(__FILE__),static_cast<const char*>(__FUNCTION__),static_cast<const int>(__LINE__));
						NMBASE::SOCK::CSockMan::m_sSocketLogTxt.append(postMessageLog);

						PostMessage(m_hWND, UM_DATA_CLOSE, (WPARAM)(LPSTR)(LPCTSTR)m_szSockName, (long)this); 
					}
					return 0;
				}
				else
				{
					// ��ȣȭ�� ��Ŷ�� ���ڵ��Ѵ�
					//m_cRecvBuf.Data_Decode(nDecodeStart, nByte);
					return nByte;
				}
			}
		}

		// ���Ͽ� �о���� ����Ÿ�� �����Ҷ� �߻�
		// (���� �����͸� ���ۿ� �����ϰ� UM_DATA_RECEIVE �޼����� �����Ͽ� �����Ͱ� ������ �˸�)
		void CDataSock::OnReceive(int nErrorCode)
		{
			// ��ȿ�� ������ �ƴϸ� ����
			if(m_bAvailable == FALSE)
			{
				CAsyncSocket::OnReceive(nErrorCode);
				return;
			}

			if(nErrorCode == WSAENETDOWN) // ��Ʈ��ũ ����
			{
				//TRACE("CDataSock::OnReceive() - ��Ʈ��ũ ���� �߻�\n");
#ifdef TRACE_OUTPUT		
				char szTemp[256] = {0, };
				sprintf(szTemp, "CDataSock::OnReceive() - ��Ʈ��ũ ���� �߻�");
				TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT

				m_bAvailable = FALSE;	// ��ȿ���� ���� �������� ��Ÿ��
				if(m_bPostCloseMsg==FALSE)
				{
					m_bPostCloseMsg=TRUE;

					CString postMessageLog;
					postMessageLog.Format("PostMessage Call.[File: %s][Function:%s][Line: %d] \r\n",static_cast<const char*>(__FILE__),static_cast<const char*>(__FUNCTION__),static_cast<const int>(__LINE__));
					NMBASE::SOCK::CSockMan::m_sSocketLogTxt.append(postMessageLog);

					PostMessage(m_hWND, UM_DATA_CLOSE, (WPARAM)(LPSTR)(LPCTSTR)m_szSockName, (long)this); 
				}
				CAsyncSocket::OnReceive(nErrorCode);
				return;
			}

			SendMessage(m_hWND, UM_DATA_RECEIVE, (WPARAM)(LPSTR)(LPCTSTR)m_szSockName, (long)this); 
			// Post�Ұ�� ���ۿ� �����Ͱ� �����Ǳ� ����. (���۰� ���鼭 ���� �����)
			// Send�� ���� �����Ͱ� ó���Ǿ�߸� �Ѿ�Ƿ� ���۰� �������� �ʴ´�.

			CAsyncSocket::OnReceive(nErrorCode);
		}

		// ���Ͽ� ����Ÿ Send()�� �����Ҷ� �߻�
		// ���� ����� �ѹ� �߻��ϰ� ��ŷ�� ���尡 �ٽ� ���������� �� �߻���
		void CDataSock::OnSend(int nErrorCode)
		{
			// ��ȿ�� ������ �ƴϸ� ����
			if(m_bAvailable == FALSE)
			{
				CAsyncSocket::OnSend(nErrorCode);
				return;
			}

			if(nErrorCode == WSAENETDOWN) // ��Ʈ��ũ ����
			{
				m_bAvailable = FALSE;	// ��ȿ���� ���� �������� ��Ÿ��
				if(m_bPostCloseMsg==FALSE)
				{
					m_bPostCloseMsg=TRUE;

					CString postMessageLog;
					postMessageLog.Format("PostMessage Call.[File: %s][Function:%s][Line: %d] \r\n",static_cast<const char*>(__FILE__),static_cast<const char*>(__FUNCTION__),static_cast<const int>(__LINE__));
					NMBASE::SOCK::CSockMan::m_sSocketLogTxt.append(postMessageLog);

					PostMessage(m_hWND, UM_DATA_CLOSE, (WPARAM)(LPSTR)(LPCTSTR)m_szSockName, (long)this); 
				}
				CAsyncSocket::OnSend(nErrorCode);
				return;
			}

			// ���ۿ��� ���� �����Ͱ� ������ ����
			if(m_cSendBuf.GetUsedSize()==0)
			{
				CAsyncSocket::OnSend(nErrorCode);
				return;
			}

			int sbyte = Send(m_cSendBuf.GetStartPointer(), m_cSendBuf.GetUsedSize());
			if(sbyte == SOCKET_ERROR)
			{
				int nECode = ::WSAGetLastError();
				if(nECode==WSAEWOULDBLOCK) // ��ŷ ���̸� ������ OnSend()���� �ٽ� ������
				{
					return;
				}
				else // ��ŷ�� �ƴ϶� ���� �������
				{
					// ���� ���� �޼����� ����
					m_bAvailable = FALSE;	// ��ȿ���� ���� �������� ��Ÿ��
					if(m_bPostCloseMsg==FALSE)
					{
						m_bPostCloseMsg=TRUE;

						CString postMessageLog;
						postMessageLog.Format("PostMessage Call.[File: %s][Function:%s][Line: %d] \r\n",static_cast<const char*>(__FILE__),static_cast<const char*>(__FUNCTION__),static_cast<const int>(__LINE__));
						NMBASE::SOCK::CSockMan::m_sSocketLogTxt.append(postMessageLog);

						PostMessage(m_hWND, UM_DATA_CLOSE,(WPARAM)(LPSTR)(LPCTSTR) m_szSockName, (long)this); 
					}
					CAsyncSocket::OnSend(nErrorCode);
					return;
				}

			}
			else // ����� ������ �Ǿ��ٸ� (sbyte > 1) �ϰ��̴�.
			{
				m_cSendBuf.DelData(sbyte);
			}

			//SendMessage(m_hWND, UM_DATA_????, m_szSockName, (long)this);

			CAsyncSocket::OnSend(nErrorCode);
		}


		// ����� ������ ������ �߻�
		void CDataSock::OnClose(int nErrorCode)
		{
			//TRACE("CDataSock::OnClose() ȣ�� \n");
#ifdef TRACE_OUTPUT		
			char szTemp[256] = {0, };
			sprintf(szTemp, "CDataSock::OnClose() ȣ��");
			TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT


			m_bAvailable = FALSE;	// ��ȿ���� ���� �������� ��Ÿ��
			if(m_bPostCloseMsg==FALSE)
			{
				m_bPostCloseMsg=TRUE;

				CString postMessageLog;
				postMessageLog.Format("PostMessage Call.[File: %s][Function:%s][Line: %d] \r\n",static_cast<const char*>(__FILE__),static_cast<const char*>(__FUNCTION__),static_cast<const int>(__LINE__));
				NMBASE::SOCK::CSockMan::m_sSocketLogTxt.append(postMessageLog);

				PostMessage(m_hWND, UM_DATA_CLOSE, (WPARAM)(LPSTR)(LPCTSTR)m_szSockName, (long)this); 
			}
			CAsyncSocket::OnClose(nErrorCode);
		}

		/*======================================================================================
		Connect ��û�� ���� ����� �߻�(������ ���� ������ ���� �ִ�)

		(�񵿱� ���Ͽ����� ���ؼ� ����� ���߿� �߻��Ѵ�-��û ���� �ٷ� �̷������ ���� �ƴ�)
		������ �����ϸ� ��õ� �ϴ��� Ŭ������ �����ؾ��Ѵ�.
		PostMessage()�� ����ϴ� ������ �Լ�ó���� Ŭ���� �����Ͱ� �����Ǹ� �ȵǱ� ������
		======================================================================================*/
		void CDataSock::OnConnect(int nErrorCode)
		{
			if(nErrorCode != 0) 
			{	
				m_strError.Format("CDataSock::OnConnect() error : %d (Socket Name=%s)", nErrorCode, m_szSockName);
				PostMessage(m_hWND, UM_DATA_CONNECT_FAILURE, (WPARAM)(LPSTR)(LPCTSTR)m_szSockName, (long)this); // ���� ���� �޼����� ����
				CAsyncSocket::OnConnect(nErrorCode);
				return; 
			}

			GetSockName(m_strSockAddr, m_unSockPort); // ����� �ּҿ� ���� ��ȣ�� ���Ѵ�
			SendMessage(m_hWND, UM_DATA_CONNECT, (WPARAM)(LPSTR)(LPCTSTR)m_szSockName, (long)this); // ���� ���� �޼����� ����
			CAsyncSocket::OnConnect(nErrorCode);
		}

		// Out of Band
		void CDataSock::OnOutOfBandData(int nErrorCode)
		{
			if(nErrorCode != WSAENETDOWN) 
			{
				return;
			}

			//TRACE("��Ʈ��ũ ���� - WSAENETDOWN : OUT OF BAND (m_szSockName=%s)\n", m_szSockName);
#ifdef TRACE_OUTPUT		
			char szTemp[256] = {0, };
			sprintf(szTemp, "��Ʈ��ũ ���� - WSAENETDOWN : OUT OF BAND (m_szSockName=%s)\n", m_szSockName);
			TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT


			m_bAvailable = FALSE;	// ��ȿ���� ���� �������� ��Ÿ��
			if(m_bPostCloseMsg==FALSE)
			{
				m_bPostCloseMsg=TRUE;

				CString postMessageLog;
				postMessageLog.Format("PostMessage Call.[File: %s][Function:%s][Line: %d] \r\n",static_cast<const char*>(__FILE__),static_cast<const char*>(__FUNCTION__),static_cast<const int>(__LINE__));
				NMBASE::SOCK::CSockMan::m_sSocketLogTxt.append(postMessageLog);

				PostMessage(m_hWND, UM_DATA_CLOSE, (WPARAM)(LPSTR)(LPCTSTR)m_szSockName, (long)this); 
			}

			CAsyncSocket::OnOutOfBandData(nErrorCode);
		}

	} //namespace SOCK

} // namespace NMBASE

