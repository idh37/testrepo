#include "stdafx.h"
#include "SockCmnd.h"
#include "CommonUserWndMsg.h"
#include "CommonDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace NMBASE
{
	namespace SOCK
	{
		/////////////////////////////////////////////////////////////////////////////
		// CSockCmnd

		CSockCmnd::CSockCmnd()
		{
			m_pParentWnd = NULL;
			m_pSockMan = NULL;
		}

		CSockCmnd::~CSockCmnd()
		{
		}


		BEGIN_MESSAGE_MAP(CSockCmnd, CWnd)
			//{{AFX_MSG_MAP(CSockCmnd)
			// NOTE - the ClassWizard will add and remove mapping macros here.

			ON_MESSAGE(UM_DATA_RECEIVE, OnDataReceive)
			ON_MESSAGE(UM_DATA_CLOSE, OnDataClose)
			ON_MESSAGE(UM_DATA_CONNECT, OnDataConnect)
			ON_MESSAGE(UM_DATA_CONNECT_FAILURE, OnDataConnectFailure)
			//}}AFX_MSG_MAP
		END_MESSAGE_MAP()


		/////////////////////////////////////////////////////////////////////////////
		// CSockCmnd message handlers

		BOOL CSockCmnd::Init(CWnd* parent, CSockMan *pSockMan)
		{
			m_pParentWnd = parent;
			m_pSockMan   = pSockMan;

			// ���� ������ �ʴ� ������ �����(���� �޼��� ó���� ����)
			CRect rect(0, 0, 0, 0);

			if(Create(NULL, NULL, WS_CHILD, rect, m_pParentWnd, 5454) == NULL) 
			{
				return FALSE;
			}

			if(m_pSockMan)
			{
				m_pSockMan->Init(m_hWnd);
			}

			return TRUE;
		}

		// ������ Receive
		LONG CSockCmnd::OnDataReceive(WPARAM wParam, LONG pSock)
		{
			if(!m_pSockMan)
			{
				return 0;
			}

			LPSTR pszSockName = (LPSTR)wParam;
			CString strSockName = pszSockName;

			CDataSock *pSocket = m_pSockMan->GetDataSocketPtr(strSockName.GetBuffer());

			if(NULL == pSocket) 
			{
				//TRACE("CSockCmnd::OnDataReceive() - �߸��� ���� �̸��� ������\n"); 
#ifdef TRACE_OUTPUT		
				char szTemp[256] = {0, };
				sprintf(szTemp, "CSockCmnd::OnDataReceive() - �߸��� ���� �̸��� ������");
				TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT

				return 0;
			}

			if(pSocket != (CDataSock*)pSock) 
			{
				return 0;	// �ε����� ������ ������ �ı��� ���� �������� �� �����Ƿ� üũ
			}

			int nByte = pSocket->ReceiveData();
			if(0 == nByte)
			{
				//TRACE("CSockCmnd::OnDataReceive() - rbyte=0\n");
#ifdef TRACE_OUTPUT		
				char szTemp[256] = {0, };
				sprintf(szTemp, "CSockCmnd::OnDataReceive() - rbyte = 0");
				TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT
				return 0;
			}

			CStreamBuf *pStreamBuffer = pSocket->GetRecvBuf();
			char *pTempBuffer = NULL;

			CCommMessage msg;

REPEAT:

			int nState  = 0;
			int nSignal = 0;
			int	nMessageLen = 0;

			if(nState == 0) // ����� ã����
			{
				int bufsize = pStreamBuffer->GetUsedSize();

				///< ����� �ʿ��� ����� �޾�����
				if(bufsize >= SOCK_PACKET_HEADERSIZE) 
				{
					int nDeleteByte = 0;
					pTempBuffer = pStreamBuffer->GetStartPointer();
					for(int i = 0; i <= bufsize-(int)SOCK_PACKET_HEADERSIZE; ++i)
					{
						nDeleteByte = i;
						if(strncmp(pTempBuffer+i, MSG_ID, strlen(MSG_ID))==0)
						{
							nState = 1;
							break;
						}
					}

					if(nDeleteByte > 0)
					{ 
						pStreamBuffer->DelData(nDeleteByte);
						//TRACE("��� ����Ʈ�� �߻��Ͽ� ������(size=%d)\n",nDeleteByte);
#ifdef TRACE_OUTPUT		
						char szTemp[256] = {0, };
						sprintf(szTemp, "��� ����Ʈ�� �߻��Ͽ� ������(size = %d)\n", nDeleteByte);
						TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT
					}

					if(nState == 1)
					{
						pTempBuffer = pStreamBuffer->GetStartPointer();

						///< �޽��� ������ üũ�Ѵ�
						msg.CheckInfo(pTempBuffer, &nSignal, &nMessageLen);
					}
				}
			}

			///< ����� ã������
			if(nState == 1) 
			{
				CString recvLog;
				recvLog.Format("RecvBuff %d, %d, SendBuff %d, %d\r\n",
					pSocket->GetRecvBuf()->GetBufSize(), pSocket->GetRecvBuf()->GetUsedSize(),
					pSocket->GetSendBuf()->GetBufSize(), pSocket->GetSendBuf()->GetUsedSize());
				int nTotalSize = SOCK_PACKET_HEADERSIZE + nMessageLen;

				if(pStreamBuffer->GetUsedSize() >= nTotalSize)
				{
					// �޸��Ҵ��� ����Ǯ ������ ��ŭ  
					int nDecompressSize = nTotalSize;
					CCommMessage::CheckCompressSizeInfo(pStreamBuffer->GetStartPointer(), &nDecompressSize);
					char* pData = new char[nDecompressSize];

					memcpy(pData, pStreamBuffer->GetStartPointer(), nTotalSize);

					// ���� ��Ŷ ���� �޽��� �غ�
					SGAMENOTIFY sGameNotiData;			
					strncpy(sGameNotiData.szSockName, strSockName.GetBuffer(), sizeof(char)*SOCKNAME_LEN);
					sGameNotiData.Signal = nSignal;
					sGameNotiData.TotSize= nTotalSize;
					sGameNotiData.lpData = pData;

					// ť�� �̸� �����Ͽ� �ߺ� ȣ���� ���´�
					pStreamBuffer->DelData(nTotalSize);

					// �ϼ��� ��Ŷ�� �����Ͽ����� ���� �信 �˸�
					// �޽���:UM_SOCKET_NOTIFY(����),  Command : UM_SOCKET_CONNECT_FAIL �� ó����
					//yoo
					SYSTEMTIME systemTime;
					GetLocalTime(&systemTime);
					CString timeStr;
					timeStr.Format("%04d.%02d.%02d.%02d:%02d.%02d\n"
						,systemTime.wYear 
						,systemTime.wMonth
						,systemTime.wDay
						,systemTime.wHour
						,systemTime.wMinute
						,systemTime.wSecond);
					if(NULL!=CSockMan::m_pFuncI_Ptr_Type){
						ErrorLogError(CSockMan::GetSocketTxt().c_str(), "Recv = %s, %d, %s, %s", (char*)CSockMan::m_pFuncI_Ptr_Type(sGameNotiData.Signal).c_str(), sGameNotiData.TotSize, sGameNotiData.lpData, sGameNotiData.szSockName);
						recvLog.AppendFormat("Recv = %s, %d, %s, %s, %s\r\n", (char*)CSockMan::m_pFuncI_Ptr_Type(sGameNotiData.Signal).c_str(), sGameNotiData.TotSize, sGameNotiData.lpData, sGameNotiData.szSockName, timeStr);
					}
					else{
						ErrorLogError(CSockMan::GetSocketTxt().c_str(), "Recv = %d, %d, %s, %s", sGameNotiData.Signal, sGameNotiData.TotSize, sGameNotiData.lpData, sGameNotiData.szSockName);
						recvLog.AppendFormat("Recv = %d, %d, %s, %s, %s\r\n", sGameNotiData.Signal, sGameNotiData.TotSize, sGameNotiData.lpData, sGameNotiData.szSockName, timeStr);
					}
					NMBASE::SOCK::CSockMan::m_sSocketLogTxt.append(recvLog);
					m_pParentWnd->SendMessage(UM_SOCKET_NOTIFY, (WPARAM)UM_SOCKET_RECEIVE, (LPARAM)&sGameNotiData);						

					if(pData)
					{
						delete [] pData;
						pData = NULL;
					}

					// �̹� ������ ���ŵ� ���Ķ��
					if(!m_pSockMan->GetDataSocketPtr(strSockName.GetBuffer()))
					{
						return 1;
					}

					// [���� ������ �۾�]
					// ���� ��Ŷ�� ó������ ���α׷��� ����� ���� ������ �ڵ��� NULL�̴�
					if(!GetSafeHwnd()) 
					{
						return 1;
					}

					// ��Ŷ ó������ �̹� ���α׷��� ����Ǿ���� ����� ���۰� �ı��Ǿ��� ������ �˻��Ͽ� ó���� �ߴ��Ѵ�
					if(IsBadWritePtr(pStreamBuffer->GetStartPointer(), 4))
					{
						return 1;
					}

					nState = 0;

					if(pStreamBuffer->GetUsedSize() > 0)
					{
						goto REPEAT;
					}
				}
			}

			return 1;
		}

		// ��Ĺ Ŭ����
		LONG CSockCmnd::OnDataClose(WPARAM wParam, LONG pSock)
		{
			if(!m_pSockMan)
				return 0;

			LPSTR pszSockName = (LPSTR)wParam;
			CString sockNameBackup = pszSockName;

			CDataSock *pSocket = m_pSockMan->GetDataSocketPtr(pszSockName);
			if(NULL == pSocket)
			{
				return 1;	
			}

			// Socket�� Close �Ѵ�.
			BOOL bResult = m_pSockMan->CloseDataSocket(pszSockName);

			// Controller�� �޽����� �����ش�.
			if(m_pParentWnd)
			{
				// �޽���:UM_SOCKET_NOTIFY(����),  Command : UM_SOCKET_CLOSE �� ó����
				m_pParentWnd->SendMessage(UM_SOCKET_NOTIFY, (WPARAM)UM_SOCKET_CLOSE, (LPARAM)(LPCTSTR)sockNameBackup);
			}

			if(FALSE == bResult)
			{
				//TRACE("CSockMan::OnDataClose() - ���� ���� ó�� ����\n");
#ifdef TRACE_OUTPUT		
				char szTemp[256] = {0, };
				sprintf(szTemp, "CSockMan::OnDataClose() - ���� ���� ó�� ����");
				TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT
				return 1;
			}

#ifdef TRACE_OUTPUT
			char szTemp[256] = {0, };
			sprintf(szTemp, "CSockMan::OnDataClose() - Success : %s", pszSockName);
			TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT

			return 1;
		}

		// ����
		LONG CSockCmnd::OnDataConnect(WPARAM wParam, LONG pSock)
		{
			if(!m_pSockMan)
			{
				return 0;
			}

			LPSTR pszSockName = (LPSTR)wParam;

			CDataSock *pSocket = m_pSockMan->GetDataSocketPtr(pszSockName);
			if(NULL == pSocket)
			{
				return 1;
			}

			if(m_pParentWnd)
			{
				// �޽���:UM_SOCKET_NOTIFY(����),  Command : UM_SOCKET_CONNECT_SUCCESS �� ó����
				m_pParentWnd->SendMessage(UM_SOCKET_NOTIFY, (WPARAM)UM_SOCKET_CONNECT_SUCCESS, (LPARAM)pszSockName);				
			}

#ifdef TRACE_OUTPUT		
			char szTemp[256] = {0, };
			sprintf(szTemp, "CSockCmnd::OnDataConnect() - Success : %s", pszSockName);
			TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT

			return 1;
		}

		// ���� ����
		LONG CSockCmnd::OnDataConnectFailure(WPARAM wParam, LONG pSock)
		{
			if(!m_pSockMan)
			{
				return 0;
			}

			LPSTR pszSockName   = (LPSTR)wParam;
			CString strSockName = (LPSTR)wParam;

			CDataSock *pSocket = m_pSockMan->GetDataSocketPtr(pszSockName);
			if(NULL == pSocket)
			{
				return 1;
			}	

			if(m_pParentWnd)
			{
				// �޽���:UM_SOCKET_NOTIFY(����),  Command : UM_SOCKET_CONNECT_FAIL �� ó����
				m_pParentWnd->SendMessage(UM_SOCKET_NOTIFY, (WPARAM)UM_SOCKET_CONNECT_FAIL, (LPARAM)pszSockName);				
			}

			m_pSockMan->CloseDataSocket(pszSockName);

#ifdef TRACE_OUTPUT		
			char szTemp[256] = {0, };
			sprintf(szTemp, "CSockCmnd::OnDataConnectFailure() - Fail : %s", strSockName.GetBuffer());
			TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT

			return 1;
		}


	} //namespace SOCK

} //namespace NMBASE