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
	m_pParentWnd = NULL;		// �޼����� ���� �θ�������
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
	// ���� ������ �ʴ� ������ �����(���� �޼��� ó���� ����)
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
	if(pSocket != (NMBASE::SOCK::CDataSock*)pSock) return 0;	// �ε����� ������ ������ �ı��� ���� �������� �� �����Ƿ� üũ
	
// 	if(sid<0 || sid>=MAX_DATA_SOCKET) 
// 	{
// 		TRACE("CVipServiceSockCmnd::OnDataReceive() - �߸��� ���� ��ȣ�� ������\n"); 
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
	
	if(state == 0) // ����� ã����
	{
		int bufsize = pRB->GetUsedSize();
		if(bufsize>=PACKET_HEADERSIZE) // ����� �ʿ��� ����� �޾�����
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
				TRACE("��� ����Ʈ�� �߻��Ͽ� ������(size=%d)\n",delbyte);
			}
			if(state == 1)
			{
				pbuf = pRB->GetStartPointer();
				// �޽��� ������ üũ�Ѵ�
				//BOOL encoded;	// �ӽ� ����
				//msg.CheckInfo(pbuf, &encoded, &signal, &msglen);
				// �޽��� ������ üũ�Ѵ�
				msg.CheckInfo(pbuf, &signal, &msglen);
			}
		}
	}

	if(state == 1) // ����� ã������
	{
		int totsize = PACKET_HEADERSIZE + msglen;

		if(pRB->GetUsedSize() >= totsize)
		{
			// �޸��Ҵ��� ����Ǯ ������ ��ŭ  
			int decompresssize = totsize;
			CCommMsg::CheckCompressSizeInfo(pRB->GetStartPointer() , &decompresssize );
			char* lpdata	= new char[ decompresssize ];

			memcpy(lpdata, pRB->GetStartPointer(), totsize);
			
			// ���� ��Ŷ ���� �޽��� �غ�
			NMBASE::SOCK::SGAMENOTIFY data;
			strncpy(data.szSockName, strSockName.GetBuffer(), sizeof(char)*SOCKNAME_LEN);
			data.Signal = signal;
			data.TotSize = totsize;
			data.lpData = lpdata;

			// ť�� �̸� �����Ͽ� �ߺ� ȣ���� ���´�
			pRB->DelData(totsize);

			// �ϼ��� ��Ŷ�� �����Ͽ����� ���� �信 �˸�
			m_pParentWnd->SendMessage(UM_VIP_PACKET_NOTIFY, (WPARAM)&data, 0);
			
			if(lpdata)
			{
				delete [] lpdata;
				lpdata = NULL;
			}

			// �̹� ������ ���ŵ� ���Ķ��
			if(!m_pSockMan->GetDataSocketPtr(strSockName.GetBuffer()))
				return 1;

			// [���� ������ �۾�]
			// ���� ��Ŷ�� ó������ ���α׷��� ����� ���� ������ �ڵ��� NULL�̴�
			if(!GetSafeHwnd()) return 1;
			
			// ��Ŷ ó������ �̹� ���α׷��� ����Ǿ���� ����� ���۰� �ı��Ǿ��� ������ �˻��Ͽ� ó���� �ߴ��Ѵ�
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
		TRACE("NMBASE::SOCK::CSockMan::OnDataClose() - ���� ���� ó�� ����\n");
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

