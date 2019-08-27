// VipServiceSockCmnd.cpp : implementation file
//

#include "stdafx.h"
//#include "Client.h"
#include "VipServiceSockCmnd.h"
#include "GlobalBase.h"
#include "CommonUserWndMsg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVipServiceSockCmnd

CVipServiceSockCmnd::CVipServiceSockCmnd()
{
	m_pParentWnd = NULL;		// 메세지를 보낼 부모윈도우
	m_pSockMan = NULL;
}

CVipServiceSockCmnd::~CVipServiceSockCmnd()
{
}


BEGIN_MESSAGE_MAP(CVipServiceSockCmnd, CWnd)
	//{{AFX_MSG_MAP(CVipServiceSockCmnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
// 	ON_MESSAGE(UM_SERV_ACCEPT, OnServAccept)
// 	ON_MESSAGE(UM_SERV_CLOSE, OnServClose)
	ON_MESSAGE(UM_DATA_RECEIVE, OnDataReceive)
	ON_MESSAGE(UM_DATA_CLOSE, OnDataClose)
	ON_MESSAGE(UM_DATA_CONNECT, OnDataConnect)
	ON_MESSAGE(UM_DATA_CONNECT_FAILURE, OnDataConnectFailure)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CVipServiceSockCmnd message handlers


BOOL CVipServiceSockCmnd::Init(CWnd* parent, NMBASE::SOCK::CSockMan* pSockMan)
{
	m_pParentWnd = parent;
	// 눈에 보이지 않는 윈도를 만든다(소켓 메세지 처리용 윈도)
	CRect rect(0,0,0,0);
	if(Create(NULL,NULL,WS_CHILD, rect, m_pParentWnd, 5872)==NULL) 
		return FALSE;
	
	m_pSockMan = pSockMan;
	m_pSockMan->Init(m_hWnd);

	return TRUE;
}

// LONG CVipServiceSockCmnd::OnServAccept(UINT wParam, LONG pSock)
// {
// 	return 1;
// }
// 
// LONG CVipServiceSockCmnd::OnServClose(UINT wParam, LONG pSock)
// {
// 	return 1;
// }

