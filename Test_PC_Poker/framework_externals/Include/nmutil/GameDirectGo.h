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

#include "UtilDefine.h"
#include <afxinet.h> 

namespace NMBASE
{
	namespace UTIL
	{

		enum ERROR_GAMEDIRECTGO
		{
			ERROR_GAMEDIRECTGO_SUCCESS = 0,
			ERROR_GAMEDIRECTGO_INVALIDPARAM,			// 인자가 잘못되었음
			ERROR_GAMEDIRECTGO_NOT_INIT,
			ERROR_GAMEDIRECTGO_ALREADY_INIT,
			ERROR_GAMEDIRECTGO_NOT_SET_DEST,
			// 쿠키 관련 오류
			ERROR_GAMEDIRECTGO_COOKIE = 1000,		
			ERROR_GAMEDIRECTGO_SSOCOOKIE_CANTGET,		// SSO WebLib에서 쿠키정보를 가져오지 못함.
			ERROR_GAMEDIRECTGO_SSOCOOKIE_MISSING,		// SSO WebLib에서 인증쿠키가 비어있음
			// HTTP 과련 오류
			ERROR_GAMEDIRECTGO_HTTP = 2000,			
			ERROR_GAMEDIRECTGO_HTTP_CANTPARSE,			// HTTP URL Parsing을 할수 없음
			ERROR_GAMEDIRECTGO_HTTP_CANTCONNECT,		// 해당 URL 서버로 접근이 불가능
			ERROR_GAMEDIRECTGO_HTTP_CANTSETCOOKIE,		// 쿠키정보를 세팅할수 없음
			ERROR_GAMEDIRECTGO_HTTP_CANTOPENFILE,		// 해당 URL 파일을 열수 없음
			ERROR_GAMEDIRECTGO_HTTP_CANTREQUEST,		// 해당 서버로 요청을 보낼수 없음
			ERROR_GAMEDIRECTGO_HTTP_UNKNWONSTATUSCODE,	// Http Status Code가 200이 아님
			ERROR_GAMEDIRECTGO_HTTP_CANTREAD,			// Http File을 Read할수 없음
			// Clipboard 전달 관련 오류
			ERROR_GAMEDIRECTGO_CBDATA = 3000,
			ERROR_GAMEDIRECTGO_CBDATA_CANTWRITE,		// Clipboard로 데이터를 쓸수 없음
			// 게임 실행 파일 관련 오류
			ERROR_GAMEDIRECTGO_EXECFILE = 4000,
			ERROR_GAMEDIRECTGO_EXECFILE_NOTFOUND,		// 실행 파일을 찾을 수 없음
			ERROR_GAMEDIRECTGO_EXECFILE_NOTEXCUTE,		// 실행 파일이 실행되지 않음.
			// 게임 실행 Status
			ERROR_GAMEDIRECTGO_STATUS = 5000,
			ERROR_GAMEDIRECTGO_STATUS_UNKNOWN,			// 게임 실행 가능
			ERROR_GAMEDIRECTGO_STATUS_DISABLE,			// 정기점검중인 게임
			ERROR_GAMEDIRECTGO_STATUS_NOMONEY,			// 입장가능 조건이 되지 않음
			ERROR_GAMEDIRECTGO_STATUS_NOTLOGIN,			// 아이디 정보를 찾을 수 없음	
			ERROR_GAMEDIRECTGO_STATUS_NOTADULT,			// 성인인증이 필요한게임.
		};

		#define NMDIRECTGAMEGO_AUTHCOOKIE	"AuthCookie"
		#define NMDIRECTGAMEGO_DATACOOKIE	"DataCookie"
		#define NMDIRECTGAMEGO_CPCOOKIE		"CpCookie"
		#define NMDIRECTGAMEGO_FROM			"GameCodeFrom"
		#define NMDIRECTGAMEGO_TO			"GameCodeTo"

		#define BUFFER_COOKIEDATA	1024

		class NMUTIL_CLASS CGameDirectGo  
		{
		public:
			CGameDirectGo();
			virtual ~CGameDirectGo();

			BOOL	IsInit();

			ERROR_GAMEDIRECTGO	Init(LPCTSTR szGameCode_This, LPCTSTR szCheckURL, HANDLE* hMutex);
			// 해당 게임을 실행시킬수 있는지 확인
			// ERROR_GAMEDIRECTGO 값이 ERROR_GAMEDIRECTGO_SUCCESS일 경우 가능
			ERROR_GAMEDIRECTGO	SetGameCodeDestination(LPCTSTR szGameCode_To);

			// 해당 게임코드의 게임을 실행
			// ERROR_GAMEDIRECTGO 값이 ERROR_GAMEDIRECTGO_SUCCESS일 경우 정상적으로 실행
			ERROR_GAMEDIRECTGO	OnGameExit(DWORD dwDelayMillisecond = 0);

		protected:
			ERROR_GAMEDIRECTGO	ExistExcuteFile();
			ERROR_GAMEDIRECTGO	GetExcuteFile(CString& szExcuteFileName_Out);
			ERROR_GAMEDIRECTGO	SetSSOCookie(CHttpFile* pFile, LPCTSTR szURL_In);
			ERROR_GAMEDIRECTGO	GetSSOCookie(CString& strCookie, CString& strDataCookie, CString& strCpCookie);
			ERROR_GAMEDIRECTGO	CheckHTMLData(LPCTSTR szURL);

			CString		m_strCheckURL;
			CString		m_strGameCode_This;
			CString		m_strGameCode_Dest;
			HANDLE*		m_pMutex;
			BOOL		m_bExcuteGame;
		};

		///////////////////////////////////////////////////
		// Global Function

		// 초기화 함수
		// 이미 초기화가 되어있을 경우, ERROR_GAMEDIRECTGO_ALREADY_INIT을 반환
		// 성공적으로 초기화되면 ERROR_GAMEDIRECTGO_SUCCESS를 반환
		// szCheckURL은 옵션사항. 변경이 되어야할 경우에만 입력
		NMUTIL_CLASS ERROR_GAMEDIRECTGO NMGoGameDirect_Init(HANDLE* hMutex, LPCTSTR szGameCode_This, LPCTSTR szCheckURL = NULL);

		// 이동할 게임코드를 입력. 이동할수 있는 게임이면 ERROR_GAMEDIRECTGO_STATUS를 반환
		// 이동못하는 경우는 ERROR_GAMEDIRECTGO_STATUS 관련 오류를 참조, 내부오류일 경우 다른 오류코드 반환
		NMUTIL_CLASS ERROR_GAMEDIRECTGO NMGoGameDirect_SetDestination(LPCTSTR szGameCode_To, CString& strErrorMsg_Out);

		// 게임이 종료될때 호출하는 함수
		// 반드시 UnInitSSOWebBrowser()가 실행되기전에 실행되어야 함.
		// 이동할 게임코드가 선택되어져 있으면, 자동으로 게임을 실행.
		NMUTIL_CLASS ERROR_GAMEDIRECTGO NMGoGameDirect_OnExitGame(DWORD dwDelayMillisecond = 0);

	}//namespace UTIL

}//namespace NMBASE


#endif // !defined(AFX_GAMEDIRECTGO_H__D27B2F19_9ADE_41DB_8211_0FB2812B78AA__INCLUDED_)
