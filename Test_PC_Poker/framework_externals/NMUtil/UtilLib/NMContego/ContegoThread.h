#pragma once

#include "ThreadQueue.h"

typedef struct _tagCONTEGODATA{
	DWORD dwErrCode;
	DWORD dwDetectCode;
	char szImage[256];
} CONTEGODATA;

class CContegoThread
{
public:
	CContegoThread(void);
	virtual ~CContegoThread(void);

public:			
	void Initilize( int nGameCode, char* szServer, char* szKeyObject, char* szObject );
	int		m_nGameCode;
	CString m_strServer;
	CString m_strKeyObject;
	CString m_strObject;

	void AddContegoData(CONTEGODATA* pstData);

public:
	static CContegoThread& GetInstance();
	void LoaderThread_Entry();
	void Destroy();

private:
	int SendContegoData(CONTEGODATA* pstData);
	bool BeginManager();
	void RecvSecurityKey(HANDLE);

protected:
	NMBASE::UTIL::CThreadQueue<CONTEGODATA>	m_ContegoQueue;
	CRITICAL_SECTION			m_Tasks_cs;
	unsigned long				m_nContegoThreadID;
	HANDLE						m_hContegoThread;
	BOOL						m_bExit;
	TCHAR						m_szSecurityKey[256];
};


CContegoThread& gContegoThread();