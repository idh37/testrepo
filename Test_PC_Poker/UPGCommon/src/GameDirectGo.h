// GameDirectGo.h: interface for the CGameDirectGo class.
// 
// Creator			: Heo Jaemin <judge@cj.net>
// Create Date		: 2007/11/26
// LastModify Date	: 2007/11/26 ( 0 Modified )
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEDIRECTGO_H__D27B2F19_9ADE_41DB_8211_0FB2812B78AA__INCLUDED_)
#define AFX_GAMEDIRECTGO_H__D27B2F19_9ADE_41DB_8211_0FB2812B78AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxinet.h> 


class UPG_COMMON_EXPORT CGameDirectGo : public NMBASE::UTIL::CGameDirectGoBase
{
public:
	CGameDirectGo();
	virtual ~CGameDirectGo();

//	BOOL	IsInit();

// 	ERROR_GAMEDIRECTGO	Init(LPCTSTR szGameCode_This, LPCTSTR szCheckURL, HANDLE* hMutex);
// 	// �ش� ������ �����ų�� �ִ��� Ȯ��
// 	// ERROR_GAMEDIRECTGO ���� ERROR_GAMEDIRECTGO_SUCCESS�� ��� ����
// 	ERROR_GAMEDIRECTGO	SetGameCodeDestination(LPCTSTR szGameCode_To);
// 
// 	// �ش� �����ڵ��� ������ ����
// 	// ERROR_GAMEDIRECTGO ���� ERROR_GAMEDIRECTGO_SUCCESS�� ��� ���������� ����
// 	ERROR_GAMEDIRECTGO	OnGameExit(DWORD dwDelayMillisecond = 0);

protected:
//	ERROR_GAMEDIRECTGO	ExistExcuteFile();
//	virtual ERROR_GAMEDIRECTGO	GetExcuteFile(CString& szExcuteFileName_Out);
//	ERROR_GAMEDIRECTGO	SetSSOCookie(CHttpFile* pFile, LPCTSTR szURL_In);
//	ERROR_GAMEDIRECTGO	GetSSOCookie(CString& strCookie, CString& strDataCookie, CString& strCpCookie);
//	ERROR_GAMEDIRECTGO	CheckHTMLData(LPCTSTR szURL);

// 	CString		m_strCheckURL;
// 	CString		m_strGameCode_This;
// 	CString		m_strGameCode_Dest;
// 	HANDLE*		m_pMutex;
// 	BOOL		m_bExcuteGame;
};

///////////////////////////////////////////////////
// Global Function

// �ʱ�ȭ �Լ�
// �̹� �ʱ�ȭ�� �Ǿ����� ���, ERROR_GAMEDIRECTGO_ALREADY_INIT�� ��ȯ
// ���������� �ʱ�ȭ�Ǹ� ERROR_GAMEDIRECTGO_SUCCESS�� ��ȯ
// szCheckURL�� �ɼǻ���. ������ �Ǿ���� ��쿡�� �Է�
// UPG_COMMON_EXPORT ERROR_GAMEDIRECTGO NMGoGameDirect_Init(HANDLE* hMutex, LPCTSTR szGameCode_This, LPCTSTR szCheckURL = NULL);
// 
// �̵��� �����ڵ带 �Է�. �̵��Ҽ� �ִ� �����̸� ERROR_GAMEDIRECTGO_STATUS�� ��ȯ
// �̵����ϴ� ���� ERROR_GAMEDIRECTGO_STATUS ���� ������ ����, ���ο����� ��� �ٸ� �����ڵ� ��ȯ
// UPG_COMMON_EXPORT ERROR_GAMEDIRECTGO NMGoGameDirect_SetDestination(LPCTSTR szGameCode_To, CString& strErrorMsg_Out);
// 
// ������ ����ɶ� ȣ���ϴ� �Լ�
// �ݵ�� UnInitSSOWebBrowser()�� ����Ǳ����� ����Ǿ�� ��.
// �̵��� �����ڵ尡 ���õǾ��� ������, �ڵ����� ������ ����.
// UPG_COMMON_EXPORT ERROR_GAMEDIRECTGO NMGoGameDirect_OnExitGame(DWORD dwDelayMillisecond = 0);

#endif // !defined(AFX_GAMEDIRECTGO_H__D27B2F19_9ADE_41DB_8211_0FB2812B78AA__INCLUDED_)