LONG CVipServiceSockCmnd::OnDataReceive(WPARAM wParam, LONG pSock)
{
	LPSTR pszSockName   = (LPSTR)wParam;
	CString strSockName = (LPSTR)wParam;

	NMBASE::SOCK::CDataSock *pSocket = m_pSockMan->GetDataSocketPtr(strSockName.GetBuffer());
	if(pSocket==NULL) return 0;
	if(pSocket != (NMBASE::SOCK::CDataSock*)pSock) return 0;	// 인덱스는 같지만 이전에 파괴된 소켓 포인터일 수 있으므로 체크
	
// 	if(sid<0 || sid>=MAX_DATA_SOCKET) 
// 	{
// 		TRACE("CVipServiceSockCmnd::OnDataReceive() - 잘못된 소켓 번호가 들어왔음\n"); 
// 		return 0;
// 	}

	int rbyte = pSocket->ReceiveData();
	if(rbyte==0)
	{
		TRACE("CVipServiceSockCmnd::OnDataReceive() - rbyte=0\n");
		return 0;
	}

	NMBASE::SOCK::CStreamBuf *pRB = pSocket->GetRecvBuf();
	char *pbuf = NULL;

	CCommMsg msg;

REPEAT:

	int state=0;
	int signal=0, msglen=0;
	
	if(state == 0) // 헤더를 찾는중
	{
		int bufsize = pRB->GetUsedSize();
		if(bufsize>=PACKET_HEADERSIZE) // 헤더에 필요한 사이즈를 받았으면
		{
			int delbyte=0;
			pbuf = pRB->GetStartPointer();
			for(int i=0; i<= (int)(bufsize-PACKET_HEADERSIZE); i++)
			{
				delbyte=i;
				if(strncmp(pbuf+i, MSG_ID, strlen(MSG_ID))==0)
				{
					state = 1;
					break;
				}
			}
			if(delbyte>0)
			{ 
				pRB->DelData(delbyte);
				TRACE("찌꺼기 바이트가 발생하여 삭제함(size=%d)\n",delbyte);
			}
			if(state == 1)
			{
				pbuf = pRB->GetStartPointer();
				// 메시지 정보를 체크한다
				//BOOL encoded;	// 임시 변수
				//msg.CheckInfo(pbuf, &encoded, &signal, &msglen);
				// 메시지 정보를 체크한다
				msg.CheckInfo(pbuf, &signal, &msglen);
			}
		}
	}

	if(state == 1) // 헤더를 찾았으면
	{
		int totsize = PACKET_HEADERSIZE + msglen;

		if(pRB->GetUsedSize() >= totsize)
		{
			// 메모리할당은 압축풀 사이즈 만큼  
			int decompresssize = totsize;
			CCommMsg::CheckCompressSizeInfo(pRB->GetStartPointer() , &decompresssize );
			char* lpdata	= new char[ decompresssize ];

			memcpy(lpdata, pRB->GetStartPointer(), totsize);
			
			// 게임 패킷 도착 메시지 준비
			NMBASE::SOCK::SGAMENOTIFY data;
			strncpy(data.szSockName, strSockName.GetBuffer(), sizeof(char)*SOCKNAME_LEN);
			data.Signal = signal;
			data.TotSize = totsize;
			data.lpData = lpdata;

			// 큐를 미리 삭제하여 중복 호출을 막는다
			pRB->DelData(totsize);

			// 완성된 패킷이 도착하였음을 메인 뷰에 알림
			m_pParentWnd->SendMessage(UM_VIP_PACKET_NOTIFY, (WPARAM)&data, 0);
			
			if(lpdata)
			{
				delete [] lpdata;
				lpdata = NULL;
			}

			// 이미 소켓이 제거된 이후라면
			if(!m_pSockMan->GetDataSocketPtr(strSockName.GetBuffer()))
				return 1;

			// [게임 아이템 작업]
			// 위의 패킷을 처리한후 프로그램이 종료된 경우는 윈도우 핸들이 NULL이다
			if(!GetSafeHwnd()) return 1;
			
			// 패킷 처리도중 이미 프로그램이 종료되어버린 경우라면 버퍼가 파괴되었기 때문에 검사하여 처리를 중단한다
			if(IsBadWritePtr(pRB->GetStartPointer(), 4))
				return 1;

		//	if(g_MainSrvSID < 0 ) return 1;

			state = 0;
			if(pRB->GetUsedSize()>0) goto REPEAT;
		}
	}
	return 1;
}

LONG CVipServiceSockCmnd::OnDataClose(WPARAM wParam, LONG pSock)
{
	LPSTR pszSockName   = (LPSTR)wParam;
	CString strSockName = (LPSTR)wParam;

	NMBASE::SOCK::CDataSock *pSocket = m_pSockMan->GetDataSocketPtr(strSockName.GetBuffer());
	if(pSocket==NULL) return 1;

	BOOL rtn = m_pSockMan->CloseDataSocket(strSockName.GetBuffer());
	if(rtn==FALSE)
	{
		TRACE("NMBASE::SOCK::CSockMan::OnDataClose() - 접속 종료 처리 실패\n");
	}

	m_pParentWnd->SendMessage(UM_VIP_ON_DISCONNECT, 0, 0);

	return 1;
}

LONG CVipServiceSockCmnd::OnDataConnect(WPARAM wParam, LONG pSock)
{
	LPSTR pszSockName   = (LPSTR)wParam;
	CString strSockName = (LPSTR)wParam;

	NMBASE::SOCK::CDataSock *pSocket = m_pSockMan->GetDataSocketPtr(strSockName.GetBuffer());
	if(pSocket==NULL) return 1;

	m_pParentWnd->SendMessage(UM_VIP_ON_CONNECT, 0, 0);

	return 1;
}

LONG CVipServiceSockCmnd::OnDataConnectFailure(WPARAM wParam, LONG pSock)
{
	LPSTR pszSockName   = (LPSTR)wParam;
	CString strSockName = (LPSTR)wParam;

	NMBASE::SOCK::CDataSock *pSocket = m_pSockMan->GetDataSocketPtr(strSockName.GetBuffer());
	if(pSocket==NULL) return 1;

	m_pSockMan->CloseDataSocket(strSockName.GetBuffer());
	m_pParentWnd->SendMessage(UM_VIP_ON_CONNECTFAILURE, 0, 0);

	return 1;
}

