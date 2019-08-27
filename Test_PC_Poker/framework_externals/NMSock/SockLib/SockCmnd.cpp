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

			// 눈에 보이지 않는 윈도를 만든다(소켓 메세지 처리용 윈도)
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

		// 데이터 Receive
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
				//TRACE("CSockCmnd::OnDataReceive() - 잘못된 소켓 이름이 들어왔음\n"); 
#ifdef TRACE_OUTPUT		
				char szTemp[256] = {0, };
				sprintf(szTemp, "CSockCmnd::OnDataReceive() - 잘못된 소켓 이름이 들어왔음");
				TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT

				return 0;
			}

			if(pSocket != (CDataSock*)pSock) 
			{
				return 0;	// 인덱스는 같지만 이전에 파괴된 소켓 포인터일 수 있으므로 체크
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

			if(nState == 0) // 헤더를 찾는중
			{
				int bufsize = pStreamBuffer->GetUsedSize();

				///< 헤더에 필요한 사이즈를 받았으면
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
						//TRACE("찌꺼기 바이트가 발생하여 삭제함(size=%d)\n",nDeleteByte);
#ifdef TRACE_OUTPUT		
						char szTemp[256] = {0, };
						sprintf(szTemp, "찌꺼기 바이트가 발생하여 삭제함(size = %d)\n", nDeleteByte);
						TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT
					}

					if(nState == 1)
					{
						pTempBuffer = pStreamBuffer->GetStartPointer();

						///< 메시지 정보를 체크한다
						msg.CheckInfo(pTempBuffer, &nSignal, &nMessageLen);
					}
				}
			}

			///< 헤더를 찾았으면
			if(nState == 1) 
			{
				CString recvLog;
				recvLog.Format("RecvBuff %d, %d, SendBuff %d, %d\r\n",
					pSocket->GetRecvBuf()->GetBufSize(), pSocket->GetRecvBuf()->GetUsedSize(),
					pSocket->GetSendBuf()->GetBufSize(), pSocket->GetSendBuf()->GetUsedSize());
				int nTotalSize = SOCK_PACKET_HEADERSIZE + nMessageLen;

				if(pStreamBuffer->GetUsedSize() >= nTotalSize)
				{
					// 메모리할당은 압축풀 사이즈 만큼  
					int nDecompressSize = nTotalSize;
					CCommMessage::CheckCompressSizeInfo(pStreamBuffer->GetStartPointer(), &nDecompressSize);
					char* pData = new char[nDecompressSize];

					memcpy(pData, pStreamBuffer->GetStartPointer(), nTotalSize);

					// 게임 패킷 도착 메시지 준비
					SGAMENOTIFY sGameNotiData;			
					strncpy(sGameNotiData.szSockName, strSockName.GetBuffer(), sizeof(char)*SOCKNAME_LEN);
					sGameNotiData.Signal = nSignal;
					sGameNotiData.TotSize= nTotalSize;
					sGameNotiData.lpData = pData;

					// 큐를 미리 삭제하여 중복 호출을 막는다
					pStreamBuffer->DelData(nTotalSize);

					// 완성된 패킷이 도착하였음을 메인 뷰에 알림
					// 메시지:UM_SOCKET_NOTIFY(공통),  Command : UM_SOCKET_CONNECT_FAIL 로 처리함
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

					// 이미 소켓이 제거된 이후라면
					if(!m_pSockMan->GetDataSocketPtr(strSockName.GetBuffer()))
					{
						return 1;
					}

					// [게임 아이템 작업]
					// 위의 패킷을 처리한후 프로그램이 종료된 경우는 윈도우 핸들이 NULL이다
					if(!GetSafeHwnd()) 
					{
						return 1;
					}

					// 패킷 처리도중 이미 프로그램이 종료되어버린 경우라면 버퍼가 파괴되었기 때문에 검사하여 처리를 중단한다
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

		// 소캣 클로즈
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

			// Socket을 Close 한다.
			BOOL bResult = m_pSockMan->CloseDataSocket(pszSockName);

			// Controller에 메시지를 보내준다.
			if(m_pParentWnd)
			{
				// 메시지:UM_SOCKET_NOTIFY(공통),  Command : UM_SOCKET_CLOSE 로 처리함
				m_pParentWnd->SendMessage(UM_SOCKET_NOTIFY, (WPARAM)UM_SOCKET_CLOSE, (LPARAM)(LPCTSTR)sockNameBackup);
			}

			if(FALSE == bResult)
			{
				//TRACE("CSockMan::OnDataClose() - 접속 종료 처리 실패\n");
#ifdef TRACE_OUTPUT		
				char szTemp[256] = {0, };
				sprintf(szTemp, "CSockMan::OnDataClose() - 접속 종료 처리 실패");
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

		// 연결
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
				// 메시지:UM_SOCKET_NOTIFY(공통),  Command : UM_SOCKET_CONNECT_SUCCESS 로 처리함
				m_pParentWnd->SendMessage(UM_SOCKET_NOTIFY, (WPARAM)UM_SOCKET_CONNECT_SUCCESS, (LPARAM)pszSockName);				
			}

#ifdef TRACE_OUTPUT		
			char szTemp[256] = {0, };
			sprintf(szTemp, "CSockCmnd::OnDataConnect() - Success : %s", pszSockName);
			TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT

			return 1;
		}

		// 연결 실패
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
				// 메시지:UM_SOCKET_NOTIFY(공통),  Command : UM_SOCKET_CONNECT_FAIL 로 처리함
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