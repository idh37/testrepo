// WebCallThread.h: interface for the CWebCallThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEBCALLTHREAD_H__B06E74CE_124B_44C3_A235_C06EAAB68052__INCLUDED_)
#define AFX_WEBCALLTHREAD_H__B06E74CE_124B_44C3_A235_C06EAAB68052__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct sMemoData
{
	bool m_bSuccess;
	std::string m_sCode;
	std::string m_sMessage;
	int m_nTotalCount;
};

class UPG_COMMON_EXPORT CWebCallThread  
{
public:

	CWinThread*	m_pThread;			// 쓰레드 포인터	
	BOOL		m_bThreadWorking;
	BOOL		m_bBusy;
	BOOL		m_bResult;
	std::string m_sAddress;
	int m_nPort;
	std::string m_sPost;
	std::string m_sPost1;
	std::string m_sPost2;
	std::string m_sJson;
	std::string m_sReturn;
	std::string m_sReturn1;
	std::string m_sReturn2;
	bool m_bHttps;
	bool m_bPopup;
	bool m_bMemo;
public:
	sMemoData m_MemoData;
	sMemoData m_BackupMemoData;
public:
	CWebCallThread();
	~CWebCallThread();
	BOOL IsWebCall(void){ return m_bThreadWorking; }
public:
	void		ClearMember();
	void		Destroy();
	BOOL		CreateThread(std::string sAddress, int nPort, std::string sPost, std::string sJson, bool bPostOrGet, bool bHttps=false);
	BOOL		TerminateThread();
	static UINT WebCallThreadFunc_Post(LPVOID pParam);								// DownLoad Thread Function
	static UINT WebCallThreadFunc_Get(LPVOID pParam);								// DownLoad Thread Function
	UINT		DoThreadLoop_Post();
	UINT		DoThreadLoop_Get();
public:
	bool MakeMemoData();
};

#endif // !defined(AFX_DOWNTHREAD_H__B06E74CE_124B_44C3_A235_C06EAAB68052__INCLUDED_)
