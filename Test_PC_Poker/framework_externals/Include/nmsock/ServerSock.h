#if !defined(AFX_WAITSOCK_H__65409B35_DE27_11D3_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_WAITSOCK_H__65409B35_DE27_11D3_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataSock.h"
#include "CommonUserWndMsg.h"


/**      
    @brief	���� ����(CAsyncSocket ���)
*/
class CServerSock : public CAsyncSocket
{
public:
	CServerSock();
	~CServerSock();

public:
	BOOL CreateSocket(HWND hwnd, UINT lPortNum, char *lpszSocketAddress=NULL);
	BOOL AcceptSocket(HWND hwnd, CDataSock *pSock, int sid); ///< ���� ��û�� �޾Ƶ��̰� ������ ������ �ʱ�ȭ

	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);

protected:
	HWND	m_hWND;				///< �޼����� Send�� ������ �ڵ�
	CString m_strError;			///< Error Code
};

#endif // !defined(AFX_WAITSOCK_H__65409B35_DE27_11D3_97A2_0050BF0FBE67__INCLUDED_)
