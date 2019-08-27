#if !defined(AFX_VIPSERVICESOCKCMND_H__3D8AF350_AADB_4D59_8DDF_ECBA788F548E__INCLUDED_)
#define AFX_VIPSERVICESOCKCMND_H__3D8AF350_AADB_4D59_8DDF_ECBA788F548E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VipServiceSockCmnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVipServiceSockCmnd window

//#include "CommMsgㅇㄷㄹ.h"

#define UM_VIP_PACKET_NOTIFY (WM_USER+1500)
#define UM_VIP_ON_CONNECT (WM_USER+1501)
#define UM_VIP_ON_DISCONNECT (WM_USER+1502)
#define UM_VIP_ON_CONNECTFAILURE (WM_USER+1503)



class UPG_COMMON_EXPORT CVipServiceSockCmnd : public CWnd
{
// Construction
public:
	CVipServiceSockCmnd();

	public:
	CWnd*		m_pParentWnd;		// 메세지를 보낼 부모윈도우
	NMBASE::SOCK::CSockMan*	m_pSockMan;
	CString szMainServerIP;	// 메인서버의 IP

	BOOL Init(CWnd* parent, NMBASE::SOCK::CSockMan* pSockMan);


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVipServiceSockCmnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVipServiceSockCmnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CVipServiceSockCmnd)
		// NOTE - the ClassWizard will add and remove member functions here.
// 	afx_msg LONG OnServAccept(WPARAM, LONG);
// 	afx_msg LONG OnServClose(WPARAM, LONG);
	afx_msg LONG OnDataReceive(WPARAM, LONG);
	afx_msg LONG OnDataClose(WPARAM, LONG);
	afx_msg LONG OnDataConnect(WPARAM, LONG);
	afx_msg LONG OnDataConnectFailure(WPARAM, LONG);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIPSERVICESOCKCMND_H__3D8AF350_AADB_4D59_8DDF_ECBA788F548E__INCLUDED_)
