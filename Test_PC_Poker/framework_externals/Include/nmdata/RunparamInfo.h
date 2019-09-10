
/**    
@file    RunparamInfo.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Runparam ����
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
			// RunParam ��
			CString m_stServerIP;		///< ������ ������ IP
			UINT	m_unServerPort;		///< ����׿� ������ ������ ��Ʈ��ȣ
			CString	m_stAuthCookie;		///< ���� ��Ű						[SSO �۾�]
			CString	m_stDataCookie;		///< ������ ��Ű					[SSO �۾�]
			CString	m_stCpCookie;		///< CP��Ű							[SSO �۾�]
			CString	m_stSpareParam;		///< ������ ����(CP���ӵ�� ���)	[SSO �۾�]
			int		m_nLoginGroup;		///< �α��ν� ��û�� �׷�
			int		m_nLoginGameCode;	///< �α��ν� ��û�� �����ڵ�	
			CString m_stLoginID;		///< �ڵ� �α��� ID
			CString	m_stLoginPass;		///< �ڵ� �α��� �н�����
			CString m_stMyUniqNo;		///< ����Ʈ�� ����(����ũ �ѹ� - ��� ����ϴ����� ��)
		};

	}//namespace DATA

}//namespace NMBASE
