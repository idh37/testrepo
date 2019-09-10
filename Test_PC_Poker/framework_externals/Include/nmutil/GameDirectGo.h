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
			ERROR_GAMEDIRECTGO_INVALIDPARAM,			// ���ڰ� �߸��Ǿ���
			ERROR_GAMEDIRECTGO_NOT_INIT,
			ERROR_GAMEDIRECTGO_ALREADY_INIT,
			ERROR_GAMEDIRECTGO_NOT_SET_DEST,
			// ��Ű ���� ����
			ERROR_GAMEDIRECTGO_COOKIE = 1000,		
			ERROR_GAMEDIRECTGO_SSOCOOKIE_CANTGET,		// SSO WebLib���� ��Ű������ �������� ����.
			ERROR_GAMEDIRECTGO_SSOCOOKIE_MISSING,		// SSO WebLib���� ������Ű�� �������
			// HTTP ���� ����
			ERROR_GAMEDIRECTGO_HTTP = 2000,			
			ERROR_GAMEDIRECTGO_HTTP_CANTPARSE,			// HTTP URL Parsing�� �Ҽ� ����
			ERROR_GAMEDIRECTGO_HTTP_CANTCONNECT,		// �ش� URL ������ ������ �Ұ���
			ERROR_GAMEDIRECTGO_HTTP_CANTSETCOOKIE,		// ��Ű������ �����Ҽ� ����
			ERROR_GAMEDIRECTGO_HTTP_CANTOPENFILE,		// �ش� URL ������ ���� ����
			ERROR_GAMEDIRECTGO_HTTP_CANTREQUEST,		// �ش� ������ ��û�� ������ ����
			ERROR_GAMEDIRECTGO_HTTP_UNKNWONSTATUSCODE,	// Http Status Code�� 200�� �ƴ�
			ERROR_GAMEDIRECTGO_HTTP_CANTREAD,			// Http File�� Read�Ҽ� ����
			// Clipboard ���� ���� ����
			ERROR_GAMEDIRECTGO_CBDATA = 3000,
			ERROR_GAMEDIRECTGO_CBDATA_CANTWRITE,		// Clipboard�� �����͸� ���� ����
			// ���� ���� ���� ���� ����
			ERROR_GAMEDIRECTGO_EXECFILE = 4000,
			ERROR_GAMEDIRECTGO_EXECFILE_NOTFOUND,		// ���� ������ ã�� �� ����
			ERROR_GAMEDIRECTGO_EXECFILE_NOTEXCUTE,		// ���� ������ ������� ����.
			// ���� ���� Status
			ERROR_GAMEDIRECTGO_STATUS = 5000,
			ERROR_GAMEDIRECTGO_STATUS_UNKNOWN,			// ���� ���� ����
			ERROR_GAMEDIRECTGO_STATUS_DISABLE,			// ������������ ����
			ERROR_GAMEDIRECTGO_STATUS_NOMONEY,			// ���尡�� ������ ���� ����
			ERROR_GAMEDIRECTGO_STATUS_NOTLOGIN,			// ���̵� ������ ã�� �� ����	
			ERROR_GAMEDIRECTGO_STATUS_NOTADULT,			// ���������� �ʿ��Ѱ���.
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
			// �ش� ������ �����ų�� �ִ��� Ȯ��
			// ERROR_GAMEDIRECTGO ���� ERROR_GAMEDIRECTGO_SUCCESS�� ��� ����
			ERROR_GAMEDIRECTGO	SetGameCodeDestination(LPCTSTR szGameCode_To);

			// �ش� �����ڵ��� ������ ����
			// ERROR_GAMEDIRECTGO ���� ERROR_GAMEDIRECTGO_SUCCESS�� ��� ���������� ����
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

		// �ʱ�ȭ �Լ�
		// �̹� �ʱ�ȭ�� �Ǿ����� ���, ERROR_GAMEDIRECTGO_ALREADY_INIT�� ��ȯ
		// ���������� �ʱ�ȭ�Ǹ� ERROR_GAMEDIRECTGO_SUCCESS�� ��ȯ
		// szCheckURL�� �ɼǻ���. ������ �Ǿ���� ��쿡�� �Է�
		NMUTIL_CLASS ERROR_GAMEDIRECTGO NMGoGameDirect_Init(HANDLE* hMutex, LPCTSTR szGameCode_This, LPCTSTR szCheckURL = NULL);

		// �̵��� �����ڵ带 �Է�. �̵��Ҽ� �ִ� �����̸� ERROR_GAMEDIRECTGO_STATUS�� ��ȯ
		// �̵����ϴ� ���� ERROR_GAMEDIRECTGO_STATUS ���� ������ ����, ���ο����� ��� �ٸ� �����ڵ� ��ȯ
		NMUTIL_CLASS ERROR_GAMEDIRECTGO NMGoGameDirect_SetDestination(LPCTSTR szGameCode_To, CString& strErrorMsg_Out);

		// ������ ����ɶ� ȣ���ϴ� �Լ�
		// �ݵ�� UnInitSSOWebBrowser()�� ����Ǳ����� ����Ǿ�� ��.
		// �̵��� �����ڵ尡 ���õǾ��� ������, �ڵ����� ������ ����.
		NMUTIL_CLASS ERROR_GAMEDIRECTGO NMGoGameDirect_OnExitGame(DWORD dwDelayMillisecond = 0);

	}//namespace UTIL

}//namespace NMBASE


#endif // !defined(AFX_GAMEDIRECTGO_H__D27B2F19_9ADE_41DB_8211_0FB2812B78AA__INCLUDED_)
