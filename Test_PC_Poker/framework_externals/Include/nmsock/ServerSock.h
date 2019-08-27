#if !defined(AFX_WAITSOCK_H__65409B35_DE27_11D3_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_WAITSOCK_H__65409B35_DE27_11D3_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataSock.h"
#include "CommonUserWndMsg.h"


/**      
    @brief	서버 소켓(CAsyncSocket 상속)
*/
class CServerSock : public CAsyncSocket
{
public:
	CServerSock();
	~CServerSock();

public:
	BOOL CreateSocket(HWND hwnd, UINT lPortNum, char *lpszSocketAddress=NULL);
	BOOL AcceptSocket(HWND hwnd, CDataSock *pSock, int sid); ///< 연결 요청을 받아들이고 데이터 소켓을 초기화

	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);

protected:
	HWND	m_hWND;				///< 메세지를 Send할 윈도우 핸들
	CString m_strError;			///< Error Code
};

#endif // !defined(AFX_WAITSOCK_H__65409B35_DE27_11D3_97A2_0050BF0FBE67__INCLUDED_)
