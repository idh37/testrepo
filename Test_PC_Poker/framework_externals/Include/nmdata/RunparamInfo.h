
/**    
@file    RunparamInfo.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Runparam 정보
*/

#pragma once

#include "DataDefine.h"

namespace NMBASE
{
	namespace DATA
	{

		class NMDATA_CLASS CRunparamInfo
		{
		public:
			CRunparamInfo();
			virtual ~CRunparamInfo();

		public:

			//////////////////////////////////////////////////////////////////////////
			// RunParam
			LPCTSTR GetServerIP(){return m_stServerIP;}
			void    SetServerIP(LPCTSTR strIP){m_stServerIP = strIP;}

			UINT	GetServerPort(){return m_unServerPort;}
			void	SetServerPort(UINT nPort){m_unServerPort = nPort;}

			LPCTSTR GetAuthCookie(){return m_stAuthCookie;}
			void    SetAuthCookie(LPCTSTR strAuthCookie){m_stAuthCookie = strAuthCookie;}

			LPCTSTR GetDataCookie(){return m_stDataCookie;}
			void    SetDataCookie(LPCTSTR strDataCookie){m_stDataCookie = strDataCookie;}

			LPCTSTR GetCpCookie(){return m_stCpCookie;}
			void    SetCpCookie(LPCTSTR strCpCookie){m_stCpCookie = strCpCookie;}

			LPCTSTR GetSpareParam(){return m_stSpareParam;}
			void    SetSpareParam(LPCTSTR strSpareParam){m_stSpareParam = strSpareParam;}

			int		GetLoginGroup(){return m_nLoginGroup;}
			void    SetLoginGroup(int nLoginGroup){m_nLoginGroup = nLoginGroup;}

			int		GetLoginGameCode(){return m_nLoginGameCode;}
			void    SetLoginGameCode(int nLoginGameCode){m_nLoginGameCode = nLoginGameCode;}

			LPCTSTR GetLoginPass(){return m_stLoginPass;}
			void    SetLoginPass(LPCTSTR strLoginPass){m_stLoginPass = strLoginPass;}

			LPCTSTR GetLoginID(){return m_stLoginID;}
			void    SetLoginID(LPCTSTR strLoginID){m_stLoginID = strLoginID;}

			LPCTSTR GetMyUniqNo(){return m_stMyUniqNo;}
			void    SetMyUniqNo(LPCTSTR strUniqNo){m_stMyUniqNo = strUniqNo;}

		private:
			// RunParam 값
			CString m_stServerIP;		///< 마스터 서버의 IP
			UINT	m_unServerPort;		///< 디버그용 마스터 서버의 포트번호
			CString	m_stAuthCookie;		///< 인증 쿠키						[SSO 작업]
			CString	m_stDataCookie;		///< 데이터 쿠키					[SSO 작업]
			CString	m_stCpCookie;		///< CP쿠키							[SSO 작업]
			CString	m_stSpareParam;		///< 여분의 인자(CP게임등에서 사용)	[SSO 작업]
			int		m_nLoginGroup;		///< 로그인시 요청한 그룹
			int		m_nLoginGameCode;	///< 로그인시 요청한 게임코드	
			CString m_stLoginID;		///< 자동 로그인 ID
			CString	m_stLoginPass;		///< 자동 로그인 패스워드
			CString m_stMyUniqNo;		///< 엑스트라 인자(유니크 넘버 - 어디에 사용하는지는 모름)
		};

	}//namespace DATA

}//namespace NMBASE
