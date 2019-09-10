#include "stdafx.h"
#include "ServerSock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CServerSock::CServerSock()
{
	m_hWND = NULL;
}

CServerSock::~CServerSock()
{
}

BOOL CServerSock::CreateSocket(HWND hwnd, UINT lPortNum, char *lpszSocketAddress)
{
	if(m_hWND) 
	{
		Close();
	}

	// ���� ����
	if(Create( lPortNum, SOCK_STREAM, FD_READ|FD_WRITE|FD_OOB|FD_ACCEPT|FD_CONNECT|FD_CLOSE, lpszSocketAddress) == 0)
	{
		m_strError.Format("������ ������ ������ �� �����ϴ� CServerSock::Create() Error : %d", GetLastError());
		return FALSE;
	}

	if(Listen() == 0)
	{ 
		m_strError.Format("������ ���� ��⿡ �����߽��ϴ�. CServerSock::Listen() error : %d", GetLastError());
		return FALSE;
	}

	m_hWND = hwnd;

	return TRUE;
}

BOOL CServerSock::AcceptSocket(HWND hwnd, CDataSock *pSock, int sid)
{
	if(pSock == NULL) 
	{
		return FALSE;
	}

	if(Accept(*pSock) == 0)
	{
		m_strError.Format("CServerSock::Accept() error : %d", GetLastError());
		return FALSE;
	}

	pSock->SetSockCID(sid);			///< ���� Ŭ���� ���̵�
	pSock->SetSrvSock(TRUE);		///< ������ �������� �˸�
	pSock->SetHwnd(hwnd);			///< �޼����� Send�� ������ �ڵ�
	pSock->SetAvailable(TRUE);		///< ��ȿ�� �������� �˸�

	return TRUE;
}


void CServerSock::OnAccept(int nErrorCode)
{
	if(nErrorCode == 0)
	{
		SendMessage(m_hWND, UM_SERV_ACCEPT, NULL, (long)this);
	}

	CAsyncSocket::OnAccept(nErrorCode);
}

void CServerSock::OnClose(int nErrorCode)
{
	PostMessage(m_hWND, UM_SERV_CLOSE, NULL, (long)this);

	CAsyncSocket::OnClose(nErrorCode);
}
