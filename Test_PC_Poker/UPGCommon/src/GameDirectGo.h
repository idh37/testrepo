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
// 	// 해당 게임을 실행시킬수 있는지 확인
// 	// ERROR_GAMEDIRECTGO 값이 ERROR_GAMEDIRECTGO_SUCCESS일 경우 가능
// 	ERROR_GAMEDIRECTGO	SetGameCodeDestination(LPCTSTR szGameCode_To);
// 
// 	// 해당 게임코드의 게임을 실행
// 	// ERROR_GAMEDIRECTGO 값이 ERROR_GAMEDIRECTGO_SUCCESS일 경우 정상적으로 실행
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

// 초기화 함수
// 이미 초기화가 되어있을 경우, ERROR_GAMEDIRECTGO_ALREADY_INIT을 반환
// 성공적으로 초기화되면 ERROR_GAMEDIRECTGO_SUCCESS를 반환
// szCheckURL은 옵션사항. 변경이 되어야할 경우에만 입력
// UPG_COMMON_EXPORT ERROR_GAMEDIRECTGO NMGoGameDirect_Init(HANDLE* hMutex, LPCTSTR szGameCode_This, LPCTSTR szCheckURL = NULL);
// 
// 이동할 게임코드를 입력. 이동할수 있는 게임이면 ERROR_GAMEDIRECTGO_STATUS를 반환
// 이동못하는 경우는 ERROR_GAMEDIRECTGO_STATUS 관련 오류를 참조, 내부오류일 경우 다른 오류코드 반환
// UPG_COMMON_EXPORT ERROR_GAMEDIRECTGO NMGoGameDirect_SetDestination(LPCTSTR szGameCode_To, CString& strErrorMsg_Out);
// 
// 게임이 종료될때 호출하는 함수
// 반드시 UnInitSSOWebBrowser()가 실행되기전에 실행되어야 함.
// 이동할 게임코드가 선택되어져 있으면, 자동으로 게임을 실행.
// UPG_COMMON_EXPORT ERROR_GAMEDIRECTGO NMGoGameDirect_OnExitGame(DWORD dwDelayMillisecond = 0);

#endif // !defined(AFX_GAMEDIRECTGO_H__D27B2F19_9ADE_41DB_8211_0FB2812B78AA__INCLUDED_)
